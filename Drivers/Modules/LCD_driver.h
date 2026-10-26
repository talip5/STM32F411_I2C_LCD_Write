/*
 * LCD_driver.h
 *
 *  Created on: Oct 15, 2026
 *      Author: efe
 */

#ifndef MODULES_LCD_DRIVER_H_
#define MODULES_LCD_DRIVER_H_

void led_init(void);
HAL_StatusTypeDef LCD_Test(void);
uint8_t deneme(void);
void PCF8574_Write(uint8_t data);
void LCD_Toggle_Enable(uint8_t data);
void LCD_Send(uint8_t value,uint8_t mode);
void LCD_Send_Cmd(uint8_t cmd);
void LCD_Send_Data(uint8_t data);
void LCD_Init(void);
void LCD_Print(char *str);
void LCD_Set_Cursor(uint8_t row, uint8_t col);
void LCD_CreateChar(uint8_t location, uint8_t *charmap);

#endif /* MODULES_LCD_DRIVER_H_ */
