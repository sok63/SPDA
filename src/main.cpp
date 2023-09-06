

#include <POS.h>

extern "C" void app_main(void)
{

   xTaskCreate(vOSShedulerTask, "os", 4096, NULL, 10, &state.os);

  while (true)
    vTaskDelay(10000);
  // M5.Power.deepSleep(10000000,false);
}