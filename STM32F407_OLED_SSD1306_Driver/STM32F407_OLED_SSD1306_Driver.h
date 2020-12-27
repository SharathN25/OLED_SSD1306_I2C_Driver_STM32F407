/**
  **********************************************************************************************************************
   * @file   : STM32F407_OLED_SSD1306_Driver.h
   * @author : Sharath N
   * @brief  : SSD1306 OLED Driver Hearder File
  **********************************************************************************************************************
*/

/*
          ------------------PIN Details-------------------
          |                                              |
          | SSD1306    |STM32F4xx    |DESCRIPTION        |
          |                                              |
          | VCC        |3.3V         |Supply Voltage     |
          | GND        |GND          |Ground             |
          | SCL        |PB6          |Serial clock line  |
          | SDA        |PB7          |Serial data line   |
          ------------------------------------------------
*/


#ifndef STM32F407_OLED_SSD1306_DRIVER_H
#define STM32F407_OLED_SSD1306_DRIVER_H

#include <stdio.h>
#include <string.h>
#include "stdint.h"
#include "stm32f4xx_hal.h"                 // Keil::Device:STM32Cube HAL:Common
#include "OLED_SSD1306_Fonts.h"

#define OLED_I2C_ADDRESS             0x78  // SSD1306 OLED Display I2C Slave address 
#define OLED_WIDTH                   128   // SSD1306 OLDE Display width in pixels
#define OLED_HEIGHT                  64    // SSD1306 OLDE Display height in pixels

#define ABS(x)   ((x) > 0 ? (x) : -(x))    //Get the absolute value

/* SSD1306 data buffer */
static uint8_t OLED_Buffer[(OLED_WIDTH * OLED_HEIGHT) / 8];


typedef enum {
	OLED_COLOR_BLACK = 0x00, /*!< Black color, no pixel */
	OLED_COLOR_WHITE = 0x01  /*!< Pixel is set. Color depends on LCD */
} OLED_COLOR_t;




/************* SSD1306 OLED Commands - (Table 9-1: Command Table , Refer  Page 28 of OLED SSD1306 Data sheet **********/

#define OLED_DISPLAY_OFF             0xAE  // Display OFF (sleep mode)
#define OLED_DISPLAY_ON              0xAF  // Display ON in normal mode
#define OLED_SET_MEM_ADDR_MODE       0x20  // Set Memory Addressing mode : 00, 01, 10 (binary A[1:0] below)
#define OLED_HORIZONTAL_ADDR_MODE    0x00  // 00 : Horizontal Addressing Mode
#define OLED_VERTICAL_ADDR_MODE      0x01  // 01 : Vertical Addressing Mode
#define OLED_PAGE_ADDR_MODE          0x02  // 10 : Page Addressing Mode (RESET)
#define OLED_PAGE_START_ADDR         0xB0  // Page Start Address for Page Addressing Mode
#define OLED_COM_SCAN_DIR_NORMAL     0xC0  // normal mode (RESET) Scan from COM0 to COM[N –1] 
#define OLED_COM_SCAN_DIR_REMAPPED   0xC8  // remapped mode. Scan from COM[N-1] to COM0
#define OLED_LOW_COLUMN_START_ADDR   0x00  // lower nibble of the column start address
#define OLED_HIGH_COLUMN_START_AADR  0x10  // higher nibble of the column start address
#define OLED_DISP_START_LINE_ADDR    0x40  // Display start line Address
#define OLED_SET_CONTRAST_CTRL_REG   0x81  // Set Contrast Control, Double byte command to select 1 out of 256(00 to FF)
#define OLED_SET_SEG_REMAP_0_SEG0    0xA0  // column address 0 is mapped to SEG0 (RESET)
#define OLED_SET_SEG_REMAP_127_SEG0  0xA1  // column address 127 is mapped to SEG0
#define OLED_SET_NORMAL_DISPLAY      0xA6  // Normal display (RESET)
#define OLED_SET_INVERSE_DISPLAY     0xA7  // Inverse Display
#define OLED_SET_MULTIPLEX_RATIO     0xA8  // Set MUX ratio to N+1 MUX
#define OLED_OUTPUT_FALLOW_RAM_CNT   0xA4  // Entire Display on, Output follows RAM content
#define OLED_OUTPTU_IGNORE_RAM_CNT   0xA5  // Entire Display on, Output ignores RAM content
#define OLED_SET_DISPLAY_OFFSET      0xD3  // Set Display offset: Set vertical shift by COM from 0d~63d
#define OLED_SET_DIS_CLK_FREQ_RATIO  0xD5  // Set Display Clock Divide Ratio/Oscillator Frequency
#define OLED_SET_PRE_CHARGE_PERIOD   0xD9  // Set Pre-charge Period
#define OLED_SET_COM_PIN_HW_CNF      0xDA  // Set COM Pins Hardware Configuration
#define OLED_SET_DCOMH_DISEL_LEVEL   0xDB  // Set Vcomh Deselect Level
#define OLED_CHARGE_PUMP_SETTING     0x8D  // Charge Pump Setting

