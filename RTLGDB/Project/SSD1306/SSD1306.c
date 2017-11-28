#include "SSD1306.h"
#include "FreeRTOS.h"

i2c_t SSD1306_i2c;
//i2c_drv_t SSD1306_i2c;

// SSD1306 data buffer
static uint8_t SSD1306_Buffer[SSD1306_WIDTH * SSD1306_HEIGHT / 8];
static SSD1306_t SSD1306;

void SSD1306_I2C_WriteMulti(uint8_t reg, uint8_t* data, uint16_t count)
{
	uint8_t datasend[count + 1];
	
	datasend[0] = reg;
	for (int i = 0; i<count; i++)
	{ 
		datasend[i + 1] = data[i];
	}

	i2c_write(&SSD1306_i2c, SSD1306_I2C_ADDR, &datasend, count + 1, 1);
	//_i2c_write(&SSD1306_i2c, SSD1306_I2C_ADDR, &datasend, count + 1, 1);
}

void SSD1306_I2C_Write(uint8_t reg, uint8_t data)
{
	uint8_t datasend[2] = { reg , data };
	i2c_write(&SSD1306_i2c, SSD1306_I2C_ADDR, &datasend, 2, 1);
	//_i2c_write(&SSD1306_i2c, SSD1306_I2C_ADDR, &datasend, 2, 1);
}

uint8_t SSD1306_Init(i2c_t *i2c)
{
	//_memset(&SSD1306_i2c, 0x00, sizeof(i2c_t));
	//i2c_init(&SSD1306_i2c, SSD1306_I2C_PIN_SDA, SSD1306_I2C_PIN_SCL);
	//i2c_frequency(&SSD1306_i2c, SSD1306_I2C_BUS_CLK);

	//SSD1306_i2c.status = DRV_I2C_OFF;
	//SSD1306_i2c.idx = 3; // I2C1
	//SSD1306_i2c.io_sel = S0; // PC_4, PC_5
	//SSD1306_i2c.mode = DRV_I2C_HS_MODE;
	
	//_i2c_setup(&SSD1306_i2c, PB_3, PB_2, DRV_I2C_HS_MODE);
	//_i2c_init(&SSD1306_i2c);

	SSD1306_i2c = *i2c;

	//vTaskDelay(10);
	
	SSD1306_WRITECOMMAND(0xAE); //display off
	SSD1306_WRITECOMMAND(0x20); //Set Memory Addressing Mode   
	SSD1306_WRITECOMMAND(0x10); //00,Horizontal Addressing Mode;01,Vertical Addressing Mode;10,Page Addressing Mode (RESET);11,Invalid
	SSD1306_WRITECOMMAND(0xB0); //Set Page Start Address for Page Addressing Mode,0-7
	SSD1306_WRITECOMMAND(0xC8); //Set COM Output Scan Direction
	SSD1306_WRITECOMMAND(0x00); //---set low column address
	SSD1306_WRITECOMMAND(0x10); //---set high column address
	SSD1306_WRITECOMMAND(0x40); //--set start line address
	SSD1306_WRITECOMMAND(0x81); //--set contrast control register
	SSD1306_WRITECOMMAND(0xFF);
	SSD1306_WRITECOMMAND(0xA1); //--set segment re-map 0 to 127
	SSD1306_WRITECOMMAND(0xA6); //--set normal display
	SSD1306_WRITECOMMAND(0xA8); //--set multiplex ratio(1 to 64)
	SSD1306_WRITECOMMAND(0x3F); //
	SSD1306_WRITECOMMAND(0xA4); //0xa4,Output follows RAM content;0xa5,Output ignores RAM content
	SSD1306_WRITECOMMAND(0xD3); //-set display offset
	SSD1306_WRITECOMMAND(0x00); //-not offset
	SSD1306_WRITECOMMAND(0xD5); //--set display clock divide ratio/oscillator frequency
	SSD1306_WRITECOMMAND(0xF0); //--set divide ratio
	SSD1306_WRITECOMMAND(0xD9); //--set pre-charge period
	SSD1306_WRITECOMMAND(0x22); //
	SSD1306_WRITECOMMAND(0xDA); //--set com pins hardware configuration
	SSD1306_WRITECOMMAND(0x12);
	SSD1306_WRITECOMMAND(0xDB); //--set vcomh
	SSD1306_WRITECOMMAND(0x20); //0x20,0.77xVcc
	SSD1306_WRITECOMMAND(0x8D); //--set DC-DC enable
	SSD1306_WRITECOMMAND(0x14); //
	SSD1306_WRITECOMMAND(0xAF); //--turn on SSD1306 panel
	
	SSD1306_Fill(SSD1306_COLOR_BLACK);
	
	SSD1306_UpdateScreen();
	
	SSD1306.CurrentX = 0;
	SSD1306.CurrentY = 0;
	
	SSD1306.Initialized = 1;
	
	return 1;
}

