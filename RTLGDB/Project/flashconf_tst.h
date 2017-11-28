#include "FreeRTOS.h"
#include "task.h"
#include "diag.h"
#include "hal_efuse.h"
#include "flash_api.h"
#include "osdep_service.h"
#include "device_lock.h"

#define FLASHCONF_SHOW 1

void flashconf_main(void);