#pragma once
/*
#ifdef __cplusplus
extern "C" {
#endif

typedef struct stb_vorbis stb_vorbis;

extern int stb_vorbis_decode_memory(const unsigned char* mem, int len, int* channels, int* sample_rate, short** output);

extern stb_vorbis* stb_vorbis_open_memory(const unsigned char* data, int len, int* error, const void* alloc_buffer);

extern void stb_vorbis_close(stb_vorbis* f);

extern int stb_vorbis_decode_filename(const char* filename, int* channels, int* sample_rate, short** output);

#ifdef __cplusplus
}
#endif*/
#define STB_VORBIS_HEADER_ONLY
#include "stb_vorbis.c"