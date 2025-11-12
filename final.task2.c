#include "msp.h"
#include "Clock.h"
#include <stdio.h>
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

void stop() {
    P3->OUT &= ~0xC0;
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

void wait_little(){
    int i; for ( i = 0; i != 50; i++ ) systick_wait1ms();
}

void systick_wait1s() {
    int i; for ( i = 0; i != 1000; i++ ) systick_wait1ms();
}


void timer_A3_capture_init() {
    P10->SEL0 |= 0x30;
    P10->SEL1 &= ~0x30;
    P10->DIR &= ~0x30;

    TIMER_A3->CTL &= ~0x0030;
    TIMER_A3->CTL = 0x0200;

    TIMER_A3->CCTL[0] = 0x4910;
    TIMER_A3->CCTL[1] = 0x4910;
    TIMER_A3->EX0 & ~0x0007;

    NVIC->IP[3] = (NVIC->IP[3]&0x00000FFFF) | 0x40400000;
    NVIC->ISER[0] = 0x0000C000;
    TIMER_A3->CTL |= 0x0024;
}

uint32_t left_count;
uint32_t right_count;

void TA3_N_IRQHandler(void) {
    TIMER_A3->CCTL[1] &= ~0x0001;
    left_count++;
    printf("l++\n");
}

void TA3_0_IRQHandler(void) {
    TIMER_A3->CCTL[0] &= ~0x0001;
    right_count++;
    //printf("r++\n");
}

uint32_t THRE = 600; //ms

unsigned char detect(){
    P5->OUT |= 0x08;
    P9->OUT |= 0x04;

    P7->DIR = 0xFF;
    P7->OUT = 0xFF;

    Clock_Delay1us(10);

    P7->DIR = 0x00;

    Clock_Delay1us(THRE);

    // unsigned char status = ~P7->IN; // detected = black = 1
    return ~P7->IN;
}

void main(void)
{
    // Initialization
    Clock_Init48MHz();
    systick_init();
    motor_init();
    timer_A3_capture_init();

    left_count = 0;
    right_count = 0;
    left_forward();
    right_forward();
    while(1) {
        unsigned char status = detect();
        int right = status & 0b00000111;
        int left = status & 0b11100000;
        printf("%x l%d r%d\n", status, !!left, !!right);
        if (left && right) {
            break;
        }
        else if (left) {
            stop(), wait_little();
            left_count = 0;
            left_backward(), right_forward(), move(10,10);
            systick_wait1s();
            //while(left_count < 60) { systick_wait1ms(); printf("%d\n", left_count);}
            stop(), wait_little();
            left_forward(), right_forward(), move(10,10);
        }
        else if (right) {
            stop(), wait_little();
           left_count = 0;
           left_forward(), right_backward(), move(10,10);
           systick_wait1s();
           //while(left_count < 60) { systick_wait1ms(); printf("%d\n", left_count);}
           stop(), wait_little();
           left_forward(), right_forward(), move(10,10);
        }
        else move(10,10);
        /*
        if ( left_count < 60 && right_count < 60 ) {
            move(10, 10);
            systick_wait1ms();
        }
        else if ( left_count < 60 ) {
            move(10, 0);
            systick_wait1ms();
        }
        else if ( right_count < 60 ) {
            move(0, 10);
            systick_wait1ms();
        }
        else {
            move(0, 0);
        }
        */
    }
    stop();
}
