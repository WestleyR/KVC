#
#  Makefile
#
# Created by WestleyR on 2021-03-14
# Source code: https://github.com/WestleyR/KVC (key-value in C)
#
# Copyright (c) 2021 WestleyR. All rights reserved.
# This software is licensed under a BSD 3-Clause Clear License.
# Consult the LICENSE file that came with this software regarding
# your rights to distribute this software.
#

# The prefix to install the gpack command. This should
# not be changed, unless you know what your doing.
PREFIX = $(HOME)/.local

# The C compiler
CC = gcc

# These flags can be changed though the command line
CFLAGS = -Wall

TARGET = test

SRC = $(wildcard ./*.c)

OBJS = $(SRC:.c=.o)

.PHONY:
all: $(TARGET)

.PHONY:
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LDFLAGS) -o $(TARGET)

.PHONY:
%.o: %.c
	$(CC) $(DEP_FLAG) $(CFLAGS) $(DEFLAGS) -o $@ -c $< $(LDFLAGS)

.PHONY:
clean:
	rm -f $(OBJS)

