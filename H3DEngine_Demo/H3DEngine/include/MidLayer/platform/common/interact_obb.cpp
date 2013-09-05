
#include "interact_obb.h"
#include "interact_ray.h"
#include "interact_plane.h"

EHitType CInteract_OBB::Hit(const CInteract_Ray& ray)const
{
	assert("not implemented"&&false);
	return Hit_None;
}

EHitType CInteract_OBB::Hit( const class CInteract_Ray& ray,H3DVec3 &point ,float& fScale) const
{
	float fMinScale = 100000.0f;
	//H3DList<float> pointlist;
	bool bIn = false;
	if(Contain(ray.m_Origin))
	{
		bIn = true;
	}
	bool bInter = false;
	//求点射线的距离
	H3DVec3 vdisO_P = m_vCenterPos-ray.m_Origin;
	float lenthPes = vdisO_P.Dot(ray.m_Dir);
	//点到直线距离
	float dis2  = vdisO_P.LengthSqr()-lenthPes * lenthPes;
	if(dis2 < m_fBoundingSphereRadius*m_fBoundingSphereRadius)
	{
		for(uint32 i = 0 ; i < OBBP_Count; ++i)
		{
			if((!bIn && (m_Planes[i].Normal().Dot(ray.m_Dir)<0.0f))
				||(bIn &&(m_Planes[i].Normal().Dot(ray.m_Dir)>0.0f)))
			{
				float f;
				if(	m_Planes[i].RayIntersection(ray.m_Origin,ray.m_Dir,f) && f > 0)
				{
					//当交点在OBB中的时候，才是真的有交点
					if(Contain(ray.m_Origin + ray.m_Dir*f))
					{
						bInter = true;
						if(f < fMinScale)
						{
							fMinScale = f;
						}
					}

				}

			}
		}
		if(bInter)
		{
			point = ray.m_Origin + ray.m_Dir*fMinScale;
			fScale = fMinScale;
			if(bIn)
			{
				return Hit_Inside;
			}
			else
			{
				return Hit_Outside;
			}
		}
		else
		{
			return Hit_None;
		}

	}
	else
	{
		return Hit_None;
	}
}
bool CInteract_OBB::Contain(const H3DVec3& point)const
{
	H3DVec3 vCenterToPoint;
	vCenterToPoint = point -m_vCenterPos;
	if((	fabs(vCenterToPoint.Dot(m_vUp)) < m_fHeight*0.5f + 0.01f)
		&&(fabs(vCenterToPoint.Dot(m_vRight)) < m_fLenth*0.5f + 0.01f)
		&&(fabs(vCenterToPoint.Dot(m_vFront)) < m_fWidth*0.5f + + 0.01f))
	{
		return true;
	}
	else
	{
		return false;
	}


}

CInteract_OBB::CInteract_OBB():
m_fLenth(0.0f),
m_fWidth(0.0f),
m_fHeight(0.0f),
m_vCenterPos(0.0f,0.0f,0.0f),
m_vFront(0.0f,1.0f,0.0f),
m_vRight(1.0f,0.0f,0.0f),
m_vUp(0.0f,0.0f,1.0f),
m_nLastUpdateFrame(-1),
m_fBoundingSphereRadius(0.0f)
{

}

void CInteract_OBB::SetWorldTransform( const H3DMat4 &mat )
{
	m_WorldTransform = mat * m_InitWorldTransform;
	//更新OBB的操作不用每帧都用，此处可以优化
	UpdataOBB();
}

