#!/usr/bin/env python
# -*- coding: utf-8 -*-

"""
pyDSK
(c) MML, 2011
"""

import sys
import glob             # glob() expande los patrones de los ficheros en windows
import os               # path.basename(), path.exists()
from optparse import make_option, OptionParser

# Constantes
DSK_ID_EXT = '\x45' + '\x58' + '\x54' + '\x45' + \
        '\x4e' + '\x44' + '\x45' + '\x44' + \
        '\x20' + '\x43' + '\x50' + '\x43' + \
        '\x20' + '\x44' + '\x53' + '\x4b' + \
        '\x20' + '\x46' + '\x69' + '\x6c' + \
        '\x65' + '\x0d' + '\x0a' + '\x44' + \
        '\x69' + '\x73' + '\x6b' + '\x2d' + \
        '\x49' + '\x6e' + '\x66' + '\x6f' + \
        '\x0d' + '\x0a'

DSK_AUTHOR = "pyDSK 1 by SyX"

TRACKS_PER_DISK = 40
DSK_NUM_SIDES = 1

SECTORS_PER_TRACK = 9

DSK_FILL_BYTE = 0xE5

DISK_INFO_BLOCK = DSK_ID_EXT + DSK_AUTHOR + chr(TRACKS_PER_DISK) + chr(DSK_NUM_SIDES) + \
                 chr(0x00) * 2 + chr(0x13) * 40 + chr(0x00) * (4 * 41)

TRACK_ID = '\x54' + '\x72' + '\x61' + '\x63' + \
            '\x6b' + '\x2d' + '\x49' + '\x6e' + \
            '\x66' + '\x6f' + '\x0d' + '\x0a'


LEN_ID_DSK = 34
LEN_DISK_INFO_BLOCK = 0x100
LEN_TRACK_INFO_BLOCK = 0x100
LEN_SECTOR = 512
FIRST_SECTOR_CATALOG = LEN_DISK_INFO_BLOCK + LEN_TRACK_INFO_BLOCK
SECTOR_OFFSETS = [FIRST_SECTOR_CATALOG + LEN_SECTOR * 0, FIRST_SECTOR_CATALOG + LEN_SECTOR * 2, \
                FIRST_SECTOR_CATALOG + LEN_SECTOR * 4, FIRST_SECTOR_CATALOG + LEN_SECTOR * 6, \
                FIRST_SECTOR_CATALOG + LEN_SECTOR * 8, FIRST_SECTOR_CATALOG + LEN_SECTOR * 1, \
                FIRST_SECTOR_CATALOG + LEN_SECTOR * 3, FIRST_SECTOR_CATALOG + LEN_SECTOR * 5, \
                FIRST_SECTOR_CATALOG + LEN_SECTOR * 7]
                
SECTOR_SORT =   [0, 5, 1, 6, 2, 7, 3, 8, 4]
SECTOR_UNSORT = [0, 2, 4, 6, 8, 1, 3, 5, 7]

def crea_track_info_block(num_pista):
    """
    Devuelve una cadea con el Track Info Block correspondiente a la pista
    """
    # Añadimos la información de la pista   # offset    description         bytes
    tib_tmp = TRACK_ID                      # 00 - 0b   "Track-Info\r\n"    12
    tib_tmp += '\x00' * 4                   # 0c - 0f   unused              4
    tib_tmp += chr(num_pista)               # 10        track number        1
    tib_tmp += chr(0x00)                    # 11        side number         1
    tib_tmp += '\x00' * 2                   # 12 - 13   unused              2
    tib_tmp += chr(0x02)                    # 14        sector size         1
    tib_tmp += chr(0x09)                    # 15        number of sectors   1
    tib_tmp += chr(0x52)                    # 16        GAP#3 length        1
    tib_tmp += chr(DSK_FILL_BYTE)           # 17        filler byte         1
    # Añadimos la lista de información de los sectores de dicha pista
    num_sector_tmp = 0xC1
    for i in range(SECTORS_PER_TRACK):
        tib_tmp += chr(num_pista)           # 00        track (C in NEC765)     1
        tib_tmp += chr(0x00)                # 01        side (H in NEC765)      1
        tib_tmp += chr(num_sector_tmp)      # 02        sector ID (R in NEC765) 1
        num_sector_tmp += (+5 if num_sector_tmp < 0xC5 else -4)
        tib_tmp += chr(0x02)                # 03        sector size (N in NEC765) 1
        tib_tmp += chr(0x00)                # 04        FDC status register 1 (ST1 in NEC765) 1
        tib_tmp += chr(0x00)                # 05        FDC status register 2 (ST2 in NEC765) 1
        tib_tmp += chr(0x00) + chr(0x02)    # 06 - 07   actual data length in bytes 2 
    # Añadimos el padding hasta llegar a los 256 bytes que ocupa el TIB
    tib_tmp += chr(0x00) * 16 * 10
    
    return tib_tmp

