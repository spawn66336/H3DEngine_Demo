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
		H3DQuat m_qRotate;		//��ת��Ԫ��
		float			m_fHoriSpeed;		//ˮƽҡ���ٶ�
		float			m_fVertSpeed;		//��ֱҡ���ٶ�
		float			m_fT;
		float			m_fHoriBeginAngle;
		float			m_fHoriEndAngle;
		float			m_fHoriAngle;
		H3DI::IPrePassLight* m_pLightBeam;

	protected: 
		static const H3DVec3 INIT_UP;			//��ʼ������
		static const H3DVec3 INIT_RIGHT;	//��ʼ������
		static const H3DVec3 INIT_DIR;		//��ʼ������
		static const H3DVec3 INIT_POS;		//��ʼλ��
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
		H3DI::ILevel*	 m_pH3DLevel;  //�ؿ�
		ActorList_t m_maleActors;		//���Խ�ɫ�б�
		ActorList_t m_femaleActors;		//Ů�Խ�ɫ�б�
		ModelList_t m_models;				//ģ���б�

		LightList_t m_pointLights;			//���Դ�б�
		LightList_t m_dirLights;				//������б� 
		LightList_t m_projLights;			//ͶӰ���б�
		LightList_t m_lightBeams;			//�����б�
		LightCtrlList_t m_lightCtrls;

		PetList_t	 m_pets;					//�����б�
	};

}//namespace ZPH3D

#endif//ZPH3DSCENE