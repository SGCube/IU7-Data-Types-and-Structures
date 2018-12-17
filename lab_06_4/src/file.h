#ifndef __FILE__H__

#define __FILE__H__

#ifdef ARR_EXPORTS
#define ARR_DLL __declspec(dllexport)
#else
#define ARR_DLL __declspec(dllimport)
#endif

#define ARR_DECL __cdecl

ARR_DLL int ARR_DECL fsearch(FILE *f, int numb);

#endif