def trocea(secuencia, longitud):
    """
    Divide una secuencia en trozos del tamaño indicado 
    """
    return [secuencia[i: i + longitud] for i in range(0, len(secuencia), longitud)]

# Procesa la línea de comandos    
def procesar_linea_comandos(linea_de_comandos):
    """
    Devuelve una tupla de dos elementos: (opciones, lista_de_ficheros).
    `linea_de_comandos` es una lista de argumentos, o `None` para ``sys.argv[1:]``.
    """
    if linea_de_comandos is None:
        linea_de_comandos = sys.argv[1:]

    version_programa = "%prog v0.1"
    uso_programa = "usage: %prog [options] file1 file2 ... fileX"
    descripcion_programa = "%prog insert files in sectors of a disk image for the Amstrad CPC."

    # definimos las opciones que soportaremos desde la línea de comandos
    lista_de_opciones = [
        make_option("-f", "--format", action="store_true", dest="format_disk", default=False, help="Format the disk image"),
#        make_option("-l", "--disk_loader", action="store", type="string", dest="disk_loader", default="", help="The file with the disk loader"),
        make_option("-d", "--disk", action="store", type="string", dest="disk_filename", default="disco.dsk", help="The file with the disk image"),
        make_option("-s", "--sector", action="store", type="int", dest="disk_sector", default=4, help="Sector where begin to insert the files")
#        make_option("-r", "--mode", action="store", type="int", dest="mode", default=1, help="Select screen mode (0, 1, 2)")
    ]
        
    parser = OptionParser(usage=uso_programa, description=descripcion_programa,
        version=version_programa, option_list=lista_de_opciones)
    
    # obtenemos las opciones y la lista de ficheros suministradas al programa
    (opciones, lista_ficheros_tmp) = parser.parse_args(linea_de_comandos)

    # comprobamos el número de argumentos y verificamos los valores
#    if (not lista_ficheros_tmp):
#        parser.error("No files to process.")
#    else:
    lista_ficheros = []
    for i in lista_ficheros_tmp:
        lista_ficheros = lista_ficheros + glob.glob(i)

    return opciones, lista_ficheros

