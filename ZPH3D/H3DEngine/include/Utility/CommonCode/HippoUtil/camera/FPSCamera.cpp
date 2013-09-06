#include "FPSCamera.h"
#include "CameraControlMap.h"
#include "../HippoGlobal.h"
#include "../log/HippoLog.h"
#include "engine_interface.h"
#include "../input/Hippo_InputManager.h"
#include <boost/function.hpp>
#include <boost/bind.hpp>
using namespace std;
using namespace boost;


FPSCamera::FPSCamera()
{
	m_quat.Identity();
	m_Pos.Set(0,0,0);
	UpdateCameraCoord();
	m_bRightHasDown=false;
	m_nLastMouseX=-1;
	m_nLastMouseY=-1;
	m_vDirection.Set(0,0,0);
	m_vVelocity.Set(0,0,0);
}
FPSCamera::~FPSCamera()
{

}

//from base
const H3DVec3& FPSCamera::GetPos()
{
	return m_Pos;
}

const H3DVec3& FPSCamera::GetViewAtDir()
{
	return m_ViewAtDir;
}

const H3DVec3& FPSCamera::GetUp()
{
	return m_UpDir;
}

void FPSCamera::SetPos(const H3DVec3& pos)
{
	m_Pos=pos;
}

void FPSCamera::SetLookAtDir(const H3DVec3& at)
{
	m_quat.GetRotationTo(INIT_VIEWAT,at);
	UpdateCameraCoord();
}

void FPSCamera::SetLookAtPos(const H3DVec3& at)
{
	H3DVec3 tmp=at-m_Pos; //dir是从pos射向目标
	tmp.Normalize();
	SetLookAtDir(tmp);
}

const H3DQuat& FPSCamera::GetRotation()
{
	return m_quat;
}

int FPSCamera::FrameUpdate(float fElapsedTime)
{
	Hippo_InputManager* pInput=GetInputManager();
	m_vDirection.Set(0,0,0);
	// Update acceleration vector based on keyboard state
	if( pInput->IsKeyDown(Camera_ControlMap::GetFPSCameraKeyCodeFromLogicKey(CAM_MOVE_FORWARD) ))
		m_vDirection.y += 1.0f;
	if( pInput->IsKeyDown(Camera_ControlMap::GetFPSCameraKeyCodeFromLogicKey(CAM_MOVE_BACKWARD) ))
		m_vDirection.y -= 1.0f;

	if( pInput->IsKeyDown(Camera_ControlMap::GetFPSCameraKeyCodeFromLogicKey(CAM_MOVE_UP) ))
		m_vDirection.z += 1.0f;
	if( pInput->IsKeyDown(Camera_ControlMap::GetFPSCameraKeyCodeFromLogicKey(CAM_MOVE_DOWN) ))
		m_vDirection.z -= 1.0f;

	if( pInput->IsKeyDown(Camera_ControlMap::GetFPSCameraKeyCodeFromLogicKey(CAM_STRAFE_RIGHT) ))
		m_vDirection.x += 1.0f;
	if( pInput->IsKeyDown(Camera_ControlMap::GetFPSCameraKeyCodeFromLogicKey(CAM_STRAFE_LEFT) ))
		m_vDirection.x -= 1.0f;
	//按下shift 速度加倍
	if( pInput->IsKeyDown(Camera_ControlMap::GetFPSCameraKeyCodeFromLogicKey(CAM_SHIFTDOWN) ))
		m_vDirection *= 2.0f;

	const int velocity_per_second=10;
	if(m_vDirection.LengthFast()==0)
	{
		//减速
		m_vVelocity *= 0.7f; //阻尼系数0.7
		static float timer=0;
		timer+=fElapsedTime;
		if(timer>1) //1s内减速
		{
			timer=0;
			m_vVelocity.Set(0,0,0);
		}
	}
	else
	{
		m_vVelocity=m_vDirection * velocity_per_second *fElapsedTime;
	}
	SetPosDelta(m_vVelocity);
	m_vDirection.Set(0,0,0);

	return 1;
	//Hippo_WriteConsoleAndLog(CC_WHITE,"fps FrameUpdate");
	//const int velocity_per_second=10;
	//if(m_vDirection.LengthFast()==0)
	//{
	//	//减速
	//	m_vVelocity *= 0.7f; //阻尼系数0.7
	//	static float timer=0;
	//	timer+=fElapsedTime;
	//	if(timer>1) //1s内减速
	//	{
	//		timer=0;
	//		m_vVelocity.Set(0,0,0);
	//	}
	//}
	//else
	//{
	//	m_vVelocity=m_vDirection * velocity_per_second *fElapsedTime;
	//}
	//SetPosDelta(m_vVelocity);
	//m_vDirection.Set(0,0,0);

	//return 1;
}

void FPSCamera::LookAt(H3DI::IRender* pRender)
{
	//Hippo_WriteConsole(CC_WHITE,"fps camera dir=%f,%f,%f",m_ViewAtDir.x,m_ViewAtDir.y,m_ViewAtDir.z);
	pRender->LookAt(m_Pos,m_Pos+m_ViewAtDir,m_UpDir);
	pRender->UpdateCamera();


}



void FPSCamera::UpdateCameraCoord()
{
	m_RightDir=INIT_RIGHT*m_quat;
	m_RightDir.Normalize();

	m_ViewAtDir=INIT_VIEWAT*m_quat;
	m_ViewAtDir.Normalize();

	//m_UpDir=m_RightDir.Cross(m_ViewAtDir);
	m_UpDir=INIT_UP*m_quat;
	m_UpDir.Normalize();
}


