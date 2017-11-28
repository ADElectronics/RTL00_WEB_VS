#ifndef _WS2812_H_
#define _WS2812_H_

#include "FreeRTOS.h"
#include "task.h"
#include "event_groups.h"
#include <spi_api.h>
#include <spi_ex_api.h>
#include <autoconf.h>
#include <platform_stdlib.h>
#include "semphr.h"
#include "event_groups.h"


#define WS2812_LEDS_MAX         500
#define WS2812_RESET_LEN        (50 / 2)
#define WS2812_DMABUF_LEN(x)    ((x) * 3 * 4 + WS2812_RESET_LEN)

#define WS2812_SPI_FREQ       3200000 // и делённое на 4 (4 бита SPI за 1 бит WS8212) = в итоге 800кГц
#define WS2812_SPI_MOSI       PC_2 // SPI0
#define WS2812_SPI_MISO       NC//PC_3
#define WS2812_SPI_SCLK       NC//PC_1
#define WS2812_SPI_CS         NC//PC_0

// Events to signal completion of DMA transfer
#define BIT_START       (1 << 0)
#define BIT_DONE        (1 << 1)

// посылается 2 бита протокола WS2812 за один байт SPI, один бит WS2812 за 4 бита SPI.
#define WS2812_BITS_00              0x88
#define WS2812_BITS_01              0x8e
#define WS2812_BITS_10              0xe8
#define WS2812_BITS_11              0xee

typedef struct 
{
    uint8_t     R;
    uint8_t     G;
    uint8_t     B;
} WS2812_RGB_t;

typedef struct 
{
    uint8_t     HUE;
    uint8_t     Sat;
    uint8_t     Value;
} WS2812_HSV_t;

typedef struct 
{
    spi_t               spi_master;
    EventGroupHandle_t *events;
    SemaphoreHandle_t  *mutex;
    uint8_t   *dma_buff;
	size_t     buff_len;
    uint16_t            strip_len;
} WS2812_t;

WS2812_t *WS2812_Init(uint16_t len);
int32_t WS2812_SetLen(WS2812_t *cfg, uint16_t len);
int32_t WS2812_DeInit(WS2812_t *cfg);
int32_t WS2812_Update(WS2812_t *cfg, WS2812_HSV_t hsv[], uint32_t len, uint16_t delay);

#define min(a,b)            ((a) < (b) ? (a) : (b))
#define max(a,b)            ((a) > (b) ? (a) : (b))
#define scale_up(x)         ((x) << 8)
#define scale_down(x)       ((x) >> 8)

#endif // _WS2812_H_
