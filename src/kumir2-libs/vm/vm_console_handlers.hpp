#ifndef VM_CONSOLE_HANDLERS_HPP
#define VM_CONSOLE_HANDLERS_HPP

#include "vm_abstract_handlers.h"

namespace VM
{

namespace Console
{

using namespace Kumir;

class VM_DLL InputFunctor :
	public VM::InputFunctor,
	public Kumir::AbstractInputBuffer
{
	Encoding locale_;
	VM::CustomTypeFromStringFunctor *customTypeFromString_;
	IO::InputStream stdin_;

public:
	InputFunctor();
	bool operator()(VariableReferencesList alist, Kumir::String *error) _override;

	void setLocale(const Encoding loc)
	{
		locale_ = loc;
	}

	void setCustomTypeFromStringFunctor(VM::CustomTypeFromStringFunctor *f)
	{
		customTypeFromString_ = f;
	}

	bool readRawChar(Char &ch) _override
	{
		return stdin_.readRawChar(ch);
	}

	void pushLastCharBack() _override
	{
		stdin_.pushLastCharBack();
	}

	void clear() _override {}

};

class VM_DLL OutputFunctor :
	public VM::OutputFunctor,
	public Kumir::AbstractOutputBuffer
{
	Encoding locale_;
	VM::CustomTypeToStringFunctor *customTypeToString_;

public:
	OutputFunctor();

	void operator()(
		VariableReferencesList alist,
		FormatsList formats,
		Kumir::String *error
	) _override;

	void setLocale(const Encoding loc)
	{
		locale_ = loc;
	}

	void setCustomTypeToStringFunctor(VM::CustomTypeToStringFunctor *f)
	{
		customTypeToString_ = f;
	}

	void writeRawString(const String &) _override;
};

class VM_DLL ReturnMainValueFunctor : public VM::ReturnMainValueFunctor
{
	Encoding locale_;
	VM::CustomTypeToStringFunctor *customTypeToString_;
	bool quietMode_;

public:
	ReturnMainValueFunctor();

	void operator()(
		const VM::Variable &reference,
		Kumir::String *error
	) _override;

	void setLocale(const Encoding loc)
	{
		locale_ = loc;
	}

	void setCustomTypeToStringFunctor(VM::CustomTypeToStringFunctor *f)
	{
		customTypeToString_ = f;
	}

	void setQuietMode(bool v)
	{
		quietMode_ = v;
	}
};

class VM_DLL GetMainArgumentFunctor : public VM::GetMainArgumentFunctor
{
	std::deque< String > m_arguments;
	size_t currentArgument_;
	Encoding locale_;
	VM::CustomTypeFromStringFunctor *customTypeFromString_;
	bool quietMode_;

public:
	GetMainArgumentFunctor();

	void operator()(VM::Variable &reference, Kumir::String *error) _override;
	void init(const std::deque<std::string> args);
	void init(int argc, char *argv[]);

	void setLocale(const Encoding loc)
	{
		locale_ = loc;
	}

	void setCustomTypeFromStringFunctor(VM::CustomTypeFromStringFunctor *f)
	{
		customTypeFromString_ = f;
	}

	void setQuietMode(bool v)
	{
		quietMode_ = v;
	}

private:
	bool readScalarArgument(
		const String &message,
		const String &name,
		VM::ValueType type,
		const std::string &customModuleAsciiName,
		const String &customModuleName,
		const std::string &customTypeAsciiName,
		const String &customTypeLocalizedName,
		VM::AnyValue &val, Kumir::String *error
	);

	static String decodeHttpStringValue(const std::string &s);
};


}
} // namespaces

#endif // VM_CONSOLE_HANDLERS_HPP
