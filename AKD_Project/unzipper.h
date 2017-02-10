#pragma once
#include "minizip/ioapi.h"
#include "minizip/unzip.h"
#include <string>
#include <vector>
#include <iostream>

namespace akdzlib
{
	class unzipper
	{
	public:
		unzipper();
		~unzipper(void);

		bool open(const char* filename);
		void close();
		bool isOpen() const;

		int openEntry(const char* filename, bool raw = false);
		void closeEntry();
		bool isOpenEntry() const;
		unsigned int getEntrySize(bool raw) const;
		unz_file_info64 getEntryHeader() const;

		std::vector<char> getContent(bool raw) const;
		const std::vector<std::string>& getFilenames() const;
		const std::vector<std::string>& getFolders() const;

		unzipper& operator >> (std::ostream& os);

	private:
		void readEntries();

		unzFile	zipFile;
		bool entryOpen;
		std::vector<std::string> files;
		std::vector<std::string> folders;
	};
};
