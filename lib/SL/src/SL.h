#pragma once

struct sDeviceLayer
{
    int Helpers;
    int Managers;
};

struct sAppLayer
{
    int Pin;
    int Screen;
    int Touch;
    int Buttons;

    int Drawer;
    int Geographic;

};

struct sOSLayer
{
    int Memory;
    int Buses;
};


struct sSL
{
    sDeviceLayer DevL;
    sOSLayer OSL;
    sAppLayer AppL;
};