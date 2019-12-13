#ifndef QUEUE_H
#define QUEUE_H

#ifdef __cplusplus
extern "C" {
#endif 

#include <stddef.h>

/* Struct type declarations. */
struct Queue;
struct QueueNode;

/* Struct typedef's. */
typedef struct Queue Queue;
typedef struct QueueNode QueueNode;

/**
 * Represents a queue.
 */
struct Queue {
    QueueNode *head;
    QueueNode *tail;
    size_t size;
};

struct QueueNode {
    QueueNode *next;
};

void queue_init(Queue *queue);

QueueNode* queue_peek(const Queue *queue);

size_t queue_size(const Queue *queue);

int queue_empty(const Queue *queue);

void queue_push(Queue *queue, QueueNode *node);

QueueNode* queue_pop(Queue *queue);

void queue_remove_all(Queue *queue);

#define QUEUE_POISON_NEXT ((QueueNode*) 0x100)

#define QUEUE_NODE_INIT { QUEUE_POISON_NEXT }

#if defined(__GNUC__) && !defined(__STRICT_ANSI__)
    #define queue_entry(node_ptr, type, member) \
        ({ \
            const typeof(((type*)0)->member) *__mptr = (node_ptr); \
            (type*) ((char*)__mptr - offsetof(type, member)); \
        })
#else
    #define queue_entry(node_ptr, type, member) \
        ( \
            (type*) ((char*)(node_ptr) - offsetof(type, member)) \
        )
#endif

#define queue_for_each(cursor_node_ptr, queue_ptr) \
    for ( \
        cursor_node_ptr = (queue_ptr)->head; \
        cursor_node_ptr; \
        cursor_node_ptr = cursor_node_ptr->next \
    )

#define queue_for_each_safe(cursor_node_ptr, backup_node_ptr, queue_ptr) \
    for ( \
        cursor_node_ptr = (queue_ptr)->head, \
        backup_node_ptr = cursor_node_ptr ? cursor_node_ptr->next : NULL; \
        \
        cursor_node_ptr; \
        \
        cursor_node_ptr = backup_node_ptr, \
        backup_node_ptr = cursor_node_ptr ? cursor_node_ptr->next : NULL \
    )

#ifdef __cplusplus
}
#endif 

#endif 