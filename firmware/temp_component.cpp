#include "temp_component.h"
#include "Arduino.h"

bool temp_component::setup()
{
    sensor.begin();
    sensor.getAddress(address, 0);
    sensor.setResolution(address, resolution);
    sensor.setWaitForConversion(false);
    conversion_delay = 750u / (1u << (12u - resolution));

    sensor.requestTemperatures();
    last_temp_request = millis();
    return true;
}

//Used to prevent cases where the displayed temperature is flickering between two values.
//For example when the sensor alternates between 25,55°C and 25,45°C.
//The loss in measurement resolution can be accepted, as only whole degrees are displayed
bool temp_component::accept_temp_change(float new_temp)
{
  bool cur_change_is_downwards = last_temp > new_temp;
  if(last_temp_change_was_downwards == cur_change_is_downwards)
    return true;

  //Accept only if the change is at least two times the measurement resolution.
  return abs(last_temp - new_temp) >= 2 * (0.0625f * (1 << (12 - resolution)));
}

float temp_component::loop()
{
    if(millis() - last_temp_request >= conversion_delay)
    {
        auto temperature = sensor.getTempCByIndex(0);
        sensor.requestTemperatures();
        last_temp_request = millis();
        
        if(!accept_temp_change(temperature))
          return last_temp;
          
        last_temp_change_was_downwards = last_temp > temperature;
        last_temp = temperature;
        return temperature;
    }
    return last_temp;
}
