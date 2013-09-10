#include "H3DScene.h"
#include "H3DFPSCamera.h"

namespace ZPH3D
{ 

	H3DScene::H3DScene( const String& name , H3DI::IRender* pRenderer): 
	m_pRenderer(pRenderer),
	m_pH3DLevel(NULL)
	{
		ZP_ASSERT( NULL != m_pRenderer ); 
		m_pH3DLevel = m_pRenderer->CreateLevel( name.c_str() ); 

		//unsigned int uiFlags = m_pH3DLevel->GetFlags();
		//m_pH3DLevel->SetFlags( uiFlags| H3DI::I_ENTITY_CAST_SHADOW );
	  
		ZP_ASSERT( NULL != m_pH3DLevel ); 
	}


	H3DScene::H3DScene(void): 
		m_pRenderer(NULL),
		m_pH3DLevel(NULL)
	{
	}


	H3DScene::~H3DScene(void)
	{
		ClearScene();
		m_pH3DLevel->Release();
	}

	H3DI::IActor* H3DScene::CreateActor( const String& name , const bool male , const int mat_lod )
	{
		H3DI::sCreateOp createOp;
		createOp.mat_lod = mat_lod;
		H3DI::IActor* pActor = 
			(H3DI::IActor*)m_pRenderer->CreateActor( createOp , name.c_str() , male , H3DI::ACTION_UPDATE_X52 );
		ZP_ASSERT( NULL != pActor );

		unsigned int uiFlags = pActor->GetFlags();
		pActor->SetFlags( uiFlags | H3DI::I_ENTITY_CAST_SHADOW |H3DI::I_ENTITY_FORCESYNCDATA );

		pActor->Update(0);
		m_actors.push_back( pActor );
		m_pH3DLevel->AttachModel( pActor , H3DI::SL_Actors );
		return pActor;
	}

	H3DI::IAvatarSkeletonModel* H3DScene::CreatePet( const String& name , const bool male , const int mat_lod )
	{
		H3DI::sCreateOp createOp;
		createOp.mat_lod = mat_lod;
		H3DI::IAvatarSkeletonModel* pPet = 
			(H3DI::IAvatarSkeletonModel*)m_pRenderer->CreateAvatarSkeletonModel( createOp , name.c_str() , male , H3DI::ACTION_UPDATE_X52 );
		ZP_ASSERT( NULL != pPet );

		unsigned int uiFlags = pPet->GetFlags();
		pPet->SetFlags( uiFlags | H3DI::I_ENTITY_CAST_SHADOW | H3DI::I_ENTITY_FORCESYNCDATA );

		pPet->Update(0);
		m_pets.push_back( pPet );
		m_pH3DLevel->AttachModel( pPet , H3DI::SL_Actors );
		return pPet;
	}
 
	H3DI::IModel* H3DScene::CreateDml( const String& path , const int mat_lod  , const bool cast_shadow )
	{
		H3DI::sCreateOp createOp;
		createOp.mat_lod = mat_lod;
		
		H3DI::IModel* pDml =  m_pRenderer->CreateModel( createOp , path.c_str()  ); 
		ZP_ASSERT( NULL != pDml );

		unsigned int uiFlags = pDml->GetFlags();
		if( cast_shadow )
		{
			pDml->SetFlags( uiFlags | H3DI::I_ENTITY_CAST_SHADOW );
		}else{
			pDml->SetFlags( uiFlags & ~H3DI::I_ENTITY_CAST_SHADOW );
		}
		pDml->Update(0);
		m_models.push_back( pDml );
		m_pH3DLevel->AttachModel( pDml , H3DI::SL_DetailObj );
		return pDml;
	}

	H3DI::IPrePassLight* H3DScene::CreateLight( const H3DI::LightAffectParam affect , const H3DI::LIGHT_TYPE type )
	{
		H3DI::IPrePassLight* pLight = m_pRenderer->CreatePrePassLight( type ); 
		ZP_ASSERT( NULL != pLight );

		unsigned int uiFlags = pLight->GetFlags();
		pLight->SetFlags( uiFlags| H3DI::I_ENTITY_CAST_SHADOW );
		 
		pLight->SetLightAffectParam( affect );
		_GetLightList( type ).push_back( pLight );
		m_pH3DLevel->AttachModel((H3DI::IMoveObject*)pLight,H3DI::SL_Lights);
		return pLight;
	}