void CInteract_OBB::SetLenghtWeithHight( float l,float w,float h )
{
	m_fLenth = l;
	m_fWidth = w;
	m_fHeight = h;
	m_fBoundingSphereRadius = H3DMath::Sqrt((l * l + w * w + h * h) * 0.25f);

}
void CInteract_OBB::GetLenghtWeithHight(float &l, float &w, float &h)
{
	l = m_fLenth;
	w = m_fWidth;
	h = m_fHeight;
}
void CInteract_OBB::UpdataOBB()
{
	//if(m_nLastUpdateFrame != SceneManager::GetSingleton().GetFrameCount())
	{
		m_vCenterPos.Set(m_WorldTransform[0][3],m_WorldTransform[1][3],m_WorldTransform[2][3]);
		m_vRight.Set(m_WorldTransform[0][0],m_WorldTransform[1][0],m_WorldTransform[2][0]);
		m_vFront.Set(m_WorldTransform[0][1],m_WorldTransform[1][1],m_WorldTransform[2][1]);
		m_vUp.Set(m_WorldTransform[0][2],m_WorldTransform[1][2],m_WorldTransform[2][2]);
		// 		m_Corners[0].Set(m_Min.x, m_Min.y, m_Min.z);
		// 		m_Corners[1].Set(m_Min.x, m_Min.y, m_Max.z);
		// 		m_Corners[2].Set(m_Min.x, m_Max.y, m_Min.z);
		// 		m_Corners[3].Set(m_Min.x, m_Max.y, m_Max.z);
		// 		m_Corners[4].Set(m_Max.x, m_Min.y, m_Min.z);
		// 		m_Corners[5].Set(m_Max.x, m_Min.y, m_Max.z);
		// 		m_Corners[6].Set(m_Max.x, m_Max.y, m_Min.z);
		// 		m_Corners[7].Set(m_Max.x, m_Max.y, m_Max.z);
		m_Corners[0] = m_vCenterPos - m_vRight*m_fLenth * 0.5f - m_vFront * m_fWidth  * 0.5f - m_vUp * m_fHeight  * 0.5f;
		m_Corners[1] = m_Corners[0] + m_vUp * m_fHeight;

		m_Corners[2] = m_Corners[0] + m_vFront * m_fWidth;
		m_Corners[3] = m_Corners[2] + m_vUp * m_fHeight;

		m_Corners[4] = m_Corners[0] + m_vRight * m_fLenth;
		m_Corners[5] = m_Corners[4] + m_vUp * m_fHeight;

		m_Corners[6] = m_Corners[4] + m_vFront * m_fWidth;
		m_Corners[7] = m_Corners[6] + m_vUp * m_fHeight;

		//通过距离和法线生成平面
		m_Planes[OBBP_Top].SetNormal(m_vUp);
		m_Planes[OBBP_Top].SetDist(m_Corners[7].Dot(m_vUp));

		m_Planes[OBBP_Bottom].SetNormal(-m_vUp);
		m_Planes[OBBP_Bottom].SetDist(m_Corners[0].Dot(-m_vUp));

		m_Planes[OBBP_Front].SetNormal(m_vFront);
		m_Planes[OBBP_Front].SetDist(m_Corners[7].Dot(m_vFront));

		m_Planes[OBBP_Back].SetNormal(-m_vFront);
		m_Planes[OBBP_Back].SetDist(m_Corners[0].Dot(-m_vFront));

		m_Planes[OBBP_Left].SetNormal(-m_vRight);
		m_Planes[OBBP_Left].SetDist(m_Corners[0].Dot(-m_vRight));

		m_Planes[OBBP_Right].SetNormal(m_vRight);
		m_Planes[OBBP_Right].SetDist(m_Corners[7].Dot(m_vRight));
		//m_nLastUpdateFrame = SceneManager::GetSingleton().GetFrameCount();

	}
}

CInteract_OBB::~CInteract_OBB()
{

}

