#ifndef _I_TARGET_POSITION_H_
#define _I_TARGET_POSITION_H_

#include "game_graphic_header.h"

/**
* @brief
*		目标点位置计算器
* @description
*		计算追光灯和相机的目标点的位置
*/
class ITargetPosition
{
public:
	virtual ~ITargetPosition(){}

	/**
	* @brief
	*		更新
	*/
	virtual void update(float t) = 0;

	/**
	* @brief
	*		设置目标计算器相关的信息
	*/
	virtual void set_info(const char* info_name, const char* info_value) = 0;

	/**
	* @brief
	*		获取目标点的位置
	*/
	virtual H3DVec3 get_target_position() = 0;
};

enum SmoothType
{
	SmoothType_NONE,
	SmoothType_AVERAGE_SMOOTH,
	SmoothType_ON_ENTER,
};

enum TargetType
{
	TargetType_ALL_TARGET,
	TargetType_SPECIFIED_TARGET,
};

typedef int TargetID;

class ITargetPositionMng
{
public:
	virtual ~ITargetPositionMng(){}
	virtual ITargetPosition* getTargetPosition(TargetType tm, const TargetID& targetid) = 0;
	/**
	@brief 设置平均值采样时最小采样间隔，用于目标系统进行自定义设置
	*/
	virtual void setMinInterval(int interval) = 0;
	/**
	@brief 取最小采样间隔，用于计算器更新采样间隔
	*/
	virtual int getMinInterval() = 0;
};

#endif //_I_TARGET_POSITION_H_
