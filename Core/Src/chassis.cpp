/*
 * chassis.cpp
 *
 *  Created on: May 13, 2025
 *      Author: Bred
 */

/* chassis.cpp */
#include "chassis.h"


float _v1_ = 0;
float _v2_ = 0;
float _v3_ = 0;
float _v4_ = 0;

//extern TIM_HandleTypeDef htim1;
//extern TIM_HandleTypeDef htim2;
//extern TIM_HandleTypeDef htim3;
//extern TIM_HandleTypeDef htim4;
//extern TIM_HandleTypeDef htim5;
//extern TIM_HandleTypeDef htim8;

float v_x = 0.f, v_y = 0.f, v_w = 0.f;
float x = 0.f, y = 0.f, theta = 0.f;
float chassis_width = 30.0f;
float chassis_length = 26.0f;
float wheel_diameter = 10.0f;
const float pi = 3.1415926f;
int try_pwm = 300;
float kp[4]={2.1,1.2,1.2,1.2},ki[4]={25,20,20,25};//3.0,0.001
int range[4]={35,32,40,60};
float temp2 = 0;
DC_motor wheel_FR(&htim1, GPIOB, GPIO_PIN_12, &htim8, TIM_CHANNEL_1);//encoder timer, dir port, dir pin, pwm timer, pwm channel
DC_motor wheel_FL(&htim2, GPIOA, GPIO_PIN_12, &htim8, TIM_CHANNEL_2);
DC_motor wheel_BR(&htim3, GPIOB, GPIO_PIN_14, &htim8, TIM_CHANNEL_3);
DC_motor wheel_BL(&htim4, GPIOB, GPIO_PIN_15, &htim8, TIM_CHANNEL_4);

void test()
{
//	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET);
//	__HAL_TIM_SET_COMPARE( &htim8, TIM_CHANNEL_1, try_pwm);
//	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, GPIO_PIN_RESET);
//	__HAL_TIM_SET_COMPARE( &htim8, TIM_CHANNEL_2, try_pwm);
//	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_RESET);
//	__HAL_TIM_SET_COMPARE( &htim8, TIM_CHANNEL_3, try_pwm);
//	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_RESET);
//	__HAL_TIM_SET_COMPARE( &htim8, TIM_CHANNEL_4, try_pwm);
}


void test_2()
{
//	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, GPIO_PIN_SET);
//	__HAL_TIM_SET_COMPARE( &htim8, TIM_CHANNEL_2, try_pwm);
//	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_SET);
//	__HAL_TIM_SET_COMPARE( &htim5, TIM_CHANNEL_1, try_pwm);
}

void test_3(float try_speed)
{
	wheel_BR.setspeed(try_speed);
	wheel_BR.update_speed(1);
	wheel_BR.PI_run(2);
}

void chassis_setup() {
	HAL_TIM_Base_Start_IT(&htim5);
    wheel_FR.setup();
    wheel_FR.set_pid(kp[0], ki[0]);
    wheel_FL.setup();
    wheel_FL.set_pid(kp[1], ki[1]);
    wheel_BR.setup();
    wheel_BR.set_pid(kp[2], ki[2]);
    wheel_BL.setup();
    wheel_BL.set_pid(kp[3], ki[3]);
}

void test_renew_pid_()
{
	wheel_FR.set_pid(kp[0], ki[0]);
	wheel_FL.set_pid(kp[1], ki[1]);
	wheel_BR.set_pid(kp[2], ki[2]);
	wheel_BL.set_pid(kp[3], ki[3]);
}

void mecan_IK_transform(float _v_x, float _v_y, float _v_w) {
    float factor = (chassis_width + chassis_length) / (pi * wheel_diameter);//2.0f *
    float v1 = -_v_x + _v_y + _v_w * factor;
    float v2 = _v_x + _v_y - _v_w * factor;
    float v3 = _v_x + _v_y + _v_w * factor;
    float v4 = -_v_x + _v_y - _v_w * factor;
    wheel_FR.setspeed(v1);
    wheel_FL.setspeed(v2);
    wheel_BR.setspeed(v3);
    wheel_BL.setspeed(v4);
    _v1_ = v1;
    _v2_ = v2;
    _v3_ = v3;
    _v4_ = v4;
}

void mecan_FK_transform() {
    float v1 = wheel_FR.get_speed() * wheel_diameter * pi;
    float v2 = wheel_FL.get_speed() * wheel_diameter * pi;
    float v3 = wheel_BR.get_speed() * wheel_diameter * pi;
    float v4 = wheel_BL.get_speed() * wheel_diameter * pi;
    v_x = (-v1 + v2 + v3 - v4) / 4.0f;
    v_y = (v1 + v2 + v3 + v4) / 4.0f;
    v_w = (-v1 +   v2 - v3 + v4) / (chassis_width + chassis_length);
}

void localization(float dt) {
//    x += v_x * dt;
//    y += v_y * dt;
//    theta += v_w * dt;
    float cosT = cosf(theta);
    float sinT = sinf(theta);

    float vx_world =  cosT * v_x - sinT * v_y;
    float vy_world =  sinT * v_x + cosT * v_y;

    x     += vx_world * dt;
    y     += vy_world * dt;
    theta += v_w      * dt;
//    if (theta >= 2 * pi)  theta -= 2 * pi;
//    if (theta <  0.0f)    theta += 2 * pi;
}

void chassis_update_speed(float _v_x, float _v_y, float _v_w) {
    wheel_FR.update_speed(1);
    wheel_FL.update_speed(-1);
    wheel_BR.update_speed(-1);
    wheel_BL.update_speed(-1);//-1，改

    mecan_IK_transform(_v_x, _v_y, _v_w);
    temp2 = _v_w;
    wheel_FR.PI_run(0);
    wheel_FL.PI_run(1);
    wheel_BL.PI_run(3);
    wheel_BR.PI_run(2);


    mecan_FK_transform();

    localization(wheel_FL.span);
}


