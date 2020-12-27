/**
  *************************************************************************************************************
   * @file   : STM32F407_OLED_SSD1306_Driver.c
   * @author : Sharath N
   * @brief  : SSD1306 OLED Driver Source File
  *************************************************************************************************************
*/


#include "STM32F407_OLED_SSD1306_Driver.h"


/* Private SSD1306 structure */
typedef struct {
	uint16_t CurrentX;
	uint16_t CurrentY;
	uint8_t Inverted;
	uint8_t Initialized;
} OLED_SSD1306_t;

/* Private Variable */
static OLED_SSD1306_t OLED_SSD1306;

/* I2C Handle  */
I2C_HandleTypeDef myI2Chandle;


/************************************* Private function for I2C initialization *******************************/

/* Configure GPIO  */
static void GPIO_Config(void)
{
	
	/* Enable GPIO Port B Clock*/
	__HAL_RCC_GPIOB_CLK_ENABLE();
	
	/* Init Typdef */
  GPIO_InitTypeDef myPinInit;

  /* I2C Pin Config */
  myPinInit.Pin = GPIO_PIN_6 | GPIO_PIN_7 ;
  myPinInit.Mode = GPIO_MODE_AF_OD;
  myPinInit.Pull = GPIO_PULLUP;
  myPinInit.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  myPinInit.Alternate = GPIO_AF4_I2C1;
  HAL_GPIO_Init(GPIOB, &myPinInit);
	
	/* Systick interrupt enable for HAL_Delay function */
	HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}


/*Configure I2C Peripheral */
static void I2C_Config(void)
{
	//Enable I2C peripheral clock
	__HAL_RCC_I2C1_CLK_ENABLE();
	
	myI2Chandle.Instance = I2C1;
	myI2Chandle.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
	myI2Chandle.Init.ClockSpeed = 400000;
	myI2Chandle.Init.DualAddressMode = I2C_DUALADDRESS_DISABLED;
	myI2Chandle.Init.DutyCycle = I2C_DUTYCYCLE_2;
	myI2Chandle.Init.GeneralCallMode = I2C_GENERALCALL_DISABLED;
	myI2Chandle.Init.NoStretchMode = I2C_NOSTRETCH_DISABLED;
	myI2Chandle.Init.OwnAddress1 = 0;
	myI2Chandle.Init.OwnAddress2 = 0;
	HAL_I2C_Init(&myI2Chandle);
}


/******************************** End of Private functions for I2C initialization ****************************/



/******************************************** OLED SSD1306 APIs **********************************************/

/**
 * @brief Send Command to OLED
 * @param cmd : OLED commands 
 * @retval None
 */
void OLED_SSD1306_Send_Command(uint8_t cmd)
{
	/* control byte to send commands "Co=0 D/C=0 0 0 0 0 0 0 ---> 0x00" 
	Refer 8.1.5.2 Write mode for I2C (Page 20 of OLED SSD1306 Data Sheet*/
	uint8_t control_byte = 0x00;
	
	/* Data buffer to store control and command bytes*/
	uint8_t command_tx[2];
	
	command_tx[0] = control_byte;
	command_tx[1] = cmd;
	
	/* Transmit the command */
	if(HAL_I2C_IsDeviceReady(& myI2Chandle, OLED_I2C_ADDRESS, 1, 10) == HAL_OK)
		HAL_I2C_Master_Transmit(& myI2Chandle,OLED_I2C_ADDRESS, command_tx, 2, 100);
}



/**
 * @brief Send Data to OLED
 * @param cmd : OLED commands 
 * @retval None
 */
void OLED_SSD1306_Send_Data(uint8_t data)
{
  /* control byte to send commands "Co=0 D/C=1 0 0 0 0 0 0 ---> 0x00" 
	Refer 8.1.5.2 Write mode for I2C (Page 20 of OLED SSD1306 Data Sheet*/
	uint8_t control_byte = 0x40;
	
	/* Data buffer to store control and data bytes*/
	uint8_t data_tx[2];
	
	data_tx[0] = control_byte;
	data_tx[1] = data;
	
	/* Transmit the data */
	if(HAL_I2C_IsDeviceReady(& myI2Chandle, OLED_I2C_ADDRESS, 1, 10) == HAL_OK)
		HAL_I2C_Master_Transmit(& myI2Chandle,OLED_I2C_ADDRESS, data_tx, 2, 100);
}


/**
 * @brief  Initializes OLED SSD1306
 * @retval None
 */