# Función principal
def main(linea_de_comandos=None):
    """
    Función principal
    """
    # obtenemos las opciones y argumentos suministrados al programa
    opciones, lista_ficheros = procesar_linea_comandos(linea_de_comandos)


    # Creamos un disco formateado
    disco = [ [crea_track_info_block(num_pista), \
            [chr(DSK_FILL_BYTE) * LEN_SECTOR] * SECTORS_PER_TRACK] \
            for num_pista in range(TRACKS_PER_DISK)]

    # Comprobamos si debemos modificar un DSK ya existente
    if not (opciones.format_disk or not(os.path.exists(opciones.disk_filename))):
        # Cargamos el DSK y generamos una estructura "disco" correcta
        disco_tmp = ""
        with open(opciones.disk_filename,"rb") as fichero:
            disco_tmp = fichero.read()

        # Eliminamos el DIB
        disco_tmp = disco_tmp [LEN_DISK_INFO_BLOCK: ]
        # Troceamos el disco en pistas
        disco_tmp = trocea (disco_tmp, (LEN_TRACK_INFO_BLOCK + (LEN_SECTOR * SECTORS_PER_TRACK)))
        # Eliminamos los TIB
        disco_tmp = "".join([disco_tmp [i] [LEN_TRACK_INFO_BLOCK: ] for i in range(len(disco_tmp))])
        # Troceamos las pistas en sectores
        disco_tmp = trocea (disco_tmp, LEN_SECTOR)
        # Los metemos en la estructura "disco"
        num_sector = 0
        num_pista = 0
        for sector_de_fichero in disco_tmp:
            # Comprobamos si necesitamos pasar de pista
            if num_sector > 8:
                num_sector = 0
                num_pista += 1
            # Reemplazamos el sector existente por el sector con datos del fichero
            disco [num_pista] [1] [num_sector] = sector_de_fichero
            num_sector += 1
        # Y los reordenamos
        for num_pista in range(TRACKS_PER_DISK):
            disco [num_pista] [1] = [disco [num_pista] [1] [SECTOR_UNSORT [i]] \
                                        for i in range(SECTORS_PER_TRACK)] 

    # E insertamos los ficheros en los sectores
    if lista_ficheros != []:
        # Leemos todos los ficheros y los partimos en trozos de 512 bytes
        num_pista = optiones.disk_sector / (SECTOR_PER_TRACK - 1)
        num_sector = opciones.disk_sector % (SECTOR_PER_TRACK - 1)  # Sector inicial
        for nombre_fichero in lista_ficheros:
            # Comprobamos si existen
            if not(os.path.exists(nombre_fichero)):
                print ("The file %s doesn't exist.", nombre_fichero)
                continue

            # Los metemos en la lista de ficheros por sectores
            print ("Opening file: " + nombre_fichero)
            with open(nombre_fichero,"rb") as fichero:
#                fichero_en_sectores = particiona_fichero(fichero.read())
                fichero_en_sectores = fichero.read()
                
            # Añadimos el padding al fichero para hacerlo multiplo del tamaño de los sectores    
            fichero_en_sectores += (chr(DSK_FILL_BYTE) * (LEN_SECTOR - \
                                    (len(fichero_en_sectores) % LEN_SECTOR)) \
                                    if (len(fichero_en_sectores) % LEN_SECTOR) else "")

            # Creamos una lista con los sectores del fichero y la devolvemos
            fichero_en_sectores = [fichero_en_sectores [i * LEN_SECTOR: (i + 1) * LEN_SECTOR] \
                                    for i in range(len(fichero_en_sectores) / LEN_SECTOR)]

            # A continuación lo vamos insertando en el disco
            sectores_totales = 0
            print ("Inicial --> Pista: %i Sector: %i" % (num_pista, num_sector))
            for sector_de_fichero in fichero_en_sectores:
                # Comprobamos si necesitamos pasar de pista
                if num_sector > 8:
                    num_sector = 0
                    num_pista += 1
                    sectores_totales += 1
                # Reemplazamos el sector existente por el sector con datos del fichero
#                print ("Pista: %i Sector: %i" % (num_pista, num_sector))
                disco [num_pista] [1] [num_sector] = sector_de_fichero
                num_sector += 1
                sectores_totales += 1
            print ("Final --> Pista: %i Sector: %i Total: %i" % (num_pista, num_sector - 1, sectores_totales))

    # Reordenamos los sectores de una pista y los precedemos por la TIB de esa pista
    for num_pista in range(TRACKS_PER_DISK):
        disco [num_pista] = [disco [num_pista] [0]] + \
                            [disco [num_pista] [1] [SECTOR_SORT [i]] \
                            for i in range(SECTORS_PER_TRACK)] 

    # Creamos el DSK fusionando todas las pistas
    dsk_tmp = DISK_INFO_BLOCK
    for num_pista in range(TRACKS_PER_DISK):
        dsk_tmp += "".join(disco [num_pista])

    # Y guardamos el disco
    with open(opciones.disk_filename,"wb") as fichero:
        fichero.write(dsk_tmp)

    return 0    # EXIT_SUCCESS

if __name__ == "__main__":
    estado = main()
    sys.exit(estado)
