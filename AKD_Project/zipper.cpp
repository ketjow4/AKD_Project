#include "zipper.h"
#include <zlib.h>
#include <algorithm>
#include <sstream>
#include <time.h>

namespace akdzlib
{
	

	zipper::zipper() :
		zipFile(nullptr),
		entryOpen(false),
		progressBar(nullptr)
	{
	}

	zipper::~zipper(void)
	{
		close();
	}

	bool zipper::open(const char* filename, bool append)
	{
		close();
		zipFile = zipOpen64(filename, append ? APPEND_STATUS_ADDINZIP : 0);

		return isOpen();
	}

	void zipper::close()
	{
		if (zipFile)
		{
			closeEntry();
			zipClose(zipFile, nullptr);
			zipFile = nullptr;
		}
	}

	bool zipper::isOpen() const
	{
		return zipFile != nullptr;
	}

	int zipper::addEntry(const char* filename, bool bz2Compression, int compressionLevel)
	{
		if (isOpen())
		{
			closeEntry();

			while (filename[0] == '\\' || filename[0] == '/')
				filename++;

			zip_fileinfo zi = { 0 };
			getTime(zi.tmz_date);

			int err = 0;
			if (!bz2Compression)
			{
				if (compressionLevel != Z_NO_COMPRESSION &&
					compressionLevel != Z_BEST_SPEED &&
					compressionLevel != Z_BEST_COMPRESSION &&
					compressionLevel != Z_DEFAULT_COMPRESSION) compressionLevel = Z_DEFAULT_COMPRESSION;

				err = zipOpenNewFileInZip(zipFile, filename, &zi,
					nullptr, 0, nullptr, 0, nullptr, Z_DEFLATED, compressionLevel);
			}
			else
			{
				if (compressionLevel > 9) compressionLevel = 9;
				if (compressionLevel < 1) compressionLevel = 1;
				err = zipOpenNewFileInZip(zipFile, filename, &zi,
					nullptr, 0, nullptr, 0, nullptr, Z_BZIP2ED, compressionLevel);
			}
			entryOpen = (err == ZIP_OK);
			return err;
		}
		return -1;
	}

	void zipper::closeEntry()
	{
		if (entryOpen)
		{
			zipCloseFileInZip(zipFile);
			entryOpen = false;
		}
	}

	bool zipper::isOpenEntry() const
	{
		return entryOpen;
	}

	bool zipper::addRawEntry(const char * filename, bool bz2Compression)
	{
		if (isOpen())
		{
			closeEntry();

			while (filename[0] == '\\' || filename[0] == '/')
				filename++;

			zip_fileinfo zi = { 0 };
			getTime(zi.tmz_date);

			int err = ZIP_OK;

			err = zipOpenNewFileInZip4_64(zipFile, filename, &zi,
				nullptr, 0, nullptr, 0, nullptr, bz2Compression ? Z_BZIP2ED : Z_DEFLATED, NULL, 1,
				-MAX_WBITS, DEF_MEM_LEVEL, Z_DEFAULT_STRATEGY, nullptr, 0, 0, 0, 0);

			entryOpen = (err == ZIP_OK);
		}
		return entryOpen;

	}

	zipper& zipper::operator<<(std::istream& is)
	{
		//get size of the file
		is.seekg(0, std::ios_base::end);
		int length = is.tellg();
		is.seekg(0, std::ios_base::beg);

		int err = ZIP_OK;
		char* buf = new char[bufferSize];
		if (isOpenEntry())
		{
			long read = 0;
			while (err == ZIP_OK && is.good())
			{
				is.read(buf, bufferSize);
				unsigned int nRead = static_cast<unsigned int>(is.gcount());
				read += nRead;
				if (nRead)
					err = zipWriteInFileInZip(zipFile, buf, nRead);
				else
					break;
				if (progressBar != nullptr)
					progressBar(read, length);
			}
		}
		delete[] buf;
		return *this;
	}

	void zipper::writeRawData(char* data, int length, long uncompressed_size, long crc32) const
	{
		int err = ZIP_OK;

		err = zipWriteInFileInZip(zipFile, data, length);
		zipCloseFileInZipRaw64(zipFile, uncompressed_size, crc32);
		if (progressBar != nullptr)
			progressBar(uncompressed_size, uncompressed_size);
	}

	void zipper::writeRawData(std::string filename, long uncompressed_size, long crc32) const
	{
		int err = ZIP_OK;
		std::ifstream in(filename, std::ios_base::binary | std::ios_base::in);
		in.seekg(0, std::ios_base::end);
		int length = in.tellg();
		in.seekg(0, std::ios_base::beg);
		if(in.is_open())
		{
			char* buf = new char[bufferSize];
			if (isOpenEntry())
			{
				long read = 0;
				while (err == ZIP_OK && in.good())
				{
					in.read(buf, bufferSize);
					unsigned int nRead = static_cast<unsigned int>(in.gcount());
					read += nRead;
					if (nRead)
						err = zipWriteInFileInZip(zipFile, buf, nRead);
					else
						break;
					if (progressBar != nullptr)
						progressBar(read, length);
				}
			}
			delete[] buf;
			zipCloseFileInZipRaw64(zipFile, uncompressed_size, crc32);
		}	
	}

	void zipper::getTime(tm_zip& tmZip)
	{
		time_t rawtime;
		time(&rawtime);
		auto timeinfo = localtime(&rawtime);
		tmZip.tm_sec = timeinfo->tm_sec;
		tmZip.tm_min = timeinfo->tm_min;
		tmZip.tm_hour = timeinfo->tm_hour;
		tmZip.tm_mday = timeinfo->tm_mday;
		tmZip.tm_mon = timeinfo->tm_mon;
		tmZip.tm_year = timeinfo->tm_year;
	}
};
