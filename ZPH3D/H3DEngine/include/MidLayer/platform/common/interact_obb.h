/*/******************
/* @file	interact_assist_info.h
/* @brief	3D交互系统obb类
/* @author	胡浩
/* @date	2011/11/03
/* @note
******************/

#ifndef _TUI_INTERACT_OBB_H_
#define _TUI_INTERACT_OBB_H_
#include <platform/engine_wrapper/game_graphic/game_graphic_interface.h>
enum EOBBPlane
{
	//!上面
	OBBP_Top = 0,
	//!下面
	OBBP_Bottom,
	//!左面
	OBBP_Left,
	//!右面
	OBBP_Right,
	//!近剪裁面
	OBBP_Front,
	//!远剪裁面
	OBBP_Back,

	OBBP_Count
};
//!物体和射线相对关系
enum EHitType
{
	//!射线起点在物体内
	Hit_Inside = 0,
	//!射线起点在物体外,射线和物体相交
	Hit_Outside,
	//!射线起点在物体外,射线和物体不相交
	Hit_None
};
class CInteract_OBB
{
public:

	CInteract_OBB();

	~CInteract_OBB();



	EHitType Hit(const class CInteract_Ray& ray)const;
	//!和射线做相交检测,返回交点
	EHitType Hit(const class CInteract_Ray& ray,H3DVec3 &point,float &fScale)const;

	bool Contain(const H3DVec3& point)const;
	//将包围体保持原有形状，并移动到某个姿态下，
	virtual void SetWorldTransform(const H3DMat4 &mat);
	//设置长宽高
	void SetLenghtWeithHight(float l,float w,float h);
	//将包围体保持原有形状，并移动到某个姿态下，
	virtual void SetInitWorldTransform(const H3DMat4 &mat);

	//更新个个顶点和三个轴的数据
	void UpdataOBB();

	//得顶点坐标
	const H3DVec3* GetCorner() const{return m_Corners;};

	//根据相机的旋转来 改变包围盒的角度

	//void UpdateOBBWithCamera(H3DI::IRender* pRender,H3DVec3 vPos); //vPos 包围盒的位置
	void BindToBone(H3DI::IRender * pRender,CAvatarSkeletonModelEntity *pEntity,char * BoneName,H3DVec3 offPos,bool bToCamera);

	void BindToMatrix(H3DI::IRender * pRender,H3DMat4 mMatrix,H3DVec3 offPos,bool bToCamera);

	// 画包围盒
	void DrawOBB(H3DI::IRender * pRender);

	void GetLenghtWeithHight(float &l,float &w,float &h);

    const H3DVec3& GetOffset();

	bool IsIntersect(const CInteract_OBB& other);

protected:
	/// 画OBB边
	void DrawOBBEdge(int begin_vertex_index, int end_vertex_index, H3DI::IRender* renderer);

	H3DVec3 U(int index) const ;
	double E(int index) const ;
protected:

	//世界矩阵
	H3DMat4 m_WorldTransform;
	//
	//初始的世界矩阵，一般情况下，这个矩阵为单位阵，但是有时候，一个OBB绑在骨骼上，那么它在初始状态下就会有一个偏移
	H3DMat4 m_InitWorldTransform;
	//长宽高
	float m_fLenth;
	float m_fWidth;
	float m_fHeight;
    //偏移
    H3DVec3 m_Off;

	float m_fBoundingSphereRadius;

public:
	//以下数据都是受矩阵和长宽高影响的。
	//中心坐标
	H3DVec3 m_vCenterPos;
	//!八个顶点坐标
	H3DVec3 m_Corners[8];
	H3DVec3 m_vFront;
	H3DVec3 m_vRight;
	H3DVec3 m_vUp;
	H3DPlane m_Planes[OBBP_Count];
	uint32	m_nLastUpdateFrame;


};
#endif

