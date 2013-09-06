#ifndef ZP_H3DFPSCAMERA
#define ZP_H3DFPSCAMERA

#include "dMathHeader.h"
#include "engine_interface.h"

namespace ZPH3D
{ 

	class H3DFPSCamera
	{
	public: 
		H3DFPSCamera(void);
		virtual ~H3DFPSCamera(void);

		H3DVec3 GetUpVec( void ) const { return m_v3Up; }
		void SetUpVec( const H3DVec3& vec ){ m_v3Up = vec; }

		H3DVec3 GetDirVec( void ) const { return m_v3Dir; }
		void SetDirVec( const H3DVec3& vec ){ m_v3Dir = vec; }

		H3DVec3 GetRightVec( void ) const { return m_v3Right; }
		void SetRightVec( const H3DVec3& vec ){ m_v3Right = vec; }

		H3DVec3 GetPos( void ) const { return m_v3Pos; }
		void SetPos( const H3DVec3& pos ){ m_v3Pos = pos; }

		H3DQuat GetRotate( void ) const { return m_qRotate; }
		void SetRotate( const H3DQuat& q ){ m_qRotate = q; }

		float GetSpeed( void ) const { return m_fSpeed; }
		void SetSpeed( const float speed ){ m_fSpeed = speed; }

		void SetLookAtDir( const H3DVec3& at );
		void SetLookAtPos( const H3DVec3& at );

		void RotateWithUp( const float thetaInRad );
		void RotateWithDir( const float thetaInRad );
		void RotateWithRight( const float thetaInRad );

		void MoveAlongDirVec( const unsigned int elapseMS , const bool inv = false );
		void MoveAlongRightVec( const unsigned int elapseMS , const bool inv = false );

		void Apply( H3DI::IRender* pRenderer );
		void Apply( H3DI::IScene* pScene );

	protected:
	
		//根据旋转四元数更新各向量朝向
		void _UpdateCameraCoords( void );

	protected:

		H3DVec3 m_v3Up;			//摄像机上向量
		H3DVec3	m_v3Dir;			//摄像机所面对的方向向量
		H3DVec3 m_v3Right;		//摄像机右向量
		H3DVec3 m_v3Pos;			//摄像机位置
		H3DQuat m_qRotate;		//摄像机用旋转四元数
		float			m_fSpeed;		//摄像机速度

	protected:

		static const H3DVec3 CAM_INIT_UP;		//摄像机初始上向量
		static const H3DVec3 CAM_INIT_RIGHT;	//摄像机初始右向量
		static const H3DVec3 CAM_INIT_DIR;		//摄像机初始朝向量
		static const H3DVec3 CAM_INIT_POS;		//摄像机初始位置
	};

}//namespace ZPH3D

#endif //ZP_H3DFPSCAMERA