#ifndef KUMIRSTDLIB_H
#define KUMIRSTDLIB_H
#include "kstdlib_dll.h"

#include <assert.h>
#include <sstream>
#include <set>
#include <vector>
#include <list>
#include <deque>
#include <map>
#include <cstdio>
#include "encodings.hpp"


#include <wchar.h>
#include <string>
#include <math.h>
#include <float.h>

#if defined(WIN32) || defined(_WIN32)
#   include <Windows.h>
#else
#   include <sys/time.h>
#   include <sys/stat.h>
#   include <fcntl.h>
#   include <unistd.h>
#   include <string.h>
#   include <errno.h>
#endif
#include <stdlib.h>


namespace VM
{
class Variable;
}

namespace Kumir
{

typedef wchar_t Char;
typedef double real;
typedef std::wstring String;

inline String &operator+(String &s1, const /*utf8*/ char *s2)
{
	EncodingError encodingError;
	String us2 = Coder::decode(UTF8, std::string(s2), encodingError);
	s1.append(us2);
	return s1;
}


struct FileType {
	enum OpenMode { NotOpen, Read, Write, Append };
	enum SpecialType { RegularFile, Console };
	static const char *_()
	{
		return "siibbl";
	}
	FileType()
	{
		valid = false;
		mode = NotOpen;
		type = RegularFile;
		autoClose = false;
		handle = 0;
	}
	void setName(const String &name)
	{
		fullPath = name;
	}
	String getName() const
	{
		return fullPath;
	}
	void setMode(OpenMode m)
	{
		mode = int(m);
	}
	OpenMode getMode() const
	{
		return OpenMode(mode);
	}
	SpecialType getType() const
	{
		return SpecialType(type);
	}
	void setType(SpecialType t)
	{
		type = int(t);
	}
	bool isValid() const
	{
		return valid;
	}
	void invalidate()
	{
		valid = false;
		handle = 0;
	}
	bool operator==(const FileType &other) const
	{
		return other.fullPath == fullPath;
	}
	bool operator!=(const FileType &other) const
	{
		return other.fullPath != fullPath;
	}

	String fullPath;
	int mode;
	int type;
	bool valid;
	bool autoClose;
	FILE *handle;
};

class AbstractInputBuffer
{
public:
	virtual bool readRawChar(Char &ch) = 0;
	virtual void pushLastCharBack() = 0;
	virtual void clear() = 0;
};

class AbstractOutputBuffer
{
public:
	virtual void writeRawString(const String &) = 0;
};

class KS_DLL Core
{
	friend class Math;
	friend class Random;
	friend class Files;
	friend class StringUtils;
	friend class System;
	friend class IO;
	friend class VM::Variable;

	static String &takeError();
	static void unsetError() { takeError().clear(); }

public:
	static const String &getError();

	static void init()
	{
		unsetError();
	}

	static void finalize() {}

	static String fromUtf8(const std::string &s);
	static String fromAscii(const std::string &s);

	static std::string toLowerCase(const std::string &s);
	static std::string toUpperCase(const std::string &s);

	static std::wstring toLowerCaseW(const std::wstring &s);
	static std::wstring toUpperCaseW(const std::wstring &s);

	static String joinStrings(const std::deque<String> &sl, Char sep);
	static std::deque<String> splitString(
		const String &s,
		const Char sep,
		bool skipEmptyParts
	);

	static Encoding getSystemEncoding();

	typedef void (*AbortHandlerType)();

	static AbortHandlerType getAbortHandler();
	static void setAbortHandler(AbortHandlerType ah);
	static void abort(const String &e);

};


class StringList: public std::deque<String>
{
public:
	StringList() {}
	StringList(const std::deque<String> &d) : std::deque<String> (d) {}
	String join(Char sep) const
	{
		return Core::joinStrings(*this, sep);
	}
};


class Math
{
public:
	static void init() {}
	static void finalize() {}

	// Check for integer overflow
	// CLang completely removes the standard check technique due to optimization.
	// Use another method: cast to 64 bit
	static bool checkSumm(int32_t l, int32_t r)
	{
		int64_t res = (int64_t) l + (int64_t) r;
		bool result = (INT32_MIN <= res && res <= INT32_MAX);
		return result;
	}

	static bool checkDiff(int32_t l, int32_t r)
	{
		int64_t res = (int64_t) l - (int64_t) r;
		bool result = (INT32_MIN <= res && res <= INT32_MAX);
		return result;
	}

	static bool checkProd(int32_t l, int32_t r)
	{
		int64_t res = (int64_t) l * (int64_t) r;
		bool result = (INT32_MIN <= res && res <= INT32_MAX);
		return result;
	}

