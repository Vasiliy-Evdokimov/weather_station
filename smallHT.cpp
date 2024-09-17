#include "smallHT.h"

Widget smallHT;

extern TFT TFTscreen;

void smallHTDraw() {    	
	byte positions[] = {0, 31, 45, 72};	
	String lines[SENSORS_SIZE * 2];
  uint32_t colors[SENSORS_SIZE * 2];
  bool update_flags[SENSORS_SIZE * 2];
  //
  for (int i = 0; i < SENSORS_SIZE; i++)
  {
    lines[i * 2] = Sensor::ValueToString(sensors[i].getCurrentValue());
    lines[i * 2 + 1] = sensors[i].getMeasurement();
    colors[i * 2] = sensors[i].getColor();
    colors[i * 2 + 1] = sensors[i].getColor();
    update_flags[i * 2] = sensors[i].isChanged();
    update_flags[i * 2 + 1] = 0;
  }
  //
	byte symbolWidth = getSymbolWidth(smallHT.textSize);
	byte symbolHeight = getSymbolHeight(smallHT.textSize);
	//			
	String line;
	char printout[100];
	int w;
  byte r, g, b;
	//	   
  int posX, len;
  for (int i = 0; i < SENSORS_SIZE * 2; i++) { 
	//
	line = lines[i];
    len = line.length();		
    w = len * (symbolWidth + 1);
	//		
	if (update_flags[i]) {
		TFTscreen.stroke(CLR_BLACK);
		TFTscreen.fill(CLR_BLACK);
		TFTscreen.rect(smallHT.x + positions[i] - 1, smallHT.y - 1, w + 2, symbolHeight + 2);  
		TFTscreen.noFill();
	}
	//
    colorToRGB(colors[i], &r, &g, &b);    
    TFTscreen.stroke(r, g, b);   
	//
	TFTscreen.setTextSize(smallHT.textSize);
    line.toCharArray(printout, len + 1);
    TFTscreen.text(printout, smallHT.x + positions[i], smallHT.y);
    //
    if (i == 1) {
      posX = smallHT.x + positions[i] - 4;
      TFTscreen.rect(posX, smallHT.y, 2, 2);
    }
  }	
}

void initSmallHT() {
    smallHT.fill("SMALL HT", 75, 3, 0, 0, 1);
    smallHT.draw = smallHTDraw; 
}
