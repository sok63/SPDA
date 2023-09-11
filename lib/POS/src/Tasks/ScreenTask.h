
#include <M5Unified.h>

#include "State/State.h"

// Global defines
#define FULL_REDRAW_AFTER 30

// Global variables
static uint32_t screen_notify_value = 0;
static char sbuf[30];

// Screen draw functions
void screen_draw_topbar(sState *pState);

// Main task function
void vScreenTask(void *vpState)
{
    sState *pState = (sState *)vpState;

    M5.Display.init_without_reset();
    M5.Display.clear();

    while (true)
    {
        // == Take notify
        xTaskNotifyWait(0x00, ULONG_MAX, &screen_notify_value, portMAX_DELAY);

        // == Fast check
        if (!state.need_redraw)
        {
            continue;
        }

        // == Prepare ==
        pState->screen_updates++;
        M5.Display.powerSave(false);
        pState->screen_updates % FULL_REDRAW_AFTER != 0 ? M5.Display.setEpdMode(epd_fastest) : M5.Display.setEpdMode(epd_quality);

        M5.Display.startWrite();

        // == Draw ==
        // Topbar
        screen_draw_topbar(pState);

        // == End routine ==
        M5.Display.endWrite();
        M5.Display.powerSave(true);
        M5.Display.waitDisplay();
        M5.Display.waitDMA();

        // == Return notify
        xTaskNotify(pState->os, 0, eNoAction);
    }
};

void screen_draw_topbar(sState *pState)
{
    M5.Display.setColor(0, 0, 0);
    M5.Display.setTextSize(3, 3);
    M5.Display.drawLine(0, 30, 960, 30);

    // Counter
    sprintf(sbuf, "%d\n", pState->screen_updates);
    M5.Display.drawString(sbuf, 5, 5);

    // Voltage
    sprintf(sbuf, "%d:%d\n", pState->battery_level, pState->battery_mv);
    M5.Display.drawString(sbuf, 540 - 10 - 7 * 20, 5);

    // Time
    auto t = M5.Rtc.getTime();
    sprintf(sbuf, "%02d:%02d:%02d\n", t.hours, t.minutes, t.seconds);
    M5.Display.drawString(sbuf, 540 / 2 - 80, 5);
};