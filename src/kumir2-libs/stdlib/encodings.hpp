#ifndef ENCODINGS_HPP
#define ENCODINGS_HPP

#include "kstdlib_dll.h"
#include <string>
#include <stdint.h>

namespace Kumir
{

enum Encoding {
	DefaultEncoding, ASCII, UTF8, UTF16INTEL,
	UTF16MOTOROLA, CP866, CP1251, KOI8R
};

enum EncodingError { NoEncodingError = 0, OutOfTable, StreamEnded };

typedef const char *charptr;

class AsciiCodingTable
{
public:
	static unsigned char enc(uint32_t symb, EncodingError &error);
	static uint32_t dec(charptr &from, EncodingError &error);
};

class CP866CodingTable
{
public:
	static unsigned char enc(uint32_t symb, EncodingError &error);
	static uint32_t dec(charptr &from, EncodingError &error);
};

class CP1251CodingTable
{
public:
	static unsigned char enc(uint32_t symb, EncodingError &error);
	static uint32_t dec(charptr &from, EncodingError &error);
};

class KOI8RCodingTable
{
public:
	static unsigned char enc(uint32_t symb, EncodingError &error);
	static uint32_t dec(charptr &from, EncodingError &error);
};

struct MultiByte {
	unsigned char data[3];
	unsigned char size;
};

class UTF8CodingTable
{
public:
	static MultiByte enc(uint32_t k, EncodingError &error);
	static uint32_t dec(charptr &from, EncodingError &error);
};

class KS_DLL Coder
{
public:
	static std::string encode(
		Encoding E,
		const std::wstring &src,
		EncodingError &error
	);

	static std::wstring decode(
		Encoding E,
		const std::string &src,
		EncodingError &error
	);
};

} // namespace Kumir

#endif
