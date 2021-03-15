
#include <stdio.h>
#include "KVC.h"

int main(int argc, char** argv) {

  KVDict* dict = KVCreate();

  KVSetKeyValue(dict, "HELLO", "WORLD");
  KVSetKeyValue(dict, "H", "Hello");
  KVSetKeyValue(dict, "W", "World");

  KVSetKeyValue(dict, "overideing", "message1");
  KVSetKeyValue(dict, "overideing", "overitten key");

  KVSetKeyValue(dict, "a", "ABCDEFGHIJKLMNOPQRSTUVWXUZ");
  KVSetKeyValue(dict, "b", "BCDEFGHIJKLMNOPQRSTUVWXUZ");
  KVSetKeyValue(dict, "c", "CDEFGHIJKLMNOPQRSTUVWXUZ");
  KVSetKeyValue(dict, "d", "DEFGHIJKLMNOPQRSTUVWXUZ");
  KVSetKeyValue(dict, "e", "EFGHIJKLMNOPQRSTUVWXUZ");
  KVSetKeyValue(dict, "f", "FGHIJKLMNOPQRSTUVWXUZ");

  KVPrintDict(dict, stdout);

  char* foo = KVValueForKey(dict, "H");
  printf("value for H: %s\n", foo);

  KVDestroy(dict);

  return 0;
}

// vim: tabstop=2 shiftwidth=2 expandtab autoindent softtabstop=0

