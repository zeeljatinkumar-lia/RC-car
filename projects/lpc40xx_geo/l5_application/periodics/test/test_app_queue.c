#include "unity.h"

// Include the source we wish to test
#include "app_queue.h"

#define MAX_QUEUE_SIZE 128
static queue_s queue;
static uint8_t queue_memory[MAX_QUEUE_SIZE];

void setUp(void) {}

void tearDown(void) {}

void test_queue_init_with_null() {
  TEST_ASSERT_FALSE(queue__init(&queue, queue_memory, 0));            // for size 0
  TEST_ASSERT_FALSE(queue__init(NULL, queue_memory, MAX_QUEUE_SIZE)); // for NULL queue
  TEST_ASSERT_FALSE(queue__init(&queue, NULL, MAX_QUEUE_SIZE));       // for NULL queue_memory
  TEST_ASSERT_FALSE(queue__init(NULL, NULL, 0));                      // for all NULL and 0
}

void test_queue_init() {
  TEST_ASSERT_TRUE(queue__init(&queue, queue_memory, sizeof(queue_memory)));
  TEST_ASSERT_EQUAL(0, queue.head);
  TEST_ASSERT_EQUAL(0, queue.tail);
  TEST_ASSERT_EQUAL(0, queue.count);
  TEST_ASSERT_EQUAL(MAX_QUEUE_SIZE, queue.static_memory_size_in_bytes);
  // TODO: can you check memory addresses??
}

void test_queue_get_max_size_with_null() {
  size_t max;
  TEST_ASSERT_FALSE(queue__get_max_size(&queue, NULL));
  TEST_ASSERT_FALSE(queue__get_max_size(NULL, &max));
  TEST_ASSERT_FALSE(queue__get_max_size(NULL, NULL));
}

void test_queue_get_max_size() {
  size_t max;
  queue__init(&queue, queue_memory, sizeof(queue_memory));
  queue__get_max_size(&queue, &max);
  TEST_ASSERT_EQUAL(MAX_QUEUE_SIZE, max);
}

void test_queue_push_with_null() { TEST_ASSERT_FALSE(queue__push(NULL, 123)); }

void test_queue_push() {
  queue__init(&queue, queue_memory, sizeof(queue_memory));
  TEST_ASSERT_TRUE(queue__push(&queue, 123));
  TEST_ASSERT_EQUAL(0, queue.head);
  TEST_ASSERT_EQUAL(1, queue.tail);
  TEST_ASSERT_EQUAL(1, queue.count);
  TEST_ASSERT_EQUAL(123, queue.static_memory_for_queue[queue.head]);
}

void test_get_count_with_null() {
  size_t count;
  TEST_ASSERT_FALSE(queue__get_item_count(NULL, &count));
  TEST_ASSERT_FALSE(queue__get_item_count(&queue, NULL));
  TEST_ASSERT_FALSE(queue__get_item_count(NULL, NULL));
}

void test_get_count() {
  size_t count;
  queue__init(&queue, queue_memory, sizeof(queue_memory));
  queue__push(&queue, 123);
  TEST_ASSERT_TRUE(queue__get_item_count(&queue, &count));
  TEST_ASSERT_EQUAL(1, count);
}

void test_queue_push_increments_the_count() {
  size_t count;
  queue__init(&queue, queue_memory, sizeof(queue_memory));
  queue__get_item_count(&queue, &count);
  TEST_ASSERT_EQUAL(0, count);
  queue__push(&queue, 123);
  queue__get_item_count(&queue, &count);
  TEST_ASSERT_EQUAL(1, count);
}

void test_pop_with_null() {
  uint8_t pop_value;
  TEST_ASSERT_FALSE(queue__pop(NULL, &pop_value));
  TEST_ASSERT_FALSE(queue__pop(&queue, NULL));
  TEST_ASSERT_FALSE(queue__pop(NULL, NULL));
}

void test_queue_pop() {
  uint8_t pop_value;
  queue__init(&queue, queue_memory, sizeof(queue_memory));
  queue__push(&queue, 123);
  TEST_ASSERT_TRUE(queue__pop(&queue, &pop_value));
  TEST_ASSERT_EQUAL(123, pop_value);
  TEST_ASSERT_EQUAL(1, queue.head);
  TEST_ASSERT_EQUAL(1, queue.tail);
  TEST_ASSERT_EQUAL(0, queue.count);
}

void test_queue_pop_decrements_the_count() {
  queue__init(&queue, queue_memory, sizeof(queue_memory));
  uint8_t pop_value;
  size_t count;
  queue__push(&queue, 123);
  queue__get_item_count(&queue, &count);
  TEST_ASSERT_EQUAL(1, count);
  queue__pop(&queue, &pop_value);
  queue__get_item_count(&queue, &count);
  TEST_ASSERT_EQUAL(0, count);
}

void test_comprehensive(void) {
  const size_t max_queue_size = MAX_QUEUE_SIZE; // Change if needed
  size_t current_count;
  queue__init(&queue, queue_memory, sizeof(queue_memory));

  for (size_t item = 0; item < max_queue_size; item++) {
    const uint8_t item_pushed = (uint8_t)item;
    TEST_ASSERT_TRUE(queue__push(&queue, item_pushed));
    queue__get_item_count(&queue, &current_count);
    TEST_ASSERT_EQUAL(item + 1, current_count);
  }

  // Should not be able to push anymore
  TEST_ASSERT_FALSE(queue__push(&queue, 123));
  queue__get_item_count(&queue, &current_count);
  TEST_ASSERT_EQUAL(max_queue_size, current_count);
  TEST_ASSERT_EQUAL(0, queue.head);
  TEST_ASSERT_EQUAL(0, queue.tail);

  // Pull and verify the FIFO order
  for (size_t item = 0; item < max_queue_size; item++) {
    uint8_t popped_value = 0;
    TEST_ASSERT_TRUE(queue__pop(&queue, &popped_value));
    TEST_ASSERT_EQUAL((uint8_t)item, popped_value);
  }

  // Test wrap-around case
  const uint8_t pushed_value = 123;
  TEST_ASSERT_TRUE(queue__push(&queue, pushed_value));
  uint8_t popped_value = 0;
  TEST_ASSERT_TRUE(queue__pop(&queue, &popped_value));
  TEST_ASSERT_EQUAL(pushed_value, popped_value);

  queue__get_item_count(&queue, &current_count);
  TEST_ASSERT_EQUAL(0, current_count);
  TEST_ASSERT_FALSE(queue__pop(&queue, &popped_value));
}
