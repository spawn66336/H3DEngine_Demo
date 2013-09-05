/**
* @file system_info.cpp
* ����ļ�����Ҫ�����ǣ�
* 
* @author �̳�(Chengchen@h3d.com.cn)
* 
* ������־
* ----------------------------------------------------
* ver 1.0.0
*       by �̳�2010.6.3
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