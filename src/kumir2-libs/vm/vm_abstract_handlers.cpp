#include <vm_abstract_handlers.h>

#if defined(WIN32) || defined(_WIN32)
#   include <windows.h>
#else
#   include <unistd.h>
#endif

namespace VM
{

Functor::~Functor() {}

void ExternalModuleResetFunctor::operator()(
	const std::string & /*moduleName*/,
	const Kumir::String &localizedName,
	Kumir::String *error
) {
	Kumir::String errorMessage =
		Kumir::Core::fromUtf8("Невозможно использовать \"") +
		localizedName +
		Kumir::Core::fromUtf8("\": исполнители не поддерживаются");
	if (error) {
		error->assign(errorMessage);
	}
}


std::deque<std::string> ExternalModuleLoadFunctor::operator()(
	const std::string & /*moduleAsciiName*/,
	const Kumir::String &moduleName,
	Kumir::String *error
) {
	Kumir::String errorMessage =
		Kumir::Core::fromUtf8("Невозможно использовать \"") +
		moduleName +
		Kumir::Core::fromUtf8("\": исполнители не поддерживаются");
	if (error) {
		error->assign(errorMessage);
	}
	return NamesList();
}


AnyValue ExternalModuleCallFunctor::operator()(
	const std::string & /*asciiModuleName*/,
	const Kumir::String &localizedModuleName,
	const uint16_t /*alogrithmId*/,
	VariableReferencesList /*arguments*/,
	Kumir::String *error
) {
	Kumir::String errorMessage =
		Kumir::Core::fromUtf8("Невозможно вызвать алгоритм исполнителя \"") +
		localizedModuleName +
		Kumir::Core::fromUtf8("\": исполнители не поддерживаются");
	if (error) {
		error->assign(errorMessage);
	}
	return AnyValue();
}

Kumir::String CustomTypeToStringFunctor::operator()(
	const Variable &variable,
	Kumir::String *error
) {
	const Kumir::String errorMessage =
		Kumir::Core::fromUtf8("Не могу вывести значение типа \"") +
		variable.recordClassLocalizedName() + Kumir::Core::fromAscii("\"");
	if (error) {
		error->assign(errorMessage);
	}
	return Kumir::String();
}


VM::AnyValue CustomTypeFromStringFunctor::operator()(
	const Kumir::String & /*source*/,
	const std::string & /*moduleAsciiName*/,
	const Kumir::String & /*moduleName*/,
	const std::string & /* typeAsciiName */,
	const Kumir::String &typeLocalizedName,
	Kumir::String *error
) {
	Kumir::String errorMessage =
		Kumir::Core::fromUtf8("Не могу разобрать значение типа \"") +
		typeLocalizedName + Kumir::Core::fromAscii("\"");
	if (error) {
		error->assign(errorMessage);
	}
	return VM::AnyValue();
}


void DelayFunctor::operator()(uint32_t msec)
{
#if defined(WIN32) || defined(_WIN32)
	Sleep(msec);
#else
	uint32_t sec = msec / 1000;
	uint32_t usec = (msec - sec * 1000) * 1000;
	// usleep works in range [0, 1000000), so
	// call sleep(sec) first for long periods
	sleep(sec);
	usleep(usec);
#endif
}

bool InputFunctor::operator()(
	VariableReferencesList /*alist*/,
	Kumir::String *error
) {
	Kumir::String errorMessage =
		Kumir::Core::fromUtf8("Операция ввода не поддерживается");
	if (error) {
		error->assign(errorMessage);
	}
	return false;
}


void OutputFunctor::operator()(
	VariableReferencesList /*vars*/,
	FormatsList /*formats*/,
	Kumir::String *error
)
{
	Kumir::String errorMessage =
		Kumir::Core::fromUtf8("Операция вывода не поддерживается");
	if (error) {
		error->assign(errorMessage);
	}
}


void GetMainArgumentFunctor::operator()(
	Variable & /*reference*/,
	Kumir::String *error
) {
	Kumir::String errorMessage =
		Kumir::Core::fromUtf8("Запуск первого алгоритма с аргументами не поддерживается");
	if (error) {
		error->assign(errorMessage);
	}
}


void ReturnMainValueFunctor::operator()(
	const Variable & /*reference*/,
	Kumir::String *error
) {
	Kumir::String errorMessage =
		Kumir::Core::fromUtf8("Возвращение значений первого алгоритма не поддерживается");
	if (error) {
		error->assign(errorMessage);
	}
}



} // namespace VM

