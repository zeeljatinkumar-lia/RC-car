#include "Mockcan_bus.h"
#include "can_dbc_initializer.h"
#include "unity.h"

#include <stddef.h>
#include <stdint.h>

void test_can_dbc_init(void) {
  can__init_ExpectAndReturn(can1, 100, 5, 5, NULL, NULL, true);
  can__bypass_filter_accept_all_msgs_Expect();
  can__reset_bus_Expect(can1);
  TEST_ASSERT_TRUE(can_dbc_init());
}
