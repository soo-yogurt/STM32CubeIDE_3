/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2022 STMicroelectronics.
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
#include "adc.h"
#include "i2c.h"
#include "rtc.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stdio.h"
#include "stdint.h"
#include "string.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
typedef struct {
	int time;
	uint8_t level;
} ClickInfoDef;



typedef struct {
	uint8_t f;
	uint8_t h;
	uint8_t m;
	uint8_t s;
}SETTIME;


/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
//EXTI
#define LONG_CLICK_MIN 700
#define LONG_CLICK_MAX 5000
#define DOUBLE_CLICK_MIN 40
#define DOUBLE_CLICK_MAX 120

//ADC
#define UP_KEY_MIN 0
#define UP_KEY_MAX 15
#define DOWN_KEY_MIN 830
#define DOWN_KEY_MAX 870
#define LEFT_KEY_MIN 1910
#define LEFT_KEY_MAX 1960
#define RIGHT_KEY_MIN 2920
#define RIGHT_KEY_MAX 3010

//LCD
#define LCD_ADDR (0x27 << 1)
#define PIN_RS    (1 << 0)
#define PIN_EN    (1 << 2)
#define BACKLIGHT (1 << 3)

#define LCD_DELAY_MS 5



#define C 956 // 도
#define D 852
#define E 758
#define F 716
#define G 638 // 솔
#define A 568
#define B 506
#define C_ 902 // C#
#define D_ 804
#define F_ 676
#define A_ 536
#define M 0 // mute

// 숫자가 클수록 소리가 작아진다.
#define VOLUME 300
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
ClickInfoDef click[3];
int current_time = 0;
int time_interval = 0;
int last_time = 0;
int alarmMode = 1;
uint8_t longClick = 0;
int ADC_value;
uint8_t rx;
uint8_t flag = 0;
uint8_t mode = 1;
RTC_TimeTypeDef sTime = { 0 };
RTC_DateTypeDef sDate = { 0 };
char buf[25];
char temp[25]; // set Time
char buf2[25]; //  set Alarm
char ampm[2][3] = { "AM", "PM" };
char alarmSet[2][4] = { "ON ", "OFF" };
// ---------------------------------------------------------------------------------
SETTIME st;
SETTIME at;



uint16_t bicycle[] = {E, G, G, M, E, G, G, M, A, A, A, A, A, M}; //, G, G, M, A, A, A, A, A};
uint16_t interval[] = {125, 125, 250, 250, 125, 125, 250, 0, 125, 125, 125, 125, 375, 625};
uint16_t mute[] =    {125, 125, 0, 0, 125, 125, 0, 0, 125, 125, 125, 125, 125, 0};

uint16_t bicycle_2[] = {G, G, G, G, F, F, F, F, E, E, E, E, E, M};
uint16_t interval_2[] = {125, 125, 125, 125, 125, 125, 125, 125, 125, 125, 125, 125, 375, 625};
uint16_t mute_2[] = {125, 125, 125 ,125, 125, 125, 125 ,125, 125, 125, 125 ,125, 125, 0};

uint16_t bicycle_3[] = {E, G, G, G, E, G, G, M, A, A, E, E, G, M};
uint16_t interval_3[] = {125, 125, 125, 125, 125, 125, 250, 250, 200, 75, 125, 125, 375, 625};
uint16_t mute_3[] = {125, 125, 125 ,125, 125, 125 ,0, 0, 125, 50, 75, 125, 125, 125, 0};


uint16_t bicycle_4[] = {F, F, F, F, E, E, E, E, D, D, G, G, C, M};
uint16_t interval_4[] = {125, 125, 125, 125, 125, 125, 125, 125, 125, 125, 125, 125, 375, 625};
uint16_t mute_4[] = {125, 125, 125, 125, 125, 125, 125, 125, 125, 125, 125, 125, 125, 0};

uint8_t bell_length = sizeof(bicycle)/sizeof(uint16_t);
uint8_t bell_length_2 = sizeof(bicycle_2)/sizeof(uint16_t);
uint8_t bell_length_3 = sizeof(bicycle_3)/sizeof(uint16_t);
uint8_t bell_length_4 = sizeof(bicycle_4)/sizeof(uint16_t);


/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_NVIC_Init(void);
/* USER CODE BEGIN PFP */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin);
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim);

//LCD
void I2C_Scan();
void loop();
void init();
void LCD_SendString(uint8_t lcd_addr, char *str);
void LCD_Init(uint8_t lcd_addr);
void LCD_SendCommand(uint8_t lcd_addr, uint8_t cmd);
void LCD_SendData(uint8_t lcd_addr, uint8_t data);
HAL_StatusTypeDef LCD_SendInternal(uint8_t lcd_addr, uint8_t data,
		uint8_t flags);
