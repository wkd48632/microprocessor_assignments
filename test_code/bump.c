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
    SET_GPIO_IN_BUMP;

    int t = 0;
    while (1) {
        t++;
        printf("t:%d - ", t);
        printf("%s ", BUMP_0?"B0":"  ");
        printf("%s ", BUMP_1?"B1":"  ");
        printf("%s ", BUMP_2?"B2":"  ");
        printf("%s ", BUMP_3?"B3":"  ");
        printf("%s ", BUMP_4?"B4":"  ");
        printf("%s ", BUMP_5?"B5":"  ");
        printf("\n");
        systick_wait_ms(200);
    }

    return 0;
}
