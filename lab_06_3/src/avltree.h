#ifndef __AVLTREE__H__

#define __AVLTREE__H__

#ifdef ARR_EXPORTS
#define ARR_DLL __declspec(dllexport)
#else
#define ARR_DLL __declspec(dllimport)
#endif

#define ARR_DECL __cdecl

#include "struct.h"

ARR_DLL int ARR_DECL height(tree_t *root);

ARR_DLL tree_t* ARR_DECL balance(tree_t *tree);

#endif