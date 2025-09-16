#include "msp.h"
#include "Clock.h"
#include <stdio.h>

int main(void)
{
    Clock_Init48MHz();

    P2->SEL0 &= 0b11111000;
    P2->SEL1 &= 0b11111000;
    P2->DIR  |= 0b00000111;
    P2->OUT  &= 0b11111001;
    
    int i = 3;
    while ( true ) {
        printf("P2->OUT: %2x\n", P2->OUT);
        Clock_Delay1ms(1000);
        P2->OUT ^= i; i <<= 1; i = i + i / 8 & 7; // when apply i directly on out, trimming i is inevitable.
    }
    return 0;
}
