
#include <stdio.h>
#include "KVC.h"

int main(int argc, char** argv) {

  KVDict* dict = KVCreate();

  KVAddKeyValue(dict, "HELLO", "WORLD");
  KVAddKeyValue(dict, "H", "Hello");
  KVAddKeyValue(dict, "W", "World");


  KVAddKeyValue(dict, "a", "ABCDEFGHIJKLMNOPQRSTUVWXUZ");
  KVAddKeyValue(dict, "b", "BCDEFGHIJKLMNOPQRSTUVWXUZ");
  KVAddKeyValue(dict, "c", "CDEFGHIJKLMNOPQRSTUVWXUZ");
  KVAddKeyValue(dict, "d", "DEFGHIJKLMNOPQRSTUVWXUZ");
  KVAddKeyValue(dict, "e", "EFGHIJKLMNOPQRSTUVWXUZ");
  KVAddKeyValue(dict, "f", "FGHIJKLMNOPQRSTUVWXUZ");

  KVPrintDict(dict, stdout);

  KVDestroy(dict);

  return 0;
}

// vim: tabstop=2 shiftwidth=2 expandtab autoindent softtabstop=0