	static bool isCorrectDouble(double x)
	{
		// !!! WARNING !!
		// this works ONLY for IEEE754-compatible representation!
		union DoubleU64 {
			double d;
			uint64_t u;
		} v;
		v.d = x;
		uint32_t expMask  = 0x7FF;
		uint32_t exponent = (v.u >> 52) & expMask;
		return exponent != expMask;
	}

	static bool isCorrectReal(real val)
	{
		return isCorrectDouble(val);
	}

	static real abs(real x)
	{
		return ::fabs(x);
	}

	static int imax(int x, int y)
	{
		return x > y ? x : y;
	}
	static int imin(int x, int y)
	{
		return x < y ? x : y;
	}

	static real rmax(real x, real y)
	{
		return x > y ? x : y;
	}
	static real rmin(real x, real y)
	{
		return x < y ? x : y;
	}

	static int iabs(int x)
	{
		unsigned int y = (unsigned int) x;
		if (y == 0x80000000U) {
			Core::abort(L"Целочисленное переполнение");
			return 0;
		}

		return x >= 0 ? x : -x;
	}

	static int intt(real x)
	{
		double y = ::floor((double) x);
		if (!(INT32_MIN <= y && y <= INT32_MAX)) {
			Core::abort(L"Целочисленное переполнение");
			return 0;
		}
		return (int) y;
	}

	static real arccos(real x)
	{
		if (x >= -1.0 && x <= 1.0) {
			return ::acos(x);
		} else {
			Core::abort(Core::fromUtf8("Неверный аргумент тригонометрической функции"));
			return 0.0;
		}
	}
	static real arcctg(real x)
	{
		return ::atan(1.0 / x);
	}
	static real arcsin(real x)
	{
		if (x >= -1.0 && x <= 1.0) {
			return ::asin(x);
		} else {
			Core::abort(Core::fromUtf8("Неверный аргумент тригонометрической функции"));
			return 0.0;
		}
	}
	static real arctg(real x)
	{
		return ::atan(x);
	}

	static real cos(real x)
	{
		return ::cos(x);
	}
	static real sin(real x)
	{
		return ::sin(x);
	}
	static real tg(real x)
	{
		return ::tan(x);
	}
	static real ctg(real x)
	{
		if (x == 0.0) {
			Core::abort(Core::fromUtf8("Неверный аргумент тригонометрической функции"));
			return 0.0;
		} else {
			return 1.0 /::tan(x);
		}
	}
	static real lg(real x)
	{
		if (x > 0.0) {
			real num = ::log(x);
			real den = ::log((real)10.0);
			real result = num / den;
			return result;
		} else {
			Core::abort(Core::fromUtf8("Логарифм от не положительного числа"));
			return 0.0;
		}
	}
	static real ln(real x)
	{
		if (x > 0.0) {
			return ::log(x);
		} else {
			Core::abort(Core::fromUtf8("Логарифм от не положительного числа"));
			return 0.0;
		}
	}
	static real exp(real x)
	{
		return ::exp(x);
	}

	static real sqrt(real x)
	{
		if (x >= 0.0) {
			return ::sqrt(x);
		} else {
			Core::abort(Core::fromUtf8("Нельзя извлечь квадратный корень из отрицательного числа"));
			return 0.0;
		}
	}
	static real pow(real a, real b)
	{
		real result = ::pow(a, b);
		if (!Math::isCorrectReal(result)) {
			Core::abort(Core::fromUtf8("Ошибка возведения в степень: результат - слишком большое число"));
			return 0.0;
		}
		return result;
	}

	static int ipow(int a, int b)
	{
		static const String e_unwhole = L"Результат - не целое число";
		static const String e_ovfl = L"Целочисленное переполнение";
		static const String e_divz = L"Деление на ноль";

		if (b <= 1) {
			if (b == 1) {
				return a;
			}
			if (b == 0) {
				return 1;
			}
			if (a != 1 && a != -1) {
				Core::abort(a == 0 ? e_divz : e_unwhole);
				return 0;
			}
		}

		if (a == 0 || a == 1) {
			return a;
		}

		if (a == -1) {
			return (b & 1) ? -1 : 1;
		}

		if (a == 2) {
			if (31 <= b) {
				Core::abort(e_ovfl);
				return 0;
			}
			return 1 << b;
		}

		if (a == -2) {
			if (31 < b) {
				Core::abort(e_ovfl);
				return 0;
			}
			unsigned int res = 1U << b;
			return (b & 1) ? -res : res;
		}

		int64_t y = a, res = 1;
		bool ok = true;
		while (0 < b && ok) {
			if (b & 1) {
				res *= y;
				ok = ok && (INT32_MIN <= res && res <= INT32_MAX);
			}
			b /= 2;
			if (b == 0) {
				break;
			}
			y *= y;
			ok = ok && (INT32_MIN <= y && y <= INT32_MAX);
		}

		if (!ok) {
			Core::abort(e_ovfl);
			return 0;
		}

		return (int) res;
	}

