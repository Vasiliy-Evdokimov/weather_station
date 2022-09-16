#include "plot.h"

#include "common.h"
#include "sensor.h"
#include "widget.h"

#define PLOT_START_X 65
#define PLOT_START_Y 60
#define PLOT_LEGEND_STEP 20
#define PLOT_GRID_STEP 10
#define PLOT_LEGEND_X 45
#define PLOT_HIGH_X (160 - 12)
#define PLOT_HIGH_Y (128 - 12)

extern TFT TFTscreen;

Sensor* currentSensor;  

Widget plotWindow;

extern bool flRedraw;

short yHigh = 0, yLow = 0, yStep = 0, yHighPrev = 0, yLowPrev = 0;
short prevV = 0;

void togglePlotSensor(Sensor* aSensor) {
  currentSensor = aSensor;
  yHigh = 0; 
  yLow = 0; 
  yStep = 0; 
  yHighPrev = 0;
  yLowPrev = 0;  
}

void drawGridLine(int x, int y) {
  TFTscreen.stroke(CLR_GRID_LINE);
  for (int i = x; i < PLOT_HIGH_X - 1; i += 5)
    TFTscreen.point(i, y);  
}

void drawGridLines() {
  for (int i = yHigh, j = PLOT_START_Y; i >= yLow; i -= yStep, j += PLOT_GRID_STEP)
    drawGridLine(PLOT_START_X + 1, j + 3);
}

void drawAxis() {  
  TFTscreen.stroke(CLR_BLACK);  
  TFTscreen.fill(CLR_BLACK);
  TFTscreen.rect(PLOT_LEGEND_X - 1, PLOT_START_Y - 1, 
    plotWindow.x + plotWindow.width - PLOT_LEGEND_X, 
    plotWindow.y + plotWindow.height - PLOT_START_Y);  
  TFTscreen.noFill();
  //
  TFTscreen.setTextSize(plotWindow.textSize);
  TFTscreen.stroke(CLR_WHITE);  
  char printout[10];
  for (int i = yHigh, j = PLOT_START_Y; i >= yLow; i -= yStep, j += PLOT_GRID_STEP) {
    String line = String(i);
    line.toCharArray(printout, line.length() + 1);
    TFTscreen.text(printout, PLOT_LEGEND_X, j);
    TFTscreen.line(PLOT_START_X - 1, j + 3, PLOT_START_X + 1, j + 3); 
  }  
  TFTscreen.line(PLOT_START_X, PLOT_START_Y + 3, PLOT_START_X, PLOT_HIGH_Y - 3);
}

void clearPlot() {
  TFTscreen.stroke(CLR_BLACK);
  TFTscreen.fill(CLR_BLACK);
  //  
  TFTscreen.rect(
    PLOT_START_X + 1, 
    PLOT_START_Y + 3, 
    PLOT_HIGH_X - PLOT_START_X - 1, 
    (PLOT_HIGH_Y - 3) - (PLOT_START_Y + 1)
  );
  //
	TFTscreen.noFill();
}

void plotDraw() {
    if (!currentSensor) 
        return;
    //
    const byte caption_y = 30;
    const byte currV_y = 45; 
    //
    byte x, y, len;
    byte r, g, b;    
    //
    char printout[100];    
    //
    String line;
    byte symbolWidth = getSymbolWidth(plotWindow.textSize);
    byte symbolHeight = getSymbolHeight(plotWindow.textSize);
    //
    TFTscreen.setTextSize(plotWindow.textSize);
    colorToRGB(currentSensor->color, &r, &g, &b);
    //    
    if (flRedraw) {
  		drawWindowBorder(&plotWindow);      
      line = currentSensor->caption;    
      line.toCharArray(printout, line.length() + 1);
      x = getTextCenterAlignmentX(line, plotWindow.width, 1);
      TFTscreen.stroke(CLR_WHITE);
      TFTscreen.text(printout, x, caption_y);           
    }          
    //
    short currV = currentSensor->getCurrentValue();
    if ((currV != prevV) || flRedraw) {
      line = convertValueToStr(currV);
      len = line.length();
      line.toCharArray(printout, len + 1);
      x = getTextCenterAlignmentX(line, plotWindow.width, 1);
      //      
      TFTscreen.stroke(CLR_BLACK);
      TFTscreen.fill(CLR_BLACK);
      TFTscreen.rect(x - 1, currV_y - 1, (symbolWidth + 1) * len + 2, symbolHeight + 2);
      TFTscreen.noFill();
      prevV = currV;
      //
      TFTscreen.stroke(r, g, b); 
      TFTscreen.text(printout, x, currV_y);    
    }    
		//
    short min_value = currentSensor->min_value / 10;  
    short max_value = currentSensor->max_value / 10;
    //    
    yLow = (min_value / 10) * 10;
    yHigh = ((max_value / 10) + 1) * 10;
    yStep = (yHigh - yLow) / 5; 
    //
    if ((yLow != yLowPrev) || (yHigh != yHighPrev) || flRedraw) {
        drawAxis();  
        yLowPrev = yLow;
    		yHighPrev = yHigh;
  	} 		
		//
		clearPlot();  
  	drawGridLines();
    //    
    TFTscreen.stroke(r, g, b); 
		for (int i = 0 ; i < SENSOR_DATA_LENGTH ; i++) {
			float v = currentSensor->getValue(i) / 10.;		
      x = i * 3 + PLOT_START_X + 1;
      y = (int)(PLOT_HIGH_Y - 3 - (float(v - yLow) / yStep) * PLOT_GRID_STEP);			
      if (x <= PLOT_HIGH_X)
  		  TFTscreen.point(x, y);
		}   
}

void initPlot() {
    plotWindow.fill("SENSOR PLOT", 32, 14, TFTscreen.width() - 32, TFTscreen.height() - 14, 1); 
    plotWindow.draw = plotDraw;    
}
