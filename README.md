# Interfacing 128 x 64 Dot Matrix OLED Display (SSD1306) on STM32F407 Discovery using I2C Protocol.
This repository tells you how to interface a 128 x 64 Dot Matrix OLED Display on STM32F4xx MCU (I am using **[STM32F407 Discovery Kit](https://github.com/SharathN25/STM32F407-Discovery)**) using  I2C Protocol. Here 4 wires are used for interfacing the OLED display Ground(GND), VCC(3.3V), Serial Data Line (PB7), and Serial Clock line (PB6). The code is written very systematically and it's easy to understand. This driver can be ported for other microcontrollers by simply replacing few functions with MCU-specific APIs. An example project using Kiel µVision 5 is also included. 

Code is written without using STM32CubeMX tool, however you can also use CubeMX to generat I2C and MCU basic initialization code.

## Features of OLED(SSD1306) Display Driver
This driver provides following functionalities.

1. Send Command
2. Send Data
3. Fill the Screen
4. Draw a Pixel
5. Put a Character
6. Put a String
7. Draw the Line
8. Draw Rectangle & Filled Rectangle
9. Draw Triangle & Filled Triangle
10. Draw Circle & Filled Circle

Only the following functions in the driver contains the STM23F407 MCU Specific code for initializing the Microcontroller and SPI Peripheral and these can be replaced by other MCU specific codes for porting.

1. **static void GPIO_Config(void)** - Configure the GPIO
2. **static void I2C_Config(void)** - Configure I2C Peripheral
3. **void OLED_SSD1306_Send_Command(uint8_t cmd)** - Send Command to OLED
4. **void OLED_SSD1306_Send_Data(uint8_t data)** - Send Data to OLED
5. **HAL_Delay() and void SysTick_Handler(void)** - For time delay (from HAL library).

## Quick References
* **[Setting up I2C on STM32F407](https://www.youtube.com/watch?v=1COFk1M2tak)**
* **[128 x 64 Dot Matrix OLED Driver SSD1306](https://cdn-shop.adafruit.com/datasheets/SSD1306.pdf)**


