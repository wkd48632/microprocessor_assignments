# 마이크로프로세서응용 과제
> This repository was created for assignments and activities in the Microprocessor Applications class.

## 자료
- 강의 페이지  
[https://www.ti.com/ko-kr/video/series/ti-robotics-system-learning-kit-max-ti-rslk-max-curriculum-modules.html](https://www.ti.com/ko-kr/video/series/ti-robotics-system-learning-kit-max-ti-rslk-max-curriculum-modules.html)
- 랩 자료모음  
[http://users.ece.utexas.edu/~valvano/arm/tirslk_max_1_00_00.zip](http://users.ece.utexas.edu/~valvano/arm/tirslk_max_1_00_00.zip)

## 노트
- Px 는 포트다.
- Px.y는 x포트의 y번째 핀을 의미하며, 비트마스크로 조작할 수 있다.
- Px->DIR은 핀의 모드를 결정한다. 각 비트별로 1이면 출력, 0이면 입력이다.
- Px->OUT은 핀의 출력을 결정한다. 각 비트별로 1이면 1, 0이면 0이다.
- Px->IN은 입력이다. 비트마스크를 사용하여 특정 입력을 받을 수도 있다. (예: P7->IN&0x01)
- 시스템 함수들이 사용된다 (Clock_Init48MHz(), Reflectance_Init(), TExaS_Init(), Clock_Delay1us() 등)
- 각종 헤더파일은 랩 자료모음의 inc 폴더 안에 있다.
- SEL0과 SEL1을 설정해 핀의 용도를 지정할 수 있다.
    - SEL1:0, SEL0:0 : GPIO
    - SEL1:0, SEL0:1 : Primary alternate function
    - SEL1:1, SEL1:0 : Secondary alternate function
    - SEL1:1, SEL1:1 : Ternary alternative function / Analog / Other
- DIR, REN, OUT을 설정해 풀업/다운을 설정할 수 있다.
    - DIR:0, REN:0, OUT:0 : INPUT
    - DIR:0, REN:1, OUT:0 : INPUT_PULLDOWN
    - DIR:0, REN:1, OUT:1 : INPUT_PULLUP
- 보드 스위치와 범프를 사용할 때에는 풀업 해줘야한다.
- 컵파일러는 'true'를 모른다. 따로 선언해야한다.

## 핀 맵
- P2: LED ( 0:R, 1:G, 2:B )
- P4: 충돌 ( 0:B0(오른쪽 센서), 2:B1, 3:B2, 5:B3, 6:B4, 7:B5(왼쪽 센서) )
- P7: 반사 ( 0:R1(오른쪽 센서: 왼쪽으로 기울었음), 1:R2, 2: R3, 3: R4(중앙), 4:R5(중앙), 5:R6, 6:R7, 7: R8(왼쪽 센서: 오른쪽으로 기울었음)
- 반사 센서를 위한 LED: P5.3(짝수번째), P9.2(홀수번째)
- 모터
  - 방향( 좌:P5.4, 우:P5.5 )
  - PWM( 좌:P2.7  우:P2.6 )
  - 사용( 좌:P3.7  우:P3.6 )
  - 모터 위치( Tachometer, 좌A: P10.5, 좌B: P5.2, 우A: P10.4, 우B: P5.0 )

## Todo
- 강의 듣고 정리하기
