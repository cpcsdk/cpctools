/* CPCSD Filesystem
 * Copyright 2012, Adrien Destugues <pulkomandy@gmail.com>
 * This program is distributed under the terms of the MIT licence
 */
#include <ctype.h>
#include <fcntl.h>
#include <libgen.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <sys/mman.h>

// linux
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>

#include <vector>

#define RED "\x1B[31m"
#define BLACK "\x1B[0m"

// Global vars neede on CPC as well.
uint8_t* diskbuffer;
	// Work-buffer used when accessing files. Blockmap and filemap sectors are
	// usually stored here.

// Constants
const uint16_t offset_filemap = 16;

// Global vars related to PC implementation only. Not needed on CPC.
int fd;

void readsector(uint16_t sector);
void writesector(uint16_t sector);

bool check_magic();
	// Check wether a drive is already formatted
void format_device();
	// Get ready for action !
void list_wd();
	// Print list of files in current dir
void writefile(const char* source, const char* dest);
	// Copy source (host file) to dest (on disk)
void readfile(const char* source, const char* dest);
	// Copy source (on disk) to dest (host file)
void erasefile(const char* file);
	// Erase file from disk (free block map and fentries)
void hexdump(uint16_t sid);
	// Dump sector
const char* normalize_name(const char* original);
	// DOES NOT HANDLE SUBDIRS

// Filemap entries :
// 0 1 2 3 4 5 6 7 8 9 A B C D E F
// Sec L|F I L E N A M E.E X T|Nxt		Nxt = offset of next filemap entry
// 											for this file (or itself for last)
// 										Sec = first sector of file (or 0 for end)
// 										L   = number of consecutive sectors
// 											starting at sec (0 = 256)
// Entry pointed by Nxt :
// Sec L|Sec L|Sec L|Sec L|1st|Nxt		1st = pointer to 1st entry of file
// 										(or is there something more useful
// 										to store here ?)
//
// Ent L|D I R N A M E L O N G|Nxt		Using extents for direntries as well
// Ent L|Ent L|Ent L|Ent L|1st|Nxt		is interesting for unfragmented files
// 										and dirs. But does not scale well on
// 										fragmented drives (only 4 entries/line
// 										instead of 6). 
// End of Catalog (always last entry of a sector)
// EOC|EOC|EOC|EOC|EOC|EOC|EOC|EOC		All fields point to entry id of itself.
// 										(for first sector it is 20002000...)
// 										Note: only safe way to detect it is :
// 										Nxt == EOC AND Sec0 == EOC.
// 										This way :
// 										* Nxt == EOC tells us it is an end of
// 										file/dir entry. Sec is either part of
// 										first or last extent.
// 										* fentry pointing to itself doesn't
// 										exist (unless we look at an empty root
// 										dir, but this can't happen either as we
// 										are on the last entry of a sector).
//
// Ent and Sec/L are allocated from END of entry. This way, the first entry
// with filename can be handled the same as others, starting with sec = 0
// instead of sec = 3.
//
// Allocated entry : Nxt != 0
// Exception is the root dir where Nxt = 0 is valid (but it is always allocated)

// For files : a sector on disk where data for the extent starts, and sector
// count at this place (0=256)
// For dirs : an entry number where the dir content starts, and the number of
// following entries that are part of the dir
struct __attribute((packed)) extent
{
	uint16_t sector;
	uint8_t length;
};


// "first" regular file or dir-entry
// next = entry-id of xentry with more extents of this file/dir data
struct __attribute__((packed)) fentry
{
	struct extent x;
	char name[11];
	uint16_t next;	
};


// "extended" file or dir-entry
// Linked list of extents. Points to "first" so we know which file/dir it 
// belongs to (but this is not used by the FS besides possible checkfs code)
struct __attribute((packed)) xentry
{
	struct extent x[4];
	uint16_t first;
	uint16_t next;
};


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


bool check_magic()
{
	// Compare the first entry of the file map to "ROOT    .DIR". If it
	// matches, the disk is formatted
	
	// File map starts at sector 16
	readsector(offset_filemap);

	return memcmp(diskbuffer + 3, "ROOT    DIR", 11) == 0;
}


