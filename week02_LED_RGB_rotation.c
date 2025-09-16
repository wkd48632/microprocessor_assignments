#include "msp.h"
#include "Clock.h"
#include <stdio.h>
#define LED_R_ON    P2->OUT|=0b00000001
#define LED_G_ON    P2->OUT|=0b00000010
#define LED_B_ON    P2->OUT|=0b00000100
#define LED_R_OFF   P2->OUT&=0b11111110
#define LED_G_OFF   P2->OUT&=0b11111101
#define LED_B_OFF   P2->OUT&=0b11111011
#define LED_ALL_OFF P2->OUT&=0b11111000

int main(void)
{
    Clock_Init48MHz();
    
    // Init
    P2->SEL0 &= 0b11111000;
    P2->SEL1 &= 0b11111000;
    P2->DIR  |= 0b00000111;
    P2->OUT  &= 0b11111000;
    
    int i;
    while ( true ) {
        if ( i & 0b00001000 ) i++;
        P2->OUT &= (0b11111000 | i);
        i <<= 1;
        Clock_Delay1ms(1000);
    }
    return 0;
}
