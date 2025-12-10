#include "msp.h"
#include "Clock.h"
#include <stdio.h>
#include <stdint.h>

/* =========================================================================
 * 하드웨어 핀 및 상수 정의 (Hardware & Constant Definitions)
 * ========================================================================= */

// 모터 PWM 설정 (Timer A0)
#define PWM_PERIOD      7500
#define MOTOR_R_PWM_REG TIMER_A0->CCR[3]
#define MOTOR_L_PWM_REG TIMER_A0->CCR[4]

// 모터 방향 제어 핀 (P5.4=Left Dir, P5.5=Right Dir)
#define DIR_PORT        P5
#define DIR_L_BIT       0x10 // P5.4
#define DIR_R_BIT       0x20 // P5.5

// 모터 활성화 핀 (P3.6=SLPL, P3.7=SLPR)
#define SLP_PORT        P3
#define SLP_MASK        0xC0

// IR 센서 및 LED 핀
// IR Emitters: Odd(P5.3), Even(P9.2)
#define IR_LED_ODD_BIT  0x08 // P5.3
#define IR_LED_EVEN_BIT 0x04 // P9.2
// IR Sensors: P7.0 ~ P7.7
#define SENSOR_PORT     P7

// 라인 트레이싱 파라미터
#define SENSOR_TRIAL    10   // 센서 읽기 반복 횟수 (평균화)
#define THRESHOLD       0.7f // 1로 인식할 임계값 비율
#define CHARGE_TIME_US  10   // 커패시터 충전 시간
#define DECAY_TIME_US   900  // 방전 대기 시간

// 전역 변수
volatile uint32_t left_enc_count = 0;
volatile uint32_t right_enc_count = 0;
float left_feel = 0;
float right_feel = 0;
const float decay_rate = 0.5f;

/* =========================================================================
 * 초기화 함수 (Initialization Functions)
 * ========================================================================= */

void SysTick_Init(void) {
    SysTick->CTRL = 0;
    SysTick->LOAD = 48000 - 1; // 1ms 단위 (48MHz 기준)
    SysTick->VAL = 0;
    SysTick->CTRL = 0x00000005; // Clock Source: Core, Int: Dis, Enable: Yes
}

void SysTick_Wait1ms(void) {
    SysTick->VAL = 0;
    while ((SysTick->CTRL & 0x00010000) == 0);
}

void Delay_ms(int ms) {
    int i; for (i = 0; i < ms; i++) SysTick_Wait1ms();
}

void PWM_Init(uint16_t period, uint16_t duty3, uint16_t duty4) {
    TIMER_A0->CCR[0] = period;
    TIMER_A0->EX0 = 0x0000;
    TIMER_A0->CCTL[3] = 0x0040;
    TIMER_A0->CCR[3] = duty3;
    TIMER_A0->CCTL[4] = 0x0040;
    TIMER_A0->CCR[4] = duty4;
    TIMER_A0->CTL = 0x02E0; // SMCLK, ID/8, Up/Down mode

    P2->DIR |= 0xC0;  // P2.6, P2.7 Output
    P2->SEL0 |= 0xC0; // Timer Function
    P2->SEL1 &= ~0xC0;
}

void Motor_Init(void) {
    // Sleep Pins (P3.6, P3.7)
    P3->SEL0 &= ~SLP_MASK;
    P3->SEL1 &= ~SLP_MASK;
    P3->DIR |= SLP_MASK;
    P3->OUT &= ~SLP_MASK; // 초기 상태: Sleep

    // Direction Pins (P5.4, P5.5)
    P5->SEL0 &= ~(DIR_L_BIT | DIR_R_BIT);
    P5->SEL1 &= ~(DIR_L_BIT | DIR_R_BIT);
    P5->DIR |= (DIR_L_BIT | DIR_R_BIT);
    P5->OUT &= ~(DIR_L_BIT | DIR_R_BIT);

    PWM_Init(PWM_PERIOD, 0, 0);
}