	static real maxreal()
	{
		return 1.7976931348623157e+308;
	}

	static int div(int a, int b)
	{
		if (b <= 0) {
			Core::abort(L"Деление на ненатуральное число");
			return 0;
		}

		int q = a / b, r = a % b;
		if (r < 0) {
			q--;
		}
		return q;

	}

	static int mod(int a, int b)
	{
		if (b <= 0) {
			Core::abort(L"Деление на ненатуральное число");
			return 0;
		}

		int r = a % b;
		if (r < 0) {
			r += b;
		}
		return r;
	}

	template <typename T> static int sign(T x)
	{
		if (x > 0) {
			return 1;
		} else if (x < 0) {
			return -1;
		} else {
			return 0;
		}
	}
};

class Random
{
private:
	static unsigned int get_max();
	static unsigned int get_sample();

public:
	static void init();
	static void finalize() {}

	static int irand(int a, int b);
	static int irnd(int x);
	static real rrand(real a, real b);
	static real rrnd(real x);


#if 0
	inline static void init()
	{
#ifndef WIN32
		FILE *urandom = fopen("/dev/urandom", "rb");
		char buffer[sizeof(unsigned)];
		fread(buffer, 1u, sizeof(unsigned), urandom);
		fclose(urandom);
		unsigned seed;
		unsigned *seed_data_ptr = reinterpret_cast<unsigned *>(buffer);
		seed = *seed_data_ptr;
		srand(seed);
#endif
#if defined(WIN32) && defined(USE_MINGW_TOOLCHAIN)
		srand(time(0));
#endif
	}

#if !defined(WIN32) || defined(USE_MINGW_TOOLCHAIN)
	inline static int irand(int a, int b)
	{
		if (a > b) {
			Core::abort(Core::fromUtf8("Неверный диапазон чисел"));
			return 0;
		} else {

			unsigned int rndValue = rand(); // in range [0..2^32]
			unsigned int rd_max = RAND_MAX;
			real scale = static_cast<real>(b - a + 1) / static_cast<real>(rd_max);
			return Kumir::Math::imin(b, a + static_cast<int>(scale * rndValue));
		}
	}
	inline static int irnd(int x)
	{

		unsigned int rndValue = rand(); // in range [0..2^32]
		unsigned int rd_max = RAND_MAX;
		real scale = static_cast<real>(x) / static_cast<real>(rd_max);
		return Kumir::Math::imin(x, 1 + static_cast<int>(scale * rndValue));
	}

	inline static real rrand(real a, real b)
	{
		if (a > b) {
			Core::abort(Core::fromUtf8("Неверный диапазон чисел"));
			return 0.0;
		} else {

			unsigned int rndValue = rand(); // in range [0..2^32]
			unsigned int rd_max = RAND_MAX;
			real scale = static_cast<real>(b - a + 1) / static_cast<real>(rd_max);
			return Kumir::Math::rmin(b, a + static_cast<real>(scale * rndValue));
		}
	}
	inline static real rrnd(real x)
	{
		unsigned int rndValue = rand(); // in range [0..2^32]
		unsigned int rd_max = RAND_MAX;
		real scale = static_cast<real>(x) / static_cast<real>(rd_max);
		return Kumir::Math::rmin(x, static_cast<real>(scale * rndValue));
	}
#else
	inline static int irand(int a, int b)
	{
		if (a > b) {
			Core::abort(Core::fromUtf8("Неверный диапазон чисел"));
			return 0;
		} else {
			std::random_device rd;
			unsigned int rndValue = rd(); // in range [0..2^32]
#undef max
			unsigned int rd_max = rd.max();
			real scale = static_cast<real>(b - a + 1) / static_cast<real>(rd_max);
			return Kumir::Math::imin(b, a + static_cast<int>(scale * rndValue));
		}
	}
	inline static int irnd(int x)
	{
		std::random_device rd;
		unsigned int rndValue = rd();
#undef max
		unsigned int rd_max = rd.max();
		real scale = static_cast<real>(x) / static_cast<real>(rd_max);
		return Kumir::Math::imin(x, 1 + static_cast<int>(scale * rndValue));
	}

	inline static real rrand(real a, real b)
	{
		if (a > b) {
			Core::abort(Core::fromUtf8("Неверный диапазон чисел"));
			return 0.0;
		} else {
			std::random_device rd;
			unsigned int rndValue = rd(); // in range [0..2^32]
#undef max
			unsigned int rd_max = rd.max();
			real scale = static_cast<real>(b - a) / static_cast<real>(rd_max);
			return Kumir::Math::rmin(b, a + static_cast<real>(scale * rndValue));
		}
	}
	inline static real rrnd(real x)
	{
		std::random_device rd;
		unsigned int rndValue = rd();
#undef max
		unsigned int rd_max = rd.max();
		real scale = static_cast<real>(x) / static_cast<real>(rd_max);
		return Kumir::Math::rmin(x, static_cast<real>(scale * rndValue));
	}
#endif
#endif
};


//typedef std::basic_stringstream<Char> StringStream;

class Converter
{
public:

