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
    
    unsigned char i = 0;
    while ( true ) {
        if ( !(i&0b00000111) ) i++;
        P2->OUT &= (0b11111000 | i);
        i <<= 1;
        Clock_Delay1ms(1000);
    }
    return 0;
}
