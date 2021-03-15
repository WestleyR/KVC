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

	dict->slice = (KVDictSlice**) malloc(sizeof(KVDictSlice*) * 5000);
	dict->len = 0;

	return dict;
}

int KVDestroy(KVDict* dict) {
  if (dict == NULL) return -1;

  // Right now, need to free the first slice.
  // The array will never start at 0.
  free(dict->slice[0]);

  for (int i = 1; i < dict->len; i++) {
    free(dict->slice[i]->key);
    free(dict->slice[i]->value);
    free(dict->slice[i]);
  }

  free(dict->slice);
  dict->len = 0;
  free(dict);

  return 0;
}

// TODO: use the hashing
int indexForKey(KVDict* dict, const char* key) {
  if (dict == NULL) return -1;

  for (int i = 0; i < dict->len; i++) {
    if (strcmp(dict->slice[i]->key, key) == 0) {
      return i;
    }
  }

  return -1;
}

unsigned int getIndexForKey(const char* key) {
  unsigned int checksum = 0;

  while (*key) {
    checksum += *key;
    key++;
  }

  return checksum;
}

char* KVValueForKey(KVDict* dict, const char* key) {
  if (dict == NULL) return NULL;


  unsigned int keyIndex = getIndexForKey(key);
  if (keyIndex > dict->len) return NULL;
  return dict->slice[keyIndex]->value;
}

void hash_32_to_string(char str[65], uint8_t hash[32]) {
  for (int i = 0; i < 32; i++) {
    str += sprintf(str, "%u", hash[i]);
  }
}

// returns a 78 digit number in a string, for a key string.
char* keyStrToHash(const char* key) {
  uint8_t hash[32];

  calc_sha_256(hash, key, strlen(key));

  char str[65];
  hash_32_to_string(str, hash);

  char* ret = (char*) malloc(64);
  strcpy(ret, str);

  return ret;
}

int KVSetKeyValue(KVDict* dict, const char* key, const char* value) {
  if (dict == NULL) return -1;

  char* hashArr = keyStrToHash(key);
  printf("HASHARR: %s\n", hashArr);

  int br = 0;

  // Only go up to 20 digits, otherwise the number gets too big
  for (int i = 0; i < 20; i++) {
    unsigned long long index = hashArr[0] - '0';

    for (int v = 1; v < i; v++) {
      index *= 10;
      index += hashArr[v] - '0';
    }

    printf("HASH: %llu\n", index);

    if (dict->len > index) {
      if (dict->slice[index]->key != NULL) {
        if (strcmp(dict->slice[index]->key, key) == 0) {
          // the key already exists
          br = 1;
          break;
        } else {
          continue;
        }
      } else {
        // Spot already used, findanother
        dict->slice[index]->key = (char*) malloc(sizeof(char) * strlen(key) + 2);
        dict->slice[index]->value = (char*) malloc(sizeof(char) * strlen(value) + 2);
        
        strcpy(dict->slice[index]->key, key);
        strcpy(dict->slice[index]->value, value);
 
        br = 1;
        break;
      }
    } else {
      // the key does not exist yet
      if (index > dict->len) {
//        if (dict->len != 0) {
//          KVDictSlice** foo = (KVDictSlice**) malloc(sizeof(KVDictSlice*) * index + 2);
//          foo[0] = (KVDictSlice*) malloc(sizeof(KVDictSlice));
//
//          for (int i = 1; i < dict->len; i++) {
//            printf("Allocating bigger array of index: %d until: %d\n", i, dict->len);
//            foo[i] = (KVDictSlice*) malloc(sizeof(KVDictSlice));
//
//            foo[i]->key = NULL;
//            foo[i]->value = NULL;
//  
//            if (dict->slice[i]->key != NULL) {
//              foo[i]->key = (char*) malloc(strlen(dict->slice[i]->key) + 2);
//              strcpy(foo[i]->key, dict->slice[i]->key);
//            }
//            if (dict->slice[i]->value != NULL) {
//              foo[i]->value = (char*) malloc(strlen(dict->slice[i]->value) + 2);
//              strcpy(foo[i]->value, dict->slice[i]->value);
//            }
//          }
//
//          // Free the old dict
//          free(dict->slice[0]);
//          for (int i = 1; i < dict->len; i++) {
//            free(dict->slice[i]->key);
//            free(dict->slice[i]->value);
//            free(dict->slice[i]);
//          }
//          free(dict->slice);
//
//          dict->slice = foo;
//        }
//        dict->slice = (KVDictSlice**) realloc(dict->slice, sizeof(KVDictSlice*) * index + 1);
        printf("LEN: %d\n", dict->len);
        while (dict->len < index) {
//          printf("Mallocing: %d\n", dict->len);
          dict->slice[dict->len] = (KVDictSlice*) malloc(sizeof(KVDictSlice) * 2);

          dict->slice[dict->len]->key = NULL;
          dict->slice[dict->len]->value = NULL;

          dict->len++;
        }
      }
  
      dict->slice[dict->len] = (KVDictSlice*) malloc(sizeof(KVDictSlice));
      
      dict->slice[dict->len]->key = (char*) malloc(sizeof(char) * strlen(key) + 2);
      dict->slice[dict->len]->value = (char*) malloc(sizeof(char) * strlen(value) + 2);
      
      strcpy(dict->slice[dict->len]->key, key);
      strcpy(dict->slice[dict->len]->value, value);
      dict->len++;
      br = 1;
    }

    if (br == 1) break;

  }

  free(hashArr);


//  if (keyIndex > dict->len) {
//    dict->slice = (KVDictSlice**) realloc(dict->slice, sizeof(KVDictSlice*) * keyIndex + 1);
//    while (dict->len < keyIndex) {
//     dict->slice[dict->len] = (KVDictSlice*) malloc(sizeof(KVDictSlice));
//     dict->len++;
//    }
//  }
//  dict->slice[dict->len] = (KVDictSlice*) malloc(sizeof(KVDictSlice));
//  
//  dict->slice[dict->len]->key = (char*) malloc(sizeof(char) * strlen(key) + 2);
//  dict->slice[dict->len]->value = (char*) malloc(sizeof(char) * strlen(value) + 2);
//  
//  strcpy(dict->slice[dict->len]->key, key);
//  strcpy(dict->slice[dict->len]->value, value);
//  dict->len++;

  return 0;
}

int KVPrintDict(KVDict* dict, FILE* stream) {
  if (dict == NULL) return -1;

  fprintf(stream, "{\n");
  for (int i = 1; i < dict->len; i++) {
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

