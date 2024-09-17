#include "monitor.h"

Widget mainWindow;

extern TFT TFTscreen;

extern bool flRedraw;

void mainWindowDraw()
{
  if (!flRedraw)
    return;
  //
	drawWindowBorder(&mainWindow);
  //
	byte textSizes[] = { 1, 2, 1, 2 }; 
	byte positions[] = { 35, 50, 75, 90 };	
	String lines[SENSORS_SIZE * 2];
  uint32_t colors[SENSORS_SIZE * 2];
  bool update_flags[SENSORS_SIZE * 2];
  //
  for (int i = 0; i < SENSORS_SIZE; i++)
  {
    lines[i * 2] = sensors[i].getCaption() + ", " + sensors[i].getMeasurement();
    lines[i * 2 + 1] = Sensor::ValueToString(sensors[i].getCurrentValue());
    colors[i * 2] = 0xFFFFFF;
    colors[i * 2 + 1] = sensors[i].getColor();
    update_flags[i * 2] = 0;
    update_flags[i * 2 + 1] = sensors[i].isChanged();
  }  
	//	
	String line;
	char printout[100];
	byte symbolWidth, symbolHeight;
  int w;
  byte r, g, b;
	//	 
  int posX, len;
  for (int i = 0; i < SENSORS_SIZE * 2; i++)
  {
	symbolWidth = getSymbolWidth(textSizes[i]);
	symbolHeight = getSymbolHeight(textSizes[i]);
	//
	if (update_flags[i])
	{
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
    if (!i)
    {
      posX = posX + w - symbolWidth - 4;
      TFTscreen.rect(posX, positions[i], 2, 2);
    }
  }
}

void initMainWindow() {
    mainWindow.fill("MAIN WINDOW", 32, 14, TFTscreen.width() - 32, TFTscreen.height() - 14, 1); 
    mainWindow.draw = mainWindowDraw;
}
