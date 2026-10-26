/*
 * LCD_driver.c
 *
 *  Created on: Oct 15, 2026
 *      Author: efe
 */

#include "stm32f4xx_hal.h"

#define LCD_ADDR (0X27<<1)
#define PIN_RS (1<<0) // P0->RS
#define PIN_EN (1<<2) // P2->E
#define BACKLIGHT_ON (1<<3) // P3->Arka Işık Kontrolu

extern HAL_StatusTypeDef status1;
extern I2C_HandleTypeDef hi2c1;

//HAL_StatusTypeDef LCD_Test(void);

void led_init(void){

	RCC->AHB1ENR |=RCC_AHB1ENR_GPIOAEN;
	RCC->AHB1ENR |=RCC_AHB1ENR_GPIODEN;

	GPIOA->MODER &=~(GPIO_MODER_MODE0_0);
	GPIOA->MODER &=~(GPIO_MODER_MODE0_1);

	GPIOA->PUPDR &=~(GPIO_PUPDR_PUPD0_0);
	GPIOA->PUPDR &=~(GPIO_PUPDR_PUPD0_1);

	GPIOD->MODER |=GPIO_MODER_MODE15_0;
	GPIOD->MODER &=~(GPIO_MODER_MODE15_1);

	GPIOD->OTYPER &=~(GPIO_OTYPER_OT15);

	GPIOD->OSPEEDR &=~(GPIO_OSPEEDER_OSPEEDR15_0);
	GPIOD->OSPEEDR &=~(GPIO_OSPEEDER_OSPEEDR15_1);

	GPIOD->PUPDR &=~(GPIO_PUPDR_PUPD15_0);
	GPIOD->PUPDR &=~(GPIO_PUPDR_PUPD15_1);

}



uint8_t deneme(void){
	return 1;
}

HAL_StatusTypeDef LCD_Test(void){
	status1= HAL_I2C_IsDeviceReady(&hi2c1, LCD_ADDR, 4, 100);
	return status1;

}

/*
uint8_t LCD_Test(void){
	status1= HAL_I2C_IsDeviceReady(&hi2c1, LCD_ADDR, 4, 100);
	return 5;

}
*/

//PCF8574'e 1 byte veri gönderen yardıncı fonksiyon
void PCF8574_Write(uint8_t data){
	uint8_t i2c_data=data | BACKLIGHT_ON; //Arka ışığı sürekli açık tutuyoruz
	HAL_I2C_Master_Transmit(&hi2c1, LCD_ADDR, &i2c_data, 1, 100);
}

void LCD_Toggle_Enable(uint8_t data){
	PCF8574_Write(data | PIN_EN );  // E=1
	HAL_Delay(1);
	PCF8574_Write(data & ~PIN_EN);  // E=0;
	HAL_Delay(1);
}

void LCD_Send(uint8_t value,uint8_t mode){
	// 1. Aşama : Yüksek 4 bit(Upper Nibble) gönderimi
	uint8_t high_nibble=(value & 0xF0) | mode;
	LCD_Toggle_Enable(high_nibble);

	// 2. Aşama : Düşük 4 bit (Lower Nibble) gönderimi
	uint8_t low_nibble=((value<<4) & 0xF0) | mode;
	LCD_Toggle_Enable(low_nibble);
}

// Kolay kullanım için sarnalayıcı fonksiyonlar

void LCD_Send_Cmd(uint8_t cmd){
	LCD_Send(cmd, 0); //RS=0 Komut modu
}

void LCD_Send_Data(uint8_t data){
	LCD_Send(data, PIN_RS);  //RS=PIN_RS veri modu
}


void LCD_Init(void){

	HAL_Delay(50);  // Ekranın kendine gelmesi için güç açılış beklemesi
	// 4 bit moduna geçiş sekansı (HD44780 dökümanına göre)
	LCD_Send_Cmd(0x03);
	HAL_Delay(5);
	LCD_Send_Cmd(0x03);
	HAL_Delay(1);
	LCD_Send_Cmd(0x03);
	HAL_Delay(10);
	LCD_Send_Cmd(0x02);  // Tam olarak 4 bit moduna aktif et

	//Ekran konfigürasyonu
	LCD_Send_Cmd(0x28);  // 2 Satır, 5x8 font , 4 bit arabirimi
	LCD_Send_Cmd(0x0C);  // Ekranı açık, imleç kapalı, kırpışma kapalı
	LCD_Send_Cmd(0x06);  // Giriş modu: Yazıldıkça imleci sağa kaydır
	LCD_Send_Cmd(0x01);  // Ekranı temizle (Clear Display)
	HAL_Delay(2);

}

// Ekrana string (yazı) yazdırma fonksiyonu
void LCD_Print(char *str){
	while(*str){
		LCD_Send_Data((uint8_t)*str);
		str++;
	}
}


//İmleç konumlandırma fonksiyonu
void LCD_Set_Cursor(uint8_t row, uint8_t col){
	uint8_t address=(row==0)?(0x80+col):(0xC0+col);
			LCD_Send_Cmd(address);
}

// Ö harfi piksel matrisi dizisi

uint8_t o_umlaut[8]={
		0x0A, // * *
		0x00, //
		0x0E, // ***
		0x11, // * *
		0x11, // * *
		0x11, // * *
		0x0E, // ***
		0x00 //
		};

void LCD_CreateChar(uint8_t location, uint8_t *charmap){
	location &=0x07; // 0 ile 7 arasında sınırla(8 slot var)

	//CGRAM adresini ayarla (0x40 taban adresidir)
	LCD_Send_Cmd(0x40 | (location<<3));
	// 8 baytlık piksel verisini sırayla gönder (RS=1 , veri modu )

	for (int i = 0; i < 8; ++i) {
		LCD_Send_Data(charmap[i]);
	}

	//LCD_CreateChar(0, o_umlaut);  // 0 numaralı slota 'ö' harfini kaydet

	// Ekrana yazdırmak istediğinizde 0 sayısını göndermeniz yeterlidir.
	// LCD_Send_Data(0);
}

