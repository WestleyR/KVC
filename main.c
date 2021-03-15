
#include <stdio.h>
#include "KVC.h"

int main(int argc, char** argv) {

  KVDict* dict = KVCreate();

  KVAddKeyValue(dict, "HELLO", "WORLD");

  KVPrintDict(dict);

  return 0;
}

// vim: set autoindent set tabstop=2 softtabstop=0 expandtab shiftwidth=2 smarttab