	enum ParseError {
		NoError,
		EmptyWord,
		WrongHex,
		WrongReal,
		WrongExpForm,
		BadSymbol,
		Overflow
	};

	inline static void init() {}
	inline static void finalize() {}

	static bool validDecimal(const String &astring)
	{
		static const String validSymbols = Core::fromAscii("0123456789");
		for (size_t i = 0; i < astring.length(); i++) {
			if (i == 0 && (astring[i] == '-' || astring[i] == '+')) {
				continue;
			}
			if (validSymbols.find(astring[i]) == String::npos) {
				return false;
			}
		}
		return true;
	}

	static real fromDecimal(const String &astring)
	{
		real result = 0;
		real power = 1;
		size_t index;
		real digit;
		static const String digits = Core::fromAscii("0123456789");
		for (int i = astring.length() - 1; i >= 0; i--) {
			if (i == 0 && astring[i] == '-') {
				result = -1 * result;
				break;
			}
			if (i == 0 && astring[i] == '+') {
				break;
			}
			index = digits.find(astring[i]);
			if (index == String::npos) {
				return 0.0;
			}
			digit = static_cast<real>(index);
			result += power * digit;
			power *= 10;
		}
		return result;
	}

	static int parseInt(String word, unsigned int base, ParseError &error)
	{
		error = NoError;
		size_t l = word.length(), pos = 0;

		if (l == 0) {
			error = EmptyWord;
			return 0;
		}

		bool negative = false;

		if (word.at(pos) == Char('-')) {
			negative = true;
			pos += 1;
		} else if (word.at(pos) == Char('+')) {
			pos += 1;
		}

		if (base == 0) {
			if (pos < l && word.at(pos) == Char('$')) {
				base = 16;
				pos += 1;
			} else {
				base = 10;
			}
		}
		assert(base);

		if (l == 0) {
			error = EmptyWord;
			return 0;
		}

		unsigned int maxabs = (1U << 31) - (negative ? 0 : 1);
		unsigned int maxabsb = maxabs / base;
		unsigned int result = 0;
		bool overflow = false;

		for (size_t i = pos; i < l; i++) {
			Char ch = word.at(i);
			unsigned int digit = base;

			if ('0' <= ch && ch <= '9') {
				digit = (unsigned int)(ch) - '0';
			} else if ('A' <= ch && ch <= 'Z') {
				digit = 10 + (unsigned int)(ch) - 'A';
			} else if ('a' <= ch && ch <= 'z') {
				digit = 10 + (unsigned int)(ch) - 'a';
			}

			if (base <= digit) {
				error = BadSymbol;
				return 0;
			}

			overflow = overflow || (maxabsb < result);
			result = result * base + digit;
			overflow = overflow || (result < digit);
		}

		overflow = overflow || (maxabs < result);

		if (overflow) {
			error = Overflow;
			return 0;
		}

		return negative ? -result : result;
	}

	static bool isCorrectIntegerConstant(const String &v)
	{
		ParseError e = NoError;
		parseInt(v, 0, e);
		return e == NoError;
	}

