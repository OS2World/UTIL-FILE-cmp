/*
   Recreation of err* , warn*  functions from FreeBSD library.

   Function implementation bodies were written from scratch using
   prototypes from  err.h file of FreeBSD distribution.

   This file is part of OS/2 port of cmp.exe utility.
   For conditions of use, modification and distribution of cmp.exe
   utility in binary/source form see Copyright notice from cmp.c 
    
   This source file is distributed under the terms of GNU GENERAL
   PUBLIC LICENSE. See file COPYING for details
   
   Written  by Yuri Baranov, 2001
*/
#include <err.h>
#include <stdio.h>
#include <stdarg.h>

void	err __P((int exit_code, const char * fmt, ...))
{
  va_list arg_ptr;

  va_start (arg_ptr, fmt);
  vfprintf (stderr, fmt, arg_ptr);
  putc ('\n', stderr);
  exit(exit_code);
}
void	errx __P((int exit_code, const char * fmt, ...))
{
  va_list arg_ptr;

  va_start (arg_ptr, fmt);
  vfprintf (stderr, fmt, arg_ptr);
  putc ('\n', stderr);
   exit(exit_code);
}
void	warnx __P((const char * fmt, ...))
{

  va_list arg_ptr;
  va_start (arg_ptr, fmt);
  vfprintf (stderr, fmt, arg_ptr);

}

