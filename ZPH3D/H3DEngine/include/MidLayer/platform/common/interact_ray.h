#ifndef _TUI_INTERACT_RAY_H_
#define _TYU_INTERACT_RAY_H_
#include <common_lib/mathlib/dMathHeader.h>
class CInteract_Ray
{
public:
	CInteract_Ray():
	  m_Origin(0.0f, 0.0f, 0.0f),
		  m_Dir(1.0f, 0.0f, 0.0f)
	  {
	  }

	  CInteract_Ray(const H3DVec3& origin, const H3DVec3& dir):
	  m_Origin(origin),
		  m_Dir(dir)
	  {
		  m_Dir.Normalize();
	  }
	  //!起点
	  H3DVec3 m_Origin;
	  //!方向
	  H3DVec3 m_Dir;
};
#endif