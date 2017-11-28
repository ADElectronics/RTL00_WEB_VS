#ifndef _C_TYPES_H_
#define _C_TYPES_H_
/*
typedef unsigned char       uint8_t;
typedef signed char         sint8_t;
typedef signed char         int8_t;
typedef unsigned short      uint16_t;
typedef signed short        sint16_t;
typedef signed short        int16_t;
typedef unsigned long       uint32_t;
typedef signed long         sint32_t;
typedef signed long         int32_t;
typedef signed long long    sint64_t;
typedef unsigned long long  uint64_t;
typedef unsigned long long  u_int64_t;
typedef float               real32_t;
typedef double              real64_t;

typedef unsigned char       uint8;
typedef unsigned char       uint8_t;
typedef signed char         sint8;
typedef signed char         int8;
typedef signed char         int8_t;
typedef unsigned short      uint16;
typedef unsigned short      uint16_t;
typedef signed short        sint16;
typedef signed short        int16_t;
typedef unsigned int        uint32;
typedef unsigned int        u_int;
typedef unsigned int        uint32_t;
typedef signed int          sint32;
typedef signed int          int32_t;
typedef int                 int32;
typedef signed long long    sint64;
typedef unsigned long long  uint64;
typedef unsigned long long  uint64_t;
typedef float               real32;
typedef double              real64;
*/

#define __le16      uint16_t

typedef unsigned int        size_t;
typedef int        			ssize_t;

#ifndef	_SYS_CDEFS_H_
#define __packed        __attribute__((packed))
#endif

#define static       static

#ifndef NULL
#define NULL (void *)0
#endif /* NULL */

/* probably should not put STATUS here */
typedef enum {
    OK = 0,
    FAIL,
    PENDING,
    BUSY,
    CANCEL,
} STATUS;

#define BIT(nr)                 (1UL << (nr))

#define REG_SET_BIT(_r, _b)  (*(volatile uint32_t*)(_r) |= (_b))
#define REG_CLR_BIT(_r, _b)  (*(volatile uint32_t*)(_r) &= ~(_b))

#define DMEM_ATTR 
#define SHMEM_ATTR

#ifdef ICACHE_FLASH
#define ICACHE_FLASH_ATTR 
#define ICACHE_RODATA_ATTR 
#else
#define ICACHE_FLASH_ATTR
#define ICACHE_RODATA_ATTR
#endif /* ICACHE_FLASH */

#ifndef __cplusplus
typedef unsigned char   bool;
//#define BOOL            bool
#define true            (1)
#define false           (0)
#define TRUE            true
#define FALSE           false


#endif /* !__cplusplus */

#endif /* _C_TYPES_H_ */