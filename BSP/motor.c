#include "motor.h"

//电机旋转的方向
int direction = 0;

//电机旋转的脉冲个数
uint16_t Motor_Pulse = 0;

//电机控制,采用8节拍来做
void motor_controld(uint8_t step,uint8_t direction)
{
		if(direction==1)  //表示反向
		{
				step = 8- step;
		}
		
		switch(step)
		{
			//A
			case 0:
					MOTOR_A_ON;
			    MOTOR_B_OFF;
			    MOTOR_C_OFF;
			    MOTOR_D_OFF;
				break;
			
			//A->B
			case 1:
					MOTOR_A_ON;
			    MOTOR_B_ON;
			    MOTOR_C_OFF;
			    MOTOR_D_OFF;
				break;
			//B
			case 2:
					MOTOR_A_OFF;
			    MOTOR_B_ON;
			    MOTOR_C_OFF;
			    MOTOR_D_OFF;
				break;			
			//B->C
			case 3:
					MOTOR_A_OFF;
			    MOTOR_B_ON;
			    MOTOR_C_ON;
			    MOTOR_D_OFF;
				break;
			//C
			case 4:
					MOTOR_A_OFF;
			    MOTOR_B_OFF;
			    MOTOR_C_ON;
			    MOTOR_D_OFF;
				break;
			//C->D
			case 5:
					MOTOR_A_OFF;
			    MOTOR_B_OFF;
			    MOTOR_C_ON;
			    MOTOR_D_ON;
				break;
			//D
			case 6:
					MOTOR_A_OFF;
			    MOTOR_B_OFF;
			    MOTOR_C_OFF;
			    MOTOR_D_ON;
				break;
			//D->A
			case 7:
					MOTOR_A_ON;
			    MOTOR_B_OFF;
			    MOTOR_C_OFF;
			    MOTOR_D_ON;
				break;
		}
}
  

void close_motor(void)
{
		  HAL_GPIO_WritePin(GPIOB, MOTOR_A_Pin|MOTOR_B_Pin|MOTOR_C_Pin|MOTOR_D_Pin, GPIO_PIN_RESET);
}


/*
	rotation_abgle:旋转角度
返回:motor pulse:根据公式计算得出的脉冲个数
*/
int motor_angle_cal(int rotation_angle)
{
			Motor_Pulse = (int)((double)(rotation_angle/5.625)*64);
			return Motor_Pulse;
}