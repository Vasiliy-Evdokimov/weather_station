#include "timer.h"

#define TIMER_PERIOD 30

#define TIMER_CHILDREN 3

extern TFT TFTscreen;
extern encoderStruct encoder;

extern bool flRedraw;

int timer_period = TIMER_PERIOD;
int timer_period_tmp = TIMER_PERIOD;

Widget timerWindow;
Widget timerValueEdit;
Widget timerBtnOk;
Widget timerBtnCancel;

Widget* timerChildren[] = {&timerValueEdit, &timerBtnOk, &timerBtnCancel};

bool timerValueEditUpdate = false;

void timerWindowResetHighlight() {
	for (int i = 0; i < TIMER_CHILDREN; i++)
		timerChildren[i]->isHighlighted = false;
}

void timerWindowHighlightChild(byte aIndex) {
	timerWindowResetHighlight();
	timerChildren[aIndex]->isHighlighted = true;
}

Widget* timerWindowGetSelectedChild() {
	Widget* res = NULL;	
	for (int i = 0; i < TIMER_CHILDREN; i++)
		if (timerChildren[i]->isSelected) {
			res = timerChildren[i];
			break;
		}
	return res;
}

Widget* timerWindowGetHighlightedChild() {
	Widget* res = NULL;	
	for (int i = 0; i < TIMER_CHILDREN; i++)
		if (timerChildren[i]->isHighlighted) {
			res = timerChildren[i];
			break;
		}
	return res;
}

void timerWindowExit() {
	timerWindowResetHighlight();
	timerWindow.isSelected = false;	
	timerValueEditUpdate = true; 
  timerValueEdit.draw(); 
}

bool timerWindowHandleEncoder() {
		encoder.minPos = 0;
    encoder.maxPos = -1;
    //
    bool changed = false;
    if (timerWindowGetSelectedChild() == NULL) {
      encoder.maxPos = 2;          
      changed = commonHandleEncoder();      
      if (changed) {
				timerWindowHighlightChild(encoder.newPos);				
			}	              
    } else if (timerValueEdit.isSelected) {
      changed = timerValueEdit.handleEncoder();
		}	
    return changed;
}

void timerWindowHandleButton() {
	if (timerWindow.isHighlighted && !timerWindow.isSelected) {
		timerWindow.isSelected = true;
		timerValueEdit.isHighlighted = true;
	} else {
		Widget* cs = timerWindowGetHighlightedChild();
		if (cs)
			cs->handleButton();
	} 
}

void timerWindowBtnOkHandleButton() {
	timer_period = timer_period_tmp;
	timerWindowExit();
}

void timerWindowBtnCancelHandleButton() {
	timer_period_tmp = timer_period;
	timerWindowExit();
}

void drawTimerValueEdit() {
  TFTscreen.setTextSize(timerValueEdit.textSize);
  //
  byte r, g, b;
  getWidgetColorByState(&timerValueEdit, &r, &g, &b);
  TFTscreen.stroke(r, g, b);    
  //
  char printout[100]; 
  //
  byte symbolWidth = getSymbolWidth(timerValueEdit.textSize);
  byte symbolHeight = getSymbolHeight(timerValueEdit.textSize);
  //
  String valueStr = (String)timer_period_tmp;
  //
  int valueLength = valueStr.length();
  int cl = valueLength * (symbolWidth + 1) - 1;
  int posX = timerValueEdit.x + (timerValueEdit.width / 2) - (cl / 2);
  int posY = timerValueEdit.y + (timerValueEdit.height / 2) - (symbolHeight / 2);    
  valueStr.toCharArray(printout, valueLength + 1);  
  if (timerValueEditUpdate) {
    TFTscreen.fill(CLR_BLACK);
    TFTscreen.rect(timerValueEdit.x, timerValueEdit.y, timerValueEdit.width, timerValueEdit.height);  
    TFTscreen.noFill();
    timerValueEditUpdate = false;
  } 
  TFTscreen.rect(timerValueEdit.x, timerValueEdit.y, timerValueEdit.width, timerValueEdit.height);  
  TFTscreen.text(printout, posX, posY); 
}

