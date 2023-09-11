
#pragma once

struct sState;

struct sAppRecord
{
    bool (*app_handle)(sState *) = nullptr;
    bool (*app_update)(sState *) = nullptr;
    bool (*app_draw)(sState *) = nullptr;
    bool (*app_background_task)(sState *) = nullptr;

    sAppRecord *prev = nullptr;
    sAppRecord *next = nullptr;
};
