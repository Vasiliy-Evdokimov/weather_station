#ifndef SCREEN_H
#define SCREEN_H

#include "common.h"
#include "menu.h"
#include "widget.h"
#include "timer.h"
#include "sensor.h"
#include "plot.h"
#include "monitor.h"
#include "smallHT.h"

void screenInit();

void screenShowWidget(Widget* aWidget);

void screenDrawSleep(); 

void drawHTWindows();

void screenWakeUp();

#endif
