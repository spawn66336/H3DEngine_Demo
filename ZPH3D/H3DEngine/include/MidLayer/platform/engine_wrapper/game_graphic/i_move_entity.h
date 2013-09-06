/**
* @file i_move_entity.h
* 这个文件的主要功能是：
* 
* @author 程陈(Chengchen@h3d.com.cn)
* 
* 更改日志
* ----------------------------------------------------
* ver 1.0.0
*       by 程陈2010.1.25
*/

#ifndef _I_MOVE_ENTITY_H_
#define _I_MOVE_ENTITY_H_

#include "game_graphic_header.h"

class IMoveEntity
{
public:
	virtual ~IMoveEntity() {};

	/**
	* @brief
	*		获取对象的位置
	* @return
	*		对象的位置
	*/
	virtual H3DVec3 position(void) const = 0;

	/**
	* @brief
	*		设定对象的位置
	* @param
	*		positon：【IN】对象的位置
	*/
	virtual void set_position(const H3DVec3& pos) = 0;

	///**
	//* @brief 
	//*		获取对象的方向
	//* @return
	//*		对象的方向
	//*/
	//virtual H3DVec3 direction(void) const = 0;

	///**
	//* @brief
	//*		设定对象的方向
	//* @param
	//*		direction：【IN】对象的方向
	//*/
	//virtual void set_direction(const H3DVec3& dir) = 0;

	/**
	* @brief
	*		获取对象的位置矩阵
	* @return
	*		对象的位置矩阵
	*/
	virtual const H3DMat4& get_location_matrix(void) const = 0;

	/**
	* @brief
	*		设定对象的位置矩阵
	* @param
	*		location：【IN】对象的位置矩阵
	*/
	virtual void set_location_matrix(const H3DMat4& location) = 0;

	/**
	* @brief
	*		获取对象的欧拉角，欧拉角和max里面保持一致
	* @return
	*		返回一个h3cVec的值，分别表示绕x、y、z三个轴的旋转弧度
	*/
	virtual H3DVec3 rotation() const = 0;

	/**
	* @brief
	*		设定对象的欧拉角，欧拉角和max里面保持一致
	* @return
	*		返回一个h3cVec的值，分别表示绕x、y、z三个轴的旋转弧度
	*/
	virtual void set_rotation(const H3DVec3& obj_rotation) = 0;
};

#endif //_I_MOVE_ENTITY_H_
