#include "menu.h"

extern TFT TFTscreen;
extern encoderStruct encoder;

extern encoderStruct encoder;

extern Widget mainWindow;
extern Widget plotWindow;
extern Widget timerWindow;

extern Sensor temperature;
extern Sensor humidity;

extern bool flRedraw;

extern Widget screen;

extern bool goSleepScreen;

Widget mainMenu;

MenuItem menu[MENU_SIZE];

void drawMenuItem(MenuItem *aMenuItem, int aStartX, int aStartY) {
  int x;
  //
  byte symbolWidth = getSymbolWidth(mainMenu.textSize);
  byte symbolHeight = getSymbolHeight(mainMenu.textSize);
  //
  int cl = strlen(aMenuItem->caption);
  if (aMenuItem->parent != NULL)    
    cl = aMenuItem->parent->getChildrenMaxCaptionLength();    
  //  
  byte r, g, b;  
  uint32_t clr;
  if (aMenuItem->isHighlighted) {
    if (aMenuItem->isSelected)
      clr = CLR_GREEN_H;      
    else 
      clr = CLR_YELLOW_H;   
  } else  
      clr = CLR_DARK_GREY_H;   
  //
  colorToRGB(clr, &r, &g, &b);
  TFTscreen.fill(r, g, b);
  TFTscreen.stroke(r, g, b);        
  TFTscreen.rect(aStartX, aStartY, (symbolWidth + 1) * cl + 3, symbolHeight + 4);
  TFTscreen.noFill();
  //
  TFTscreen.stroke(CLR_BLACK);    
  TFTscreen.text(aMenuItem->caption, aStartX + 2, aStartY + 2);
  //  
  if (aMenuItem->isSelected) {   
    int j = 0;
    if (aMenuItem->getLevel() == 0) {    
       x = aStartX + 3;
       aStartY += (7 + 4);
    } else
       x = aStartX + (symbolWidth + 1) * cl + 2;    
    //
    cl = aMenuItem->getChildrenMaxCaptionLength();		
		for (int i = 0; i < MENU_SIZE; i++)
			if ((menu[i].parent == aMenuItem) && (menu[i].isVisible)) {
				drawMenuItem(
					&menu[i], 
					x, 
					aStartY + (7 + 4) * j
				);	
				j++;	
			} 
    //      
    TFTscreen.stroke(CLR_WHITE);          
    TFTscreen.rect(x, aStartY, (symbolWidth + 1) * cl + 3, (symbolHeight + 4) * j);    
  } 
  //
  TFTscreen.noFill();   
}

void mainMenuDraw() {
  Serial.println("mainMenuDraw()");
  //
  TFTscreen.setTextSize(mainMenu.textSize);
  //
	drawMenuItem(&menu[0], mainMenu.x, mainMenu.y);  
  //
  mainMenu.redraw = false;
}

bool mainMenuHandleEncoder() {
  MenuItem* cs = menu[0].getCurrentSelected();
  //    
  encoder.minPos = 0;  
  //
  if (cs == NULL)            
    encoder.maxPos = 0;
  else  
    encoder.maxPos = cs->getVisibleChildrenSize() - 1;  
  //
  bool changed = commonHandleEncoder();
  //
  if (changed && cs)      
      cs->setChildHighlighted(encoder.newPos);    
  //
  mainMenu.redraw = changed;
  //             
  return changed; 
}

void mainMenuHandleButton() {
  MenuItem* ch = menu[0].getCurrentHighlighted();  
  //
  if (ch)
    ch->handleButton();  
  //   
  ch = menu[0].getCurrentHighlighted();
  if (ch != NULL) 
    encoder.setPosition(ch->GetIndex());    
}

void showMonitor() {
  screenShowWidget(&mainWindow);
}

void showTemperaturePlot() {
  togglePlotSensor(&temperature);
  screenShowWidget(&plotWindow);
}

void showHumidityPlot() {
  togglePlotSensor(&humidity);
  screenShowWidget(&plotWindow);
}

void showTimerWindow() {
  screenShowWidget(&timerWindow);
}

void goSleep() {
	goSleepScreen = true;
}

void mainMenuCollapse() {
  while (true) {
    MenuItem* cs = menu[0].getCurrentSelected(); 
    if (!cs) 
      break;
    cs->collapseChildren();
  }     
}

void initMainMenu() {
  mainMenu.fill("MENU", 32, 0, 0, 0, 1);
  mainMenu.draw = mainMenuDraw;
  mainMenu.handleEncoder = mainMenuHandleEncoder;
  mainMenu.handleButton = mainMenuHandleButton;
  //
  menu[0].fill(mainMenu.caption, NULL);
  menu[0].isHighlighted = true;
  //
  menu[1].fill("MONITOR", &menu[0], showMonitor);
  //
  menu[2].fill("SENSORS", &menu[0]);
  menu[3].fill("TEMPERATURE", &menu[2], showTemperaturePlot);
  menu[4].fill("HUMIDITY", &menu[2], showHumidityPlot);
  menu[5].fill("..", &menu[2]);
  //
  menu[6].fill("SETTINGS", &menu[0]);
  menu[7].fill("SLEEP TIMER", &menu[6], showTimerWindow);
  menu[8].fill("SLEEP NOW", &menu[6], goSleep);
  menu[9].fill("..", &menu[6]);
  //
  menu[10].fill("..", &menu[0]);
}

