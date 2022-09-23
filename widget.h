#ifndef WIDGET_H
#define WIDGET_H

#include "common.h"

struct Widget {
    char caption[CAPTION_LENGTH] = "";
    //
    byte x;
    byte y;
    byte width;
    byte height;
    byte textSize;
    //    
    void fill(const char aCaption[], byte aX, byte aY, byte aWidth, byte aHeight, byte aTextSize);
		//
    bool redraw = false;
    //
		bool isHighlighted = false;
    bool isSelected = false;
    bool isVisible = true;  
    //
    void (*draw)() = NULL;
    bool (*handleEncoder)() = NULL;
    void (*handleButton)() = NULL;
};

void drawWindowBorder(Widget* aWidget);

void getWidgetColorByState(Widget* aWidget, byte* r, byte* g, byte* b);

#endif
