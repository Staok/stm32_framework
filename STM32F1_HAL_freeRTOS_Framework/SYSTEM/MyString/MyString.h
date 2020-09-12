#ifndef __MYSTRING_H
#define __MYSTRING_H

#include "sysVar.h"

int mystrlen (const char *str);
char * mystrcat (char *dest, const char *src);
char * mystrncat (char *dest, const char *src, int n);
char * mystrcpy (char *dest, const char *src);
char * mystrncpy (char *dest, const char *src, int n);
int mystrcmp (const char *s1, const char *s2);
int mystrncmp (const char *s1, const char *s2, int n);
void * mymemset (void *s, int c, unsigned n);
void *memcpy(void *des,const void *src,size_t len);


#endif


