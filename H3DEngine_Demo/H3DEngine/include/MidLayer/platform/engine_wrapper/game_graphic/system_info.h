/**
* @file system_info.h
* ����ļ�����Ҫ�����ǣ�
* 
* @author �̳�(Chengchen@h3d.com.cn)
* 
* ������־
* ----------------------------------------------------
* ver 1.0.0
*       by �̳�2010.6.3
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
	*		����single-tonģʽ������һʵ��
	*/
	static CSystemInfo* instance();

	/**
	* @brief
	*		��ȡһ֡��ʱ��
	* @return
	*		һ֡��ʱ�䣬��λ����
	*/
	float frame_time();

	/**
	* @brief
	*		����һ֡��ʱ��
	* @parma
	*		millseconds_per_frame����IN��һ֡��Ҫ���ٺ���
	*/
	void set_frame_time(float millseconds_per_frame);
private:
	float m_frame_time;
};

#endif //_SYSTEM_INFO_H_
