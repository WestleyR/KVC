
#ifndef KV_DICT_H__
#define KV_DICT_H__

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

typedef struct KVDictSlice KVDictSlice;
typedef struct KVDict KVDict;

KVDict* KVCreate();

int KVAddKeyValue(KVDict* dict, const char* key, const char* value);

int KVPrintDict(KVDict* dict);

#endif // KV_DICT_H__

// vim: set autoindent set tabstop=2 softtabstop=0 expandtab shiftwidth=2 smarttab

