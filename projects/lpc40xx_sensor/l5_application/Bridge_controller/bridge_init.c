#include "bridge_init.h"
#include "board_io.h"
#include <stdbool.h>
#include <stdio.h>

bool dest_is_towards_north;
bool dest_is_towards_south;
bool dest_is_towards_east;
bool dest_is_towards_west;
bool switch0_is_pressed;
bool switch1_is_pressed;
bool switch2_is_pressed;
bool switch3_is_pressed;
void bridge_init_led(void) {
  gpio__set(board_io__get_led0());
  gpio__set(board_io__get_led1());
  gpio__set(board_io__get_led2());
  gpio__set(board_io__get_led3());
}

void dbg_set_fake_dest_coords() {
  if (switch0_is_pressed == false && gpio__get(board_io__get_sw0())) {
    switch0_is_pressed = true;
    gpio__toggle(board_io__get_led0());
  } else if (switch0_is_pressed && !gpio__get(board_io__get_sw0())) {
    switch0_is_pressed = false;
  }
  if (switch1_is_pressed == false && gpio__get(board_io__get_sw1())) {
    switch1_is_pressed = true;
    gpio__toggle(board_io__get_led1());
  } else if (switch1_is_pressed && !gpio__get(board_io__get_sw1())) {
    switch1_is_pressed = false;
  }
  if (switch2_is_pressed == false && gpio__get(board_io__get_sw2())) {
    switch2_is_pressed = true;
    gpio__toggle(board_io__get_led2());
  } else if (switch2_is_pressed && !gpio__get(board_io__get_sw2())) {
    switch2_is_pressed = false;
  }
  if (switch3_is_pressed == false && gpio__get(board_io__get_sw3())) {
    switch3_is_pressed = true;
    gpio__toggle(board_io__get_led3());
  } else if (switch3_is_pressed && !gpio__get(board_io__get_sw3())) {
    switch3_is_pressed = false;
  }

  if (!gpio__get(board_io__get_led0())) {
    dest_is_towards_north = true;
  } else {
    dest_is_towards_north = false;
  }
  if (!gpio__get(board_io__get_led1())) {
    dest_is_towards_south = true;
  } else {
    dest_is_towards_south = false;
  }
  if (!gpio__get(board_io__get_led2())) {
    dest_is_towards_east = true;
  } else {
    dest_is_towards_east = false;
  }
  if (!gpio__get(board_io__get_led3())) {
    dest_is_towards_west = true;
  } else {
    dest_is_towards_west = false;
  }
}
