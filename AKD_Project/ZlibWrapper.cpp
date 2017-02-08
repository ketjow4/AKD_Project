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

void ZlibWrapper::AddFile(const char* fileName)
{
	std::ifstream file;
	file.open(fileName);
	zipArchive.addEntry(fileName);
	zipArchive << file;
	zipArchive.closeEntry();
	file.close();
}

std::ofstream ZlibWrapper::GetFile(const char* fileName)
{
	unzipArchive.openEntry(fileName);
	std::ofstream file;
	unzipArchive >> file;
	return file;
}

bool ZlibWrapper::fileExist(const std::string& name)
{
	struct stat buffer;
	return (stat(name.c_str(), &buffer) == 0);
}
