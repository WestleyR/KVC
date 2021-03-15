//
//  KVC.c
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

#include "KVC.h"

struct KVDictSlice {
  char* key;
  char* value;
};

struct KVDict {
  KVDictSlice** slice;
  unsigned int len;
};

KVDict* KVCreate() {
	KVDict* dict = (KVDict*) malloc(sizeof(KVDict));

	dict->slice = (KVDictSlice**) malloc(sizeof(KVDictSlice*));
	dict->len = 0;

	return dict;
}

int KVDestroy(KVDict* dict) {
  if (dict == NULL) return -1;

  for (int i = 0; i < dict->len; i++) {
    free(dict->slice[i]->key);
    free(dict->slice[i]->value);
    free(dict->slice[i]);
  }

  free(dict->slice);
  free(dict);

  return 0;
}

int KVSetKeyValue(KVDict* dict, const char* key, const char* value) {
  if (dict == NULL) return -1;

	dict->slice = (KVDictSlice**) realloc(dict->slice, sizeof(KVDictSlice*) * dict->len + 1);
  dict->slice[dict->len] = (KVDictSlice*) malloc(sizeof(KVDictSlice));

  dict->slice[dict->len]->key = (char*) malloc(sizeof(char) * strlen(key) + 2);
  dict->slice[dict->len]->value = (char*) malloc(sizeof(char) * strlen(value) + 2);

  strcpy(dict->slice[dict->len]->key, key);
  strcpy(dict->slice[dict->len]->value, value);
  dict->len++;

  return 0;
}

int KVPrintDict(KVDict* dict, FILE* stream) {
  if (dict == NULL) return -1;

  fprintf(stream, "{\n");
  for (int i = 0; i < dict->len; i++) {
    fprintf(stream, "  %s->%s\n", dict->slice[i]->key, dict->slice[i]->value);
  }
  fprintf(stream, "}\n");

  return 0;
}

// vim: tabstop=2 shiftwidth=2 expandtab autoindent softtabstop=0

