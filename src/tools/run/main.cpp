#include <iostream>
#include <fstream>
#include <kumir2-libs/stdlib/kumirstdlib.hpp>
#include <kumir2-libs/vm/vm_abstract_handlers.h>
#include <kumir2-libs/vm/vm_console_handlers.hpp>
#include <kumir2-libs/vm/variant.hpp>
#include <kumir2-libs/vm/vm_bytecode.hpp>
#include <kumir2-libs/vm/vm.hpp>

#include <algorithm>

#if defined(WIN32) || defined(_WIN32)
#include <Windows.h>
#else
extern "C" {
//#include <unistd.h>
}
#endif

using namespace Kumir;

#if defined(WIN32) || defined(_WIN32)
static Encoding LOCALE = CP866;
#else
static Encoding LOCALE = UTF8;
#endif

static std::string toLocale(const String &s)
{
	return Core::toEncoding(LOCALE, s);
}

static String fromLocale(const std::string &s)
{
	return Core::fromEncoding(LOCALE, s);
}

static void do_output(const String &s)
{
	std::string ls = toLocale(s);
	std::cout << ls;
}

static void do_output(const std::string &s)
{
	do_output(Core::fromUtf8(s));
}

int usage(const char *programName)
{
	using namespace Kumir;

	String progName = fromLocale(std::string(programName));
	Char _n = Char('\n');
	bool russianLanguage = false;

#if defined(WIN32) || defined(_WIN32)
	russianLanguage = true;
#else
	char *env = getenv("LANG");
	if (env) {
		std::string LANG(env);
		russianLanguage = LANG.find("ru") != std::string::npos;
	}
#endif

	String message;

	if (russianLanguage) {
		message  = L"Вызов:";
		message.push_back(_n);
		message += L"\t" + progName;
		message += L" [-ansi] [-p] [--pipe] [-t] [--test] ИМЯФАЙЛА.kod [ПАРАМ1 [ПАРАМ2 ... [ПАРАМn]]]";
		message.push_back(_n);
		message.push_back(_n);
		message += L"\t-ansi\t\tИспользовать кодировку 1251 вместо 866 в терминале (только для Windows)";
		message.push_back(_n);
		message += L"\t-p | --pipe \tТихий режим";
		message.push_back(_n);
		message += L"\t-t | --test \tТестирование (запускает @тестирование вместо главного алгоритма";
		message.push_back(_n);
		message += L"\tИМЯФАЙЛА.kod\tИмя выполняемой программы";
		message.push_back(_n);
		message += L"\tПАРАМ1...ПАРАМn\tАргументы главного алгоритма Кумир-программы";
		message.push_back(_n);
		message += L"\t\t\tВ настоящее время не используются, парам-пам-пам ";
		message.push_back(_n);
	} else {
		message  = L"Usage:";
		message.push_back(_n);
		message += L"\t" + progName;
		message += L" [-ansi] [-p] [--pipe] [-t] [--test] FILENAME.kod [ARG1 [ARG2 ... [ARGn]]]";
		message.push_back(_n);
		message.push_back(_n);
		message += L"\t-ansi\t\tUse codepage 1251 instead of 866 in console (Windows only)";
		message.push_back(_n);
		message += L"\t-p | --pipe \tQuiet mode";
		message.push_back(_n);
		message += L"\t-t | --test \tTest mode (run @тестирование instead of the main algorithm";
		message.push_back(_n);
		message += L"\tFILENAME.kod\tKumir runtime file name";
		message.push_back(_n);
		message += L"\tARG1...ARGn\tKumir program main algorithm arguments";
		message.push_back(_n);
		message += L"\t\t\tOf no use currently";
		message.push_back(_n);
	}
	std::cerr << toLocale(message);
	return 127;
}

int showErrorMessage(const String &message, int code)
{
	bool toHttp = false;
#if !defined(WIN32) && !defined(_WIN32)
	char *REQUEST_METHOD = getenv("REQUEST_METHOD");
	char *QUERY_STRING = getenv("QUERY_STRING");
	toHttp = (REQUEST_METHOD != 0 && QUERY_STRING != 0);
#endif
	if (!toHttp) {
		std::string localMessage = toLocale(message);
		std::cerr << localMessage << std::endl;
		return code;
	} else {
		std::string localMessage = Core::toUtf8(message);
		std::cout << "Content-type: text/plain;charset=utf-8\n\n";
		std::cout << localMessage << std::endl;
		return 0;
	}
}

bool IsPluginExtern(const Bytecode::TableElem &e)
{
	bool isExtern = e.type == Bytecode::EL_EXTERN;
	bool isKumirModule = e.fileName.length() > 4 &&
		e.fileName.substr(e.fileName.length() - 4) == Core::fromAscii(".kod");
	return isExtern && !isKumirModule;
}

