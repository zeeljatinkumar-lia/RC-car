#include "line_buffer.h"
#include "stdio.h"
#include <string.h>

static bool is_circular_queue_empty(line_buffer_s *buffer) {
  if (buffer->read_index == -1) {
    // fprintf(stderr, "read_index is -1\n");
    return true;
  } else {
    // printf(stderr, "read_index is %d\n", buffer->read_index);
    return false;
  }
}

static bool is_circular_queue_full(line_buffer_s *buf) {
  if (buf->read_index == buf->write_index) {
    return true;
  } else {
    return false;
  }
}

static int scan_for_new_line_token(line_buffer_s *buffer, char token) {
  char *new_line_location;
  int return_val = -1;
  int read_i = 0;

  if (!is_circular_queue_empty(buffer)) {
    for (int i = 0; i < buffer->max_size; i++) {
      read_i = (buffer->read_index + i) % buffer->max_size;

      if (read_i != buffer->write_index) {
        //   fprintf(stderr, "scan_for_new_line_token: mem is %c\n", *((char *)(buffer->memory) + read_i));
        if ((((char *)buffer->memory)[read_i]) == '\n') {
          return_val = read_i;
          break;
        }
      } else {
        break;
      }
    }
  }

  // printf("scan_for_new_line_token: %d\n", return_val);

  return return_val;
}

static bool enqueue(line_buffer_s *buffer, char byte) {
  if (is_circular_queue_full(buffer)) {
    return false;
  }
  *((char *)(buffer->memory) + buffer->write_index) = byte;
  // printf("enqueued element is : %c %p\n", *((char *)(buffer->memory) + buffer->write_index),
  //  ((char *)(buffer->memory) + buffer->write_index));
  buffer->write_index = (buffer->write_index + 1) % buffer->max_size;
  // printf("buffer->write index: %d\n", buffer->write_index);
  if (is_circular_queue_empty(buffer)) {
    buffer->read_index++;
  }
  return true;
}

static bool dequeue(line_buffer_s *buffer, char *byte) {
  if (!is_circular_queue_empty(buffer)) {
    *byte = ((char *)buffer->memory)[buffer->read_index];
    //  fprintf(stderr, "dequeued element is : %c\n", *byte);
    buffer->read_index = (buffer->read_index + 1) % buffer->max_size;
    //  printf(stderr, "buffer->read_index: %d\n", buffer->read_index);
    if (buffer->read_index == buffer->write_index) {
      buffer->read_index = -1;
      buffer->write_index = 0;
    }
    return true;
  }
  return false;
}

void line_buffer__init(line_buffer_s *buffer, void *memory, size_t size) {
  if (memory != NULL) {
    buffer->memory = memory;
    buffer->read_index = -1; // tail
    buffer->write_index = 0; // head
    buffer->max_size = size;
  }
}
bool line_buffer__add_byte(line_buffer_s *buffer, char byte) { return enqueue(buffer, byte); }

bool dequeue_multiple(line_buffer_s *buffer, char *buf, size_t deq_num) {
  for (int i = 0; i < deq_num; i++) {
    if (!dequeue(buffer, buf + i)) {
      return false;
    }
  }

  return true;
}

bool line_buffer__remove_line(line_buffer_s *buffer, char *line, size_t line_max_size) {
  bool return_value = false;
  int new_line_location = -1;
  const char new_line_token = '#';
  // memset(line, 0, line_max_size);

  // check line is null and line_max_size is greater 0
  if (line == NULL || line_max_size == 0) {
    // do nothing
  }
  // check buffer is not empty
  /*else if (-1 != (new_line_location = scan_for_new_line_token(buffer, new_line_token))) {
    if (new_line_location < buffer->read_index) {
      int temp = buffer->max_size - buffer->read_index + new_line_location;
      if ((temp) < line_max_size) {
        return_value = dequeue_multiple(buffer, line, temp);
        (void)dequeue(buffer, &new_line_location);
      }
    } else if (new_line_location - buffer->read_index < line_max_size) {
      return_value = dequeue_multiple(buffer, line, new_line_location - buffer->read_index);
      (void)dequeue(buffer, &new_line_location);
    }

  } else if (is_circular_queue_full(buffer)) {
    return_value = dequeue_multiple(buffer, line, line_max_size - 1);
    (void)dequeue(buffer, &new_line_location);
  }*/
  return_value = dequeue_multiple(buffer, line, line_max_size - 1);
  (void)dequeue(buffer, &new_line_location);
  return return_value;
}
