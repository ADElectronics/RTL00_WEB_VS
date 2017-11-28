#include "WS2812.h"

/* Quick and dirty, we use one big DMA buffer for the whole strip length.
 * TODO: use smaller DMA buffer and fill in bit patterns on the fly */
uint8_t dma_buffer[WS2812_DMABUF_LEN(WS2812_LEDS_MAX)];

/* scale uint8 value from range 2-255 to range 0-scale */
static inline uint8_t WS2812_Scale(uint8_t value, uint8_t scale)
{
	uint32_t tmp;

	tmp = value * scale;
	tmp /= 256;

	return (uint8_t)tmp;
}

// wake up waiting tasks when DMA transfer is complete 
static void master_tr_done_callback(void *pdata, SpiIrq event)
{
	BaseType_t task_woken, result;
	WS2812_t *cfg;

	task_woken = pdFALSE;
	cfg = (WS2812_t *) pdata;

	switch (event) 
	{
		case SpiRxIrq:
			break;
			
		case SpiTxIrq:
			result = xEventGroupSetBitsFromISR(cfg->events, BIT_DONE, &task_woken);
			if(result == pdPASS)
			{
				portYIELD_FROM_ISR(task_woken);
			}
			break;
			
		default:
			DBG_8195A("WS2812: Unknown SPI irq event!\n");
	}
}

static void WS2812_HSV2RGB(WS2812_HSV_t *hsv, WS2812_RGB_t *rgb)
{
	//uint8_t r, g, b;
	uint8_t hue, sat, val;
	uint8_t base, sector, offset;
	uint8_t rise, fall;

	// scale hue to range 0- 3*64. Makes subsequent calculations easier
	hue = WS2812_Scale(hsv->HUE, 192);
	sat = hsv->Sat;
	val = hsv->Value;

	sector = hue / 64;
	offset = hue % 64;

	// get common white base level and remaining colour amplitude
	base = 255 - sat;

	rise = (offset * sat * 4) / 256;
	fall = 255 - base - rise;

	rise = (rise * val) / 256;
	fall = (fall * val) / 256;
	base = (base * val) / 256;

	rgb->R = base;
	rgb->G = base;
	rgb->B = base;

	switch (sector)
	{
	case 0:
		rgb->R += fall;
		rgb->G += rise;
		break;
	case 1:
		rgb->G += fall;
		rgb->B += rise;
		break;
	case 2:
		rgb->R += rise;
		rgb->B += fall;
		break;
	}
}

// convert a RGB byte into SPI data stream with 2 bits per byte
static uint8_t *WS2812_RGB2PWM(uint8_t *dst, const uint8_t colour)
{
	uint32_t cnt, data = colour;

	for (cnt = 0; cnt < 4; ++cnt)
	{
		switch (data & 0xC0)
		{
		case 0x00:
			*dst = WS2812_BITS_00;
			break;
		case 0x40:
			*dst = WS2812_BITS_01;
			break;
		case 0x80:
			*dst = WS2812_BITS_10;
			break;
		case 0xC0:
			*dst = WS2812_BITS_11;
			break;
		}
		dst++;
		data <<= 2;
	}
	return dst;
}

static int32_t WS2812_TX(WS2812_t *cfg, uint16_t delay)
{
    int32_t result;
    EventBits_t rcvd_events;
    TickType_t timeout;
    BaseType_t status;

    // wait for any SPI transfer to finish
    while(cfg->spi_master.state & SPI_STATE_TX_BUSY)
	{
        vTaskDelay(0);
    }

    result = 0;
    
    // obey requested delay
    if(delay > 0)
	{
        vTaskDelay(delay);
    }

    // lock the DMA buffer mutex while it is transferred
    status = xSemaphoreTake(cfg->mutex, configTICK_RATE_HZ);
    if(status != pdTRUE)
	{
        printf("WS2812: [%s] Timeout waiting for config mutex.\n", __func__);
        result = -1;
        goto err_out;
    }
    
    if(cfg->dma_buff == NULL || cfg->buff_len == 0)
	{
        printf("WS2812: [%s] DMA buffer invalid\n", __func__);
        result = -1;
        goto err_out;
    }

    xEventGroupClearBits(cfg->events, BIT_DONE);
    spi_master_write_stream_dma(&cfg->spi_master, (char *)(cfg->dma_buff[0]), cfg->buff_len);

    timeout = 1000 / portTICK_PERIOD_MS;
    rcvd_events = xEventGroupWaitBits(
                        cfg->events, 
                        BIT_DONE,        // wait for DMA TX done
                        pdTRUE,          // clear event bit
                        pdFALSE,         // do not wait for all bits to be set
                        timeout );

    if(!(rcvd_events & BIT_DONE))
	{
        printf("WS2812: [%s] DMA timeout\n", __func__);
        result = -1;
        goto err_out;
    }

err_out:
    xSemaphoreGive(cfg->mutex);
 
    return result;
}

