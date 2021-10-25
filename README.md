# KVC

Key-Values Dictionary interface for C (WIP)

## What KVC does

KVC provides a simple interface for storing key-values in a dictionary struct
that can be saved to a file, and read back from a file.

## How to use

KVC is currently in development, unstable api. please check back later or lmk
if your interested.

## Performance

Currently, for adding key/values, it would be typically be 100 tries per
million entities. Not exactly the best, but reasonably fast.

Lookup attempts is be similar, and over 500x faster then standard key string
compare.

As far as efficiencies, the total array size would be about double of the
entities. Again, maybe not the best, but not too bad.

