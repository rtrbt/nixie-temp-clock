#include "gps_component.h"
#include <Time.h>

bool gps_clock_component::setup()
{
    return wire.begin(Wire, I2C_SPEED_FAST);
}

time_t gps_clock_component::loop(Timezone *selected_timezone)
{
    //only sync if this is the first loop call or 60 seconds after the last sync
    if(last_date_time != 0
        && now() - last_date_time < 60)
    {
        while(wire.available())
            wire.read(); //trash available bytes
        return 0;
    }
    
    parser.reset();
    
    while(wire.available())
    {
        auto read_byte = wire.read();
        if(parser.parse(read_byte))
            break;      
    } 
    
    if(parser.time_and_date_valid())
    {        
        TimeElements te;
        te.Second = parser.get_second();
        te.Minute = parser.get_minute();
        te.Hour   = parser.get_hour();
        te.Day    = parser.get_day();
        te.Month  = parser.get_month();
        te.Year   = parser.get_year() - 1970;
        if(te.Day > 31 || te.Month > 12) //GPS module reports wrong dates if date fix is lost.
          return 0;
        
        auto t = makeTime(te);
        last_date_time = selected_timezone->toLocal(t);
        return selected_timezone->toLocal(t);
    }
    return 0;
}