	static real parseReal(String word, Char dot, ParseError &error)
	{
		error = NoError;
		if (word.length() == 0) {
			error = EmptyWord;
			return 0;
		}
		size_t pos = 0;
		bool negative = false;
		if (word.at(pos) == Char('-')) {
			negative = true;
			pos += 1;
		} else if (word.at(pos) == Char('+')) {
			pos += 1;
		}
		real mantissa = 0.0;
		real exponenta = 0.0;
		real fraction = 0.0;
		String sIntegral, sFractional, sExponenta;
		sIntegral.reserve(30);
		sFractional.reserve(30);
		sExponenta.reserve(30);
		bool dotFound = false;
		bool eFound = false;
		static const String E = Core::fromUtf8("eEеЕ"); // includes cyrillic 'e'
		bool hasE = false;
		for (size_t i = 0; i < E.length(); i++) {
			if (!hasE) {
				hasE = word.find(E[i]) != String::npos;
			}
			if (hasE) {
				break;
			}
		}
		for (size_t i = pos; i < word.length(); i++) {
			Char ch = word.at(i);
			if (!dotFound) {
				// parse integral part of mantissa
				if (ch == dot || E.find_first_of(ch) != String::npos) {
					if (ch == dot) {
						dotFound = true;
					} else {
						dotFound = true;
						eFound = true;
					}

				} else {
					sIntegral.push_back(ch);
				}
			} else if (!eFound) {
				// parse fractional part of mantissa
				if (E.find_first_of(ch) != String::npos) {
					if (sFractional.length() > 0) {
						if (!validDecimal(sFractional)) {
							error = hasE ? WrongExpForm : WrongReal;
							return 0.0;
						}
						fraction = fromDecimal(sFractional);
					}
					if (fraction < 0) {
						error = hasE ? WrongExpForm : WrongReal;
						return 0.0;
					}
					eFound = true;
				} else {
					sFractional.push_back(ch);
				}
			} else {
				// parse exponenta
				sExponenta.push_back(ch);
			}
		}
		if (hasE && sExponenta.length() == 0) {
			error = WrongExpForm;
			return 0.0;
		}
		if (sIntegral.length() + sFractional.length() == 0) {
			error = sExponenta.length() > 0 ? WrongExpForm : WrongReal;
			return 0.0;
		}
		int fractionalLength = sFractional.length();
		for (int i = sFractional.length() - 1; i >= 0; i--) {
			Char ch = sFractional.at(i);
			if (ch == Char('0')) {
				fractionalLength -= 1;
			} else {
				break;
			}
		}
		if (!validDecimal(sIntegral) || !validDecimal(sFractional) || !validDecimal(sExponenta)) {
			error = WrongReal;
			return 0.0;
		}

		if (sExponenta.length() > 0 && sExponenta.find(Char('.')) != String::npos) {
			error = WrongExpForm;
			return 0.0;
		}

		fraction = fromDecimal(sFractional.substr(0, fractionalLength));
		for (int i = 0; i < fractionalLength; i++) {
			fraction /= 10.0;
		}
		mantissa = fromDecimal(sIntegral);
		if (mantissa < 0) {
			// Extra '-' at start
			error = WrongReal;
			return 0.0;
		}
		mantissa += fraction;
		exponenta = fromDecimal(sExponenta);
		real result = mantissa * ::pow(10, exponenta);
		if (negative && result != 0) {
			result *= -1;
		}
		if (!Math::isCorrectReal(result)) {
			error = Overflow;
		}
		return result;
	}

	static String sprintfInt(int value, char base, int width, char al)
	{
		String result;
		result.reserve(30);
		static const char *digits = "0123456789abcdefghijklmnopqrstuvwxyz";
		int q, r;
		bool negative = value < 0;
		if (int64_t(value) == -2147483648LL) {
			if (base == 10) {
				result = Core::fromAscii(std::string("-2147483648"));
			}
		} else {
			q = negative ? -value : value;
			if (q > 0) {
				while (q > 0) {
					r = q % base;
					result.insert(0, 1, digits[r]);
					q = q / base;
				}
			} else {
				result.insert(0, 1, '0');
			}
			if (base == 16) {
				result.insert(0, 1, '$');
			}
			if (negative) {
				result.insert(0, 1, '-');
			}
		}
		if (width > 0) {
			int leftSpaces = 0;
			int rightSpaces = 0;
			if (al == 'l') {
				rightSpaces = width - result.length();
			} else if (al == 'r') {
				leftSpaces = width - result.length();
			} else {
				leftSpaces = (width - result.length()) / 2;
				rightSpaces = width - result.length() - leftSpaces;
			}
			if (leftSpaces > 0) {
				result.insert(0, leftSpaces, ' ');
			}
			if (rightSpaces > 0) {
				result.append(rightSpaces, ' ');
			}
		}
		return result;
	}

	static String sprintfReal(
		real value, Char dot, bool expform,
		int width, int decimals, char al
	)
	{
		(void) dot;
		int sdecimals = decimals;
		if (sdecimals < 0) {
			double absVal = fabs(double(value));
			sdecimals = 13;
			if (0 < absVal && absVal <= DBL_MAX) {
				if (absVal < 1e-4 || absVal >= 1e6) {
					expform = true;
				}
				if (!expform) {
					sdecimals -= ::floor(::log(absVal) / ::log(10.0));
				}
			}
		}

		std::stringstream stream;
		if (expform) {
			stream << std::scientific;
		} else {
			stream << std::fixed;
		}
		stream.precision(sdecimals < 0 ? 6 : sdecimals);
		stream << value;

		std::string rpart = stream.str();
		std::string expPart;

		if (expform) {
			size_t ePos = rpart.find('e');
			if (std::string::npos == ePos) {
				ePos = rpart.find('E');
			}
			expPart = rpart.substr(ePos);
			rpart.resize(ePos);
		}

		// Replace ',' with '.' (for some locales like Russian)
		const size_t dotPos = rpart.find(',');
		if (std::string::npos != dotPos) {
			rpart.replace(dotPos, 1, ".");
		}
		if ((expform || 0 > decimals) && std::string::npos != rpart.find('.')) {
			while (rpart.size() > 1 && '0' == rpart.at(rpart.size() - 1)) {
				rpart.resize(rpart.length() - 1);
			}
			if ('.' == rpart.at(rpart.length() - 1)) {
				if (expform) {
					rpart.resize(rpart.length() - 1);
				} else {
					rpart.push_back('0');
				}
			}
		}
		std::string ascii = rpart + expPart;
		while (width > 0 && ascii.length() < static_cast<size_t>(width)) {
			if ('r' == al) {
				ascii = std::string(" ") + ascii;
			} else {
				ascii.push_back(' ');
			}
		}

		EncodingError encodingError;
		String result = Coder::decode(ASCII, ascii, encodingError);
		return result;
	}


