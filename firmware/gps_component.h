#pragma once

#include <Timezone.h>
#include <SparkFun_I2C_GPS_Arduino_Library.h>
#include "gps_parser.h"

class gps_clock_component
{
public:
    bool setup();
    time_t loop(Timezone *selected_timezone);
private:
    I2CGPS wire;
    time_t last_date_time;
    gps_parser parser;
};