void SSD1306_UpdateScreen(void) 
{
	uint8_t m;
	
	for (m = 0; m < 8; m++) 
	{
		SSD1306_WRITECOMMAND(0xB0 + m);
		SSD1306_WRITECOMMAND(0x00);
		SSD1306_WRITECOMMAND(0x10);
		
		SSD1306_I2C_WriteMulti(0x40, &SSD1306_Buffer[SSD1306_WIDTH * m], SSD1306_WIDTH);
	}
}

void SSD1306_ToggleInvert(void) 
{
	uint16_t i;
	
	SSD1306.Inverted = !SSD1306.Inverted;
	
	for (i = 0; i < sizeof(SSD1306_Buffer); i++) 
	{
		SSD1306_Buffer[i] = ~SSD1306_Buffer[i];
	}
}

void SSD1306_Fill(SSD1306_COLOR_t color) 
{
	memset(SSD1306_Buffer, (color == SSD1306_COLOR_BLACK) ? 0x00 : 0xFF, sizeof(SSD1306_Buffer));
}

void SSD1306_DrawPixel(uint16_t x, uint16_t y, SSD1306_COLOR_t color) 
{
	if(x >= SSD1306_WIDTH || y >= SSD1306_HEIGHT) 
	{
		return;
	}
	
	if (SSD1306.Inverted) 
	{
		color = (SSD1306_COLOR_t)!color;
	}
	
	if (color == SSD1306_COLOR_WHITE) 
	{
		SSD1306_Buffer[x + (y / 8) * SSD1306_WIDTH] |= 1 << (y % 8);
	} 
	else 
	{
		SSD1306_Buffer[x + (y / 8) * SSD1306_WIDTH] &= ~(1 << (y % 8));
	}
}

void SSD1306_GotoXY(uint16_t x, uint16_t y) 
{
	SSD1306.CurrentX = x;
	SSD1306.CurrentY = y;
}

char SSD1306_Putc(char ch, FontDef_t* Font, SSD1306_COLOR_t color) 
{
	uint32_t i, b, j;
	
	// Check available space in LCD
	if(SSD1306_WIDTH <= (SSD1306.CurrentX + Font->FontWidth) || SSD1306_HEIGHT <= (SSD1306.CurrentY + Font->FontHeight)) 
	{
		return 0;
	}
	
	for (i = 0; i < Font->FontHeight; i++) 
	{
		b = Font->data[(ch - 32) * Font->FontHeight + i];
		for (j = 0; j < Font->FontWidth; j++) 
		{
			if ((b << j) & 0x8000) 
			{
				SSD1306_DrawPixel(SSD1306.CurrentX + j, (SSD1306.CurrentY + i), (SSD1306_COLOR_t) color);
			} 
			else 
			{
				SSD1306_DrawPixel(SSD1306.CurrentX + j, (SSD1306.CurrentY + i), (SSD1306_COLOR_t)!color);
			}
		}
	}
	
	SSD1306.CurrentX += Font->FontWidth;
	return ch;
}

char SSD1306_Puts(char* str, FontDef_t* Font, SSD1306_COLOR_t color) 
{
	while (*str) 
	{
		if (SSD1306_Putc(*str, Font, color) != *str) 
		{
			return *str; // Error
		}
		str++;
	}

	return *str; // Ok
}
 

