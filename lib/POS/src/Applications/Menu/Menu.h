
#include <M5Unified.h>

#include <SL.h>

#include "../Applications.h"

static sAppRecord app_menu;

// bool app_menu_handle(sSL *);
// bool app_menu_update(sSL *);
bool app_menu_draw(sState *)
{
    static char sbuf[30];
    sprintf(sbuf, "Menu app\n");
    M5.Display.drawString(sbuf, 60, 60);

    return false;
};
// bool app_menu_background(sSL *);

void set_app_menu()
{
    app_menu.app_draw = app_menu_draw;
};