void format_device()
{
	diskbuffer[0] = 0xFF;	// Block map occupies the first 16 sectors
	diskbuffer[1] = 0xFF;
	diskbuffer[2] = 1;		// And root file map is just next to it
							// (1 sector at initialization, will grow later)
	memset(diskbuffer + 3, 0, 512 - 3); // Clear the block map
	writesector(0);

	// Other sectors of the block map are 0-filled (all empty)
	diskbuffer[0] = 0;
	diskbuffer[1] = 0;
	diskbuffer[2] = 0;
	for (int i = 1; i <= 16; i++)
	{
		writesector(i);
	}

	// First sector of the catalog has the root dir, and is otherwise zero 
	// filled
	memcpy(diskbuffer, "\0\0\0ROOT    DIR\0", 16); // Set up the root direntry
	for (int u = 512 - 16; u < 512; u += 2)
	{
		*(uint16_t*)(diskbuffer + u) = 32;
	}
	writesector(offset_filemap);
}


// Hexdump 512 bytes of pointed data. For debug purposes
void hexdump(uint8_t* diskbuffer)
{
	for (int i = 0; i < 512; i += 16)
	{
		char buf[17];
		buf[16] = 0;
		for (int j = 0; j < 16; j++)
		{
			buf[j] = isprint(*(diskbuffer+i+j)) ? *(diskbuffer+i+j) : '.';
		}
		printf("%04.4X %02.2X %02.2X %02.2X %02.2X %02.2X %02.2X %02.2X %02.2X "
				"%02.2X %02.2X %02.2X %02.2X %02.2X %02.2X %02.2X %02.2X "
			"%s\n",
			i, *(diskbuffer+i), *(diskbuffer+i+1), *(diskbuffer+i+2),
			*(diskbuffer+i+3), *(diskbuffer+i+4), *(diskbuffer+i+5),
			*(diskbuffer+i+6), *(diskbuffer+i+7), *(diskbuffer+i+8),
			*(diskbuffer+i+9), *(diskbuffer+i+10), *(diskbuffer+i+11),
			*(diskbuffer+i+12), *(diskbuffer+i+13), *(diskbuffer+i+14),
			*(diskbuffer+i+15), buf
		);
	}
}


// Hexdump a sector from disk. Corrupts the diskbuffer.
void hexdump(uint16_t sid)
{
	readsector(sid);
	hexdump(diskbuffer);
}


// Just round up to next sector size. I'm sure there is no need for a division
// in z80 code...
uint16_t ceil_512(uint32_t a)
{
	return 1 + ( a - 1 ) / 512;

	// Something like that maybe ?
	// HL = value to round
	// 	LD A,L
	// 	CP 0
	// 	JR Z, no_increase
	// 	INC H
	// .no_increase
	//	SRA H
	//	RET
	//	> Result is in H
}


uint16_t fentry_to_sector(uint16_t fentry)
{
	return fentry / 512 + offset_filemap;
}


uint16_t fentry_to_offset(uint16_t fentry)
{
	return (fentry % 32) * 16;
}


// Get all the sectors for a file, or all the entries for a dir, on a stack for
// later handling.
void stack_extents(uint16_t fentry, std::vector<struct extent>& extents)
{
	uint16_t sector, offset, nxt;
	int8_t xtc = 1;
	do {
		sector = fentry_to_sector(fentry);
		offset = fentry_to_offset(fentry);

		readsector(sector);

		for(; --xtc >= 0;)
		{
			struct extent* x = (struct extent*)(diskbuffer + offset +
					xtc * sizeof(struct extent));
			if (x->sector == 0) {
				// Allocation may end in the middle of an extent (or a dir may
				// be empty). Since count = 0 means 256 entries, we have to
				// check for sector being 0 to know
				return;
			}
			extents.push_back(*x);
		}

		nxt = *(uint16_t*)(diskbuffer + offset + 14);
		xtc = 4;
	} while(fentry != nxt);
}


// Ever heard of JP (HL) ? Well it's time to use it !
typedef bool(*handle_file)(uint16_t fentry, void* cookie);

