#
# Makefile for compiling cmp.exe with pgcc/EMX
# Written by Yuri Baranov, Feb 2001
#
# NO WARRANTIES WHATSOEVER. Just worked for me.
# 

cmp.exe: cmp.c noregular.c special.c misc.c include/err.h \
         include/machine/ansi.h include/sys/cdefs.h extern.h
	gcc  -Iinclude -o cmp.exe cmp.c noregular.c \
	special.c misc.c err.c