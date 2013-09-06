/**
* @file target_position_calculator.h
* 这个文件的主要功能是：
* 
* @author 程陈(Chengchen@h3d.com.cn)
* 
* 更改日志
* ----------------------------------------------------
* ver 1.0.0
*       by 程陈2010.12.15
*/

#ifndef _TARGET_POSITION_H_
#define _TARGET_POSITION_H_

#include "game_graphic_header.h"
#include "i_target_position.h"

/**
* @brief
*		目标点位置计算器，实现一些基本的功能
*/
class BaseTargetPosition : public ITargetPosition
{
public:
	virtual ~BaseTargetPosition();

	/**
	* @brief
	*		更新
	*/
	virtual void update(float t);

	/**
	* @brief
	*		设置目标计算器相关的信息
	*/
	virtual void set_info(const char* info_name, const char* info_value);

	/**
	* @brief
	*		获取目标点的位置
	*/
	virtual H3DVec3 get_target_position();

private:
	typedef std::map<std::string, std::string> InfoCollection;

	InfoCollection		m_info_collection;
};



#endif //_TARGET_POSITION_H_
