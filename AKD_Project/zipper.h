#pragma once
#include <string>
#include <vector>
#include <iostream>
#include <time.h>
#include "minizip/zip.h"
#include "minizip/ioapi.h"

namespace akdzlib
{
	class zipper
	{
	public:
		zipper();
		~zipper(void);

		bool open( const char* filename, bool append = false );
		void close();
		bool isOpen();

		int addEntry( const char* filename,  bool bz2Compression, int compressionLevel);
		void closeEntry();
		bool isOpenEntry();

		bool addRawEntry(const char* filename, bool bz2Compression);

		void zipper::WriteRawData(char* data, int length, long uncompressed_size, long crc32);

		zipper& operator<<( std::istream& is );

	private:
		void getTime(tm_zip& tmZip);



	private:
		zipFile			zipFile_;
		bool			entryOpen_;
	};

};
