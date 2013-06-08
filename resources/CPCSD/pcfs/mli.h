#include <stdint.h>

extern int fd; // TODO cleanup, make an open() call.
extern uint8_t* diskbuffer;

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