void SSD1306_DrawLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, SSD1306_COLOR_t c) 
{
	int16_t dx, dy, sx, sy, err, e2, i, tmp; 

	if(x0 >= SSD1306_WIDTH) 
	{
		x0 = SSD1306_WIDTH - 1;
	}
	if(x1 >= SSD1306_WIDTH) 
	{
		x1 = SSD1306_WIDTH - 1;
	}
	if(y0 >= SSD1306_HEIGHT) 
	{
		y0 = SSD1306_HEIGHT - 1;
	}
	if(y1 >= SSD1306_HEIGHT) 
	{
		y1 = SSD1306_HEIGHT - 1;
	}
	
	dx = (x0 < x1) ? (x1 - x0) : (x0 - x1); 
	dy = (y0 < y1) ? (y1 - y0) : (y0 - y1); 
	sx = (x0 < x1) ? 1 : -1; 
	sy = (y0 < y1) ? 1 : -1; 
	err = ((dx > dy) ? dx : -dy) / 2; 

	if(dx == 0) 
	{
		if(y1 < y0) 
		{
			tmp = y1;
			y1 = y0;
			y0 = tmp;
		}
		
		if(x1 < x0) 
		{
			tmp = x1;
			x1 = x0;
			x0 = tmp;
		}
		
		// Vertical line
		for(i = y0; i <= y1; i++)
		{
			SSD1306_DrawPixel(x0, i, c);
		}
		
		return;
	}
	
	if(dy == 0)
	{
		if (y1 < y0) 
		{
			tmp = y1;
			y1 = y0;
			y0 = tmp;
		}
		
		if (x1 < x0) 
		{
			tmp = x1;
			x1 = x0;
			x0 = tmp;
		}
		
		// Horizontal line
		for (i = x0; i <= x1; i++) 
		{
			SSD1306_DrawPixel(i, y0, c);
		}
		
		return;
	}
	
	while (1) 
	{
		SSD1306_DrawPixel(x0, y0, c);
		if (x0 == x1 && y0 == y1) 
		{
			break;
		}
		e2 = err; 
		if (e2 > -dx) 
		{
			err -= dy;
			x0 += sx;
		} 
		if (e2 < dy) 
		{
			err += dx;
			y0 += sy;
		} 
	}
}

void SSD1306_DrawRectangle(uint16_t x, uint16_t y, uint16_t w, uint16_t h, SSD1306_COLOR_t c) 
{
	if (x >= SSD1306_WIDTH || y >= SSD1306_HEIGHT) 
	{
		return;
	}
	
	if ((x + w) >= SSD1306_WIDTH) 
	{
		w = SSD1306_WIDTH - x;
	}
	if ((y + h) >= SSD1306_HEIGHT) 
	{
		h = SSD1306_HEIGHT - y;
	}
	
	SSD1306_DrawLine(x, y, x + w, y, c);         //Top line
	SSD1306_DrawLine(x, y + h, x + w, y + h, c); // Bottom line
	SSD1306_DrawLine(x, y, x, y + h, c);         // Left line
	SSD1306_DrawLine(x + w, y, x + w, y + h, c); // Right line
}

void SSD1306_DrawFilledRectangle(uint16_t x, uint16_t y, uint16_t w, uint16_t h, SSD1306_COLOR_t c) 
{
	uint8_t i;

	if(x >= SSD1306_WIDTH || y >= SSD1306_HEIGHT) 
	{
		return;
	}
	
	if ((x + w) >= SSD1306_WIDTH) 
	{
		w = SSD1306_WIDTH - x;
	}
	if ((y + h) >= SSD1306_HEIGHT) 
	{
		h = SSD1306_HEIGHT - y;
	}
	
	for (i = 0; i <= h; i++)
	{
		SSD1306_DrawLine(x, y + i, x + w, y + i, c);
	}
}

void SSD1306_DrawTriangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t x3, uint16_t y3, SSD1306_COLOR_t color) 
{
	SSD1306_DrawLine(x1, y1, x2, y2, color);
	SSD1306_DrawLine(x2, y2, x3, y3, color);
	SSD1306_DrawLine(x3, y3, x1, y1, color);
}


