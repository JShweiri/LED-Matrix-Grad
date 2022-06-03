#include "definitions.h"
//Define a header with all characters, heart, ucr logo, images...

//PWM Quanta
#define PWM_SIZE 16.0

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

}

void loop() {
    int x = 32.0 * rand() / RAND_MAX;
    int y = 32.0 * rand() / RAND_MAX;
    BUF[y][x][RED] = PWM_SIZE * rand() / RAND_MAX;
    BUF[y][x][BLUE] = PWM_SIZE * rand() / RAND_MAX;
    BUF[y][x][GREEN] = PWM_SIZE * rand() / RAND_MAX;

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
    //        for(uint8_t PWM=0;PWM<PWM_SIZE;PWM++){

    for (uint8_t line = 0; line < 16; line++) {

        digitalWrite(OE, 1); // Turn off output (maybe remove this if too dim)

        // somehow all 1s
        digitalWrite(A, ((line & 1) > 0) ? 1 : 0);
        digitalWrite(B, ((line & 2) > 0) ? 1 : 0);
        digitalWrite(C, ((line & 4) > 0) ? 1 : 0);
        digitalWrite(D, ((line & 8) > 0) ? 1 : 0);

        // Send 2 rows of color data
        uint8_t x;
        for (x = 0; x < 32; x++) {
            digitalWrite(R1, F_BUF_R[line][x] > PWM); // Output each color data to the pin
            digitalWrite(R2, F_BUF_R[line + 16][x] > PWM); // Dot brightness is brighter than the current PWM counter
            digitalWrite(G1, F_BUF_G[line][x] > PWM); // 1 when bright,
            digitalWrite(G2, F_BUF_G[line + 16][x] > PWM); // Output 0 when it is dark
            digitalWrite(B1, F_BUF_B[line][x] > PWM);
            digitalWrite(B2, F_BUF_B[line + 16][x] > PWM);
            digitalWrite(CLK, 1); // Send one clock to shift the data
            digitalWrite(CLK, 0);
        }
        
        digitalWrite(OE, 0); // Turn on output
        
        digitalWrite(LAT, 1); // Import the contents of the shift register to the latch
        digitalWrite(LAT, 0);
        
        delayMicroseconds(600); // to prevent ghosting 500-1000

//         }
    }
}
