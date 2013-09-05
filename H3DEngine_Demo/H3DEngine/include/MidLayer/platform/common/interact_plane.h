#ifndef _TUI_INTERACT_PLANE_H_
#define _TUI_INTERACT_PLANE_H_
#include <common_lib/mathlib/dMathHeader.h>
//!顶点和平面的关系
	enum EPlaneSide
	{
		//!在平面上
		PS_Self = 0,
		//!在平面正向
		PS_Positive,
		//!在平面反向
		PS_Negative
	};

	//!平面类
	/**
	平面方程是Ax + By + Cz + D = 0
	*/
class CInteract_Plane
{
public:
	CInteract_Plane();
	CInteract_Plane(const CInteract_Plane& rhs);
	//!给定平面法线方向和距原点的距离
	CInteract_Plane(const H3DVec3& normal, float dist);
	//!给定平面法线方向和平面上的一个点
	CInteract_Plane(const H3DVec3& normal, const H3DVec3& point);
	//!给定平面上的三个点
	CInteract_Plane(const H3DVec3& point0, const H3DVec3& point1, const H3DVec3& point2);
	//!检测点在平面的哪个方向
    inline EPlaneSide GetSide(const H3DVec3& point)const
	{
		float dist = point.Dot(m_Normal)+m_fDistance;
		if (dist>0.0f)
			return PS_Positive;
		else if (dist<0.0f)
			return PS_Negative;
		else
			return PS_Self;
	}
	//!计算点和平面距离
	inline float GetDistance(const H3DVec3& point)const
	{
		return m_Normal.Dot(point)+m_fDistance;
	}
	//!根据给定的面法线和距原点距离重新计算平面方程
	inline void ReCalculate(const H3DVec3& normal, float dist)
	{
		m_fDistance = dist;
		m_Normal = normal;
	}
	//!根据给定的面法线和面上一点重新计算平面方程
	inline void ReCalculate(const H3DVec3& normal, const H3DVec3& point)
	{
		m_Normal = normal;
		m_fDistance = -m_Normal.Dot(point);
	}
	//!根据面上三个点重新计算平面方程
	inline void ReCalculate(const H3DVec3& point0, const H3DVec3& point1, const H3DVec3& point2)
	{
		H3DVec3 edge0 = point1 - point0;
		H3DVec3 edge1 = point2 - point0;
		m_Normal = edge0.Cross(edge1);
		m_Normal.Normalize();
		m_fDistance = -m_Normal.Dot(point0);
	}
	//!方程规一化
	inline void Normalize()
	{
		float len = m_Normal.Normalize();
		m_fDistance /= len;
	}
	//!面法线
	H3DVec3 m_Normal;
	//!距原点距离
	float m_fDistance;

	bool operator==(const CInteract_Plane& rhs) const
	{
		return (rhs.m_fDistance == m_fDistance && rhs.m_Normal == m_Normal);
	}
};
#endif