void OLED_SSD1306_Init(void)
{
	/* Configure GPIO */
	GPIO_Config();
	 
	/* Configure I2C*/
	I2C_Config();
	
	/* Give a little delay - 100ms*/
	HAL_Delay(100); 
	
	/* Init OLED */
	OLED_SSD1306_Send_Command(OLED_DISPLAY_OFF);            //Display off
	OLED_SSD1306_Send_Command(OLED_SET_MEM_ADDR_MODE);      //Set Memory Addressing Mode
	OLED_SSD1306_Send_Command(OLED_PAGE_ADDR_MODE);         //Select Page Addressing Mode
	OLED_SSD1306_Send_Command(OLED_PAGE_START_ADDR);        //Set Page Start Address for Page Addressing Mode,0-7
	OLED_SSD1306_Send_Command(OLED_COM_SCAN_DIR_REMAPPED);  //Set COM Output Scan Direction
	OLED_SSD1306_Send_Command(OLED_LOW_COLUMN_START_ADDR);  //set low column address
	OLED_SSD1306_Send_Command(OLED_HIGH_COLUMN_START_AADR); //set high  column address
	OLED_SSD1306_Send_Command(OLED_DISP_START_LINE_ADDR);   //set start line address
	OLED_SSD1306_Send_Command(OLED_SET_CONTRAST_CTRL_REG);  //set contrast control register (Next cmd 00 to FF )
	OLED_SSD1306_Send_Command(0xAA);                        //0xFF: 256 Contrast (Max)
	OLED_SSD1306_Send_Command(OLED_SET_SEG_REMAP_127_SEG0); //Set Segment Re-map
  OLED_SSD1306_Send_Command(OLED_SET_NORMAL_DISPLAY);     //set normal display
  OLED_SSD1306_Send_Command(OLED_SET_MULTIPLEX_RATIO);    //set multiplex ratio(1 to 64)
  OLED_SSD1306_Send_Command(0x3F);                        //Multplex ratio - 64MUX 
  OLED_SSD1306_Send_Command(OLED_OUTPUT_FALLOW_RAM_CNT);  //Entire Display on, Output follows RAM content
  OLED_SSD1306_Send_Command(OLED_SET_DISPLAY_OFFSET);     //Set Display offset
  OLED_SSD1306_Send_Command(0x00);                        // 00 - No offset
  OLED_SSD1306_Send_Command(OLED_SET_DIS_CLK_FREQ_RATIO); //set display clock divide ratio/oscillator frequency
  OLED_SSD1306_Send_Command(0xF0);                        //set divide ratio
  OLED_SSD1306_Send_Command(OLED_SET_PRE_CHARGE_PERIOD);  //Set Pre-charge Period
  OLED_SSD1306_Send_Command(0x22);                        //Pre charge Value
  OLED_SSD1306_Send_Command(OLED_SET_COM_PIN_HW_CNF);     //set com pins hardware configuration
 	OLED_SSD1306_Send_Command(0x12);                        //com pin config value
	OLED_SSD1306_Send_Command(OLED_SET_DCOMH_DISEL_LEVEL);  //set vcomh
	OLED_SSD1306_Send_Command(0x20);                        //0x20,0.77xVcc
	OLED_SSD1306_Send_Command(OLED_CHARGE_PUMP_SETTING);    //Charge Pump Setting
	OLED_SSD1306_Send_Command(0x14);                        //Enable Charge Pump
	OLED_SSD1306_Send_Command(OLED_DISPLAY_ON);             //Display ON
	
	/* Clear the screen & Update screen*/
	OLED_SSD1306_Fill(OLED_COLOR_BLACK);
	
	/*Update the Screen */
	OLED_SSD1306_UpdateScreen();

	/* Set default values */
	OLED_SSD1306.CurrentX = 0;
	OLED_SSD1306.CurrentY = 0;
	
	/*Initialized ok */
	OLED_SSD1306.Initialized = 1;
	
}


/**
 * @brief  Fill the OLED SSD1306 Display
 * @param  color:  Set the display balck or color(whichever)
 * @retval None
 */
void OLED_SSD1306_Fill(OLED_COLOR_t color)
{
	/* Set the memory */
	memset(OLED_Buffer, (color == OLED_COLOR_BLACK) ? 0x00 : 0xFF, sizeof(OLED_Buffer));
}


/**
 * @brief  Update the OLED Screen
 * @retval None
 */
