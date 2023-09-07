
#include <M5Unified.h>

#include "State/State.h"

static uint32_t screen_notify_value = 0;
static char sbuf[30];

void vScreenTask(void *vpState)
{
    sState *pState = (sState *)vpState;

    M5.Display.init_without_reset();
    M5.Display.setEpdMode(epd_quality);
    M5.Display.clear();

    while (true)
    {

        xTaskNotifyWait(0x00, ULONG_MAX, &screen_notify_value, portMAX_DELAY);
        printf("Try to screen update..\n");

        M5.Display.powerSave(false);

        if (pState->screen_updates % 8 == 0)
        {
            M5.Display.setEpdMode(epd_quality);
        }
        else
        {
            M5.Display.setEpdMode(epd_fastest);
        }

        M5.Display.startWrite();

        // Draw topbar
        // // topbar line
        M5.Display.setColor(0, 0, 0);
        M5.Display.setTextSize(3, 3);
        M5.Display.drawLine(0, 30, 960, 30);

        // // Counter
        pState->screen_updates++;
        sprintf(sbuf, "%d\n", pState->screen_updates);
        M5.Display.drawString(sbuf, 5, 5);

        // Voltage
        sprintf(sbuf, "%d:%d\n", pState->battery_level, pState->battery_mv);
        M5.Display.drawString(sbuf, 540 - 10 - 7 * 20, 5);

        // Time
        auto t = M5.Rtc.getTime();
        sprintf(sbuf, "%02d:%02d:%02d\n", t.hours, t.minutes, t.seconds);
        M5.Display.drawString(sbuf, 540 / 2 - 80, 5);

        // for(auto idx=0;idx<12;idx++){
        //     auto x = idx % 3;
        //     auto y = idx / 3;
        // M5.Display.fillSmoothRoundRect((x+1)*60, (y+1)*60, 50, 50, 5);
        // }

        M5.Display.endWrite();
        M5.Display.powerSave(true);
        M5.Display.waitDisplay();
        M5.Display.waitDMA();
        xTaskNotify(pState->os, 0, eNoAction);
    }
};