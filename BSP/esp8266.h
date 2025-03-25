#ifndef __ESP8266_H
#define __ESP8266_H

#include "main.h"
#include "stdbool.h"
#include "usart.h"
#include "oled.h"

#define AT_MODE2 		"AT+CWMODE=2\r\n"   //13
#define AT_SET      "AT+CWSAP=\"ESP8266\",\"0123456789\",11,0\r\n"  //38
#define AT_RST   		"AT+RST\r\n"  //8
#define AT_CIPMUX 	"AT+CIPMUX=1\r\n"
#define AT_CIPSERVER "AT+CIPSERVER=1,5000\r\n"  //start


extern bool device_connect;

bool start_esp8266(void);

void handle_esp8266(void);


void send_wifi(char *data,int size);








#endif