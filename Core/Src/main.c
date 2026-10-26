/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

#include <stdio.h>
#include "LCD_driver.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

I2C_HandleTypeDef hi2c1;
uint8_t data=0x00;
HAL_StatusTypeDef status1;
uint8_t status_no=1; // led ON
// HAL_StatusTypeDef status1;
char *char1="B";
//char str1[2];
//char kr='h';
extern uint8_t o_umlaut[8];

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C1_Init(void);
/* USER CODE BEGIN PFP */

HAL_StatusTypeDef value5;

// extern uint8_t deneme;
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

	/* USER CODE BEGIN 1 */

	//str1[0] = kr;   // Karakteri aktar
	//str1[1] = '\0'; // Metin sonlandırıcısını ekle

	int sayi = 12;
	char metin[20]; // Sayıyı ve sonundaki '\0' karakterini tutabilecek büyüklükte bir dizi

	// int -> char dizisi dönüştürme
	//sprintf(metin, "%d", sayi);

	/* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

	/* USER CODE BEGIN SysInit */

	/* USER CODE END SysInit */

	/* Initialize all configured peripherals */
	MX_GPIO_Init();
	MX_I2C1_Init();
	/* USER CODE BEGIN 2 */
	value5=LCD_Test();
	//value5=deneme();

	LCD_Init();
	led_init();
	LCD_CreateChar(0, o_umlaut);  // 0 numaralı slota 'ö' harfini kaydet

	LCD_Set_Cursor(0, 2);
	//LCD_Send_Data(49); // Ascii tablosuna göre 1 değerini verir
	LCD_Send_Data(0);  //0 numaralı slotta bulunan 'ö' harfini yazdırır.
	//LCD_Print(metin);
	//LCD_Print(char1);

	/*
	if (value5 == HAL_OK) {
		for (uint8_t var = 0; var < 5; ++var) {
			sayi = var;
			sprintf(metin, "%d", sayi);
			LCD_Set_Cursor(0, 2);
			LCD_Print(metin);
			HAL_Delay(1000);
			LCD_Send_Cmd(0x01);
		}
	}
	*/
	/* USER CODE END 2 */

	/* Infinite loop */
	/* USER CODE BEGIN WHILE */
	while (1) {

		if ((GPIOA->IDR & (GPIO_IDR_ID0)) && status_no == 1) {

			GPIOD->BSRR |= GPIO_BSRR_BS_15;

			//sayi = status_no;
			//sprintf(metin, "%d", sayi);
			LCD_Send_Cmd(0x01);
			LCD_Set_Cursor(0, 2);
			//LCD_Print(metin);
			LCD_Print("ON");
			HAL_Delay(200);
			status_no = 0;


		} else if ((GPIOA->IDR & (GPIO_IDR_ID0)) && status_no == 0) {

			GPIOD->BSRR |= GPIO_BSRR_BR15;

			//sayi = status_no;
			//sprintf(metin, "%d", sayi);
			//LCD_Set_Cursor(0, 2);
			//LCD_Print(metin);
			LCD_Send_Cmd(0x01);
			LCD_Set_Cursor(0, 2);
			LCD_Print("OFF");
			HAL_Delay(200);
			status_no = 1;


		}

		/*
		 LCD_Set_Cursor(0, 2);
		 HAL_Delay(100);
		 LCD_Print("M");
		 HAL_Delay(100);
		 LCD_Set_Cursor(0, 4);
		 HAL_Delay(100);
		 LCD_Print(str1);
		 HAL_Delay(100);
		 */

		/* USER CODE END WHILE */

		/* USER CODE BEGIN 3 */
	}
	/* USER CODE END 3 */
}

/*
 void LCD_Test(void){
 status1= HAL_I2C_IsDeviceReady(&hi2c1, LCD_ADDR, 4, 100);
 if(status1==HAL_OK){
 status1=HAL_OK;
 }else{
		status1=HAL_ERROR;
	}

}

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

*/

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 192;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
  RCC_OscInitStruct.PLL.PLLQ = 8;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 100000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  /* USER CODE BEGIN MX_GPIO_Init_1 */

  /* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /* USER CODE BEGIN MX_GPIO_Init_2 */

  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}
#ifdef USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
