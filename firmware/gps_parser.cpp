#include "gps_parser.h"
#include <string.h>

void gps_parser::reset()
{
    skip = 0;
    skip_fields = 0;
    
    rmc_offset = 0;
    rmc_tag_valid = false;

    time_offset = 0;
    time_valid = false;

    date_offset = 0;
    date_valid = false;
}

//format is [...]$G?RMC,time,[8 other fields],date
//time is HHMMSS.000
//date is DDMMYY
bool gps_parser::parse(uint8_t new_byte)
{
    if(skip > 0)
    {
        --skip;
        return false;
    }
    if(skip_fields > 0)
    {
        if(new_byte == ',')
            --skip_fields;
        return false;
    }

    if(!rmc_tag_valid)
    {
        if(new_byte == '$')
            rmc_offset = 0;
        
        rmc[rmc_offset] = new_byte;
        ++rmc_offset;

        if(rmc_offset == 6)
        {
            if (memcmp(rmc, "$G", 2) == 0
                && memcmp(rmc + 3, "RMC", 3) == 0)
            {
                rmc_tag_valid = true;
                skip = 1; //skip ','
            } 
            else
                rmc_offset = 0;
        }
        return false;
    }

    if(time_offset < 6)
    {
        time[time_offset] = new_byte;
        ++time_offset;
        if(time_offset == 6)
        {
            time_valid = true;
            skip = 4; //skip '.000'
            skip_fields = 1 + 7;
        }
        return false;
    }

    if(date_offset < 6)
    {
        date[date_offset] = new_byte;
        ++date_offset;
        if(date_offset == 6)
        {
            date_valid = true;
            return true;
        }
        return false;
    }
    return true;
}

int gps_parser::two_digits_to_int(const uint8_t *digits)
{
    uint8_t tens_char = digits[0];
    uint8_t ones_char = digits[1];
    int tens =  tens_char - 48;
    int ones =  ones_char - 48;
    return tens * 10 + ones;
}

bool gps_parser::time_and_date_valid()
{
    return time_valid && date_valid;
}

int gps_parser::get_hour()
{
    return two_digits_to_int(time);
}

int gps_parser::get_minute()
{
    return two_digits_to_int(time + 2);
}

int gps_parser::get_second()
{
    return two_digits_to_int(time + 4);
}

int gps_parser::get_day()
{
    return two_digits_to_int(date);
}

int gps_parser::get_month()
{
    return two_digits_to_int(date + 2);
}

int gps_parser::get_year()
{
    return two_digits_to_int(date + 4) + 2000;
}
