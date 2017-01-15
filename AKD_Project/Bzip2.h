#pragma once



#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <bzlib.h>



class Bzip2
{
public:
	Bzip2() {}
	~Bzip2() {}

	int example()
	{

		//https://www.codeproject.com/Questions/362653/Compressing-text-files-with-bzlib-library-using-Cp

		char srcDir[] = "dirToZip/";
		char extractTo[] = ".";


		char tbz2Filename[] = "file.tar.bz2";
		FILE *tbz2File = fopen(tbz2Filename, "wb");
		int bzError;
		const int BLOCK_MULTIPLIER = 7;
		BZFILE *pBz = BZ2_bzWriteOpen(&bzError, tbz2File, BLOCK_MULTIPLIER, 0, 0);

		const int BUF_SIZE = 10000;
		char* buf = new char[BUF_SIZE];
		size_t bytesRead;
		/*while ((bytesRead = fread(tarFD, buf, BUF_SIZE)) > 0) {
			BZ2_bzWrite(&bzError, pBz, buf, bytesRead);
		}*/
		BZ2_bzWriteClose(&bzError, pBz, 0, NULL, NULL);


		delete[] buf;
		return 0;
	}


//anther example from internet
//http://stackoverflow.com/questions/3912157/how-do-i-extract-all-the-data-from-a-bzip2-archive-with-c

	int bunzip_one(FILE *f) {
		int bzError;
		BZFILE *bzf;
		char buf[4096];

		bzf = BZ2_bzReadOpen(&bzError, f, 0, 0, NULL, 0);
		if (bzError != BZ_OK) {
			fprintf(stderr, "E: BZ2_bzReadOpen: %d\n", bzError);
			return -1;
		}

		while (bzError == BZ_OK) {
			int nread = BZ2_bzRead(&bzError, bzf, buf, sizeof buf);
			if (bzError == BZ_OK || bzError == BZ_STREAM_END) {
				size_t nwritten = fwrite(buf, 1, nread, stdout);
				if (nwritten != (size_t)nread) {
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

	int
		bunzip_many(const char *fname) {
		FILE *f;

		f = fopen(fname, "rb");
		if (f == NULL) {
			perror(fname);
			return -1;
		}

		fseek(f, 0, SEEK_SET);
		if (bunzip_one(f) == -1)
			return -1;

		fseek(f, 42, SEEK_SET); /* hello.bz2 is 42 bytes long in my case */
		if (bunzip_one(f) == -1)
			return -1;

		fclose(f);
		return 0;
	}


private:

};



