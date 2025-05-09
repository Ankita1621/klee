//===-- fortify-klibc.c ---------------------------------------------------===//
//
//                     The KLEE Symbolic Virtual Machine
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

/* Fortified versions of the libc functions defined in the klee-libc library */

#include "klee/klee.h"

#include <stddef.h>

void *mempcpy(void *destaddr, void const *srcaddr, size_t len);

void *__mempcpy_chk(void *dest, const void *src, size_t len, size_t destlen) {
  if (len > destlen)
    klee_report_error(__FILE__, __LINE__, "mempcpy overflow", "ptr.err");

  return mempcpy(dest, src, len);
}

char *stpcpy(char * to, const char * from);

char *__stpcpy_chk(char *dest, const char *src, size_t destlen) {
  return stpcpy(dest, src);
}

char *strcat(char * s, const char * append);

char *__strcat_chk(char *dest, const char *src, size_t destlen) {
  return strcat(dest, src);
}

char *strcpy(char *to, const char *from);

char *__strcpy_chk(char *dest, const char *src, size_t destlen) {
  return strcpy(dest, src);
}

char *strncpy(char * dst, const char * src, size_t n);

char *__strncpy_chk(char *s1, const char *s2, size_t n, size_t s1len) {
  return strncpy(s1, s2, n);
}