void AdcSwitch(uint8_t *adc_point, int *location);
void bufferState();
void SetTimeUp(const int *location);
void SaveAlarm();
void SaveSeting();

void BicycleSong();
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
int __io_putchar(int ch) {
	HAL_UART_Transmit(&huart3, (uint8_t*) ch, 1, 100);
	return ch;
}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

	int location = 0;
	uint8_t adc_point = 0;

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
  MX_RTC_Init();
  MX_USART3_UART_Init();
  MX_TIM3_Init();
  MX_ADC1_Init();
  MX_USART2_UART_Init();
  MX_I2C1_Init();
  MX_TIM2_Init();

  /* Initialize interrupts */
  MX_NVIC_Init();
  /* USER CODE BEGIN 2 */


   HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);
	init();
	HAL_TIM_Base_Init(&htim3);
	HAL_TIM_Base_Start_IT(&htim3);

	at.f = 0;
	at.h = 12;
	at.m = 30;
	at.s = 30;


	sTime.Hours = 12;
	sTime.Minutes = 0;
	sTime.Seconds = 0;
	sTime.TimeFormat = 0;

	sDate.Year = 22;
	sDate.Month = 6;
	sDate.Date = 20;

	HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BIN);
	HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BIN);
	memset(buf, 0, sizeof(buf));
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	while (1) {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
		char alarmOnOff[2][2] = {"A", " "};
		char flag1buf[25];
		//clock
		if (flag == 0) {
			//********************** cursor ****************************************
			bufferState();
			//********************* Display ** LINE 1 ******************************
			sprintf(flag1buf, " %s   LCD Clock  ", alarmOnOff[alarmMode]);
			LCD_SendCommand(LCD_ADDR, 0b10000000);
			LCD_SendString(LCD_ADDR, flag1buf);
			//********************* Display ** LINE 2 ******************************
			location = 0;
			while (flag == 0) {
				HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BIN);
				HAL_RTC_GetDate(&hrtc, &sDate, RTC_FORMAT_BIN);
				HAL_UART_Transmit(&huart3, (uint8_t*) buf, sizeof(buf), 2000);
				sprintf(buf, "%s %02d:%02d:%02d     ", ampm[sTime.TimeFormat],
						sTime.Hours, sTime.Minutes, sTime.Seconds);
				printf("\r\n");
				LCD_SendCommand(LCD_ADDR, 0b11000000);
				LCD_SendString(LCD_ADDR, buf);
			//**********************************************************************
			}
		}
		//set Time
		else if (flag == 1) {
			//********************* Display ** LINE 1 ******************************
			sprintf(flag1buf, " %s   Set Time   ", alarmOnOff[alarmMode]);
			LCD_SendCommand(LCD_ADDR, 0b10000000);
			LCD_SendString(LCD_ADDR, flag1buf);
			//********************* Display ** LINE 2 ******************************
			sprintf(buf, "%s %02d:%02d:%02d     ", ampm[sTime.TimeFormat],
									sTime.Hours, sTime.Minutes, sTime.Seconds);
			LCD_SendCommand(LCD_ADDR, 0b11000000);
			LCD_SendString(LCD_ADDR, buf);
			//********************** cursor ****************************************
			bufferState();
			location = 0;
			//***********************st 초기화 *******************************************
			st.f = sTime.TimeFormat;
			st.h = sTime.Hours;
			st.m = sTime.Minutes;
			st.s = sTime.Seconds;
			//**********************************************************************
			longClick = 0;
			while (flag == 1) {
				AdcSwitch(&adc_point, &location);
				SaveSeting();
			}
			//**********************************************************************
		}
		//alarm
		else if (flag == 2) {
			//********************* Display ** LINE 1 ******************************
			sprintf(flag1buf, " %s   alarm      ", alarmOnOff[alarmMode]);
			LCD_SendCommand(LCD_ADDR, 0b10000000);
			LCD_SendString(LCD_ADDR, flag1buf);
			//********************* Display ** LINE 2 ******************************
			sprintf(buf2, "%s %02d:%02d:%02d %s ", ampm[at.f],
									at.h, at.m, at.s, alarmSet[alarmMode]);
			LCD_SendCommand(LCD_ADDR, 0b11000000);
			LCD_SendString(LCD_ADDR, buf2);
			//********************** cursor ****************************************
			bufferState();
			location = 0;
			//***********************st 초기화 *******************************************
			st.f = at.f;
			st.h = at.h;
			st.m = at.m;
			st.s = at.s;
			//**********************************************************************
			longClick = 0;
			while (flag == 2) {
				AdcSwitch(&adc_point, &location);
				SaveAlarm();
			}
			//**********************************************************************
		} else if (flag > 2)
			flag = 0;
	}
  /* USER CODE END 3 */
}

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
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_LSE;
  RCC_OscInitStruct.LSEState = RCC_LSE_ON;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 180;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Activate the Over-Drive mode
  */
  if (HAL_PWREx_EnableOverDrive() != HAL_OK)
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

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief NVIC Configuration.
  * @retval None
  */
