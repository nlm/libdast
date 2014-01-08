#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "uhash.h"

struct uhash* uhash_new(size_t size, unsigned int flags,
    int (*keycmp)(const void *key1, const void *key2),
    size_t (*keyidx)(const void *key, size_t max))
{
    struct uhash *new;

    if (size == 0 || keycmp == NULL || keyidx == NULL)
        return NULL;

    new = malloc(sizeof(struct uhash));
    if (new == NULL)
        return NULL;

    new->data = calloc(size, sizeof(struct uhashelt *));
    if (new->data == NULL)
    {
        free(new);
        return NULL;
    }

    new->size = size;
    new->flags = flags;
    new->keycmp = keycmp;
    new->keyidx = keyidx;

    return new;
}

int uht_set(struct uhash *ht, void *key, void *data)
{
    size_t idx;
    struct uhashelt *he, *hi, **hp;

    if (ht == NULL)
        return -1;

    he = malloc(sizeof(struct uhashelt));
    if (he == NULL)
        return -1;
    he->key = key;
    he->data = data;
    he->next = NULL;

    idx = ht->keyidx(key, ht->size);
    if (idx > ht->size)
        return -2;

    hi = ht->data[idx];
    hp = &ht->data[idx];

    while (hi != NULL)
    {
        switch (ht->keycmp(hi->key, he->key))
        {
            case 0:
                if (ht->flags & UHASH_DONT_OVERWRITE)
                    return -3;
                he->next = hi->next;
                *hp = he;
                if (!(ht->flags & UHASH_DONT_FREE_KEYS))
                    free(hi->key);
                if (!(ht->flags & UHASH_DONT_FREE_DATA))
                    free(hi->data);
                free(hi);
                return 1;
            case 1:
                he->next = hi;
                *hp = he;
                return 1;
            case -1:
                hp = &hi->next;
                hi = hi->next;
        }
    }

    *hp = he;
    return 1;
}

void *uht_get(const struct uhash *ht, const void *key)
{
    size_t idx;
    struct uhashelt *hi;

    if (ht == NULL)
        return NULL;

    idx = ht->keyidx(key, ht->size);
    if (idx > ht->size)
        return NULL;

    hi = ht->data[idx];

    while (hi != NULL)
    {
        switch(ht->keycmp(hi->key, key))
        {
            case 0:
                return hi->data;
            case 1:
                return NULL;
            case -1:
                hi = hi->next;
        }
    }

    return NULL;
}

int uht_delete(struct uhash *ht, const void *key)
{
    size_t idx;
    struct uhashelt *hi, **hp;

    if (ht == NULL)
        return -1;

    idx = ht->keyidx(key, ht->size);
    if (idx > ht->size)
        return -1;

    hi = ht->data[idx];
    hp = &ht->data[idx];

    while (hi != NULL)
    {
        switch (ht->keycmp(hi->key, key))
        {
            case 0:
                if (!(ht->flags & UHASH_DONT_FREE_KEYS))
                    free(hi->key);
                if (!(ht->flags & UHASH_DONT_FREE_DATA))
                    free(hi->data);
                *hp = hi->next;
                free(hi);
                return 1;
            case 1:
                return 0;
            case -1:
                hp = &hi->next;
                hi = hi->next;
        }
    }

    return 0;
}

inline size_t uht_size(const struct uhash *ht)
{
    if (ht == NULL)
        return 0;
    return ht->size;
}

/*
int uht_resize(struct uhashtable *ht, size_t newsize)
{
    struct uhashelt *hi, *hin, *newhi, **newhp;
    struct uhashelt **data, **newdata;
    size_t newidx;
    int i;

    if (ht == NULL)
        return -1;

    if (size < 1)
        return -1;

    newdata = calloc(size, sizeof(struct uhashelt *));
    if (newdata == NULL)
        return -1;

    for (i = 0; i < ht->size; i++)
    {
        hi = ht->data[i];
        while (hi != NULL)
        {
            hin = hi->next;

            newidx = ht->keyidx(hi->key, newsize);
            if (idx > ht->size)
                return -2;
            
            newhi = newdata[idx];
            newhp = &newdata[idx];

            while (newhi != NULL)
            {
                switch (ht->keycmp(hi->key, newhi->key))
                {
                    case 0:
                        break;
                    case 1:
                        hi->next = newhi;
                        *hp = hi;
                        return 1;
                    case -1:
                        hp = &newhi->next;
                        newhi = newhi->next;
                }
            }

            *hp = hi;
            return 1;

            hi = hin;
        }
    }
}
*/

int uht_free(struct uhash *ht)
{
    size_t i;
    struct uhashelt *hi, *hn;

    if (ht == NULL)
        return -1;

    /* free the elements */
    for (i = 0; i < ht->size; i++)
    {
        hi = ht->data[i];
        while (hi != NULL)
        {
            if (!(ht->flags & UHASH_DONT_FREE_KEYS))
                free(hi->key);
            if (!(ht->flags & UHASH_DONT_FREE_DATA))
                free(hi->data);
            hn = hi->next;
            free(hi);
            hi = hn;
        }
    }

    free(ht->data);
    free(ht);

    return 1;
}

void uht_dump(const struct uhash *ht,
    void (*keydump)(const void *key), void (*datadump)(const void *data))
{
    size_t i;
    struct uhashelt *hi;

    if (ht == NULL)
        return; 

    printf("HASHTABLE elts=%zd\n", ht->size);

    for (i = 0; i < ht->size; i++)
    {
        hi = ht->data[i];
        if (hi)
        {
            printf("idx(key=%zd)", i);
            while (hi != NULL)
            {
                printf("->elt(key=");
                if (keydump)
                    keydump(hi->key);
                else
                    printf("%p", hi->key);
                printf(",data=");
                if (datadump)
                    datadump(hi->data);
                else
                    printf("%p", hi->data);
                printf(")");
                hi = hi->next;
            }
            printf("\n");
        }
    }
}
