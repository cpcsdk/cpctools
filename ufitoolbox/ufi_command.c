/*
    ufiformat Version 0.9.4 2007/11/09

    Copyright (C) 2005-2007 Kazuhiro Hayashi <tedi@tedi.sakura.ne.jp>
    Copyright (C) 2005 John Floyd <jfloyd@bigpond.net.au>

    The method of formatting a floppy on USB-FDD used in this program
    is introduced by Bruce M Simpson.

    This program is free software; you can redistribute it and/or
    modify it under the terms of the GNU General Public License
    as published by the Free Software Foundation; either version 2
    of the License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/

#ifdef HAVE_CONFIG_H
#  include "config.h"
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <scsi/scsi.h>
#include <scsi/sg.h>
#include <errno.h>
#include "ufi_command.h"

#define TIME_OUT (100 * 1000)	/* milliseconds */
#define LUN 0

static const unsigned char FORMAT_UNIT_CMD[] = 
{
    4,		/*Opcode							*/
	LUN << 5 + 0x17,
	0, 		/* Track number						*/ 
	0,0, 	/* Interleave (0=default, (1=1:1)	*/ 
	0,0,	/* Reserved 						*/
	0, 12, 	/* Parameter list length			*/
	0, 0, 0 /* Reserved 						*/
};

/* Paramètres : */

/* DEFECT LIST HEADER */
/*
 *	Reserved
 *	%10100000 + ST << 5 + SD //(ST = Single track / SD = Side)
 *	0
 *	8
 */

/* FORMAT DESCRIPTOR */
/*
 * Doit être l'un des formats retournés par le lecteur quand on demande ce qu'il sait faire
 * 0,0,5,160 //Nombre de secteurs (80*9*2=1440 pour du 720k, mais on va surement tricher pour formater en faisant plusieurs tours et essayer d'avoir des numéros de secteurs amsdos ?). Pour du single track, mettre 9...
 * 0
 * 0,2,0 //Taille d'un secteur (512 octets)
 */

