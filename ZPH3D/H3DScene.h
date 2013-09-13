#ifndef ZPH3DSCENE
#define ZPH3DSCENE
#include "ZPDependency.h"
#include "engine_interface.h"
#include "dMathHeader.h"
 
namespace ZPH3D
{
	class H3DFPSCamera;
	class H3DDressSelector;
	class H3DActionSelector;

	class H3DLightBeamControl
	{
	public:
		H3DLightBeamControl( H3DI::IPrePassLight* pLight );
		H3DLightBeamControl( const H3DLightBeamControl& ctrl );
		~H3DLightBeamControl();

		void SetPos( const H3DVec3& pos  ); 

		void SetLookAt( const H3DVec3& lookAt );

		void SetTimeOffset( const float offset );

		void Update( const float elapse );

		H3DLightBeamControl& operator=( const H3DLightBeamControl& rhs );

	protected:
		 

		void _Apply( void );

		H3DLightBeamControl();

	protected:

		H3DVec3 m_v3Pos;
		H3DVec3 m_v3Dir;
		H3DVec3 m_v3Up;
		H3DVec3 m_v3Right;
		H3DQuat m_qRotate;		//旋转四元数
		float			m_fHoriSpeed;		//水平摇摆速度
		float			m_fVertSpeed;		//垂直摇摆速度
		float			m_fT;
		float			m_fHoriBeginAngle;
		float			m_fHoriEndAngle;
		float			m_fHoriAngle;
		H3DI::IPrePassLight* m_pLightBeam;

	protected: 
		static const H3DVec3 INIT_UP;			//初始上向量
		static const H3DVec3 INIT_RIGHT;	//初始右向量
		static const H3DVec3 INIT_DIR;		//初始朝向量
		static const H3DVec3 INIT_POS;		//初始位置
		static const float		   INIT_HORI_SPEED;
		static const float			INIT_VERT_SPEED;
	};

	class H3DScene
	{
	public:

		typedef std::vector<H3DI::IActor*> ActorList_t;
		typedef std::vector<H3DI::IModel*> ModelList_t;
		typedef std::vector<H3DI::IPrePassLight*> LightList_t;
		typedef std::vector<H3DLightBeamControl> LightCtrlList_t;
		typedef std::vector<H3DI::IAvatarSkeletonModel*> PetList_t;

	public:
		H3DScene( const String& name ,  H3DI::IRender* pRenderer );
		virtual ~H3DScene(void);

		H3DI::IActor* CreateActor( const String& name , const bool male , const int mat_lod = 0 );
		 
		H3DI::IAvatarSkeletonModel* CreatePet( const String& name , const bool male , const int mat_lod = 0 );

		H3DI::IModel* CreateDml( const String& path , const int mat_lod = 0 , const bool cast_shadow = true , const bool refect = false ); 
		 
		H3DI::IPrePassLight* CreateLight( const H3DI::LightAffectParam affect , const H3DI::LIGHT_TYPE type );

		H3DI::IPrePassLight* CreateLightBeam( const H3DVec3& pos , const H3DVec3& lookAt , const H3DVec4& color , const float timeOffset = 0.0f );
		 
		void Update( const float elapse );

		void PushToRenderer( void );

		H3DI::ILevel* GetH3DLevel( void );

		void RestructScene();

		void ClearScene( void );

		void ApplyCamera( H3DFPSCamera* pCam );

		void RandomActorDress( H3DDressSelector& dressSelector );

		void SwitchActorAction( H3DActionSelector& actionSelector );

	protected:

		H3DScene( void );

		LightList_t& _GetLightList( const H3DI::LIGHT_TYPE type );

		ActorList_t& _GetActorList( const bool male );

	protected:
		H3DI::IRender* m_pRenderer;
		H3DI::ILevel*	 m_pH3DLevel;  //关卡
		ActorList_t m_maleActors;		//男性角色列表
		ActorList_t m_femaleActors;		//女性角色列表
		ModelList_t m_models;				//模型列表

		LightList_t m_pointLights;			//点光源列表
		LightList_t m_dirLights;				//方向光列表 
		LightList_t m_projLights;			//投影光列表
		LightList_t m_lightBeams;			//光柱列表
		LightCtrlList_t m_lightCtrls;

		PetList_t	 m_pets;					//宠物列表
	};

}//namespace ZPH3D

#endif//ZPH3DSCENE