/*********************************************** SSD1306 OLED Commands -END *******************************************/


/**
 * @brief Send Command to OLED
 * @param cmd : OLED commands 
 * @retval None
 */
void OLED_SSD1306_Send_Command(uint8_t cmd);


/**
 * @brief Send Data to OLED
 * @param cmd : OLED commands 
 * @retval None
 */
void OLED_SSD1306_Send_Data(uint8_t data);


/**
 * @brief  Initializes OLED SSD1306
 * @retval None
 */
void OLED_SSD1306_Init(void);

/**
 * @brief  Fill the OLED SSD1306 Display
 * @color  Set the display balck or color(whichever)
 * @retval None
 */
void OLED_SSD1306_Fill(OLED_COLOR_t color);


/**
 * @brief  Update the OLED Screen
 * @retval None
 */
void OLED_SSD1306_UpdateScreen(void);


/**
 * @brief  Draw Pixel
 * @param  x,y: pixel cordinates
 * @param  color: colour value
 * @retval None
 */
void OLED_SSD1306_DrawPixel(uint16_t x, uint16_t y, OLED_COLOR_t color);

/**
 * @brief  Go to the location (x,y)
 * @param  x,y: pixel cordinates
 * @retval None
 */
void OLED_SSD1306_GotoXY(uint16_t x, uint16_t y);



/**
 * @brief  Puts character on OLED
 * @note   @ref OLED_SSD1306_UpdateScreen() must be called after that in order to see updated LCD screen
 * @param  ch: Character to be written
 * @param  *Font: Pointer to @ref OLED_FontDef_t structure with used font
 * @param  color: Color used for drawing. This parameter can be a value of @ref OLED_COLOR_t enumeration
 * @retval Character written
 */
char OLED_SSD1306_Putc(char ch, OLED_FontDef_t* Font, OLED_COLOR_t color);


/**
 * @brief  Puts string on OLED
 * @note   @ref OLED_SSD1306_UpdateScreen() must be called after that in order to see updated LCD screen
 * @param  *str: String to be written
 * @param  *Font: Pointer to @ref OLED_FontDef_t structure with used font
 * @param  color: Color used for drawing. This parameter can be a value of @ref OLED_COLOR_t enumeration
 * @retval Zero on success or character value when function failed
 */
char OLED_SSD1306_Puts(char* str, OLED_FontDef_t* Font, OLED_COLOR_t color);


/**
 * @brief  Draws line on OLED
 * @note   @ref OLED_SSD1306_UpdateScreen() must be called after that in order to see updated LCD screen
 * @param  x0: Line X start point. Valid input is 0 to OLED_WIDTH - 1
 * @param  y0: Line Y start point. Valid input is 0 to OLED_HEIGHT - 1
 * @param  x1: Line X end point. Valid input is 0 to OLED_WIDTH - 1
 * @param  y1: Line Y end point. Valid input is 0 to OLED_HEIGHT - 1
 * @param  c: Color to be used. This parameter can be a value of @ref OLED_COLOR_t enumeration
 * @retval None
 */
void OLED_SSD1306_DrawLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, OLED_COLOR_t color);



/**
 * @brief  Draws rectangle on OLED
 * @note   @ref OLED_SSD1306_UpdateScreen() must be called after that in order to see updated LCD screen
 * @param  x: Top left X start point. Valid input is 0 to OLED_WIDTH - 1
 * @param  y: Top left Y start point. Valid input is 0 to OLED_HEIGHT - 1
 * @param  w: Rectangle width in units of pixels
 * @param  h: Rectangle height in units of pixels
 * @param  c: Color to be used. This parameter can be a value of @ref OLED_COLOR_t enumeration
 * @retval None
 */
