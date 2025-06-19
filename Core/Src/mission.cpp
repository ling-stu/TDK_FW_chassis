/*
 * mission.cpp
 *
 *  Created on: Jun 16, 2025
 *      Author: Bred
 */
#include "mission.h"
float cmd_x=0.0,cmd_y=0.0;
float std_speed =1.25,min_speed=0.10,circle_speed=0.15; //1.5 0.1 ; 1.25 0.15
float error_ok = 0.004,buffer_dis=20.0; //25 ; 20
//float cp[13][2]={{0.0,0.0},{0.0,20.0},{-14.6,20.0},{-14.6,65.35},{-95.6,65.35},{-95.6,94.65},{-176.3,94.65},{-176.3,165.65},{-95.6,165.65},{-95.6,194.65},{129.4,194.65},{129.4,265.35},{-80,265.35}};
float cp[14][2]={{0.0,0.0},{0.0,20.0},{-14.0,20.0},{-14.0,70.0},{-96.0,70.0},{-96.0,93.0},{-205.0,93.0},{-205.0,170.0},{-96.0,170.0},{-96.0,182.0},{125.0,182.0},{125.0,263.0},{-20.0,263.0},{-20.0,273.0}};
//float cp[15][2]={{0.0,0.0},{0.0,20.0},{-14.0,20.0},{-14.0,70.0},{-96.0,70.0},{-96.0,93.0},{-205.0,93.0},{-205.0,170.0},{-96.0,170.0},{-96.0,185.0},{125.0,185.0},{125.0,263.0},{-20.0,263.0},{-20.0,273.0},{-20.0,273.0}};
int i=1;

void mission_one()
{
	if(y<440)
	{
		cmd_x=-1*x*1;
//		if(y<=90)
//		{
//			cmd_y=1*y/90;
//		}
		if(y<=40)
		{
			cmd_y=0.1+1.4*y/40;
		}
		else if(y<90)
		{
			cmd_y=1.5;
		}
		else if(y<120)
		{
			cmd_y=1+0.5*(120-y)/30.0;
		}
		else if(y<320)
		{
			cmd_y=1.0;
		}
		else if(y<350)
		{
			cmd_y=0.5+0.5*(350-y)/30;
		}
		else if(y<410)
		{
			cmd_y=0.5;
		}
		else
		{
			cmd_y=0.5*(440-y)/30;
		}
	}
	else
	{
		stop();
	}
	chassis_update_speed(cmd_x,cmd_y,0.0);
}

void mission_four()
{
	if(y>=265.35 && x<=-80)
	{
		stop();
	}
	else
	{
		if(y<100)
		{
			if(y<20)
			{
				forward();
			}
			else if(x>-14.6)
			{
				leftward();
			}
			else if(y<65.35)
			{
				forward();
			}
			else if(x>-95.6)
			{
				leftward();
			}
			else if(y<94.65)
			{
				forward();
			}
			else if(x>-176.3)
			{
				leftward();
			}
			else
			{
				forward();
			}
		}
		else if(y<200)
		{
			if(y<165.35)
			{
				forward();
			}
			else if(x<-95.6)
			{
				rightward();
			}
			if(y<194.65)
			{
				forward();
			}
			else if(x<129.4)
			{
				rightward();
			}
			else
			{
				forward();
			}
		}
		else
		{
			if(y<265.35)
			{
				forward();
			}
			else
			{
				leftward();
			}
		}
	}
	chassis_update_speed(cmd_x,cmd_y,0.0);
}
void mission_four_2()
{
	if (linear_toward(cp[i-1][0],cp[i-1][1],cp[i][0],cp[i][1])) i++;
	if(i==14)stop();
//	if(i==14)
//	{
//		theta=0.0;
//		i++;
//	}
//	if(i==15)
//	{
//		if(theta>-2*pi) clockwise();
//		else stop();
//	}
	chassis_update_speed(cmd_x,cmd_y,0.0);
}
void forward()
{
	cmd_x=0.0;
	cmd_y=std_speed;
}

void leftward()
{
	cmd_x=-std_speed;
	cmd_y=0.0;
}

void rightward()
{
	cmd_x=std_speed;
	cmd_y=0.0;
}
void stop()
{
	cmd_x=0.0;
	cmd_y=0.0;
}
bool linear_toward(float from_x,float from_y,float tar_x,float tar_y)
{
	float x_error = fabs(x-tar_x),y_error=fabs(y-tar_y),x_moved=fabs(x-from_x),y_moved=fabs(y-from_y);
	if(x_error<=error_ok && y_error<=error_ok)
	{
		cmd_x=0.0;
		cmd_y=0.0;
		return 1;
	}
	else
	{
		float speed = std_speed;
		if(x_error>y_error)
		{
			if(x_moved<buffer_dis) speed = (std_speed-min_speed)*x_moved/buffer_dis+min_speed;
			else if(x_error<buffer_dis) speed = (std_speed-min_speed)*x_error/buffer_dis+min_speed;
			if(x>tar_x)
			{
				cmd_x=-speed;
				cmd_y=-1*(y-tar_y)*0.6;
			}
			else
			{
				cmd_x=speed;
				cmd_y=-1*(y-tar_y)*0.6;
			}
		}
		else
		{
			if(y_moved<buffer_dis) speed = (std_speed-min_speed)*y_moved/buffer_dis+min_speed;
			else if(y_error<buffer_dis) speed = (std_speed-min_speed)*y_error/buffer_dis+min_speed;
			if(y>tar_y)
			{
				cmd_x=-1*(x-tar_x)*0.6;
				cmd_y=-speed;
			}
			else
			{
				cmd_x=-1*(x-tar_x)*0.6;
				cmd_y=speed;
			}
		}
		return 0;
	}
}
void clockwise()
{
	chassis_update_speed(0.0,0.0,circle_speed);
}
