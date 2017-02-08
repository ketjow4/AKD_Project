#pragma once

#include "zipper.h"
#include "unzipper.h"
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
	std::vector<std::string> ListContents();
	unz_file_info64 GetHeader(const char* fileName);
	void AddFile(const char* fileName);
	std::ofstream GetFile(const char*fileName);
private:
	akdzlib::zipper zipArchive;
	akdzlib::unzipper unzipArchive;
	bool _isArchiveOpen;

	static bool fileExist(const std::string& name);
};

