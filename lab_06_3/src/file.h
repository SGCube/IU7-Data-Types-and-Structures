#ifndef __FILE__H__

#define __FILE__H__

#ifdef ARR_EXPORTS
#define ARR_DLL __declspec(dllexport)
#else
#define ARR_DLL __declspec(dllimport)
#endif

#define ARR_DECL __cdecl

ARR_DLL size_t ARR_DECL getword(char **lineptr, size_t *n, FILE *stream);

ARR_DLL char* ARR_DECL fsearch(FILE *f, const char *word, int *rc);

#endif