#include "widget.h"

extern TFT TFTscreen;

bool flRedraw = false;

void Widget::fill(const char aCaption[], 
    byte aX, byte aY, byte aWidth, byte aHeight, byte aTextSize) {
    //
    strcpy(caption, aCaption);
    x = aX;
    y = aY;
    width = aWidth;
    height = aHeight;
    textSize = aTextSize;
}

void drawWindowBorder(Widget* aWidget) {
  byte r, g, b;
  getWidgetColorByState(aWidget, &r, &g, &b);
  TFTscreen.stroke(r, g, b);
  TFTscreen.rect(aWidget->x, aWidget->y, aWidget->width, aWidget->height);   
}

void getWidgetColorByState(Widget* aWidget, byte* r, byte* g, byte* b) {
   uint32_t clr;
  if (aWidget->isHighlighted) {
    if (aWidget->isSelected)
      clr = CLR_GREEN_H;      
    else 
      clr = CLR_YELLOW_H;   
  } else  
      clr = CLR_DARK_GREY_H;   
  //
  colorToRGB(clr, r, g, b);  
}
