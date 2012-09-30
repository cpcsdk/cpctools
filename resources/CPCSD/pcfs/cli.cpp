/* CPCSD Filesystem - Command Line Interface
 * Copyright 2012, Adrien Destugues <pulkomandy@gmail.com>
 * Distributed under the terms of the MIT Licence
 */

#include <ctype.h>
#include <fcntl.h>
#include <libgen.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "mli.h"

#define RED "\x1B[31m"
#define BLACK "\x1B[0m"


bool yesno()
{
	char c;
	do c = tolower(getchar()); while (c != 'y' && c != 'n');
	return c == 'y';
}


int main(int argc, const char* argv[])
{
	if (argc != 2)
	{
		fprintf(stderr, "usage: %s path_to_device\n", basename((char*)argv[0]));
		exit(-1);
	}

	fd = open(argv[1], O_RDWR);

	if (fd <= 0)
	{
		fprintf(stderr, "can't open device. Sorry!\n");
		exit(-2);
	}

	char c = 0;

	diskbuffer = (uint8_t*)malloc(512);
		// We could allocate it statically or on the stack, but a bug in Haiku
		// prevents access to unaligned memory for EHCI usb driver. Using
		// malloc in Haiku is enough to make sure the memory is aligned.

	// on z80, we also need to check if device has a partition map, and if so,
	// wether one of the partitions is "magic". (this allows sharing an SD card
	// between MLI and another FS like FAT. An example use is inide the HxC
	// floppy emulator, which needs a FAT partition to load the config file.
	if (!check_magic())
	{
		printf(RED "WARNING" BLACK 
			" This device does not look like MLI-formatted !\n"
			"Format it now ? (y/n)\n");

		if (yesno())
		{
			format_device();
		}
		else
		{
			puts("Not touching device. Bye!");
			free(diskbuffer);
			close(fd);
			exit(0);
		}
	}

	// do stuff !
	puts("Now entering command mode. Press h for help.");

	for(;;)
	{
		printf(">");
		fflush(stdout);
		do {
			c = tolower(getchar());
		} while(!isalpha(c));

		switch(c)
		{
			/*
			case 'd':
				puts("not yet!");
				break;
			*/
			case 'e':
				char d[1024];
				scanf(" %s",d);
				printf("About to erase file %s. Are you sure ?\n",d);
				if (yesno())
					erasefile(d);
				break;
			case 'f':
				format_device();
				break;
			case 'h':
				// Show help
				puts(
					"Syntax of operands:\n"
					"  n - sector number\n"
					"  s - source file\n"
					"  d - destination file\n"
					"Available commands:\n"
					// C      - check disk structures
					//"  D d    - change dir to d\n"
					"  E d    - erase file\n"
					"  F      - format drive\n"
					"  H      - show this helptext\n"
					"  L      - list files\n"
					// M n    - mkdir n
					"  Q      - quit fs-shell\n"
					"  R s d  - read s (MLI) to d (host file)\n"
					"  T s    - hexdump file s\n"
					// U      - disk usage stats
					"  W s d  - write s (host file) to d (MLI file)\n"
					"  X n    - hexdump sector n\n"
				);
				break;
			case 'l':
				list_wd();
				break;
			case 'q':
				close(fd);
				free(diskbuffer);
				exit(0);
			case 'r':
				char s[1024];
				scanf(" %s %s",s,d);
				readfile(s,d);
				break;
			case 't':
				scanf(" %s",s);
				readfile(s,NULL);
				break;
			case 'w':
				scanf(" %s %s",s,d);
				writefile(s,d);
				break;
			case 'x':
				uint16_t sid;
				scanf(" %d",&sid);
				hexdump(sid);
			default:
				// Do nothing.
				break;
		}
	}
}
