//contains all the defines
#include "definitions.h"

//contains all the text/gifs/images
#include "graphics.h"

// [Y][X][R/G/B]
uint8_t BUF[32][32][3];

void setup() {
    //set all pins for output
    gpio_set_dir_out_masked(0b111111111111001); // gpio_set_dir_masked(0x3, 0x2); -> set pin 0 to input, pin 1 to output, simultaneously.
    gpio_set_function(A, GPIO_FUNC_SIO);
    gpio_set_function(B, GPIO_FUNC_SIO);
    gpio_set_function(C, GPIO_FUNC_SIO);
    gpio_set_function(D, GPIO_FUNC_SIO);
    
        gpio_set_function(R1, GPIO_FUNC_SIO);
    gpio_set_function(R2, GPIO_FUNC_SIO);
    gpio_set_function(G1, GPIO_FUNC_SIO);
        gpio_set_function(G2, GPIO_FUNC_SIO);
    gpio_set_function(B1, GPIO_FUNC_SIO);
    gpio_set_function(B2, GPIO_FUNC_SIO);

    
    gpio_set_function(LAT, GPIO_FUNC_SIO);

//  Serial.begin(9600);//for debugging only
  Serial1.begin(9600);

}

void displayImage(const Image img, const int frameLength = 33, int ms = 0, const uint8_t x = 0, const uint8_t y = 0){
  //default to 1 loop length
  if (ms == 0){
    ms = img.numFrames*frameLength;
  }
  for (int j = 0; j < ms / (img.numFrames*frameLength); j++){
    for (int i = 0; i < img.numFrames; i++){
      for (int k = 0; k < img.height; k++){
        //96 * (y+k) + x*3:
        //(y+k) is the current y poisition in the buffer
        // the beginning of each row is 96 bytes apart
        // so to move y+k rows we multiply 96* (y+k)
        // we then move x pixels to the left. there are 3 bytes per pixel so x*3

        //w*h*3*i + w*3*k
        //there are w*h*3 bytes per frame so we multiply that by i to get to the i-th frame
        //within that frame we need to be outputting the appropriate row
        // so we take the width of a row (w*3) and multiply it by what row index we are currently on (k)
        memcpy(((unsigned char*)BUF) + 96 * (y+k) + x*3, ((unsigned char*)img.imageData) + img.width*img.height*3*i + img.width*3*k , img.width * 3);
      }
      delayWhileDisplaying(frameLength);
    }
  }
}

//add comments
void displayCharacter(const Font font, char *s, const uint8_t y = 16, const int frameLength = 33, const uint8_t squeezeFactor = 1){

clearBuffer();

const uint8_t n = strlen(s);

for(int oldX = 31; oldX > -n*(font.charWidth-squeezeFactor); oldX--){

  for(uint8_t letterIndex = 0; letterIndex < n ||  s[letterIndex] != '\0'; letterIndex++){

    const int decodedCY = (s[letterIndex] - ' ') / (font.totalWidth / font.charWidth);
    const int decodedCX = (s[letterIndex] - ' ') % (font.totalWidth / font.charWidth);

    const int x = oldX + (font.charWidth-squeezeFactor)*letterIndex;
    
      for (uint8_t k = 0; k < font.charHeight; k++){

        if (x - 31 > 0 || x + font.charWidth < 0 ){
          
        }else if (x < 0){
           memcpy(((unsigned char*)BUF) + 96 * (y+k), ((unsigned char*)font.characterData) + font.totalWidth*3*(k+decodedCY*font.charHeight) + font.charWidth*3*decodedCX + (-x)*3 , font.charWidth * 3 - (-x)*3);
        }
        else if (x+font.charWidth > 31){
           memcpy(((unsigned char*)BUF) + 96 * (y+k) + x*3, ((unsigned char*)font.characterData) + font.totalWidth*3*(k+decodedCY*font.charHeight) + font.charWidth*3*decodedCX, (32-x) * 3);
        }
        else {
        memcpy(((unsigned char*)BUF) + 96 * (y+k) + x*3, ((unsigned char*)font.characterData) + font.totalWidth*3*(k+decodedCY*font.charHeight) + font.charWidth*3*decodedCX , font.charWidth * 3);
        }
      }
  }
      
      delayWhileDisplaying(frameLength);
}
}

//if you're not going to overwrite entire buffer make sure to clear the contents first.
inline void clearBuffer() { memset(BUF, 0, 3072); }


char incomingByte = 0;
// if incoming byte = '*' read string until newline (put in temp)
char temp[255] = "test";

char incomingStr[255] = "test";


void loop() {

//incomingStr is replaced by temp in between displaying things.. to avoid glitchiness
  strcpy(incomingStr, temp);

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
          displayImage(heart, 33, 500, 17, 2);
          break;
      case 6:
          displayImage(rickRoll, 60);
          break;
      case 7:
          clearBuffer();
          displayImage(heart, 33, 500, 6, 9);
          break;
      case 8:
          displayImage(congratsRGB);
          break;
       case 9:
          displayImage(hiMom);
          break;
       case 10:
          displayImage(pika);
          break;
       case 15:
          displayCharacter(font1, incomingStr);
          break;
      default: // UCR2 by default
       displayImage(UCR2);
       break;
  }

}

bool recievedData(){
  if (Serial1.available() > 0) {
    // read the incoming byte:
    const char prev = incomingByte;
    incomingByte = Serial1.read();
    if(incomingByte == 255 || incomingByte == '\0'){
      incomingByte = prev;
      return false;
    }

    uint8_t i = 0;
    if(incomingByte == '*'){
      while (incomingByte != '\0'){
        //stringTOBeDisplayed append Serial1.read();
        incomingByte = Serial1.read();
        if(incomingByte == 255) continue;
        temp[i] = incomingByte;

    Serial.print("I received: ");
    Serial.println(incomingByte, DEC);
        
        i++;
      }
      temp[i] = '\0';
      incomingByte = 15; //whatever value represents stringDisplay in switch case
      return false;
    }

    Serial.print("I received: ");
    Serial.println(incomingByte, DEC);
          
    return true;
  }
  return false;
}


//Assuming the other operations are fast this should keep the display running 
inline void delayWhileDisplaying(const long period) {
    const long time_now = millis();
    while (millis() < time_now + period) {
        sendBuffer();

        if (recievedData()){
          //make this return true if you want it to interrupt the current image 
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

gpio_put_masked  ( 0b111100000000000, line << 11);
            
            gpio_put(LAT, 1);
            gpio_put(LAT, 0);

            digitalWrite(OE, 0); // Turn on output
            

        }
    }
}

//https://www.kyohritsu.jp/eclib/PROD/MANUAL/kprgb3232e.pdf
//https://prod.kyohritsu.com/program/KPRGB3232E_Example.cpp
//https://cdn-learn.adafruit.com/downloads/pdf/32x16-32x32-rgb-led-matrix.pdf
//https://github.com/adafruit/RGB-matrix-Panel
//https://github.com/hzeller/rpi-rgb-led-matrix
