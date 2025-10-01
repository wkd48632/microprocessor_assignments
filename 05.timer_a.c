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

}
