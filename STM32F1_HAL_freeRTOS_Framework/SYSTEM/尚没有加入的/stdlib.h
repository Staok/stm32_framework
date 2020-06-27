/**
 * @file stdlib.h
 * @version 3.01[By LPLD]
 * @date 2013-11-4
 * @brief C标准代码库
 *
 * 更改建议:不建议修改
 *
 * 由标准C代码库移植，仅支持ASCII
 *
 * 版权所有:北京拉普兰德电子技术有限公司
 * http://www.lpld.cn
 * mail:support@lpld.cn
 *
 * @par
 * 本代码由拉普兰德[LPLD]开发并维护，并向所有使用者开放源代码。
 * 开发者可以随意修使用或改源代码。但本段及以上注释应予以保留。
 * 不得更改或删除原版权所有者姓名，二次开发者可以加注二次版权所有者。
 * 但应在遵守此协议的基础上，开放源代码、不得出售代码本身。
 * 拉普兰德不负责由于使用本代码所带来的任何事故、法律责任或相关不良影响。
 * 拉普兰德无义务解释、说明本代码的具体原理、功能、实现方法。
 * 除非拉普兰德[LPLD]授权，开发者不得将本代码用于商业产品。
 */

#ifndef _STDLIB_H
#define _STDLIB_H
/********************************************************************
 * Standard library functions
 ********************************************************************/

int isspace (int);

int isalnum (int);

int isdigit (int);

int isupper (int);

int strcasecmp (const int8 *, const int8 *);

int strncasecmp (const int8 *, const int8 *, int);

unsigned long strtoul (int8 *, int8 **, int);

int strlen (const int8 *);

int8 * strcat (int8 *, const int8 *);

int8 * strncat (int8 *, const int8 *, int);

int8 * strcpy (int8 *, const int8 *);

int8 * strncpy (int8 *, const int8 *, int);

int strcmp (const int8 *, const int8 *);

int strncmp (const int8 *, const int8 *, int);

void * memcpy (void *, const void *, unsigned);

void * memset (void *, int, unsigned);

// void free (void *);
 
// void * malloc (unsigned);

/********************************************************************/

#endif
