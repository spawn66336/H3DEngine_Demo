/*/******************
/* @file	interact_assist_info.h
/* @brief	3D����ϵͳobb��
/* @author	����
/* @date	2011/11/03
/* @note
******************/

#ifndef _TUI_INTERACT_OBB_H_
#define _TUI_INTERACT_OBB_H_
#include <platform/engine_wrapper/game_graphic/game_graphic_interface.h>
enum EOBBPlane
{
	//!����
	OBBP_Top = 0,
	//!����
	OBBP_Bottom,
	//!����
	OBBP_Left,
	//!����
	OBBP_Right,
	//!��������
	OBBP_Front,
	//!Զ������
	OBBP_Back,

	OBBP_Count
};
//!�����������Թ�ϵ
enum EHitType
{
	//!���������������
	Hit_Inside = 0,
	//!���������������,���ߺ������ཻ
	Hit_Outside,
	//!���������������,���ߺ����岻�ཻ
	Hit_None
};
class CInteract_OBB
{
public:

	CInteract_OBB();

	~CInteract_OBB();



	EHitType Hit(const class CInteract_Ray& ray)const;
	//!���������ཻ���,���ؽ���
	EHitType Hit(const class CInteract_Ray& ray,H3DVec3 &point,float &fScale)const;

	bool Contain(const H3DVec3& point)const;
	//����Χ�屣��ԭ����״�����ƶ���ĳ����̬�£�
	virtual void SetWorldTransform(const H3DMat4 &mat);
	//���ó����
	void SetLenghtWeithHight(float l,float w,float h);
	//����Χ�屣��ԭ����״�����ƶ���ĳ����̬�£�
	virtual void SetInitWorldTransform(const H3DMat4 &mat);

	//���¸�������������������
	void UpdataOBB();

	//�ö�������
	const H3DVec3* GetCorner() const{return m_Corners;};

	//�����������ת�� �ı��Χ�еĽǶ�

	//void UpdateOBBWithCamera(H3DI::IRender* pRender,H3DVec3 vPos); //vPos ��Χ�е�λ��
	void BindToBone(H3DI::IRender * pRender,CAvatarSkeletonModelEntity *pEntity,char * BoneName,H3DVec3 offPos,bool bToCamera);

	void BindToMatrix(H3DI::IRender * pRender,H3DMat4 mMatrix,H3DVec3 offPos,bool bToCamera);

	// ����Χ��
	void DrawOBB(H3DI::IRender * pRender);

	void GetLenghtWeithHight(float &l,float &w,float &h);

    const H3DVec3& GetOffset();

	bool IsIntersect(const CInteract_OBB& other);

protected:
	/// ��OBB��
	void DrawOBBEdge(int begin_vertex_index, int end_vertex_index, H3DI::IRender* renderer);

	H3DVec3 U(int index) const ;
	double E(int index) const ;
protected:

	//�������
	H3DMat4 m_WorldTransform;
	//
	//��ʼ���������һ������£��������Ϊ��λ�󣬵�����ʱ��һ��OBB���ڹ����ϣ���ô���ڳ�ʼ״̬�¾ͻ���һ��ƫ��
	H3DMat4 m_InitWorldTransform;
	//�����
	float m_fLenth;
	float m_fWidth;
	float m_fHeight;
    //ƫ��
    H3DVec3 m_Off;

	float m_fBoundingSphereRadius;

public:
	//�������ݶ����ܾ���ͳ����Ӱ��ġ�
	//��������
	H3DVec3 m_vCenterPos;
	//!�˸���������
	H3DVec3 m_Corners[8];
	H3DVec3 m_vFront;
	H3DVec3 m_vRight;
	H3DVec3 m_vUp;
	H3DPlane m_Planes[OBBP_Count];
	uint32	m_nLastUpdateFrame;


};
#endif

