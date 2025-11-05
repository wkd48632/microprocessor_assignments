// 30cm forward
// 7pi = 1 round
// 30 / (7pi) round

// 30deg rotate
// W = 14, Rw = 14pi
// W:30deg = 12:1
// 14pi/12 cm to go
// 14pi/12 / (7pi) round
// 14pi/12/7pi * 360deg = 60deg@motor

#include "msp.h"
#include "Clock.h"
// Page 13

void turn_on_led(int color) {
    P2->OUT &= ~0x07;
    P2->OUT |= color;
}
void turn_off_led() {
    P2->OUT &= ~0x07;
}

// Page 14: DC Motor Using PWM

void pwm_init34(uint16_t period, uint16_t duty3, uint16_t duty4) {
  // CCR0 period
  TIMER_A0->CCR[0] = period;

  // divide by 1
  TIMER_A0->EX0 = 0x0000;

  // toggle/reset
  TIMER_A0->CCTL[3] = 0x0040;
  TIMER_A0->CCR[3]  = duty3;
  TIMER_A0->CCTL[4] = 0x0040;
  TIMER_A0->CCR[4]  = duty4;

  // 0x0200 -> SMCLK
  // 0b1100 0000 -> input divider /8
  // 0b0010 0000 -> up/down mode
  TIMER_A0->CTL = 0x02E0;

  // set alternative
  P2->DIR  |= 0xC0;
  P2->SEL0 |= 0xC0;
  P2->SEL1 &= ~0xC0;
}

void motor_init(void) {
  P3->SEL0 &= ~0xC0;   // 1) configure nSLPR & nSLPL as GPIO
  P3->SEL1 &= ~0xC0;   // 2) make nSLPR & nSLPL as output
  P3->DIR  |=  0xC0;
  P3->OUT  &= ~0xC0;   // 3) output LOW

  P5->SEL0 &= ~0x30;   // 1) configure DIRR & DIRL as GPIO
  P5->SEL1 &= ~0x30;   // 2) make DIRR & DIRL as output
  P5->DIR  |=  0x30;
  P5->OUT  &= ~0x30;   // 3) output LOW

  P2->SEL0 &= ~0xC0;   // 1) configure PWMR & PWML as GPIO
  P2->SEL1 &= ~0xC0;   // 2) make PWMR & PWML as output
  P2->DIR  |=  0xC0;
  P2->OUT  &= ~0xC0;   // 3) output LOW

  pwm_init34(7500, 0, 0);
}

void move(uint16_t leftDuty, uint16_t rightDuty) {
  P3->OUT |= 0xC0;
  TIMER_A0->CCR[3] = leftDuty;
  TIMER_A0->CCR[4] = rightDuty;
}

void left_forward() {
  P5->OUT &= ~0x10;
}

void left_backward() {
  P5->OUT |= 0x10;
}

void right_forward() {
  P5->OUT &= ~0x20;
}

void right_backward() {
  P5->OUT |= 0x20;
}


void systick_init(void){
    SysTick->CTRL = 0;
    SysTick->LOAD = 48000-1;
    SysTick->VAL = 0;
    SysTick->CTRL = 0x00000005;
}

void systick_wait1ms(){
    SysTick->VAL = 0;
    while((SysTick->CTRL & 0x00010000) == 0);
}


void systick_wait1s() {
    int i; for ( i = 0; i != 1000; i++ ) systick_wait1ms();
}


// Page 22: Example
void main(void)
{
    // Initialization
    Clock_Init48MHz();
    systick_init();
    motor_init();

    while(1) {
        left_forward();
        right_backward();
        move(2000, 2000);
        systick_wait1s();

        left_backward();
        right_forward();
        move(1000, 3000);
        systick_wait1s();
    }
}
