/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM32F429_LCD_H
#define __STM32F429_LCD_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include "stm32f4xx_hal.h"
#include "fonts.h"
   
#define LCD_FRAME_BUFFER0        ((uint32_t)0xD0000000)
#define LCD_FRAME_BUFFER1        (LCD_FRAME_BUFFER0 + LCD_BUFFER_SIZE)
   
   
#define LCD_TIMING_HSYNC        96
#define LCD_TIMING_HBP          48
#define LCD_TIMING_HFP          16

#define LCD_TIMING_VSYNC        2
#define LCD_TIMING_VBP          33
#define LCD_TIMING_VFP          10

#define LCD_BYTES_IN_PIXEL      2 //Not done!!!
   
/** 
  * @brief  LCD color RGB565
  */ 
#define LCD_COLOR_WHITE         0xFFFF
#define LCD_COLOR_BLACK         0x0000
#define LCD_COLOR_GREY          0xF7DE
#define LCD_COLOR_BLUE          0x001F
#define LCD_COLOR_BLUE2         0x051F
#define LCD_COLOR_RED           0xF800
#define LCD_COLOR_MAGENTA       0xF81F
#define LCD_COLOR_GREEN         0x07E0
#define LCD_COLOR_CYAN          0x7FFF
#define LCD_COLOR_YELLOW        0xFFE0
   
/* LCD Size (Width and Height) */
#define  LCD_PIXEL_WIDTH    ((uint16_t)640)
#define  LCD_PIXEL_HEIGHT   ((uint16_t)480)
#define  LCD_PIXELS_CNT     ((uint32_t)LCD_PIXEL_WIDTH * (uint32_t)LCD_PIXEL_HEIGHT)
#define  LCD_BUFFER_SIZE    ((uint32_t)(LCD_PIXELS_CNT * LCD_BYTES_IN_PIXEL))
   
void MX_LTDC_Init(void);
void lcd_switch_to_single_buffer_mode(void);
void lcd_switch_buffer(void);

void lcd_draw_test_picture(void);

void LCD_DrawFullRect(uint16_t xpos, uint16_t ypos, uint16_t width, uint16_t height, uint16_t color);
void LCD_SetTextColor(uint16_t color);
void lcd_clear(uint16_t Color);
void LCD_SetColors(uint16_t TextColor, uint16_t BackColor);
void LCD_GetColors(uint16_t *TextColor, uint16_t *BackColor);
void LCD_SetTextColor(uint16_t Color);
void LCD_SetBackColor(uint16_t Color);

void LCD_SetFont(sFONT *fonts);
sFONT *LCD_GetFont(void);
void LCD_DrawChar(uint16_t Xpos, uint16_t Ypos, const uint16_t *c);
void LCD_DisplayChar(uint16_t Line, uint16_t Column, uint8_t Ascii);
void LCD_DisplayStringLine(uint16_t Line, uint8_t *ptr);
void lcd_print_char(char c);

void lcd_set_pixel(uint16_t x, uint16_t y, uint16_t color);

void lcd_draw_yuyv_picture(uint8_t* source);
uint32_t lcd_convert_to_color32(uint16_t color);
   
#endif /* __STM32F429_LCD_H */
