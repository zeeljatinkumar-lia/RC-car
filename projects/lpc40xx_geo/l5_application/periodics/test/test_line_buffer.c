#include "unity.h"
#include <string.h>

// Include the source we wish to test
#include "app_queue.h"
#include "line_buffer.h"

// Most unit-tests focus on nominal cases, but you should also have
// tests that use larger line buffers etc.
static line_buffer_s line_buffer;
static char memory[8];

// This method re-initializes the line_buffer for the rest of the tests
void setUp(void) { line_buffer__init(&line_buffer, memory, sizeof(memory)); }

void tearDown(void) {}

static void add_to_line_buffer(const char *string) {
  for (size_t index = 0; index < strlen(string); index++) {
    TEST_ASSERT_TRUE(line_buffer__add_byte(&line_buffer, string[index]));
  }
}

void test_line_buffer__nominal_case(void) {
  add_to_line_buffer("abc\n");

  char line[8];
  TEST_ASSERT_TRUE(line_buffer__remove_line(&line_buffer, line, sizeof(line)));
  TEST_ASSERT_EQUAL_STRING("abc", line);
}

void test_incomplete_line(void) {
  add_to_line_buffer("xy");

  char line[8];
  // Line buffer doesn't contain entire line yet (defined by \n)
  TEST_ASSERT_FALSE(line_buffer__remove_line(&line_buffer, line, sizeof(line)));

  // Line buffer receives \n
  line_buffer__add_byte(&line_buffer, '\n');
  TEST_ASSERT_TRUE(line_buffer__remove_line(&line_buffer, line, sizeof(line)));
  TEST_ASSERT_EQUAL_STRING("xy", line);
}

void test_line_buffer__slash_r_slash_n_case(void) {
  add_to_line_buffer("ab\r\n");

  char line[8];
  TEST_ASSERT_TRUE(line_buffer__remove_line(&line_buffer, line, sizeof(line)));
  TEST_ASSERT_EQUAL_STRING("ab\r", line);
}

// Line buffer should be able to add multiple lines and we should be able to remove them one at a time
void test_line_buffer__multiple_lines(void) {
  add_to_line_buffer("ab\ncd\n");

  char line[8];
  TEST_ASSERT_TRUE(line_buffer__remove_line(&line_buffer, line, sizeof(line)));
  TEST_ASSERT_EQUAL_STRING("ab", line);

  TEST_ASSERT_TRUE(line_buffer__remove_line(&line_buffer, line, sizeof(line)));
  TEST_ASSERT_EQUAL_STRING("cd", line);
}

void test_line_buffer__overflow_case(void) {
  // Add chars until full capacity
  for (size_t i = 0; i < sizeof(memory); i++) {
    TEST_ASSERT_TRUE(line_buffer__add_byte(&line_buffer, 'a' + i));
  }

  // Buffer should be full now
  TEST_ASSERT_FALSE(line_buffer__add_byte(&line_buffer, 'b'));

  // Retreive truncated output (without the newline char)
  // Do not modify this test; instead, change your API to make this test pass
  // Note that line buffer was full with "abcdefgh" but we should only
  // retreive "abcdefg" because we need to write NULL char to line[8]
  char line[8] = {0};
  TEST_ASSERT_TRUE(line_buffer__remove_line(&line_buffer, line, sizeof(line)));
  TEST_ASSERT_EQUAL_STRING("abcdefg", line);
}