// For each file in the given directory, call the given handler function giving
// it the fentry.
// The cookie is kept accross calls to the handler
bool for_each_file_in_dir(uint16_t dentry, handle_file handler, void* cookie)
{
	std::vector<struct extent> entries;
	stack_extents(dentry, entries);
	
	while(!entries.empty())
	{
		// Handle next extent
		struct extent x = entries.back();
		entries.pop_back();

		do {
			if (handler(x.sector, cookie))
				return true;

			++x.sector;
		} while (-- x.length > 0);
	}

	return false;
}

struct listwd_cookie
{
	int nfiles;
	uint16_t cursec;
};

bool listwd_handle(uint16_t fentry, void* cookie)
{
	struct listwd_cookie* cooked = (struct listwd_cookie*)cookie;
	char fname[12];
	fname[11] = 0;

	uint16_t newsec = fentry_to_sector(fentry);
	// Do we need to change sector ?
	if (newsec != cooked->cursec)
	{
		cooked->cursec = newsec;
		readsector(newsec);
	}

	uint16_t offset = fentry_to_offset(fentry);
	struct fentry* entry = (struct fentry*)(diskbuffer + offset);
	memcpy(fname, entry->name, 11);

	// TODO print size ? check isdir and attrs
	printf("%s\t", fname);

	cooked->nfiles++;

	return false;
}

// list files in current dir
void list_wd()
{
	uint16_t cwd = 0;
	// TODO for now this lists files in the root dir

	struct listwd_cookie cookie = {
		/*.nfiles =*/ 0,
		/*.cursec =*/ 0
			// Sector 0 is part of the block map, so it can't be mistaken for a
			// sector in a file entry. This way we make sure we read the first
			// needed sector. It would be better to know the last sector read
			// by stack_extents here (TODO)
	};

	for_each_file_in_dir(cwd, listwd_handle, &cookie);

	printf("\n %d files.\n", cookie.nfiles);
}


void readsector(uint16_t which)
{
	if (lseek(fd, which * 512, SEEK_SET) == -1)
		perror("Read sector can't seek");
	if (read(fd, diskbuffer, 512) == - 1)
		perror("Read sector can't read");
}

void writesector(uint16_t which)
{
	if (lseek(fd, which * 512, SEEK_SET) == -1)
		perror("Write sector can't seek");
	if (write(fd, diskbuffer, 512) == - 1)
		perror("Write sector can't write");
}


// Allocate space in the block map
// We start from the END of the map, in the beginning is the sector list,
// which we want to stay contiguous.
bool allocateFromEnd(uint16_t sectorcount, std::vector<uint16_t>& secList)
{
	bool changed = false;
	
	for (int8_t k = 16; --k >= 0;)
	{
		readsector(k);
		for (int16_t j = 512; --j >= 0;)
		for (int8_t i = 7; i >= 0; --i)
		{
			if (diskbuffer[j] & (1<<i)) continue; // not free

			// Mark the sector in the block map
			diskbuffer[j] |= (1<<i);
			changed = true;

			// Compute sector address
			uint16_t sector = i + 8 * (j + 512 * k);
			//printf("Allocating sector %d (%d %d %d)\n", sector,k,j,i);
			// store the new sector on the sector stack
			if (secList.empty())
			{
				// First entry in the list
				secList.push_back(sector);
				secList.push_back(1);
			} else {
				// List already has some entries, check if we can merge in the
				// previous extent
				uint16_t len = secList.back(); secList.pop_back();
				uint16_t oldsec = secList.back(); secList.pop_back();

				if (oldsec - 1 == sector)
				{
					// We have a match ! Merge this sector in the existing
					// extent
					// IMPORTANT REMARK: then len here is 16 BIT, on-disk
					// structure it is stored as 8 bits. So we might have to
					// split the extent later on. BUT, don't do it yet, at this
					// point it would increase memory usage to store multiple
					// consecutive extents.
					len++;
					oldsec--;

					secList.push_back(oldsec);
					secList.push_back(len);
				} else {
					secList.push_back(oldsec);
					secList.push_back(len);
					
					secList.push_back(sector);
					secList.push_back(1);
				}
			}

			--sectorcount; // one sector less to store
			if (sectorcount == 0)
			{
				writesector(k);
				return true;
			}
		}
		if (changed) writesector(k);
	}

	return false;
}