void OLED_SSD1306_DrawRectangle(uint16_t x, uint16_t y, uint16_t w, uint16_t h, OLED_COLOR_t c);


/**
 * @brief  Draws filled rectangle on OLED
 * @note   @ref OLED_SSD1306_UpdateScreen() must be called after that in order to see updated LCD screen
 * @param  x: Top left X start point. Valid input is 0 to OLED_WIDTH - 1
 * @param  y: Top left Y start point. Valid input is 0 to OLED_HEIGHT - 1
 * @param  w: Rectangle width in units of pixels
 * @param  h: Rectangle height in units of pixels
 * @param  c: Color to be used. This parameter can be a value of @ref OLED_COLOR_t enumeration
 * @retval None
 */
void OLED_SSD1306_DrawFilledRectangle(uint16_t x, uint16_t y, uint16_t w, uint16_t h, OLED_COLOR_t c);


/**
 * @brief  Draws triangle on OLED
 * @note   @ref OLED_SSD1306_UpdateScreen() must be called after that in order to see updated LCD screen
 * @param  x1: First coordinate X location. Valid input is 0 to OLED_WIDTH - 1
 * @param  y1: First coordinate Y location. Valid input is 0 to OLED_HEIGHT - 1
 * @param  x2: Second coordinate X location. Valid input is 0 to OLED_WIDTH - 1
 * @param  y2: Second coordinate Y location. Valid input is 0 to OLED_HEIGHT - 1
 * @param  x3: Third coordinate X location. Valid input is 0 to OLED_WIDTH - 1
 * @param  y3: Third coordinate Y location. Valid input is 0 to OLED_HEIGHT - 1
 * @param  c: Color to be used. This parameter can be a value of @ref OLED_COLOR_t enumeration
 * @retval None
 */
void OLED_SSD1306_DrawTriangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t x3, uint16_t y3, OLED_COLOR_t color);


/**
 * @brief  Draws filled triangle on OLED
 * @note   @ref OLED_SSD1306_UpdateScreen() must be called after that in order to see updated LCD screen
 * @param  x1: First coordinate X location. Valid input is 0 to OLED_WIDTH - 1
 * @param  y1: First coordinate Y location. Valid input is 0 to OLED_HEIGHT - 1
 * @param  x2: Second coordinate X location. Valid input is 0 to OLED_WIDTH - 1
 * @param  y2: Second coordinate Y location. Valid input is 0 to OLED_HEIGHT - 1
 * @param  x3: Third coordinate X location. Valid input is 0 to OLED_WIDTH - 1
 * @param  y3: Third coordinate Y location. Valid input is 0 to OLED_HEIGHT - 1
 * @param  c: Color to be used. This parameter can be a value of @ref OLED_COLOR_t enumeration
 * @retval None
 */
void OLED_SSD1306_DrawFilledTriangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t x3, uint16_t y3, OLED_COLOR_t color);


/**
 * @brief  Draws circle on OLED
 * @note   @ref OLED_SSD1306_UpdateScreen() must be called after that in order to see updated LCD screen
 * @param  x: X location for center of circle. Valid input is 0 to OLED_WIDTH - 1
 * @param  y: Y location for center of circle. Valid input is 0 to OLED_HEIGHT - 1
 * @param  r: Circle radius in units of pixels
 * @param  c: Color to be used. This parameter can be a value of @ref OLED_COLOR_t enumeration
 * @retval None
 */
void OLED_SSD1306_DrawCircle(int16_t x0, int16_t y0, int16_t r, OLED_COLOR_t c);


/**
 * @brief  Draws filled circle on OLED
 * @note   @ref OLED_SSD1306_UpdateScreen() must be called after that in order to see updated LCD screen
 * @param  x: X location for center of circle. Valid input is 0 to OLED_WIDTH - 1
 * @param  y: Y location for center of circle. Valid input is 0 to OLED_HEIGHT - 1
 * @param  r: Circle radius in units of pixels
 * @param  c: Color to be used. This parameter can be a value of @ref OLED_COLOR_t enumeration
 * @retval None
 */
void OLED_SSD1306_DrawFilledCircle(int16_t x0, int16_t y0, int16_t r, OLED_COLOR_t c);



#endif
