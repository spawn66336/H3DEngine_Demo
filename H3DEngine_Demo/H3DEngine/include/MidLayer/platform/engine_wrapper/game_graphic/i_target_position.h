#ifndef _I_TARGET_POSITION_H_
#define _I_TARGET_POSITION_H_

#include "game_graphic_header.h"

/**
* @brief
*		Ŀ���λ�ü�����
* @description
*		����׷��ƺ������Ŀ����λ��
*/
class ITargetPosition
{
public:
	virtual ~ITargetPosition(){}

	/**
	* @brief
	*		����
	*/
	virtual void update(float t) = 0;

	/**
	* @brief
	*		����Ŀ���������ص���Ϣ
	*/
	virtual void set_info(const char* info_name, const char* info_value) = 0;

	/**
	* @brief
	*		��ȡĿ����λ��
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
	@brief ����ƽ��ֵ����ʱ��С�������������Ŀ��ϵͳ�����Զ�������
	*/
	virtual void setMinInterval(int interval) = 0;
	/**
	@brief ȡ��С������������ڼ��������²������
	*/
	virtual int getMinInterval() = 0;
};

#endif //_I_TARGET_POSITION_H_
