#ifndef __URING_H__
# define __URING_H__

#include <stdlib.h>

struct uring
{
    size_t size;
    off_t ri;
    off_t wi;
    void **data;
};

struct uring* uring_new(size_t size);
void uring_free(struct uring *uring);
void *uring_get(struct uring *uring);
int uring_put(struct uring *uring, void *data);
void uring_dump(const struct uring *uring);

#endif /* __URING_H__ */
