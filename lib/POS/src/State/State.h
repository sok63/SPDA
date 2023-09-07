#pragma once

#include <freertos/task.h>

struct sState {
  // OS task section
  TaskHandle_t os = nullptr;

  // Battery task section
  TaskHandle_t tBattery = nullptr;
  uint32_t battery_mv = 0;
  uint8_t battery_level =0;

  // Screen task section
  TaskHandle_t screen = nullptr;
  uint32_t screen_updates = 0;

};