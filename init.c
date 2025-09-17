#include "define.h"

void init() {
  SET_SYSTICK_FOR_1MS;
  SET_GPIO_OUT_REFLECTANCE_LED;
  SET_GPIO_OUT_BOARD_LED;
  SET_GPIO_OUT_MOTOR;
  SET_GPIO_IN_BOARD_BTN;
  SET_GPIO_IN_BUMP;
  SET_GPIO_IN_REFLECTANCE;
  SET_GPIO_IN_TACHOMETER;
}

void systick_wait1ms() {
    SysTick->VAL = 0;
    while((SysTick->CTRL & 0x00010000) == 0);
}
