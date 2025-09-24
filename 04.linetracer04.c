#include "msp.h"

void systick_wait_ms(int n) {
    int i = 0;
    for ( i = 0; i != n; i++ ) {
        SysTick->VAL = 0;
        while((SysTick->CTRL & 0x00010000) == 0);
    }
}

int main()
{
    Clock_Init48MHz();

    // IR LED 0, 2, 4, 6
    P5->SEL0 &= ~0x08;
    P5->SEL1 &= ~0x08;
    P5->DIR  |=  0x08;
    P5->OUT  &= ~0x08;

    // IR LED 1, 3, 5, 7
    P9->SEL0 &= ~0x04;
    P9->SEL1 &= ~0x04;
    P9->DIR  |=  0x04;
    P9->OUT  &= ~0x04;

    // IR Sensor 0 ~ 7
    P7->SEL0 &= ~0xFF;
    P7->SEL1 &= ~0xFF;
    P7->DIR  &= ~0xFF;

    P2->SEL0 &= ~0b11000000;
    P2->SEL1 &= ~0b11000000;
    P2->DIR  |=  0b11000000;
    P2->OUT  &= ~0b00111111;

    while (1) {
        // Turn on IR LEDs
        P5->OUT  |=  0x08;
        P9->OUT  |=  0x04;
    }
    return 0;
}