static void MX_NVIC_Init(void)
{
  /* USART3_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(USART3_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(USART3_IRQn);
  /* EXTI15_10_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(EXTI15_10_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
}

/* USER CODE BEGIN 4 */
void bufferState() {
	if (flag == 2 || flag == 1) {
		LCD_SendCommand(LCD_ADDR, 0b11000000);
		LCD_SendCommand(LCD_ADDR, 0b00001111);
	} else
		LCD_SendCommand(LCD_ADDR, 0b00001110);
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
	GPIO_PinState pin;

	if (GPIO_Pin == GPIO_PIN_13) {
		current_time = HAL_GetTick();
		time_interval = current_time - last_time;
		last_time = current_time;

		pin = HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13);

		if (time_interval <= 4) {
			printf("Noise %d, %d\r\n", pin, time_interval);
		} else {

			click[1].time = click[0].time;
			click[1].level = click[0].level;

			click[0].time = time_interval;
			click[0].level = pin;
		}
		if (click[0].level == GPIO_PIN_RESET && click[0].time >= LONG_CLICK_MIN) // long click
		{
			printf("\r\nLong Key\r\n");
			longClick = 1;

		} else if (click[0].level == GPIO_PIN_RESET
				&& click[1].level == GPIO_PIN_SET) {
			printf("\r\nSelect Key, %d\r\n", click[0].time);
			flag++;
		}
	}
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
	if (htim->Instance == TIM3) {
		HAL_ADC_Start(&hadc1);
		ADC_value = HAL_ADC_GetValue(&hadc1);
		//	printf("ADC_value = %d\r\n", ADC_value);

	}
}

void I2C_Scan() {
	char info[] = "Scanning I2C bus...\r\n";
	HAL_UART_Transmit(&huart2, (uint8_t*) info, strlen(info), HAL_MAX_DELAY);

	HAL_StatusTypeDef res;
	for (uint16_t i = 0; i < 128; i++) {
		res = HAL_I2C_IsDeviceReady(&hi2c1, i << 1, 1, 10);
		if (res == HAL_OK) {
			char msg[64];
			snprintf(msg, sizeof(msg), "0x%02X", i);
			HAL_UART_Transmit(&huart2, (uint8_t*) msg, strlen(msg),
			HAL_MAX_DELAY);
		} else {
			HAL_UART_Transmit(&huart2, (uint8_t*) ".", 1, HAL_MAX_DELAY);
		}
	}

	HAL_UART_Transmit(&huart2, (uint8_t*) "\r\n", 2, HAL_MAX_DELAY);
}

HAL_StatusTypeDef LCD_SendInternal(uint8_t lcd_addr, uint8_t data,
		uint8_t flags) {
	HAL_StatusTypeDef res;
	for (;;) {
		res = HAL_I2C_IsDeviceReady(&hi2c1, lcd_addr, 1, HAL_MAX_DELAY);
		if (res == HAL_OK)
			break;
	}

	uint8_t up = data & 0xF0;
	uint8_t lo = (data << 4) & 0xF0;

	uint8_t data_arr[4];
	data_arr[0] = up | flags | BACKLIGHT | PIN_EN;
	data_arr[1] = up | flags | BACKLIGHT;
	data_arr[2] = lo | flags | BACKLIGHT | PIN_EN;
	data_arr[3] = lo | flags | BACKLIGHT;

	res = HAL_I2C_Master_Transmit(&hi2c1, lcd_addr, data_arr, sizeof(data_arr),
	HAL_MAX_DELAY);
	HAL_Delay(LCD_DELAY_MS);
	return res;
}

void LCD_SendCommand(uint8_t lcd_addr, uint8_t cmd) {
	LCD_SendInternal(lcd_addr, cmd, 0);
}

void LCD_SendData(uint8_t lcd_addr, uint8_t data) {
	LCD_SendInternal(lcd_addr, data, PIN_RS);
}