	inline static int stringToInt(const String &str, bool &ok)
	{
		ParseError error = NoError;
		int result = parseInt(str, 0, error);
		ok = error == NoError;
		return result;
	}
	inline static real stringToReal(const String &str, bool &ok)
	{
		ParseError error = NoError;
		real result = parseReal(str, '.', error);
		ok = error == NoError;
		return result;
	}
	inline static int stringToIntDef(const String &str, int def)
	{
		bool ok;
		int val = stringToInt(str, ok);
		if (ok) {
			return val;
		} else {
			return def;
		}
	}
	inline static real stringToRealDef(const String &str, real def)
	{
		bool ok;
		real val = stringToReal(str, ok);
		if (ok) {
			return val;
		} else {
			return def;
		}
	}
	inline static bool stringToBoolDef(const String &str, bool def)
	{
		String v = Core::toLowerCaseW(str);
		static const String trues[5] = {
			Core::fromAscii("true"), Core::fromAscii("yes"),
			Core::fromAscii("1"), Core::fromUtf8("да"),
			Core::fromUtf8("истина")
		};
		static const String falses[5] = {
			Core::fromAscii("false"), Core::fromAscii("no"),
			Core::fromAscii("0"), Core::fromUtf8("нет"),
			Core::fromUtf8("ложь")
		};
		for (size_t i = 0; i < 5; i++) {
			const String &t = trues[i];
			const String &f = falses[i];
			if (v == t) {
				return true;
			}
			if (v == f) {
				return false;
			}
		}
		return def;
	}

	inline static String realToString(real value)
	{
		return sprintfReal(value, '.', false, 0, -1, 'l');
	}
	inline static String intToString(int value)
	{
		return sprintfInt(value, 10, 0, 'l');
	}


};

class StringUtils
{
public:
	inline static void init() {}
	inline static void finalize() {}
	inline static int length(const String &str)
	{
		return str.length();
	}

	inline static int code(Char ch)
	{
		unsigned char value = 0;
		EncodingError error;
		value = CP1251CodingTable::enc(ch, error);
		if (error) {
			if (OutOfTable == error) {
				Core::abort(Core::fromUtf8("Символ вне кодировки CP-1251"));
			} else {
				Core::abort(Core::fromUtf8("Ошибка кодирования символа"));
			}
		}
		return static_cast<int>(value);
	}

	inline static Char symbol(int code)
	{
		if (code < 0 || code > 255) {
			Core::abort(Core::fromUtf8("Код вне диапазона [0..255]"));
			return L'\0';
		} else {
			char buf[2] = { static_cast<char>(code), '\0' };
			charptr p = reinterpret_cast<charptr>(&buf);
			EncodingError encodingError;
//            uint32_t val = KOI8RCodingTable::dec(p, encodingError);
			uint32_t val = CP1251CodingTable::dec(p, encodingError);
			if (OutOfTable == encodingError) {
				Core::abort(Core::fromUtf8("Символ с таким кодом не определен в кодировке CP-1251"));
			}
			return static_cast<wchar_t>(val);
		}
	}

	inline static int unicode(Char ch)
	{
		return static_cast<int>(ch);
	}
	inline static Char unisymbol(int code)
	{
		if (code < 0 || code > 65535) {
			Core::abort(Core::fromUtf8("Код вне диапазона [0..65535]"));
			return L'\0';
		} else {
			return static_cast<wchar_t>(code);
		}
	}

	template <class S, typename C>
	inline static S &trim(S &s)
	{
		int nonSpacePos = 0;
		for (; nonSpacePos < (int)s.length(); nonSpacePos++) {
			if (s[nonSpacePos] != C(' ') &&
				s[nonSpacePos] != C('\t') &&
				s[nonSpacePos] != C('\n')
			) {
				break;
			}
		}
		if (nonSpacePos > 0) {
			s.erase(0, nonSpacePos);
		}
		nonSpacePos = s.length() - 1;
		for (; nonSpacePos >= 0; nonSpacePos--) {
			if (s[nonSpacePos] != C(' ') &&
				s[nonSpacePos] != C('\t') &&
				s[nonSpacePos] != C('\n')
			) {
				break;
			}
		}
		if (s.length() - (nonSpacePos + 1) > 0) {
			s.erase(nonSpacePos + 1);
		}
		return s;
	}

