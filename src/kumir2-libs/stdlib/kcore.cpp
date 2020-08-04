#include "kumirstdlib.hpp"

namespace Kumir {

static String kumirError;
static Core::AbortHandlerType abortHandler = 0;

const String &Core::getError()
{
	return kumirError;
}

String &Core::takeError()
{
	return kumirError;
}

Core::AbortHandlerType Core::getAbortHandler()
{
	return abortHandler;
}

void Core::setAbortHandler(Core::AbortHandlerType ah)
{
	abortHandler = ah;
}


String Core::fromUtf8(const std::string &s)
{
	EncodingError encodingError;
	String result = Coder::decode(UTF8, s, encodingError);
	return result;
}

String Core::fromAscii(const std::string &s)
{
	EncodingError encodingError;
	String result = Coder::decode(ASCII, s, encodingError);
	return result;
}

std::string Core::toLowerCase(const std::string &s)
{
	// !!!! Only for ASCII in onebyte case !!!!!
	std::string result;
	result.reserve(s.length());
	for (size_t i = 0; i < s.length(); i++) {
		char ch = s[i];
		if (ch >= 'A' && ch <= 'Z') {
			ch += 32;
		}
		result.push_back(ch);
	}
	return result;
}

std::string Core::toUpperCase(const std::string &s)
{
	// !!!! Only for ASCII in onebyte case !!!!!
	std::string result;
	result.reserve(s.length());
	for (size_t i = 0; i < s.length(); i++) {
		char ch = s[i];
		if (ch >= 'a' && ch <= 'z') {
			ch -= 32;
		}
		result.push_back(ch);
	}
	return result;
}

std::wstring Core::toLowerCaseW(const std::wstring &s)
{
	std::wstring result;
	result.reserve(s.length());
	for (size_t i = 0; i < s.length(); i++) {
		wchar_t ch = s[i];
		if (ch >= L'A' && ch <= L'Z') {
			ch += 32;
		} else if (ch >= 0x0400 && ch <= 0x042F) {
			ch += 0x20;
		}
		result.push_back(ch);
	}
	return result;
}

std::wstring Core::toUpperCaseW(const std::wstring &s)
{
	std::wstring result;
	result.reserve(s.length());
	for (size_t i = 0; i < s.length(); i++) {
		wchar_t ch = s[i];
		if (ch >= L'a' && ch <= L'z') {
			ch -= 32;
		} else if (ch >= 0x0430 && ch <= 0x044F) {
			ch -= 0x20;
		}
		result.push_back(ch);
	}
	return result;
}

String Core::joinStrings(const std::deque<String> &sl, Char sep)
{
	String result;
	size_t resultSize = 0, l = sl.size();
	if (l == 0)
		return result;

	for (size_t i = 0; i < l; i++) {
		resultSize += sl.at(i).length();
	}

	resultSize += l - 1;
	result.reserve(resultSize);
	for (size_t i = 0; i < l; i++) {
		result.append(sl.at(i));
		if (i < l - 1) {
			result.push_back(sep);
		}
	}
	return result;
}

std::deque<String> Core::splitString(
	const String &s,
	const Char sep,
	bool skipEmptyParts
) {
	std::deque<String> result;
	size_t prev_index;
	prev_index = 0;
	while (true) {
		size_t cur_index = s.find(sep, prev_index);
		if (cur_index == s.npos) {
			cur_index = s.length();
		}
		size_t length = cur_index - prev_index;
		if (length == 0 && !skipEmptyParts) {
			result.push_back(String());
		} else if (length > 0) {
			result.push_back(s.substr(prev_index, length));
		}
		prev_index = cur_index + 1;
		if (prev_index >= s.length()) {
			break;
		}
	}
	return result;
}

Encoding Core::getSystemEncoding()
{
#if defined(WIN32) || defined(_WIN32)
		return CP1251;
#elif defined(DOS)
		return CP866;
#else
	char *lc = 0;
	lc = getenv("LC_CTYPE");
	if (!lc) {
		lc = getenv("LC_ALL");
	}
	if (!lc) {
		return UTF8;
	}

	std::deque<String> parts = splitString(
		fromAscii(std::string(lc)),
		Char('.'),
		true
	);

	if (parts.size() == 0) {
		return UTF8;
	}

	EncodingError encodingError;
	std::string last = Coder::encode(
		ASCII,
		parts.at(parts.size() - 1),
		encodingError
	);

	if (last == std::string("KOI8-R")) {
		return KOI8R;
	} else if (last == std::string("CP866") ||
		last == std::string("IBM866") ||
		last == std::string("CP-866") ||
		last == std::string("IBM-866")) {
		return CP866;
	} else if (last == std::string("CP1251") ||
		last == std::string("WINDOWS1251") ||
		last == std::string("CP-1251") ||
		last == std::string("WINDOWS-1251")) {
		return CP1251;
	} else {
		return UTF8;
	}
#endif
}


void Core::abort(const String &e)
{
	kumirError = e;
	if (abortHandler) {
		abortHandler();
	}
}


} // namespace Kumir

