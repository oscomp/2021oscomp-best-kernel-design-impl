
#ifndef INCLUDE_STRING_H_
#define INCLUDE_STRING_H_

#include <type.h>

void memcpy(uint8_t *dest, const uint8_t *src, uint32_t len);
void memset(void *dest, uint8_t val, uint32_t len);
void memmove(uint8_t *dest, const uint8_t *src, uint32_t len);
int memcmp(const void *ptr1, const void *ptr2, size_t num);
void bzero(void *dest, uint32_t len);
int strcmp(const char *str1, const char *str2);
char *strcpy(char *dest, const char *src);
char *strcat(char *dest, const char *src);
int strlen(const char *src);

#endif