void SSD1306_DrawFilledTriangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t x3, uint16_t y3, SSD1306_COLOR_t color) 
{
	int16_t deltax = 0, deltay = 0, x = 0, y = 0, xinc1 = 0, xinc2 = 0, 
	yinc1 = 0, yinc2 = 0, den = 0, num = 0, numadd = 0, numpixels = 0, 
	curpixel = 0;
	
	deltax = ABS(x2 - x1);
	deltay = ABS(y2 - y1);
	x = x1;
	y = y1;

	if(x2 >= x1) 
	{
		xinc1 = 1;
		xinc2 = 1;
	} 
	else 
	{
		xinc1 = -1;
		xinc2 = -1;
	}

	if(y2 >= y1) 
	{
		yinc1 = 1;
		yinc2 = 1;
	} 
	else 
	{
		yinc1 = -1;
		yinc2 = -1;
	}

	if(deltax >= deltay)
	{
		xinc1 = 0;
		yinc2 = 0;
		den = deltax;
		num = deltax / 2;
		numadd = deltay;
		numpixels = deltax;
	} 
	else 
	{
		xinc2 = 0;
		yinc1 = 0;
		den = deltay;
		num = deltay / 2;
		numadd = deltax;
		numpixels = deltay;
	}

	for (curpixel = 0; curpixel <= numpixels; curpixel++) 
	{
		SSD1306_DrawLine(x, y, x3, y3, color);

		num += numadd;
		if (num >= den) 
		{
			num -= den;
			x += xinc1;
			y += yinc1;
		}
		x += xinc2;
		y += yinc2;
	}
}

void SSD1306_DrawCircle(int16_t x0, int16_t y0, int16_t r, SSD1306_COLOR_t c) 
{
	int16_t f = 1 - r;
	int16_t ddF_x = 1;
	int16_t ddF_y = -2 * r;
	int16_t x = 0;
	int16_t y = r;

    SSD1306_DrawPixel(x0, y0 + r, c);
    SSD1306_DrawPixel(x0, y0 - r, c);
    SSD1306_DrawPixel(x0 + r, y0, c);
    SSD1306_DrawPixel(x0 - r, y0, c);

    while (x < y) 
	{
        if (f >= 0) 
		{
            y--;
            ddF_y += 2;
            f += ddF_y;
        }
        x++;
        ddF_x += 2;
        f += ddF_x;

        SSD1306_DrawPixel(x0 + x, y0 + y, c);
        SSD1306_DrawPixel(x0 - x, y0 + y, c);
        SSD1306_DrawPixel(x0 + x, y0 - y, c);
        SSD1306_DrawPixel(x0 - x, y0 - y, c);

        SSD1306_DrawPixel(x0 + y, y0 + x, c);
        SSD1306_DrawPixel(x0 - y, y0 + x, c);
        SSD1306_DrawPixel(x0 + y, y0 - x, c);
        SSD1306_DrawPixel(x0 - y, y0 - x, c);
    }
}

void SSD1306_DrawFilledCircle(int16_t x0, int16_t y0, int16_t r, SSD1306_COLOR_t c) 
{
	int16_t f = 1 - r;
	int16_t ddF_x = 1;
	int16_t ddF_y = -2 * r;
	int16_t x = 0;
	int16_t y = r;

    SSD1306_DrawPixel(x0, y0 + r, c);
    SSD1306_DrawPixel(x0, y0 - r, c);
    SSD1306_DrawPixel(x0 + r, y0, c);
    SSD1306_DrawPixel(x0 - r, y0, c);
    SSD1306_DrawLine(x0 - r, y0, x0 + r, y0, c);

    while (x < y) 
	{
        if (f >= 0) 
		{
            y--;
            ddF_y += 2;
            f += ddF_y;
        }
        x++;
        ddF_x += 2;
        f += ddF_x;

        SSD1306_DrawLine(x0 - x, y0 + y, x0 + x, y0 + y, c);
        SSD1306_DrawLine(x0 + x, y0 - y, x0 - x, y0 - y, c);

        SSD1306_DrawLine(x0 + y, y0 + x, x0 - y, y0 + x, c);
        SSD1306_DrawLine(x0 + y, y0 - x, x0 - y, y0 - x, c);
    }
}
 
void SSD1306_ON(void) 
{
	SSD1306_WRITECOMMAND(0x8D);  
	SSD1306_WRITECOMMAND(0x14);  
	SSD1306_WRITECOMMAND(0xAF);  
}

void SSD1306_OFF(void) 
{
	SSD1306_WRITECOMMAND(0x8D);  
	SSD1306_WRITECOMMAND(0x10);
	SSD1306_WRITECOMMAND(0xAE);  
}
