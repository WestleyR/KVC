

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

int KVAddKeyValue(KVDict* dict, const char* key, const char* value) {
  if (dict == NULL) return -1;

  dict->slice[dict->len] = (KVDictSlice*) malloc(sizeof(KVDictSlice));
  dict->slice[dict->len]->key = (char*) malloc(sizeof(char) * strlen(key) + 2);
  dict->slice[dict->len]->value = (char*) malloc(sizeof(char) * strlen(value) + 2);

  strcpy(dict->slice[dict->len]->key, key);
  strcpy(dict->slice[dict->len]->value, value);
  dict->len++;

  return 0;
}

int KVPrintDict(KVDict* dict) {

  for (int i = 0; i < dict->len; i++) {
    printf("%s->%s\n", dict->slice[i]->key, dict->slice[i]->value);
  }

  return 0;
}

// vim: set autoindent set tabstop=2 softtabstop=0 expandtab shiftwidth=2 smarttab

