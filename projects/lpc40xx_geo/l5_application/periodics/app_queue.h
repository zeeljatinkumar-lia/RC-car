#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/* In this part, the queue memory is statically defined
 * by the user and provided to you upon queue__init()
 */
typedef struct {
  uint8_t *static_memory_for_queue;
  size_t static_memory_size_in_bytes;
  uint8_t head, tail;
  size_t count;
} queue_s;

/* Initialize the queue with user provided static memory
 * @param static_memory_for_queue This memory pointer should not go out of scope
 *
 * @code
 *   static uint8_t memory[128];
 *   queue_s queue;
 *   queue__init(&queue, memory, sizeof(memory));
 * @endcode
 */
bool queue__init(queue_s *queue, void *static_memory_for_queue, size_t static_memory_size_in_bytes);

bool queue__is_queue_full(queue_s *queue);

/// @returns false if the queue is full
bool queue__push(queue_s *queue, uint8_t push_value);

/// @returns false if the queue was empty
/// Write the popped value to the user provided pointer pop_value_ptr
bool queue__pop(queue_s *queue, uint8_t *pop_value_ptr);

bool queue__get_item_count(const queue_s *queue, size_t *count);

bool queue__get_max_size(const queue_s *queue, size_t *max);