void OLED_SSD1306_UpdateScreen(void)
{
	uint8_t m;
	uint8_t control_byte = 0x40;
	uint8_t temp[OLED_WIDTH+1];
  for (m = 0; m < 8 ; m++)
	{
		OLED_SSD1306_Send_Command(OLED_PAGE_START_ADDR + m);
		OLED_SSD1306_Send_Command(OLED_LOW_COLUMN_START_ADDR);
		OLED_SSD1306_Send_Command(OLED_HIGH_COLUMN_START_AADR);
		
		
		/* Fill the data by prepending control byte */
		temp[0] = control_byte;
		uint8_t i;
		for (i=0; i< OLED_WIDTH; i++)
		{
			temp[i+1] = OLED_Buffer[(OLED_WIDTH *m) + i];
		}
	  
		/* Trasnmit the appropriate data in the OLED_buffer */
		if(HAL_I2C_IsDeviceReady(& myI2Chandle, OLED_I2C_ADDRESS, 1, 10) == HAL_OK)
		  HAL_I2C_Master_Transmit(& myI2Chandle, OLED_I2C_ADDRESS, temp, OLED_WIDTH+1,100);
		
	}
}


/**
 * @brief  Draw Pixel
 * @param  x,y: pixel cordinates
 * @param  color: colour value
 * @retval None
 */
void OLED_SSD1306_DrawPixel(uint16_t x, uint16_t y, OLED_COLOR_t color)
{
	if (x >= OLED_WIDTH || y >= OLED_HEIGHT)
	{
		/*error*/
		return;
	}
	
	/* check if pixels are inverted */
	if(OLED_SSD1306.Inverted)
	{
		color = (OLED_COLOR_t)!color;
	}
	
	/* set color */
	if(color == OLED_COLOR_WHITE)
	{
		OLED_Buffer[ x + (y / 8) * OLED_WIDTH ] |= 1 << (y % 8);
	}
	else
	{
		OLED_Buffer[ x + (y / 8) * OLED_WIDTH ] &= ~(1 << (y % 8));
	}
	
}
	


/**
 * @brief  Go to the location (x,y)
 * @param  x,y: pixel cordinates
 * @retval None
 */
void OLED_SSD1306_GotoXY(uint16_t x, uint16_t y)
{
	/* Set the write position */
	OLED_SSD1306.CurrentX = x;
	OLED_SSD1306.CurrentY = y;
}



/**
 * @brief  Puts character on OLED
 * @note   @ref OLED_SSD1306_UpdateScreen() must be called after that in order to see updated LCD screen
 * @param  ch: Character to be written
 * @param  *Font: Pointer to @ref OLED_FontDef_t structure with used font
 * @param  color: Color used for drawing. This parameter can be a value of @ref OLED_COLOR_t enumeration
 * @retval Character written
 */
char OLED_SSD1306_Putc(char ch, OLED_FontDef_t* Font, OLED_COLOR_t color)
{
	uint32_t i, b, j;
	
	/* Check available space in LCD */
	if((OLED_WIDTH <= (OLED_SSD1306.CurrentX)) || ((OLED_HEIGHT <= OLED_SSD1306.CurrentY)))
	{
		return 0;
	}
	
	/* Go through the font data and draw the corresponding pixel to display the char*/
	for (i = 0; i < Font->FontHeight; i++)
	{
		b = Font->data[(ch - 32) * Font->FontHeight + i];
		for (j = 0; j < Font->FontWidth; j++)
		{
			if((b << j) & 0x8000)
			{
				OLED_SSD1306_DrawPixel(OLED_SSD1306.CurrentX +j, OLED_SSD1306.CurrentY + i, (OLED_COLOR_t) color);
			}
			else
			{
				OLED_SSD1306_DrawPixel(OLED_SSD1306.CurrentX +j, OLED_SSD1306.CurrentY + i, (OLED_COLOR_t) !color);
			}
		}
	}
	
	/* Increase the pointer along x- direction*/
	OLED_SSD1306.CurrentX += Font->FontWidth;
	
	//OLED_SSD1306_UpdateScreen();
	/* Return the character written */
	return ch;
	
}


/**
 * @brief  Puts string on OLED
 * @note   @ref OLED_SSD1306_UpdateScreen() must be called after that in order to see updated LCD screen
 * @param  *str: String to be written
 * @param  *Font: Pointer to @ref OLED_FontDef_t structure with used font
 * @param  color: Color used for drawing. This parameter can be a value of @ref OLED_COLOR_t enumeration
 * @retval Zero on success or character value when function failed
 */
