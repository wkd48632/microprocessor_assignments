#include "msp.h"
#include "Clock.h"
#include <stdio.h>

void systick_init(void){
    SysTick->CTRL = 0;
    SysTick->LOAD = 48000-1;
    SysTick->VAL = 0;
    SysTick->CTRL = 0x00000005;
}

void systick_wait1ms(){
    printf("###");
    SysTick->VAL = 0;
    while((SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk) == 0);
}


void systick_wait1s() {
    int i = 0;
    for ( i = 0; i != 1000; i++ ) {
        printf("##");
        systick_wait1ms();

    }
}

int main(void)
{
    Clock_Init48MHz();

    P2->SEL0 &= 0b11111000;
    P2->SEL1 &= 0b11111000;
    P2->DIR  |= 0b00000111;
    P2->OUT  &= 0b11111000;

    systick_init();
    char i = 1;
    while ( 1 ) {
        printf("P2->OUT: %2x\n", P2->OUT);
        systick_wait1s();
        //systick_wait1s();
        P2->OUT = P2->OUT & 0b11111000 | i;
        i <<= 1; i = i + i / 8 & 7;
        //^= i; i <<= 1; i = i + i / 8 & 7; // when apply i directly on out, trimming i is inevitable.
    }
    return 0;
}
