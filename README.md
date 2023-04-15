# KVC

Key-Values Dictionary interface for C.

This is a proof-of-concept/work-in-progress. It may have issues...

## What KVC does

KVC provides a simple interface for storing key-values in a dictionary struct
that can be saved to a file, and read back from a file.

## How to use

KVC is currently in development, unstable api. please check back later or lmk
if your interested.

### C interface

Copy the `KVC.h` and `KVC.c` to your project. See the [`KVC.h`](./KVC.h) for
api.

## Performance

Currently, for adding key/values, it would be typically be 100 tries per
million entities. Not exactly the best, but reasonably fast.

Lookup attempts is be similar, and over 500x faster then standard key string
compare.

## License

This project is licensed under the terms of The Clear BSD License. See the
[LICENSE file](./LICENSE) for more details.

