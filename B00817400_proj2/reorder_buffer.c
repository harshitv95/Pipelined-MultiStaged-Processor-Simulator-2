#include "reorder_buffer.h"
#include <stdbool.h>

void init(struct queue *reorderBuffer)
{
  reorderBuffer -> head = -1;
  reorderBuffer -> tail = 0;
}


bool isFull(struct queue* reorderBuffer) {
  if(reorderBuffer->tail == reorderBuffer->head){
      printf("Queue empty.\n");
      return 1;
  } else {
    return 0;
  }
}

int enqueue(struct queue * reorderBuffer, int element) {
  if (reorderBuffer -> tail == 7 && isFull(reorderBuffer)) {
    // stall
  } else if (reorderBuffer -> tail == 7 ) {
    reorderBuffer -> tail = 0;
  } else {
    reorderBuffer-> array[reorderBuffer->tail] = element;
    (reorderBuffer->tail)++;
  }
}


int dequeue(struct queue *reorderBuffer, int *status)
{
  if(reorderBuffer-> head == -1 && reorderBuffer -> tail == 0){
      *status=0;
      return -1;
  } else{
      *status=1;
      int item = reorderBuffer->array[++reorderBuffer->head%size];
      return item;
  }
}

int _is_head_valid() {

}