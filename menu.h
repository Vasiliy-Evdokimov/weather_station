#ifndef MENU_H
#define MENU_H

#include "common.h"
#include "sensor.h"
#include "screen.h"
#include "plot.h"

extern const int MENU_SIZE;

typedef struct MenuItem
{
  char caption[CAPTION_LENGTH] = "";
  MenuItem* parent = NULL;
  //
  bool isHighlighted = false;
  bool isSelected = false;
  bool isVisible = true;
  //
  void (*action)();
  //
  void fill(const char aCaption[], MenuItem* aParent, void(*aAction)() = NULL);
  int getLevel();
  void print();  
  int getChildrenSize();  
  int getVisibleChildrenSize();
  MenuItem* getChild(byte aIndex);
  MenuItem* getSelectedChild();
  MenuItem* getHighlightedChild();
  MenuItem* getCurrentSelected();
  MenuItem* getCurrentHighlighted();  
  void resetChildrenHighlight();
  void setChildHighlighted(byte aIndex);
  int getChildrenMaxCaptionLength();
  void handleButton();
	byte GetIndex();
	void expandChildren();
	void collapseChildren();
	void showLevelUpChild();
	void hideLevelUpChild();
  //
} MenuItem;

void initMainMenu();

void mainMenuCollapse();

void showTemperaturePlot();
void showHumidityPlot();

#endif
