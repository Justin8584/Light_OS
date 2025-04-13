#ifndef INCLUDE_STRING_H
#define INCLUDE_STRING_H

#include <stddef.h>

size_t strlen(const char *str);
char *strcpy(char *dest, const char *src);
int strcmp(const char *str1, const char *str2);
void *memset(void *s, int c, size_t n);
int strncmp(const char *s1, const char *s2, size_t n);
char *strchr(const char *s, int c);
char *strcat(char *dest, const char *src);

#endif /* INCLUDE_STRING_H */
