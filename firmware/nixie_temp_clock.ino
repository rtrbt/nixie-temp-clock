#include <climits>

#include <Time.h>
#include <Timezone.h>
#include "timezones.h"
#include "exixe_component.h"
#include "temp_component.h"
#include "gps_component.h"

Timezone *selected_timezone = &timezones::CE;

//#define USE_SERIAL

const auto TEMP_DATA_PIN = A4;

exixe_component nixies;
temp_component temp_sensor{TEMP_DATA_PIN};
gps_clock_component gps_clock;

void setup()
{
#ifdef USE_SERIAL
    Serial.begin(9600);
    Serial.println("Starting setup");
#endif
    if(!nixies.setup())
    {
#ifdef USE_SERIAL
            Serial.println("Could not initialize exixe boards!");
#endif
            while(true)
                    delay(1000);
    }
    nixies.report_setup_progress(1/3);
    
    if(!temp_sensor.setup())
    {
#ifdef USE_SERIAL
            Serial.println("Could not initalize temperature sensor!");
#endif
            nixies.report_setup_error_and_exit();
    }
    nixies.report_setup_progress(2/3);
    
    if(!gps_clock.setup())
    {
#ifdef USE_SERIAL
            Serial.println("Could not initialize GPS!");
#endif
            nixies.report_setup_error_and_exit();
    }
    nixies.report_setup_progress(1);
    nixies.report_setup_progress(0); //disable dots
}


unsigned long sub_millis(unsigned long start, unsigned long end)
{
    if(end >= start)
        return end - start;

    //handle overflow
    auto start_to_overflow = ULONG_MAX - start;
    return end + start_to_overflow;
}


void loop()
{ 
    static bool timeWasSet = false;
    auto start = millis();
    
    auto temp = temp_sensor.loop();
    auto t = gps_clock.loop(selected_timezone);
    if(t != 0)
    {
      setTime(t);
      timeWasSet = true;
    }

    if(timeWasSet)
      nixies.loop(hour(), minute(), second(), temp);
    else
      nixies.loop(0,0,0, temp);
    
    auto runtime = sub_millis(start, millis());
    if(runtime < 33)
        delay(33 - runtime);
#ifdef USE_SERIAL 
    else
    {
        Serial.print("loop runtime was longer than 33ms: ");
        Serial.print(runtime);
        Serial.println("ms");
    }
#endif    
}

