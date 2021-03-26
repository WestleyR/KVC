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

  // Right now, need to free the first slice.
  // The array will never start at 0.
  free(dict->slice[0]);

  for (int i = 1; i < dict->len; i++) {
    if (dict->slice[i] != NULL) {
      free(dict->slice[i]->key);
      free(dict->slice[i]->value);
      free(dict->slice[i]);
    }
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

void hash_32_to_string(char str[65], uint8_t hash[32]) {
  for (int i = 0; i < 32; i++) {
    str += sprintf(str, "%u", hash[i]);
  }
}

// returns a 78 digit number in a string, for a key string.
char* keyStrToHash(const char* key) {
  uint8_t hash[32];

  calc_sha_256(hash, key, strlen(key));

  char str[256];
  hash_32_to_string(str, hash);


//  unsigned int hash = 0;
//  for (int i = 0; i < strlen(key); i++) {
//    hash += key[i];
//  }

  char* ret = (char*) malloc(strlen(str) + 2);
  strcpy(ret, str);
  //sprintf(ret, "%u", hash);

  return ret;
}

char* KVValueForKey(KVDict* dict, const char* key) {
  if (dict == NULL) return NULL;

  char* hashArr = keyStrToHash(key);

  // Only go up to 20 digits, otherwise the number gets too big
  for (int i = 0; i < 20; i++) {
    unsigned long long index = hashArr[0] - '0';

    for (int v = 1; v < i; v++) {
      index *= 10;
      index += hashArr[v] - '0';
    }

    printf("Searching at index: %llu\n", index);

    if (index > dict->len) {
      printf("Key: %s not found\n", key);
      return NULL;
    }

    if (dict->slice[index] != NULL) {
      if (dict->slice[index]->key != NULL) {
        if (strcmp(dict->slice[index]->key, key) == 0) {
          printf("Found %s at index %llu\n\n", dict->slice[index]->key, index);
          return dict->slice[index]->value;
        }
      }
    }
  }

  free(hashArr);

  return NULL;
}

int KVSetKeyValue(KVDict* dict, const char* key, const char* value) {
  if (dict == NULL) return -1;

  char* hashArr = keyStrToHash(key);
//  printf("HASHARR: %s\n", hashArr);

  unsigned int indexContinue = 0;

//  for (int i = strlen(hashArr) - 1; i >= 0; i--) {

  int indexI = strlen(hashArr) - 1;

  while (true) {

    unsigned long long index = hashArr[indexI] - '0';

    for (int i = 0; i < indexContinue; i++) {
      index *= 1.1;
      index += hashArr[indexI] - '0';

      indexI--;
      if (indexI < 0) indexI = strlen(hashArr)-1;
    }

//    printf("HASH: %llu\n", index);

    if (index < dict->len) {
      // May have an open spot for it (or already exists)

      if (dict->slice[index] != NULL) {
        if (dict->slice[index]->key != NULL) {
          if (strcmp(dict->slice[index]->key, key) == 0) {
            // Found the key! it already exists in our array

            // TODO: fix this
            printf("Key already exists in our array\n");
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
//        printf("Mallocing: %d\n", dict->len);

        dict->slice[dict->len] = NULL;

//          dict->slice[dict->len] = (KVDictSlice*) malloc(sizeof(KVDictSlice));
//          dict->slice[dict->len]->key = NULL;
//          dict->slice[dict->len]->value = NULL;

        dict->len++;
      }
  
      printf("Mallocing -> %d\n", dict->len);
      dict->slice[dict->len] = (KVDictSlice*) malloc(sizeof(KVDictSlice));
      
      dict->slice[dict->len]->key = (char*) malloc(sizeof(char) * strlen(key) + 2);
      dict->slice[dict->len]->value = (char*) malloc(sizeof(char) * strlen(value) + 2);
      
      strcpy(dict->slice[dict->len]->key, key);
      strcpy(dict->slice[dict->len]->value, value);
      dict->len++;
      break;
    }
  }

//  printf("Max trys: %d\n", indexContinue);


//  int br = 0;
//
//  // Only go up to 20 digits, otherwise the number gets too big
//  for (int i = 0; i < 20; i++) {
//    unsigned long long index = hashArr[strlen(hashArr)-1] - '0';
//
//    if (i != 0) {
//      for (int v = 1; v < i; v++) {
//        index += hashArr[strlen(hashArr) - v];
//      }
//    };
//
////    for (int v = 1; v < i; v++) {
////      index *= 10;
////      index += hashArr[v] - '0';
////    }
//
////    printf("HASH: %llu\n", index);
//
//    if (dict->len > index) {
//      if (dict->slice[index] != NULL) {
//        if (dict->slice[index]->key != NULL) {
//          if (strcmp(dict->slice[index]->key, key) == 0) {
//            // the key already exists
//            br = 1;
//            break;
//          } else {
//            // Not the right key/index, so keep going
//            continue;
//          }
//        }
//      } else {
//        printf("Found open spot at: %llu\n", index);
//        dict->slice[index] = (KVDictSlice*) malloc(sizeof(KVDictSlice));
//        dict->slice[index]->key = (char*) malloc(sizeof(char) * strlen(key) + 2);
//        dict->slice[index]->value = (char*) malloc(sizeof(char) * strlen(value) + 2);
//        
//        strcpy(dict->slice[index]->key, key);
//        strcpy(dict->slice[index]->value, value);
// 
//        br = 1;
//        break;
//      }
//    } else {
//      // the key does not exist yet
//      if (index > dict->len) {
//        dict->slice = (KVDictSlice**) realloc(dict->slice, sizeof(KVDictSlice*) * index + (2 * sizeof(KVDictSlice*)));
//        while (dict->len < index) {
////          printf("Mallocing: %d\n", dict->len);
//
//          dict->slice[dict->len] = NULL;
//
////          dict->slice[dict->len] = (KVDictSlice*) malloc(sizeof(KVDictSlice));
////          dict->slice[dict->len]->key = NULL;
////          dict->slice[dict->len]->value = NULL;
//
//          dict->len++;
//        }
////        printf("Done mallocing\n");
//      }
//  
//      printf("Mallocing -> %d\n", dict->len);
//      dict->slice[dict->len] = (KVDictSlice*) malloc(sizeof(KVDictSlice));
//      
//      dict->slice[dict->len]->key = (char*) malloc(sizeof(char) * strlen(key) + 2);
//      dict->slice[dict->len]->value = (char*) malloc(sizeof(char) * strlen(value) + 2);
//      
//      strcpy(dict->slice[dict->len]->key, key);
//      strcpy(dict->slice[dict->len]->value, value);
//      dict->len++;
//      br = 1;
//    }
//
//    if (br == 1) break;
//  }
//
//  free(hashArr);
//
//

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

