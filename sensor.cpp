#include "menu.h"
#include "sensor.h"

DHT dht(5, DHT11);

Sensor sensors[SENSORS_SIZE] = 
  {
    { "TEMPERATURE", "C", 0x00FF00, showTemperaturePlot },
    { "HUMIDITY", "%", 0x0000FF, showHumidityPlot }
  };

Sensor* temperature = &sensors[0];
Sensor* humidity = &sensors[1];

String Sensor::getCaption()
{
  return FCaption;
}

String Sensor::getMeasurement()
{
  return FMeasurement;
}

uint32_t Sensor::getColor()
{
  return FColor;
}

void_function_pointer Sensor::getPlotFunction()
{
  return FPlotFunction;
}

void Sensor::addNewValue(float aValue)
{
	short newValue = (short)(aValue * 10);
  if (newValue < FMinValue)	FMinValue = newValue;
  if (newValue > FMaxValue) FMaxValue = newValue;
  //
	for (int i = SENSOR_DATA_LENGTH - 1 ; i > 0 ; i--)
		FValues[i] = FValues[i - 1];	
	//
  FValues[0] = newValue;
}

short Sensor::getValue(int aIndex)
{
  if ((aIndex < 0) || (aIndex >= SENSOR_DATA_LENGTH))
    return 0;
  return FValues[aIndex];  
}

short Sensor::getMinValue()
{
  return FMinValue;  
}

short Sensor::getMaxValue()
{
  return FMaxValue;
}

short Sensor::getCurrentValue()
{
  return getValue(0);
}

bool Sensor::isChanged()
{
  return (getValue(0) != getValue(1));
}

String Sensor::ValueToString(short aValue)
{
  return (String)(aValue / 10) + "." + (String)(aValue % 10);
}

void readSensors()
{
  temperature->addNewValue(dht.readTemperature());
  humidity->addNewValue(dht.readHumidity());
}
