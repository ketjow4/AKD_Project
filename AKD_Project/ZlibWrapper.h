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
	void Open(std::string archiveName);
	void Open(std::string archiveName, bool forceNew);
	void Close();
	void Create(std::string archiveName);
	const std::vector<std::string>& ListContents();
	unz_file_info64 GetHeader(std::string fileName);


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
	void AddFile(std::string fileName, bool bz2Compression, int compressionLevel);

	void AddRawFile(std::string archivePath, std::string fileName, std::vector<char>& data, bool bz2Compression, unz_file_info64 fi);

	void AddRawFile(std::string archivePath, std::string fileName, std::string fromFile, bool bz2Compression, unz_file_info64 fi);

	std::vector<char> GetFile(std::string fileName);

	std::vector<char> GetRawFile(std::string fileName);

	void GetFile(std::string fileName, std::string saveToFile);

	void GetRawFile(std::string fileName, std::string saveToFile);

	void setProgressBarFunction(std::function<void(long long, long long)> fun);

	void setBufferSize(long long size);

	
private:
	akdzlib::zipper zipArchive;
	akdzlib::unzipper unzipArchive;
	bool _isArchiveOpen;
	std::function<void(long long, long long)> progressBar;

	static bool fileExist(const std::string& name);
	static void throwCustomException(int exceptionNumber);
};

