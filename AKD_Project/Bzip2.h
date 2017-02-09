#pragma once



#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <bzlib.h>
#include <fstream>

using namespace std;

class Bzip2
{
public:
	Bzip2() {}
	~Bzip2() {}

	int compress()
	{
		ifstream  panTadeusz("foo2.txt");

		std::string str((std::istreambuf_iterator<char>(panTadeusz)),
			std::istreambuf_iterator<char>());

		char tbz2Filename[] = "file.bz2";
		FILE *tbz2File = fopen(tbz2Filename, "wb");
		int bzError;
		const int BLOCK_MULTIPLIER = 7;
		//BZFILE *pBz = BZ2_bzWriteOpen(&bzError, tbz2File, BLOCK_MULTIPLIER, 0, 0);

		BZFILE *pBz = BZ2_bzopen("file.bz2", "wb");

		const int BUF_SIZE = 10000;
		char* buf = new char[BUF_SIZE];
		size_t bytesRead;
		
			//BZ2_bzWrite(&bzError, pBz, &str[0], str.length());
		
		//BZ2_bzWriteClose(&bzError, pBz, 0, NULL, NULL);

		BZ2_bzwrite(pBz, &str[0], str.length());
		BZ2_bzclose(pBz);

		delete[] buf;
		return 0;
	}


//anther example from internet
//http://stackoverflow.com/questions/3912157/how-do-i-extract-all-the-data-from-a-bzip2-archive-with-c

	int decompress() 
	{
		int bzError;
		BZFILE *bzf;
		char buf[4096];
		FILE *f = fopen("file.zip", "rb");


		bzf = BZ2_bzReadOpen(&bzError, f, 0, 0, NULL, 0);
		if (bzError != BZ_OK) {
			fprintf(stderr, "E: BZ2_bzReadOpen: %d\n", bzError);
			return -1;
		}

		while (bzError == BZ_OK) 
		{
			int nread = BZ2_bzRead(&bzError, bzf, buf, sizeof buf);
			
			if (bzError == BZ_OK || bzError == BZ_STREAM_END) 
			{
				size_t nwritten = fwrite(buf, 1, nread, stdout);
				if (nwritten != (size_t)nread) 
				{
					fprintf(stderr, "E: short write\n");
					return -1;
				}
			}
		}

		if (bzError != BZ_STREAM_END) {
			fprintf(stderr, "E: bzip error after read: %d\n", bzError);
			return -1;
		}

		BZ2_bzReadClose(&bzError, bzf);
		return 0;
	}

	


private:

};



