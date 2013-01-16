#ifndef CONTEXT_H
#define CONTEXT_H

#include <vector>
#include "variant.hpp"
#include "vm_instruction.hpp"
#include "vm_tableelem.hpp"

namespace VM {

enum ContextRunMode {
    CRM_UntilReturn,
    CRM_ToEnd,
    CRM_OneStep
};

// Two strings

typedef std::pair<String,String> TwoStrings;

struct ExternReference {
    int moduleContext;
    uint32_t funcKey;
    String moduleName;
};

typedef std::map<uint32_t, ExternReference> ExternsMap;

// module_id|alg_id -> instructions
typedef std::map<uint32_t, Bytecode::TableElem> FunctionMap;

// module_id|global_id
typedef std::pair<uint8_t,uint16_t> GlobalsIndex;

// module_id|global_id -> global variable
typedef std::map<GlobalsIndex, Variable > GlobalsMap;

// constant_id -> constant value
typedef std::map<uint16_t,Variable> ConstantsMap;

typedef std::vector<Variable> VariantArray;

// module_id|alogithm_id -> local variable
typedef std::map<uint32_t, VariantArray> LocalsMap;

struct Context {
    inline Context() {
        IP = -1; type = Bytecode::EL_FUNCTION;
        runMode = CRM_ToEnd; lineNo = -1;
        algId = -1;
        program = 0;
        moduleContextNo = 0;
    }

    int registers[201];
    int IP;
    std::vector<Variable> locals;
    const std::vector<Bytecode::Instruction> * program;
    Bytecode::ElemType type;
    ContextRunMode runMode;
    uint8_t moduleId;
    int algId;
    int lineNo;
    size_t moduleContextNo;
};

struct ModuleContext {
    Kumir::String filename;
    FunctionMap functions;
    ExternsMap externs;
    FunctionMap inits;
    LocalsMap cleanLocalTables;
    GlobalsMap globals;
    ConstantsMap constants;
    uint8_t exportModuleId;
};

}

#endif