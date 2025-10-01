#include "msp.h"
#include "Clock.h"


void systick_wait_ms(int n) {
    int i = 0;
    for ( i = 0; i != n; i++ ) {
        SysTick->VAL = 0;
        while((SysTick->CTRL & 0x00010000) == 0);
    }
}

void pwm_init34(uint16_t period, uint16_t duty3, uint16_t duty4) {
    // CCR0 period
    TIMER_A0->CCR[0] = period;

    // divided by 1
    TIMER_A0->EX0 = 0x0000;

    //toggle/reset
    TIMER_A0->CCTL[3] = 0x0040;
    TIMER_A0->CCR[3] = duty3;
    TIMER_A0->CCTL[4] = 0x0040;
    TIMER_A0->CCR[4] = duty4;

    TIMER_A0->CTL = 0x02F0;

    // set alternative
    P2->SEL0 |=  0xC0;
    P2->SEL1 &= ~0xC0;
    P2->DIR  |=  0xC0;
}

int main()
{

}#include "msp.h"
#include "Clock.h"
#define SET_GPIO_OUT(X,Y)   X->SEL0&=~Y,X->SEL1&=~Y,X->DIR|=Y,X->OUT|=~Y

int main()
{
    // Initialization
    Clock_Init48MHz();
    SET_GPIO_OUT(P2, 0x01); // Built-in LED red
    SET_GPIO_OUT(P5, 0x08); // IR LED even
    SET_GPIO_OUT(P9, 0x04); // IR LED odd
    SET_GPIO_OUT(P7, 0xFF); // IR sensor
    SET_GPIO_OUT(P2, 0xC0); // Motor nSLP
    SET_GPIO_OUT(P5, 0x30); // Motor DIR
    SET_GPIO_OUT(P3, 0xC0); // Motor PWM

    P2->OUT |=  0xC0; // Set motor move
    P5->OUT &= ~0x30; // Set motor direction forward

    while (1) {
        // Turn on IR LED
        P5->OUT |= 0x08;
        P9->OUT |= 0x04;

        // Charge IR sensor
        P7->DIR = 0xFF;
        P7->OUT = 0xFF;
        Clock_Delay1us(10);
        P7->DIR = 0x00;

        // Get sensor status
        Clock_Delay1us(700);
        unsigned char status = ~P7->IN; // white = 0, black = 1
        printf("Reflectance sensor status: %x\n", status);

        unsigned char full_detected   = status & 0b01111110;
        unsigned char center_detected = status & 0b00011000;

        // Control LED
        if ( center_detected ) {
            printf("Turn on LED!\n");
            P2->OUT |=  0x01;
        }
        else {
            printf("Turn off LED!\n");
            P2->OUT &= ~0x01;
        }


        // Control motor
        if ( full_detected == 0b01111110 ) {
            printf("Don't move!\n");
            P3->OUT &= ~0xC0;
        }
        else {
            printf("Move!\n");
            P3->OUT |=  0xC0;
        }

        // Turn off IR LED
        P5->OUT &= ~0x08;
        P9->OUT &= ~0x04;
        
        Clock_Delay1ms(10);
        P3->OUT &= ~0xC0;
        Clock_Delay1ms(20);
        
    }

    return 0;
}

