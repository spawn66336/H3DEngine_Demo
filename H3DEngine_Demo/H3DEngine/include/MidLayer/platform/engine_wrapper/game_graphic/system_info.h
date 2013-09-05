/**
* @file system_info.h
* 这个文件的主要功能是：
* 
* @author 程陈(Chengchen@h3d.com.cn)
* 
* 更改日志
* ----------------------------------------------------
* ver 1.0.0
*       by 程陈2010.6.3
*/

#ifndef _SYSTEM_INFO_H_
#define _SYSTEM_INFO_H_

#include "game_graphic_header.h"

class CSystemInfo
{
public:
	CSystemInfo();

	/**
	* @brief
	*		采用single-ton模式创建单一实例
	*/
	static CSystemInfo* instance();

	/**
	* @brief
	*		获取一帧的时间
	* @return
	*		一帧的时间，单位毫秒
	*/
	float frame_time();

	/**
	* @brief
	*		设置一帧的时间
	* @parma
	*		millseconds_per_frame：【IN】一帧需要多少毫秒
	*/
	void set_frame_time(float millseconds_per_frame);
private:
	float m_frame_time;
};

#endif //_SYSTEM_INFO_H_
