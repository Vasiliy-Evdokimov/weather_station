#ifndef SENSOR_H
#define SENSOR_H

#include "common.h"
#include <DHT.h>

#define SENSORS_SIZE 2
#define SENSOR_DATA_LENGTH 30

struct Sensor
{
  private:
    String FCaption = "";
    String FMeasurement = "";
    uint32_t FColor = 0xFFFFFF;
    void_function_pointer FPlotFunction = nullptr;
    //
    short FValues[SENSOR_DATA_LENGTH] = {0};
    short FMinValue = 0;
    short FMaxValue = 0;
  public:  
    String getCaption();
    String getMeasurement();
    uint32_t getColor();
    void_function_pointer getPlotFunction();
    //
    void addNewValue(float aValue);
    short getValue(int aIndex);
    short getCurrentValue();
    short getMinValue();
    short getMaxValue();
    bool isChanged();
    //
    static String ValueToString(short aValue);
    //
    Sensor (String aCaption, String aMeasurement, uint32_t aColor, void_function_pointer aPlotFunction = nullptr) : 
      FCaption(aCaption), FMeasurement(aMeasurement), FColor(aColor), FPlotFunction(aPlotFunction) {}
};

void readSensors();

extern Sensor sensors[];

extern Sensor* temperature; 
extern Sensor* humidity;

#endif
