
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
    char key[200];
    sprintf(key, "KEY_%u", i);
    if (i == loops-1) {
      KVSetKeyValue(dict, key, "Can you see me?");
    } else {
      KVSetKeyValue(dict, key, "hello-world");
    }
  }

  KVPrintDict(dict, stdout);

  // Print a key at the end of the "array" (which is a hashtable)

  char lastKey[200];
  sprintf(lastKey, "KEY_%u", loops-1);
 
  // Time the lookup speed
  clock_t t;
  t = clock();

  char* last_value = KVValueForKey(dict, lastKey);

  t = clock() - t;
  double time_taken = ((double)t)/CLOCKS_PER_SEC; // in seconds

  printf("lookup took %f seconds\n", time_taken);
  printf("value for end-of-array: (key)%s -> %s\n", lastKey, last_value);

  // Write the dict to a file
  FILE* fp = fopen("test.kv", "wb");

  printf("Writting dict to file: test.kv\n");
  KVWriteToFile(dict, fp);
  fclose(fp);

  KVDestroy(dict);

  fp = fopen("test.kv", "rb");
  
  printf("Reading dict from: file.kv...\n");

  KVDict* bar = KVReadFromFile(fp);
  fclose(fp);

  printf("Printing dict from file: ");
  KVPrintDict(bar, stdout);

  KVDestroy(bar);

  return 0;
}

// vim: tabstop=2 shiftwidth=2 expandtab autoindent softtabstop=0

