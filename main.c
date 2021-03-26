
#include <stdio.h>
#include "KVC.h"

int main(int argc, char** argv) {

  KVDict* dict = KVCreate();

  KVSetKeyValue(dict, "HELLO", "WORLD");
  KVSetKeyValue(dict, "h", "hello");
  KVSetKeyValue(dict, "w", "world");

  for (int i = 0; i < 100000; i++) {
    char foo[200];
    sprintf(foo, "KEY_%u", i);
    if (i == 350000) {
      KVSetKeyValue(dict, foo, "Can you see me?");
    } else {
      KVSetKeyValue(dict, foo, "hello-world");
    }
  }

  KVPrintDict(dict, stdout);


  // Print a key at the end of the "array" (which is a hashtable)
//  char* last_value = KVValueForKey(dict, "KEY_20");
//  printf("value for end-of-array: %s\n", last_value);

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

