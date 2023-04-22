#include "line_buffer.h"
#include <string.h>

bool line_buffer__init(line_buffer_s *buffer, void *memory, size_t size) {
  return queue__init((queue_s *)buffer, memory, size);
}

bool line_buffer__add_byte(line_buffer_s *buffer, char byte) { return queue__push((queue_s *)buffer, (uint8_t)byte); }

static bool line_buffer__find_linefeed_without_pop(line_buffer_s *buffer) {
  bool linefeed_found = false;
  queue_s *queue = (queue_s *)buffer;
  size_t index = queue->head;
  size_t queue_item_count = 0;
  size_t max;
  queue__get_max_size(queue, &max);
  queue__get_item_count(queue, &queue_item_count);

  for (size_t count = 0; count < queue_item_count; count++) {
    if (queue->static_memory_for_queue[index] == '\n') {
      linefeed_found = true;
      break;
    }
    index = (index + 1) % max; // to make sure that head wraps around after reaching the limit
  }
  return linefeed_found;
}

static void line_buffer__remove_line_until_first_linefeed(line_buffer_s *buffer, char *line, size_t line_max_size) {
  char popped_char = 0;
  int i = 0;
  while (i < line_max_size) {
    if (queue__pop((queue_s *)buffer, (uint8_t *)&popped_char)) {
      if (popped_char == '\n') {
        break;
      } else {
        line[i++] = popped_char;
      }
    }
  }
}

static bool line_buffer__is_buffer_full(line_buffer_s *buffer) { return queue__is_queue_full((queue_s *)buffer); }

static void line_buffer__remove_full_line(line_buffer_s *buffer, char *line, size_t line_max_size) {
  int i = 0;
  while (i < line_max_size) {
    queue__pop((queue_s *)buffer, (uint8_t *)&line[i++]);
  }
  line[line_max_size - 1] = 0; // to make the last char as null
}

bool line_buffer__remove_line(line_buffer_s *buffer, char *line, size_t line_max_size) {
  bool api_status = true, linefeed_found = false;
  if (buffer && line && line_max_size > 0) {
    memset(line, 0, line_max_size);
    linefeed_found = line_buffer__find_linefeed_without_pop(buffer);
    if (linefeed_found) {
      line_buffer__remove_line_until_first_linefeed(buffer, line, line_max_size);
    } else {
      if (line_buffer__is_buffer_full(buffer)) {
        line_buffer__remove_full_line(buffer, line, line_max_size);
      } else {
        api_status = false; // buffer is not full and linefeed is not found, so the application must wait and try again
      }
    }
  }
  return api_status;
}