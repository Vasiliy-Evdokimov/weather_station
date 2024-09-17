#ifndef COMMON_H
#define COMMON_H

#include "RotaryEncoder.h"
#include <TFT.h>
#include <string.h>

#define CLR_WHITE 255, 255, 255
#define CLR_BLACK 0, 0, 0
#define CLR_YELLOW 255, 255, 0
#define CLR_GREEN 0, 255, 0
#define CLR_RED 255, 0, 0

#define CLR_WHITE_H 0xFFFFFF
#define CLR_BLACK_H 0x000000
#define CLR_YELLOW_H 0xFFFF00
#define CLR_GREEN_H 0x00FF00
#define CLR_RED_H 0xFF0000
#define CLR_DARK_GREY_H 0xA9A9A9

#define CLR_GRID_LINE 105, 105, 105

#define CAPTION_LENGTH 15

typedef void(*void_function_pointer)();

struct encoderStruct {
  RotaryEncoder* _encoder;
  //
  int currPos = 0;
  int newPos = 0;
  int lastPos = 0;
  int maxPos = 0;
  int minPos = 0; 
  //
  void tick();
  void setPosition(int aValue);
  int getPosition();
};

void initEncoder();

byte getSymbolWidth(byte aTextSize);

byte getSymbolHeight(byte aTextSize);

bool commonHandleEncoder();

void colorToRGB(const uint32_t aColor, byte* aR, byte* aG, byte* aB);

byte getTextCenterAlignmentX(String aLine, byte aWidth, byte aTextSize);

#endif
