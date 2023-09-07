
#include <M5Unified.h>
#include <utility/RTC8563_Class.hpp>
#include <SL.h>
#include <string.h>

#include "freertos/FreeRTOS.h"
#include <freertos/task.h>

#include <esp_sleep.h>

#include "State/State.h"

#include "Tasks/BatteryTask.h"
#include "Tasks/ScreenTask.h"

#include "Applications/Applications.h"

static sState state;

void vOSShedulerTask(void *pvParams)
{
    M5.begin();
    M5.Rtc.begin();

    xTaskCreate(vScreenTask, "screen", 4096, &state, 10, &state.screen);
    xTaskCreate(vBatteryTask, "bat", 4096, &state, 9, &state.tBattery);

    vTaskDelay(1000 / portTICK_RATE_MS);

    printf("Try to wakeup screen..\n");

    xTaskNotify(state.tBattery, 0, eNoAction);
    xTaskNotify(state.screen, 0, eNoAction);

    vTaskDelay(1000 / portTICK_RATE_MS);

    M5.Rtc.clearIRQ();

    m5::rtc_time_t t;
    t.seconds = 5;

    // M5.Rtc.setAlarmIRQ(t);
    printf("%d\n", M5.Rtc.getIRQstatus() ? 1 : 0);

    vTaskDelay(1000 / portTICK_RATE_MS);
    //  M5.Power.deepSleep(5000000, false);

    // gpio_set_direction(GPIO_NUM_2, GPIO_MODE_OUTPUT);
    // gpio_set_level(GPIO_NUM_2, 0);

    // gpio_set_direction(GPIO_NUM_5, GPIO_MODE_OUTPUT);
    // gpio_set_level(GPIO_NUM_5, 0);

    // gpio_set_direction(GPIO_NUM_23, GPIO_MODE_OUTPUT);
    // gpio_set_level(GPIO_NUM_23, 0);

    // esp_light_sleep_start();
    uint32_t nv;
    uint32_t interval = 5000000;
    struct timeval tv_now;
    while (true)
    {

        // M5.Power.lightSleep(5000000, false);
        esp_sleep_enable_timer_wakeup(interval);
        esp_light_sleep_start();

        // Begin
        gettimeofday(&tv_now, NULL);
        int64_t time_us_b = (int64_t)tv_now.tv_sec * 1000000L + (int64_t)tv_now.tv_usec;

        xTaskNotify(state.tBattery, 0, eNoAction);
        xTaskNotify(state.screen, 0, eNoAction);

        xTaskNotifyWait(0x00, ULONG_MAX, &nv, portMAX_DELAY);

        gettimeofday(&tv_now, NULL);
        int64_t time_us_a = (int64_t)tv_now.tv_sec * 1000000L + (int64_t)tv_now.tv_usec;

        interval = 5000000 - (time_us_a - time_us_b);

        if (interval > 5000000)
        {
            interval = 5000000;
        }
        else if (interval < 1000000)
        {
            interval = 1000000;
        }

        // vTaskDelay(5000 / portTICK_RATE_MS);
    }

    vTaskDelete(NULL);
}
