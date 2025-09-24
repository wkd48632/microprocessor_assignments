#include "msp.h"
#include "Clock.h"

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

    // Built-in LED RED
    P2->SEL0 &= ~0x01;
    P2->SEL1 &= ~0x01;
    P2->DIR  |=  0x01;
    P2->OUT  &= ~0x01;

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

    P3->SEL0 &= ~0xC0;
    P3->SEL1 &= ~0xC0;
    P3->DIR  |=  0xC0;
    P3->OUT  &= ~0xC0;


    P2->SEL0 &= ~0b11000000;
    P2->SEL1 &= ~0b11000000;
    P2->DIR  |=  0b11000000;
    P2->OUT  &= ~0b00111111;

    printf("Done.\n");
    while (1) {
        P5->OUT |= 0x08;
        P9->OUT |= 0x04;

        P7->DIR = 0xFF;
        P7->OUT = 0xFF;

        Clock_Delay1us(10);

        P7->DIR = 0x00;

        Clock_Delay1us(500);

        unsigned char status = ~P7->IN; // detected = black = 1

        printf("Reflectance sensor status: %x\n", status);

        unsigned char side_detected = status & 0b11100111;
        unsigned char center_detected = status & 0b00011000;

        if ( !side_detected && center_detected) {
            printf("on\n");
            P2->OUT |=  0x01;
        }
        else {
            printf("off\n");
            P2->OUT &= ~0x01;
        }
        P5->OUT &= ~0x08;
        P9->OUT &= ~0x04;
        Clock_Delay1ms(10);
    }

    return 0;
}
