
#include <M5Unified.h>

#include "Applications/Applications.h"
#include "State/State.h"

static uint32_t app_notify_value = 0;

enum class eApplicationNotifyValue
{
    HANDLE = 1,
    UPDATE = 2,
    DRAW = 3
};

void vApplicationTask(void *vpState)
{
    sState *pState = (sState *)vpState;

    while (true)
    {
        xTaskNotifyWait(0x00, ULONG_MAX, &app_notify_value, portMAX_DELAY);

        if ((eApplicationNotifyValue)app_notify_value == eApplicationNotifyValue::UPDATE)
        {

            pState->current_app->app_update(pState);
        }

        if ((eApplicationNotifyValue)app_notify_value == eApplicationNotifyValue::HANDLE)
        {
            pState->current_app->app_handle(pState);
        }

        if ((eApplicationNotifyValue)app_notify_value == eApplicationNotifyValue::DRAW)
        {
            pState->current_app->app_draw(pState);
        }

        // == Unblock scheduler
        xTaskNotify(pState->os, 0, eNoAction);
    }
};