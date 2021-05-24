#include <string.h>
#include <pgtable.h>

int strlen(const char *src)
{
    int i;
    for (i = 0; src[i] != '\0'; i++) {
    }
    return i;
}

void memcpy(uint8_t *dest, const uint8_t *src, uint32_t len)
{
    for (; len != 0; len--) {
        *dest++ = *src++;
    }
}

int memcmp(const void *ptr1, const void *ptr2, size_t num)
{
    for (int i = 0; i < num; ++i) {
        if (((char*)ptr1)[i] != ((char*)ptr2)[i]) {
            return ((char*)ptr1)[i] - ((char*)ptr2)[i];
        }
    }
    return 0;
}

void memmove(uint8_t *dest, const uint8_t *src, uint32_t len)
{
    uint8_t temp[len];
    int32_t i;
    for (i = 0; len != 0; len--) {
        temp[i] = *src++;
    }
    for (; len != 0; len--) {
        *dest++ = temp[i];
    }
}

void memset(void *dest, uint8_t val, uint32_t len)
{
    uint8_t *dst = (uint8_t *)dest;

    for (; len != 0; len--) {
        *dst++ = val;
    }
}

void bzero(void *dest, uint32_t len) { memset(dest, 0, len); }

int strcmp(const char *str1, const char *str2)
{
    while (*str1 && *str2) {
        if (*str1 != *str2) {
            return (*str1) - (*str2);
        }
        ++str1;
        ++str2;
    }
    return (*str1) - (*str2);
}

char *strcpy(char *dest, const char *src)
{
    char *tmp = dest;

    while (*src) {
        *dest++ = *src++;
    }

    *dest = '\0';

    return tmp;
}

char *strcat(char *dest, const char *src)
{
    char *tmp = dest;

    while (*dest != '\0') { dest++; }
    while (*src) {
        *dest++ = *src++;
    }

    *dest = '\0';

    return tmp;
}