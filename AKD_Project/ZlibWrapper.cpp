#include "ZlibWrapper.h"

ZlibWrapper::ZlibWrapper()
{
	_isArchiveOpen = false;
}

ZlibWrapper::~ZlibWrapper()
{
}

void ZlibWrapper::Open(std::string archiveName)
{
	if (fileExist(archiveName))
	{
		unzipArchive.open(archiveName.c_str());
		zipArchive.open(archiveName.c_str(),true);
	}
	else
		zipArchive.open(archiveName.c_str());

	_isArchiveOpen = true;
}

void ZlibWrapper::Open(std::string archiveName, bool forceNew)
{
	if (forceNew)
	{
		zipArchive.open(archiveName.c_str());
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

void ZlibWrapper::Create(std::string archiveName)
{
	Open(archiveName, true);
}

const std::vector<std::string>& ZlibWrapper::ListContents()
{
	if (!unzipArchive.isOpen())
		throw new ZipNotOpenException("Nie znaleziono otwartego archiwum.");
	return unzipArchive.getFilenames();
}

unz_file_info64 ZlibWrapper::GetHeader(std::string fileName)
{
	if (!unzipArchive.isOpen())
		throw new ZipNotOpenException("Nie znaleziono otwartego archiwum.");
	throwCustomException(unzipArchive.openEntry(fileName.c_str()));
	return unzipArchive.getEntryHeader();
}

void ZlibWrapper::AddFile(std::string fileName, bool bz2Compression, int compressionLevel)
{
	if(!zipArchive.isOpen())
		throw new ZipNotOpenException("Nie znaleziono otwartego archiwum.");
	std::ifstream file;
	file.open(fileName);
	throwCustomException(zipArchive.addEntry(fileName.c_str(), bz2Compression, compressionLevel));
	zipArchive << file;
	zipArchive.closeEntry();
	file.close();
}

void ZlibWrapper::AddRawFile(std::string archivePath, std::string fileName, std::vector<char>& data, bool bz2Compression, unz_file_info64 fi)
{
	zipArchive.open(archivePath.c_str(), true);
	if (!zipArchive.isOpen())
		zipArchive.open(archivePath.c_str(), false);
	zipArchive.addRawEntry(fileName.c_str(), bz2Compression);
	zipArchive.writeRawData(data.data(), data.size(),fi.uncompressed_size,fi.crc);
	zipArchive.closeEntry();
}

void ZlibWrapper::AddRawFile(std::string archivePath, std::string fileName, std::string fromFile, bool bz2Compression, unz_file_info64 fi)
{
	zipArchive.open(archivePath.c_str(), true);
	if (!zipArchive.isOpen())
		zipArchive.open(archivePath.c_str(), false);
	zipArchive.addRawEntry(fileName.c_str(), bz2Compression);
	zipArchive.writeRawData(fromFile, fi.uncompressed_size, fi.crc);
	zipArchive.closeEntry();
}

std::vector<char> ZlibWrapper::GetFile(std::string fileName)
{
	if (!unzipArchive.isOpen())
		throw new ZipNotOpenException("Nie znaleziono otwartego archiwum.");
	throwCustomException(unzipArchive.openEntry(fileName.c_str(), false));
	auto ptr = unzipArchive.getContent(false);
	return ptr;
}

std::vector<char> ZlibWrapper::GetRawFile(std::string fileName)
{
	if (!unzipArchive.isOpen())
		throw new ZipNotOpenException("Nie znaleziono otwartego archiwum.");
	throwCustomException(unzipArchive.openEntry(fileName.c_str(), true));
	auto ptr = unzipArchive.getContent(true);
	return ptr;
}

void ZlibWrapper::GetFile(std::string fileName, std::string saveToFile)
{
	if (!unzipArchive.isOpen())
		throw new ZipNotOpenException("Nie znaleziono otwartego archiwum.");
	throwCustomException(unzipArchive.openEntry(fileName.c_str(), false));
	std::ofstream file(saveToFile,std::ios_base::out | std::ios_base::binary);
	unzipArchive.isFileRaw = false;
	unzipArchive >> file;
}

void ZlibWrapper::GetRawFile(std::string fileName, std::string saveToFile)
{
	if (!unzipArchive.isOpen())
		throw new ZipNotOpenException("Nie znaleziono otwartego archiwum.");
	throwCustomException(unzipArchive.openEntry(fileName.c_str(), true));
	std::ofstream file(saveToFile, std::ios_base::out | std::ios_base::binary);
	unzipArchive.isFileRaw = true;
	unzipArchive >> file;
}

void ZlibWrapper::setProgressBarFunction(std::function<void(long long, long long)> fun)
{
	zipArchive.progressBar = fun;
	unzipArchive.progressBar = fun;
}

void ZlibWrapper::setBufferSize(long long size)
{
	unzipArchive.bufferSize = size;
	zipArchive.bufferSize = size;
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
