#ifndef KSTDLIB_DLL_H
#define KSTDLIB_DLL_H

#if defined(_MSC_VER)
#define DLL_IMP __dllspec(dllimport)
#define DLL_EXP __dllspec(dllexport)
#else
#define DLL_IMP
#define DLL_EXP
#endif

#ifdef KSTDLIB_LIBRARY
#define KS_DLL DLL_EXP
#else
#define KS_DLL DLL_IMP
#endif


#endif // KSTDLIB_DLL_H