	inline static String toLowerCase(const String &s)
	{
		return Core::toLowerCaseW(s);
	}

	inline static String toUpperCase(const String &s)
	{
		return Core::toUpperCaseW(s);
	}

	inline static int find(int from, const String &substr, const String &s)
	{
		if (from < 1) {
			Core::abort(Core::fromUtf8("Индекс меньше 1"));
			return 0;
		}
		size_t start = static_cast<size_t>(from - 1);
		size_t pos = s.find(substr, start);
		if (pos == String::npos) {
			return 0;
		} else {
			return static_cast<int>(pos + 1);
		}
	}

	inline static int find(const String &substr, const String &s)
	{
		return find(1, substr, s);
	}

	inline static void insert(const String &substr, String &s, int pos)
	{
		if (pos < 1) {
			Core::abort(Core::fromUtf8("Индекс меньше 1"));
		} else if (pos - 1 >= (int)(s.length())) {
			s.append(substr);
		} else {
			size_t spos = static_cast<size_t>(pos - 1);
			s.insert(spos, substr);
		}
	}

	inline static void replace(String &s, const String &oldSubstr, const String &newSubstr, bool all)
	{
		size_t pos = 0;
		while (true) {
			pos = s.find(oldSubstr, pos);
			if (pos == String::npos) {
				break;
			}
			s.replace(pos, oldSubstr.length(), newSubstr);
			if (all) {
				pos += newSubstr.length();
			} else {
				break;
			}
		}
	}

	inline static void remove(String &s, int pos, int count)
	{
		if (pos < 1) {
			Core::abort(Core::fromUtf8("Индекс меньше 1"));
			return;
		}
		if (count < 0) {
			Core::abort(Core::fromUtf8("Количество удаляемый символов меньше 0"));
			return;
		}
		if (count == 0) {
			return;
		}
		if (pos - 1 + count > (int)(s.length())) {
			s.resize(static_cast<size_t>(pos - 1));
		} else {
			s.replace(static_cast<size_t>(pos - 1), static_cast<size_t>(count), String());
		}
	}
};

class KS_DLL Files
{
	friend class IO;
	static bool isOpenedFiles();

public:
	AbstractInputBuffer *getConsoleInputBuffer();
	static void setConsoleInputBuffer(AbstractInputBuffer *b);

	AbstractOutputBuffer *getConsoleOutputBuffer();
	static void setConsoleOutputBuffer(AbstractOutputBuffer *b);

	static void init();
	static void finalize();

	static void setFileEncoding(const String &enc);

	static String getAbsolutePath(const String &fileName);
	static String CurrentWorkingDirectory();


	static String getNormalizedPath(const String &path, Char sep);
	static bool canOpenForRead(const String &path);
	static bool canOpenForWrite(const String &path);


	static bool exist(const String &fileName);
	static bool isDirectory(const String &fileName);
	static bool unlink(const String &fileName);
	static bool rmdir(const String &fileName);
	static bool mkdir(const String &fileName);

	static FileType getConsoleBuffer();

	static FileType open(
		const String &shortName,
		FileType::OpenMode mode,
		bool remember,
		FILE **fh
	);

	static void close(const FileType &key);

	static void reset(FileType &key);
	static bool eof(const FileType &key);
	static bool hasData(const FileType &key);

	static bool overloadedStdIn();
	static bool overloadedStdOut();

	static FILE *getAssignedIn();
	static FILE *getAssignedOut();

	static void assignInStream (String fileName);
	static void assignOutStream(String fileName);
};


class KS_DLL IO
{
public:
	enum StreamType { File, InternalBuffer, ExternalBuffer };

	class OutputStream
	{
	public:
		OutputStream()
		{
			streamType_ = InternalBuffer;
			file = 0;
			encoding = UTF8;
			buffer.reserve(100);
			externalBuffer_ = 0;
		}

		OutputStream(FILE *f, Encoding enc)
		{
			streamType_ = File;
			file = f;
			encoding = enc;
			if (encoding == DefaultEncoding) {
				encoding = UTF8;
			}
			externalBuffer_ = 0;
		}

		OutputStream(AbstractOutputBuffer *buffer)
		{
			streamType_ = ExternalBuffer;
			file = 0;
			encoding = UTF8;
			externalBuffer_ = buffer;
		}

		StreamType type() const { return streamType_; }
		const String &getBuffer() const { return buffer; }

		void writeRawString(const String &s);

