#include "vm_instruction.hpp"
#include <sstream>
#include <set>
#include <algorithm>
#define DO_NOT_DECLARE_STATIC
#include <kumir2-libs/stdlib/kumirstdlib.hpp>

namespace Bytecode
{

std::string typeToString(InstructionType t)
{
	if (t == NOP) {
		return ("nop");
	} else if (t == CALL) {
		return ("call");
	} else if (t == INIT) {
		return ("init");
	} else if (t == SETARR) {
		return ("setarr");
	} else if (t == STORE) {
		return ("store");
	} else if (t == STOREARR) {
		return ("storearr");
	} else if (t == LOAD) {
		return ("load");
	} else if (t == LOADARR) {
		return ("loadarr");
	} else if (t == SETMON) {
		return ("setmon");
	} else if (t == UNSETMON) {
		return ("unsetmon");
	} else if (t == JUMP) {
		return ("jump");
	} else if (t == JNZ) {
		return ("jnz");
	} else if (t == JZ) {
		return ("jz");
	} else if (t == POP) {
		return ("pop");
	} else if (t == PUSH) {
		return ("push");
	} else if (t == RET) {
		return ("return");
	} else if (t == PAUSE) {
		return ("pause");
	} else if (t == ERRORR) {
		return ("error");
	} else if (t == SUM) {
		return ("sum");
	} else if (t == SUB) {
		return ("sub");
	} else if (t == MUL) {
		return ("mul");
	} else if (t == DIV) {
		return ("div");
	} else if (t == POW) {
		return ("pow");
	} else if (t == NEG) {
		return ("neg");
	} else if (t == AND) {
		return ("and");
	} else if (t == OR) {
		return ("or");
	} else if (t == EQ) {
		return ("eq");
	} else if (t == NEQ) {
		return ("neq");
	} else if (t == LS) {
		return ("ls");
	} else if (t == GT) {
		return ("gt");
	} else if (t == LEQ) {
		return ("leq");
	} else if (t == GEQ) {
		return ("geq");
	} else if (t == REF) {
		return ("ref");
	} else if (t == REFARR) {
		return ("refarr");
	} else if (t == LINE) {
		return ("line");
	} else if (t == SHOWREG) {
		return ("showreg");
	} else if (t == CLEARMARG) {
		return ("clearmarg");
	} else if (t == SETREF) {
		return ("setref");
	} else if (t == PAUSE) {
		return ("pause");
	} else if (t == HALT) {
		return ("halt");
	} else if (t == CTL) {
		return ("ctl");
	} else if (t == INRANGE) {
		return ("inrange");
	} else if (t == UPDARR) {
		return ("updarr");
	} else if (t == CLOAD) {
		return ("cload");
	} else if (t == CSTORE) {
		return ("cstore");
	} else if (t == CDROPZ) {
		return ("cdropz");
	} else if (t == CACHEBEGIN) {
		return ("cachebegin");
	} else if (t == CACHEEND) {
		return ("cacheend");
	} else {
		return "nop";
	}
}

InstructionType typeFromString(const std::string &ss)
{
	std::string s = Kumir::Core::toLowerCase(ss);
	if (s == "nop") {
		return NOP;
	} else if (s == "call") {
		return CALL;
	} else if (s == "init") {
		return INIT;
	} else if (s == "setarr") {
		return SETARR;
	} else if (s == "store") {
		return STORE;
	} else if (s == "storearr") {
		return STOREARR;
	} else if (s == "load") {
		return LOAD;
	} else if (s == "loadarr") {
		return LOADARR;
	} else if (s == "setmon") {
		return SETMON;
	} else if (s == "unsetmon") {
		return UNSETMON;
	} else if (s == "jump") {
		return JUMP;
	} else if (s == "jnz") {
		return JNZ;
	} else if (s == "jz") {
		return JZ;
	} else if (s == "pop") {
		return POP;
	} else if (s == "push") {
		return PUSH;
	} else if (s == "return") {
		return RET;
	} else if (s == "pause") {
		return PAUSE;
	} else if (s == "error") {
		return ERRORR;
	} else if (s == "sum") {
		return SUM;
	} else if (s == "sub") {
		return SUB;
	} else if (s == "mul") {
		return MUL;
	} else if (s == "div") {
		return DIV;
	} else if (s == "pow") {
		return POW;
	} else if (s == "neg") {
		return NEG;
	} else if (s == "and") {
		return AND;
	} else if (s == "or") {
		return OR;
	} else if (s == "eq") {
		return EQ;
	} else if (s == "neq") {
		return NEQ;
	} else if (s == "ls") {
		return LS;
	} else if (s == "gt") {
		return GT;
	} else if (s == "leq") {
		return LEQ;
	} else if (s == "geq") {
		return GEQ;
	} else if (s == "ref") {
		return REF;
	} else if (s == "refarr") {
		return REFARR;
	} else if (s == "line") {
		return LINE;
	} else if (s == "showreg") {
		return SHOWREG;
	} else if (s == "clearmarg") {
		return CLEARMARG;
	} else if (s == "setref") {
		return SETREF;
	} else if (s == "pause") {
		return PAUSE;
	} else if (s == "halt") {
		return HALT;
	} else if (s == "ctl") {
		return CTL;
	} else if (s == "inrange") {
		return INRANGE;
	} else if (s == "updarr") {
		return UPDARR;
	} else if (s == "cload") {
		return CLOAD;
	} else if (s == "cstore") {
		return CSTORE;
	} else if (s == "cdropz") {
		return CDROPZ;
	} else if (s == "cachebegin") {
		return CACHEBEGIN;
	} else if (s == "cacheend") {
		return CACHEEND;
	} else {
		return NOP;
	}
}

std::string instructionToString(
	const Instruction &instr,
	const AS_Helpers &helpers,
	uint8_t moduleId, uint16_t algId
) {
	static std::set<InstructionType> VariableInstructions;
	VariableInstructions.insert(INIT);
	VariableInstructions.insert(SETARR);
	VariableInstructions.insert(STORE);
	VariableInstructions.insert(STOREARR);
	VariableInstructions.insert(LOAD);
	VariableInstructions.insert(LOADARR);
	VariableInstructions.insert(SETMON);
	VariableInstructions.insert(UNSETMON);
	VariableInstructions.insert(REF);
	VariableInstructions.insert(REFARR);
	VariableInstructions.insert(SETREF);
	VariableInstructions.insert(UPDARR);

	static std::set<InstructionType> ModuleNoInstructions;
	ModuleNoInstructions.insert(CALL);
	ModuleNoInstructions.insert(CTL);

	static std::set<InstructionType> RegisterNoInstructions;
	RegisterNoInstructions.insert(POP);
	RegisterNoInstructions.insert(PUSH);
	RegisterNoInstructions.insert(JZ);
	RegisterNoInstructions.insert(JNZ);
	RegisterNoInstructions.insert(SHOWREG);

	static std::set<InstructionType> HasValueInstructions;
	HasValueInstructions.insert(CALL);
	HasValueInstructions.insert(INIT);
	HasValueInstructions.insert(SETARR);
	HasValueInstructions.insert(STORE);
	HasValueInstructions.insert(STOREARR);
	HasValueInstructions.insert(LOAD);
	HasValueInstructions.insert(LOADARR);
	HasValueInstructions.insert(SETMON);
	HasValueInstructions.insert(UNSETMON);
	HasValueInstructions.insert(JUMP);
	HasValueInstructions.insert(JNZ);
	HasValueInstructions.insert(JZ);
	HasValueInstructions.insert(ERRORR);
	HasValueInstructions.insert(LINE);
	HasValueInstructions.insert(REF);
	HasValueInstructions.insert(REFARR);
	HasValueInstructions.insert(CLEARMARG);
	HasValueInstructions.insert(SETREF);
	HasValueInstructions.insert(HALT);
	HasValueInstructions.insert(PAUSE);
	HasValueInstructions.insert(CTL);
	HasValueInstructions.insert(UPDARR);

	std::stringstream result;
	result.setf(std::ios::hex, std::ios::basefield);
	result.setf(std::ios::showbase);

	InstructionType t = instr.type;
	result << typeToString(t);

	if (ModuleNoInstructions.count(t)) {
		result << " " << int(instr.module);
	}

	if (VariableInstructions.count(t)) {
		VariableScope s = instr.scope;
		if (s == GLOBAL) {
			result << " global";
		} else if (s == LOCAL) {
			result << " local";
		} else if (s == CONSTT) {
			result << " constant";
		}
	}

	if (t == LINE) {
		uint32_t from, to;
		result.unsetf(std::ios::basefield);
		result.unsetf(std::ios::showbase);
		if (extractColumnPositionsFromLineInstruction(instr, from, to)) {
			result << " col " << int(from) << "-" << int(to);
		} else {
			result << " no " << int(instr.arg);
		}
	}
	if (HasValueInstructions.count(t) && t != LINE) {
		if (t == JUMP || t == JNZ || t == JZ) {
			result.unsetf(std::ios::basefield);
			result.unsetf(std::ios::showbase);
		}
		result << " " << int(instr.arg);
	}
	if (RegisterNoInstructions.count(t)) {
		result << " " << int(instr.registerr);
	}

	std::string r = result.str();

	if (VariableInstructions.count(t)) {
		VariableScope s = instr.scope;
		AS_Key akey;
		const AS_Values *vals = nullptr;
		akey.first = 0;
		if (s == LOCAL) {
			akey.first = (moduleId << 16) | algId;
			akey.second = instr.arg;
			vals = &(helpers.locals);
		} else if (s == GLOBAL) {
			akey.first = (moduleId << 16);
			akey.second = instr.arg;
			vals = &(helpers.globals);
		} else if (s == CONSTT) {
			akey.first = 0;
			akey.second = instr.arg;
			vals = &(helpers.constants);
		}
		if (vals && vals->count(akey)) {
			while (r.length() < 25) {
				r.push_back(' ');
			}
			r += std::string("# ") + vals->at(akey);
		}
	} else if (t == CALL) {
		AS_Key akey(instr.module << 16, instr.arg);
		const AS_Values *vals = &(helpers.algorithms);
		if (vals && vals->count(akey)) {
			while (r.length() < 25) {
				r.push_back(' ');
			}
			r += std::string("# ") + vals->at(akey);
		}
	}
	return r;
}

uint32_t toUint32(const Instruction &instr)
{
	static std::set<InstructionType> ModuleNoInstructions;
	ModuleNoInstructions.insert(CALL);
	ModuleNoInstructions.insert(CTL);

	static std::set<InstructionType> RegisterNoInstructions;
	RegisterNoInstructions.insert(POP);
	RegisterNoInstructions.insert(PUSH);
	RegisterNoInstructions.insert(JZ);
	RegisterNoInstructions.insert(JNZ);
	RegisterNoInstructions.insert(SHOWREG);

	uint32_t first = uint8_t(instr.type);
	first = first << 24;
	uint32_t second = 0u;
	if (ModuleNoInstructions.count(instr.type)) {
		second = uint8_t(instr.module);
	} else if (RegisterNoInstructions.count(instr.type)) {
		second = uint8_t(instr.registerr);
	} else {
		second = uint8_t(instr.scope);
	}
	if (instr.type == LINE) {
		second = uint8_t(instr.lineSpec);
	}
	second = second << 16;
	uint32_t last = instr.arg;
	last = last << 16; // Ensure first two bytes are 0x0000
	last = last >> 16;
	uint32_t result = first | second | last;
	return result;
}

Instruction fromUint32(uint32_t value)
{
	static std::set<InstructionType> ModuleNoInstructions;
	ModuleNoInstructions.insert(CALL);
	ModuleNoInstructions.insert(CTL);

	static std::set<InstructionType> RegisterNoInstructions;
	RegisterNoInstructions.insert(POP);
	RegisterNoInstructions.insert(PUSH);
	RegisterNoInstructions.insert(JZ);
	RegisterNoInstructions.insert(JNZ);
	RegisterNoInstructions.insert(SHOWREG);

	uint32_t first  = value & 0xFF000000;
	uint32_t second = value & 0x00FF0000;
	uint32_t last   = value & 0x0000FFFF;
	first = first >> 24;
	second = second >> 16;
	Instruction i;
	i.type = InstructionType(first);
	if (ModuleNoInstructions.count(i.type)) {
		i.module = uint8_t(second);
	} else if (RegisterNoInstructions.count(i.type)) {
		i.registerr = uint8_t(second);
	} else {
		i.scope = VariableScope(second);
	}
	if (i.type == LINE) {
		i.lineSpec = LineSpecification(second);
	}
	i.arg = uint16_t(last);
	return i;
}





};

