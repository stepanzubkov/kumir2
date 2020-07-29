#ifndef BYTECODE_DATA_H
#define BYTECODE_DATA_H

#include "vm_tableelem.hpp"
#include <deque>
#include <istream>
#include <ostream>


namespace Bytecode
{

struct Data {
	std::deque <TableElem> d;
	uint8_t versionMaj;
	uint8_t versionMin;
	uint8_t versionRel;
	unsigned long lastModified;
};

bool isValidSignature(const std::list<char> &ds);
void bytecodeToDataStream(std::ostream &ds, const Data &data);
void bytecodeToDataStream(std::list<char> &ds, const Data &data);
void bytecodeFromDataStream(std::list<char> &ds, Data &data);
void bytecodeFromDataStream(std::istream &is, Data &data);
void bytecodeToTextStream(std::ostream &ts, const Data &data);


} // namespace Bytecode

#endif // BYTECODE_DATA_H
