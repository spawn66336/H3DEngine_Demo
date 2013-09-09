#include "H3DFPSCamera.h"

namespace ZPH3D
{ 
	const H3DVec3 H3DFPSCamera::CAM_INIT_DIR( 0.0f , 1.0f , 0.0f );
	const H3DVec3 H3DFPSCamera::CAM_INIT_RIGHT( 1.0f , 0.0f , 0.0f ); 
	const H3DVec3 H3DFPSCamera::CAM_INIT_UP( 0.0f , 0.0f , 1.0f ); 
	const H3DVec3 H3DFPSCamera::CAM_INIT_POS( 0.0f , -10.0f , 0.0f ); 


	H3DFPSCamera::H3DFPSCamera(void)
	{ 
		m_qRotate.Identity();
		m_v3Up = CAM_INIT_UP;			
		m_v3Dir = CAM_INIT_DIR;			
		m_v3Right = CAM_INIT_RIGHT;
		m_v3Pos = CAM_INIT_POS;
		m_fSpeed = 10.0f;

		
	}


	H3DFPSCamera::~H3DFPSCamera(void)
	{
	}

	void H3DFPSCamera::_UpdateCameraCoords( void )
	{
		m_v3Right= CAM_INIT_RIGHT*m_qRotate;
		m_v3Right.Normalize();

		m_v3Dir= CAM_INIT_DIR*m_qRotate;
		m_v3Dir.Normalize();

		m_v3Up= CAM_INIT_UP*m_qRotate;
		m_v3Up.Normalize();
	}

	void H3DFPSCamera::RotateWithUp( const float thetaInRad )
	{
		H3DQuat q;
		q.FromAngleAxis( thetaInRad, m_v3Up );
		m_qRotate=q*m_qRotate;
		m_qRotate.Normalize();
		this->_UpdateCameraCoords();
	}

	void H3DFPSCamera::RotateWithDir( const float thetaInRad )
	{
		H3DQuat q;
		q.FromAngleAxis( thetaInRad, m_v3Dir );
		m_qRotate=q*m_qRotate;
		m_qRotate.Normalize();
		this->_UpdateCameraCoords();
	}

	void H3DFPSCamera::RotateWithRight( const float thetaInRad )
	{
		H3DQuat q;
		q.FromAngleAxis( thetaInRad, m_v3Right );
		m_qRotate=q*m_qRotate;
		m_qRotate.Normalize();
		this->_UpdateCameraCoords();
	}

	void H3DFPSCamera::SetLookAtDir( const H3DVec3& at )
	{
		m_qRotate.GetRotationTo( m_v3Dir , at );
		this->_UpdateCameraCoords();
	}

	void H3DFPSCamera::SetLookAtPos( const H3DVec3& at )
	{
		H3DVec3 tmp = at - m_v3Pos; //dir是从pos射向目标
		tmp.Normalize();
		SetLookAtDir(tmp);
	}

	void H3DFPSCamera::MoveAlongDirVec( const unsigned int elapseMS  , const bool inv )
	{
		float fSpeed = m_fSpeed;
		if( inv )
		{
			fSpeed = -fSpeed;
		}

		m_v3Pos += m_v3Dir*fSpeed*( static_cast<float>(elapseMS) / 1000.0f );
	}

	void H3DFPSCamera::MoveAlongRightVec( const unsigned int elapseMS  , const bool inv  )
	{
		float fSpeed = m_fSpeed;
		if( inv )
		{
			fSpeed = -fSpeed;
		}

		m_v3Pos += m_v3Right*fSpeed*( static_cast<float>(elapseMS) / 1000.0f );
	}

	void H3DFPSCamera::Apply( H3DI::IRender* pRenderer )
	{
		pRenderer->LookAt( m_v3Pos , m_v3Pos+m_v3Dir , m_v3Up );
		pRenderer->UpdateCamera();
	}

	void H3DFPSCamera::Apply( H3DI::IScene* pScene )
	{ 
		pScene->LookAt( m_v3Pos , m_v3Pos+m_v3Dir , m_v3Up );
		pScene->UpdateCamera();
	}


}//namespace ZPH3D