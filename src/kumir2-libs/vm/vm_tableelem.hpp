#ifndef BYTECODE_TABLEELEM_H
#define BYTECODE_TABLEELEM_H

//#define DO_NOT_DECLARE_STATIC
//#include <kumir2-libs/stdlib/kumirstdlib.hpp>
#include "variant.hpp"
#include "vm_instruction.hpp"

#include <string>
#include <vector>
#include <list>

namespace Bytecode
{

using Kumir::String;


struct TableElem {
	ElemType type; // Element type
	std::list<ValueType> vtype; // Value type
	uint8_t dimension; // 0 for regular, [1..3] for arrays
	ValueKind refvalue;
	// 1 for in-argument,
	// 2 for in/out-argument,
	// 3 for out-argument
	// 0 for regular variable
	uint8_t module;  // module id
	uint16_t algId; // algorhitm id
	uint16_t id; // element id

	String name; // variable or function name
	std::string moduleAsciiName;
	String moduleLocalizedName; // external module name
	String fileName;
	String signature;
	std::string recordModuleAsciiName;
	String recordModuleLocalizedName;
	std::string recordClassAsciiName;
	String recordClassLocalizedName;
	VM::Variable initialValue; // constant value
	std::vector<Instruction> instructions; // for local defined function

	TableElem()
	{
		type = EL_NONE;
		vtype.push_back(VT_void);
		dimension = 0;
		refvalue = VK_Plain;
		module = 0;
		algId = id = 0;
	}
};

static inline bool isLittleEndian()
{
	static const uint16_t test = 0xFF00;
	const char *buf = reinterpret_cast<const char *>(&test);
	return buf[0] == 0x00;
}

template <typename T> inline void valueToDataStream(
	std::list<char> &stream, T value
) {
	static const bool le = isLittleEndian();
	const char *buf = reinterpret_cast<char *>(&value);
	if (le) {
		for (size_t i = sizeof(T); i --> 0; ) {
			stream.push_back(buf[i]);
		}
	} else {
		for (size_t i = 0; i < sizeof(T); i++) {
			stream.push_back(buf[i]);
		}
	}
}

template <typename T> inline void valueFromDataStream(
	std::list<char> &stream,
	T &value
) {
	char buf[sizeof(T)];
	static const bool le = isLittleEndian();
	if (le) {
		for (size_t i = sizeof(value); i --> 0 ; ) {
			buf[i] = stream.front();
			stream.pop_front();
		}
	} else {
		for (size_t i = 0; i < sizeof(value); i++) {
			buf[i] = stream.front();
			stream.pop_front();
		}
	}
	const T *t = reinterpret_cast<T *>(&buf);
	value = *t;
}

void tableElemToBinaryStream(std::list<char> &ds, const TableElem &e);
void tableElemFromBinaryStream(std::list<char> &ds, TableElem &e);

void tableElemToTextStream(
	std::ostream &ts,
	const TableElem &e,
	const AS_Helpers &helpers
);

} // namespace Bytecode

#endif // BYTECODE_TABLEELEM_H
