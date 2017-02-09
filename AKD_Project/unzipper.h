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

		bool open( const char* filename );
		void close();
		bool isOpen();

		bool openEntry( const char* filename );
		void closeEntry();
		bool isOpenEntry();
		unsigned int getEntrySize();
		unz_file_info64 getEntryHeader() const;

		char* getContent();

		const std::vector<std::string>& getFilenames();
		const std::vector<std::string>& getFolders();

		unzipper& operator>>( std::ostream& os );

	private:
		void readEntries();

	private:
		unzFile			zipFile_;
		bool			entryOpen_;

		std::vector<std::string> files_;
		std::vector<std::string> folders_;
	};
};
