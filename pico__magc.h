#ifndef __I_MAGC__
#define __I_MAGC__

typedef unsigned int    UINT;   /* int must be 16-bit or 32-bit */
typedef unsigned char   BYTE;   /* char must be 8-bit */
typedef uint16_t        WORD;   /* 16-bit unsigned integer */
typedef uint32_t        DWORD;  /* 32-bit unsigned integer */
typedef uint64_t        QWORD;  /* 64-bit unsigned integer */
typedef WORD            WCHAR;  /* UTF-16 character type */

#include "pico.h"

/*
 *
 */
#define DCS_SOFT_RESET                 0x01
#define DCS_EXIT_SLEEP_MODE            0x11
#define DCS_EXIT_INVERT_MODE           0x20
#define DCS_ENTER_INVERT_MODE          0x21
#define DCS_SET_DISPLAY_ON             0x29
#define DCS_SET_COLUMN_ADDRESS         0x2A
#define DCS_SET_PAGE_ADDRESS           0x2B
#define DCS_WRITE_MEMORY_START         0x2C
#define DCS_SET_ADDRESS_MODE           0x36
#define DCS_SET_PIXEL_FORMAT           0x3A
#define DCS_WRITE_DISPLAY_BRIGHTNESS    0x51
#define DCS_GAMMA_SET                   0x26

#define DCS_PIXEL_FORMAT_16BIT         0x55 /* 0b01010101 */
#define DCS_PIXEL_FORMAT_8BIT          0x22 /* 0b00100010 */

#define DCS_ADDRESS_MODE_MIRROR_Y      0x80
#define DCS_ADDRESS_MODE_MIRROR_X      0x40
#define DCS_ADDRESS_MODE_SWAP_XY       0x20
#define DCS_ADDRESS_MODE_BGR           0x08
#define DCS_ADDRESS_MODE_RGB           0x00
#define DCS_ADDRESS_MODE_FLIP_X        0x02


#ifdef ST7735
// ST7735: офіційно ~15 МГц, на коротких дротах зазвичай тримає 16-24 МГц.
// Якщо на екрані шум -- зменш (наприклад до 8000000).
#define    DISPLAY_SPI_CLOCK_SPEED_HZ 8000000
#define    DISPLAY_PIXEL_FORMAT 0x05   /* 16 біт для ST7735 */
#else
#define    DISPLAY_SPI_CLOCK_SPEED_HZ 63000000
#define    DISPLAY_PIXEL_FORMAT DCS_PIXEL_FORMAT_16BIT
#endif

#if defined(ST7735)
// Орієнтація/порядок кольорів для ST7735 (ландшафт 160x128).
// Якщо картинка догори ногами: заміни MIRROR_Y на MIRROR_X.
// Якщо червоний і синій поміняні місцями: заміни BGR на RGB.
#define    DISPLAY_ADDRESS_MODE  (DCS_ADDRESS_MODE_BGR | DCS_ADDRESS_MODE_SWAP_XY | DCS_ADDRESS_MODE_MIRROR_Y)
#elif defined(ILI9341)
#define    DISPLAY_ADDRESS_MODE DCS_ADDRESS_MODE_RGB | DCS_ADDRESS_MODE_SWAP_XY
#elif defined(ST7789)
//#define    DISPLAY_ADDRESS_MODE DCS_ADDRESS_MODE_RGB | DCS_ADDRESS_MODE_SWAP_XY | DCS_ADDRESS_MODE_MIRROR_Y
#define    DISPLAY_ADDRESS_MODE  DCS_ADDRESS_MODE_BGR | DCS_ADDRESS_MODE_SWAP_XY | DCS_ADDRESS_MODE_MIRROR_Y
#endif

// Зсув вікна дисплея. Для 128x160 ST7735 з "green tab" інколи треба 1 і 2 (або 2 і 1).
#define    DISPLAY_OFFSET_X 0
#define    DISPLAY_OFFSET_Y 0

#ifdef ILI9341
#define    DISPLAY_WIDTH 320
#define    DISPLAY_HEIGHT 240
#endif
#ifdef ST7789
#define    DISPLAY_WIDTH 160
#define    DISPLAY_HEIGHT 128
#endif

#undef    DISPLAY_INVERT 

const uint LED_PIN = PICO_DEFAULT_LED_PIN;

/*
 *
 */
#define UP 2
#define DN 3
#define LT 4
#define RT 5
#define SL 0
#define ST 8
#define A 6
#define B 7

#endif