	void H3DScene::Update( const float elapse )
	{
		ActorList_t::iterator itActor = m_actors.begin();
		while( itActor != m_actors.end() )
		{
			(*itActor)->Update( elapse );
			(*itActor)->UpdateAdornment( elapse );
			itActor++;
		}

		PetList_t::iterator itPet = m_pets.begin();
		while( itPet != m_pets.end() )
		{
			(*itPet)->Update( elapse );
			(*itPet)->UpdateAdornment( elapse );
			itPet++;
		}

		LightList_t::iterator itPointLight = m_pointLights.begin();
		while( itPointLight != m_pointLights.end() )
		{
			(*itPointLight)->Update( elapse );
			itPointLight++;
		}

		LightList_t::iterator itDirLight = m_dirLights.begin();
		while( itDirLight != m_dirLights.end() )
		{ 
			(*itDirLight)->Update( elapse );
			itDirLight++;
		} 

		LightList_t::iterator itProjLight = m_projLights.begin();
		while( itProjLight != m_projLights.end() )
		{ 
			(*itProjLight)->Update( elapse );
			itProjLight++;
		}

		ModelList_t::iterator itModel = m_models.begin();
		while( itModel != m_models.end() )
		{
			(*itModel)->Update( elapse );
			itModel++;
		}

		m_pH3DLevel->Update( elapse );
	}

	H3DI::ILevel* H3DScene::GetH3DLevel( void )
	{
		return m_pH3DLevel;
	}

	void H3DScene::RestructScene()
	{
		m_pH3DLevel->RestructScene();
	}

	void H3DScene::ClearScene( void )
	{
		ActorList_t::iterator itActor = m_actors.begin();
		while( itActor != m_actors.end() )
		{
			m_pH3DLevel->DetachModel( (*itActor) ); 
			(*itActor)->Release();
			itActor++;
		}
		m_actors.clear();

		PetList_t::iterator itPet = m_pets.begin();
		while( itPet != m_pets.end() )
		{
			m_pH3DLevel->DetachModel( (*itPet) ); 
			(*itPet)->Release();
			itPet++;
		}
		m_pets.clear();

		LightList_t::iterator itPointLight = m_pointLights.begin();
		while( itPointLight != m_pointLights.end() )
		{
			m_pH3DLevel->DetachModel( (*itPointLight) );
			(*itPointLight)->Release(); 
			itPointLight++;
		}
		m_pointLights.clear();

		LightList_t::iterator itDirLight = m_dirLights.begin();
		while( itDirLight != m_dirLights.end() )
		{
			m_pH3DLevel->DetachModel( (*itDirLight) );
			(*itDirLight)->Release(); 
			itDirLight++;
		}
		m_dirLights.clear();

		LightList_t::iterator itProjLight = m_projLights.begin();
		while( itProjLight != m_projLights.end() )
		{
			m_pH3DLevel->DetachModel( (*itProjLight) );
			(*itProjLight)->Release(); 
			itProjLight++;
		}
		m_projLights.clear();

		ModelList_t::iterator itModel = m_models.begin();
		while( itModel != m_models.end() )
		{
			m_pH3DLevel->DetachModel( (*itModel) );
			(*itModel)->Release(); 
			itModel++;
		}
		m_models.clear();
	}

	void H3DScene::PushToRenderer( void )
	{ 
		m_pRenderer->PushScene( m_pH3DLevel );
	}

	void H3DScene::ApplyCamera( H3DFPSCamera* pCam )
	{
		pCam->Apply( m_pRenderer );
		pCam->Apply( m_pH3DLevel );
	}

	H3DScene::LightList_t& H3DScene::_GetLightList( const H3DI::LIGHT_TYPE type )
	{
		if( H3DI::LIGHT_DIR == type )
		{
			return m_dirLights;
		}else if( H3DI::LIGHT_POINT == type ){
			return m_pointLights;
		}else if( H3DI::LIGHT_PROJECT == type ){
			return m_projLights;
		} 
		return m_pointLights;
	}

	 

}//namespace ZPH3D