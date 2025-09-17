#include "msp.h"
#include "define.h"

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
    SET_SYSTICK_FOR_1MS;
    SET_GPIO_IN_BOARD_BTN;

    int t = 0;
    while (1) {
        t++
        printf("t:%d - ", t);
        if(BOARD_BTN_S1) printf("S1 pressed. ");
        if(BOARD_BTN_S2) printf("S2 pressed. ");
        printf('\n')
        systick_wait_ms(200);
    }

    return 0;
}
