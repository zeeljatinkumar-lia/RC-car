#include "Mockcan_bus.h"
#include "unity.h"

#include "can_bus_module.h"

void test_can_bus_module__init_failure() {
  can__init_ExpectAndReturn(can1, 100, 10, 10, NULL, NULL, false);
  can__bypass_filter_accept_all_msgs_Expect();
  can__reset_bus_Expect(can1);
  TEST_ASSERT_FALSE(can_bus_module__init(can1))
};

void test_can_bus_module__init_success() {
  can__bypass_filter_accept_all_msgs_Expect();
  can__init_ExpectAndReturn(can1, 100, 10, 10, NULL, NULL, true);
  can__reset_bus_Expect(can1);
  TEST_ASSERT_TRUE(can_bus_module__init(can1));
}