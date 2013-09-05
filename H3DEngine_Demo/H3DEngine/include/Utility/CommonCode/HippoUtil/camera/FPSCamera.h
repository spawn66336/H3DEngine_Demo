/********************************************************************
	created:	2011/12/26
	created:	26:12:2011   22:45
	filename: 	f:\TestHippo\TestHippo\HIPPO_FrameWork\camera\FPSCamera.h
	file path:	f:\TestHippo\TestHippo\HIPPO_FrameWork\camera
	file base:	FPSCamera
	file ext:	h
	author:		sssa2000
	
	purpose:	
*********************************************************************/
#pragma once

#include "CameraBase.h"
#include "../HippoAppBase/Win32MsgUtil.h"
class FPSCamera:public CameraBase
{
public:
	FPSCamera();
	~FPSCamera();

	//from base
	const H3DVec3& GetPos();
	const H3DVec3& GetViewAtDir();
	const H3DVec3& GetUp();
	void SetPos(const H3DVec3& pos);
	void SetLookAtDir(const H3DVec3& at);
	void SetLookAtPos(const H3DVec3& at);

	const H3DQuat& GetRotation();
	int FrameUpdate(float fElapsedTime);
	void LookAt(H3DI::IRender* pRender);

	MouseKeyCallback GetMouseRightDownCallback();
	MouseKeyCallback GetMouseMoveCallback();
	MouseKeyCallback GetMouseRightUpCallback();
	MouseWheelCallback GetMouseWheelCallback();
	KeyCallback GetKeyDownCallback();

	void SyncCamera(CameraBase* pCam);

protected:
private:

	int OnMouseRightDown(HippoMouseEvent& e);
	int OnMouseRightUp(HippoMouseEvent& e);
	int OnMouseMove(HippoMouseEvent& e);
	int OnMouseWheel(HippoWheelEvent& w);
	int OnKeyDown(unsigned int);


	H3DQuat m_quat;
	H3DVec3 m_Pos;
	H3DVec3 m_ViewAtDir;
	H3DVec3 m_UpDir;
	H3DVec3 m_RightDir;

	//!运动的方向，计算加速度会用到
	H3DVec3 m_vDirection;
	H3DVec3 m_vVelocity;

	//!更新相机的坐标系即up、right、lookat
	void UpdateCameraCoord();
	int AnalysisKeyInput(float fElapsedTime);
	int AnalysisMouseInput(float fElapsedTime);

	void SetPosDelta(const H3DVec3& posdelta);
	void SetPosDelta(float dir);

	void RoateWithZAix(float degreeInRad);
	void RoateWithXAix(float degreeInRad);

	bool m_bRightHasDown;
	int m_nLastMouseX;
	int m_nLastMouseY;
};