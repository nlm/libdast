#ifndef __MTQUEUE_H__
# define __MTQUEUE_H__

#include <pthread.h>

struct mtqueueelt
{
    void *data;
    struct mtqueueelt *prev;
};

struct mtqueue
{
    pthread_cond_t cond;
    pthread_mutex_t mutex;
    pthread_mutex_t lock;
    size_t size;
    struct mtqueueelt *first;
    struct mtqueueelt *last;
};

struct mtqueue* mtqueue_new();
void mtqueue_free(struct mtqueue *mtqueue);
void *mtqueue_get(struct mtqueue *mtqueue, unsigned int wait);
int mtqueue_put(struct mtqueue *mtqueue, void *data);

#endif /* __MTQUEUE_H__ */
