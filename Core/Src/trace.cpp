/*
 * pathsensor.cpp
 *
 *  Created on: Jul 4, 2025
 *      Author: 88698
 */
//#include "pathsensor.h"
//#include "location.h"
//#include "script.h"
#include "stm32f4xx_hal.h"
#include "chassis.h"

extern ADC_HandleTypeDef hadc2;
extern float map_x, map_y, last_x, last_y;
float cmd_v_x, cmd_v_y, cmd_v_w;
extern bool arrive;

#define normal_Speed 1
#define w_kp 0.21
#define w_kd 0
#define boundry 1000
#define spin_sp 1

uint16_t adcRead[7];
int   check = 0;
float temp = 0;
float weight_err = 0;
float weight_lasttime = 0;
float weight_change = 0;
float tempSpeed[2];
float path_motor_speed[2];
float path_dis = 0;
/*
adcRead[0]  adc3-0   PC2  right
adcRead[1]  adc3-1   PC3    |
adcRead[2]  adc3-2   PF3    |
adcRead[3]  adc3-3   PA1    V
adcRead[4]  adc3-4   PA0  left
adcRead[5]  adc3-5   PB0  middle right
adcRead[6]  adc3-6   PB1  middle left
*/
void path_setup(){
	//if(HAL_ADC_Start_DMA(&hadc3,(uint32_t *)adcRead,7) != HAL_OK)
	HAL_ADC_Start_DMA(&hadc2,(uint32_t *)adcRead,7);
	check++;
}
void weight() {
    // Calculate line following error (P and D)
    weight_err = ((float)(-3*adcRead[0] - adcRead[1] + adcRead[3] + 3*adcRead[4])
    				/(adcRead[0] + adcRead[1] + adcRead[2] + adcRead[3] + adcRead[4]));
    weight_change = weight_err - weight_lasttime;
    weight_lasttime = weight_err;

    // For mecanum: output chassis velocity vector
    cmd_v_y = normal_Speed; // Forward speed (positive: forward)
    cmd_v_x = 0;            // No strafe (add logic here if you want to strafe)
    cmd_v_w = (weight_err * w_kp + weight_change * w_kd); // Robtion correction//    temp = (weight_err * w_kp + weight_change * w_kd); // Rotation correction
    temp = cmd_v_w;
    chassis_update_speed(0,1,cmd_v_w); //positive:counterclockwise
    check++;
}

//motor_speed[0]:right motor speed, motor_speed[1]:left motor speed
void path() { // follow path for mecanum chassis

    // turn right (in place)
//    if (adcRead[5] >= boundry && adcRead[6] < boundry && adcRead[0] < boundry && adcRead[1] < boundry
//        && adcRead[2] < boundry && adcRead[3] < boundry && adcRead[4] < boundry) {
//
//        cmd_v_y = 0;
//        cmd_v_x = 0;
//        cmd_v_w = -spin_sp;
//
//        while (adcRead[2] < 3 * boundry) {}
//
//        ach(1);
//
//        cmd_v_y = 0;
//        cmd_v_x = 0;
//        cmd_v_w = 0;
//    }
//    // turn left (in place)
//    else if (adcRead[5] < boundry && adcRead[6] >= 2 * boundry && adcRead[0] < boundry && adcRead[1] < boundry
//        && adcRead[2] < boundry && adcRead[3] < boundry && adcRead[4] < boundry) {
//
//        cmd_v_y = 0;
//        cmd_v_x = 0;
//        cmd_v_w = spin_sp;
//
//        while (adcRead[2] < 3 * boundry) {}
//
//        ach(1);
//
//        cmd_v_y = 0;
//        cmd_v_x = 0;
//        cmd_v_w = 0;
//    }
//    // forward (line following)
//    else {
        weight(); // sets cmd_v_y, cmd_v_x, cmd_v_w
//    }
}

//go to (x,y)
void path_moveto(float path_d){

	//location_reset(0, 0, 90);

	//location_data(0);

	path_dis = path_d;

	//ach(0);

	while(!arrive)
		path();
}
//1:front find line, 2:middle find line, 3:find cross road, 4:find line
bool line_check(int type){//確認特徵點，更新座標

	int b = 3000;//黑線

	switch(type){

	//front line
	case 1:

		if(adcRead[0] >= b && adcRead[2] >= b && adcRead[4] >= b)
			return 1;
		else
			return 0;
		break;
	//middle line
	case 2:

		if(adcRead[5] >= b && adcRead[6] >= b)
			return 1;
		else
			return 0;
		break;
		//cross road
	case 3:

		if(adcRead[2] >= b && adcRead[5] >= b && adcRead[6] >= b)
			return 1;
		else
			return 0;
		break;
	case 4:

		if(adcRead[0] >= b || adcRead[1] >= b || adcRead[2] >= b || adcRead[3] >= b || adcRead[4] >= b)
			return 1;
		else
			return 0;
		break;
	case 5:

		if(adcRead[2] >= b)
			return 1;
		else
			return 0;
		break;
	case 6://right line

		if(adcRead[5] >= b && adcRead[6] >= b)
			return 1;
		else
			return 0;
		break;
	}
}



/*
 * trace.cpp
 *
 *  Created on: Jul 18, 2025
 *      Author: Ling
 */




