// Set GPIO mode
// SET_GPIO_IN/OUT ( PORT, USING_BIT )
#define SET_GPIO_OUT(X,Y)               X->SEL0&=~Y,X->SEL1&=~Y,X->DIR|=Y,X->OUT|=~Y
#define SET_GPIO_IN(X,Y)                X->SEL0&=~Y,X->SEL1&=~Y,X->DIR&=~Y,X->REN&=~Y,X->OUT&=~Y
#define SET_GPIO_IN_PULLDOWN(X,Y)       X->SEL0&=~Y,X->SEL1&=~Y,X->DIR&=~Y,X->REN|=Y,X->OUT&=~Y
#define SET_GPIO_IN_PULLUP(X,Y)         X->SEL0&=~Y,X->SEL1&=~Y,X->DIR&=~Y,X->REN|=Y,X->OUT|=Y

// Set component
#define SET_GPIO_OUT_REFLECTANCE_LED    SET_GPIO_OUT(P5,0b00001000),SET_GPIO_OUT(P5,0b00000100)
#define SET_GPIO_OUT_BOARD_LED          SET_GPIO_OUT(P2,0b00000111)
#define SET_GPIO_OUT_MOTOR_ENABLE       SET_GPIO_OUT(P3,0b11000000)
#define SET_GPIO_OUT_MOTOR_DIRECTION    SET_GPIO_OUT(P5,0b00110000)
#define SET_GPIO_OUT_MOTOR_SIGNAL       SET_GPIO_OUT(P2,0b11000000)
#define SET_GPIO_IN_BOARD_BTN           SET_GPIO_IN_PULLUP(P1,0b00010010)
#define SET_GPIO_IN_BUMP                SET_GPIO_IN_PULLUP(P4,0b11101101)
#define SET_GPIO_IN_REFLECTANCE         SET_GPIO_IN(P7,0b11111111)
#define SET_GPIO_IN_TACHOMETER_A        SET_GPIO_IN(P10,0b00110000)
#define SET_GPIO_IN_TACHOMETER_B        SET_GPIO_IN(P5,0b00000101)

// Set module
#define SET_GPIO_OUT_MOTOR              SET_GPIO_OUT_MOTOR_ENABLE,SET_GPIO_OUT_MOTOR_DIRECTION,SET_GPIO_OUT_MOTOR_SIGNAL
#define SET_GPIO_IN_TACHOMETER          SET_GPIO_IN_TACHOMETER_A,SET_GPIO_IN_TACHOMETER_B



// SysTick Init for 1ms
#define SET_SYSTICK_FOR_1MS            Systick->CTRL=0,SysTick->LOAD=48000-1,SysTick->VAL=0,SysTick->CTRL=0x00000005;



// I/Os

// BOARD_BTN
// BOARD_BTN 0 1
#define BOARD_BTN_S1    ((P1->IN&0b00000010)>>1)
#define BOARD_BTN_S2    ((P1->IN&0b00010000)>>4)

// BOARD_BTN true false;
#define BOARD_BTN_S1_TF  (P1->IN&0b00000010)
#define BOARD_BTN_S2_TF  (P1->IN&0b00010000)



// BOARD_LED
// BOARD_LED_ON/OFF
#define BOARD_LED_R_ON      P2->OUT|=0b00000001
#define BOARD_LED_G_ON      P2->OUT|=0b00000010
#define BOARD_LED_B_ON      P2->OUT|=0b00000100
#define BOARD_LED_R_OFF     P2->OUT&=0b11111110
#define BOARD_LED_G_OFF     P2->OUT&=0b11111101
#define BOARD_LED_B_OFF     P2->OUT&=0b11111011

// BOARD_LED_ALL_ON/OFF
#define BOARD_LED_ALL_ON    P2->OUT|=0b00000111
#define BOARD_LED_ALL_OFF   P2->OUT&=0b11111000

// BOARD_LED_SET by specific color
#define BOARD_LED_SET_K     P2->OUT=(P2->OUT&0b11111000)|0b00000000
#define BOARD_LED_SET_R     P2->OUT=(P2->OUT&0b11111000)|0b00000001
#define BOARD_LED_SET_G     P2->OUT=(P2->OUT&0b11111000)|0b00000010
#define BOARD_LED_SET_B     P2->OUT=(P2->OUT&0b11111000)|0b00000100
#define BOARD_LED_SET_C     P2->OUT=(P2->OUT&0b11111000)|0b00000110
#define BOARD_LED_SET_M     P2->OUT=(P2->OUT&0b11111000)|0b00000101
#define BOARD_LED_SET_Y     P2->OUT=(P2->OUT&0b11111000)|0b00000011
#define BOARD_LED_SET_W     P2->OUT=(P2->OUT&0b11111000)|0b00000111



// Bump sensor
// BUMP_0: rightmost, BUMP_5: leftmost

