#ifndef VM_ABSTRACT_HANDLERS_H
#define VM_ABSTRACT_HANDLERS_H

/* This file contains interface declarations for VM handlers.
 *
 * By default, all handlers are trivial and do nothing.
 *
 * Debugger handler is implemented as a Java-style handler with a set
 * of virtual methods.
 *
 * Mutex interface is just a abstract class with Lock and Unlock virtual
 *
 * More critical handlers are implemented as distinct functors.
 *
 */

#include "vm_dll.h"

#ifndef _override
#if defined(_MSC_VER)
#   define _override
#else
#   define _override override
#endif
#endif

#include "variant.hpp"


namespace VM
{

/** A generic "Mutex" interface to be implemented using
 *  platform-specific Mutex implementation */

class CriticalSectionLocker
{
public:
	virtual void lock() {}
	virtual void unlock() {}
	virtual void reset() {}
	// destructor MUST me virtual even not need
	virtual ~CriticalSectionLocker() {}
};

/* =================== Begin functors declarations ======================= */

class Functor
{
public:
	enum Type {
		Invalid = 0,
		ExternalModuleReset,
		ExternalModuleLoad,
		ExternalModuleCall,
		ConvertToString,
		ConvertFromString,
		Input,
		Output,
		GetMainArgument,
		ReturnMainValue,
		Pause,
		Delay
	};
	virtual Type type() const = 0;
protected:
	// destructor MUST me virtual even not need
	virtual ~Functor() {}
};

/* ====== Functors for external modules initialization ====== */

/** A functor to reset external module before execution by a given name */
class VM_DLL ExternalModuleResetFunctor: public Functor
{
public:
	ExternalModuleResetFunctor(): callFunctor_(0) {}

	Type type() const _override
	{
		return ExternalModuleReset;
	}

	virtual void operator()(
		const std::string & /*moduleName*/,
		const Kumir::String &localizedName,
		Kumir::String *error
	);

	void setCallFunctor(class ExternalModuleCallFunctor *callFunctor)
	{
		callFunctor_ = callFunctor;
	}

protected:
	class ExternalModuleCallFunctor *callFunctor_;
};


/** A functor to load external module plugin by a given name and
 *  canonical module file name, e.g. a file name without prefix 'lib' and
 *  any dot-after suffix in name.
 *
 *  Example:
 *    ExternalModuleLoadFunctor("A great module", "greatModule", &errorMessage)
 *
 *    --- loads 'A great module' from file 'libGreatModule.so' (on Linux), or
 *        from file 'GreatModule.dll' (on Windows).
 *    --- returns a list of module provided names
 */
class VM_DLL ExternalModuleLoadFunctor: public Functor
{
public:
	Type type() const _override
	{
		return ExternalModuleLoad;
	}

	typedef std::deque<std::string> NamesList;

	virtual NamesList operator()(
		const std::string & /*moduleAsciiName*/,
		const Kumir::String &moduleName,
		Kumir::String *error
	);
};


/** A functor to call external module module by a given name and ID.
 *
 *  An arguments list is passed to functor; return value is a
 *  calling function return (is any), or a dummy any value (if void).
 *
 */
class VM_DLL ExternalModuleCallFunctor: public Functor
{
public:
	Type type() const _override
	{
		return ExternalModuleCall;
	}

	typedef const std::deque<Variable> &VariableReferencesList;
	virtual AnyValue operator()(
		const std::string & /*asciiModuleName*/,
		const Kumir::String &localizedModuleName,
		const uint16_t /*alogrithmId*/,
		VariableReferencesList /*arguments*/,
		Kumir::String *error
	);

	virtual void checkForActorConnected(const std::string & /*asciiModuleName*/) {}
	virtual void terminate() {}
};

class VM_DLL CustomTypeToStringFunctor: public Functor
{
public:
	Type type() const _override
	{
		return ConvertToString;
	}

