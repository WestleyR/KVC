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

// For debugging
#ifdef DEBUG
#define debugf(...) printf(__VA_ARGS__)
#else
#define debugf(fmt, ...)
#endif

struct KVDictSlice {
  char* key;
  char* value;
};

struct KVDict {
  KVDictSlice** slice;
  unsigned int len;
  unsigned int entries;
};

KVDict* KVCreate() {
  KVDict* dict = (KVDict*) malloc(sizeof(KVDict));

  dict->slice = (KVDictSlice**) malloc(sizeof(KVDictSlice*));
  dict->len = 0;
  dict->entries = 0;

  return dict;
}

int KVDestroy(KVDict* dict) {
  if (dict == NULL) return -1;

  for (unsigned int i = 0; i < dict->len; i++) {
    if (dict->slice[i] != NULL) {
      free(dict->slice[i]->key);
      free(dict->slice[i]->value);
      free(dict->slice[i]);
    }
  }

  free(dict->slice);
  dict->len = 0;
  dict->entries = 0;
  free(dict);

  return 0;
}

unsigned int KVLen(KVDict* dict) {
	return dict->entries;
}

// keyStrToHash gets the most (or highest yet) efficent hash for a key. Keep in
// mind that the hash is iterated in reverse order.
char* keyStrToHash(const char* key) {
  unsigned int hash, i;
  for (hash = i = 0; i < strlen(key); ++i) {
      hash += key[i];
      hash += (hash << 10);
      hash ^= (hash >> 6);
  }

  hash += (hash << 3);
  hash ^= (hash >> 11);
  hash += (hash << 15);

  char* ret = (char*) malloc(128);
  sprintf(ret, "%u", hash);

  return ret;
}

char* KVValueForKey(KVDict* dict, const char* key) {
  if (dict == NULL) return NULL;

  char* hashArr = keyStrToHash(key);

  unsigned int indexContinue = 0;
  int indexI = strlen(hashArr) - 1;

  while (1) {
    unsigned long long index = hashArr[indexI] - '0';

    for (unsigned int i = 0; i < indexContinue; i++) {
      index *= hashMultiplier;
      index += hashArr[indexI] - '0';

      indexI--;
      if (indexI < 0) indexI = strlen(hashArr)-1;
    }

    if (index < dict->len) {
      // May have an open spot for it (or already exists)

      if (dict->slice[index] != NULL) {
        if (dict->slice[index]->key != NULL) {
          if (strcmp(dict->slice[index]->key, key) == 0) {
            // Found the key! it already exists in our array

            debugf("Lookup attemtps: %d\n", indexContinue + 1);
            free(hashArr);
            return dict->slice[index]->value;
          }
        }
      }
    } else {
      debugf("Key does not exist\n");
      free(hashArr);
      return NULL;
    }
    indexContinue++;
  }

  free(hashArr);

  return NULL;
}

int KVSetKeyValue(KVDict* dict, const char* key, const char* value) {
  if (dict == NULL) return -1;

  char* hashArr = keyStrToHash(key);
//  debugf("HASHARR: %s for key: %s\n", hashArr, key);

  unsigned int indexContinue = 0;
  int indexI = strlen(hashArr) - 1;

  while (1) {
    unsigned long long index = hashArr[indexI] - '0';

    for (unsigned int i = 0; i < indexContinue; i++) {
      index *= hashMultiplier;
      index += hashArr[indexI] - '0';

      indexI--;
      if (indexI < 0) indexI = strlen(hashArr)-1;
    }

//    debugf("HASH: %llu\n", index);

    if (index < dict->len) {
      // May have an open spot for it (or already exists)

      if (dict->slice[index] != NULL) {
        if (dict->slice[index]->key != NULL) {
          if (strcmp(dict->slice[index]->key, key) == 0) {
            // Found the key! it already exists in our array

            // TODO: fix this
            debugf("Key already exists in our array\n");
            break;
          } else {
            // Opps, that index is already takken. So index the index
            indexContinue++;
          }
        }
      } else {
        // Theres an open spot for it
        dict->slice[index] = (KVDictSlice*) malloc(sizeof(KVDictSlice));
        dict->slice[index]->key = (char*) malloc(sizeof(char) * strlen(key) + 2);
        dict->slice[index]->value = (char*) malloc(sizeof(char) * strlen(value) + 2);
        
        strcpy(dict->slice[index]->key, key);
        strcpy(dict->slice[index]->value, value);

        break;
      }
    } else {
      // Our array is not big enought, so realloc it bigger

      dict->slice = (KVDictSlice**) realloc(dict->slice, sizeof(KVDictSlice*) * index + (2 * sizeof(KVDictSlice*)));
      while (dict->len < index) {
        dict->slice[dict->len] = NULL;
        dict->len++;
      }
  
      debugf("Mallocing -> %d\n", dict->len);
      dict->slice[dict->len] = (KVDictSlice*) malloc(sizeof(KVDictSlice));
      
      dict->slice[dict->len]->key = (char*) malloc(sizeof(char) * strlen(key) + 2);
      dict->slice[dict->len]->value = (char*) malloc(sizeof(char) * strlen(value) + 2);
      
      strcpy(dict->slice[dict->len]->key, key);
      strcpy(dict->slice[dict->len]->value, value);
      dict->len++;
      break;
    }
  }

  free(hashArr);

  dict->entries++;

  return 0;
}

int KVPrintDict(KVDict* dict, FILE* stream) {
  if (dict == NULL) return -1;

  fprintf(stream, "{\n");
  for (unsigned int i = 1; i < dict->len; i++) {
    if (dict->slice[i] == NULL) continue;
    if (dict->slice[i]->key == NULL) continue;
    fprintf(stream, "  %s->%s (at index: %u)\n", dict->slice[i]->key, dict->slice[i]->value, i);
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
        //debugf("KEY: %s\n", key);
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
          //debugf("VALUE: %s -> %s\n", value, key);
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

  fprintf(fp, "<KVC><github.com/WestleyR/KVC><dict_version>0.1.0</dict_version>\n");
  fprintf(fp, "<dict>\n");

  for (unsigned long long i = 0; i < dict->len; i++) {
    if (dict->slice[i] != NULL) {
      if (dict->slice[i]->key != NULL) {
        fprintf(fp, "<\"%s\",\"%s\">\n", dict->slice[i]->key, dict->slice[i]->value);
      }
    }
  }

  fprintf(fp, "</dict>\n");

  return 0;
}

// vim: tabstop=2 shiftwidth=2 expandtab autoindent softtabstop=0
