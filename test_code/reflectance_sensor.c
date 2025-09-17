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
    SET_GPIO_OUT_REFLECTANCE_LED;
    SET_GPIO_IN_REFLECTANCE;
    REFLECTANCE_LED_ALL_OFF;

    printf("Step1: Test reflectance led\n");
    REFLECTANCE_LED_ALL_OFF; printf("Ready\n"); systick_wait_ms(1000);
    printf("Even LED\n");   systick_wait_ms(1000); REFLECTANCE_LED_0_ON; systick_wait_ms(1000); REFLECTANCE_LED_ALL_OFF;
    printf("Odd LED\n");    systick_wait_ms(1000); REFLECTANCE_LED_1_ON; systick_wait_ms(1000); REFLECTANCE_LED_ALL_OFF;
    printf("Even LED\n");   systick_wait_ms(1000); REFLECTANCE_LED_0_ON; systick_wait_ms(1000); REFLECTANCE_LED_ALL_OFF;
    printf("Odd LED\n");    systick_wait_ms(1000); REFLECTANCE_LED_1_ON; systick_wait_ms(1000); REFLECTANCE_LED_ALL_OFF;
    printf("All LED\n");    systick_wait_ms(1000); REFLECTANCE_LED_ALL_ON; systick_wait_ms(1000); REFLECTANCE_LED_ALL_OFF;
    BOARD_LED_ALL_OFF; printf("Step1: Test reflectance led - Done.\n"); systick_wait_ms(1000);

    printf("Step2: Test reflectance sensor\n");
    systick_wait_ms(1000); printf("Ready\n"); BOARD_LED_ALL_ON;
    int t = 0;
    while (1) {
        t++;
        printf("t:%d - ", t);
        printf("%s ", REFLECTANCE_1?"R1":"  ");
        printf("%s ", REFLECTANCE_2?"R2":"  ");
        printf("%s ", REFLECTANCE_3?"R3":"  ");
        printf("%s ", REFLECTANCE_4?"R4":"  ");
        printf("%s ", REFLECTANCE_5?"R5":"  ");
        printf("%s ", REFLECTANCE_6?"R6":"  ");
        printf("%s ", REFLECTANCE_7?"R7":"  ");
        printf("%s ", REFLECTANCE_8?"R8":"  ");
        printf("\n");
        systick_wait_ms(200);
    }

    return 0;
}
