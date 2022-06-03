//contains all the defines
#include "definitions.h"

//contains all the text/gifs/images
#include "graphics.h"

// [Y][X][R/G/B]
uint8_t BUF[32][32][3];

void setup() {
    //set all pins for output

    //these pins are for controlling the upper? RGB value to be shifted/clocked into the temp registers
    pinMode(R1, OUTPUT);
    pinMode(G1, OUTPUT);
    pinMode(B1, OUTPUT);

    //these pins are for controlling the lower RGB value to be shifted/clocked into the temp registers
    pinMode(R2, OUTPUT);
    pinMode(G2, OUTPUT);
    pinMode(B2, OUTPUT);

    //this is the clk pin used for pulsing our data into the temp registers 
    pinMode(CLK, OUTPUT);

    //the latch pin stores the value in the temp register to the main register
    pinMode(LAT, OUTPUT);

    // the output enable pin allows the contents of the main register to be displayed on the LEDS
    pinMode(OE, OUTPUT);
    digitalWrite(OE, HIGH); //initialize off

    // the ABCD select pins allow you to choose which 2 rows you are outputting the main register to
    //the rows light together like this: (0 and 16), (1 and 17), etc..
    pinMode(A, OUTPUT);
    pinMode(B, OUTPUT);
    pinMode(C, OUTPUT);
    pinMode(D, OUTPUT);

    //Only 2 rows (192 LEDs) can be on at once
    //this is why the temp and main registers are 24 Bytes. 1bit per color(LED) * 192 pixels
    // we use persistance of vision to maintain an image.
    // PWM is not supported either so we use a loop to do it ourselves.

}

void displayImage(Image img, int ms, int frameLength = 33){
  for (int j = 0; j < ms / (img.size*frameLength); j++){
    for (int i = 0; i < img.size; i++){
      memcpy(BUF, ((unsigned char*)img.frames)+ i*3072, 3072 * sizeof(char));
      delayWhileDisplaying(frameLength);
    }
  }
}


void loop() { 
//    displayImage(rainbowSwirl, 1000); 
//    displayImage(UCR1, 1000);
//    displayImage(UCR2, 1000);
    displayImage(rainbowSwirl, 1000);
}

//make gif a struct with data and numFrames
//regular image can just be 1 frame


//Assuming the other operations are fast this should keep the display running 
inline void delayWhileDisplaying(long period) {
    long time_now = millis();
    while (millis() < time_now + period) {
        sendBuffer();
    }
}

//this should take in a buffer? then no data needs to be moved... idk
inline void sendBuffer() {

    for (uint8_t PWM = 0; PWM < PWM_SIZE; ++PWM) {

        for (uint8_t line = 0; line < 16; ++line) {

            // Send 2 rows of color data
            uint8_t x;
            for (x = 0; x < 32; ++x) {
                digitalWrite(R1, BUF[line][x][RED] * PWM_SIZE / 255 > PWM); // Output each color data to the pin
                digitalWrite(R2, BUF[line + 16][x][RED] * PWM_SIZE / 255 > PWM); // Dot brightness is brighter than the current PWM counter
                digitalWrite(G1, BUF[line][x][GREEN] * PWM_SIZE / 255 > PWM); // 1 when bright,
                digitalWrite(G2, BUF[line + 16][x][GREEN] * PWM_SIZE / 255 > PWM); // Output 0 when it is dark
                digitalWrite(B1, BUF[line][x][BLUE] * PWM_SIZE / 255 > PWM);
                digitalWrite(B2, BUF[line + 16][x][BLUE] * PWM_SIZE / 255 > PWM);
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
