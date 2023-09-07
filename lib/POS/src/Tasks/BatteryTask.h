
#include <M5Unified.h>

#include "State/State.h"

#define BAT_AVG 16

static uint32_t bat_notify_value = 0;

void vBatteryTask(void *vpState)
{
    sState *pState = (sState *)vpState;
    uint32_t measurements[BAT_AVG];
    for (auto idx = 0; idx < BAT_AVG; idx++)
        measurements[idx] = 4400;

    while (true)
    {
        xTaskNotifyWait(0x00, ULONG_MAX, &bat_notify_value, portMAX_DELAY);

        // Level
        pState->battery_level = M5.Power.getBatteryLevel();
        if (pState->battery_level > 99)
            pState->battery_level -= 1;

        // Avg measure
        uint32_t avg = 0;
        for (auto idx = 0; idx < 32; idx++)
            avg += M5.Power.getBatteryVoltage();

        avg = avg >> 5;

        measurements[pState->screen_updates % BAT_AVG] = avg;

        // Find min
        uint32_t min_v = 4400;
        for (auto idx = 0; idx < BAT_AVG; idx++)
        {
            if (min_v > measurements[idx])

                min_v = measurements[idx];
        }

        pState->battery_mv = min_v;
    }
}