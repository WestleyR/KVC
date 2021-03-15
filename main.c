
#include <stdio.h>
#include "KVC.h"

int main(int argc, char** argv) {

  KVDict* dict = KVCreate();

  KVSetKeyValue(dict, "HELLO", "WORLD");
  KVSetKeyValue(dict, "h", "hello");

//  for (int i = 0; i < 300000; i++) {
//    char foo[20];
//    sprintf(foo, "KEY_%u", i);
//    if (i == 299999) {
//      KVSetKeyValue(dict, foo, "Can you see me?");
//    } else {
//      KVSetKeyValue(dict, foo, "hello-world");
//    }
//  }

//  KVSetKeyValue(dict, "H", "Hello");
//  KVSetKeyValue(dict, "W", "World");
//
//  KVSetKeyValue(dict, "overideing", "message1");
//  KVSetKeyValue(dict, "overideing", "overitten key");
//
//  KVSetKeyValue(dict, "a", "ABCDEFGHIJKLMNOPQRSTUVWXUZ");
//  KVSetKeyValue(dict, "b", "BCDEFGHIJKLMNOPQRSTUVWXUZ");
//  KVSetKeyValue(dict, "c", "CDEFGHIJKLMNOPQRSTUVWXUZ");
//  KVSetKeyValue(dict, "d", "DEFGHIJKLMNOPQRSTUVWXUZ");
//  KVSetKeyValue(dict, "e", "EFGHIJKLMNOPQRSTUVWXUZ");
//  KVSetKeyValue(dict, "f", "FGHIJKLMNOPQRSTUVWXUZ");

//  printf("Entire dict: ");
  KVPrintDict(dict, stdout);

  // Print a key at the end of the "array" (which is a hashtable)
  char* last_value = KVValueForKey(dict, "KEY_299999");
  printf("value for end-of-array: %s\n", last_value);

//  char* foo = KVValueForKey(dict, "H");
//  printf("value for H: %s\n", foo);
//  
//  printf("\nWritting the dict to file: test.kv...\n\n");
//
//  FILE* fp = fopen("test.kv", "wb");
//  KVWriteToFile(dict, fp);
//  fclose(fp);

  KVDestroy(dict);

//  fp = fopen("test.kv", "rb");
//  
//  printf("Reading dict from: file.kv...\n");
//
//  KVDict* bar = KVReadFromFile(fp);
//  fclose(fp);
//
//  printf("Printing dict from file: ");
//  KVPrintDict(bar, stdout);
//  KVDestroy(bar);

  return 0;
}

// vim: tabstop=2 shiftwidth=2 expandtab autoindent softtabstop=0

