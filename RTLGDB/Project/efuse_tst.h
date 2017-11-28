#include "FreeRTOS.h"
#include "task.h"
#include "diag.h"
#include "hal_efuse.h"
#include "efuse_api.h"
#include "osdep_service.h"
#include "device_lock.h"

#define EFUSE_SHOW 1

// https://esp8266.ru/forum/threads/calibration-data.1631/

void efuse_main(void);