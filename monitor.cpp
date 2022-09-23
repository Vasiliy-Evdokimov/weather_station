#include "monitor.h"

Widget mainWindow;

extern Sensor temperature;
extern Sensor humidity;

extern TFT TFTscreen;

extern bool flRedraw;

short prevT = 0;
short prevH = 0;

void mainWindowDraw() {
  if (!flRedraw)
    return;
  //
	drawWindowBorder(&mainWindow);
  //
	byte textSizes[] = {1, 2, 1, 2}; 
	byte positions[] = {35, 50, 75, 90};	
	uint32_t colors[] = {0xFFFFFF, temperature.color, 0xFFFFFF, humidity.color};
	short currT = temperature.getCurrentValue();
	short currH = humidity.getCurrentValue();
	bool update_flag[] = {0, (currT != prevT), 0,  (currH != prevH)};
	prevT = currT;
	prevH = currH;  
	String lines[] = {
		"TEMPERATURE, C",
    convertValueToStr(currT),    
		"HUMIDITY, %",
    convertValueToStr(currH)		
	};		
	//	
	String line;
	char printout[100];
	byte symbolWidth, symbolHeight;
  int w;
  byte r, g, b;
	//	 
  int posX, len;
  for (int i = 0; i < 4; i++) { 
		symbolWidth = getSymbolWidth(textSizes[i]);
		symbolHeight = getSymbolHeight(textSizes[i]);
		//
		if (update_flag[i]) {
			TFTscreen.stroke(CLR_BLACK);
			TFTscreen.fill(CLR_BLACK);
			TFTscreen.rect(mainWindow.x + 1, positions[i] - 1, mainWindow.width - 2, symbolHeight + 2);  
			TFTscreen.noFill();
		}
		//
    colorToRGB(colors[i], &r, &g, &b);		
		TFTscreen.stroke(r, g, b);   
		//		    
    line = lines[i];
    len = line.length();		
    w = len * (symbolWidth + 1);    
    posX = getTextCenterAlignmentX(line, mainWindow.width, textSizes[i]); 
		//
		TFTscreen.setTextSize(textSizes[i]);
    line.toCharArray(printout, len + 1);
    TFTscreen.text(printout, posX, positions[i]);
    //
    if (!i) {
      posX = posX + w - symbolWidth - 4;
      TFTscreen.rect(posX, positions[i], 2, 2);
    }
  }    
}

void initMainWindow() {
    mainWindow.fill("MAIN WINDOW", 32, 14, TFTscreen.width() - 32, TFTscreen.height() - 14, 1); 
    mainWindow.draw = mainWindowDraw;
}