struct readfile_cookie
{
	const char* d;
	const char* s;
};

bool readfile_handle(uint16_t entry, void* cookie)
{
	struct readfile_cookie* cooked = (struct readfile_cookie*)cookie;
	// Look at filename
	readsector(fentry_to_sector(entry));
	uint16_t off = fentry_to_offset(entry);
	if(memcmp(diskbuffer + off + 3, cooked->s, 11) == 0)
	{
		int outfd;
		bool toscreen = (cooked->d == NULL);
		if (toscreen) {
			printf("Extracting %s to %s\n", cooked->s, cooked->d);
			outfd = open(cooked->d, O_WRONLY | O_TRUNC | O_CREAT, 0444);
			if (outfd < 0)
				perror("Can't open host file for writing");

		}

		uint8_t* buf = (uint8_t*)malloc(512);
		//uint16_t which = *(uint16_t*)(diskbuffer + off);

		// Since the file might span several entries, we need to stack up
		// all the sectors in a list. Then, we can browse this list and
		// handle each element.
		std::vector<struct extent> entries;
		stack_extents(entry, entries);

		while(!entries.empty())
		{
			// Handle next extent
			struct extent x = entries.back();
			entries.pop_back();

			do {
				// Read sector x.sector to storage area
				if (lseek(fd, x.sector * 512, SEEK_SET) == -1)
					perror("Read sector can't seek");
				if (read(fd, buf, 512) == - 1)
					perror("Read sector can't read");

				if (toscreen)
					hexdump(buf);
				else
					write(outfd, buf, 512);

				++x.sector;
			} while (-- x.length > 0);
		}

		free(buf);
		if (!toscreen) close(outfd);
		return true;
	}

	return false;
}

void readfile(const char* s, const char* d)
{
	s = normalize_name(s);

	struct readfile_cookie cookie = {
		d, s
	};

	// TODO 0 = root dir for now, need to extract the dir from filepath
	if(!for_each_file_in_dir(0, readfile_handle, &cookie))
	{
		fprintf(stderr, "Source file %s not found.\n", s);
	}
}


bool try_allocate_sector(uint16_t secid)
{
	int fms = secid / (512*8);
	readsector(fms);

	int off = (secid % (512*8)) / 8;
	int bit = (secid % (512*8)) % 8;

	if (diskbuffer[off] & (1 << bit))
		return false;
	diskbuffer[off] |= (1 << bit);
	return true;
}


