#ifndef VM_HPP
#define VM_HPP

#include <memory>

#include <kumir2-libs/stdlib/kumirstdlib.hpp>
#include "vm_bytecode.hpp"
#include "variant.hpp"
#include "context.hpp"
#include "stack.hpp"
#include "vm_abstract_handlers.h"
#include "vm_breakpoints_table.hpp"

namespace VM
{

typedef std::shared_ptr<CriticalSectionLocker> ShPtrCSL;

class KumirVM
{
public /*typedefs*/:
	enum EntryPoint { EP_Main, EP_Testing };

public /*methods*/:
	/** Set parsed Kumir bytecode */
	void setProgram(const Bytecode::Data &data, bool isMain, const String &filename, Kumir::String *error);
	void setProgramDirectory(const Kumir::String &path)
	{
		programDirectory_.clear();
		programDirectory_ = path;
	}

	bool loadProgramFromBinaryBuffer(std::list<char> &stream, bool isMain, const String &filename, String &error);

	/** Set entry point to Main or Testing algorithm */
	void setEntryPoint(EntryPoint ep)
	{
		entryPoint_ = ep;
	}

	EntryPoint entryPoint() const { return entryPoint_; }

	/** Set 'blind' mode flag: do not emit debugging information */
	void setDebugOff(bool b)
	{
		blindMode_ = b;
	}

	bool isDebugOff() const { return blindMode_; }

	/** Set 'step into' flag for next function call */
	void setStepInto(bool b)
	{
		nextCallInto_ = b;
	}

	/** Reset program to initial state */
	void reset();

	bool hasTestingAlgorithm() const;
	unsigned long int stepsDone() const
	{
		return stepsCounter_;
	}


	/** Breakpoint operations */
	void removeAllBreakpoints();
	void insertOrChangeBreakpoint(
		bool enabled,
		const String &fileName,
		uint32_t lineNo, uint32_t ignoreCount,
		const String &condition
	);
	void insertSingleHitBreakpoint(const String &fileName, uint32_t lineNo);
	void removeBreakpoint(const String &fileName, const uint32_t lineNo);

	/** Sets the Debugging Interaction handler for this VM */
	void setDebuggingHandler(DebuggingInteractionHandler *h)
	{
		debugHandler_ = h;
	}

	/** Sets a distinct functor */
	void setFunctor(Functor *functor);

	/** In GUI mode it's need a Mutex to protect internal data
	 *  from been discovered.
	 *     You must set actual Mutex implementation object
	 *     (Qt or C++11) with methods: lock() and unlock()
	 */
	void setMutex(ShPtrCSL m)
	{
		stacksMutex_ = m;
	}

	/**
	 * Returns top-level value stack scalar item or returns invalid if not
	 * applicable
	 */
	uint8_t returnCode() const /* nothrow */;

	/** The following two functions are basic to use for actual run:
	 *  while ( vm.hasMoreInstructions() )
	 *      vm.evaluateNextInstruction();
	 */
	bool hasMoreInstructions() const;
	void evaluateNextInstruction();

	/** Return current 'line number' or -1 if not applicable */
	int effectiveLineNo() const;
	std::pair<uint32_t, uint32_t> effectiveColumn() const;

	/** Returns 'true' if evaluating non-toplevel user function */
	bool canStepOut() const;

	/** Used by debugger. Returns local variables for a given stack frame */
	const std::vector<Variable> &getLocals(int frameNo) const;

	/** Used by debugger. Returns global variables for a given module name */
	const std::vector<Variable> &getGlobals(const Kumir::String &moduleName) const;

	const std::vector<Variable> *getMainModuleGlobals() const;

	std::pair< Kumir::String, const std::vector<Variable> * >
	getLocalsAndName(size_t stackIndex) const;

	size_t functionCallStackSize() const;
	const Stack<Context> &callStack() const
	{
		return contextsStack_;
	}

	/** Returns last error */
	const String &error() const
	{
		if (error_.length() == 0 && Kumir::Core::getError().length() > 0) {
			return Kumir::Core::getError();
		} else {
			return error_;
		}
	}

	void setConsoleInputBuffer(Kumir::AbstractInputBuffer *b)
	{
		consoleInputBuffer_ = b;
		Kumir::Files::setConsoleInputBuffer(b);
	}

	Kumir::AbstractInputBuffer *consoleInputBuffer() const
	{
		return consoleInputBuffer_;
	}

