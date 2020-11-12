#ifndef KSTDLIB_DLL_H
#define KSTDLIB_DLL_H

#if !defined(DLL_IMP) && !defined(DLL_EXP)
#if defined(_MSC_VER)
#pragma warning (disable : 4251)
#define DLL_IMP __declspec(dllimport)
#define DLL_EXP __declspec(dllexport)
#else
#define DLL_IMP
#define DLL_EXP
#endif
#endif

#ifdef KSTDLIB_LIBRARY
#define KS_DLL DLL_EXP
#else
#define KS_DLL DLL_IMP
#endif


#endif // KSTDLIB_DLL_H