void LCD_Init(uint8_t lcd_addr) {
	// 4-bit mode, 2 lines, 5x7 format
	LCD_SendCommand(lcd_addr, 0b00110000);
	// display & cursor home (keep this!)
	LCD_SendCommand(lcd_addr, 0b00000010);
	// display on, right shift, underline off, blink off
	LCD_SendCommand(lcd_addr, 0b00001100);
	// clear display (optional here)
	LCD_SendCommand(lcd_addr, 0b00000001);
}

void LCD_SendString(uint8_t lcd_addr, char *str) {
	while (*str) {
		LCD_SendData(lcd_addr, (uint8_t) (*str));
		str++;
	}
}

void init() {
	I2C_Scan();
	LCD_Init(LCD_ADDR);
}

void loop() {
	HAL_Delay(100);
}
void SaveAlarm()
{
	/***************** Save 기능 **************************/
	if(longClick == 1)
	{
		at.f = st.f;
		at.h = st.h;
		at.m = st.m;
		at.s = st.s;
		longClick = 0;
		flag = 0;
	}
}

void SaveSeting()
{

	/***************** Save 기능 **************************/
	if(longClick == 1)
	{
		sTime.TimeFormat = st.f;
		sTime.Hours = st.h;
		sTime.Minutes = st.m;
		sTime.Seconds = st.s;

		HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BIN);
		HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BIN);

		longClick = 0;
		flag = 0;
	}
}
void SetTimeDown(const int *location)
{
	if(*location == 0){
		if(st.f == 0)
			st.f = 1;
		else if(st.f == 1)
			st.f = 0;
	}
	else if(*location == 4){
		if(st.h == 0)
			st.h = 12;
		else
			st.h -= 1;
	}
	else if(*location == 6){
		if(st.m < 10)
			st.m += 50;
		else
			st.m -= 10;
	}
	else if(*location == 7){
		if(st.m % 10 == 0)
			st.m += 9;
		else
			st.m--;
	}
	else if(*location == 9){
		if(st.s < 10)
			st.s += 50;
		else
			st.s -=10;
	}else if(*location == 10){
		if(st.s % 10 == 0)
			st.s += 9;
		else
			st.s--;
	} else if (*location == 12 && flag == 2)
		alarmMode ^= 1;

	char format[3];
	if(st.f == 0)
		strcpy(format, "AM");
	else if(st.f == 1)
		strcpy(format, "PM");
	/****************** Display *************************************/
	if(flag == 1)
		sprintf(temp, "%s %02d:%02d:%02d     ", format, st.h, st.m, st.s);
	else if(flag == 2)
		sprintf(temp, "%s %02d:%02d:%02d %s  ", format, st.h, st.m, st.s, alarmSet[alarmMode]);
	LCD_SendCommand(LCD_ADDR, 0b11000000);
	LCD_SendString(LCD_ADDR, temp);


	/***************** SetTime 후 커서 되돌리기 **************************/
	LCD_SendCommand(LCD_ADDR, 0b11000000);
	for(int j = 0; j < *location; j++)
	{
		LCD_SendCommand(LCD_ADDR, 0b00010100);
	}

}

void SetTimeUp(const int *location)
{
	if(*location == 3) {}
	else if(*location == 4){
		if(st.h == 12)
			st.h = 0;
		else
			st.h += 1;
	} else if(*location == 0){
		if(st.f == 0)
			st.f = 1;
		else if(st.f == 1)
			st.f = 0;
	}
	else if(*location == 6){
		if(st.m >= 50)
			st.m -= 50;
		else if(st.m < 50) {
			st.m += 10;
		}
	} else if (*location == 7){
		if(st.m % 10 == 9)
			st.m -= 9;
		else
			st.m++;
	} else if (*location == 9){
		if(st.s >= 50)
			st.s -=50;
		else if(st.s < 50)
			st.s +=10;
	} else if (*location == 10){
		if(st.s % 10 == 9)
			st.s -=9;
		else
			st.s++;
	} else if (*location == 12 && flag == 2)
		alarmMode ^= 1;
	char format[3];
	if(st.f == 0)
		strcpy(format, "AM");
	else if(st.f == 1)
		strcpy(format, "PM");
	/****************** Display *************************************/
	if(flag == 1)
		sprintf(temp, "%s %02d:%02d:%02d Save", format, st.h, st.m, st.s);
	else if(flag == 2)
		sprintf(temp, "%s %02d:%02d:%02d %s  ", format, st.h, st.m, st.s, alarmSet[alarmMode]);
	LCD_SendCommand(LCD_ADDR, 0b11000000);
	LCD_SendString(LCD_ADDR, temp);


	/***************** SetTime 후 커서 되돌리기 **************************/
	LCD_SendCommand(LCD_ADDR, 0b11000000);
	for(int j = 0; j < *location; j++)
	{
		LCD_SendCommand(LCD_ADDR, 0b00010100);
	}
}


