/**
* @file equation.h
* 这个文件的主要功能是：
* 
* @author 程陈(Chengchen@h3d.com.cn)
* 
* 更改日志
* ----------------------------------------------------
* ver 1.0.0
*       by 程陈2010.6.8
*/

#ifndef _EQUATION_H_
#define _EQUATION_H_

#include "game_graphic_header.h"

bool is_equal(float f0, float f1);
bool is_greater(float f0, float f1);
bool is_less(float f0, float f1);
bool is_greate_equal(float f0, float f1);
bool is_less_equal(float f0, float f1);

struct EQUATION_RESULT 
{
	float first_result;
	int result_count;
	EQUATION_RESULT():first_result(0),result_count(0){;}
};

struct QUADRATIC_EQUATION_RESULT 
	:public EQUATION_RESULT
{
	float second_result;
	QUADRATIC_EQUATION_RESULT():second_result(0){;}
};

struct CUBIC_EQUATION_RESULT 
	:public QUADRATIC_EQUATION_RESULT
{
	float third_result;
	CUBIC_EQUATION_RESULT():third_result(0){;}
};

bool calculate_cubic_equations(float a, float b, float c, float d, CUBIC_EQUATION_RESULT& result);
bool calculate_quadratic_equation(float a, float b, float c, QUADRATIC_EQUATION_RESULT& result);
bool calculate_liner_equation(float a, float b, EQUATION_RESULT& result);

bool calculate_bezier_equation(float T0, float T1, float T2, float T3, float f_x, float& result);

//bool is_equal(float f0, float f1);
//bool is_greater(float f0, float f1);
#endif //_EQUATION_H_