void FPSCamera::RoateWithZAix(float degreeInRad)
{
	//return;
	H3DQuat tquat;
	tquat.FromAngleAxis(-degreeInRad,H3DVec3(0,0,1));
	m_quat=tquat*m_quat;
	m_quat.Normalize();
	UpdateCameraCoord();
}

void FPSCamera::RoateWithXAix(float degreeInRad)
{

	H3DQuat tquat;
	tquat.FromAngleAxis(degreeInRad,m_RightDir);
	m_quat=tquat*m_quat;
	m_quat.Normalize();
	UpdateCameraCoord();
}


//!设置移动的距离
void FPSCamera::SetPosDelta(const H3DVec3& posdelta)
{
	H3DVec3 t=posdelta* m_quat;
	m_Pos+=t;

}

void FPSCamera::SetPosDelta(float len)
{
	SetPosDelta(H3DVec3(len,len,len));
}
MouseKeyCallback FPSCamera::GetMouseRightDownCallback()
{
	MouseKeyCallback cb= boost::bind(&FPSCamera::OnMouseRightDown,this,_1);
	return cb;
}
MouseKeyCallback FPSCamera::GetMouseMoveCallback()
{
	MouseKeyCallback cb= boost::bind(&FPSCamera::OnMouseMove,this,_1);
	return cb;
}
MouseKeyCallback FPSCamera::GetMouseRightUpCallback()
{
	MouseKeyCallback cb= boost::bind(&FPSCamera::OnMouseRightUp,this,_1);
	return cb;
}
MouseWheelCallback FPSCamera::GetMouseWheelCallback()
{
	MouseWheelCallback cb= boost::bind(&FPSCamera::OnMouseWheel,this,_1);
	return cb;
}
KeyCallback FPSCamera::GetKeyDownCallback()
{
	KeyCallback cb= boost::bind(&FPSCamera::OnKeyDown,this,_1);
	return cb;
}

int FPSCamera::OnMouseRightDown(HippoMouseEvent& e)
{
	m_bRightHasDown=true;
	POINT ptCurMousePos;
	GetCursorPos( &ptCurMousePos );
	m_nLastMouseX=ptCurMousePos.x;
	m_nLastMouseY=ptCurMousePos.y;

	return 1;
}
int FPSCamera::OnMouseRightUp(HippoMouseEvent& e)
{
	m_bRightHasDown=false;
	return 1;
}
int FPSCamera::OnMouseMove(HippoMouseEvent& e)
{
	if(!m_bRightHasDown)
		return 1;

	POINT ptCurMousePos;
	GetCursorPos( &ptCurMousePos );

	int mouseX_dis = ptCurMousePos.x-m_nLastMouseX;
	int mousey_dis = ptCurMousePos.y-m_nLastMouseY;

	RoateWithZAix(mouseX_dis*0.3f);
	RoateWithXAix(-mousey_dis*0.3f);

	m_nLastMouseX=ptCurMousePos.x;
	m_nLastMouseY=ptCurMousePos.y;
	return 1;
}
int FPSCamera::OnMouseWheel(HippoWheelEvent& w)
{

	return 1;
}
int FPSCamera::OnKeyDown(unsigned int k)
{
	return 1;
	//Hippo_WriteConsole(CC_WHITE,"fps key down");
	m_vDirection.Set(0,0,0);
	if( k==(Camera_ControlMap::GetFPSCameraKeyCodeFromLogicKey(CAM_MOVE_FORWARD) ))
		m_vDirection.y = 1.0f;
	else if( k==(Camera_ControlMap::GetFPSCameraKeyCodeFromLogicKey(CAM_MOVE_BACKWARD) ))
		m_vDirection.y = -1.0f;

	if( k==(Camera_ControlMap::GetFPSCameraKeyCodeFromLogicKey(CAM_MOVE_UP) ))
		m_vDirection.z = 1.0f;
	else if( k==(Camera_ControlMap::GetFPSCameraKeyCodeFromLogicKey(CAM_MOVE_DOWN) ))
		m_vDirection.z = -1.0f;

	if( k==(Camera_ControlMap::GetFPSCameraKeyCodeFromLogicKey(CAM_STRAFE_RIGHT) ))
		m_vDirection.x = 1.0f;
	else if( k==(Camera_ControlMap::GetFPSCameraKeyCodeFromLogicKey(CAM_STRAFE_LEFT) ))
		m_vDirection.x = -1.0f;

	//float fElapsedTime=1.0f / Hippo_GetFrameTime();
	//const int velocity_per_second=10;
	//if(m_vDirection.LengthFast()==0)
	//{
	//	//减速
	//	m_vVelocity *= 0.7f; //阻尼系数0.7
	//	static float timer=0;
	//	timer+=fElapsedTime;
	//	if(timer>1) //1s内减速
	//	{
	//		timer=0;
	//		m_vVelocity.Set(0,0,0);
	//	}
	//}
	//else
	//{
	//	m_vVelocity=m_vDirection * velocity_per_second *fElapsedTime;
	//}
	//SetPosDelta(m_vVelocity);
	//m_vDirection.Set(0,0,0);

	return 1;
}

void FPSCamera::SyncCamera(CameraBase* pCam)
{
	SetPos(pCam->GetPos());
	SetLookAtDir(pCam->GetViewAtDir());

}