int runKumirXRun(int , char *argv[])
{
	fprintf(stderr, "program has plugin dependencies, run it by kumir2-xrun instead");
	return 1;

#if defined(WIN32) || defined(_WIN32)
	// Win32 implementation
	char buf[MAX_PATH];
	GetModuleFileNameA(NULL, buf, MAX_PATH);
	std::string kumir2run(buf);
	size_t lastSlash = kumir2run.find_last_of('\\');
	const std::string basePath = kumir2run.substr(0, lastSlash + 1);
	const std::string kumir2xrun = basePath + std::string("kumir2-xrun.exe");
	std::string commandLine = kumir2xrun;
	for (int i = 1; i < argc; i++) {
		std::string arg(argv[i]);
		commandLine.push_back(' ');
		commandLine.append(arg);
	}
	STARTUPINFOA startupInfo;
	ZeroMemory(&startupInfo, sizeof(startupInfo));
	startupInfo.cb = sizeof(startupInfo);
	PROCESS_INFORMATION processInformation;
	ZeroMemory(&processInformation, sizeof(processInformation));
	LPSTR lpCommandLine = const_cast<LPSTR>(commandLine.c_str());
	BOOL success = CreateProcessA(
			NULL, // lpApplicationName
			lpCommandLine, // lpCommandLine
			NULL, // lpProcessAttributes
			NULL, // lpThreadAttributes
			TRUE, // bInheritHandles
			0, // dwCreationFlags
			NULL, // lpEnvironment
			NULL, // lpCurrentDirectory
			&startupInfo, // lpStartupInfo
			&processInformation // lpProcessInformation
		);
	if (success) {
		return WaitForSingleObject(processInformation.hProcess, INFINITE);
	}
#else
	// Posix implementation
	std::string kumir2run = std::string(argv[0]);
	if (kumir2run.at(0) != '/') {
		char buf[2048];
		getcwd(buf, 2048);
		std::string cwd(buf);
		cwd.push_back('/');
		kumir2run = cwd + kumir2run;
	}
	size_t lastSlash = kumir2run.find_last_of('/');
	const std::string basePath = kumir2run.substr(0, lastSlash + 1);
	const std::string kumir2xrun = basePath + std::string("kumir2-xrun");
	return execv(kumir2xrun.c_str(), argv);
#endif
	return 127;
}

