/**
* @file operation.h
* 这个文件的主要功能是：
*			声明了媒体层供上层使用的数据结构
* 
* @author 郑成俊(zhengchengjun@h3d.com.cn)
* 
* 更改日志
* ----------------------------------------------------
* ver 1.0.0
*       by 郑成俊2012.7.18
*/

#ifndef _DATA_STRUCT_H_
#define _DATA_STRUCT_H_

#include "H3DColor.h"

/// 缓冲的灯光属性
struct LightCachedProperty
{
	H3DColor m_light_color; /// 灯光颜色
	H3DColor m_shadow_color; /// 阴影颜色
	float m_intensity; /// 灯光强度
	int m_priority; /// 灯光优先级
	bool m_enable_shadow; /// 是否生成shadow
	float m_near_range; /// 近处光范围
	float m_far_range; /// 远处光范围
	float m_location[4][4]; /// 灯光位置与朝向
	int m_light_affect_param; /// 灯光影响的类型
	float m_inner_angle, m_outter_angle; /// 灯光内外角

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