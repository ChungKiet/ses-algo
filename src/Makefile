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
LFLAGS = -lpthread

.PHONY: all clean makeobjdir
all: makeobjdir main.o \
	timevec.o sentvec.o \
	threads.o logs.o config.o ses.o
	$(CC) $(LFLAGS) $(OBJDIR)main.o \
		$(OBJDIR)timevec.o $(OBJDIR)sentvec.o $(OBJDIR)threads.o \
		$(OBJDIR)logs.o $(OBJDIR)config.o $(OBJDIR)ses.o \
		-o 1512284

main.o: main.c $(INCDIR)defs.h config.o threads.o ses.o
	$(CC) $(CFLAGS) -c main.c -o $(OBJDIR)main.o

timevec.o: timevec.c $(INCDIR)defs.h $(INCDIR)timevec.h
	$(CC) $(CFLAGS) -c timevec.c -o $(OBJDIR)timevec.o

sentvec.o: sentvec.c $(INCDIR)defs.h $(INCDIR)sentvec.h timevec.o
	$(CC) $(CFLAGS) -c sentvec.c -o $(OBJDIR)sentvec.o

threads.o: threads.c $(INCDIR)defs.h $(INCDIR)threads.h \
	sentvec.o timevec.o logs.o
	$(CC) $(CFLAGS) -c threads.c -o $(OBJDIR)threads.o

logs.o: logs.c $(INCDIR)defs.h $(INCDIR)logs.h
	$(CC) $(CFLAGS) -c logs.c -o $(OBJDIR)logs.o

config.o: config.c $(INCDIR)defs.h $(INCDIR)config.h
	$(CC) $(CFLAGS) -c config.c -o $(OBJDIR)config.o

ses.o: ses.c $(INCDIR)defs.h $(INCDIR)ses.h \
	timevec.o sentvec.o logs.o
	$(CC) $(CFLAGS) -c ses.c -o $(OBJDIR)ses.o

makeobjdir:
	mkdir -p obj

clean:
	rm -r ./obj 1512284