int32_t WS2812_Update(WS2812_t *cfg, WS2812_HSV_t hsv_values[], uint32_t strip_len, uint16_t delay)
{
    uint32_t i;
    uint8_t *bufp;
    uint16_t len;
    BaseType_t status;
    int32_t result;
	WS2812_RGB_t rgb;

    // make sure DMA buffer is not in use
    while(cfg->spi_master.state & SPI_STATE_TX_BUSY)
	{
        vTaskDelay(0);
    }

    // lock the DMA buffer mutex while we fill it
    status = xSemaphoreTake(cfg->mutex, configTICK_RATE_HZ);
    if(status != pdTRUE)
	{
        printf("WS2812: [%s] Timeout waiting for config mutex.\n", __func__);
        result = -1;
        goto err_out;
    }

    bufp = &(cfg->dma_buff[0]);
    
    // make sure that we do not exceed the buffer
    len = min(strip_len, cfg->strip_len);

    // copy pixel data into DMA buffer
    for(i = 0; i < len; ++i) // ++i ??
	{
        WS2812_HSV2RGB(&hsv_values[i], &rgb);
        bufp = WS2812_RGB2PWM(bufp, rgb.G);
        bufp = WS2812_RGB2PWM(bufp, rgb.R);
        bufp = WS2812_RGB2PWM(bufp, rgb.B);
    }
    
    /* turn unused pixels at end of strip off */ 
    if(cfg->strip_len > len)
	{
        memset(bufp, WS2812_BITS_00, cfg->strip_len - len);
        bufp += cfg->strip_len - len;
    }

    /* add reset pulse */
    memset(bufp, 0x0, WS2812_RESET_LEN); 

    /* release buffer mutex */
    xSemaphoreGive(cfg->mutex);
 
    /* send it off to the strip */   
    result = WS2812_TX(cfg, delay);

err_out:    
    return result;
}

WS2812_t *WS2812_Init(uint16_t strip_len)
{
    int32_t result;
    WS2812_t *cfg;

    result = 0;

    cfg = malloc(sizeof(*cfg));
    if(cfg == NULL)
	{
        printf("WS2812: [%s] malloc for cfg failed\n", __func__);
        result = -1;
        goto err_out;
    }

    memset(cfg, 0x0, sizeof(*cfg));

    cfg->mutex = xSemaphoreCreateMutex();
    if(cfg->mutex == NULL)
	{
        printf("WS2812: [%s] Mutex creation failed\n", __func__);
        result = -1;
        goto err_out;
    }
    
    cfg->events = xEventGroupCreate();
    if(cfg->events == NULL)
	{
        printf("WS2812: [%s] Creating event group failed\n", __func__);
        result = -1;
        goto err_out;
    }

    spi_init(&(cfg->spi_master), WS2812_SPI_MOSI, WS2812_SPI_MISO, WS2812_SPI_SCLK, WS2812_SPI_CS);
    spi_format(&(cfg->spi_master), 8, 3, 0);
    spi_frequency(&(cfg->spi_master), WS2812_SPI_FREQ);
    spi_irq_hook(&(cfg->spi_master), master_tr_done_callback, (uint32_t)cfg);

    result = WS2812_SetLen(cfg, strip_len);
    if(result != 0)
	{
        printf("WS2812: [%s] ws2812_set_len() failed\n", __func__);
    }

err_out:
    if(result != 0 && cfg != NULL)
	{
        if(cfg->mutex != NULL)
		{
            vQueueDelete(cfg->mutex);
        }

        if(cfg->events != NULL)
		{
            vEventGroupDelete(cfg->events);
        }

        if(cfg->dma_buff != NULL)
		{
            free(cfg->dma_buff);
        }

        free(cfg);
        cfg = NULL;
    }
    
    return cfg;
}

int32_t WS2812_SetLen(WS2812_t *cfg, uint16_t strip_len)
{
    int32_t result;
    BaseType_t status;
    uint32_t reset_off;

    result = 0;

    if(cfg == NULL)
	{
        printf("WS2812: [%s] no config given\n", __func__);
        result = -1;
        goto err_out;
    }

    /* lock the config mutex */
    status = xSemaphoreTake(cfg->mutex, configTICK_RATE_HZ);
    if(status != pdTRUE)
	{
        printf("WS2812: [%s] Timeout waiting for config mutex.\n", __func__);
        result = -1;
        goto err_out;
    }

    if(strip_len <= WS2812_LEDS_MAX)
	{
        /* TODO: use dynamically allocated buffer */
        cfg->dma_buff = dma_buffer;

        /* initialise LEDs to off and add reset pulse at end of strip */
        reset_off = WS2812_DMABUF_LEN(strip_len) - WS2812_RESET_LEN;
        memset(&(cfg->dma_buff[0]), WS2812_BITS_00, reset_off);
        memset(&(cfg->dma_buff[reset_off]), 0x0, WS2812_RESET_LEN);
        cfg->strip_len = strip_len;
        cfg->buff_len = WS2812_DMABUF_LEN(strip_len);
    } 
	else 
	{
        printf("WS2812: [%s] Strip too long for DMA buffer\n", __func__);
        result = -1;
    }

    xSemaphoreGive(cfg->mutex);

err_out:
    return result;
}