void AdcSwitch(uint8_t *adc_point, int *location)
{
	//***************** UP *************************************************
	if ((ADC_value <= UP_KEY_MAX) && (*adc_point != 1)){
		*adc_point = 1;
		printf("ADC_value = %d\r\n", ADC_value);
		SetTimeUp(location);
	}
	//***************** DOWN ***********************************************
	if ((ADC_value >= DOWN_KEY_MIN && ADC_value <= DOWN_KEY_MAX)
			&& (*adc_point != 2)) {
		*adc_point = 2;
		printf("ADC_value = %d\r\n", ADC_value);
		SetTimeDown(location);
	}
	//****************** LEFT **********************************************
	if ((ADC_value >= LEFT_KEY_MIN && ADC_value <= LEFT_KEY_MAX)
			&& (*adc_point != 3)) {
		printf("ADC_value = %d\r\n", ADC_value);
		*adc_point = 3;
	//****************** LEFT **********************************************
		if(*location <= 0){
			for(int r = 0; r < 12; r++){
				LCD_SendCommand(LCD_ADDR, 0b00010100);
			}

		*location = 12;
		}

		else
		{
			if(*location == 4)
			{
				LCD_SendCommand(LCD_ADDR, 0b00010000);
				LCD_SendCommand(LCD_ADDR, 0b00010000);
				LCD_SendCommand(LCD_ADDR, 0b00010000);
				(*location) -= 3;
			}
			if(*location == 6 || *location == 9 || *location == 12)
			{
				LCD_SendCommand(LCD_ADDR, 0b00010000);
				(*location) -= 1;
			}
			LCD_SendCommand(LCD_ADDR, 0b00010000);
			(*location)--;
		}
	}
	//***************** RIGHT **********************************************
	if ((ADC_value >= RIGHT_KEY_MIN && ADC_value <= RIGHT_KEY_MAX)
			&& (*adc_point != 4)) {
		printf("ADC_value = %d\r\n", ADC_value);
		*adc_point = 4;
	//***************** RIGHT **********************************************
		if (*location >= 12){
			for(int l = 12; l > 0; l--){
				LCD_SendCommand(LCD_ADDR, 0b00010000);
			}
			*location = 0;
		}
		else
		{
			if(*location == 0)
			{
				LCD_SendCommand(LCD_ADDR, 0b00010100);
				LCD_SendCommand(LCD_ADDR, 0b00010100);
				LCD_SendCommand(LCD_ADDR, 0b00010100);
				(*location) += 3;
			}
			if(*location == 4 || *location == 7 || *location == 10)
			{
				LCD_SendCommand(LCD_ADDR, 0b00010100);
				(*location) += 1;
			}
			LCD_SendCommand(LCD_ADDR, 0b00010100);
			(*location)++;
		}
	}
	if (ADC_value > RIGHT_KEY_MAX && *adc_point != 0) {
		*adc_point = 0;
	}
	//**********************************************************************

}

void BicycleSong()
{
	 for(int i = 0; i < bell_length; i++){
		  TIM2 -> ARR = bicycle[i];
		  TIM2 -> CCR1 = TIM2 -> ARR / VOLUME;
		  HAL_Delay(interval[i]);
		  TIM2 -> CCR1 = 0;
		  HAL_Delay(mute[i]);
	  }
	  for(int i = 0; i < bell_length_2; i++){
		  TIM2 -> ARR = bicycle_2[i];
		  TIM2 -> CCR1 = TIM2 -> ARR / VOLUME;
		  HAL_Delay(interval_2[i]);
		  TIM2 -> CCR1 = 0;
		  HAL_Delay(mute_2[i]);
	  }
	  for(int i = 0; i < bell_length_3; i++){
		  TIM2 -> ARR = bicycle_3[i];
		  TIM2 -> CCR1 = TIM2 -> ARR / VOLUME;
		  HAL_Delay(interval_3[i]);
		  TIM2 -> CCR1 = 0;
		  HAL_Delay(mute_3[i]);
	  }
	  for(int i = 0; i < bell_length_4; i++){
		  TIM2 -> ARR = bicycle_4[i];
		  TIM2 -> CCR1 = TIM2 -> ARR / VOLUME;
		  HAL_Delay(interval_4[i]);
		  TIM2 -> CCR1 = 0;
		  HAL_Delay(mute_4[i]);
	  }
}
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
	while (1) {
	}
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
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