char OLED_SSD1306_Puts(char* str, OLED_FontDef_t* Font, OLED_COLOR_t color)
{
	/* Write characters */
	while(*str)
	{
		/*Write Character by character */
		if(OLED_SSD1306_Putc(*str, Font, color) != *str)
		{
			/* Return Error */
			return *str;
		}
    
     /* Increase the string pointer */
     str++;
	}

 /* Everything is ok, return 0*/
 return	*str;
	
}



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
void OLED_SSD1306_DrawLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, OLED_COLOR_t color)
{
	int16_t dx, dy, sx, sy, err, e2, i, tmp; 
	
	/* Check for overflow */
	if (x0 >= OLED_WIDTH) 
	{
		x0 = OLED_WIDTH - 1;
	}
	
	if (x1 >= OLED_WIDTH) 
	{
		x1 = OLED_WIDTH - 1;
	}
	
	if (y0 >= OLED_HEIGHT) 
	{
		y0 = OLED_HEIGHT - 1;
	}
	
	if (y1 >= OLED_HEIGHT) 
	{
		y1 = OLED_HEIGHT - 1;
	}
	
	 dx = (x0 < x1) ? (x1 - x0) : (x0 - x1); 
	 dy = (y0 < y1) ? (y1 - y0) : (y0 - y1); 
	 sx = (x0 < x1) ? 1 : -1; 
	 sy = (y0 < y1) ? 1 : -1; 
	 err = ((dx > dy) ? dx : -dy) / 2; 

	if (dx == 0)
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
		
		/* Vertical line */
		for (i = y0; i <= y1; i++)
		{
			OLED_SSD1306_DrawPixel(x0, i, color);
		}
		
		/* Return from function */
		return;
	}
	
	if (dy == 0) 
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
		
		/* Horizontal line */
		for (i = x0; i <= x1; i++) 
		{
			OLED_SSD1306_DrawPixel(i, y0, color);
		}
		
		/* Return from function */
		return;
	}
	
	while (1) 
	{
		OLED_SSD1306_DrawPixel(x0, y0, color); 
			
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
void OLED_SSD1306_DrawRectangle(uint16_t x, uint16_t y, uint16_t w, uint16_t h, OLED_COLOR_t c)
{
	
	/* Check input parameters */
	if ( x >= OLED_WIDTH || y >= OLED_HEIGHT ) 
	{
		/* Return error */
		return;
	}
	
	/* Check width and height */
	if ((x + w) >= OLED_WIDTH)
	{
		w = OLED_WIDTH - x;
	}
		
	if ((y + h) >= OLED_HEIGHT)
	{
		h = OLED_HEIGHT - y;
	}
	
	/* Draw 4 lines */
	OLED_SSD1306_DrawLine(x, y, x + w, y, c);         /* Top line */
	OLED_SSD1306_DrawLine(x, y + h, x + w, y + h, c); /* Bottom line */
	OLED_SSD1306_DrawLine(x, y, x, y + h, c);         /* Left line */
	OLED_SSD1306_DrawLine(x + w, y, x + w, y + h, c); /* Right line */
	
}


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
void OLED_SSD1306_DrawFilledRectangle(uint16_t x, uint16_t y, uint16_t w, uint16_t h, OLED_COLOR_t c)
{
	uint8_t i;
	
	/* Check input parameters */
	if (x >= OLED_WIDTH || y >= OLED_HEIGHT)
	{
		/* Return error */
		return;
	}
	
	/* Check width and height */
	if ((x + w) >= OLED_WIDTH) {
		w = OLED_WIDTH - x;
	}
	
	if ((y + h) >= OLED_HEIGHT) {
		h = OLED_HEIGHT - y;
	}
	
	/* Draw lines */
	for (i = 0; i <= h; i++) {
		/* Draw lines */
		OLED_SSD1306_DrawLine(x, y + i, x + w, y + i, c);
	}
	
}



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
void OLED_SSD1306_DrawTriangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t x3, uint16_t y3, OLED_COLOR_t color)
{
	/* Draw lines */
	OLED_SSD1306_DrawLine(x1, y1, x2, y2, color);
	OLED_SSD1306_DrawLine(x2, y2, x3, y3, color);
	OLED_SSD1306_DrawLine(x3, y3, x1, y1, color);
}
	


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
void OLED_SSD1306_DrawFilledTriangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t x3, uint16_t y3, OLED_COLOR_t color)
{
	int16_t deltax = 0, deltay = 0, x = 0, y = 0, xinc1 = 0, xinc2 = 0, 
	yinc1 = 0, yinc2 = 0, den = 0, num = 0, numadd = 0, numpixels = 0, 
	curpixel = 0;
	
	deltax = ABS(x2 - x1);
	deltay = ABS(y2 - y1);
	x = x1;
	y = y1;

	if (x2 >= x1) 
	{
		xinc1 = 1;
		xinc2 = 1;
	} 
	else 
	{
		xinc1 = -1;
		xinc2 = -1;
	}

	if (y2 >= y1) 
	{
		yinc1 = 1;
		yinc2 = 1;
	} 
	else 
	{
		yinc1 = -1;
		yinc2 = -1;
	}

	if (deltax >= deltay)
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
		OLED_SSD1306_DrawLine(x, y, x3, y3, color);

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



/**
 * @brief  Draws circle on OLED
 * @note   @ref OLED_SSD1306_UpdateScreen() must be called after that in order to see updated LCD screen
 * @param  x: X location for center of circle. Valid input is 0 to OLED_WIDTH - 1
 * @param  y: Y location for center of circle. Valid input is 0 to OLED_HEIGHT - 1
 * @param  r: Circle radius in units of pixels
 * @param  c: Color to be used. This parameter can be a value of @ref OLED_COLOR_t enumeration
 * @retval None
 */
void OLED_SSD1306_DrawCircle(int16_t x0, int16_t y0, int16_t r, OLED_COLOR_t c)
{
	int16_t f = 1 - r;
	int16_t ddF_x = 1;
	int16_t ddF_y = -2 * r;
	int16_t x = 0;
	int16_t y = r;

  OLED_SSD1306_DrawPixel(x0, y0 + r, c);
  OLED_SSD1306_DrawPixel(x0, y0 - r, c);
  OLED_SSD1306_DrawPixel(x0 + r, y0, c);
  OLED_SSD1306_DrawPixel(x0 - r, y0, c);

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

    OLED_SSD1306_DrawPixel(x0 + x, y0 + y, c);
    OLED_SSD1306_DrawPixel(x0 - x, y0 + y, c);
    OLED_SSD1306_DrawPixel(x0 + x, y0 - y, c);
    OLED_SSD1306_DrawPixel(x0 - x, y0 - y, c);

    OLED_SSD1306_DrawPixel(x0 + y, y0 + x, c);
    OLED_SSD1306_DrawPixel(x0 - y, y0 + x, c);
    OLED_SSD1306_DrawPixel(x0 + y, y0 - x, c);
    OLED_SSD1306_DrawPixel(x0 - y, y0 - x, c);
		
    }
	
}


