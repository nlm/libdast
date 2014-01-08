#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>
#include <assert.h>
#include "mtqueue.h"

struct mtqueue* mtqueue_new()
{
    struct mtqueue *mtqueue;

    mtqueue = malloc(sizeof(struct mtqueue));
    if (!mtqueue)
        return NULL;

    pthread_mutex_init(&mtqueue->lock, NULL);
    pthread_mutex_init(&mtqueue->mutex, NULL);
    pthread_cond_init(&mtqueue->cond, NULL);
    mtqueue->first = NULL;
    mtqueue->last = NULL;

    return mtqueue;
}

void mtqueue_free(struct mtqueue *mtqueue)
{
    pthread_cond_destroy(&mtqueue->cond);
    pthread_mutex_destroy(&mtqueue->mutex);
    pthread_mutex_destroy(&mtqueue->lock);
    free(mtqueue);
}

static void *mtqueue_getelt(struct mtqueue *mtqueue)
{
    struct mtqueueelt *elt = mtqueue->last;

    if (elt != NULL)
    {
        if (mtqueue->last->prev != NULL)
            mtqueue->last = mtqueue->last->prev;
        else
            mtqueue->last = mtqueue->first = NULL;
    }

    return elt;
}

void *mtqueue_get(struct mtqueue *mtqueue, unsigned int wait)
{
    void *data = NULL;
    struct mtqueueelt *elt;

    pthread_mutex_lock(&mtqueue->lock);
    elt = mtqueue_getelt(mtqueue);
    pthread_mutex_unlock(&mtqueue->lock);

    if (elt)
    {
        data = elt->data;
        free(elt);
    }
    else if (wait)
    {
        while (!elt)
        {
            pthread_mutex_lock(&mtqueue->mutex);
            pthread_cond_wait(&mtqueue->cond, &mtqueue->mutex);

            pthread_mutex_lock(&mtqueue->lock);
            elt = mtqueue_getelt(mtqueue);
            pthread_mutex_unlock(&mtqueue->lock);

            pthread_mutex_unlock(&mtqueue->mutex);
        }

        assert(elt);
        data = elt->data;
        free(elt);
        return data;
    }

    return data;
}

static int mtqueue_putelt(struct mtqueue *mtqueue, void *data)
{
    struct mtqueueelt *elt;

    elt = malloc(sizeof(struct mtqueueelt));
    if (elt == NULL)
        return 0;

    elt->data = data;
    elt->prev = NULL;

    if (mtqueue->first != NULL)
    {
        mtqueue->first->prev = elt;
        mtqueue->first = elt;
    }
    else
        mtqueue->first = mtqueue->last = elt;

    return 1;
}

int mtqueue_put(struct mtqueue *mtqueue, void *data)
{
    int rval;

    pthread_mutex_lock(&mtqueue->lock);
    rval = mtqueue_putelt(mtqueue, data);
    pthread_mutex_unlock(&mtqueue->lock);

    if (rval)
        pthread_cond_signal(&mtqueue->cond);

    return rval;
}
