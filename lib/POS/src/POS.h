
#include <string.h>

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include <esp_sleep.h>

#include <M5Unified.h>

#include <SL.h>

#include "State/State.h"

#include "Tasks/BatteryTask.h"
#include "Tasks/ScreenTask.h"


#include "Applications/Menu/Menu.h"
#include "Applications/Applications.h"

#define CYCLE_MS 50
#define LIGHT_SLEEP_AFTER_S 30
#define LIGHT_SLEEP_S 10
#define POWER_OFF_AFTER_S 600

static sState state;

sAppRecord apps;
sAppRecord current_app;

void vOSShedulerTask(void *pvParams)
{
    // == Init board
    M5.begin();
    M5.Rtc.begin();

    // == Start core tasks
    xTaskCreate(vScreenTask, "scr", 4096, &state, 10, &state.screen);
    xTaskCreate(vBatteryTask, "bat", 4096, &state, 9, &state.tBattery);

    // == Set applications
    // 1. Menu application
    set_app_menu();
    state.applications = &app_menu;

    // == Configure
    uint32_t nv;
    uint32_t interval = LIGHT_SLEEP_AFTER_S * 1000000;

    while (true)
    {
        // If no app, reset to default == menu app
        if (state.current_app == nullptr)
            state.current_app = state.applications; // Menu is first

        // Check how long we have no changes
        state.uptime = (float)clock() / (float)CLK_TCK;

        // Handle

        // Update
        xTaskNotify(state.tBattery, 0, eNoAction);
        

        xTaskNotifyWait(0x00, ULONG_MAX, &nv, portMAX_DELAY);

        // Draw
        xTaskNotify(state.screen, 0, eNoAction);
        xTaskNotifyWait(0x00, ULONG_MAX, &nv, portMAX_DELAY);

        // Power off

        // Wait
        if ((state.uptime - state.user_action_time) > LIGHT_SLEEP_AFTER_S)
        {
            esp_sleep_enable_timer_wakeup(interval);
            esp_light_sleep_start();
        }
        else
        {
            vTaskDelay(CYCLE_MS / portTICK_RATE_MS);
        }
    }

    vTaskDelete(NULL);
}
