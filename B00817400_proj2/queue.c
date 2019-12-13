#include <assert.h>
#include <stddef.h>

#include "queue.h"
#include "reorder_buffer.h"

void queue_init(Queue *queue) {
    assert(queue);

    queue->head = NULL;
    queue->tail = NULL;
    queue->size = 0;
}

QueueNode* queue_peek(const Queue *queue) {
    assert(queue);

    return queue->head;
}

size_t queue_size(const Queue *queue) {
    assert(queue);

    return queue->size;
}

int queue_empty(const Queue *queue) {
    assert(queue);

    return queue->size == 0;
}

void queue_push(Queue *queue, QueueNode *node) {
    assert(queue && node);

    if (!queue->head) {
        queue->head = node;
    } else {
        queue->tail->next = node;
    }

    queue->tail = node;
    node->next = NULL;

    ++queue->size;
}

QueueNode *queue_pop(Queue *queue) {
    QueueNode *n;

    assert(queue);

    n = queue->head;

    if (!n) {
        return NULL;
    }

    if (!n->next) {
        queue->tail = NULL;
    }

    queue->head = n->next;
    n->next = QUEUE_POISON_NEXT;

    --queue->size;

    return n;
}

void queue_remove_all(Queue *queue) {
    assert(queue);

    if (queue->head) {
        queue->head->next = QUEUE_POISON_NEXT;
        queue->tail->next = QUEUE_POISON_NEXT;
    }

    queue->head = NULL;
    queue->tail = NULL;
    queue->size = 0;
}