	virtual Kumir::String operator()(
		const Variable &variable,
		Kumir::String *error
	);
};

class VM_DLL CustomTypeFromStringFunctor: public Functor
{
public:
	Type type() const _override
	{
		return ConvertFromString;
	}
	virtual VM::AnyValue operator()(
		const Kumir::String & /*source*/,
		const std::string & /*moduleAsciiName*/,
		const Kumir::String & /*moduleName*/,
		const std::string & /* typeAsciiName */,
		const Kumir::String &typeLocalizedName,
		Kumir::String *error
	);
};

/* ====== Functors for execution control function run ======= */

/** A functor to make a pause while debugging */
class PauseFunctor : public Functor
{
public:
	Type type() const _override
	{
		return Pause;
	}
	virtual void operator()() {}
};


/** A functor to sleep execution.
 *  Default implementation is not multi-thread
 *
 *  Argument is a sleep time in milliseconds
 */
class VM_DLL DelayFunctor : public Functor
{
public:
	Type type() const _override
	{
		return Delay;
	}

	virtual void operator()(uint32_t msec);
};

/* ====== Functors for user input and output================= */

/** A functor to input variables values within user interface
 *
 *  Argument is a list of Reference-variables to be set.
 *
 */
class VM_DLL InputFunctor: public Functor
{
public:
	Type type() const _override
	{
		return Input;
	}

	typedef std::deque<Variable> &VariableReferencesList;

	virtual bool operator()(
		VariableReferencesList /*alist*/,
		Kumir::String *error
	);
};


/** A functor to output variables values within user interface
 *
 *  Argument is a list of Reference-variables to be shown and
 *  a list of the same size of int-pairs (format parameters).
 *
 */
class VM_DLL OutputFunctor: public Functor
{
public:
	Type type() const _override
	{
		return Output;
	}

	typedef const std::deque<Variable> &VariableReferencesList;
	typedef const std::deque< std::pair<int, int> > &FormatsList;

	virtual void operator()(
		VariableReferencesList /*vars*/,
		FormatsList /*formats*/,
		Kumir::String *error
	);
};

class VM_DLL GetMainArgumentFunctor: public Functor
{
public:
	Type type() const _override
	{
		return GetMainArgument;
	}

	virtual void operator()(Variable & /*reference*/, Kumir::String *error);
};

class VM_DLL ReturnMainValueFunctor: public Functor
{
public:
	Type type() const _override
	{
		return ReturnMainValue;
	}

	virtual void operator()(
		const Variable & /*reference*/,
		Kumir::String *error
	);
};

/* ====== End all functors declarations ============== */

/** An interface to communicating GUI debugger */

class DebuggingInteractionHandler
{
public:
	virtual bool appendTextToMargin(
		int /*lineNo*/,
		const Kumir::String & /*noticeText*/
	) {
		return false;
	}

	virtual bool setTextToMargin(
		int /*lineNo*/,
		const Kumir::String & /*text*/,
		bool /*redColorForeground*/
	) {
		return false;
	}

	virtual bool clearMargin(
		int /*fromLine*/, int /*toLine*/
	) {
		return false;
	}

	virtual bool noticeOnFunctionReturn()
	{
		return false;
	}

	virtual bool noticeOnLineChanged(
		int /*lineNo*/,
		uint32_t /*columnStartNo*/,
		uint32_t /*columnEndNo*/
	) {
		return false;
	}

	virtual bool noticeOnStepsChanged(
		uint64_t /*stepsDone*/
	) {
		return false;
	}

	virtual void debuggerReset() {}
	virtual void debuggerNoticeBeforePopContext() {}
	virtual void debuggerNoticeBeforePushContext() {}
	virtual void debuggerNoticeAfterPopContext() {}
	virtual void debuggerNoticeAfterPushContext() {}
	virtual void debuggerNoticeBeforeArrayInitialize(const VM::Variable & /*variable*/,
		const int /*bounds*/[7]) {}
	virtual void debuggerNoticeAfterArrayInitialize(const VM::Variable & /*variable*/) {}
	virtual void debuggerNoticeOnValueChanged(const VM::Variable & /*variable*/,
		const int[4] /*indeces*/) {}

	virtual void debuggerNoticeOnBreakpointHit(const String & /*filename*/, const uint32_t /*lineNo*/) {}
};

}

#endif // VM_ABSTRACT_HANDLERS_H
