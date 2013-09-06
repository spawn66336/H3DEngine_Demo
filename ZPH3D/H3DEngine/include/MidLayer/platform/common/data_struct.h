/**
* @file operation.h
* ����ļ�����Ҫ�����ǣ�
*			������ý��㹩�ϲ�ʹ�õ����ݽṹ
* 
* @author ֣�ɿ�(zhengchengjun@h3d.com.cn)
* 
* ������־
* ----------------------------------------------------
* ver 1.0.0
*       by ֣�ɿ�2012.7.18
*/

#ifndef _DATA_STRUCT_H_
#define _DATA_STRUCT_H_

#include "H3DColor.h"

/// ����ĵƹ�����
struct LightCachedProperty
{
	H3DColor m_light_color; /// �ƹ���ɫ
	H3DColor m_shadow_color; /// ��Ӱ��ɫ
	float m_intensity; /// �ƹ�ǿ��
	int m_priority; /// �ƹ����ȼ�
	bool m_enable_shadow; /// �Ƿ�����shadow
	float m_near_range; /// �����ⷶΧ
	float m_far_range; /// Զ���ⷶΧ
	float m_location[4][4]; /// �ƹ�λ���볯��
	int m_light_affect_param; /// �ƹ�Ӱ�������
	float m_inner_angle, m_outter_angle; /// �ƹ������

	LightCachedProperty()
	{
		memset(this, 0, sizeof(LightCachedProperty));
	}
};

inline void MiddleAlwaysAssertFailed( const char *file, int line, const char *expression )
{
#if defined(_WIN32) || defined(__WIN32__) || defined(WIN32)
	_asm int 3;
#else
	__asm__("int3");
#endif
}

#define middleAlwaysAssert(exp)	(int)( (exp) || (MiddleAlwaysAssertFailed(__FILE__, __LINE__, #exp), 0) )

#ifdef _FINAL_RELEASE
#define middleAssert(exp)
#else
#define middleAssert(exp)	 middleAlwaysAssert(exp)
#endif

#endif //_DATA_STRUCT_H_