static const unsigned char INQUIRY_CMD[] = {
    0x12, // Opcode
	LUN << 5,
	0x00, 0x00, 
	0x00, // Allocation length (taille max de la réponse 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

/* Réponse de cette commande : */
/*
 * Peripheral Device Type = 0 si disquette, 1F si rien (pas de lecteur connecté à l'UFI)
 * RMB, 0 si disque fixe, 128 si éjectable
 * 0
 * Longueur des réponses supplémentaires : 1F
 * 0,0,0
 * VENDOR (String, 8 caractères)
 * PRODUCT (String, 8 char)
 * VERSION (String, 4 char)
*/

static const unsigned char START_STOP_CMD[] = {
    0x1B, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x40, 0x00, 0x00, 0x00
};

static const unsigned char MODE_SELECT_CMD[] = {
    0x55, 
	0x10 + (LUN << 5),
	0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x40, //Parameter list length
	0x00, 0x00, 0x00
};

#define PC 0
#define PageCode 0x3F

static const unsigned char MODE_SENSE_CMD[] = {
    0x5A, 
	LUN << 5, 
	PC+PageCode, 
		// PC : 0 pour actuel, 64 pour masque des valeurs modifiables,
		// 128 pour défaut
		// Page code : 3F pour tout,
		// 1 > RW ERR recovery msg
		// 5 > Flexible disk
		// 1B Removeable block access capacities
		// 1C > Timer and protect
	0x00, 0x00, 0x00, 0x00, 
	0x00, 0x40, //Longueur params (longueur max réponse)
	0x00, 0x00, 0x00
};

//Réponse
//HEADER commun à toutes les pages
/*
 * 0,0 // Taille des données (pour le SENSE, pas pour le SET)
 * 1E  // Type de media : 1E = 720k, 93 = 1.25M, 94 = 1.44M. Peut-être d'autres selon le lecteur ...
 * 		// Pour select : 0 = pas de changement, garder actuel
 *		// Pour faire un format perso, utiliser la page "flexible disk", plus souple (mais ce truc marche pour les formats par défaut)
 * WP  // 128 si protégé en écriture. Ignoré pour SELECT
 */

// PAGE 1 : RW Error recovery
/* 1 // Page code
 * 4 = signale les erreurs qu'on a réussi à récupérer (le lecteur fait plusieurs essais)
 * 0 //Nombre de tentatives max en cas d'erreur lecture
 * 0,0,0,0
 * 0 // Idem pour écriture
 * 0,0,0
 */

// Page 5 : Flexible Disk
/*
 * 5 // Page code
 * 0x1E //Longueur de la page
 * 0,0 // Transfer rate : FA =250k, 12C = 300k, 1F4 = 500k, 3E8 = 1M, 7D0 = 2M, 1388=(M (bit/s)
 * 2 //Nombre de têtes (a priori pas possible de faire du simple face !)
 * 0,9 //Secteurs par piste (de 1 à 63) > Peut permettre de lire des secteurs vec des id bizarres comme les fait amsdos ?
 * 0,80 // Nombre de pistes. A priori pas de souci pour mettre 40 ? voir ac mode sense si c'est modifiable
 * 0
 * 5 //Motor on delay. Pas modifiable
 * 30 // Motor off delay (1/10 de secondes), pas modifiable
 * 0
 * 256,(44 ou 104) //Medium rotation rate (300 ou 360) (en rpm)
 * 0,0
 */

// Page 1B : Removable Block Access Capabilities
/* 0x1B // page code
 * 0x0A // page length
 * 128 	// si peut être utilisé comme disquette système
 * 1 	// Nombre de LUN supportés
 * 0,0,0,0,0,0,0,0
 */

// Page 1C : Timer and protect
/* 0x1C // Page code
 * 0x06 // Taille page
 * 0
 * 5 //Durée de la ause après une opération :
 * 		//0 = infini
 *		//1 = 125ms, *2 jusqu'a 15=32 minutes
 *		//Fixé à 5, non modifiable sur USBFDU (2 secondes)
 * 0,0,0,0
 */

static const unsigned char ALLOW_MEDIA_REMOVAL_CMD[] = {
    0x1E, 
	LUN << 5, 
	0, 0, 
	0, // 1 pour interdire l'éjection (sur un lecteur autoeject)  
	0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00
};

static const unsigned char READ_10_CMD[] = {
    0x28, 
	LUN << 5, 
	0x05, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x40, 0x00, 0x00, 0x00
};

static const unsigned char READ_12_CMD[] = {
    0xA8, 0x00, 0x05, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x40, 0x00, 0x00, 0x00
};

static const unsigned char READ_CAPACITY_CMD[] = {
    0x25, 0x00, 0x05, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x40, 0x00, 0x00, 0x00
};

static const unsigned char READ_FORMAT_CAPACITIES_CMD[] = {
    0x23, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x40, 0x00, 0x00, 0x00
};

static const unsigned char REQUEST_SENSE_CMD[] = {
    0x03, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x40, 0x00, 0x00, 0x00
};

static const unsigned char REZERO_UNIT_CMD[] = {
    0x01, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x40, 0x00, 0x00, 0x00
};

static const unsigned char SEEK_CMD[] = {
    0x2B, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x40, 0x00, 0x00, 0x00
};

static const unsigned char SEND_DIAGNOSTIC_CMD[] = {
    0x1B, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x40, 0x00, 0x00, 0x00
};

static const unsigned char TEST_UNIT_READY_CMD[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

static const unsigned char VERIFY_CMD[] = {
    0x2F, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x40, 0x00, 0x00, 0x00
};

static const unsigned char WRITE_10_CMD[] = {
    0x2A, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x40, 0x00, 0x00, 0x00
};


static const unsigned char WRITE_12_CMD[] = {
    0xAA, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x40, 0x00, 0x00, 0x00
};

static const unsigned char WRITE_AND_VERIFY_CMD[] = {
    0x2E, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x40, 0x00, 0x00, 0x00
};



//??
static const unsigned char FORMAT_UNIT_DATA[] = {
    0x00, 0xB0, 0x00, 0x08, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

static int verbose = 0;

void ufi_set_verbose(int v)
{
    verbose = v;
}

static int ufi_invoke(int fd, const char *cmd, size_t cmd_size, char *data, size_t data_size, int direction)
{
    sg_io_hdr_t sg_io_hdr;
    unsigned char sense_buffer[32];

    memset(&sg_io_hdr, 0, sizeof(sg_io_hdr));
    sg_io_hdr.interface_id = 'S';
    sg_io_hdr.cmdp = (char *)cmd;
    sg_io_hdr.cmd_len = cmd_size;
    sg_io_hdr.dxfer_direction = direction;
    sg_io_hdr.dxferp = data;
    sg_io_hdr.dxfer_len = data_size;
    sg_io_hdr.sbp = sense_buffer;
    sg_io_hdr.mx_sb_len = sizeof(sense_buffer);
    sg_io_hdr.timeout = TIME_OUT;

    if (ioctl(fd, SG_IO, &sg_io_hdr) < 0) {
        return UFI_ERROR;
    }
    if (cmd[0] == TEST_UNIT_READY_CMD[0]) {
	if (sg_io_hdr.masked_status == CHECK_CONDITION &&
	    (sense_buffer[2] & 0xf) == 0x6 && sense_buffer[12] == 0x28 && sense_buffer[13] == 0x00) {
	    /* media change */
	    if (ioctl(fd, SG_IO, &sg_io_hdr) < 0) {
		return UFI_ERROR;
	    }
	}
	if (sg_io_hdr.masked_status == CHECK_CONDITION &&
	    (sense_buffer[2] & 0xf) == 0x3 && sense_buffer[12] == 0x30 && sense_buffer[13] == 0x01) {
	    /* unformatted media */
	    return UFI_UNFORMATTED_MEDIA;
	}
	if (sg_io_hdr.masked_status == CHECK_CONDITION &&
	    (sense_buffer[2] & 0xf) == 0x2 && sense_buffer[12] == 0x3a && sense_buffer[13] == 0x00) {
	    /* no media */
	    return UFI_NO_MEDIA;
	}
    }
    if (sg_io_hdr.masked_status != GOOD) {
	if (verbose) {
	    int i;
	    fprintf(stderr, "SCSI error(command=%02x, status=%02x)\n", *cmd, sg_io_hdr.masked_status);
	    for (i = 0; i < sizeof(sense_buffer); i++) {
		printf("%02x ", sense_buffer[i]);
		if (i % 16 == 15) printf("\n");
	    }
	}
	errno = EPROTO;
	return UFI_ERROR;
    }
    return UFI_GOOD;
}

#define ufi_invoke_to(fd, cmd, data) \
  ufi_invoke(fd, cmd, sizeof(cmd), data, sizeof(data), SG_DXFER_TO_DEV)
#define ufi_invoke_from(fd, cmd, data) \
  ufi_invoke(fd, cmd, sizeof(cmd), data, sizeof(data), SG_DXFER_FROM_DEV)
#define ufi_invoke_no(fd, cmd) \
  ufi_invoke(fd, cmd, sizeof(cmd), NULL, 0, SG_DXFER_TO_DEV)

static char *dup_name(const char *src, int offset, int count)
{
    int i;
    char *name;

    for (i = count; i > 0; i--) {
	if (src[offset + i - 1] != ' ') {
	    break;
	}
    }
    name = malloc(i + 1);
    memcpy(name, src + offset, i);
    name[i] = '\0';
    return name;
}

#define TWO_BYTES_TO_INT(ptr) ((*(ptr) << 8) + *((ptr) + 1))
#define THREE_BYTES_TO_INT(ptr) ((TWO_BYTES_TO_INT(ptr) << 8) + *((ptr) + 2))
#define FOUR_BYTES_TO_INT(ptr) ((THREE_BYTES_TO_INT(ptr) << 8) + *((ptr) + 3))

int ufi_test_unit_ready(int fd)
{
    int result;
    result = ufi_invoke_no(fd, TEST_UNIT_READY_CMD);
    return result == UFI_GOOD ? UFI_FORMATTED_MEDIA : result;
}

struct ufi_capacities *ufi_read_format_capacities(int fd, int *type)
{
    unsigned char response[READ_FORMAT_CAPACITIES_CMD[8]];
    struct ufi_capacities *top = NULL;
    struct ufi_capacities *last = NULL;
    struct ufi_capacities *current;
    int i;

    if (ufi_invoke_from(fd, READ_FORMAT_CAPACITIES_CMD, response) != 0) {
	return NULL;
    }
    for (i = 0; i < response[3]; i += 8) {
	current = malloc(sizeof(struct ufi_capacities));
        current->blocks = FOUR_BYTES_TO_INT(response + 4 + i);
        current->block_size = THREE_BYTES_TO_INT(response + 4 + i + 5);
	current->next = NULL;
	if (last != NULL) {
	    last->next = current;
	}
	if (top == NULL) {
	    top = current;
	}
	last = current;
    }
    if (top == NULL) {
	errno = EPROTO;
	return NULL;
    }
    *type = response[4 + 4] & 0x3;
    return top;
}

struct ufi_product *ufi_inquiry(int fd)
{
    unsigned char response[INQUIRY_CMD[8]];
    struct ufi_product *product;

    if (ufi_invoke_from(fd, INQUIRY_CMD, response) != 0) {
	return NULL;
    }
    product = malloc(sizeof(struct ufi_product));
    product->vendor = dup_name(response, 8, 8);
    product->product = dup_name(response, 16, 16);
    return product;
}

int ufi_mode_sense(int fd, int *write_protected)
{
    unsigned char response[MODE_SENSE_CMD[8]];

    if (ufi_invoke_from(fd, MODE_SENSE_CMD, response) != 0) {
	return UFI_ERROR;
    }
    *write_protected = (response[3] & 0x80) ? UFI_PROTECTED : UFI_NOT_PROTECTED;
    return UFI_GOOD;
}

int ufi_format_unit(int fd, int blocks, int block_size, int track, int head)
{
    unsigned char command[sizeof(FORMAT_UNIT_CMD)];
    unsigned char data[sizeof(FORMAT_UNIT_DATA)];

    memcpy(command, FORMAT_UNIT_CMD, sizeof(command));
    memcpy(data, FORMAT_UNIT_DATA, sizeof(data));
    command[2] = track;
    data[1] |= head;
    data[4] = (blocks >> 24) & 0xff;
    data[5] = (blocks >> 16) & 0xff;
    data[6] = (blocks >> 8) & 0xff;
    data[7] = blocks & 0xff;
    data[9] = (block_size >> 16) & 0xff;
    data[10] = (block_size >> 8) & 0xff;
    data[11] = block_size & 0xff;

    if (ufi_invoke_to(fd, command, data) != 0) {
	return UFI_ERROR;
    }
    return UFI_GOOD;
}
