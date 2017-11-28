/*
 * user_start.c
 *
 *  Created on: 26/03/2017
 *      Author: pvvx
 */
#include "user_config.h"
#include "platform_autoconf.h"
#include "autoconf.h"
#include "FreeRTOS.h"
#include "freertos_pmu.h"
#include "task.h"
#include "diag.h"
#include "netbios/netbios.h"
#include "sntp/sntp.h"
#include "sys_cfg.h"
#include "wifi_api.h"
#include "web/web_srv.h"
#include "webfs/webfs.h"

#include "MPU6050.h"
//#include "SSD1306.h"
//#include "WS2812.h"
#include "wifi_api.h"

#include "gpio_api.h"   // mbed
#include "i2c_api.h"
#include "pinmap.h"

#include "lwip/pbuf.h"
#include "lwip/udp.h"
#include "lwip/tcp.h"

#include "flashconf_tst.h"
#include "efuse_tst.h"

#define OFFSET 		0x298							
#define GAIN_DIV	0x34C							
#define AD2MV(ad,offset,gain) (((ad/16)-offset)*1000/gain)	



struct SystemCfg syscfg = {
		.cfg.w = SYS_CFG_DEBUG_ENA
		 | SYS_CFG_PIN_CLR_ENA
#if defined(USE_NETBIOS) && USE_NETBIOS
		 | SYS_CFG_NETBIOS_ENA
#endif
#if defined(USE_SNTP) && USE_SNTP
		 | SYS_CFG_SNTP_ENA
#endif
		 ,
#if defined(USE_WEB)
		.web_port = USE_WEB,
#else
		.web_port = 0,
#endif
		.web_twrec = 5,
		.web_twcls = 5
};

void connect_start(void)
{
	info_printf("\%s: Time at start %d ms.\n", __func__, xTaskGetTickCount());
}

void connect_close(void)
{
	info_printf("\%s: Time at start %d ms.\n", __func__, xTaskGetTickCount());
}

void user_start(void)
{
	info_printf("\%s: Time at start %d ms.\n", __func__, xTaskGetTickCount());

}

void sys_write_cfg(void)
{
	flash_write_cfg(&syscfg, FEEP_ID_SYS_CFG, sizeof(syscfg));
}

extern void console_init(void);

void udp_echoserver_receive_callback(void *arg, struct udp_pcb *upcb, struct pbuf *p, struct ip_addr *addr, uint16_t port);
void user_init2_thrd(void)
{
	gpio_t gpio_usrled, gpio_usrbt, gpio_pwren, gpio_pwrbt;
	MPU6050_DataPack dp[MPU6050_PACKSIZE];
	struct pbuf *pb;
	struct udp_pcb *upcb;
	ip_addr_t servIP;

	for (uint16_t i = 0; i < MPU6050_PACKSIZE; i++)
	{
		dp[i].X = 0;
		//dp[i].Y = 0;
		//dp[i].Z = 0;
	}

	//IP4_ADDR(&servIP, 192, 168, 1, 39);
	//IP4_ADDR(&servIP, 192, 168, 137, 1);
	IP4_ADDR(&servIP, 192, 168, 1, 55);

	gpio_usrled.direction = PIN_OUTPUT;
	gpio_usrled.mode = PullNone;
	gpio_init(&gpio_usrled, PC_2);

	gpio_pwren.direction = PIN_OUTPUT;
	gpio_pwren.mode = PullNone;
	gpio_init(&gpio_pwren, PC_0);

	gpio_pwrbt.direction = PIN_INPUT;
	gpio_pwrbt.mode = PullDown;
	gpio_init(&gpio_pwrbt, PC_1);

	gpio_usrbt.direction = PIN_INPUT;
	gpio_usrbt.mode = PullUp;
	gpio_init(&gpio_usrbt, PC_3);

	gpio_write(&gpio_pwren, 1);
	gpio_write(&gpio_usrled, 1);

	vTaskDelay(1000);

	MPU6050_Init();

	upcb = udp_new();

	for (;;)
	{
		vTaskDelay(50);
		WDGRefresh();
		
		if (gpio_read(&gpio_pwrbt))
		{
			gpio_write(&gpio_usrled, 0);
			gpio_write(&gpio_pwren, 0);
		}

		if (wifi_st_status == WIFI_STA_CONNECTED)
		{
			if(MPU6050_IsDataPacketReady(&dp))
			{ 
				if (upcb)
				{
					//DBG_8195A("Send UDP data...\n");
					udp_connect(upcb, &servIP, 5001);

					pb = pbuf_alloc(PBUF_TRANSPORT, sizeof(MPU6050_DataPack) * MPU6050_PACKSIZE, PBUF_RAM);
					memcpy(pb->payload, &dp, sizeof(MPU6050_DataPack) * MPU6050_PACKSIZE);

					udp_send(upcb, pb);
					udp_disconnect(upcb);
					pbuf_free(pb);
				}
			}
		}
	}
	vTaskDelete(NULL);
}

void user_init_thrd(void) 
{
	//if(syscfg.cfg.b.pin_clear_cfg_enable && 0)
	//{  
		// user_test_clear_pin();
		wifi_cfg.load_flg = 0;
	//}
	//else flash_read_cfg(&syscfg, FEEP_ID_SYS_CFG, sizeof(syscfg));

	if(!syscfg.cfg.b.debug_print_enable) print_off = 1;

	/* Initilaize the console stack */
	console_init();

	/* Web Disk Init */
	WEBFSInit();

	/* Load cfg, init WiFi + LwIP init, WiFi start if wifi_cfg.mode !=  RTW_MODE_NONE */
	wifi_init();

#if defined(USE_NETBIOS)
	if(syscfg.cfg.b.netbios_ena) netbios_init();
#endif
#if defined(USE_SNTP)
	if(syscfg.cfg.b.sntp_ena) sntp_init();
#endif
	// webstuff_init(); // httpd_init();
	webserver_init(syscfg.web_port);

	if(syscfg.cfg.b.powersave_enable) 
	{
		release_wakelock(~WAKELOCK_WLAN);
	}

	//	xTaskCreate(x_init_thrd, "wifi_init", 1024, NULL, tskIDLE_PRIORITY + 1 + PRIORITIE_OFFSET, NULL);

	/* Kill init thread after all init tasks done */
	vTaskDelete(NULL);
}