	void setConsoleOutputBuffer(Kumir::AbstractOutputBuffer *b)
	{
		consoleOutputBuffer_ = b;
		Kumir::Files::setConsoleOutputBuffer(b);
	}

	Kumir::AbstractOutputBuffer *consoleOutputBuffer() const
	{
		return consoleOutputBuffer_;
	}

	/** Debug control methods */
	void setNextCallInto();
	void setNextCallToEndOfContext();
	void setNextCallToEnd();
	void setNextCallStepOver();

	static Record toRecordValue(const Kumir::FileType &ft);
	static Kumir::FileType fromRecordValue(const Record &record);

private /*fields*/:
	std::vector<ModuleContext> moduleContexts_;
	EntryPoint entryPoint_;
	bool blindMode_;
	bool nextCallInto_;
	ShPtrCSL stacksMutex_;
	DebuggingInteractionHandler *debugHandler_;
	ExternalModuleLoadFunctor *externalModuleLoad_;
	ExternalModuleResetFunctor *externalModuleReset_;
	ExternalModuleCallFunctor *externalModuleCall_;
	InputFunctor *input_;
	OutputFunctor *output_;
	CustomTypeFromStringFunctor *customTypeFromString_;
	CustomTypeToStringFunctor *customTypeToString_;
	GetMainArgumentFunctor *getMainArgument_;
	ReturnMainValueFunctor *returnMainValue_;
	PauseFunctor *pause_;
	DelayFunctor *delay_;

	uint8_t evaluationResult_;

	Context lastContext_;
	int backtraceSkip_;
	String error_;
	AnyValue register0_;
	Stack<Variable> valuesStack_;
	Stack<Context> contextsStack_;
	Stack< std::pair<bool, Variable> > cacheStack_;
	Kumir::String programDirectory_;
	typedef std::vector<Variable> VariablesTable;
	VariablesTable *currentConstants_;
	VariablesTable *currentGlobals_;
	VariablesTable *currentLocals_;
	unsigned long int stepsCounter_;
	Kumir::AbstractInputBuffer *consoleInputBuffer_;
	Kumir::AbstractOutputBuffer *consoleOutputBuffer_;
	int previousLineNo_;
	uint32_t previousColStart_;
	uint32_t previousColEnd_;

	BreakpointsTable breakpointsTable_;


public /*constructors*/:
	KumirVM();

private /*methods*/:
	static Variable fromTableElem(const Bytecode::TableElem &e);
	int contextByIds(int moduleId, int algorhitmId) const;

	Context &currentContext()
	{
		return contextsStack_.top();
	}

	void nextIP()
	{
		if (contextsStack_.size() > 0) {
			contextsStack_.top().IP ++;
		}
	}

	Variable &findVariable(uint8_t scope, uint16_t id);
	void checkFunctors();
	bool isRunningMain() const;

private /*instruction methods*/:
	void do_call(uint8_t, uint16_t);
	void do_stdcall(uint16_t);
	void do_filescall(uint16_t);
	void do_stringscall(uint16_t);
	void do_specialcall(uint16_t);

	void do_init(uint8_t, uint16_t);
	void do_setarr(uint8_t, uint16_t);
	void do_updarr(uint8_t, uint16_t);
	void do_store(uint8_t, uint16_t);
	void do_storearr(uint8_t, uint16_t);
	void do_load(uint8_t, uint16_t);
	void do_loadarr(uint8_t, uint16_t);
	void do_jump(uint16_t);
	void do_jz(uint8_t, uint16_t);
	void do_jnz(uint8_t, uint16_t);
	void do_pop(uint8_t);
	void do_push(uint8_t);
	void do_cload();
	void do_cstore();
	void do_cdropz();
	void do_cachebegin();
	void do_cacheend();
	void do_ret();
	void do_error(uint8_t, uint16_t);
	void do_line(const Bytecode::Instruction &instr);
	void do_ref(uint8_t, uint16_t);
	void do_setref(uint8_t, uint16_t);
	void do_refarr(uint8_t, uint16_t);
	void do_showreg(uint8_t);
	void do_clearmarg(uint16_t);
	void do_pause(uint16_t);
	void do_halt(uint16_t);
	void do_ctl(uint8_t parameter, uint16_t value);
	void do_inrange();

	void do_sum();
	void do_sub();
	void do_mul();
	void do_div();
	void do_pow();
	void do_neg();
	void do_and();
	void do_or();
	void do_eq();
	void do_neq();
	void do_ls();
	void do_gt();
	void do_leq();
	void do_geq();
};


} // end namespace VM
#endif