void writefile(const char* s, const char* d)
{
	d = normalize_name(d);
	// TODO parse d to find directory we are adding to
	// for now it's always the root
	uint16_t dirent = 0;

	// Get some info about the source file
	struct stat info;
	stat(s, &info);
	int filesize = ceil_512(info.st_size); // filesize is in sectors

	printf("Writing file %s (%d sectors)\n",s, filesize);

	// Allocate sectors to store the file
	std::vector<uint16_t> sectors;
	if (!allocateFromEnd(filesize, sectors))
	{
		puts(RED "ERROR" BLACK " Not enough space on disk to write this file.");
		// TODO free the blocks again - share code with ERA function
		return;
	}

	// Find a free dir entry and register it
	uint16_t dirsec = offset_filemap;
	uint16_t off =16;
	for(;; off = 0)
	{
		readsector(dirsec);
		// Note: we start at off = 16 (past ROOT.DIR) bcause the root dir is
		// always allocated (hopefully), and may still have Nxt==0.
		for(; off < 512; off += 16)
		{
			struct fentry* entry = (struct fentry*)(diskbuffer + off);
			if(entry->next == 0) break;
			// Check for free entries using the Nxt field
		}

		// Did we find a free entry in this sector ?
		if(off < 512) break;

		// Go back to last scanned entry (last one in sector)
		off -= 16;

		// Check if it is EOC marker.
		if (*(uint16_t*)(diskbuffer + off) == off / 16
				&&	*(uint16_t*)(diskbuffer + off + 14) == off / 16)
		{
			// End of directory reached
			// allocate more directory space if possible (only contiguous
			// sectors are allowed, so if the next one is not free, we're toast)
			if (!try_allocate_sector(dirsec + 1))
			{
				fprintf(stderr, "Directory full.\n");
				// TODO handle directory full error. Need to free the allocated
				// blocks again !
				return;
			}

			// Note : Allocating next sector will free this last entry, so we
			// can use it. We need to clear out the next sector and put the end
			// marker there, however (is it time to mess with the sector buffer
			// again ?)
			memset(diskbuffer, 0, 512 - 16);
			for (int u = 512 - 16; u < 512; u += 2)
			{
				*(uint16_t*)(diskbuffer + u) = 32 + (dirsec + 1) * 32;
			}
			writesector(dirsec + 1);
			readsector(dirsec);

			break;
		} else {
			dirsec++;
		}
	}

	// Store file name
	struct fentry* entry = (struct fentry*)(diskbuffer + off);
	memcpy(entry->name, d, 11);
	entry->next = off / 16 + (dirsec - offset_filemap) * 32;

	// Add the allocated blocks to the filemap now
	// DO NOT empty the sector list, we'll need it again when writing data !
	size_t n = sectors.size();

	struct xentry* entrymap = (struct xentry*) entry;
	int8_t entrypos = 0;
	while(n >= 0)
	{
		uint16_t len = sectors[--n];
		uint16_t sector = sectors[--n];

		entrymap->x[entrypos].sector = sector;
		entrymap->x[entrypos].length = len;

		if (--entrypos < 0)
		{
			// TODO allocate next catalog entry to continue entrylist
			// (maybe we need to allocate all the cat-entries before filling
			// them, because they can be scattered accross sectors...)
			if (n != 0) fprintf(stderr, RED "ERROR" BLACK
				" fragmented files are not handled properly.\n"
				"File has %d fragments.\n", n/2 + 1
			);
			break;
		}
	}

	printf("About to write sector 16 with following data:\n");
	hexdump(diskbuffer);
	writesector(offset_filemap);

	// Store the file data (finally ! :) )
	int infd = open(s, O_RDONLY);

	// On CPC, the data to write is already in memory.
	uint8_t* buf = (uint8_t*)mmap(NULL, filesize*512, PROT_READ, MAP_PRIVATE, infd, 0);
	uint8_t* ptr = buf;

	while(!sectors.empty())
	{
		uint16_t len = sectors.back(); sectors.pop_back();
		uint16_t sec = sectors.back(); sectors.pop_back();

		if (len == 0) len = 256;
		lseek(fd, sec * 512, SEEK_SET);
		write(fd, buf, 512 * len);
	}

	munmap(buf, filesize * 512);
	close(infd);

	uint16_t olddirsec = 0;
	dirent = 0;
		// TODO update the parent dir (root dir for now)
	char entx = 0;
	for(dirsec = offset_filemap;;)
	{
		if (dirsec != olddirsec) {
			readsector(dirsec);
			olddirsec = dirsec;
		}

		off /= 16; // This is the file entry offset

		struct xentry* dir = (struct xentry*)diskbuffer + fentry_to_offset(dirent);
		if (dir->x[entx].sector == 0)
		{
			// Easy case, this is the first file !
			dir->x[entx].sector = off;
			dir->x[entx].length = 1;
			break;
		} else if (dir->x[entx].sector + dir->x[entx].length == off) {
			// Append to the END of entryblock
			++ dir->x[entx].length;
			// TODO check for overflow ! If it happens we need to allocate
			// another entryblock
			break;
		} else if (dir->x[entx].sector - 1 == off) {
			// Append to the BEGINNING of entryblock
			++ dir->x[entx].length;
			// TODO check for overflow ! If it happens we need to allocate
			// another entryblock
			-- dir->x[entx].sector;
			break;
		} else if (--entx < 0) {
			// move on to next entry
			dirent = dir->next;
			dirsec = fentry_to_sector(dirent);
			entx = 3;
		} // else continue with next entx
	}

	writesector(dirsec);
}

// Rather primitive way of doing it, but should be ok most of the time.
const char* normalize_name(const char* original)
{
	static char buffer[12];
	buffer[11] = 0; // Not useful on CPC ?

	int j = 0;
	for(int i = 0; original[i] != 0; i++)
	{
		if (original[i] == '.')
		{
			while(j < 8) buffer[j++] = ' ';
			continue;
		}
		buffer[j++] = toupper(original[i]);

		if (j > 11) break;
	}

	while(j < 12) buffer[j++] = ' ';
	return buffer;
}