void CInteract_OBB::SetInitWorldTransform( const H3DMat4 &mat )
{

	m_InitWorldTransform = mat;
}
//void CInteract_OBB::UpdateOBBWithCamera(H3DI::IRender* pRender,H3DVec3 vPos)
//{
//	H3DMat4 mat4;
//	mat4.Identity();
//	SetInitWorldTransform(mat4);
//
//	//计算旋转 和位置移动
//
//	pRender->GetViewMatrix(mat4);
//	mat4.InverseSelf();
//
//	//得到公告栏的位置
//	mat4[0].w = vPos.x;
//	mat4[1].w = vPos.y;
//	mat4[2].w = vPos.z;
//
//	SetWorldTransform(mat4);
//}
void CInteract_OBB::DrawOBB(H3DI::IRender * pRender)
{
	if (!pRender)
	{
		return;
	}
	//for(int i = 0; i < 8; i++)
	//{
	//	float box[6] = {m_Corners[i].x - 0.05f, m_Corners[i].y - 0.05f, m_Corners[i].z - 0.05f, m_Corners[i].x + 0.05f, m_Corners[i].y + 0.05f,m_Corners[i].z + 0.05f};
	//	float bc0[4] = {1.0f, 1.0f, 1.0f, 1.0f};
	//	pRender->DrawAABB(box,bc0, 10);
	//}

	DrawOBBEdge(0, 1, pRender);
	DrawOBBEdge(0, 2, pRender);
	DrawOBBEdge(2, 3, pRender);
	DrawOBBEdge(1, 3, pRender);
	DrawOBBEdge(0, 4, pRender);
	DrawOBBEdge(1, 5, pRender);
	DrawOBBEdge(4, 5, pRender);
	DrawOBBEdge(2, 6, pRender);
	DrawOBBEdge(3, 7, pRender);
	DrawOBBEdge(6, 7, pRender);
	DrawOBBEdge(5, 7, pRender);
	DrawOBBEdge(4, 6, pRender);
}
void CInteract_OBB::BindToBone(H3DI::IRender * pRender,CAvatarSkeletonModelEntity *pEntity,char * BoneName,H3DVec3 offPos,bool bToCamera)
{
	if (!pRender)
	{
		return;
	}
	if (!pEntity)
	{
		pRender->OutPutLogError(H3DI::CRITICAL_ERROR, H3DI::OutPut_File, "[Error] entity doesn't exist");
		return;
	}
	float bonePos[3] = {0};
	H3DI::ISkeleton* ts = pEntity->getISkeletonModel()->GetSkeleton();

	if(ts) 
	{
		H3DI::IBone* pBone = ts->GetBone(BoneName);
		middleAlwaysAssert(pBone);
		H3DVec3 vPos;
		if (pBone)
		{
			pEntity->getISkeletonModel()->GetRendingMatrix(pBone->GetID()).GetTranslation(vPos);
		}

		vPos = vPos + offPos;

		H3DMat4 mat4;
		mat4.Identity();
		SetInitWorldTransform(mat4);

		//计算旋转 和位置移动

		if (bToCamera)
		{
			pRender->GetViewMatrix(mat4);
			mat4.InverseSelf();
		}
		//得到公告栏的位置
		mat4[0].w = vPos.x;
		mat4[1].w = vPos.y;
		mat4[2].w = vPos.z;

		SetWorldTransform(mat4);

		//char tmp[260];
		//sprintf_s(tmp, "[Obb Info] bone pos %f %f %f", bonePos[0], bonePos[1], bonePos[2]);
		//pRender->OutPutLogInfo(H3DI::MODEL_INFO, H3DI::OutPut_File, tmp);
		//sprintf_s(tmp, "[Obb Info] vpos %f %f %f", vPos[0], vPos[1], vPos[2]);
		//pRender->OutPutLogInfo(H3DI::MODEL_INFO, H3DI::OutPut_File, tmp);
	}
	else
	{
		pRender->OutPutLogError(H3DI::CRITICAL_ERROR, H3DI::OutPut_File, "[Error] skeleton doesn't exist");
	}
}
void CInteract_OBB::BindToMatrix(H3DI::IRender * pRender,H3DMat4 mMatrix,H3DVec3 offPos,bool bToCamera)
{
	if (!pRender)
	{
		return;
	}
	float bonePos[3] = {0};

	bonePos[0] = mMatrix[0].w;
	bonePos[1] = mMatrix[1].w;
	bonePos[2] = mMatrix[2].w;
    m_Off = offPos;
	H3DVec3 vPos(bonePos[0],bonePos[1],bonePos[2]);
	vPos = vPos + offPos;

	H3DMat4 mat4;
	mat4.Identity();
	SetInitWorldTransform(mat4);

	//计算旋转 和位置移动

	if (bToCamera)
	{
		pRender->GetViewMatrix(mat4);
		mat4.InverseSelf();
		mat4[0].w = 0;
		mat4[1].w = 0;
		mat4[2].w = 0;
		
		mat4[3] = H3DVec4(0.0f, 0.0f, 0.0f, 1.0f);

		H3DMat4 temp;
		temp.Identity();
		temp[0].w = vPos.x;
		temp[1].w = vPos.y;
		temp[2].w = vPos.z;

		mat4 = temp * mat4;
	}
	else
	{
		//得到公告栏的位置
		mat4[0].w = vPos.x;
		mat4[1].w = vPos.y;
		mat4[2].w = vPos.z;
	}

	SetWorldTransform(mat4);

	//char tmp[260];
	//sprintf_s(tmp, "[Obb Info] bone pos %f %f %f", bonePos[0], bonePos[1], bonePos[2]);
	//pRender->OutPutLogInfo(H3DI::MODEL_INFO, H3DI::OutPut_File, tmp);
	//sprintf_s(tmp, "[Obb Info] vpos %f %f %f", vPos[0], vPos[1], vPos[2]);
	//pRender->OutPutLogInfo(H3DI::MODEL_INFO, H3DI::OutPut_File, tmp);
}

