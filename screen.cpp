#include "screen.h"

Widget screen;

bool sleepScreen = false; 
bool goSleepScreen = false;
unsigned long power_timer;

extern bool flRedraw;

extern TFT TFTscreen;
extern encoderStruct encoder;

extern Widget mainMenu;
extern MenuItem menu[MENU_SIZE];

extern Widget mainWindow;
extern Widget plotWindow;
extern Widget timerWindow;
extern Widget smallHT;

Widget* htWindowWidgets[] = {&smallHT, &mainWindow, &plotWindow};
Widget* windowWidgets[] = {&smallHT, &mainWindow, &plotWindow, &timerWindow};
Widget* currentWindow; 

extern Sensor temperature;
extern Sensor humidity;

void drawHTWindows() {
  flRedraw = temperature.hasChanged() || humidity.hasChanged();  
  mainMenu.redraw = flRedraw;
  for (int i = 0; i < 3; i++)
    if (htWindowWidgets[i]->isVisible)
      htWindowWidgets[i]->draw();
  flRedraw = false;      
  //    
  if (mainMenu.redraw)
    mainMenu.draw();
}

void screenHideWindowWidgets() {
  for (int i = 0; i < 4; i++) {
    windowWidgets[i]->isVisible = false;  
  }  
}

void screenShowWidget(Widget* aWidget) {
  mainMenuCollapse();
  screenHideWindowWidgets();  
  aWidget->isVisible = true;
  smallHT.isVisible = !mainWindow.isVisible;
  //
  currentWindow = aWidget;
  //
  mainMenu.redraw = true;
  //
  flRedraw = true;
  screen.draw();
  flRedraw = false;
}

bool screenHandleEncoder() {
    encoder.minPos = 0;    
    encoder.maxPos = 0;    
    //     
    bool changed = false;
    if (!menu[0].isSelected && 
        !currentWindow->isSelected && 
        currentWindow->handleEncoder) {
      //    
      encoder.maxPos = 1;    
      changed = commonHandleEncoder();
      if (changed) {
        menu[0].isHighlighted = false;    
        currentWindow->isHighlighted = false;
        switch (encoder.newPos) {
          case 0:
            menu[0].isHighlighted = true;                            
            break;
          case 1:
            currentWindow->isHighlighted = true;
            break;
        }        
      }                    
      mainMenu.redraw = changed;
    } else if (menu[0].isSelected) {
      mainMenu.redraw = mainMenu.handleEncoder();
      return false;
    } else if (currentWindow->isSelected) {
      changed = currentWindow->handleEncoder();      
      //mainMenu.draw();
    }
    //
    return changed;
}

void screenHandleButton() {
  if (menu[0].isHighlighted) {
    mainMenu.handleButton();
    mainMenu.redraw = true;
  } else if (currentWindow->isHighlighted && currentWindow->handleButton) {
    currentWindow->handleButton();
    mainMenu.redraw = true;
  }  
  //
  TFTscreen.background(CLR_BLACK);
}

void screenDraw() {  
  for (int i = 0; i < 4; i++)
    if (windowWidgets[i]->isVisible)
      windowWidgets[i]->draw();
  //    
  if (mainMenu.redraw)
    mainMenu.draw();
}

void screenInit() {
	screen.fill("WEATHER", 0, 0, TFTscreen.width(), TFTscreen.height(), 1);
	TFTscreen.background(0, 0, 0);
	//
	screen.draw = screenDraw;
	screen.handleButton = screenHandleButton;
	screen.handleEncoder = screenHandleEncoder;
	//  
	initMainMenu();
  initSensors();
  //
  initTimer();   
  initPlot();
  initMainWindow();
  initSmallHT();
  //      
  screenShowWidget(&mainWindow);
}

void screenDrawSleep() {
  screenHideWindowWidgets();
  //
  TFTscreen.background(CLR_BLACK);
  TFTscreen.setTextSize(1);
  TFTscreen.stroke(CLR_WHITE);
  //
  String lines[] = { "I AM GOING TO", "SLEEP IN 5 SECONDS", 
    "PUSH THE BUTTON", "TO WAKE ME UP"};
  byte posY[] = {10, 20, 100, 110};
  char printout[100];
  byte posX, w;
  String line;
  w = TFTscreen.width();
  for (int i = 0; i < 4; i++)
  {
    line = lines[i];    
    posX = getTextCenterAlignmentX(line, w, 1) - 16;
    line.toCharArray(printout, line.length() + 1);
    TFTscreen.text(printout, posX, posY[i]);
  }
  //    
  const byte startX = 50;
  const byte startY = 44;
  const byte sideW = 40;
  const byte innerOff = 7;
  const byte pinStep = 5;
  const byte pinLen = 5;
  const byte innerStep = 6;
  // 
  TFTscreen.rect(startX, startY, sideW, sideW);  
  TFTscreen.rect(startX + innerOff, startY + innerOff, 
    sideW - innerOff * 2, sideW - innerOff * 2);
  for (int i = startX + pinStep; i < startX + sideW; i += pinStep) {
    TFTscreen.line(i, startY - pinLen, i, startY);
    TFTscreen.line(i, startY + sideW, i, startY + sideW + pinLen);    
  }
  for (int i = startY + pinStep; i < startY + sideW; i += pinStep) {
    TFTscreen.line(startX - pinLen, i, startX, i);
    TFTscreen.line(startX + sideW, i, startX + sideW + pinLen, i);
  }    
  //
  line = "Z";
  line.toCharArray(printout, line.length() + 1);  
  const byte countZ = 4;
  byte posZ[countZ][3] = {{100, 50, 1}, {110, 55, 1}, {120, 50, 1}, {130, 55, 1}}; 
  for (int i = 0; i < countZ; i++) {
    TFTscreen.setTextSize(posZ[i][2]);
    TFTscreen.text(printout, posZ[i][0], posZ[i][1]);
  }
}

void screenWakeUp() {
  TFTscreen.background(CLR_BLACK);
  sleepScreen = false;
  goSleepScreen = false;
  power_timer = millis();
  //
  mainMenuCollapse();
  screenShowWidget(&mainWindow);
  //
  flRedraw = true;
  mainMenu.redraw = true;
  screen.draw();
  flRedraw = false;
}
