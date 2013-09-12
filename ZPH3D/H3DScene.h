#ifndef ZPH3DSCENE
#define ZPH3DSCENE
#include "ZPDependency.h"
#include "engine_interface.h"
 
namespace ZPH3D
{
	class H3DFPSCamera;
	class H3DDressSelector;
	class H3DActionSelector;

	class H3DScene
	{
	public:

		typedef std::vector<H3DI::IActor*> ActorList_t;
		typedef std::vector<H3DI::IModel*> ModelList_t;
		typedef std::vector<H3DI::IPrePassLight*> LightList_t;
		typedef std::vector<H3DI::IAvatarSkeletonModel*> PetList_t;

	public:
		H3DScene( const String& name ,  H3DI::IRender* pRenderer );
		virtual ~H3DScene(void);

		H3DI::IActor* CreateActor( const String& name , const bool male , const int mat_lod = 0 );
		 
		H3DI::IAvatarSkeletonModel* CreatePet( const String& name , const bool male , const int mat_lod = 0 );

		H3DI::IModel* CreateDml( const String& path , const int mat_lod = 0 , const bool cast_shadow = true , const bool refect = false ); 
		 
		H3DI::IPrePassLight* CreateLight( const H3DI::LightAffectParam affect , const H3DI::LIGHT_TYPE type );
		 
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

		PetList_t	 m_pets;					//�����б�
	};

}//namespace ZPH3D

#endif//ZPH3DSCENE