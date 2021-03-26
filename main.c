
#include <stdio.h>
#include <time.h>
#include "KVC.h"

int main(int argc, char** argv) {

  KVDict* dict = KVCreate();

  KVSetKeyValue(dict, "HELLO", "WORLD");
  KVSetKeyValue(dict, "h", "hello");
  KVSetKeyValue(dict, "w", "world");

  unsigned int loops = 100000;
  for (int i = 0; i < loops; i++) {
    char foo[200];
    sprintf(foo, "KEY_%u", i);
    if (i == loops-1) {
      KVSetKeyValue(dict, foo, "Can you see me?");
    } else {
      KVSetKeyValue(dict, foo, "hello-world");
    }
  }

  KVPrintDict(dict, stdout);

  // Print a key at the end of the "array" (which is a hashtable)

  char lastKey[200];
  sprintf(lastKey, "KEY_%u", loops-1);
 
  clock_t t;
  t = clock();

  char* last_value = KVValueForKey(dict, lastKey);

  t = clock() - t;
  double time_taken = ((double)t)/CLOCKS_PER_SEC; // in seconds

  printf("lookup took %f seconds\n", time_taken);
  printf("value for end-of-array: %s\n", last_value);

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

