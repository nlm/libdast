#include <stdlib.h>
#include <stdio.h>

#ifndef __UHASH_H__
# define __UHASH_H__

#define UHASH_DONT_FREE_KEYS 0x01
#define UHASH_DONT_FREE_DATA 0x02
#define UHASH_DONT_OVERWRITE 0x04

struct uhashelt
{
    void *key;
    void *data;
    struct uhashelt *next;
};

struct uhash
{
    struct uhashelt **data;
    size_t size;
    unsigned int flags;
    int (*keycmp)(const void *key1, const void *key2);
    size_t (*keyidx)(const void *key, size_t max);
};

struct uhash* uhash_new(size_t size, unsigned int flags,
    int (*keycmp)(const void *key1, const void *key2),
    size_t (*keyidx)(const void *key, size_t max));
int uhash_set(struct uhash *ht, void *key, void *data);
void *uhash_get(const struct uhash *ht, const void *key);
int uhash_delete(struct uhash *ht, const void *key);
int uhash_free(struct uhash *ht);
void uhash_dump(const struct uhash *ht,
    void (*keydump)(const void *key),
    void (*datadump)(const void *data));

#endif
