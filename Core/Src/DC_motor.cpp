/* DC_motor.cpp */
#include "DC_motor.h"

void DC_motor::setup() {
    HAL_TIM_Encoder_Start(this->_enc_htim, TIM_CHANNEL_ALL);
    HAL_TIM_PWM_Start(this->_PWM_htim, this->_PWM_TIM_CHANNEL);
    this->_arr = __HAL_TIM_GET_AUTORELOAD(this->_PWM_htim);//理論上就是1000，不應該改變
}

void DC_motor::setspeed(float goal_speed) {
    this->_sp = goal_speed;
}

void DC_motor::set_pid(float kp, float ki){
	this->_kp = kp;
	this->_ki = ki;
}

void DC_motor::update_speed(int sign) {
    this->_enc = __HAL_TIM_GET_COUNTER(this->_enc_htim);
    __HAL_TIM_SET_COUNTER(this->_enc_htim, 0);
    this->_speed = sign * (float)this->_enc / (4.0f * this->_resolution * span * this->_reduction_ratio);// 100.0 *   //每秒圈數
}

void DC_motor::PI_run(int index) {
    float bound = 1.0f / this->_ki;
    this->_error = this->_sp - this->_speed;

    this->_integral += this->_error * span;
//    differential = error - pre_error;
    if(this->_integral > bound) this->_integral = bound;
    else if(this->_integral < -bound) this->_integral = -bound;
//    else if (fabs(this->_speed) < 0.0005) this->_integral =0;
//    if (ki * integral >= 1) ki * integral =0.01;

    this->_u = this->_kp * this->_error + this->_ki * this->_integral ;//+ kd * differential
//    pre_error = error;

    if (this->_u > 1) this->_u = 1;
    else if (this->_u < -1) this->_u = -1;

    this->_pulse = fabs(this->_u) * this->_arr;

    if(this->_u > 0) HAL_GPIO_WritePin(this->_dirPort, this->_dirPin, GPIO_PIN_SET);
    else HAL_GPIO_WritePin(this->_dirPort, this->_dirPin, GPIO_PIN_RESET);

    if(this->_pulse <= range[index]) this->_pulse = 0;
	__HAL_TIM_SET_COMPARE(this->_PWM_htim, this->_PWM_TIM_CHANNEL, (uint16_t)this->_pulse);
}

float DC_motor::get_speed() {
    return this->_speed;
}
