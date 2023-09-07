
#include <M5Unified.h>

#include "State/State.h"

#define BAT_ARR_SIZE 16
#define BAT_MEASURE_COUNT_POW 5

static uint32_t bat_notify_value = 0;

void vBatteryTask(void *vpState)
{
    sState *pState = (sState *)vpState;
    uint32_t measurements[BAT_ARR_SIZE];
    for (auto idx = 0; idx < BAT_ARR_SIZE; idx++)
        measurements[idx] = 0;

    while (true)
    {
        xTaskNotifyWait(0x00, ULONG_MAX, &bat_notify_value, portMAX_DELAY);

        // Level
        pState->battery_level = M5.Power.getBatteryLevel();
        if (pState->battery_level > 99)
            pState->battery_level -= 1;

        // Avg measure
        uint32_t avg = 0;
        for (auto idx = 0; idx < (int)pow(2, BAT_MEASURE_COUNT_POW); idx++)
            avg += M5.Power.getBatteryVoltage();
        avg = avg >> BAT_MEASURE_COUNT_POW;

        measurements[pState->screen_updates % BAT_ARR_SIZE] = avg;

        // Find min
        uint32_t v = 0;
        for (auto idx = 0; idx < BAT_ARR_SIZE; idx++)
        {
            if (measurements[idx] > v)
                v = measurements[idx];
        }

        pState->battery_mv = v;
    }
}