	private:
		StreamType streamType_;
		FILE *file;
		Encoding encoding;
		String buffer;
		AbstractOutputBuffer *externalBuffer_;
	};


	class InputStream
	{
	public:
		InputStream()
		{
			streamType_ = InternalBuffer;
			file_ = 0;
			encoding_ = UTF8;
			errStart_ = 0;
			errLength_ = 0;
			currentPosition_ = 0;
			externalBuffer_ = 0;
		}

		InputStream(const String &b)
		{
			streamType_ = InternalBuffer;
			file_ = 0;
			encoding_ = UTF8;
			errStart_ = 0;
			errLength_ = 0;
			buffer_ = b;
			currentPosition_ = 0;
			externalBuffer_ = 0;
		}

		InputStream(AbstractInputBuffer *buffer)
		{
			streamType_ = ExternalBuffer;
			file_ = 0;
			encoding_ = UTF8;
			errStart_ = 0;
			errLength_ = 0;
			currentPosition_ = 0;
			externalBuffer_ = buffer;
		}

		InputStream(FILE *f, Encoding enc);

		StreamType type() const { return streamType_; }
		int currentPosition() const { return currentPosition_; }

		void getError(String &text, int &start, int &len) const
		{
			text = error_;
			start = errStart_;
			len = errLength_;
		}

		void setError(const String &err)
		{
			if (type() != InternalBuffer) {
				Core::abort(err);
			} else {
				error_ = err;
			}
		}

		bool hasError() const
		{
			if (type() != InternalBuffer) {
				return Core::getError().length() > 0;
			} else {
				return error_.length() > 0;
			}
		}

		void markPossibleErrorStart()
		{
			errStart_ = currentPosition_;
			errLength_ = 0;
			error_.clear();
		}

		bool readRawChar(Char &x);
		void pushLastCharBack();
		String readUntil(const String &delimeters);
		void skipDelimiters(const String &delim);

	private:
		StreamType streamType_;
		FILE *file_;
		long fileSize_;
		Encoding encoding_;
		String buffer_;
		String error_;
		int errStart_;
		int errLength_;
		int currentPosition_;
		char lastCharBuffer_[3];
		AbstractInputBuffer *externalBuffer_;
	}; // end inner class InputStream


private:
	// Format parsing function
	static StringList splitIntoLexemsByDelimeter(
		const String &s,
		Char delim = Char(':')
	);

	static String readWord(InputStream &is);

public:

	// Generic functions to be in use while input from GUI
	static String readString(InputStream &is);
	static String readLine(InputStream &is);
	static Char readChar(InputStream &is);
	static int readInteger(InputStream &is);
	static real readReal(InputStream &is);
	static bool readBool(InputStream &is);

	static void writeString(OutputStream &os, const String &str, int width);
	static void writeChar(OutputStream &os, const Char &chr, int width);
	static void writeInteger(OutputStream &os, int value, int width);
	static void writeReal(OutputStream &os, real value, int width, int decimals);
	static void writeBool(OutputStream &os, bool value, int width);

	// Actual functions to be in use while input from stream

	static void setLocaleEncoding(Encoding enc);
	static InputStream makeInputStream(FileType fileNo, bool fromStdIn);
	static OutputStream makeOutputStream(FileType fileNo, bool toStdOut);

	static String readString(FileType fileNo = FileType(), bool fromStdIn = true);
	static String readLine(FileType fileNo = FileType(), bool fromStdIn = true);
	static Char readChar(FileType fileNo = FileType(), bool fromStdIn = true);
	static int readInteger(FileType fileNo = FileType(), bool fromStdIn = true);
	static real readReal(FileType fileNo = FileType(), bool fromStdIn = true);
	static bool readBool(FileType fileNo = FileType(), bool fromStdIn = true);

	static void writeString(int width, const String &value, FileType fileNo = FileType(), bool toStdOut = true);
	static void writeChar(int width, Char value, FileType fileNo = FileType(), bool toStdOut = true);
	static void writeInteger(int width, int value, FileType fileNo = FileType(), bool toStdOut = true);
	static void writeReal(int width, int decimals, real value, FileType fileNo = FileType(), bool toStdOut = true);
	static void writeBool(int width, bool value, FileType fileNo = FileType(), bool toStdOut = true);

}; // end class IO


class System
{
public:
	static void init() {}
	static void finalize() {}

	static int time();
};

inline void initStandardLibrary()
{
	Core::init();
	Math::init();
	Random::init();
	Converter::init();
	StringUtils::init();
	Files::init();
	System::init();
}

inline void finalizeStandardLibrary()
{
	Core::finalize();
	Math::finalize();
	Random::finalize();
	Converter::finalize();
	StringUtils::finalize();
	Files::finalize();
	System::finalize();
}


}

#endif // KUMIRSTDLIB_H