// Bump 0 1
#define BUMP_0      ((P4->IN&0b00000001)>>0)
#define BUMP_1      ((P4->IN&0b00000100)>>2)
#define BUMP_2      ((P4->IN&0b00001000)>>3)
#define BUMP_3      ((P4->IN&0b00100000)>>5)
#define BUMP_4      ((P4->IN&0b01000000)>>6)
#define BUMP_5      ((P4->IN&0b10000000)>>7)

// Bump true false: 
#define BUMP_0_TF   (P4->IN&0b00000001)
#define BUMP_1_TF   (P4->IN&0b00000100)
#define BUMP_2_TF   (P4->IN&0b00001000)
#define BUMP_3_TF   (P4->IN&0b00100000)
#define BUMP_4_TF   (P4->IN&0b01000000)
#define BUMP_5_TF   (P4->IN&0b10000000)

// Bump all/condensed
#define BUMP_ALL        (P4->IN&0b11101101)
#define BUMP_CONDENSED  (((P4->IN&0b00000001)>>0)|((P4->IN&0b00001100)>>1)|((P4->IN&0b11100000)>>2))



// Reflectance sensor
// REFLECTANCE_LED_0: even, REFLECTANCE_LED_1: odd
// REFLECTANCE_1: rightmost(tilted left), REFLECTANCE_8: leftmost(tilted right)

// Reflectance LED on/off
#define REFLECTANCE_LED_0_ON        P5->OUT|=0b00001000
#define REFLECTANCE_LED_1_ON        P9->OUT|=0b00000100
#define REFLECTANCE_LED_0_OFF       P5->OUT&=0b11110111
#define REFLECTANCE_LED_1_OFF       P9->OUT&=0b11111011

// Reflectance 0 1
#define REFLECTANCE_1           ((P7->IN&0b00000001)>>0)
#define REFLECTANCE_2           ((P7->IN&0b00000010)>>1)
#define REFLECTANCE_3           ((P7->IN&0b00000100)>>2)
#define REFLECTANCE_4           ((P7->IN&0b00001000)>>3)
#define REFLECTANCE_5           ((P7->IN&0b00010000)>>4)
#define REFLECTANCE_6           ((P7->IN&0b00100000)>>5)
#define REFLECTANCE_7           ((P7->IN&0b01000000)>>6)
#define REFLECTANCE_8           ((P7->IN&0b10000000)>>7)

// Reflectance true false
#define REFLECTANCE_1_TF        (P7->IN&0b00000001)
#define REFLECTANCE_2_TF        (P7->IN&0b00000010)
#define REFLECTANCE_3_TF        (P7->IN&0b00000100)
#define REFLECTANCE_4_TF        (P7->IN&0b00001000)
#define REFLECTANCE_5_TF        (P7->IN&0b00010000)
#define REFLECTANCE_6_TF        (P7->IN&0b00100000)
#define REFLECTANCE_7_TF        (P7->IN&0b01000000)
#define REFLECTANCE_8_TF        (P7->IN&0b10000000)

// Reflectance all(=condensed)
#define REFLECTANCE_ALL         (P7->IN)
#define REFLECTANCE_CONDENSED   (P7->IN)



// Motor

// Motor R/L enable/disable
#define MOTOR_R_SET_ENABLE      P3->OUT|=0b10000000
#define MOTOR_L_SET_ENABLE      P3->OUT|=0b01000000
#define MOTOR_R_SET_DISABLE     P3->OUT&=0b01111111
#define MOTOR_L_SET_DISABLE     P3->OUT&=0b10111111

// Motor all enable/disable
#define MOTOR_ALL_SET_ENABLE    P3->OUT|=0b11000000
#define MOTOR_ALL_SET_DISABLE   P3->OUT&=0b00111111

// Motor R/L front/back
#define MOTOR_R_SET_FRONT       P5->OUT|=0b00010000
#define MOTOR_L_SET_FRONT       P5->OUT|=0b00100000
#define MOTOR_R_SET_BACK        P5->OUT&=0b11101111
#define MOTOR_L_SET_BACK        P5->OUT&=0b11011111

// Motor signal high/low
#define MOTOR_R_SIGNAL_HIGH     P2->OUT|=0b10000000
#define MOTOR_L_SIGNAL_HIGH     P2->OUT|=0b01000000
#define MOTOR_R_SIGNAL_LOW      P2->OUT&=0b01111111
#define MOTOR_L_SIGNAL_LOW      P2->OUT&=0b10111111



// Tachometer

// Tachometer 0 1
#define TACHOMETER_L_A         ((P10->IN&0b00100000)>>5)
#define TACHOMETER_R_A         ((P10->IN&0b00010000)>>4)
#define TACHOMETER_L_B         ((P5->IN&0b00000100)>>2)
#define TACHOMETER_R_B         ((P5->IN&0b00000001)>>0)

// Tachometer true false
#define TACHOMETER_L_A_TF      (P10->IN&0b00100000)
#define TACHOMETER_R_A_TF      (P10->IN&0b00010000)
#define TACHOMETER_L_B_TF      (P5->IN&0b00000100)
#define TACHOMETER_R_B_TF      (P5->IN&0b00000001)



