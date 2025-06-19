/*
 * mission.h
 *
 *  Created on: Jun 16, 2025
 *      Author: Bred
 */

#ifndef INC_MISSION_H_
#define INC_MISSION_H_
#include "chassis.h"

void mission_one();
void mission_four();
void mission_four_2();
void forward();
void leftward();
void rightward();
void stop();
bool linear_toward(float from_x,float from_y,float tar_x,float tar_y);
void clockwise();
extern float cmd_x,cmd_y;




#endif /* INC_MISSION_H_ */
