//
//  KVC.h
//  Key-Values interface for C
//
// Created by WestleyR on 2021-03-14
// Source code: https://github.com/WestleyR/KVC
//
// Copyright (c) 2021 WestleyR. All rights reserved.
// This software is licensed under a BSD 3-Clause Clear License.
// Consult the LICENSE file that came with this software regarding
// your rights to distribute this software.
//

#ifndef KV_DICT_H__
#define KV_DICT_H__

#include <stdio.h>  // For printf, sprintf, fprintf and FILE
#include <stdlib.h> // For malloc/free
#include <string.h> // For strlen and strcmp
//#include <stdint.h>

// The higher the value, the faster the adding/lookup. But the bigger the array.
// Cannot be <= 1. Recommend: More efficient (but slow) = 1.2, very fast = 2.2.
static const float hashMultiplier = 1.3;

typedef struct KVDictSlice KVDictSlice;
typedef struct KVDict KVDict;

// KVCreate will alloc a new emtpy KVDict.
KVDict* KVCreate();

// KVDestroy will dealloc and free the whole KVDict. After dealloc you should
// not use the dict for anything until its re-created.
int KVDestroy(KVDict* dict);

// KVLen returns the number of entries in the dict.
unsigned int KVLen(KVDict* dict);

// KVValueForKey returns a pointer to the value for the given key. Returned
// pointer should not be freed.
char* KVValueForKey(KVDict* dict, const char* key);

// KVWriteToFile will write a KVDict to a file pointer (could also be stdout).
int KVWriteToFile(KVDict* dict, FILE* fp);

// KVReadFromFile will alloc a new dict with the contents of *fp.
KVDict* KVReadFromFile(FILE* fp);

// KVSetKeyValue will set a key string for a value.
int KVSetKeyValue(KVDict* dict, const char* key, const char* value);

// KVPrintDict should only be used for debugging.
int KVPrintDict(KVDict* dict, FILE* stream);

#endif // KV_DICT_H__

// vim: tabstop=2 shiftwidth=2 expandtab autoindent softtabstop=0