byte MenuItem::GetIndex() {
	byte res = 0;
	byte index = 0;	 
	if (parent != NULL) {
		for (int i = 0; i < MENU_SIZE; i++)
    	if (menu[i].parent == parent) {
				if (&menu[i] == this) {
					res = index;
					break;
				}
				index++;
			}			
	}
	return res;
}

void MenuItem::fill(const char aCaption[], MenuItem* aParent, void(*aAction)() = NULL) {
	strcpy(caption, aCaption);
	parent = aParent;
  action = aAction;
}
//
int MenuItem::getLevel() {
		MenuItem* p = parent;
		int level = 0;
		while (p) {
				level++;
				p = p->parent;
		}
		return level;
}
//
void MenuItem::print() {
		int level = getLevel();
		for (int i = 0; i < level; i++)
				Serial.print("  ");
		Serial.println(caption);
}
//  
int MenuItem::getChildrenSize() {
	int r = 0;
	for (int i = 0; i < MENU_SIZE; i++)
		if (menu[i].parent == this)
				r++; 
	return r;
}
//
int MenuItem::getVisibleChildrenSize() {
  int r = 0;
  for (int i = 0; i < MENU_SIZE; i++)
    if ((menu[i].parent == this) && menu[i].isVisible)
        r++; 
  return r;
}
//
MenuItem* MenuItem::getChild(byte aIndex) {
	MenuItem* r = NULL;
	byte c = 0;
	for (int i = 0; i < MENU_SIZE; i++)
		if (menu[i].parent == this) {
			if (c == aIndex) {
					r = &menu[i];
					break;;
			} 
			c++;
		}
	return r;          
}

MenuItem* MenuItem::getSelectedChild() {
  MenuItem* sc = NULL;  
	for (int i = 0; i < MENU_SIZE; i++)
		if ((menu[i].parent == this) && (menu[i].isSelected)) {
      	sc = &menu[i];
      	break;
    	}		
	return sc;    
}

MenuItem* MenuItem::getHighlightedChild() {
  MenuItem* sc = NULL;  
	for (int i = 0; i < MENU_SIZE; i++)
		if ((menu[i].parent == this) && (menu[i].isHighlighted)) {
      	sc = &menu[i];
      	break;
    	}		
	return sc;    
}

MenuItem* MenuItem::getCurrentSelected() {
	MenuItem* cs = NULL;
  MenuItem* cs2;
  if (isSelected)
    cs = this;
	for (int i = 0; i < MENU_SIZE; i++)
		if (menu[i].parent == this) {
			cs2 = menu[i].getCurrentSelected();
			if (cs2 != NULL) {
      	cs = cs2;
      	break;
    	}
		}  
  return cs;
}

MenuItem* MenuItem::getCurrentHighlighted() {
  MenuItem* cs = NULL;
  MenuItem* cs2;
  if (isHighlighted)
    cs = this;
	for (int i = 0; i < MENU_SIZE; i++)
		if (menu[i].parent == this) {
			cs2 = menu[i].getCurrentHighlighted();
			if (cs2 != NULL) {
      	cs = cs2;
      	break;
    	}
		}  
  return cs;
}

void MenuItem::resetChildrenHighlight() {
	for (int i = 0; i < MENU_SIZE; i++)
		if (menu[i].parent == this) 
			menu[i].isHighlighted = false;
}

void MenuItem::setChildHighlighted(byte aIndex) {
  resetChildrenHighlight();
	MenuItem* mi = getChild(aIndex);
	if (mi)
  	mi->isHighlighted = true;
}

int MenuItem::getChildrenMaxCaptionLength() {
  int ml = 0, cl = 0;
	for (int i = 0; i < MENU_SIZE; i++)
		if (menu[i].parent == this) {
			cl = strlen(menu[i].caption);
			if (cl > ml)
      	ml = cl;
		} 	  
  return ml;
}

void MenuItem::handleButton() {
	if (isHighlighted) {
		if (!strcmp(caption, "..")) {
			MenuItem* p = (MenuItem*)parent;
			if (p) 
				p->collapseChildren();			     
		} else {			
			expandChildren(); 
			//
			if (action != NULL)
        //
				action();
		}
	}  
}

void MenuItem::showLevelUpChild() {
	byte cs = getChildrenSize(); 
  if ((cs > 0) && !strcmp(getChild(cs - 1)->caption, ".."))
    getChild(cs - 1)->isVisible = true;
}

void MenuItem::hideLevelUpChild() {
	byte cs = getChildrenSize(); 
  if ((cs > 0) && !strcmp(getChild(cs - 1)->caption, ".."))
    getChild(cs - 1)->isVisible = false;
}

void MenuItem::expandChildren() {
  resetChildrenHighlight();
	byte cs = getChildrenSize();
  if (cs == 0)
    return;
  if (cs > 0)
  	getChild(0)->isHighlighted = true; 
  //
  if (parent) 
    parent->hideLevelUpChild();
	//	
  showLevelUpChild();
  //
  isSelected = true;  
}

void MenuItem::collapseChildren() {    
  hideLevelUpChild(); 
  if (parent)
    parent->showLevelUpChild();  
  //
  resetChildrenHighlight();
  isSelected = false;
  isHighlighted = true;
}
