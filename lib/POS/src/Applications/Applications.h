
#include <SL.h>

struct sApplicationRecord
{
    void (*app_init_function)(sSL *) =nullptr;
    void (*app_work_function)(sSL *) =nullptr;
    void (*app_end_function)(sSL *) =nullptr;
    void (*app_background_task)(sSL *)=nullptr;

    sApplicationRecord *prev = nullptr;
    sApplicationRecord *next = nullptr;
};



