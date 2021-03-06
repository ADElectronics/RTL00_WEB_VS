/*
 *  Routines to access hardware
 *
 *  Copyright (c) 2013 Realtek Semiconductor Corp.
 *
 *  This module is a confidential and proprietary property of RealTek and
 *  possession or use of this module requires written permission of RealTek.
 */

#ifndef _HAL_DIAG_H_
#define _HAL_DIAG_H_


//Register offset
#define UART_REV_BUF_OFF            0x00
#define UART_TRAN_HOLD_OFF          0x00
#define UART_DLH_OFF                0x04
#define UART_DLL_OFF                0x00
#define UART_INTERRUPT_EN_REG_OFF   0x04
#define UART_INTERRUPT_IDEN_REG_OFF 0x08
#define UART_FIFO_CTL_REG_OFF       0x08
#define UART_LINE_CTL_REG_OFF       0x0c
#define UART_MODEM_CTL_REG_OFF      0x10
#define UART_LINE_STATUS_REG_OFF    0x14
#define UART_MODEM_STATUS_REG_OFF   0x18
#define UART_FIFO_ACCESS_REG_OFF    0x70
#define UART_STATUS_REG_OFF         0x7c
#define UART_TFL_OFF                0x80
#define UART_RFL_OFF                0x84


//Buad rate
#define UART_BAUD_RATE_2400         2400
#define UART_BAUD_RATE_4800         4800
#define UART_BAUD_RATE_9600         9600
#define UART_BAUD_RATE_19200        19200
#define UART_BAUD_RATE_38400        38400
#define UART_BAUD_RATE_57600        57600
#define UART_BAUD_RATE_115200       115200
#define UART_BAUD_RATE_921600       921600
#define UART_BAUD_RATE_1152000      1152000

#define UART_PARITY_ENABLE          0x08
#define UART_PARITY_DISABLE         0

#define UART_DATA_LEN_5BIT          0x0
#define UART_DATA_LEN_6BIT          0x1
#define UART_DATA_LEN_7BIT          0x2
#define UART_DATA_LEN_8BIT          0x3

#define UART_STOP_1BIT              0x0
#define UART_STOP_2BIT              0x4

#ifndef DEFAULT_BAUDRATE
#define DEFAULT_BAUDRATE			UART_BAUD_RATE_38400
#endif

#define HAL_UART_READ32(addr)            HAL_READ32(LOG_UART_REG_BASE, addr)
#define HAL_UART_WRITE32(addr, value)    HAL_WRITE32(LOG_UART_REG_BASE, addr, value)
#define HAL_UART_READ16(addr)            HAL_READ16(LOG_UART_REG_BASE, addr)
#define HAL_UART_WRITE16(addr, value)    HAL_WRITE16(LOG_UART_REG_BASE, addr, value)
#define HAL_UART_READ8(addr)             HAL_READ8(LOG_UART_REG_BASE, addr)
#define HAL_UART_WRITE8(addr, value)     HAL_WRITE8(LOG_UART_REG_BASE, addr, value)

typedef struct _LOG_UART_ADAPTER_ {
    uint32_t     BaudRate;
    uint32_t     FIFOControl; 
    uint32_t     IntEnReg;
    uint8_t      Parity;
    uint8_t      Stop;
    uint8_t      DataLength;
}LOG_UART_ADAPTER, *PLOG_UART_ADAPTER;

typedef struct _COMMAND_TABLE_ {
    const   char * cmd;
    uint16_t     ArgvCnt;
    void     (*func)(int argc, char * argv[]);
    const   char * msg;
}COMMAND_TABLE, *PCOMMAND_TABLE;

//void
//HalLogUartHandle(void);


extern _LONG_CALL_ROM_ uint32_t 
HalLogUartInit(
    IN  LOG_UART_ADAPTER    UartAdapter
    );


extern _LONG_CALL_ROM_ void 
HalSerialPutcRtl8195a(
    IN  uint8_t c
    );

extern _LONG_CALL_ROM_ uint8_t 
HalSerialGetcRtl8195a(
    IN  BOOL    PullMode
    );

extern _LONG_CALL_ROM_ uint32_t
HalSerialGetIsrEnRegRtl8195a(void);

extern _LONG_CALL_ROM_ void
HalSerialSetIrqEnRegRtl8195a (
    IN  uint32_t SetValue
);


#endif//_HAL_DIAG_H_
