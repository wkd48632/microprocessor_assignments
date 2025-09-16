#include "msp.h"
#include "Clock.h"
#include <stdio.h>

int main(void)
{
    Clock_Init48MHz();

    P2->SEL0 &= 0b11111000;
    P2->SEL1 &= 0b11111000;
    P2->DIR  |= 0b00000111;
    P2->OUT  &= 0b11111000;
    
    unsigned char i = 8;
    while ( true ) {
        P2OUT=P2OUT&-8|(i|=i/8);
        printf("P2->OUT: %2x\n", P2->OUT);
        i <<= 1;
        Clock_Delay1ms(1000);
    }
    return 0;
}
