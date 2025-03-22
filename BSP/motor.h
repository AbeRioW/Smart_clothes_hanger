#ifndef __MOTOR_H
#define __MOTOR_H

#include "main.h"
#include "gpio.h"

//4相控制定义
#define  MOTOR_A_ON   HAL_GPIO_WritePin(GPIOB, MOTOR_A_Pin, GPIO_PIN_SET)
#define  MOTOR_A_OFF  HAL_GPIO_WritePin(GPIOB, MOTOR_A_Pin, GPIO_PIN_RESET)

#define  MOTOR_B_ON   HAL_GPIO_WritePin(GPIOB, MOTOR_B_Pin, GPIO_PIN_SET)
#define  MOTOR_B_OFF  HAL_GPIO_WritePin(GPIOB, MOTOR_B_Pin, GPIO_PIN_RESET)

#define  MOTOR_C_ON   HAL_GPIO_WritePin(GPIOB, MOTOR_C_Pin, GPIO_PIN_SET)
#define  MOTOR_C_OFF  HAL_GPIO_WritePin(GPIOB, MOTOR_C_Pin, GPIO_PIN_RESET)

#define  MOTOR_D_ON   HAL_GPIO_WritePin(GPIOB, MOTOR_D_Pin, GPIO_PIN_SET)
#define  MOTOR_D_OFF  HAL_GPIO_WritePin(GPIOB, MOTOR_D_Pin, GPIO_PIN_RESET)


extern int direction;
extern uint16_t Motor_Pulse;

int motor_angle_cal(int rotation_angle);
void motor_controld(uint8_t step,uint8_t direction);
void close_motor(void);

#endif