#include "flashconf_tst.h"

#define FCONF_MAX_LEN 0x2000
#define FCONF_ADDR_SHIFT 0x9000

static void fconf_read_task(void *param)
{
	uint32_t i;
	uint8_t buf[FCONF_MAX_LEN];
	flash_t flash;

	DBG_8195A("\nFlash conf block: Test Start\n");
	_memset(buf, 0xFF, FCONF_MAX_LEN);
	device_mutex_lock(RT_DEV_LOCK_FLASH);
	flash_stream_read(&flash, FCONF_ADDR_SHIFT, FCONF_MAX_LEN, &buf);
	device_mutex_unlock(RT_DEV_LOCK_FLASH);
	for(i=0; i < FCONF_MAX_LEN; i+=8)
	{
		DBG_8195A("[%x]\t%02X %02X %02X %02X  %02X %02X %02X %02X\n", i + FCONF_ADDR_SHIFT, buf[i], buf[i+1], buf[i+2], buf[i+3], buf[i+4], buf[i+5], buf[i+6], buf[i+7]);
	}
	
	DBG_8195A("Flash conf block: Test Done\n");
	vTaskDelete(NULL);
}

void flashconf_main(void)
{
#if FLASHCONF_SHOW > 0
	
    ConfigDebugErr = -1; // ~_DBG_GDMA_;
    ConfigDebugInfo = -1; // ~_DBG_GDMA_;
    ConfigDebugWarn = -1; // ~_DBG_GDMA_;

	if(xTaskCreate(fconf_read_task, ((const char*)"fconf_read_task"), 512, NULL, tskIDLE_PRIORITY + 1, NULL) != pdPASS)
		printf("\n\r%s xTaskCreate(fconf_read_task) failed", __FUNCTION__);

	// Enable Schedule, Start Kernel
	if(rtw_get_scheduler_state() == OS_SCHEDULER_NOT_STARTED)
		vTaskStartScheduler();
	else
		vTaskDelete(NULL);
	
	//for (;;) {}
		
#endif
}
