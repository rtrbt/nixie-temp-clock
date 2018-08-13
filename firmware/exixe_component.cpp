#include "stddef.h"
#include "exixe_component.h"

bool exixe_component::setup()
{
    nixies = (exixe *) malloc(sizeof(exixe) * tube_count);
    for (size_t i = 0; i < tube_count; ++i)
        nixies[i] = exixe(exixe_cs_pins[i]);
// ONLY CALL THIS ONCE
    nixies[0].spi_init();

    for (size_t i = 0; i < tube_count; ++i)
        nixies[i].clear();
    return true;
}

void exixe_component::report_setup_progress(float progress)
{
    int max_dots = tube_count * 2 - 4; //in-19a and in-19v don't have dots
    int dots_to_light = progress * max_dots;
    for (size_t i = 0; i < tube_count; ++i)
    {
        if (i == 6 || i == 9)
            continue;
        if (dots_to_light >= 2)
        {
            nixies[i].set_dots(127, 127);
            dots_to_light -= 2;
            continue;
        }
        if (dots_to_light == 1)
        {
            nixies[i].set_dots(127, 0);
            --dots_to_light;
            continue;
        }
        nixies[i].set_dots(0, 0);
    }
}

void exixe_component::report_setup_error_and_exit()
{
    for (size_t i = 0; i < tube_count; ++i)
        nixies[i].set_led(127, 0, 0);
    while (true)
        delay(1000);
}

void exixe_component::show_two_digits(int num, int tensNixieIdx, int onesNixieIdx)
{
    int tens = num / 10;
    num -= tens * 10;
    int ones = num;
    nixies[tensNixieIdx].show_digit(tens, 127, 0);
    nixies[onesNixieIdx].show_digit(ones, 127, 0);
}

void exixe_component::loop(int hour, int minute, int second, float temperature)
{
    show_two_digits(hour, 0, 1);
    show_two_digits(minute, 2, 3);
    show_two_digits(second, 4, 5);

    bool sign = temperature >= 0;
    if (sign)
        nixies[6].show_symbol(exixe::IN_19V_PLUS, 127, 0);
    else
        nixies[6].show_symbol(exixe::IN_19V_MINUS, 127, 0);

    show_two_digits((int) (round(abs(temperature))), 7, 8);
    nixies[9].show_symbol(exixe::IN_19A_DEGREE_CELSIUS, 127, 0);
}
