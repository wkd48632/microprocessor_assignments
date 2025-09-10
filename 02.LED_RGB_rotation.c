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
    P2 -> SEL0 &= ~0x07;
    P2 -> SEL1 &= ~0x07;
    P2 -> DIR |= 0x07;
    P2 -> OUT &= ~0x07;
    
    
    for ( int status = 0; ; status = status + 1 % 3) {
        // LED_ON
        switch(status){
        case 0:
            LED_R_ON;
            break;
        case 1:
            LED_G_ON;
            break;
        case 2:
            LED_B_ON;
            break;
        }
        Clock_Delay1ms(1000);
        LED_ALL_OFF;
    }
    return 0;
}
