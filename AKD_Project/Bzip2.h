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


private:

};



