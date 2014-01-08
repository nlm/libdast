#include <stdio.h>
#include <string.h>
#include <stdint.h>

inline size_t keycmp_uint32(const void *key1, const void *key2)
{
    uint32_t *i1 = (uint32_t *) key1;
    uint32_t *i2 = (uint32_t *) key2;

    if (*i1 == *i2)
        return 0;
    else if (*i1 > *i2)
        return 1;
    else
        return -1;
}

inline size_t keyidx_uint32(const void *key, size_t max)
{
    uint32_t *i = (uint32_t *) key;

    return *i % max;
}

inline size_t keycmp_uint64(const void *key1, const void *key2)
{
    uint64_t *i1 = (uint64_t *) key1;
    uint64_t *i2 = (uint64_t *) key2;

    if (*i1 == *i2)
        return 0;
    else if (*i1 > *i2)
        return 1;
    else
        return -1;
}

inline size_t keyidx_uint64(const void *key, size_t max)
{
    uint64_t *i = (uint64_t *) key;

    return *i % max;
}

inline size_t keycmp_int(const void *key1, const void *key2)
{
    int *i1 = (int *) key1;
    int *i2 = (int *) key2;

    if (*i1 == *i2)
        return 0;
    else if (*i1 > *i2)
        return 1;
    else
        return -1;
}

inline size_t keyidx_int(const void *key, size_t max)
{
    int *i = (int *) key;

    return *i % max;
}

inline size_t keycmp_str(const void *key1, const void *key2)
{
    char *s1 = (char *) key1;
    char *s2 = (char *) key2;

    return strcmp(s1, s2);
}

inline size_t keyidx_str(const void *key, size_t max)
{
    char *s = (char *) key;
    int val = 0;

    while (s && *s)
        val += *s++;

    return val % max;
}

inline void dump_int(const void *val)
{
    printf("%d", *(int *) val);
}

inline void dump_str(const void *str)
{
    printf("\"%s\"", (char *) str);
}
