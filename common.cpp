#include "common.h"

RotaryEncoder rotary_encoder(3, 4);
TFT TFTscreen = TFT(10, 9, 8);

encoderStruct encoder;

void initEncoder() {
  encoder._encoder = &rotary_encoder;   
}

void encoderStruct::tick() {
  _encoder->tick();
  currPos = getPosition();
}
  
void encoderStruct::setPosition(int aValue) {
  _encoder->setPosition(aValue);   
}

int encoderStruct::getPosition() {
  return _encoder->getPosition();  
}

byte getSymbolWidth(byte aTextSize) {
  byte res = 5;
  switch (aTextSize) {
    case 2: 
      res = 10;
      break;  
  }
  return res;
}

byte getSymbolHeight(byte aTextSize) {
  byte res = 7;
  switch (aTextSize) {
    case 2: 
      res = 14;
      break;
  }
  return res;  
}

byte getTextCenterAlignmentX(String aLine, byte aWidth, byte aTextSize) {
  byte sw = getSymbolWidth(aTextSize);
  byte len = aLine.length();    
  byte w = len * (sw + 1);
  return (aWidth / 2) - (w / 2) + 32;     
}

bool commonHandleEncoder() {
  encoder.tick();
  encoder.currPos = encoder.getPosition();
  //
  if (encoder.currPos < encoder.minPos) {
    encoder.setPosition(encoder.minPos);
    encoder.newPos = encoder.minPos;
  } else if (encoder.currPos > encoder.maxPos) {
    encoder.setPosition(encoder.maxPos);
    encoder.newPos = encoder.maxPos;
  } else
    encoder.newPos = encoder.currPos;
  //  
  bool changed = (encoder.lastPos != encoder.newPos);
  if (encoder.lastPos != encoder.newPos) {
    encoder.lastPos = encoder.newPos;
  }
  return changed;
}

void colorToRGB(const uint32_t aColor, byte* aR, byte* aG, byte* aB) {
  *aR = (aColor & 0xFF0000) >> 16;        
  *aG = (aColor & 0x00FF00) >> 8;
  *aB = aColor & 0x0000FF;  
}
