/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
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
#include "dma.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "oled.h"
#include "string.h"
#include "stdio.h"
#include "motor.h"
#include "DHT11.h"
#include "esp8266.h"

uint8_t data_t=30,data_r=3,data_h=50;
float data_l = 0.3;
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
static void ui_setting(void);
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

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
	static int i_count = 0;
  float adcy,adcy1;
	uint16_t adcx = 0,adcx1 = 0;
	char data_light[20]={0},data_light1[20]={0};
	char show_data[30];
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
  MX_DMA_Init();
  MX_ADC1_Init();
  MX_ADC2_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */
	OLED_Init();
	OLED_ColorTurn(0);
  OLED_DisplayTurn(0);
	OLED_Clear();
	start_esp8266();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
		handle_esp8266();
		HAL_ADC_Start(&hadc1);   
		HAL_ADC_PollForConversion(&hadc1,10); 
		adcx = (uint16_t)HAL_ADC_GetValue(&hadc1);  
		adcy = (float)adcx*3.3/4096;             
		sprintf(data_light,"Light:%.3f",adcy);
		OLED_ShowString(0,0,(uint8_t*)data_light,16,1);
			OLED_Refresh();
		
				HAL_ADC_Start(&hadc2);   
		HAL_ADC_PollForConversion(&hadc2,10); 
		adcx1 = (uint16_t)HAL_ADC_GetValue(&hadc2);  
		adcy1 = (float)adcx1*3.3/4096;              //guangzhao 
		sprintf(data_light1,"Rain:%.3f",adcy1);
		OLED_ShowString(0,20,(uint8_t*)data_light1,16,1);
		OLED_Refresh();
  
		i_count++;
		if(i_count/50)
		{
			i_count=0;
			sprintf(show_data,"light:%.3f Rain:%.3f\r\n",adcy,adcy1);
			send_wifi(show_data,27); 
		}

		DHT11();
		
		if(adcy<data_l)  //光照太强
		{
				direction = 0; //Turn to loosen the soil in a positive direction
				for(int i=0;i<(motor_angle_cal(90))/8;i++)
				{
					for(uint8_t step=0;step<8;step++)
					{	
							motor_controld(step,direction);
							HAL_Delay(1);
					}
				}				
		}
		
		if(adcy1<data_r)
		{
								direction = 1; //Turn to loosen the soil in a positive direction
				for(int i=0;i<(motor_angle_cal(90))/8;i++)
				{
					for(uint8_t step=0;step<8;step++)
					{	
							motor_controld(step,direction);
							HAL_Delay(1);
					}
				}			
		}
		
		
		if(botton==MIDLE)  
		{
			  botton = UNPRESS;
			  OLED_Clear();
				ui_setting();			   
		}
		
		if(botton==LEFT)
		{
			  botton = UNPRESS;
				HAL_GPIO_TogglePin(GPIOB, LED1_Pin);
		}
		
		if(botton==RIGHT)
		{
			  botton = UNPRESS;
				HAL_GPIO_TogglePin(GPIOB, LED2_Pin);
		}
//						direction = 0; //Turn to loosen the soil in a positive direction
//				for(int i=0;i<(motor_angle_cal(90))/8;i++)
//				{
//					for(uint8_t step=0;step<8;step++)
//					{	
//							motor_controld(step,direction);
//							HAL_Delay(1);
//					}
//				}
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
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;
  PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV6;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
static void ui_setting(void)
{
	  char show_data[10],show_data1[10],show_data2[10],show_data3[10];
	  int position = 0;
		OLED_ShowString(30,0,(uint8_t*)"Setting",8,1);
		OLED_Refresh();
	  while(1)
		{
				sprintf(show_data,"Light:%0.2f",data_l);
				OLED_ShowString(0,10,(uint8_t*)show_data,8,position==0?0:1);  //关照
			
			  sprintf(show_data1,"humidity:%02d",data_h);
			  OLED_ShowString(0,20,(uint8_t*)show_data1,8,position==1?0:1); //湿度
			
			  sprintf(show_data2,"humidity:%02d",data_t);
				OLED_ShowString(0,30,(uint8_t*)show_data2,8,position==2?0:1);//温度
			
			  sprintf(show_data3,"Rain:%02d",data_r);
				OLED_ShowString(0,40,(uint8_t*)show_data3,8,position==3?0:1);    //下雨
				OLED_ShowString(0,50,(uint8_t*)"Back",8,position==4?0:1);
				OLED_Refresh();
			
			 if(botton==MIDLE)
			 {
					 botton = UNPRESS;
				   position++;
				   if(position>4)
						 position=0;
			 }
			 
			 if(botton==LEFT)
			 {
				   botton = UNPRESS;
					 switch(position)
					 {	
						 case 0:  //光照
								data_l-=0.1;
						    if(data_l<0)
									 data_l=4;
							 break;
						case 1:  //湿度
								data_h-=1;
						    if(data_h<0)
									 data_h=60;
							 break;

							case 2:  //温度
								data_t-=1;
						    if(data_t<0)
									 data_t=40;
							 break;
						 case 3:  //RAIN
								data_r-=1;
						    if(data_r<0)
									 data_r=4;
							 break;

					 }
			 }
			 
			 if(botton==RIGHT)
			 {
				   botton = UNPRESS;
					 switch(position)
					 {	
						 case 0:  //光照
								data_l+=0.1;
						    if(data_l>4)
									 data_l=0;
							 break;
							case 1:  //
								data_h+=1;
						    if(data_h>60)
									 data_h=0;
							 break;
						  case 2:  //温度
								data_t+=1;
						    if(data_t>40)
									 data_t=0;
							 break;
							case 3:  //湿度
								data_r+=1;
						    if(data_r>4)
									 data_r=0;
							 break;


							case 4:  //
								  OLED_Clear();
									return;
							 break;
					 }
			 }
			 
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
  while (1)
  {
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
