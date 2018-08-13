#pragma once
#include <OneWire.h>
#include <DallasTemperature.h>

class temp_component
{
public:
    temp_component(int data_pin) : one_wire(data_pin), sensor(&one_wire)
    {}
    bool setup();
    float loop();
private:
    bool accept_temp_change(float new_temp);

    OneWire one_wire;
    DallasTemperature sensor;
    DeviceAddress address{};
    unsigned int last_temp_request = 0;
    float last_temp = 0;
    bool last_temp_change_was_downwards = false;
    unsigned int conversion_delay = 0;

    /*Data-sheet:
    Resolution:       9       10      11       12
    Corresponding:    0.5°C   0.25°C  0.125°C  0.0625°C
    Conversion delay: 93.75ms 187.5ms 375ms    750ms
    */
    const int resolution = 12;
};
