#include <stdint.h>
#include <vector>

extern int fd; // TODO cleanup, make an open() call.
extern uint8_t* diskbuffer;

// Public API

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
void makedir(const char* name);
	// Create a directory in current directory
void changedir(const char* where);

// Internal stuff. Don't use.

const char* normalize_name(const char* original);
	// Normalize a filename to uppercase, space-padded, without extension dot.
	// DOES NOT HANDLE SUBDIRS

uint16_t fentry_to_sector(uint16_t fentry);
uint16_t fentry_to_offset(uint16_t fentry);
	// Given an fentry, compute the sector, and offset in sector, where it is
	// stored.

void stack_extents(uint16_t fentry, std::vector<struct extent>& extents);
	// Get all the sectors for a file, or all the entries for a dir, on a stack
	// for later handling.

typedef bool(*handle_file)(uint16_t fentry, void* cookie);
	// Ever heard of JP (HL) ? Well it's time to use it !

bool for_each_file_in_dir(uint16_t dentry, handle_file handler, void* cookie);
	// For each file in the given directory, call the given handler function
	// giving it the fentry.
	// The cookie is kept accross calls to the handler.

bool allocateFromEnd(uint16_t sectorcount, std::vector<uint16_t>& secList);
	// Allocate space in the block map
	// We start from the END of the map, in the beginning is the sector list,
	// which we want to stay contiguous.

//uint16_t allocate_direntry();
	// Find a free entry in the file list and allocate it. Shared between write
	// and mkdir.

//uint16_t register_direntry(uint16_t ent);
	// Register a new entry in the current directory. USed when creating a new
	// file or directory.
