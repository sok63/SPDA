#pragma once

struct sDevice
{
    int Power;
    int Screen;
    int Touch;
    int Buttons;
    int Buses;
    int Memory;
};


struct sHelpers {
    int GFX;
    int Geographic;
};



struct sSL
{
    sDevice Device;
    int Helpers;
};