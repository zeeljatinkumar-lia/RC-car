#include "Mockcan_bus.h"
#include "unity.h"

#include "can_bus_module.h"

void test_can_bus_module__reset_when_bus_is_on() {
  can__is_bus_off_ExpectAndReturn(can1, false);
  can_bus_module__reset(can1);
}

void test_can_bus_module__reset_when_bus_is_off() {
  can__is_bus_off_ExpectAndReturn(can1, true);
  can__reset_bus_Expect(can1);
  can_bus_module__reset(can1);
}

void test_can_bus_module__init_failure() {
  can__init_ExpectAndReturn(can1, 100, 10, 10, NULL, NULL, false);
  can__bypass_filter_accept_all_msgs_Expect();
  can__is_bus_off_ExpectAndReturn(can1, false);
  TEST_ASSERT_FALSE(can_bus_module__init(can1))
};

void test_can_bus_module__init_success() {
  can__bypass_filter_accept_all_msgs_Expect();
  can__init_ExpectAndReturn(can1, 100, 10, 10, NULL, NULL, true);
  can__is_bus_off_ExpectAndReturn(can1, false);
  TEST_ASSERT_TRUE(can_bus_module__init(can1));
}