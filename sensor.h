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
    String getCaption() const;
    String getMeasurement() const;
    uint32_t getColor() const;
    void_function_pointer getPlotFunction() const;
    //
    void addNewValue(float aValue);
    short getValue(int aIndex) const;
    short getCurrentValue() const;
    short getMinValue() const;
    short getMaxValue() const;
    bool isChanged() const;
    //
    static String ValueToString(short aValue);
    //
    Sensor (String aCaption, String aMeasurement, uint32_t aColor, void_function_pointer aPlotFunction = nullptr) : 
      FCaption(aCaption), FMeasurement(aMeasurement), FColor(aColor), FPlotFunction(aPlotFunction) {}
};

void readSensors();

Sensor* getSensorByIndex(int aIndex);
Sensor* getTemperatureSensor();
Sensor* getHumiditySensor();

#endif
