/*
 * chassis.h
 *
 *  Created on: May 13, 2025
 *      Author: Bred
 */

#ifndef INC_CHASSIS_H_
#define INC_CHASSIS_H_

#include <stdint.h>
#include "stm32f4xx_hal.h"
#include "DC_motor.h"

extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim3;
extern TIM_HandleTypeDef htim4;
extern TIM_HandleTypeDef htim5;
extern TIM_HandleTypeDef htim8;

extern float v_x, v_y, v_w;//這邊是底盤速度
extern float x, y, theta;
extern float chassis_width;//輪子寬度
extern float chassis_length;
extern float wheel_diameter;
extern const float pi;
extern int range[4];

void test();
void test_2();
void test_3(float try_speed);
//void test_renew_pid_();
void chassis_setup();
void chassis_update_speed(float vx, float vy, float vw);
void mecan_IK_transform(float vx, float vy, float vw);
void mecan_FK_transform();
void localization(float dt);

#endif /* INC_CHASSIS_H_ */

