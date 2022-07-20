#ifndef __USER_LCD_
#define __USER_LCD_
#include "main.h"
#include "usart.h"
#include "i2c.h"

#define LCD_ADDR (0x27 << 1)
#define PIN_RS    (1 << 0)
#define PIN_EN    (1 << 2)
#define BACKLIGHT (1 << 3)
#define LCD_DELAY_MS 5


void init();
void I2C_Scan();
void LCD_Init(uint8_t lcd_addr);
void loop();

void LCD_SendString(uint8_t lcd_addr, char *str);

void LCD_SendCommand(uint8_t lcd_addr, uint8_t cmd);
void LCD_SendData(uint8_t lcd_addr, uint8_t data);
HAL_StatusTypeDef LCD_SendInternal(uint8_t lcd_addr, uint8_t data, uint8_t flags);


// USER CODE BEGIN
void bufferState();
// USER CODE END

#endif /* __USER_LCD_ */
