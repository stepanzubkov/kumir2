#include "kumirstdlib.hpp"


namespace Kumir
{
void dummy() {}

String Core::error = String();
void (*Core::AbortHandler)() = 0;
std::deque<FileType> Files::openedFiles;
AbstractInputBuffer *Files::consoleInputBuffer = 0;
AbstractOutputBuffer *Files::consoleOutputBuffer = 0;
AbstractOutputBuffer *Files::consoleErrorBuffer = 0;
FILE *Files::assignedIN = stdin;
FILE *Files::assignedOUT = stdout;
#if defined(WIN32) || defined(_WIN32)
Encoding IO::LOCALE_ENCODING = CP866;
#else
Encoding IO::LOCALE_ENCODING = UTF8;
#endif
Encoding Files::fileEncoding;
String Kumir::IO::inputDelimeters = Kumir::Core::fromAscii(" \n\t");

} // namespace Kumir
