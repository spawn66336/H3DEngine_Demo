/**
* @file system_info.cpp
* 这个文件的主要功能是：
* 
* @author 程陈(Chengchen@h3d.com.cn)
* 
* 更改日志
* ----------------------------------------------------
* ver 1.0.0
*       by 程陈2010.6.3
*/

#include "game_graphic_body.h"

CSystemInfo::CSystemInfo()
{
	m_frame_time = 1.0;
}

CSystemInfo* CSystemInfo::instance()
{
	static CSystemInfo system_info;
	return &system_info;
}

float CSystemInfo::frame_time()
{
	return m_frame_time;
}

void CSystemInfo::set_frame_time(float millseconds_per_frame)
{
	assert(millseconds_per_frame > 0);
	m_frame_time = millseconds_per_frame;
}