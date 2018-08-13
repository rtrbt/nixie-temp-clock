#pragma once

#include <stdint.h>

class gps_parser
{
public:
    void reset();
    bool parse(uint8_t new_byte);
    
    bool time_and_date_valid();
    
    int get_hour();
    int get_minute();
    int get_second();
    
    int get_day();
    int get_month();
    int get_year();
    
private:
    int two_digits_to_int(const uint8_t *digits);
    int skip;
    int skip_fields;

    uint8_t rmc[6];    
    uint8_t rmc_offset;
    bool rmc_tag_valid;

    uint8_t time[6];
    uint8_t time_offset;
    bool time_valid;

    uint8_t date[6];
    uint8_t date_offset;
    bool date_valid;
};
