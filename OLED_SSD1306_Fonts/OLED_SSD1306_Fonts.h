/**
  **********************************************************************************************************************
   * @file   : OLED_SSD1306_Fonts.h
   * @author : Sharath N
   * @brief  : Fonts header file for SSD1306 OLED Display
  **********************************************************************************************************************
*/

#ifndef OLED_SSD1306_FONTS_H
#define OLED_SSD1306_FONTS_H

#include <string.h>
#include <stdint.h>

/**
 * @brief  Font structure used by OLED Drivers
 */
typedef struct {
	uint8_t FontWidth;    /*!< Font width in pixels */
	uint8_t FontHeight;   /*!< Font height in pixels */
	const uint16_t *data; /*!< Pointer to data font data array */
} OLED_FontDef_t;


/** 
 * @brief  String length and height 
 */
typedef struct {
	uint16_t Length;      /*!< String length in units of pixels */
	uint16_t Height;      /*!< String height in units of pixels */
} OLED_Fonts_Size_t;


/**
 * @brief  7 x 10 pixels font size structure 
 */
extern OLED_FontDef_t OLED_Font_7x10;

/**
 * @brief  11 x 18 pixels font size structure 
 */
extern OLED_FontDef_t OLED_Font_11x18;

/**
 * @brief  16 x 26 pixels font size structure 
 */
extern OLED_FontDef_t OLED_Font_16x26;


/**
 * @brief  Calculates string length and height in units of pixels depending on string and font used
 * @param  *str: String to be checked for length and height
 * @param  *SizeStruct: Pointer to empty @ref OLED_Fonts_Size_t structure where informations will be saved
 * @param  *Font: Pointer to @ref OLED_FontDef_t font used for calculations
 * @retval Pointer to string used for length and height
 */
char* OLED_FONTS_GetStringSize(char* str, OLED_Fonts_Size_t* SizeStruct, OLED_FontDef_t* Font);


#endif
