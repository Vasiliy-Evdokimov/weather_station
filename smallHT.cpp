#include "smallHT.h"

Widget smallHT;

extern Sensor temperature;
extern Sensor humidity;

extern TFT TFTscreen;

short prevT2 = 0;
short prevH2 = 0;

void smallHTDraw() {    	
	byte positions[] = {0, 31, 45, 72};	
	uint32_t colors[] = {temperature.color, temperature.color, humidity.color, humidity.color};
	short currT = temperature.getCurrentValue();
	short currH = humidity.getCurrentValue();
	bool update_flag[] = {(currT != prevT2), 0, (currH != prevH2), 0};
	prevT2 = currT;
	prevH2 = currH;  
	String lines[] = {
		convertValueToStr(currT),    
		"C",
    convertValueToStr(currH),
		"%"   		
	};		
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
  for (int i = 0; i < 4; i++) { 
		//
		line = lines[i];
    len = line.length();		
    w = len * (symbolWidth + 1);
		//		
		if (update_flag[i]) {
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
