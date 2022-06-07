#ifndef DEFINITIONS_H
#define DEFINITIONS_H

//               PIN ASSIGNMENTS

// There are no special reasons I chose these pins
#define R1 0
#define G1 3
#define B1 4
#define R2 5
#define G2 6
#define B2 7

#define CLK 8

#define LAT 9

#define OE 10

#define A 11
#define B 12
#define C 13
#define D 14

#define RED 0
#define GREEN 1
#define BLUE 2


//                OTHER


//PWM Quanta  lower this for less colors and higher performance
#define PWM_SIZE 10

struct Image {
const unsigned char numFrames;

const unsigned char height;
const unsigned char width;

const void *const  imageData;
};

struct Font {
const unsigned char totalHeight;
const unsigned char totalWidth;

const unsigned char charHeight;
const unsigned char charWidth;

const void* const characterData;
};

#endif