void IR_Sensor_Init(void) {
    // IR LED Odd (P5.3)
    P5->SEL0 &= ~IR_LED_ODD_BIT;
    P5->SEL1 &= ~IR_LED_ODD_BIT;
    P5->DIR |= IR_LED_ODD_BIT;
    P5->OUT &= ~IR_LED_ODD_BIT;

    // IR LED Even (P9.2)
    P9->SEL0 &= ~IR_LED_EVEN_BIT;
    P9->SEL1 &= ~IR_LED_EVEN_BIT;
    P9->DIR |= IR_LED_EVEN_BIT;
    P9->OUT &= ~IR_LED_EVEN_BIT;

    // Sensors (P7.0 - P7.7)
    P7->SEL0 &= ~0xFF;
    P7->SEL1 &= ~0xFF;
    P7->DIR &= ~0xFF; // Input mode default
}

void Encoder_Timer_Init(void) {
    P10->SEL0 |= 0x30;
    P10->SEL1 &= ~0x30;
    P10->DIR &= ~0x30;

    TIMER_A3->CTL &= ~0x0030;
    TIMER_A3->CTL = 0x0200;

    TIMER_A3->CCTL[0] = 0x4910;
    TIMER_A3->CCTL[1] = 0x4910;
    TIMER_A3->EX0 &= ~0x0007;

    NVIC->IP[3] = (NVIC->IP[3] & 0x00000FFFF) | 0x40400000;
    NVIC->ISER[0] = 0x0000C000;
    TIMER_A3->CTL |= 0x0024;
}

void System_Init(void) {
    Clock_Init48MHz();
    SysTick_Init();
    Motor_Init();
    IR_Sensor_Init();
    Encoder_Timer_Init();

    // Global Vars Init
    left_enc_count = 0;
    right_enc_count = 0;
}

/* =========================================================================
 * 인터럽트 핸들러 (Interrupt Handlers)
 * ========================================================================= */

void TA3_N_IRQHandler(void) {
    TIMER_A3->CCTL[1] &= ~0x0001;
    left_enc_count++;
}

void TA3_0_IRQHandler(void) {
    TIMER_A3->CCTL[0] &= ~0x0001;
    right_enc_count++;
}

/* =========================================================================
 * 모터 제어 함수 (Motor Control Functions)
 * ========================================================================= */

void Motor_SetDir(int left_fwd, int right_fwd) {
    // Left Motor
    if (left_fwd) DIR_PORT->OUT &= ~DIR_L_BIT;
    else          DIR_PORT->OUT |= DIR_L_BIT;

    // Right Motor
    if (right_fwd) DIR_PORT->OUT &= ~DIR_R_BIT;
    else           DIR_PORT->OUT |= DIR_R_BIT;
}

void Motor_Drive(uint16_t left_duty, uint16_t right_duty) {
    // Enable Motors (Wake up)
    SLP_PORT->OUT |= SLP_MASK;
    MOTOR_L_PWM_REG = left_duty;
    MOTOR_R_PWM_REG = right_duty;
}

void Motor_Stop(void) {
    SLP_PORT->OUT &= ~SLP_MASK; // Sleep mode
    MOTOR_L_PWM_REG = 0;
    MOTOR_R_PWM_REG = 0;
}

/* =========================================================================
 * 센서 처리 함수 (Sensor Processing Functions)
 * ========================================================================= */

