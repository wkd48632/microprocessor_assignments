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
    SET_GPIO_OUT_BOARD_LED;

    BOARD_LED_ALL_OFF; printf("Ready\n"); systick_wait_ms(1000);
    printf("Red\n");        systick_wait_ms(1000); BOARD_LED_SET_R; systick_wait_ms(1000); BOARD_LED_ALL_OFF;
    printf("Green\n");      systick_wait_ms(1000); BOARD_LED_SET_G; systick_wait_ms(1000); BOARD_LED_ALL_OFF;
    printf("Blue\n");       systick_wait_ms(1000); BOARD_LED_SET_B; systick_wait_ms(1000); BOARD_LED_ALL_OFF;
    printf("Cyan\n");       systick_wait_ms(1000); BOARD_LED_SET_C; systick_wait_ms(1000); BOARD_LED_ALL_OFF;
    printf("Magenta\n");    systick_wait_ms(1000); BOARD_LED_SET_M; systick_wait_ms(1000); BOARD_LED_ALL_OFF;
    printf("Yellow\n");     systick_wait_ms(1000); BOARD_LED_SET_Y; systick_wait_ms(1000); BOARD_LED_ALL_OFF;
    printf("White\n");      systick_wait_ms(1000); BOARD_LED_SET_W; systick_wait_ms(1000); BOARD_LED_ALL_OFF;
    BOARD_LED_ALL_OFF; printf("Done.\n"); systick_wait_ms(1000);

    return 0;
}
