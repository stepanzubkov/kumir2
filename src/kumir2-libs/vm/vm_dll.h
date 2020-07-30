#ifndef VM_DLL_H
#define VM_DLL_H

#if !defined(DLL_IMP) && !defined(DLL_EXP)
#if defined(_MSC_VER)
#define DLL_IMP __declspec(dllimport)
#define DLL_EXP __declspec(dllexport)
#else
#define DLL_IMP
#define DLL_EXP
#endif
#endif

#ifdef VM_LIBRARY
#define VM_DLL DLL_EXP
#else
#define VM_DLL DLL_IMP
#endif


#endif // VM_DLL_H

