/**
* @file target_position.cpp
* 这个文件的主要功能是：
* 
* @author 程陈(Chengchen@h3d.com.cn)
* 
* 更改日志
* ----------------------------------------------------
* ver 1.0.0
*       by 程陈2010.12.15
*/

#include "game_graphic_body.h"

BaseTargetPosition::~BaseTargetPosition()
{
	NULL;
}

void BaseTargetPosition::update(float t)
{
	NULL;
}

void BaseTargetPosition::set_info(const char* info_name, const char* info_value)
{
	assert(NULL != info_name);
	assert(NULL != info_value);

	m_info_collection[info_name] = info_value;
}

H3DVec3 BaseTargetPosition::get_target_position()
{
	return H3DVec3(0.0f, 0.0f, 0.0f);
}