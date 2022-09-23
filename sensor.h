#ifndef SENSOR_H
#define SENSOR_H

#include "common.h"
#include <DHT.h>

#define SENSOR_DATA_LENGTH 30

typedef struct Sensor {      
    char caption[CAPTION_LENGTH] = "";      
    uint32_t color = 0xFFFFFF;
    short values[SENSOR_DATA_LENGTH] = {0};
    short min_value = 0;
    short max_value = 0;
    void addNewValue(float aValue);        
    short getValue(int aIndex);
    short getCurrentValue(); 
    bool hasChanged();   
} Sensor;

String convertValueToStr(short aValue);

void readSensors();

void initSensors();

#endif
