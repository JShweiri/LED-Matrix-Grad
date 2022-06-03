#include "definitions.h"
#include "graphics.h"

// [Y][X][R/G/B]
uint8_t BUF[32][32][3];

void setup() {
    pinMode(R1, OUTPUT);
    pinMode(G1, OUTPUT);
    pinMode(B1, OUTPUT);
    pinMode(R2, OUTPUT);
    pinMode(G2, OUTPUT);
    pinMode(B2, OUTPUT);

    pinMode(CLK, OUTPUT);

    pinMode(LAT, OUTPUT);

    pinMode(OE, OUTPUT);
    digitalWrite(OE, LOW);

    pinMode(A, OUTPUT);
    pinMode(B, OUTPUT);
    pinMode(C, OUTPUT);
    pinMode(D, OUTPUT);

    memcpy (BUF, wilcox, 3072 * sizeof(char));
}


void loop() {

//random effect 1:
//    int x = rand() % 32;
//    int y = rand() % 32;
//    BUF[y][x][RED] = rand() % PWM_SIZE;
//    BUF[y][x][BLUE] = rand() % PWM_SIZE;
//    BUF[y][x][GREEN] = rand() % PWM_SIZE;

//random effect 2:
//for(int i = 0; i < 180; i++){
//  BUF[i/32][i%32][RED] = i%10 + i/10;
//  BUF[i/32][i%32][GREEN] = i%10;
//  BUF[i/32][i%32][BLUE] = i%10 - i/10;
//}

    delayWhileDisplaying(33);
}

inline void delayWhileDisplaying(long period){
    long time_now = millis();
    while (millis() < time_now + period) {
        sendBuffer();
    }
}

inline void sendBuffer() {

    // PWM counter 0-PWM_SIZE
for(uint8_t PWM=0;PWM<PWM_SIZE;++PWM){

    for (uint8_t line = 0; line < 16; ++line) {
        
        // Send 2 rows of color data
        uint8_t x;
        for (x = 0; x < 32; ++x) {
            digitalWrite(R1, BUF[line][x][RED]*PWM_SIZE/255 > PWM); // Output each color data to the pin
            digitalWrite(R2, BUF[line + 16][x][RED]*PWM_SIZE/255 > PWM); // Dot brightness is brighter than the current PWM counter
            digitalWrite(G1, BUF[line][x][GREEN]*PWM_SIZE/255 > PWM); // 1 when bright,
            digitalWrite(G2, BUF[line + 16][x][GREEN]*PWM_SIZE/255 > PWM); // Output 0 when it is dark
            digitalWrite(B1, BUF[line][x][BLUE]*PWM_SIZE/255 > PWM);
            digitalWrite(B2, BUF[line + 16][x][BLUE]*PWM_SIZE/255 > PWM);
            digitalWrite(CLK, 1); // Send one clock to shift the data
            digitalWrite(CLK, 0);
        }

        digitalWrite(OE, 1); // Turn off output while we switch lines and latch data

        digitalWrite(A, ((line & 1) > 0));
        digitalWrite(B, ((line & 2) > 0));
        digitalWrite(C, ((line & 4) > 0));
        digitalWrite(D, ((line & 8) > 0));

        digitalWrite(LAT, 1); // Import the contents of the shift register to the latch
        digitalWrite(LAT, 0);

        digitalWrite(OE, 0); // Turn on output
        
         }
    }
}


















//https://www.kyohritsu.jp/eclib/PROD/MANUAL/kprgb3232e.pdf
//https://prod.kyohritsu.com/program/KPRGB3232E_Example.cpp
//https://cdn-learn.adafruit.com/downloads/pdf/32x16-32x32-rgb-led-matrix.pdf
//https://github.com/adafruit/RGB-matrix-Panel
//https://github.com/hzeller/rpi-rgb-led-matrix
