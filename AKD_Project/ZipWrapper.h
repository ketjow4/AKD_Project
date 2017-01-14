#pragma once
#define CHUNK 16384

#include <stdio.h>
#include <string.h>
#include <assert.h>
#define ZLIB_WINAPI
#include <zlib.h>
#include <string>
#include <iostream>

#if defined(MSDOS) || defined(OS2) || defined(WIN32) || defined(__CYGWIN__)
#  include <fcntl.h>
#  include <io.h>
#  define SET_BINARY_MODE(file) setmode(fileno(file), O_BINARY)
#else
#  define SET_BINARY_MODE(file)
#endif


class ZipWrapper
{
public:
	ZipWrapper();
	ZipWrapper(char * path);
	~ZipWrapper();

	bool CompressFromStrandard();
	bool DeCompressFromStrandard();
	bool CompressFile(FILE *source, FILE *dest);
private:
	int Compress(FILE *source, FILE *dest, int level);
	int DeCompress(FILE *source, FILE *dest);

	FILE *_sourceFile;
};

