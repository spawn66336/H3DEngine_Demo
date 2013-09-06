#include "interact_plane.h"
CInteract_Plane::CInteract_Plane():
m_Normal(0.0f, 0.0f, 1.0f),
m_fDistance(0.0f)
{
}

CInteract_Plane::CInteract_Plane(const CInteract_Plane& rhs):
m_Normal(rhs.m_Normal),
m_fDistance(rhs.m_fDistance)
{
}

CInteract_Plane::CInteract_Plane(const H3DVec3& normal, float dist)
{
	ReCalculate(normal, dist);
}

CInteract_Plane::CInteract_Plane(const H3DVec3& normal, const H3DVec3& point)
{
	ReCalculate(normal, point);
}


CInteract_Plane::CInteract_Plane(const H3DVec3& point0, const H3DVec3& point1, const H3DVec3& point2)
{
	ReCalculate(point0, point1, point2);
}