#pragma once

#include <Time.h>
#include <Timezone.h>

namespace timezones
{
//Central European Time (Berlin)
TimeChangeRule CET  = {"CET" , Last, Sun, Oct, 3, 1 * 60};
TimeChangeRule CEST = {"CEST", Last, Sun, Mar, 2, 2 * 60};
Timezone CE(CEST, CET);
}
