
#ifndef INC_PATHSENSOR_H_
#define INC_PATHSENSOR_H_

#include "stm32f4xx_hal.h"

void path_setup();
void path();

//go to (x,y)
void path_moveto(float path_d);

void weight();

//1:front find line, 2:middle find line, 3:find cross road, 4:find line
bool line_check(int type);

#endif /* INC_PATHSENSOR_H_ */
