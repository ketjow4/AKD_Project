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
	{
		unzipArchive.open(archiveName);
		zipArchive.open(archiveName,true);
	}
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

void ZlibWrapper::Create(char* archiveName)
{
	Open(archiveName, true);
}

std::vector<std::string> ZlibWrapper::ListContents()
{
	if (!unzipArchive.isOpen())
		throw new ZipNotOpenException("Nie znaleziono otwartego archiwum.");
	return unzipArchive.getFilenames();
}

unz_file_info64 ZlibWrapper::GetHeader(const char* fileName)
{
	if (!unzipArchive.isOpen())
		throw new ZipNotOpenException("Nie znaleziono otwartego archiwum.");
	throwCustomException(unzipArchive.openEntry(fileName));
	return unzipArchive.getEntryHeader();
}

void ZlibWrapper::AddFile(const char* fileName, bool bz2Compression, int compressionLevel)
{
	if(!zipArchive.isOpen())
		throw new ZipNotOpenException("Nie znaleziono otwartego archiwum.");
	std::ifstream file;
	file.open(fileName);
	throwCustomException(zipArchive.addEntry(fileName, bz2Compression, compressionLevel));
	zipArchive << file;
	zipArchive.closeEntry();
	file.close();
}

void ZlibWrapper::AddRawFile(const char* archivePath, const char* fileName, std::vector<char> data, bool bz2Compression, unz_file_info64 fi)
{
	zipArchive.open(archivePath, true);
	if (!zipArchive.isOpen())
		zipArchive.open(archivePath, false);
	zipArchive.addRawEntry(fileName, bz2Compression);
	zipArchive.writeRawData(data.data(), data.size(),fi.uncompressed_size,fi.crc);
	zipArchive.closeEntry();
}

std::vector<char> ZlibWrapper::GetFile(const char* fileName)
{
	if (!unzipArchive.isOpen())
		throw new ZipNotOpenException("Nie znaleziono otwartego archiwum.");
	throwCustomException(unzipArchive.openEntry(fileName, false));
	std::ofstream file;
	auto ptr = unzipArchive.getContent(false);
	return ptr;
}

std::vector<char> ZlibWrapper::GetRawFile(const char* fileName)
{
	if (!unzipArchive.isOpen())
		throw new ZipNotOpenException("Nie znaleziono otwartego archiwum.");
	throwCustomException(unzipArchive.openEntry(fileName, true));
	std::ofstream file;
	auto ptr = unzipArchive.getContent(true);
	return ptr;
}

void ZlibWrapper::setProgressBarFunction(std::function<void(long, long)> fun)
{
	zipArchive.progressBar = fun;
	unzipArchive.progressBar = fun;
}

bool ZlibWrapper::fileExist(const std::string& name)
{
	struct stat buffer;
	return (stat(name.c_str(), &buffer) == 0);
}

void ZlibWrapper::throwCustomException(int exceptionNumber)
{
	switch (exceptionNumber)
	{
	case ZIP_PARAMERROR:
		throw new ZipParameterException("B³êdny parametr wywo³ania.");
	case ZIP_BADZIPFILE:
		throw new ZipBadFileException("B³¹d odczytu pliku.");
	case ZIP_INTERNALERROR:
		throw new ZipInternalException("Wewnêtrzny b³¹d pliku .zip .");
	case UNZ_END_OF_LIST_OF_FILE:
		throw new ZipEndOfListofFileException("B³¹d odczytania listy plików.");
	case -1:
		throw new ZipNotOpenException("Nie znaleziono otwartego archiwum.");
	default:
		break;
	}
}
