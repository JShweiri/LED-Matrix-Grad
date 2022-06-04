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


  Serial.begin(9600);//for debugging only
  Serial1.begin(9600);
//  while (!Serial);

}

//move width and height into struct

void displayImage(Image img, int ms = 0, int frameLength = 33, int x = 0, int y = 0, int w = 32, int h = 32){
  //default to 1 loop length
  if (ms == 0){
    ms = img.size*frameLength;
  }
  for (int j = 0; j < ms / (img.size*frameLength); j++){
    for (int i = 0; i < img.size; i++){
      for (int k = 0; k < h; k++){
        //96 * (y+k) + x*3:
        //(y+k) is the current y poisition in the buffer
        // the beginning of each row is 96 bytes apart
        // so to move y+k rows we multiply 96* (y+k)
        // we then move x pixels to the left. there are 3 bytes per pixel so x*3

        //w*h*3*i + w*3*k
        //there are w*h*3 bytes per frame so we multiply that by i to get to the i-th frame
        //within that frame we need to be outputting the appropriate row
        // so we take the width of a row (w*3) and multiply it by what row index we are currently on (k)
        memcpy(((unsigned char*)BUF) + 96 * (y+k) + x*3, ((unsigned char*)img.frames) + w*h*3*i + w*3*k , w * 3);
      }
      delayWhileDisplaying(frameLength);
    }
  }
}

////using the width position and what frame we are on find the bits needed to be imported into the buffer from fontimage
//void displayString(Image font, string s, int x = 0, int y = 0, int ms = 33){
//  for (int j = 0; j < ms / (img.size*frameLength); j++){
//      for (int k = 0; k < h; k++){
//        memcpy(((unsigned char*)BUF) + 96 * (y+k) + x*3, ((unsigned char*)img.frames) + w*h*3*i + w*3*k , w * 3);
//      }
//      delayWhileDisplaying(frameLength);
//  }
//}

//if you're not going to overwrite entire buffer make sure to clear the contents first.
void clearBuffer() { memset(BUF, 0, 3072); }

int incomingByte = 0;

void loop() { 

switch (incomingByte)
{
  
    case 1:
        displayImage(rainbowSwirl);
        break;
    case 2:
        displayImage(UCR1);
        break;
    case 3:
        displayImage(wilcox);
        break;
    case 4:
        displayImage(UCR2);
        break;
    case 5:
        displayImage(pika);
        break;
    case 6:
        displayImage(rickRoll, 5000, 60);
        break;
    case 7:
        clearBuffer();
        displayImage(heart, 1000, 33, 6, 9, 13, 12);
        break;
    default: // UCR2 by default
     displayImage(UCR2);
     break;
}

}

bool recievedData(){
  if (Serial1.available() > 0) {
    // read the incoming byte:
    incomingByte = Serial1.read();

    Serial.print("I received: ");
    Serial.println(incomingByte, DEC);
          
    return true;
  }
  return false;
}


//Assuming the other operations are fast this should keep the display running 
inline void delayWhileDisplaying(long period) {
    long time_now = millis();
    while (millis() < time_now + period) {
        sendBuffer();

        //put this inside sendBuffer if you want to be able to interrupt a gif/image
        if (recievedData()){
          break;
        }
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
