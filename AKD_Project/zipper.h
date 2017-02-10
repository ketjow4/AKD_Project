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

		bool open(const char* filename, bool append = false);
		void close();
		bool isOpen() const;

		int addEntry(const char* filename, bool bz2Compression, int compressionLevel);
		bool addRawEntry(const char* filename, bool bz2Compression);
		void closeEntry();
		bool isOpenEntry() const;

		void zipper::writeRawData(char* data, int length, long uncompressed_size, long crc32) const;

		zipper& operator<<(std::istream& is);

	private:
		static void getTime(tm_zip& tmZip);

		zipFile			zipFile;
		bool			entryOpen;
	};

};
