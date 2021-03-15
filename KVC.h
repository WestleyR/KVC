
#ifndef KV_DICT_H__
#define KV_DICT_H__

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

typedef struct KVDictSlice KVDictSlice;
typedef struct KVDict KVDict;

KVDict* KVCreate();
int KVDestroy(KVDict* dict);

int KVAddKeyValue(KVDict* dict, const char* key, const char* value);

int KVPrintDict(KVDict* dict, FILE* stream);

#endif // KV_DICT_H__

// vim: tabstop=2 shiftwidth=2 expandtab autoindent softtabstop=0

