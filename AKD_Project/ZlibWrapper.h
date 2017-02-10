#pragma once

#include "zipper.h"
#include "unzipper.h"
#include "CustomExceptions.h"
#include <list>
#include <fstream>

class ZlibWrapper
{
public:
	ZlibWrapper();
	~ZlibWrapper();
	void Open(char* archiveName);
	void Open(char* archiveName, bool forceNew);
	void Close();
	void Create(char* archiveName);
	std::vector<std::string> ListContents();
	unz_file_info64 GetHeader(const char* fileName);


	/*
	Levels of compression for deflate
		Z_NO_COMPRESSION         0
		Z_BEST_SPEED             1
		Z_BEST_COMPRESSION       9
		Z_DEFAULT_COMPRESSION  (-1)

	Levels of compression for bzip2:
	1 - best speed
	2...8
	9 - best compression
	*/
	void AddFile(const char* fileName, bool bz2Compression, int compressionLevel);

	void AddRawFile(const char* archivePath ,const char* fileName, std::vector<char> data, bool bz2Compression, unz_file_info64 fi);

	std::vector<char> GetFile(const char*fileName);

	std::vector<char> GetRawFile(const char* fileName);

private:
	akdzlib::zipper zipArchive;
	akdzlib::unzipper unzipArchive;
	bool _isArchiveOpen;

	static bool fileExist(const std::string& name);
	static void throwCustomException(int exceptionNumber);
};

