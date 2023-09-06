#include <freertos/task.h>

struct sState {
  TaskHandle_t os = nullptr;
  TaskHandle_t screen = nullptr;


};