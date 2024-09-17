#include "menu.h"
#include "sensor.h"

DHT dht(5, DHT11);

Sensor sensors[SENSORS_SIZE] = 
  {
    { "TEMPERATURE", "C", 0x00FF00, showTemperaturePlot },
    { "HUMIDITY", "%", 0x0000FF, showHumidityPlot }
  };

Sensor* getTemperatureSensor() { return &sensors[0]; }
Sensor* getHumiditySensor() { return &sensors[1]; }

String Sensor::getCaption() const
{
  return FCaption;
}

String Sensor::getMeasurement() const
{
  return FMeasurement;
}

uint32_t Sensor::getColor() const
{
  return FColor;
}

void_function_pointer Sensor::getPlotFunction() const
{
  return FPlotFunction;
}

void Sensor::addNewValue(float aValue)
{
	short newValue = static_cast<short>(aValue * 10);
  if (newValue < FMinValue)	FMinValue = newValue;
  if (newValue > FMaxValue) FMaxValue = newValue;
  //
  memmove(&FValues[1], &FValues[0], (SENSOR_DATA_LENGTH - 1) * sizeof(*FValues));	
  //
  FValues[0] = newValue;
}

short Sensor::getValue(int aIndex) const
{
  if ((aIndex < 0) || (aIndex >= SENSOR_DATA_LENGTH))
    return 0;
  return FValues[aIndex];  
}

short Sensor::getMinValue() const
{
  return FMinValue;  
}

short Sensor::getMaxValue() const
{
  return FMaxValue;
}

short Sensor::getCurrentValue() const
{
  return getValue(0);
}

bool Sensor::isChanged() const
{
  return (getValue(0) != getValue(1));
}

String Sensor::ValueToString(short aValue)
{
  return
    static_cast<String>(aValue / 10) + "." +
    static_cast<String>(aValue % 10);
}

void readSensors()
{
  getTemperatureSensor()->addNewValue(dht.readTemperature());
  getHumiditySensor()->addNewValue(dht.readHumidity());
}

Sensor* getSensorByIndex(int aIndex)
{
  if ((aIndex < 0) || (aIndex >= SENSORS_SIZE))
    return nullptr;
  return &sensors[aIndex];
}