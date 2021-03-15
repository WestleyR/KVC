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
  dict->len = 0;
  free(dict);

  return 0;
}

int indexForKey(KVDict* dict, const char* key) {
  if (dict == NULL) return -1;

  for (int i = 0; i < dict->len; i++) {
    if (strcmp(dict->slice[i]->key, key) == 0) {
      return i;
    }
  }

  return -1;
}

char* KVValueForKey(KVDict* dict, const char* key) {
  if (dict == NULL) return NULL;

  int index = indexForKey(dict, key);
  if (index != -1) {
    // It exists
    return dict->slice[index]->value;
  }

  return NULL;
}

int KVSetKeyValue(KVDict* dict, const char* key, const char* value) {
  if (dict == NULL) return -1;

  // First, check if the key already exists. If so, just modify it. Otherwise create a new slice.
  int index = indexForKey(dict, key);
  if (index != -1) {
    // Already exists
    dict->slice[index]->value = realloc(dict->slice[index]->value, strlen(value) + 2);
    strcpy(dict->slice[index]->value, value);
  } else {
    dict->slice = (KVDictSlice**) realloc(dict->slice, sizeof(KVDictSlice*) * dict->len + 1);
    dict->slice[dict->len] = (KVDictSlice*) malloc(sizeof(KVDictSlice));
    
    dict->slice[dict->len]->key = (char*) malloc(sizeof(char) * strlen(key) + 2);
    dict->slice[dict->len]->value = (char*) malloc(sizeof(char) * strlen(value) + 2);
    
    strcpy(dict->slice[dict->len]->key, key);
    strcpy(dict->slice[dict->len]->value, value);
    dict->len++;
  }

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

void addCharToCharArr(char* arr, char c) {
  int len = strlen(arr);
  arr[len] = c;
  arr[len+1] = '\0';
}

KVDict* KVReadFromFile(FILE* fp) {
  KVDict* dict = KVCreate();

  fseek(fp, 0, SEEK_END);
  int size = ftell(fp);
  fseek(fp, 0, SEEK_SET);
  char* data = (char*) malloc(size + 1);
  fread(data, 1, size, fp);
  data[size] = '\0';
  fclose(fp);

  char* ptr = data;

  char key[1024];
  key[0] = '\0';
  char value[1024];
  value[0] = '\0';

  while (*ptr) {
    if (*ptr == '<') {
      // Begin key
      ptr++;
      if (*ptr != '"') {
        // Not a key
      } else {
        ptr++;
        while (*ptr != '"') {
          addCharToCharArr(key, *ptr);
          ptr++;
        }
        //printf("KEY: %s\n", key);
      }
      // Now check for the value
      ptr++;
      if (*ptr == ',') {
        ptr++;
        if (*ptr == '"') {
          ptr++;
          while (*ptr != '"') {
            addCharToCharArr(value, *ptr);
            ptr++;
          }
          //printf("VALUE: %s\n", value);
          KVSetKeyValue(dict, key, value);
          key[0] = '\0';
          value[0] = '\0';
        }
      }
    }

    if (*ptr == '>') {
      // End key
    }
    ptr++;
  }

  free(data);

  return dict;
}

int KVWriteToFile(KVDict* dict, FILE* fp) {
  if (dict == NULL) return -1;

  fprintf(fp, "<dict_version>0.1.0</dict_version>\n");
  fprintf(fp, "<dict_begin>\n");

  for (int i = 0; i < dict->len; i++) {
    fprintf(fp, "<\"%s\",\"%s\">\n", dict->slice[i]->key, dict->slice[i]->value);
  }

  fprintf(fp, "</dict_begin>\n");

  return 0;
}

// vim: tabstop=2 shiftwidth=2 expandtab autoindent softtabstop=0

