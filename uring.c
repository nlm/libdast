#include <stdlib.h>
#include <stdio.h>
#include "uring.h"

struct uring* uring_new(size_t size)
{
    struct uring *uring;

    uring = malloc(sizeof(struct uring));
    if (!uring)
        return NULL;

    uring->data = calloc(size, sizeof(void *));
    if (!uring->data)
    {
        free(uring);
        return NULL;
    }

    uring->size = size;
    uring->wi = 0;
    uring->ri = 0;

    return uring;
}

void uring_free(struct uring *uring)
{
    free(uring->data);
    free(uring);
}

void *uring_get(struct uring *uring)
{
    void *data;

    if (uring->ri == uring->wi)
        return NULL;

    data = uring->data[uring->ri];
    uring->ri = (uring->ri + 1) % uring->size;
    return data;
}

int uring_put(struct uring *uring, void *data)
{
    off_t new_wi = (uring->wi + 1) % uring->size;

    if (new_wi == uring->ri)
        return 0;

    uring->data[uring->wi] = data;
    uring->wi = new_wi;

    return 1;
}

void uring_dump(const struct uring *uring)
{
    printf("uring: %zu slots (ri=%zu wi=%zu)\n", uring->size, uring->ri, uring->wi);
}
