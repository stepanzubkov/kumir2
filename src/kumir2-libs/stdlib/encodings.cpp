#include "encodings.hpp"

namespace Kumir
{

unsigned char AsciiCodingTable::enc(uint32_t symb, EncodingError &error)
{
	error = NoEncodingError;
	if (symb < 128) {
		return static_cast<unsigned char>(symb);
	} else {
		error = OutOfTable;
		return '?';
	}
}

uint32_t AsciiCodingTable::dec(charptr &from, EncodingError &error)
{
	error = NoEncodingError;
	if (from == 0 || (*from) == '\0') {
		return L'\0';
	}
	unsigned char k = static_cast<unsigned char>(*from++);
	if (k < 128) {
		return static_cast<uint32_t>(k);
	} else {
		error = OutOfTable;
		return L'?';
	}
}


unsigned char CP866CodingTable::enc(uint32_t symb, EncodingError &error)
{
	error = NoEncodingError;
	if (symb < 128) {
		return static_cast<unsigned char>(symb);
	}
	switch (symb) {
#include "wchar_cp866.table"
	default:
		error = OutOfTable;
		return '?';
	}
}

uint32_t CP866CodingTable::dec(charptr &from, EncodingError &error)
{
	error = NoEncodingError;
	if (from == 0 || (*from) == '\0') {
		return L'\0';
	}
	unsigned char k = static_cast<unsigned char>(*from++);
	if (k < 128) {
		return static_cast<uint32_t>(k);
	}
	switch (k) {
#include "cp866_wchar.table"
	default:
		error = OutOfTable;
		return L'?';
	}
}


unsigned char CP1251CodingTable::enc(uint32_t symb, EncodingError &error)
{
	error = NoEncodingError;
	if (symb < 128) {
		return static_cast<unsigned char>(symb);
	}

	switch (symb) {
#include "wchar_windows-1251.table"
	default:
		error = OutOfTable;
		return '?';
	}
}

uint32_t CP1251CodingTable::dec(charptr &from, EncodingError &error)
{
	error = NoEncodingError;
	if (from == 0 || (*from) == '\0') {
		return L'\0';
	}

	unsigned char k = static_cast<unsigned char>(*from++);
	if (k < 128) {
		return static_cast<uint32_t>(k);
	}
	switch (k) {
#include "windows-1251_wchar.table"
	default:
		error = OutOfTable;
		return L'?';
	}
}


unsigned char KOI8RCodingTable::enc(uint32_t symb, EncodingError &error)
{
	error = NoEncodingError;
	if (symb < 128) {
		return static_cast<unsigned char>(symb);
	}
	switch (symb) {
#include "wchar_koi8-r.table"
	default:
		error = OutOfTable;
		return '?';
	}
}

uint32_t KOI8RCodingTable::dec(charptr &from, EncodingError &error)
{
	error = NoEncodingError;
	if (from == 0 || (*from) == '\0') {
		return L'\0';
	}
	unsigned char k = static_cast<unsigned char>(*from++);
	if (k < 128) {
		return static_cast<uint32_t>(k);
	}
	switch (k) {
#include "koi8-r_wchar.table"
	default:
		error = OutOfTable;
		return L'?';
	}
}


MultiByte UTF8CodingTable::enc(uint32_t k, EncodingError &error)
{
	// for implementation details see:
	//   man utf-8
	error = NoEncodingError;
	uint32_t v = k;
	MultiByte result;
	if (v <= 0x7F) {
		// 0xxxxxxx
		result.size = 1;
		result.data[0] = v & 0x7F; // 0x7F = 01111111
	} else if (v <= 0x7FF) {
		// 110xxxxx,10xxxxxx
		result.size = 2;
		result.data[1] = v & 0x3F; // 0x3F = 00111111
		result.data[1] |= 0x80; // 0x80 = 10000000
		v = v >> 6;
		result.data[0] = v & 0x1F; // 0x1F = 00011111
		result.data[0] |= 0xC0; // 0xC0 = 11000000
	} else if (v <= 0xFFFF) {
		// 1110xxxx, 10xxxxxx, 10xxxxxx
		result.size = 3;
		result.data[2] = v & 0x3F; // 0x3F = 00111111
		result.data[2] |= 0x80; // 0x80 = 10000000
		v = v >> 6;
		result.data[1] = v & 0x3F; // 0x3F = 00111111
		result.data[1] |= 0x80; // 0x80 = 10000000
		v = v >> 6;
		result.data[0] = v & 0x0F; // 0x0F = 00001111
		result.data[0] |= 0xE0; // 0xE0 = 11100000
	} else {
		// Support only Unicode Plane 0 (Base) !!!
		result.size = 1;
		result.data[0] = '?';
		error = OutOfTable;
	}
	return result;
}

uint32_t UTF8CodingTable::dec(charptr &from, EncodingError &error)
{
	error = NoEncodingError;
	uint32_t v = 0;
	if (from == 0 || (*from) == '\0') {
		error = StreamEnded;
		return L'\0';
	}
	unsigned char byte = (*from++);
	unsigned char byte_first_3_bits = byte >> 5;
	unsigned char byte_first_4_bits = byte >> 4;
	if ((byte & 0x80) == 0) {
		// first byte mask: 0xxxxxxx
		// -- only one byte used
		v = byte & 0x7F; // 0x7F = 01111111;
	} else if (byte_first_3_bits == 0x06) {
		// first byte mask: 110xxxxx
		// -- use two bytes
		v = byte & 0x1F; // 0x1F = 000xxxxx
		byte = (*from++);
		if (byte == '\0') {
			error = StreamEnded;
			return L'?';
		}
		v = (v << 6) | (byte & 0x3F); // 0x3F = 00111111
	} else if (byte_first_4_bits == 0x0E) {
		// first byte mask: 1110xxxx
		// -- use three bytes
		v = byte & 0x0F; // 0x0F = 00001111
		byte = (*from++);
		if (byte == '\0') {
			error = StreamEnded;
			return L'?';
		}
		v = (v << 6) | (byte & 0x3F); // 0x3F = 00111111
		byte = (*from++);
		if (byte == '\0') {
			error = StreamEnded;
			return L'?';
		}
		v = (v << 6) | (byte & 0x3F); // 0x3F = 00111111
	} else {
		// Something going wrong:
		//   1) corrupted data; or
		//   2) symbol out of Basic Unicode
		//      (we don't support supplementary plane)
		error = OutOfTable;
		return L'?';
	}
	return v;
}


std::string Coder::encode(
	Encoding E,
	const std::wstring &src,
	EncodingError &error
) {
	error = NoEncodingError;
	std::string result;
	if (E != UTF8) {
		result.reserve(src.length());
		for (size_t i = 0; i < src.length(); i++) {
			char ch = '\0';
			if (E == CP866) {
				ch = CP866CodingTable::enc(src[i], error);
			} else if (E == CP1251) {
				ch = CP1251CodingTable::enc(src[i], error);
			} else if (E == KOI8R) {
				ch = KOI8RCodingTable::enc(src[i], error);
			} else if (E == ASCII) {
				ch = AsciiCodingTable::enc(src[i], error);
			}
			if (error) {
				return result;
			}
			result.push_back(ch);
		}
	} else {
		result.reserve(3 * src.length());
		for (size_t i = 0; i < src.length(); i++) {
			MultiByte mb = UTF8CodingTable::enc(src[i], error);
			if (error) {
				return result;
			}
			for (unsigned char j = 0; j < mb.size; j++) {
				result.push_back(mb.data[j]);
			}
		}
	}
	return result;
}

std::wstring Coder::decode(
	Encoding E,
	const std::string &src,
	EncodingError &error
) {
	error = NoEncodingError;
	std::wstring result;
	result.reserve(src.length());
	const char *p = src.c_str();
	wchar_t wch = L'\0';
	while (p != 0 && (*p) != '\0') {
		if (E == CP866) {
			wch = CP866CodingTable::dec(p, error);
		} else if (E == CP1251) {
			wch = CP1251CodingTable::dec(p, error);
		} else if (E == KOI8R) {
			wch = KOI8RCodingTable::dec(p, error);
		} else if (E == UTF8) {
			wch = UTF8CodingTable::dec(p, error);
		} else if (E == ASCII) {
			wch = AsciiCodingTable::dec(p, error);
		}
		if (error) {
			return result;
		}
		result.push_back(wch);
	}
	return result;
}


} // namespace Kumir

