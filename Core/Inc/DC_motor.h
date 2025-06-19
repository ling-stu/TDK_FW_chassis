/* DC_motor.h */
#ifndef INC_DC_MOTOR_H_
#define INC_DC_MOTOR_H_

#include "stm32f4xx_hal.h"
#include <cstdlib>
#include "math.h"
#include "chassis.h"//for range

class DC_motor {
public:
    DC_motor(TIM_HandleTypeDef* enc, GPIO_TypeDef* port, uint16_t pin, TIM_HandleTypeDef* pwm, uint32_t ch)
    		: _enc_htim(enc), _dirPort(port), _dirPin(pin), _PWM_htim(pwm), _PWM_TIM_CHANNEL(ch) {}

    void setup();
    void setspeed(float speed);
    void update_speed(int sign = 1);
    void PI_run(int index); // FR=0,FL=1,BR=2,BL=3
    float get_speed();
    void set_pid(float kp, float ki);
    float span = 0.001;

private:
    TIM_HandleTypeDef* _enc_htim;
    GPIO_TypeDef* _dirPort;
    uint16_t _dirPin;
    TIM_HandleTypeDef* _PWM_htim;
    uint32_t _PWM_TIM_CHANNEL;

    float _sp = 0.0;
    float _speed = 0.0;
    float _error = 0.0;
    float _integral = 0.0;
    float _differential = 0.0;
    float _pre_error = 0.0;

    float _u = 0;
    float _kp = 0;
    float _ki = 0;
    float _kd = 0.0;

    int16_t _enc;
    float _resolution = 256.0;
    float _reduction_ratio = 24.0;

    float _arr = 1000.0;
    int _pulse = 0;
};

#endif /* INC_DC_MOTOR_H_ */
