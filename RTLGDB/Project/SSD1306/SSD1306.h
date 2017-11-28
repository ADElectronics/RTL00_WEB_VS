#ifndef _SSD1306_H_
#define _SSD1306_H_

#include "device.h"
#include "fonts.h"

#include "stdlib.h"
#include "string.h"

#include "i2c_api.h"
#include "pinmap.h"
#include "ex_api.h"

#include "driver/i2c_drv.h"

#define SSD1306_I2C_PIN_SDA		PC_4
#define SSD1306_I2C_PIN_SCL		PC_5
#define SSD1306_I2C_BUS_CLK		1000000  //hz

// I2C address
#ifndef SSD1306_I2C_ADDR
#define SSD1306_I2C_ADDR		 (0x78 >> 1)
//#define SSD1306_I2C_ADDR       0x7A
#endif

// SSD1306 settings
#ifndef SSD1306_WIDTH
#define SSD1306_WIDTH            128
#endif
#ifndef SSD1306_HEIGHT
#define SSD1306_HEIGHT           64
#endif

typedef enum {
	SSD1306_COLOR_BLACK = 0x00,
	SSD1306_COLOR_WHITE = 0x01
} SSD1306_COLOR_t;

typedef struct {
	uint16_t CurrentX;
	uint16_t CurrentY;
	uint8_t Inverted;
	uint8_t Initialized;
} SSD1306_t;

#define SSD1306_WRITECOMMAND(command)		SSD1306_I2C_Write(0x00, (command))
#define SSD1306_WRITEDATA(data)				SSD1306_I2C_Write(0x40, (data))
#define ABS(x)								((x) > 0 ? (x) : -(x))

uint8_t SSD1306_Init(i2c_t *i2c);
void SSD1306_UpdateScreen(void);
void SSD1306_ToggleInvert(void);
void SSD1306_Fill(SSD1306_COLOR_t Color);
void SSD1306_DrawPixel(uint16_t x, uint16_t y, SSD1306_COLOR_t color);
void SSD1306_GotoXY(uint16_t x, uint16_t y);
char SSD1306_Putc(char ch, FontDef_t* Font, SSD1306_COLOR_t color);
char SSD1306_Puts(char* str, FontDef_t* Font, SSD1306_COLOR_t color);
void SSD1306_DrawLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, SSD1306_COLOR_t c);
void SSD1306_DrawRectangle(uint16_t x, uint16_t y, uint16_t w, uint16_t h, SSD1306_COLOR_t c);
void SSD1306_DrawFilledRectangle(uint16_t x, uint16_t y, uint16_t w, uint16_t h, SSD1306_COLOR_t c);
void SSD1306_DrawTriangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t x3, uint16_t y3, SSD1306_COLOR_t color);
void SSD1306_DrawCircle(int16_t x0, int16_t y0, int16_t r, SSD1306_COLOR_t c);
void SSD1306_DrawFilledCircle(int16_t x0, int16_t y0, int16_t r, SSD1306_COLOR_t c);

#endif // _SSD1306_H_