const H3DVec3& CInteract_OBB::GetOffset()
{
    return m_Off;
}

bool CInteract_OBB::IsIntersect(const CInteract_OBB& other)
{
	double c[3][3];
	double absC[3][3];
	double d[3];

	double r0 = 0, r1 = 0, r = 0;
	int i = 0;

	const double cutoff = 0.999999;
	bool existParallelPair = false;

	H3DVec3 diff = this->m_vCenterPos - other.m_vCenterPos;

	//c0 + t*a0
	for ( i = 0 ; i < 3 ; i++ )
	{
		c[0][i] = U(0).Dot(other.U(i));
		absC[0][i] = abs(c[0][i]);
		if ( absC[0][i] > cutoff )
		{
			existParallelPair = true;
		}
	}
	d[0] = diff.Dot(U(0));
	r = abs(d[0]);
	r0 = E(0);
	r1 = other.E(0)*absC[0][0] + other.E(1)*absC[0][1] + other.E(2)*absC[0][2];
	if ( r > r0 + r1 )
	{
		return false;
	}

	//c0 + t*a1
	for ( i = 0 ; i < 3 ; i++ )
	{
		c[1][i] = U(1).Dot(other.U(i));
		absC[1][i] = abs(c[1][i]);
		if ( absC[1][i] > cutoff )
		{
			existParallelPair = true;
		}
	}
	d[1] = diff.Dot(U(1));
	r = abs(d[1]);
	r0 = E(1);
	r1 = other.E(0)*absC[1][0] + other.E(1)*absC[1][1] + other.E(2)*absC[1][2];
	if ( r > r0 + r1 )
	{
		return false;
	}

	//c0 + t*a2
	for ( i = 0 ; i < 3 ; i++ )
	{
		c[2][i] = U(2).Dot(other.U(i));
		absC[2][i] = abs(c[2][i]);
		if ( absC[2][i] > cutoff )
		{
			existParallelPair = true;
		}
	}
	d[2] = diff.Dot(U(2));
	r = abs(d[2]);
	r0 = E(2);
	r1 = other.E(0)*absC[2][0] + other.E(1)*absC[2][1] + other.E(2)*absC[2][2];
	if ( r > r0 + r1 )
	{
		return false;
	}

	//c0 + t*b0
	r = abs(diff.Dot(U(0)));
	r0 = E(0)*absC[0][0] + E(1)*absC[1][0] + E(2)*absC[2][0];
	r1 = E(0);
	if ( r > r0 + r1 )
	{
		return false;
	}

	//c0 + t*b1
	r = abs(diff.Dot(U(1)));
	r0 = E(0)*absC[0][1] + E(1)*absC[1][1] + E(2)*absC[2][1];
	r1 = E(1);
	if ( r > r0 + r1 )
	{
		return false;
	}

	//c) + t*b2
	r = abs(diff.Dot(U(2)));
	r0 = E(0)*absC[0][2] + E(1)*absC[1][2] + E(2)*absC[2][2];
	r1 = E(2);
	if ( r > r0 + r1 )
	{
		return false;
	}

	if ( existParallelPair )
	{
		return true;
	}

	//c0 + t*a0Xb0
	r = abs(d[2]*c[1][0] - d[1]*c[2][0]);
	r0 = E(1)*absC[2][0] + E(2)*absC[1][0];
	r1 = E(1)*absC[0][2] + E(2)*absC[0][1];
	if ( r > r0 + r1 )
	{
		return false;
	}

	return true;
}

void CInteract_OBB::DrawOBBEdge( int begin_vertex_index, int end_vertex_index, H3DI::IRender* renderer )
{
	if (begin_vertex_index < 0 || begin_vertex_index >= 8 ||
		end_vertex_index < 0 || end_vertex_index >= 8 || !renderer)
	{
		return;
	}

	float color[4] = {1.0f, 1.0f, 1.0f, 1.0f};
	renderer->DrawSegment(m_Corners[begin_vertex_index], m_Corners[end_vertex_index], color, 10.0f);
}

H3DVec3 CInteract_OBB::U(int index) const 
{
	switch(index)
	{
	case 0:
		return m_vUp;
	case 1:
		return m_vFront;
	case 2:
		return m_vRight;
	}

	return H3DVec3(0, 0, 0);
}

double CInteract_OBB::E(int index) const 
{
	switch(index)
	{
	case 0:
		return m_fHeight;
	case 1:
		return m_fWidth;
	case 2:
		return m_fLenth;
	}

	return 0;
}