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
- P2는 LED다.
- Px->DIR은 핀의 모드를 결정한다. 각 비트별로 1이면 출력, 0이면 입력이다.
- Px->OUT은 핀의 출력을 결정한다. 각 비트별로 1이면 1, 0이면 0이다.
- Px->IN은 입력이다. 비트마스크를 사용하여 특정 입력을 받을 수도 있다. (예: P7->IN&0x01)
- 시스템 함수들이 사용된다 (Clock_Init48MHz(), Reflectance_Init(), TExaS_Init(), Clock_Delay1us() 등)
- 각종 헤더파일은 랩 자료모음의 inc 폴더 안에 있다.

## Todo
- 강의 듣고 정리하기
