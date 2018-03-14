# Project 2 - Advanced Operating System
# CAUSAL MESSAGE ORDERING IN DISTRIBUTED SYSTEM
# Using Schiper-Eggli-Sandoz Algorithm
#
# 1512284 - Ha Tan Linh
# Email: hatanlinh13@gmail.com
#
# Module: Makefile

CC     = gcc
INCDIR = ./include/
OBJDIR = ./obj/
INCOPT = -I./include
CFLAGS = $(INCOPT)
LFLAGS =

.PHONY: all clean makeobjdir
all: makeobjdir main.o \
	timevec.o sentvec.o
	$(CC) $(LFLAGS) $(OBJDIR)main.o \
		$(OBJDIR)timevec.o $(OBJDIR)sentvec.o \
		-o 1512284

main.o: main.c $(INCDIR)defs.h
	$(CC) $(CFLAGS) -c main.c -o $(OBJDIR)main.o

timevec.o: timevec.c $(INCDIR)timevec.h
	$(CC) $(CFLAGS) -c timevec.c -o $(OBJDIR)timevec.o

sentvec.o: sentvec.c $(INCDIR)sentvec.h $(INCDIR)defs.h
	$(CC) $(CFLAGS) -c sentvec.c -o $(OBJDIR)sentvec.o

makeobjdir:
	mkdir -p obj

clean:
	rm -r ./obj 1512284
