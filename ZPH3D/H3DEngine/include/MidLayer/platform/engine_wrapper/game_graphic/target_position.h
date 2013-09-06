/**
* @file target_position_calculator.h
* ����ļ�����Ҫ�����ǣ�
* 
* @author �̳�(Chengchen@h3d.com.cn)
* 
* ������־
* ----------------------------------------------------
* ver 1.0.0
*       by �̳�2010.12.15
*/

#ifndef _TARGET_POSITION_H_
#define _TARGET_POSITION_H_

#include "game_graphic_header.h"
#include "i_target_position.h"

/**
* @brief
*		Ŀ���λ�ü�������ʵ��һЩ�����Ĺ���
*/
class BaseTargetPosition : public ITargetPosition
{
public:
	virtual ~BaseTargetPosition();

	/**
	* @brief
	*		����
	*/
	virtual void update(float t);

	/**
	* @brief
	*		����Ŀ���������ص���Ϣ
	*/
	virtual void set_info(const char* info_name, const char* info_value);

	/**
	* @brief
	*		��ȡĿ����λ��
	*/
	virtual H3DVec3 get_target_position();

private:
	typedef std::map<std::string, std::string> InfoCollection;

	InfoCollection		m_info_collection;
};



#endif //_TARGET_POSITION_H_
