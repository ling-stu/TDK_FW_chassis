/*
 * mainpp.cpp
 *
 *  Created on: May 8, 2025
 *      Author: Bred
 */

/* main.cpp */
#include "chassis.h"
#include "mainpp.h"
#include "stm32f4xx_hal.h"
#include "DC_motor.h"
#include "mission.h"
int sec=0;
float try_speed = 1.0;
bool on=0;



void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
    if (htim->Instance == TIM5) {
        // 每 1ms 呼叫一次底盤更新
    	sec++;
    	if(on)
    	{
//    		test();
//    		if(!linear_toward(0.0,0.0,14.0,20.0))chassis_update_speed(cmd_x,cmd_y,0.0);
//			else chassis_update_speed(0.0,0.0,0.0);
//    		if(!linear_toward(0.0,0.0,0.0,150.0))chassis_update_speed(cmd_x,cmd_y,0.0);
//			else chassis_update_speed(0.0,0.0,0.0);
//    		mission_one();
//    		mission_four();
    		mission_four_2();
//    		if(x<150)chassis_update_speed(0.5,0.0,0.0);
//    		else chassis_update_speed(0.0,0, 0.0);
    	}
    	else
    	{
//    		test_3(0);
    		chassis_update_speed(0.0,0.0,0.0);
    	}

    }
    if (htim->Instance == TIM6) {
        HAL_IncTick();
      }
}

void main_function() {
    //HAL_Init();
    chassis_setup();  // 初始化四輪的 encoder 與 PWM
    //pid_setup();
    while (1) {
    	if(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_4)==GPIO_PIN_RESET) on = 1;
    	else on = 0;
    	HAL_Delay(500);
    }
}

