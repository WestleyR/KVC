//
// Example for KVC.
//

#include <stdio.h> // For printf, fopen
#include <time.h>  // For timing

#include "KVC.h"

#define writeDictToFile 1

int main() {
  // Allocate a new dict
  KVDict* dict = KVCreate();

  // Set a couple key-values
  KVSetKeyValue(dict, "HELLO", "WORLD");
  KVSetKeyValue(dict, "h", "hello");
  KVSetKeyValue(dict, "w", "world");

  // Loop and set a lot of values
  unsigned int loops = 1000000;
  printf("Creating dict of len=%d... this may take 10 seconds\n", loops);

  for (unsigned int i = 0; i < loops; i++) {
	char key[200];
	sprintf(key, "KEY_%u", i);
	if (i == loops-1) {
	  KVSetKeyValue(dict, key, "Can you see me?");
	} else {
	  KVSetKeyValue(dict, key, "hello-world");
	}
  }

  printf("Dict length: %u\n", KVLen(dict));

  // Print a key at the end of the "array" (which is a hashtable)
  char lastKey[200];
  sprintf(lastKey, "KEY_%u", loops-1);
 
  // Time the lookup speed
  clock_t t;
  t = clock();

  // The actral lookup. This value does not need to be freed.
  char* last_value = KVValueForKey(dict, lastKey);

  t = clock() - t;
  double time_taken = ((double)t)/CLOCKS_PER_SEC;

  printf("Lookup took %f seconds\n", time_taken);
  printf("Value for end of array: %s -> %s\n", lastKey, last_value);

  if (writeDictToFile) {
    // Write the dict to a file
    FILE* fp = fopen("test.kv", "wb");

    printf("\n");
    printf("Writting dict to file: test.kv\n");

    KVWriteToFile(dict, fp);
    fclose(fp);
  }

  KVDestroy(dict);

  if (writeDictToFile) {
    FILE* fp = fopen("test.kv", "rb");

    printf("Reading dict from: file.kv...\n");

    KVDict* bar = KVReadFromFile(fp);
    fclose(fp);

    printf("\nLast key value: %s\n", KVValueForKey(bar, lastKey));

    KVDestroy(bar);
  }

  return 0;
}

// vim: tabstop=2 shiftwidth=2 expandtab autoindent softtabstop=0