bool timerWindowValueEditHandleEncoder() {
    encoder.minPos = 0;
    encoder.maxPos = 60;
    //        
    bool changed = commonHandleEncoder();
    if (changed) {
      timer_period_tmp = encoder.newPos;
      timerValueEditUpdate = true;
      drawTimerValueEdit(); 
    } 
    //
    return changed;
}

void timerWindowValueEditHandleButton() {	
	if (timerValueEdit.isHighlighted) 
		timerValueEdit.isSelected = !timerValueEdit.isSelected;	
	//	
	if (timerValueEdit.isSelected) {
		timer_period_tmp = timer_period;
		encoder.setPosition(timer_period_tmp);		
	} else {
    encoder.setPosition(0);
	}
  //
  drawTimerValueEdit();
}

void drawTimerWindow() {
  char printout[100];
  String lines[] = {"SLEEP AFTER", "SECONDS"};
  byte positions[] = {37, 77};
	//
  if (flRedraw) {
  	drawWindowBorder(&timerWindow);
  	//
    TFTscreen.setTextSize(timerWindow.textSize);
    TFTscreen.stroke(CLR_WHITE); 
    int sw = timerWindow.width;
    String line;
    int posX, len;
    for (int i = 0; i < 2; i++) { 
      line = lines[i];
      len = line.length();
      posX = getTextCenterAlignmentX(line, timerWindow.width, timerWindow.textSize); 
      line.toCharArray(printout, len + 1);
      TFTscreen.text(printout, posX, positions[i]);
    }
  }  
  //
	for (int i = 0; i < TIMER_CHILDREN; i++)
		timerChildren[i]->draw();  
}

void drawButton(Widget* aButton) {
	TFTscreen.setTextSize(aButton->textSize);
	//
  byte r, g, b;
  getWidgetColorByState(aButton, &r, &g, &b);
  TFTscreen.fill(r, g, b);
  TFTscreen.stroke(r, g, b);        
  TFTscreen.rect(aButton->x, aButton->y, aButton->width, aButton->height);
  TFTscreen.noFill();
  //
  TFTscreen.stroke(CLR_BLACK); 
  //
  char printout[100];
  //
	byte symbolWidth = getSymbolWidth(aButton->textSize);
  byte symbolHeight = getSymbolHeight(aButton->textSize);
	//
  int cl = strlen(aButton->caption) * (symbolWidth + 1) - 1;
  int posX = aButton->x + (aButton->width / 2) - (cl / 2);
  int posY = aButton->y + (aButton->height / 2) - (symbolHeight / 2);
  TFTscreen.text(aButton->caption, posX, posY);
}

void drawTimerBtnOk() {
  drawButton(&timerBtnOk);
}

void drawTimerBtnCancel() {
	drawButton(&timerBtnCancel);
}

void initTimer() {
    timerWindow.fill("TIMER", 32, 14, TFTscreen.width() - 32, TFTscreen.height() - 14, 1);
    timerWindow.draw = drawTimerWindow;
    timerWindow.handleEncoder = timerWindowHandleEncoder;
		timerWindow.handleButton = timerWindowHandleButton;
		//
    timerBtnOk.fill("OK", 60, 108, 45, 15, 1);
    timerBtnOk.draw = drawTimerBtnOk;
		timerBtnOk.handleButton = timerWindowBtnOkHandleButton;                              
    //
    timerBtnCancel.fill("CANCEL", 110, 108, 45, 15, 1);
    timerBtnCancel.draw = drawTimerBtnCancel;
		timerBtnCancel.handleButton = timerWindowBtnCancelHandleButton;
    //
    timerValueEdit.fill("VALUE EDIT", 74, 50, 45, 20, 2);
    timerValueEdit.draw = drawTimerValueEdit;
		timerValueEdit.handleEncoder = timerWindowValueEditHandleEncoder;
		timerValueEdit.handleButton = timerWindowValueEditHandleButton;
}