/**
 * @brief  Draws filled circle on OLED
 * @note   @ref OLED_SSD1306_UpdateScreen() must be called after that in order to see updated LCD screen
 * @param  x: X location for center of circle. Valid input is 0 to OLED_WIDTH - 1
 * @param  y: Y location for center of circle. Valid input is 0 to OLED_HEIGHT - 1
 * @param  r: Circle radius in units of pixels
 * @param  c: Color to be used. This parameter can be a value of @ref OLED_COLOR_t enumeration
 * @retval None
 */
void OLED_SSD1306_DrawFilledCircle(int16_t x0, int16_t y0, int16_t r, OLED_COLOR_t c)
{
	int16_t f = 1 - r;
	int16_t ddF_x = 1;
	int16_t ddF_y = -2 * r;
	int16_t x = 0;
	int16_t y = r;

  OLED_SSD1306_DrawPixel(x0, y0 + r, c);
  OLED_SSD1306_DrawPixel(x0, y0 - r, c);
  OLED_SSD1306_DrawPixel(x0 + r, y0, c);
  OLED_SSD1306_DrawPixel(x0 - r, y0, c);
  OLED_SSD1306_DrawLine(x0 - r, y0, x0 + r, y0, c);

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

     OLED_SSD1306_DrawLine(x0 - x, y0 + y, x0 + x, y0 + y, c);
     OLED_SSD1306_DrawLine(x0 + x, y0 - y, x0 - x, y0 - y, c);

     OLED_SSD1306_DrawLine(x0 + y, y0 + x, x0 - y, y0 + x, c);
     OLED_SSD1306_DrawLine(x0 + y, y0 - x, x0 - y, y0 - x, c);
    }
}
	


/*Systick Handler*/
void SysTick_Handler(void)
{
	HAL_IncTick();
	HAL_SYSTICK_IRQHandler();
}
	

 
		



