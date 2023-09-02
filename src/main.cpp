#include <M5Unified.h>
#include <SL.h>


extern "C" void app_main(void){


  M5.begin();

  /// If you want to output logs to the display, write this.
  M5.setLogDisplayIndex(0);

  /// use wrapping from bottom edge to top edge.
  //M5.Display.setTextWrap(true, true);

  M5.Display.init_without_reset();

  M5.Display.clear();

        M5.Display.drawRect(200,200,200,200);


vTaskDelay(10000);
//M5.Power.deepSleep(10000000,false);
}