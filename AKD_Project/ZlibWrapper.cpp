#include "ZlibWrapper.h"

ZlibWrapper::ZlibWrapper()
{
	_isArchiveOpen = false;
}

ZlibWrapper::~ZlibWrapper()
{
}

void ZlibWrapper::Open(char* archiveName)
{
	if (fileExist(archiveName))
		unzipArchive.open(archiveName);
	else
		zipArchive.open(archiveName);

	_isArchiveOpen = true;
}

void ZlibWrapper::Open(char* archiveName, bool forceNew)
{
	if (forceNew)
	{
		zipArchive.open(archiveName);
		_isArchiveOpen = true;
	}
	else
		Open(archiveName);
}

void ZlibWrapper::Close()
{
	zipArchive.close();
	unzipArchive.close();
	_isArchiveOpen = false;
}

std::vector<std::string> ZlibWrapper::ListContents()
{
	return unzipArchive.getFilenames();
}

unz_file_info64 ZlibWrapper::GetHeader(const char* fileName)
{
	unzipArchive.openEntry(fileName);
	return unzipArchive.getEntryHeader();
}

void ZlibWrapper::AddFile(const char* fileName, bool bz2Compression, int compressionLevel)
{
	std::ifstream file;
	file.open(fileName);
	zipArchive.addEntry(fileName, bz2Compression, compressionLevel);
	zipArchive << file;
	zipArchive.closeEntry();
	file.close();
}

char* ZlibWrapper::GetFile(const char* fileName)
{
	unzipArchive.openEntry(fileName);
	std::ofstream file;
	auto ptr = unzipArchive.getContent();
	return ptr;
}

bool ZlibWrapper::fileExist(const std::string& name)
{
	struct stat buffer;
	return (stat(name.c_str(), &buffer) == 0);
}
