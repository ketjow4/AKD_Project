#include "unzipper.h"
#include <zlib.h>
#include <algorithm>
#include <sstream>

namespace akdzlib
{
	unzipper::unzipper() :
		zipFile(nullptr),
		entryOpen(false),
		progressBar(nullptr)
	{
	}

	unzipper::~unzipper(void)
	{
		close();
	}

	bool unzipper::open(const char* filename)
	{
		close();
		zipFile = unzOpen64(filename);
		if (zipFile)
		{
			readEntries();
		}

		return isOpen();
	}

	void unzipper::close()
	{
		if (zipFile)
		{
			files.clear();
			folders.clear();

			closeEntry();
			unzClose(zipFile);
			zipFile = nullptr;
		}
	}

	bool unzipper::isOpen() const
	{
		return zipFile != nullptr;
	}

	const std::vector<std::string>& unzipper::getFilenames() const
	{
		return files;
	}

	const std::vector<std::string>& unzipper::getFolders() const
	{
		return folders;
	}

	int unzipper::openEntry(const char* filename, bool raw)
	{
		if (isOpen())
		{
			closeEntry();
			int err = 0;
			err = unzLocateFile(zipFile, filename, 0);
			if (err == UNZ_OK)
			{
				if (raw)
					err = unzOpenCurrentFile2(zipFile, nullptr, nullptr, 1);
				else
					err = unzOpenCurrentFile(zipFile);
				entryOpen = (err == UNZ_OK);
			}
			return err;
		}
		return 0;
	}

	void unzipper::closeEntry()
	{
		if (entryOpen)
		{
			unzCloseCurrentFile(zipFile);
			entryOpen = false;
		}
	}

	bool unzipper::isOpenEntry() const
	{
		return entryOpen;
	}

	unsigned int unzipper::getEntrySize(bool raw) const
	{
		if (entryOpen)
		{
			unz_file_info64 fileInfo;

			int err = unzGetCurrentFileInfo64(zipFile, &fileInfo, nullptr, 0, nullptr, 0, nullptr, 0);

			if (err == UNZ_OK)
			{
				if (raw)
					return static_cast<unsigned int>(fileInfo.compressed_size);
				else
					return static_cast<unsigned int>(fileInfo.uncompressed_size);
			}
		}
		return 0;
	}

	unz_file_info64 unzipper::getEntryHeader() const
	{
		unz_file_info64 fileInfo;
		if (entryOpen)
		{
			int err = unzGetCurrentFileInfo64(zipFile, &fileInfo, nullptr, 0, nullptr, 0, nullptr, 0);

			if (err == UNZ_OK)
				return fileInfo;
		}
		return fileInfo;
	}

	void unzipper::readEntries()
	{
		files.clear();
		folders.clear();

		if (isOpen())
		{
			unz_global_info64 globalInfo;
			int err = unzGetGlobalInfo64(zipFile, &globalInfo);
			for (unsigned long i = 0; i < globalInfo.number_entry && err == UNZ_OK; i++)
			{
				char filename[FILENAME_MAX];
				unz_file_info64 oFileInfo;

				err = unzGetCurrentFileInfo64(zipFile, &oFileInfo, filename,
					sizeof(filename), nullptr, 0, nullptr, 0);
				if (err == UNZ_OK)
				{
					char nLast = filename[oFileInfo.size_filename - 1];
					if (nLast == '/' || nLast == '\\')
						folders.push_back(filename);
					else
						files.push_back(filename);

					err = unzGoToNextFile(zipFile);
				}
			}
		}
	}

	unzipper& unzipper::operator >> (std::ostream& os)
	{
		if (isOpenEntry())
		{
			unsigned int size = getEntrySize(true);
			char* buf = new char[size];
			size = unzReadCurrentFile(zipFile, buf, size);
			if (size > 0)
			{
				os.write(buf, size);
				os.flush();
			}
			delete[] buf;
		}
		return *this;
	}

	std::vector<char> unzipper::getContent(bool raw) const
	{
		char* buf = nullptr;
		unsigned int size = 0;
		if (isOpenEntry())
		{
			size = getEntrySize(raw);
			buf = new char[size];
			size = unzReadCurrentFile(zipFile, buf, size);
			if (progressBar != nullptr)
				progressBar(size, size);
		}

		return std::vector<char>(buf, buf + size);
	}

};