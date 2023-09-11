#pragma once

#include <freertos/task.h>

#include "Applications/Applications.h"

struct sState
{
  // == Global information
  bool need_redraw = true;
  float user_action_time = 0;
  float uptime = 0;

  // == Tasks
  // OS task section
  TaskHandle_t os = nullptr;

  // Battery task section
  TaskHandle_t tBattery = nullptr;
  uint32_t battery_mv = 0;
  uint8_t battery_level = 0;

  // Screen task section
  TaskHandle_t screen = nullptr;
  uint32_t screen_updates = 0;

  // == Applications
  sAppRecord *applications = 0;
  sAppRecord *current_app = 0;
};