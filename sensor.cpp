#include "sensor.h"

DHT dht(5, DHT11);

Sensor temperature;
Sensor humidity;

void Sensor::addNewValue(float aValue) {
	short newValue = (short)(aValue * 10);
	min_value = newValue;
  max_value = min_value;		
	for (int i = SENSOR_DATA_LENGTH - 1 ; i > 0 ; i--) {
		values[i] = values[i - 1];	
		if (values[i] < min_value)
			min_value = values[i];
		if (values[i] > max_value) 
			max_value = values[i];      
  }  
  values[0] = newValue;
}

short Sensor::getValue(int aIndex) {
  if ((aIndex < 0) || (aIndex >= SENSOR_DATA_LENGTH))
    return 0;
  return values[aIndex];  
}

short Sensor::getCurrentValue() {
  return getValue(0);
}

bool Sensor::hasChanged() {
  return (getValue(0) != getValue(1));
}

String convertValueToStr(short aValue) {
  return (String)(aValue / 10) + "." + (String)(aValue % 10);
}

void initSensors() {
  strcpy(temperature.caption, "TEMPERATURE");
  temperature.color = 0x00FF00;
  strcpy(humidity.caption, "HUMIDITY");
  humidity.color = 0x0000FF;
}

void readSensors() {
    temperature.addNewValue(dht.readTemperature());
    humidity.addNewValue(dht.readHumidity());
}
