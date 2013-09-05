/**
* @file equation.cpp
* 这个文件的主要功能是：
* 
* @author 程陈(Chengchen@h3d.com.cn)
* 
* 更改日志
* ----------------------------------------------------
* ver 1.0.0
*       by 程陈2010.6.8
*/

#include "game_graphic_body.h"

#include <cmath>

#ifndef _FLOAT_PRESION_GRAPHIC_
#define _FLOAT_PRESION_GRAPHIC_  0.00001
#endif

bool is_equal(float f0, float f1)
{
	if (fabs(f0 - f1) <= _FLOAT_PRESION_GRAPHIC_)
	{
		return true;
	}

	return false;
}

bool is_greater(float f0, float f1)
{
	if ((f0 - f1) > _FLOAT_PRESION_GRAPHIC_)
	{
		return true;
	}

	return false;
}

bool is_less(float f0, float f1)
{
	if ((f0 - f1) < _FLOAT_PRESION_GRAPHIC_)
	{
		return true;
	}

	return false;
}

bool is_greate_equal(float f0, float f1)
{
	return is_greater(f0, f1) || is_equal(f0, f1);
}

bool is_less_equal(float f0, float f1)
{
	return is_less(f0, f1) || is_equal(f0, f1);
}

bool calculate_cubic_equations(float a, float b, float c, float d, CUBIC_EQUATION_RESULT& result)
{
	double shengjin_a = powf(b, 2.0) - 3 * a * c;
	double shengjin_b = b * c - 9 * a * d;
	double shengjin_c = powf(c, 2.0) - 3 * b * d;

	if (is_equal(0, shengjin_a) && is_equal(0, shengjin_b))
	{
		result.first_result = result.second_result = result.third_result = -b / (3 * a);
		return true;
	}

	double shengjin_delta = powf(shengjin_b, 2.0) - 4 * shengjin_a * shengjin_c;
	if (is_equal(0, shengjin_delta)) //(0 == shengjin_delta)
	{
		if (!is_equal(0, shengjin_delta)) //(0 != shengjin_a)
		{
			double shengjin_k = shengjin_b / shengjin_a;
			result.first_result = -b/a + shengjin_k;
			result.second_result = -shengjin_k/2;
			result.result_count = 2;
			return true;
		}
	}
	else if (is_greater(shengjin_delta, 0)) //(shengjin_delta > 0)
	{
		double shengjin_y1 = shengjin_a * b + 3 * a * (-shengjin_b + sqrt(shengjin_delta)) / 2;
		double shengjin_y2 = shengjin_a * b + 3 * a * (-shengjin_b - sqrt(shengjin_delta)) / 2;

		if (shengjin_y1 < 0)
		{
			shengjin_y1 = -1 * powf(-shengjin_y1, 1.0 / 3);
		}
		else
		{
			shengjin_y1 = powf(shengjin_y1, 1.0 / 3);
		}

		if (shengjin_y2 < 0)
		{
			shengjin_y2 = -1 * powf(-shengjin_y2, 1.0 / 3);
		}
		else
		{
			shengjin_y2 = powf(shengjin_y2, 1.0 / 3);
		}

		result.first_result = (-b - shengjin_y1 - shengjin_y2) / (3 * a);
		result.result_count = 1;
		return true;
	}
	else
	{
		double shengjin_t = (2 * shengjin_a * b - 3 * a * shengjin_b)/(2 * powf(shengjin_a, 1.5));

		if (shengjin_t > 1.0)
		{
			shengjin_t = 1.0;
		}
		else if (shengjin_t < -1.0)
		{
			shengjin_t = -1.0;
		}
		double shengjin_angle = acos(shengjin_t);

		double tmp1 = cos(shengjin_angle / 3);
		result.first_result = (-b - 2 * sqrt(shengjin_a) * tmp1)/(3 * a);
		double tmp2 = sqrt(3.0) * sin(shengjin_angle / 3);
		result.second_result = (-b + sqrt(shengjin_a) * (tmp1 + tmp2)) / (3 * a);
		result.third_result = (-b + sqrt(shengjin_a) * (tmp1 - tmp2)) / (3 * a);
		result.result_count = 3;

		return true;
	}

	return false;
}

bool calculate_quadratic_equation(float a, float b, float c, QUADRATIC_EQUATION_RESULT& result)
{
	float f = powf(b, 2.0) - 4 * a * c;
	if (f < 0)
	{
		return false;
	}

	f = f / (4 * powf(a, 2.0));
	f = sqrt(f);

	result.first_result = -b / (2 * a) + f;
	result.second_result = -b / (2 * a) - f;

	result.result_count = 2;
	return true;
}

bool calculate_liner_equation(float a, float b, EQUATION_RESULT& result)
{
	if (is_equal(0, a) && !is_equal(0, b)) //(0 == a) && (0 != b))
	{
		return false;
	}

	result.first_result = -b / a;

	result.result_count = 1;

	return true;
}


bool calculate_bezier_equation(float T0, float T1, float T2, float T3, float f_x, float& bezier_result)
{
 	float c1 = (T1 - T0) * 3;
 	float c2= (T2 - T1) * 3- c1;
 	float c3= T3 - T0 - c1 - c2;
 	float c0 = T0 - f_x;

	CUBIC_EQUATION_RESULT result;
	if (!is_equal(0, c3)) // (0 != c3)
	{
		if(!calculate_cubic_equations(c3, c2, c1, c0, result))
		{
			return false;
		}
	}
	else if (!is_equal(0, c2)) // (0 != c2)
	{
		if(!calculate_quadratic_equation(c2, c1, c0, result))
		{
			return false;
		}
	}
	else if (!is_equal(0, c1)) // (0 != c1)
	{
		if(!calculate_liner_equation(c1, c0, result))
		{
			return false;
		}	
	}
	else
	{
		return false;
	}

	if (result.result_count > 0)
	{
		// 		if ((result.first_result >= 0)
		// 			&& (result.first_result <= 1)
		// 			)
		if (!is_greater(0, result.first_result) && !is_greater(result.first_result, 1))
		{
			bezier_result = result.first_result;
			return true;
		}
	}

	if (result.result_count > 1)
	{
		// 		if ((result.second_result >= 0)
		// 			&& (result.second_result <= 1)
		// 			)
		if (!is_greater(0, result.second_result) && !is_greater(result.second_result, 1))
		{
			bezier_result = result.second_result;
			return true;
		}
	}

	if (result.result_count > 2)
	{
		//if ((result.third_result >= 0)
		//	&& (result.third_result <= 1)
		//	)
		if (!is_greater(0, result.third_result) && !is_greater(result.third_result, 1))
		{
			bezier_result = result.third_result;
			return true;
		}
	}

	return false;
}
