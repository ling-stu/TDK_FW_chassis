/*
 * pid.cpp
 *
 *  Created on: May 8, 2025
 *      Author: Bred
 */


#include "pid.h"

extern TIM_HandleTypeDef htim1;//encoder1
extern TIM_HandleTypeDef htim2;//encoder2
extern TIM_HandleTypeDef htim3;//encoder3
extern TIM_HandleTypeDef htim4;//encoder4
extern TIM_HandleTypeDef htim5;//timer中斷+pwm
extern TIM_HandleTypeDef htim8;//pwm

double speed_1,speed_2,speed_3,speed_4;
double pulse_1,pulse_2,pulse_3,pulse_4;
int resolution = 256;
int reduction_ratio = 24;
float span = 0.001;
int freq = 1000;

double sp_1 = 0.0, ki_1 = 0.01, kp_1 = 10.0, kd_1 = 0.0;
double sp_2 = 0.0, ki_2 = 0.01, kp_2 = 10.0, kd_2 = 0.0;
double sp_3 = 0.0, ki_3 = 0.01, kp_3 = 10.0, kd_3 = 0.0;
double sp_4 = 0.0, ki_4 = 0.01, kp_4 = 10.0, kd_4 = 0.0;
double u_1,u_2,u_3,u_4;
double bound_1,bound_2,bound_3,bound_4;
double integral_1,integral_2,integral_3,integral_4;
double error_1,error_2,error_3,error_4;
int time_div = 0, time_s = 0;
int16_t enc_1,enc_2,enc_3,enc_4;

int explode_1=0;

/*void pid_setup() {
    HAL_TIM_Base_Start_IT(&htim5);
    HAL_TIM_Encoder_Start(&htim1, TIM_CHANNEL_ALL);
    HAL_TIM_Encoder_Start(&htim2, TIM_CHANNEL_ALL);
    HAL_TIM_Encoder_Start(&htim3, TIM_CHANNEL_ALL);
    HAL_TIM_Encoder_Start(&htim4, TIM_CHANNEL_ALL);
    HAL_TIM_PWM_Start(&htim5, TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_2);
    HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_4);


}*/

/*float compute_u(int16_t enc, float sp, float kp, float ki)
{
	speed = (double) enc*60 / (4 * resolution * reduction_ratio * span);
	__HAL_TIM_SetCounter(&htim2, 0);

	error = sp - speed;

	integral += error * span;
	bound = 1/ki;
	if (ki * integral > 1) integral = bound;
	else if (ki * integral < -1) integral = -bound;

	 float u = kp * error + ki * integral;
	 if (u > 1) u = 1;
	 else if (u < -1) u = -1;
	 return u;
}*/

/*void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
    if (htim->Instance == TIM5) {
		if (time_div++ % freq == 0) time_s++;

		enc_1 = __HAL_TIM_GetCounter(&htim1);
		speed_1 = (double) enc_1*60 / (4 * resolution * reduction_ratio * span);
		__HAL_TIM_SetCounter(&htim1, 0);
		error_1 = sp_1 - speed_1;
		integral_1 += error_1 * span;
		bound_1 = 1/ki_1;
		if (ki_1 * integral_1 > 1) integral_1 = bound_1;
		else if (ki_1 * integral_1 < -1) integral_1 = -bound_1;
		u_1 = kp_1 * error_1 + ki_1 * integral_1;
		if (u_1 > 1) u_1 = 1;
		else if (u_1 < -1) u_1 = -1;

		enc_2 = __HAL_TIM_GetCounter(&htim2);
		speed_2 = (double) enc_2*60 / (4 * resolution * reduction_ratio * span);
		__HAL_TIM_SetCounter(&htim2, 0);
		error_2 = sp_2 - speed_2;
		integral_2 += error_2 * span;
		bound_2 = 1/ki_2;
		if (ki_2 * integral_2 > 1) integral_2 = bound_2;
		else if (ki_2 * integral_2 < -1) integral_2 = -bound_2;
		u_2 = kp_2 * error_2 + ki_2 * integral_2;


		enc_3 = __HAL_TIM_GetCounter(&htim3);
		speed_3 = (double) enc_3*60 / (4 * resolution * reduction_ratio * span);
		__HAL_TIM_SetCounter(&htim3, 0);
		error_3 = sp_3 - speed_3;
		integral_3 += error_3 * span;
		bound_3 = 1/ki_3;
		if (ki_3 * integral_3 > 1) integral_3 = bound_3;
		else if (ki_3 * integral_3 < -1) integral_3 = -bound_3;
		u_3 = kp_3 * error_3 + ki_3 * integral_3;
		if (u_3 > 1) u_3 = 1;
		else if (u_3 < -1) u_3 = -1;

		enc_4 = __HAL_TIM_GetCounter(&htim4);
		speed_4 = (double) enc_4*60 / (4 * resolution * reduction_ratio * span);
		__HAL_TIM_SetCounter(&htim4, 0);
		error_4 = sp_4 - speed_4;
		integral_4 += error_4 * span;
		bound_4 = 1/ki_4;
		if (ki_4 * integral_4 > 1) integral_4 = bound_4;
		else if (ki_4 * integral_4 < -1) integral_4 = -bound_4;
		u_4 = kp_4 * error_4 + ki_4 * integral_4;
		if (u_4 > 1) u_4 = 1;
		else if (u_4 < -1) u_4 = -1;

		if (u_1 > 0) {
			pulse_1 = (int)(u_1 * freq);
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);
			//HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET);
		} else {
			pulse_1 = (int)(-u_1 * freq);
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET);
			//HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET);
		}
		__HAL_TIM_SET_COMPARE(&htim8, TIM_CHANNEL_1, pulse_1);

		if (u_2 > 0) {
			pulse_2 = (int)(u_2 * freq);
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_SET);
			//HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, GPIO_PIN_RESET);
		} else {
			pulse_2 = (int)(-u_2 * freq);
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_RESET);
			//HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, GPIO_PIN_SET);
		}
		__HAL_TIM_SET_COMPARE(&htim8, TIM_CHANNEL_2, pulse_2);

		if (u_3 > 0) {
			pulse_3 = (int)(u_3 * freq);
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_SET);
			//HAL_GPIO_WritePin(GPIOC, GPIO_PIN_5, GPIO_PIN_RESET);
		} else {
			pulse_3 = (int)(-u_3 * freq);
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_RESET);
			//HAL_GPIO_WritePin(GPIOC, GPIO_PIN_5, GPIO_PIN_SET);
		}
		__HAL_TIM_SET_COMPARE(&htim5, TIM_CHANNEL_1, pulse_3);

		if (u_4 > 0) {
			pulse_4 = (int)(u_4 * freq);
			//pulse = 300;
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_SET);//ok
			//HAL_GPIO_WritePin(GPIOB, GPIO_PIN_11, GPIO_PIN_RESET);//ok
		} else {
			pulse_4 = (int)(-u_4 * freq);
			//pulse = 300;
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_RESET);
			//HAL_GPIO_WritePin(GPIOB, GPIO_PIN_11, GPIO_PIN_SET);
		}
		__HAL_TIM_SET_COMPARE(&htim8, TIM_CHANNEL_4, pulse_4);


    }
    if (htim->Instance == TIM6) {
    	HAL_IncTick();
    }
}*/