// 하드웨어 레벨에서 한 번 센싱
uint8_t Sensor_Read_Raw(void) {
    // Turn on IR LEDs
    P5->OUT |= IR_LED_ODD_BIT;
    P9->OUT |= IR_LED_EVEN_BIT;

    // Charge Capacitors
    SENSOR_PORT->DIR = 0xFF; // Output
    SENSOR_PORT->OUT = 0xFF; // High
    Clock_Delay1us(CHARGE_TIME_US);

    // Discharge (Measure)
    SENSOR_PORT->DIR = 0x00; // Input
    Clock_Delay1us(DECAY_TIME_US);

    uint8_t result = SENSOR_PORT->IN;

    // Turn off LEDs (Optional: Power saving)
    // P5->OUT &= ~IR_LED_ODD_BIT;
    // P9->OUT &= ~IR_LED_EVEN_BIT;

    return result;
}

/**
 * 여러 번 샘플링하여 0/1 상태 배열과 중앙 감지 여부를 반환
 * @param status_out : 결과가 저장될 배열 (크기 8)
 * @return 1 if center detected, 0 otherwise
 */
int Sensor_ReadProcess(int *status_out) {
    int accu[8] = {0};
    int i, j;
    uint8_t raw_val;

    // Sampling & Accumulation
    for (i = 0; i < SENSOR_TRIAL; i++) {
        raw_val = Sensor_Read_Raw();
        for (j = 0; j < 8; j++) {
            if (raw_val & (1 << j)) {
                accu[j]++;
            }
        }
        Clock_Delay1us(300); // Interval between samples
    }

    // Thresholding
    int threshold_count = (int)(SENSOR_TRIAL * THRESHOLD);
    for (j = 0; j < 8; j++) {
        status_out[j] = (accu[j] >= threshold_count) ? 1 : 0;
    }

    // Check Center (Sensor 3 or 4)
    return (status_out[3] == 1 || status_out[4] == 1);
}

/* =========================================================================
 * 메인 함수 (Main Function)
 * ========================================================================= */

void rotate_l45() {
    Motor_Stop();
    uint32_t snapshot_r = right_enc_count;
    uint32_t snapshot_l = left_enc_count;
    uint32_t diff_r = 0;
    uint32_t diff_l = 0;
    Motor_SetDir(0,1);
    Motor_Drive(10,10);
    while( 1 ){
        diff_r = right_enc_count - snapshot_r;
        diff_l = left_enc_count - snapshot_l;
        if ( diff_r + diff_l >= 45 * 2 * 2 ) break;
    }
    Motor_Stop();
}

void rotate_r45() {
    Motor_Stop();
    uint32_t snapshot_r = right_enc_count;
    uint32_t snapshot_l = left_enc_count;
    uint32_t diff_r = 0;
    uint32_t diff_l = 0;
    Motor_SetDir(1,0);
    Motor_Drive(10,10);
    while( 1 ){
        diff_r = right_enc_count - snapshot_r;
        diff_l = left_enc_count - snapshot_l;
        if ( diff_r + diff_l >= 45 * 2 * 2 ) break;
    }
    Motor_Stop();
}

void main(void) {
    System_Init();

    Motor_SetDir(1, 1);
    int status[8];
    int detected_center = 0;
    int cnt;
    int i, j;

    while ( 1 ) {
        //Motor_Stop();
        Delay_ms(50);
        detected_center = Sensor_ReadProcess(status);
        if ( detected_center ) {
            Motor_SetDir(1,1);
            Motor_Drive(10,10);
        }
        else {
            Motor_Stop();
            int rights = status[0] + status[1] + status[2];
            int lefts = status[5] + status[6] + status[7];
            Motor_SetDir(0,0);
            Motor_Drive(10,10);
            while ( !detected_center ) {Delay_ms(50); detected_center = Sensor_ReadProcess(status), Delay_ms(50);}
            Motor_Stop();
            printf("%d %d %d\n", detected_center, rights, lefts);
            //if ( rights > 2 && lefts > 2 ) Delay_ms(100);
            if ( rights > lefts) {
                rotate_r45();
                printf("%d %d\n", right_enc_count, left_enc_count);
            }
            else if ( lefts < rights){
                rotate_l45();
                printf("%d %d\n", right_enc_count, left_enc_count);
            }
        }
    }
}
