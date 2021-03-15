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

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>

#include "sha-256.h"

typedef struct KVDictSlice KVDictSlice;
typedef struct KVDict KVDict;


// Creation/cleanups
KVDict* KVCreate();
int KVDestroy(KVDict* dict);


// Getting values
char* KVValueForKey(KVDict* dict, const char* key);


// Reading/writting
int KVWriteToFile(KVDict* dict, FILE* fp);
KVDict* KVReadFromFile(FILE* fp);


// KVSetKeyValue will set a key string for a value. Will create the
// value if it does not exist.
int KVSetKeyValue(KVDict* dict, const char* key, const char* value);


// Printing
int KVPrintDict(KVDict* dict, FILE* stream);

#endif // KV_DICT_H__

// vim: tabstop=2 shiftwidth=2 expandtab autoindent softtabstop=0

