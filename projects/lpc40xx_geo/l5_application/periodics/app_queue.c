#include "app_queue.h"

// This should initialize all members of queue_s
bool queue__init(queue_s *queue, void *static_memory_for_queue, size_t static_memory_size_in_bytes) {
  bool init_status = false;
  if (queue && static_memory_for_queue && static_memory_size_in_bytes) {
    queue->static_memory_for_queue = static_memory_for_queue;
    queue->static_memory_size_in_bytes = static_memory_size_in_bytes;
    queue->head = 0;
    queue->tail = 0;
    queue->count = 0;
    init_status = true;
  }
  return init_status;
}

bool queue__get_max_size(const queue_s *queue, size_t *max) {
  bool api_status = false;
  if (queue && max) {
    *max = queue->static_memory_size_in_bytes;
    api_status = true;
  }
  return api_status;
}

static bool queue__is_queue_empty(queue_s *queue) {
  bool api_status = false;
  if (queue && queue->count == 0) {
    api_status = true;
  }
  return api_status;
}

/// @returns true if the queue is full
bool queue__is_queue_full(queue_s *queue) {
  bool api_status = false;
  size_t max = 0;
  if (queue) {
    queue__get_max_size(queue, &max);
    if (queue->count == max) {
      api_status = true;
    }
  }
  return api_status;
}

/// @returns false if the queue is full
bool queue__push(queue_s *queue, uint8_t push_value) {
  bool push_status = false;
  if (queue == NULL || queue__is_queue_full(queue)) {
    push_status = false;
  } else {
    size_t max;
    queue__get_max_size(queue, &max);
    queue->static_memory_for_queue[queue->tail] = push_value;
    queue->tail = (queue->tail + 1) % max; // to make sure that tail wraps
                                           // around after reaching the limit
    queue->count++;
    push_status = true;
  }
  return push_status;
}

/// @returns false if the queue was empty
bool queue__pop(queue_s *queue, uint8_t *pop_value) {
  bool pop_status = false;
  if (queue == NULL || pop_value == NULL || queue__is_queue_empty(queue)) {
    pop_status = false;
  } else {
    size_t max;
    queue__get_max_size(queue, &max);
    *pop_value = queue->static_memory_for_queue[queue->head];
    queue->head = (queue->head + 1) % max; // to make sure that head wraps
                                           // around after reaching the limit
    queue->count--;
    pop_status = true;
  }
  return pop_status;
}

bool queue__get_item_count(const queue_s *queue, size_t *count) {
  bool api_status = false;
  if (queue && count) {
    *count = queue->count;
    api_status = true;
  }
  return api_status;
}
