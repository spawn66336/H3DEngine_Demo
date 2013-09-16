#include "H3DScene.h"
#include "H3DFPSCamera.h"
#include "H3DDressSelector.h"
#include "H3DActionSelector.h"

namespace ZPH3D
{ 

	H3DScene::H3DScene( const String& name , H3DI::IRender* pRenderer): 
	m_pRenderer(pRenderer),
	m_pH3DLevel(NULL)
	{
		ZP_ASSERT( NULL != m_pRenderer ); 
		m_pH3DLevel = m_pRenderer->CreateLevel( name.c_str() ); 
		float v4Ambient[]= { 0.25f , 0.25f , 0.25f , 1.0f };
		m_pH3DLevel->SetAmbientColor( v4Ambient );
		m_pH3DLevel->SetActorAmbientColor( v4Ambient );
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

		for( int iAdornmentSlot = 0 ; 
			iAdornmentSlot < H3DI::ACTOR_ADORNMENT_NUM ; 
			iAdornmentSlot++ )
		{
			pActor->SetAdornmentVisibility( static_cast<H3DI::EActorAdornmentPosition>(iAdornmentSlot) , true );
		}

		_GetActorList( male ).push_back( pActor );

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
 
	H3DI::IModel* H3DScene::CreateDml( 
		const String& path , 
		const int mat_lod  , 
		const bool cast_shadow , 
		const bool reflect )
	{
		H3DI::sCreateOp createOp;
		createOp.mat_lod = mat_lod;
		
		H3DI::IModel* pDml  = NULL;
		if( reflect )
		{
			float FloorPlane[] = { 0.0f , 0.0f , 1.0f , 0.0f };
			H3DI::IReflectModel* pRefectDml 
				= m_pRenderer->CreateReflectModel( path.c_str() );
			pRefectDml->SetPlane( FloorPlane );
			pDml = pRefectDml; 
		}else{
			pDml =  m_pRenderer->CreateModel( createOp , path.c_str()  ); 
		}

		ZP_ASSERT( NULL != pDml );

		unsigned int uiFlags = pDml->GetFlags();
		if( cast_shadow )
		{
			uiFlags |= H3DI::I_ENTITY_CAST_SHADOW; 
		}else{
			uiFlags &= ~H3DI::I_ENTITY_CAST_SHADOW; 
		} 
		if( reflect )
		{
			uiFlags |= H3DI::I_ENTITY_RENDERIN_REFLECTION;
		} 

		pDml->SetFlags( uiFlags );
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


	H3DI::IPrePassLight* H3DScene::CreateLightBeam( 
		const H3DVec3& pos , const H3DVec3& lookAt , const H3DVec4& color  , const float timeOffset )
	{
		H3DI::IPrePassLight* pLightBeam = m_pRenderer->CreatePrePassLight( H3DI::LIGHT_PROJECT ); 
		ZP_ASSERT( NULL != pLightBeam ); 

		pLightBeam->SetLightAffectParam( H3DI::AFFECT_ALL );

		float fInnerAngle = 30.0f;
		float fOutterAngle = 40.0f;
		float fNear = 0.0f;
		float fFar = 4.0f;

		pLightBeam->SetPosition( pos );
		pLightBeam->SetColor( (float*)(&color.x));

		pLightBeam->SetAngle( fInnerAngle , fOutterAngle );
		pLightBeam->SetRange( fNear , 50.0f ); 
		pLightBeam->SetIntensity( 3.0f , 5.0f );
		pLightBeam->SetLightEnable( true );

		pLightBeam->CreateLightBeam( H3DI::E_H3D_BEAM_UE );
		pLightBeam->SetBeamAngle( fInnerAngle , fOutterAngle );
		pLightBeam->SetBeamRange( fNear , fFar );
		pLightBeam->SetBeamBrightness( 1.0f ); 
		pLightBeam->SetBeamVisable( true );

		H3DLightBeamControl lightCtrl( pLightBeam );
		lightCtrl.SetTimeOffset( timeOffset );
		lightCtrl.SetLookAt( lookAt );

		m_lightCtrls.push_back( lightCtrl );
		m_lightBeams.push_back( pLightBeam );
		m_pH3DLevel->AttachModel((H3DI::IMoveObject*)pLightBeam,H3DI::SL_Lights);
		return pLightBeam;
	}


	void H3DScene::Update( const float elapse )
	{
		//更新男性角色
		ActorList_t::iterator itActor = m_maleActors.begin();
		while( itActor != m_maleActors.end() )
		{
			(*itActor)->Update( elapse );
			(*itActor)->UpdateAdornment( elapse );
			itActor++;
		}

		//更新女性角色
		itActor = m_femaleActors.begin();
		while( itActor != m_femaleActors.end() )
		{
			(*itActor)->Update( elapse );
			(*itActor)->UpdateAdornment( elapse );
			itActor++;
		}

		//更新宠物
		PetList_t::iterator itPet = m_pets.begin();
		while( itPet != m_pets.end() )
		{
			(*itPet)->Update( elapse );
			(*itPet)->UpdateAdornment( elapse );
			itPet++;
		}

		//更新点光源
		LightList_t::iterator itPointLight = m_pointLights.begin();
		while( itPointLight != m_pointLights.end() )
		{
			(*itPointLight)->Update( elapse );
			itPointLight++;
		}

		//更新方向光源
		LightList_t::iterator itDirLight = m_dirLights.begin();
		while( itDirLight != m_dirLights.end() )
		{ 
			(*itDirLight)->Update( elapse );
			itDirLight++;
		} 

		//更新投影光源
		LightList_t::iterator itProjLight = m_projLights.begin();
		while( itProjLight != m_projLights.end() )
		{ 
			(*itProjLight)->Update( elapse );
			itProjLight++;
		}

		//更新光柱
		LightList_t::iterator itLightBeam = m_lightBeams.begin();
		while( itLightBeam != m_lightBeams.end() )
		{ 
			(*itLightBeam)->Update( elapse );
			itLightBeam++;
		}

		LightCtrlList_t::iterator itLightCtrl = m_lightCtrls.begin();
		while( itLightCtrl != m_lightCtrls.end() )
		{
			(*itLightCtrl).Update( elapse );
			itLightCtrl++;
		}

		//更新模型
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
		ActorList_t::iterator itActor = m_maleActors.begin();
		while( itActor != m_maleActors.end() )
		{
			m_pH3DLevel->DetachModel( (*itActor) ); 
			(*itActor)->Release();
			itActor++;
		}
		m_maleActors.clear();

		itActor = m_femaleActors.begin();
		while( itActor != m_femaleActors.end() )
		{
			m_pH3DLevel->DetachModel( (*itActor) ); 
			(*itActor)->Release();
			itActor++;
		}
		m_femaleActors.clear();

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

		LightList_t::iterator itLightBeam = m_lightBeams.begin();
		while( itLightBeam != m_lightBeams.end() )
		{
			m_pH3DLevel->DetachModel( (*itLightBeam) );
			(*itLightBeam)->Release();
			itLightBeam++;
		}
		m_lightBeams.clear();

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

	void H3DScene::RandomActorDress( H3DDressSelector& dressSelector )
	{
		ActorList_t::iterator itActor = m_maleActors.begin();
		while( itActor != m_maleActors.end() )
		{ 
			dressSelector.RandomActorDresses( (*itActor) , true );  
			itActor++;
		}

		itActor = m_femaleActors.begin();
		while( itActor != m_femaleActors.end() )
		{ 
			dressSelector.RandomActorDresses( (*itActor) , false );  
			itActor++;
		}
	}


	void H3DScene::SwitchActorAction( H3DActionSelector& actionSelector )
	{ 
		ActorList_t::iterator itActor = m_maleActors.begin();
		while( itActor != m_maleActors.end() )
		{  
			H3DI::IAnimationChannel* pAnimCh = 
				(*itActor)->GetAnmChannel( 0 ); 
			pAnimCh->SetAction( actionSelector.GetCurrAction(true).c_str() , true );     
			itActor++;
		}

		itActor = m_femaleActors.begin();
		while( itActor != m_femaleActors.end() )
		{ 
			H3DI::IAnimationChannel* pAnimCh = 
				(*itActor)->GetAnmChannel( 0 ); 
			pAnimCh->SetAction( actionSelector.GetCurrAction(false).c_str() , false );     
			itActor++;
		}

		PetList_t::iterator itPet = m_pets.begin();
		while( itPet != m_pets.end() )
		{
			H3DI::IAnimationChannel* pAnimCh = 
				(*itPet)->GetAnmChannel( 0 ); 
			pAnimCh->SetAction( actionSelector.GetCurrAction(false).c_str() , false );     
			itPet++;
		}
	}


	H3DScene::ActorList_t& H3DScene::_GetActorList( const bool male )
	{
		if( male )
		{
			return m_maleActors; 
		}
		return m_femaleActors;
	}
	 

	H3DLightBeamControl::H3DLightBeamControl():
	m_v3Pos( INIT_POS ),
	m_v3Dir( INIT_DIR ),
	m_v3Up( INIT_UP ),
	m_v3Right( INIT_RIGHT ),
	m_fHoriSpeed( INIT_HORI_SPEED ),
	m_fVertSpeed( INIT_VERT_SPEED ),
	m_fHoriBeginAngle( - 40.0f ),
	m_fHoriEndAngle( 40.0f ),
	m_fHoriAngle(0.0f),
	m_fT( 0.0f ),
	m_pLightBeam(NULL)
	{ 
		m_qRotate.Identity();
	}


	H3DLightBeamControl::H3DLightBeamControl( const H3DLightBeamControl& ctrl )
	{
		this->operator=( ctrl );
	}


	H3DLightBeamControl::H3DLightBeamControl( H3DI::IPrePassLight* pLight ):
	m_v3Pos( INIT_POS ),
	m_v3Dir( INIT_DIR ),
	m_v3Up( INIT_UP ),
	m_v3Right( INIT_RIGHT ),
	m_fHoriSpeed( INIT_HORI_SPEED ),
	m_fVertSpeed( INIT_VERT_SPEED ), 
	m_fHoriBeginAngle( - 40.0f ),
	m_fHoriEndAngle( 40.0f ),
	m_fHoriAngle(0.0f),
	m_fT( 0.0f ),
	m_pLightBeam( pLight )
	{ 
		m_pLightBeam->GetPosition( m_v3Pos );
		m_qRotate.Identity();
	}

	H3DLightBeamControl::~H3DLightBeamControl()
	{  
	}

	void H3DLightBeamControl::Update( const float elapse )
	{
		float fRadHoriSpeed = DEG2RAD( m_fHoriSpeed );
		float fRadVertSpeed = DEG2RAD( m_fVertSpeed ); 
		m_fT += elapse; 
		float fFactor = H3DMath::Cos( fRadHoriSpeed * m_fT );
		fFactor = ( fFactor + 1.0f ) * 0.5f; 
		m_fHoriAngle = m_fHoriBeginAngle + ( m_fHoriEndAngle - m_fHoriBeginAngle )*fFactor; 
		_Apply();
	}

	void H3DLightBeamControl::SetPos( const H3DVec3& pos  )
	{
		m_v3Pos = pos;  
		_Apply();
	}

	void H3DLightBeamControl::_Apply( void )
	{
		H3DQuat q;
		q.FromAngleAxis( m_fHoriAngle , m_v3Up );

		H3DVec3 v3Right = m_v3Right*q;
		v3Right.Normalize();

		H3DVec3 v3Dir= m_v3Dir*q;
		v3Dir.Normalize();

		H3DVec3 v3Up= m_v3Up*q;
		v3Up.Normalize();

		m_pLightBeam->SetDirection( -v3Dir ); 
		m_pLightBeam->SetPosition( m_v3Pos );

		//H3DVec4 v[4];
		//v[0].Set( v3Right.x , v3Right.y , v3Right.z , 0.0f );  
		//v[1].Set( v3Dir.x , v3Dir.y , v3Dir.z , 0.0f ); 
		//v[2].Set( v3Up.x , v3Up.y , v3Up.z , 0.0f );  
		//v[3].Set( m_v3Pos.x , m_v3Pos.y , m_v3Pos.z , 1.0f );
		//H3DMat4 mat( v[0] , v[1] , v[2] , v[3] ); 
		//m_pLightBeam->SetLocationMatrix( mat.Transpose() );
	}
	 
	void H3DLightBeamControl::SetLookAt( const H3DVec3& lookAt )
	{
		m_v3Dir = lookAt - m_v3Pos;
		m_v3Dir.Normalize();
		m_v3Right = m_v3Dir.Cross( m_v3Up );
		m_v3Right.Normalize();
		m_v3Up = m_v3Right.Cross( m_v3Dir );
		m_v3Up.Normalize();
	}

	 
	H3DLightBeamControl& H3DLightBeamControl::operator=( const H3DLightBeamControl& rhs )
	{
		m_v3Pos = rhs.m_v3Pos;
		m_v3Dir = rhs.m_v3Dir;
		m_v3Up = rhs.m_v3Up;
		m_v3Right = rhs.m_v3Right;
		m_qRotate = rhs.m_qRotate;		 
		m_fHoriSpeed = rhs.m_fHoriSpeed; 
		m_fVertSpeed = rhs.m_fVertSpeed; 
		m_fHoriBeginAngle = rhs.m_fHoriBeginAngle;
		m_fHoriEndAngle = rhs.m_fHoriEndAngle;
		m_fHoriAngle = rhs.m_fHoriAngle;
		m_fT = rhs.m_fT;			
		m_pLightBeam = rhs.m_pLightBeam;
		return *this;
	}

	void H3DLightBeamControl::SetTimeOffset( const float offset )
	{
		m_fT += offset;
	}

	

	const H3DVec3 H3DLightBeamControl::INIT_POS( 0.0f , 0.0f , 0.0f ); 
	const H3DVec3 H3DLightBeamControl::INIT_DIR( 0.0f , 1.0f , 0.0f ); 
	const H3DVec3 H3DLightBeamControl::INIT_RIGHT( -1.0f , 0.0f , 0.0f ); 
	const H3DVec3 H3DLightBeamControl::INIT_UP( 0.0f , 0.0f , 1.0f );
	const float H3DLightBeamControl::INIT_HORI_SPEED = 90.0f; 
	const float H3DLightBeamControl::INIT_VERT_SPEED = 40.0f;


}//namespace ZPH3D