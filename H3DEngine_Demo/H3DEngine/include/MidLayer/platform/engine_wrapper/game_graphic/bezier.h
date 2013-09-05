/**
* @file bezier.h
* 这个文件的主要功能是：
* 
* @author 程陈(Chengchen@h3d.com.cn)
* 
* 更改日志
* ----------------------------------------------------
* ver 1.0.0
*       by 程陈2010.6.3
*/

#ifndef _BEZIER_H_
#define _BEZIER_H_

#include "game_graphic_header.h"

#ifdef _USE_SIMPLE_BEZIER_
template<typename T>
T calculate_bezier(float percent, const T& T0, const T& T1, const T& T2, const T& T3)
{
	T c1 = (T1 - T0) * 3;
	T c2= (T2 - T1) * 3- c1;
	T c3= T3 - T0 - c1 - c2;

	return (T)( T0 + ((c3 * percent + c2) * percent + c1) * percent);
}
#endif

template<typename T>
T calculate_bezier(float percent, const T& T0, const T& T1, const T& T2, const T& T3)
{
	T c1 = (T1 - T0) * 3;
	T c2= (T2 - T1) * 3- c1;
	T c3= T3 - T0 - c1 - c2;

	return (T)(T0 + ((c3 * percent + c2) * percent + c1) * percent);
}


#endif //_BEZIER_H_
