#include "esp8266.h"
#include "string.h"
#include "stdio.h"

bool device_connect = false;
bool start_esp8266(void)
{ 
	
			HAL_UART_Transmit(&huart1,(uint8_t*)(AT_MODE2),13,0xffff);
			HAL_Delay(1000);
      HAL_UART_Transmit(&huart1,(uint8_t*)(AT_SET),38,0xffff);
				HAL_Delay(1000);
			
			HAL_UART_Transmit(&huart1,(uint8_t*)(AT_RST),8,0xffff);
			HAL_Delay(4000);
			
			HAL_UART_Transmit(&huart1,(uint8_t*)(AT_CIPMUX),13,0xffff);
			HAL_Delay(1000);
			
		  HAL_UART_Transmit(&huart1,(uint8_t*)(AT_CIPSERVER),21,0xffff);
			HAL_Delay(1000);
			
				__HAL_UART_ENABLE_IT(&huart1,UART_IT_IDLE);  
			HAL_UART_Receive_DMA(&huart1,uart1_rx,1000);   
			
			 return true;
			 
}

void handle_esp8266(void)
{
//	char *send = "hello\r\n";
	char *wifi_connect = "0,CONNECT";
	char *led1on = "led1on";   
	char *led1off = "led1off";
	char *led2on = "led2on";
	char *led2off = "led2off";

	if(rx1_end_flag)
	{
			rx1_end_flag = false;
		
		  if(memcmp(uart1_rx,wifi_connect,9)==0)  //wifi已连接
			{
						device_connect=true;
			}
			
			if(strstr((char *)uart1_rx,led1on)!=NULL)  //wifi已连接
			{
						  HAL_GPIO_WritePin(GPIOB, LED1_Pin, GPIO_PIN_RESET);
			}
			
			if(strstr((char *)uart1_rx,led1off)!=NULL)  //wifi已连接
			{
						  HAL_GPIO_WritePin(GPIOB, LED1_Pin, GPIO_PIN_SET);
			}
			
			
						if(strstr((char *)uart1_rx,led2on)!=NULL)  //wifi已连接
			{
						  HAL_GPIO_WritePin(GPIOB, LED2_Pin, GPIO_PIN_RESET);
			}
			
			if(strstr((char *)uart1_rx,led2off)!=NULL)  //wifi已连接
			{
						  HAL_GPIO_WritePin(GPIOB, LED2_Pin, GPIO_PIN_SET);
			}
			
			
			rx1_count=0;
			memset(uart1_rx,0,1000);
			HAL_UART_Receive_DMA(&huart1,uart1_rx,1000);  //需要重新启动DMA
	} 
				
}


void send_wifi(char *data,int size)
{
	  int send_size=0;
	  if(size<10)
		{
			 send_size=1; 
		}
		
		if(size>10&&size<100)
		{
				send_size=2;
		}
		
			
	  if(device_connect)
		{
			char send_data[50]={0};
			char send_data1[50]={0};
			sprintf(send_data,"AT+CIPSEND=0,%d\r\n",size);
			HAL_UART_Transmit(&huart1,(uint8_t*)(send_data),15+send_size,0xffff);

			HAL_Delay(100);
//		  HAL_UART_Transmit(&huart2,(uint8_t*)(data),size,0xffff);
			HAL_UART_Transmit(&huart1,(uint8_t*)data,size,0xffff);
		}


}