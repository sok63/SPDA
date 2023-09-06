
#include <M5Unified.h>
#include <utility/RTC8563_Class.hpp>
#include <SL.h>
#include <string.h>

#include "freertos/FreeRTOS.h"
#include <freertos/task.h>

#include <esp_sleep.h>

#include "State/State.h"
#include "Applications/Applications.h"

static sState state;

static uint32_t ulNotifiedValue = 0;
static uint32_t draw_counter = 0;
static char sbuf[30];

void vScreenTask(void *pvParam)
{

    M5.Display.init_without_reset();
    M5.Display.clear();
    // m5::rtc_time_t st = {21,26,30};
    // M5.Rtc.setTime(st);

    while (true)
    {

        xTaskNotifyWait(0x00, ULONG_MAX, &ulNotifiedValue, portMAX_DELAY);

        M5.Display.powerSave(false);

        // M5.Display.clear();
        printf("Screen wakeuped\n");
        // M5.Display.powerSave(false);

        M5.Display.startWrite();
        // Draw topbar
        // // topbar line
        M5.Display.setColor(0, 0, 0);
        M5.Display.setTextSize(3, 3);
        M5.Display.drawLine(0, 30, 960, 30);

        // // Counter
        draw_counter++;
        sprintf(sbuf, "%d\n", draw_counter);
        M5.Display.drawString(sbuf, 5, 5);

        // Voltage
        uint32_t avg_v = 0;
        for (auto idx = 0; idx < 32; idx++)
        {
            avg_v += M5.Power.getBatteryVoltage();
        }
        avg_v = avg_v >> 5;

        sprintf(sbuf, "%d:%d\n", M5.Power.getBatteryLevel(), avg_v);
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
        xTaskNotify(state.os, 0, eNoAction);
    }
};

void vOSShedulerTask(void *pvParams)
{
    M5.begin();
    M5.Rtc.begin();

    xTaskCreate(vScreenTask, "screen", 4096, &state, 10, &state.screen);
    vTaskDelay(1000 / portTICK_RATE_MS);

    printf("Try to wakeup screen..\n");

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
    while (true)
    {

        // M5.Power.lightSleep(5000000, false);
        esp_sleep_enable_timer_wakeup(5000000);
        esp_light_sleep_start();
        xTaskNotify(state.screen, 0, eNoAction);
        xTaskNotifyWait(0x00, ULONG_MAX, &nv, portMAX_DELAY);
        // vTaskDelay(5000 / portTICK_RATE_MS);
    }

    vTaskDelete(NULL);
}