int main(int argc, char *argv[])
{
	// Look at arguments
	IO::setLocaleEncoding(LOCALE);

	std::string programName;
	std::deque<std::string> args;
	bool testingMode = false;
	bool quietMode = false;

	static const std::string minus_ansi("-ansi");
	static const std::string minus_t("-t");
	static const std::string minus_minus_testing("--test");
	static const std::string minus_p("-p");
	static const std::string minus_minus_pipe("--pipe");
	bool nmo = false;

	for (int i = 1; i < argc; i++) {
		const char *argvi = argv[i];
		std::string arg(argvi);

		if (argvi[0] == '-' && !nmo) {
			if (argvi[1] == '-' && argvi[2] == 0) {
				nmo = true; // no more options
			} else if (arg == minus_t || arg == minus_minus_testing) {
				testingMode = true;
				fprintf(stderr, "Testing mode on\n");
			} else if (arg == minus_p || arg == minus_minus_pipe) {
				quietMode = true;
				fprintf(stderr, "Quiet mode on\n");
			} else if (arg == minus_ansi) {
				LOCALE = CP1251;
				IO::setLocaleEncoding(LOCALE);
				fprintf(stderr, "CP1251 locale on\n");
			} else {
				fprintf(stderr, "Unknown option '%s' ignored\n", argvi);
			}
			continue;
		}

		if (programName.empty()) {
			programName = arg;
		} else {
			args.push_back(arg);
		}
	}

	fprintf(stderr, "Arguments parsed, pname='%s'\n", programName.c_str());
	for (size_t i = 0; i < args.size(); i++)
		fprintf(stderr, "arg[%u] = '%s'\n", (unsigned int) i, args[i].c_str());

	if (programName.empty()) {
		usage(argv[0]);
		return 127;
	}

	std::ifstream programFile(programName.c_str(), std::ios::in | std::ios::binary);
	if (!programFile.is_open()) {
		std::cerr << "Can't open program file: " << programName << std::endl;
		return 1;
	}

	fprintf(stderr, "Program loaded\n");

	Bytecode::Data programData;
	fprintf(stderr, "%s: %d \n", __FILE__, __LINE__);
	Bytecode::bytecodeFromDataStream(programFile, programData);
	fprintf(stderr, "%s: %d \n", __FILE__, __LINE__);
	programFile.close();
	fprintf(stderr, "%s: %d \n", __FILE__, __LINE__);

	// Check if it's possible to run using regular runtime
	bool hasPluginDependency =
		0 != std::count_if(programData.d.begin(), programData.d.end(), IsPluginExtern);

	fprintf(stderr, "plugins dependency: %d \n", (int) hasPluginDependency);

	if (hasPluginDependency) {
		return runKumirXRun(argc, argv);
	}

	fprintf(stderr, "%s: %d \n", __FILE__, __LINE__);
	// Prepare runner
	VM::KumirVM vm;
	VM::Console::InputFunctor inputFunctor;
	VM::Console::OutputFunctor outputFunctor;
	VM::Console::GetMainArgumentFunctor getMainArgumentFunctor;
	VM::Console::ReturnMainValueFunctor returnMainValueFunctor;
	getMainArgumentFunctor.setQuietMode(quietMode);
	returnMainValueFunctor.setQuietMode(quietMode);

	fprintf(stderr, "%s: %d \n", __FILE__, __LINE__);

	inputFunctor.setLocale(LOCALE);
	outputFunctor.setLocale(LOCALE);
	getMainArgumentFunctor.setLocale(LOCALE);
	returnMainValueFunctor.setLocale(LOCALE);

	fprintf(stderr, "%s: %d \n", __FILE__, __LINE__);
	getMainArgumentFunctor.init(argc, argv);
	fprintf(stderr, "%s: %d \n", __FILE__, __LINE__);

	vm.setFunctor(&inputFunctor);
	vm.setFunctor(&outputFunctor);
	vm.setFunctor(&getMainArgumentFunctor);
	vm.setFunctor(&returnMainValueFunctor);
	vm.setConsoleInputBuffer(&inputFunctor);
	vm.setConsoleOutputBuffer(&outputFunctor);
	fprintf(stderr, "%s: %d \n", __FILE__, __LINE__);

	String progName = fromLocale(programName);
	String programPath = Files::getAbsolutePath(progName);
	size_t slashPos = programPath.find_last_of(Char('/'));
	String programDir;
	if (slashPos != String::npos) {
		programDir = programPath.substr(0, slashPos);
	}

	fprintf(stderr, "%s: %d \n", __FILE__, __LINE__);
	vm.setProgramDirectory(programDir);
	fprintf(stderr, "%s: %d \n", __FILE__, __LINE__);

	String LOAD_ERROR = L"ОШИБКА ЗАГРУЗКИ ПРОГРАММЫ: ";
	String setProgramError;

	vm.setProgram(programData, true, progName, &setProgramError);
	fprintf(stderr, "%s: %d \n", __FILE__, __LINE__);

	if (setProgramError.length() > 0) {
		String message = LOAD_ERROR + setProgramError;
		return showErrorMessage(message, 126);
	}

	fprintf(stderr, "%s: %d \n", __FILE__, __LINE__);
	if (testingMode) {
		if (!vm.hasTestingAlgorithm()) {
			String NO_TESTING = L"В ПРОГРАММЕ НЕТ ТЕСТОВОГО АЛГОРИТМА";
			return showErrorMessage(NO_TESTING, 125);
		}
		vm.setEntryPoint(VM::KumirVM::EP_Testing);
	}
	fprintf(stderr, "%s: %d \n", __FILE__, __LINE__);
	vm.reset();
	fprintf(stderr, "%s: %d \n", __FILE__, __LINE__);
	vm.setDebugOff(true);
	fprintf(stderr, "%s: %d \n", __FILE__, __LINE__);


	// Main loop
	while (vm.hasMoreInstructions()) {
	fprintf(stderr, "%s: %d \n", __FILE__, __LINE__);
		vm.evaluateNextInstruction();
	fprintf(stderr, "%s: %d \n", __FILE__, __LINE__);
		if (vm.error().length() > 0) {
	fprintf(stderr, "%s: %d \n", __FILE__, __LINE__);
			String RUNTIME_ERROR = L"ОШИБКА ВЫПОЛНЕНИЯ: ";
			String RUNTIME_ERROR_AT = L"ОШИБКА ВЫПОЛНЕНИЯ В СТРОКЕ ";
			String COLON = L": ";
			String message;
			if (vm.effectiveLineNo() != -1) {
				message = RUNTIME_ERROR_AT +
					Converter::sprintfInt(vm.effectiveLineNo() + 1, 10, 0, 0) +
					COLON +
					vm.error();
			} else {
				message = RUNTIME_ERROR + vm.error();
			}
			return showErrorMessage(message, 120);
			return 120;
		}
	}

	if (testingMode) {
	fprintf(stderr, "%s: %d \n", __FILE__, __LINE__);
		return vm.returnCode();
	} else {
	fprintf(stderr, "%s: %d \n", __FILE__, __LINE__);
		return 0;
	}
}

