#pragma once
#include <exception>>

class ZipParameterException : public std::exception
{
public:
	explicit ZipParameterException(char* msg) : exception(msg){};
};

class ZipNotOpenException : public std::exception
{
public:
	explicit ZipNotOpenException(char* msg) : exception(msg) {};
};

class ZipBadFileException : public std::exception
{
public:
	explicit ZipBadFileException(char* msg) : exception(msg) {};
};

class ZipInternalException : public std::exception
{
public:
	explicit ZipInternalException(char* msg) : exception(msg) {};
};

class ZipEndOfListofFileException : public std::exception
{
public:
	explicit ZipEndOfListofFileException(char* msg) : exception(msg) {};
};
