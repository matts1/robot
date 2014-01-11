#include "i2c.h"  // not the wire library, can't use pull-ups

#ifndef ACCEL_H
#define ACCEL_H

#define byte uint8_t

#define SA0 1  // Breakout board defaults to 1, set to 0 if SA0 jumper is set
#if SA0
  #define MMA8452_ADDRESS 0x1D  // SA0 is high, 0x1C if low
#else
  #define MMA8452_ADDRESS 0x1C
#endif

/* Set the scale below either 2, 4 or 8*/
const byte scale = 2;  // Sets full-scale range to +/-2, 4, or 8g. Used to calc real g values.
/* Set the output data rate below. Value should be between 0 and 7*/
const byte dataRate = 0;  // 0=800Hz, 1=400, 2=200, 3=100, 4=50, 5=12.5, 6=6.25, 7=1.56

byte data[6];  // x/y/z accel register data store here

typedef struct Point { // To store the calculated point
  int x;
  int y;
  int z;
} Point;

int readAccel(); // Reading the accel and calculating the value...

#endif
