//
// Caprice32 emulator predefined types
//

#ifndef _CAP32TYPES_H_
#define _CAP32TYPES_H_

#include <cstdint>

//typedef unsigned char byte;
//typedef unsigned short word;
//typedef unsigned int dword;
typedef uint8_t byte;
typedef uint16_t word;
typedef uint32_t dword;

typedef union {
#if SDL_BYTEORDER == SDL_LIL_ENDIAN
   struct { byte l, h, h2, h3; } b;
   struct { word l, h; } w;
#else
   struct { byte h3, h2, h, l; } b;
   struct { word h, l; } w;
#endif
   dword d;
}  reg_pair;

// TODO: Byte order
typedef struct ColorXRGB888 {
	uint8_t x; // Not used. Only for 32bits size.
	uint8_t r;
	uint8_t g;
	uint8_t b;
} ColorXRGB888;

typedef struct ColorARGB8888 {
	uint8_t a;
	uint8_t r;
	uint8_t g;
	uint8_t b;
} ColorARGB8888;

#define MAX_LINE_LEN 256

#endif