bool erase_handle(uint16_t entry, void* cookie)
{
	// get the name we are looking for
	const char* d = (const char*)cookie;

	// Look at filename
	readsector(fentry_to_sector(entry));
	uint16_t off = fentry_to_offset(entry);

	// Does name match ?
	if(memcmp(diskbuffer + off + 3, d, 11) != 0)
		return false;

	// Ok, name found, now handle the file
	
	// Since the file might span several entries, we need to stack up
	// all the sectors in a list. Then, we can browse this list and
	// handle each element.
	std::vector<struct extent> entries;
	// TODO Would be nice to clear the entries as we gather them. Else we'll
	// need to read the same sectors again when removing them. But this code
	// is getting crazy enough, and I want to test it first.
	stack_extents(entry, entries);

	while(!entries.empty())
	{
		// Handle next extent
		struct extent x = entries.back();
		entries.pop_back();

		// Free the extent in the block map (it is faster to free a whole
		// extent at once than bit by bit)
		
		// Convert extent start sector to byte + bit in sector map
		uint8_t sbit = 1 << (x.sector & 7);
		uint16_t sbyte = (x.sector >> 3) & 0x1FF;
		uint8_t ssector = x.sector >> 9;

		if (x.length == 0)
			x.sector += 256;
		else
			x.sector += x.length;
		// Compute end position
		uint8_t bite = 1 << (x.sector & 7);
		uint16_t bytee = (x.sector >> 3) & 0x1FF,
		uint8_t sectore = x.sector >> 9;

		uint8_t sector = ssector;
		uint16_t byte = sbyte;
		while (sector <= sectore)
		{
			readsector(sector);

			if (sector == ssector)
			{
				// process first byte
				uint8_t bits = 0;
				while(bits < sbit)
				{
					bits = (bits<<1) | 1;
				}

				diskbuffer[byte] &= ~bits;
			} else 
				byte = -1;

			if (sector == sectore)
			{
				sbyte = bytee;
			} else {
				sbyte = 512;
			}

			// process middle bytes (if any)
			while (++byte < sbyte)
			{
				diskbuffer[byte] = 0;
			}

			if (sector == sectore)
			{
				// process last byte
				bits = 255;
				while(bits > bite)
				{
					bits >>= 1;
				}

				diskbuffer[byte] &= ~bits;
			}

			writesector(sector);
			sector++;
		}
	}

	// TODO
	// Now we need to erase the fentry itself and any associated xentries.
	// We know the sector number and offset, so all is fine.
	
	// TODO
	// And finally, the most tricky part, we need to erase the pointer to this
	// file in the parent dir. We may need to split an extent if the file is
	// in the middle of it. This means the direntry can grow and require an
	// extra xentry. We know we can reuse the one from the file we just deleted,
	// so this is safe. The directory never grows here (phew!).
	
	return true;
}

// Erase one single file from the disk.
// TODO handle file patterns (* and ?) for multi-file erase
void erase_file(const char* name)
{
	// Locate the file in the entry table.
	if (!for_each_file_in_dir(0, erase_handle, &cookie))
	{
		fprintf(stderr, "File %s not found.\n", s);
	}
}

// More TODO
// CheckDisk features :
//  * Check every file is in a dir
//  * Check allocated xentries are linked to a file or dir
//  * check no files are overlapped (both storage and entries !)
//  * check allocation of file matches bit map
//  * check each bit allocated in map actually belongs to a file

// Note on directories
// There is no concept of a current path nor parent dirs. Only the current dir
// is relevant. This means there is no simple way of doing 'cd ..'. Start from
// the rootdir again !

// Thoughts about RSXs
// ùCD with no args goes back to root dir. May use ùCD,"path","to","subfolder" ?
// or is it simpler to use "path/to/subfolder" ? Or accept both ?
// ùERA remove dirs, but needs to check for emptyness before
// Since the filesystem structure allows it, maybe ùSEARCH to look for a file ?
// but we have no way to find which dir it's in.

// When creating a file, we also need to add an xentry to the dir. This can
// very easily get quite tricky to do right...
