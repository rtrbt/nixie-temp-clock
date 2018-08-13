#pragma once

#include <Arduino.h>
#include <exixe.h>

const auto NIXIE_PWR_EN = A5;
const auto EXIXE_RESET = A6;

const auto EXIXE_H1_CS = 5;
const auto EXIXE_H2_CS = 4;
const auto EXIXE_M1_CS = 3;
const auto EXIXE_M2_CS = 2;
const auto EXIXE_S1_CS = 1;
const auto EXIXE_S2_CS = 0;
const auto EXIXE_T1_CS = 14;
const auto EXIXE_T2_CS = 13;
const auto EXIXE_T3_CS = 7;
const auto EXIXE_T4_CS = 6;

const decltype(EXIXE_H1_CS) exixe_cs_pins[10] = {
        EXIXE_H1_CS,
        EXIXE_H2_CS,
        EXIXE_M1_CS,
        EXIXE_M2_CS,
        EXIXE_S1_CS,
        EXIXE_S2_CS,
        EXIXE_T1_CS,
        EXIXE_T2_CS,
        EXIXE_T3_CS,
        EXIXE_T4_CS
};

const auto tube_count = sizeof(exixe_cs_pins) / sizeof(exixe_cs_pins[0]);

class exixe_component
{
public:
    bool setup();

    void report_setup_progress(float progress);

    void report_setup_error_and_exit();

    void loop(int hour, int minute, int second, float temperature);

private:
    void show_two_digits(int num, int tensNixieIdx, int onesNixieIdx);

    float temperature;
    exixe *nixies;
};
