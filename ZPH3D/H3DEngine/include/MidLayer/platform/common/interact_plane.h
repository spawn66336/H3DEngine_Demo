#ifndef _TUI_INTERACT_PLANE_H_
#define _TUI_INTERACT_PLANE_H_
#include <common_lib/mathlib/dMathHeader.h>
//!�����ƽ��Ĺ�ϵ
	enum EPlaneSide
	{
		//!��ƽ����
		PS_Self = 0,
		//!��ƽ������
		PS_Positive,
		//!��ƽ�淴��
		PS_Negative
	};

	//!ƽ����
	/**
	ƽ�淽����Ax + By + Cz + D = 0
	*/
class CInteract_Plane
{
public:
	CInteract_Plane();
	CInteract_Plane(const CInteract_Plane& rhs);
	//!����ƽ�淨�߷���;�ԭ��ľ���
	CInteract_Plane(const H3DVec3& normal, float dist);
	//!����ƽ�淨�߷����ƽ���ϵ�һ����
	CInteract_Plane(const H3DVec3& normal, const H3DVec3& point);
	//!����ƽ���ϵ�������
	CInteract_Plane(const H3DVec3& point0, const H3DVec3& point1, const H3DVec3& point2);
	//!������ƽ����ĸ�����
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
	//!������ƽ�����
	inline float GetDistance(const H3DVec3& point)const
	{
		return m_Normal.Dot(point)+m_fDistance;
	}
	//!���ݸ������淨�ߺ;�ԭ��������¼���ƽ�淽��
	inline void ReCalculate(const H3DVec3& normal, float dist)
	{
		m_fDistance = dist;
		m_Normal = normal;
	}
	//!���ݸ������淨�ߺ�����һ�����¼���ƽ�淽��
	inline void ReCalculate(const H3DVec3& normal, const H3DVec3& point)
	{
		m_Normal = normal;
		m_fDistance = -m_Normal.Dot(point);
	}
	//!�����������������¼���ƽ�淽��
	inline void ReCalculate(const H3DVec3& point0, const H3DVec3& point1, const H3DVec3& point2)
	{
		H3DVec3 edge0 = point1 - point0;
		H3DVec3 edge1 = point2 - point0;
		m_Normal = edge0.Cross(edge1);
		m_Normal.Normalize();
		m_fDistance = -m_Normal.Dot(point0);
	}
	//!���̹�һ��
	inline void Normalize()
	{
		float len = m_Normal.Normalize();
		m_fDistance /= len;
	}
	//!�淨��
	H3DVec3 m_Normal;
	//!��ԭ�����
	float m_fDistance;

	bool operator==(const CInteract_Plane& rhs) const
	{
		return (rhs.m_fDistance == m_fDistance && rhs.m_Normal == m_Normal);
	}
};
#endif