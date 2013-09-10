
#include "H3DEngineBox.h"
#include "dMathHeader.h"
#include "H3DFPSCamera.h"
#include "engine_interface_internal.h"
#include "EffectCore_dx.h"
#include "H3DScene.h"


namespace ZPH3D
{


H3DEngineBox::H3DEngineBox(void):
m_hWnd(NULL),
m_iWidth(0),
m_iHeight(0),
m_hEngineDll(NULL),
m_pH3DRenderer(NULL),
m_pProxyFactory(NULL),
m_pSpecEffectMgr(NULL),
m_pfCreateRenderPtr(NULL),
m_pfDeleteRenderPtr(NULL),
m_pfCreateIProxyFactoryPtr(NULL),
m_pCamera(NULL), 
m_pVB(NULL),
m_uiLastTick(0),
m_uiElapseTick(0)
{
	m_uiLastTick = ::GetTickCount();
}


H3DEngineBox::~H3DEngineBox(void)
{
}

void H3DEngineBox::Init( const HWND hWnd )
{
	String strEngineDllName = "Engine.dll";
	ZP_ASSERT( NULL == m_hEngineDll );
	m_hEngineDll = LoadLibraryA( strEngineDllName.c_str() );
	ZP_ASSERT( NULL != m_hEngineDll );

	//��ȡ��Ⱦ����������
	m_pfCreateRenderPtr = (PF_CREATERENDERPTR)GetProcAddress( m_hEngineDll , "CreateRenderer" );
	ZP_ASSERT( NULL != m_pfCreateRenderPtr );
	//��ȡ��Ⱦ��ɾ������
	m_pfDeleteRenderPtr = (PF_DELETERENDERPTR)GetProcAddress( m_hEngineDll , "DeleteRenderer" );
	ZP_ASSERT( NULL != m_pfDeleteRenderPtr );
	//��ȡIProxyFactory��������
	m_pfCreateIProxyFactoryPtr = (PF_CREATEIPROXYFACTORYPTR)GetProcAddress( m_hEngineDll , "CreateIProxyFactory");
	ZP_ASSERT( NULL != m_pfCreateIProxyFactoryPtr );

	//������Ⱦ��
	m_pH3DRenderer = m_pfCreateRenderPtr();
	ZP_ASSERT( NULL != m_pH3DRenderer);
	 
	
	//��ú��İ汾��Ϣ
	String strCoreVersionInfo = m_pH3DRenderer->GetCoreVersionInfo();

	m_hWnd = hWnd;

	RECT wndRect;
	::GetClientRect( m_hWnd , &wndRect );
	m_iWidth =  wndRect.right - wndRect.left;
	m_iHeight = wndRect.bottom - wndRect.top;

	H3DI::tWindowCreateInfoImp wndCreateInfo;
	m_pH3DRenderer->GetCurrentWindowSetting(wndCreateInfo);
	wndCreateInfo.hWnd =hWnd; 

	char strCurrDirectory[256]={0};
	GetCurrentDirectoryA( 256 , strCurrDirectory );

	//������Ⱦ����
	int iRes = m_pH3DRenderer->CreateRenderWindow( wndCreateInfo );
	ZP_ASSERT( iRes >=0 );

	m_pH3DRenderer->SetUseActionLib(true);
	m_pH3DRenderer->SetUseActionChannel(true);
	m_pH3DRenderer->SetMergBodyPart(true);
	m_pH3DRenderer->SetImageScaleFactor(0);
	m_pH3DRenderer->SetUseTriList(true);
	m_pH3DRenderer->EnableCloth( true );
	m_pH3DRenderer->SetEnableAllPhy( true );
	m_pH3DRenderer->EnableShadow( true );
	m_pH3DRenderer->EnableLightPrePassRendering( true ); 

	m_pVB = m_pH3DRenderer->GetDynamicVB();
	bool bCreateVBRes = m_pVB->CreateBuffer( 36 * 8192, 4096, - 1, - 1, 0, 0 );
	ZP_ASSERT( true == bCreateVBRes );

	m_pH3DRenderer->QueryInfo(1,(void*)(&m_isEditMode),0,0,0,0);
  
	float fColor[4]={0.0f,0.0f,0.0f,0.0f};
	m_pH3DRenderer->SetClearColor(fColor);
	m_pH3DRenderer->SetClearFlags( 
		H3DI::CLEAR_BUF_COLOR|H3DI::CLEAR_BUF_DEPTH|H3DI::CLEAR_BUF_STENCIL );

	//������Ч������
	m_pSpecEffectMgr=m_pH3DRenderer->CreateEffectManager();
	ZP_ASSERT( NULL != m_pSpecEffectMgr );

	//����������
	m_pProxyFactory = m_pfCreateIProxyFactoryPtr();
	ZP_ASSERT( NULL != m_pProxyFactory );

	m_pCamera = new H3DFPSCamera;

	this->InitResources();
}

void H3DEngineBox::Resize( void )
{
	RECT clientRect;
	::GetClientRect( m_hWnd , &clientRect );
	m_iWidth = clientRect.right - clientRect.left;
	m_iHeight = clientRect.bottom - clientRect.top; 

	if( m_iWidth == 0 )
	{
		m_iWidth = 1;
	} 
	if( m_iHeight == 0 )
	{
		m_iHeight = 1;
	}

	float fFov = 60.0f;
	float fNear = 2.0f;
	float fFar = 1000.0f;
	//�����߱�
	float fRatio = static_cast<float>( m_iWidth ) / static_cast<float>( m_iHeight );
	 
	if( NULL != m_pH3DScene->GetH3DLevel() )
	{
		m_pH3DScene->GetH3DLevel()->SetFrustum( fFov , fRatio , fNear , fFar  );
	} 

	if( NULL !=  m_pH3DRenderer )
	{ 
		m_pH3DRenderer->SetFrustum( fFov , fRatio , fNear , fFar );
		m_pH3DRenderer->ResizeWindowForEditor( m_iWidth , m_iHeight );
	}

}

void H3DEngineBox::Destroy( void )
{
	this->DestroyResources();

	//ɾ������� 
	ZP_SAFE_DELETE( m_pCamera );

	ZP_ASSERT( NULL != m_pfDeleteRenderPtr );
	m_pfDeleteRenderPtr();
	m_pH3DRenderer = NULL;

	ZP_ASSERT( NULL != m_pProxyFactory );
	m_pProxyFactory->Release();
	m_pProxyFactory = NULL;

	//�ͷų�ʼ����Engine��̬��
	BOOL bRes = FreeLibrary( m_hEngineDll );
	ZP_ASSERT( TRUE == bRes );
	m_hEngineDll = NULL;
	m_hWnd = NULL;
}

void H3DEngineBox::RenderOneFrame( void )
{
	if( NULL == m_pH3DRenderer )
		return;
	 
	//������ʧʱ��
	this->_FrameBegin();
	 
	//Ӧ�����
	this->_SetupCamera();

	m_pH3DRenderer->FrameBegin();
	m_pH3DRenderer->ClearScreen();  
	   
	m_pH3DScene->Update( static_cast<float>(m_uiElapseTick) * 0.001f );

	if( false == m_isEditMode )
	{
		m_pH3DRenderer->UpdateCpuSkin();
		m_pH3DRenderer->ForceSyncData();
	}

	m_pH3DRenderer->UpdatePhx(m_uiElapseTick); 

	m_pH3DScene->PushToRenderer();

	m_pH3DRenderer->Render();

	//���ƻ�׼����
	//this->_DrawHelpGrid();

	m_pH3DRenderer->FrameEnd();  
	m_pH3DRenderer->SwapBuffer();
	 

	this->_FrameEnd();
} 

void H3DEngineBox::_FrameBegin( void )
{
	unsigned int uiCurrTick = ::GetTickCount();
	m_uiElapseTick = uiCurrTick - m_uiLastTick;
	m_uiLastTick = uiCurrTick; 
}

void H3DEngineBox::_FrameEnd( void )
{ 
}


void H3DEngineBox::_SetupCamera( void )
{ 
	if( ::GetKeyState('W') & 0x80 ||
		::GetKeyState('w') & 0x80 )
	{
		 m_pCamera->MoveAlongDirVec( m_uiElapseTick );
	}

	if( ::GetKeyState('S') & 0x80 ||
		::GetKeyState('s') & 0x80 )
	{
		m_pCamera->MoveAlongDirVec( m_uiElapseTick , true );
	}

	if( ::GetKeyState('A') & 0x80 ||
		::GetKeyState('a') & 0x80 )
	{
		m_pCamera->MoveAlongRightVec( m_uiElapseTick , true );
	}

	if( ::GetKeyState('D') & 0x80 ||
		::GetKeyState('d') & 0x80 )
	{
		m_pCamera->MoveAlongRightVec( m_uiElapseTick , false  );
	}

	if( ::GetKeyState('Q') & 0x80 ||
		::GetKeyState('q') & 0x80 )
	{
		m_pCamera->RotateWithDir( -0.1f );
	}

	if( ::GetKeyState('E') & 0x80 ||
		::GetKeyState('e') & 0x80 )
	{
		m_pCamera->RotateWithDir( 0.1f );
	}

	m_pH3DScene->ApplyCamera( m_pCamera ); 
}


void H3DEngineBox::_DrawHelpGrid( void )
{
	H3DVec3 b(0,0,0),x(100,0,0),y(0,100,0),z(0,0,100);
	float c[4] = {1,0,0,0};
	c[0] = 1;c[1] = c[2] = 1;

	for (float  i=-5.f;i<5.f;i+=0.5f)
	{
		b[0] = i;b[1] = -5;b[2] = 0;
		x[0] = i;x[1] = 5;x[2] = 0;
		y[0] = -5;y[1] = i;y[2] = 0;
		z[0]=  5;z[1] = i;z[2] = 0;
		m_pH3DRenderer->DrawSegment(b, x, c, 1);
		m_pH3DRenderer->DrawSegment(y, z, c, 1);
	}

	c[0] = 1;c[1] =c[2] = 0;
	b[0] = b[1] = b[2] = 0;
	x[0] = 100;x[1] = x[2] = 0;
	y[0] = 0;y[1] = 100;y[2] = 0;
	z[0] = 0;z[1] = 0;z[2] = 100;
	m_pH3DRenderer->DrawSegment(b, x, c, 5);
	c[0]=0;c[1]=1;
	m_pH3DRenderer->DrawSegment(b, y, c, 5);
	c[2]=1;c[1]=0;
	m_pH3DRenderer->DrawSegment(b, z, c, 5);
}

void H3DEngineBox::InitResources( void )
{  
	m_dressSelector.LoadDressesFromXmlFile("../resources/config/shared/item/dress.xml");

	//�򿪶�����
	m_pH3DRenderer->OpenActionLib( "../resources/art/role/actions/role.xml" ); 
	//�½�����
	m_pH3DScene = new H3DScene("Level0" , m_pH3DRenderer  );
	
	//������ɫ
	this->_InitActors();
	//������Դ
	this->_InitLights();
	//����������̬ģ��
	this->_InitDmls();
	//����������Ч
	this->_InitPostProcess(); 
	  
	m_pH3DScene->RestructScene();
}

void H3DEngineBox::DestroyResources( void )
{
	ZP_SAFE_DELETE( m_pH3DScene );
}

void H3DEngineBox::RotateCameraWithUpAxis( const float thetaInRad )
{
	m_pCamera->RotateWithUp( thetaInRad );
}

void H3DEngineBox::RotateCameraWithRightAxis( const float thetaInRad )
{
	m_pCamera->RotateWithRight( thetaInRad );
}

void H3DEngineBox::_InitActors( void )
{
	String strActorName = "ac2"; 
	int iMatLod = 0;

	//ָ������
	String strActionName = "2PMShining"; 
	if( m_pH3DRenderer->IsActionExist( strActionName.c_str() , H3DI::ACTOR_HUMAN , false ) )
	{ 
		m_pH3DRenderer->LoadAction( strActionName.c_str()  , H3DI::ACTOR_HUMAN , false );
	}

	//������ɫ0
	H3DI::IActor* pActor = m_pH3DScene->CreateActor( "actor0" , false , iMatLod ); 

	pActor->SetBodyPart("../resources/art/role/bodypart/female/hair/113025001/113025001.BPT"); 
	pActor->SetBodyPart("../resources/art/role/bodypart/female/body/114004001/114004001.BPT"); 
	pActor->SetBodyPart("../resources/art/role/bodypart/female/trousers/116027001/116027001.BPT"); 
	pActor->SetBodyPart("../resources/art/role/bodypart/female/shoe/118022001/118022001.BPT");

	H3DI::IAnimationChannel* pAnimCh = 
		pActor->GetAnmChannel( 0 ); 
	pAnimCh->SetAction( strActionName.c_str() , true );   

	pActor->SetAdornment(H3DI::ACTOR_ADORNMENT_TAIL, "../resources/art/role/link/female/4112/4112001001/4112001001.spe");
	pActor->SetAdornment(H3DI::ACTOR_ADORNMENT_LEFTHAND, "../resources/art/role/link/female/4113/4113002001/4113002001.spe");
	pActor->SetAdornment(H3DI::ACTOR_ADORNMENT_RIGHTHAND, "../resources/art/role/link/female/4114/4114001001/4114001001.spe");
	pActor->SetAdornment(H3DI::ACTOR_ADORNMENT_BACK, "../resources/art/role/link/female/4115/4115001001/4115001001.spe");
	pActor->SetAdornmentVisibility(H3DI::ACTOR_ADORNMENT_TAIL, true);
	pActor->SetAdornmentVisibility(H3DI::ACTOR_ADORNMENT_RIGHTHAND, true);
	pActor->SetAdornmentVisibility(H3DI::ACTOR_ADORNMENT_LEFTHAND, true);
	pActor->SetAdornmentVisibility(H3DI::ACTOR_ADORNMENT_BACK, true); 

	//������ɫ1
	pActor = m_pH3DScene->CreateActor( "actor1" , false , iMatLod ); 
	pActor->SetPosition( H3DVec3(  -5.0f , 3.0f , 0.0f ) );

	pActor->SetBodyPart("../resources/art/role/bodypart/female/hair/113011001/113011001.BPT"); 
	pActor->SetBodyPart("../resources/art/role/bodypart/female/body/114036001/114036001.BPT"); 
	pActor->SetBodyPart("../resources/art/role/bodypart/female/trousers/116006001/116006001.BPT"); 
	pActor->SetBodyPart("../resources/art/role/bodypart/female/shoe/118005001/118005001.BPT");

	pAnimCh = 
		pActor->GetAnmChannel( 0 ); 
	pAnimCh->SetAction( strActionName.c_str() , true );   

	//������ɫ2
	pActor = m_pH3DScene->CreateActor( "actor2" , true , iMatLod ); 
	pActor->SetPosition( H3DVec3(  5.0f , 3.0f , 0.0f ) );

	pActor->SetBodyPart("../resources/art/role/bodypart/male/hair/103005001/103005001.BPT"); 
	pActor->SetBodyPart("../resources/art/role/bodypart/male/body/104011001/104011001.BPT"); 
	pActor->SetBodyPart("../resources/art/role/bodypart/male/trousers/106007001/106007001.BPT"); 
	pActor->SetBodyPart("../resources/art/role/bodypart/male/shoe/108004001/108004001.BPT");

	pAnimCh = 
		pActor->GetAnmChannel( 0 ); 
	pAnimCh->SetAction( strActionName.c_str() , true );   

	//��������
	H3DI::IAvatarSkeletonModel* pPet = m_pH3DScene->CreatePet( "pet0" , false , iMatLod );
	pPet->SetPosition( H3DVec3( 3.5f , 0.0f , 0.0f ) );
	pAnimCh =  pPet->GetAnmChannel( 0 ); 
	pAnimCh->SetAction( strActionName.c_str() , true );   
}

void H3DEngineBox::_InitLights( void )
{
	//������Դ
	H3DI::IPrePassLight* pPrePassLight = m_pH3DScene->CreateLight( H3DI::AFFECT_ALL , H3DI::LIGHT_DIR ); 

	float v4LightColor[] = { 0.75f ,0.75f ,0.75f , 1.0f }; 
	float v4ShadowColor[] = { 0.0f , 0.0f , 0.0f , 1.0f };

	H3DVec3 v3LightDir( 0.0f , 0.0f , 1.0f );
	v3LightDir.Normalize();

	pPrePassLight->SetDirection( v3LightDir );
	pPrePassLight->SetColor( v4LightColor );
	pPrePassLight->SetIntensity( 0.4f , 0.5f );
	pPrePassLight->SetShadowColor( v4ShadowColor );
	pPrePassLight->SetShadowEnable( true );
	pPrePassLight->SetLightEnable( true ); 
}

void H3DEngineBox::_InitDmls( void )
{
	int iMatLod = 0;

	//������̬ģ�� 
	m_pH3DScene->CreateDml("../resources/art/stage/palaceroom/model/palaceroom_carpet001.dml"   );   
	m_pH3DScene->CreateDml("../resources/art/stage/palaceroom/model/palaceroom_crown001.dml"  );   
	m_pH3DScene->CreateDml("../resources/art/stage/palaceroom/model/palaceroom_floor001.dml"  );   
	m_pH3DScene->CreateDml("../resources/art/stage/palaceroom/model/palaceroom_floor002.dml"  ); 
	m_pH3DScene->CreateDml("../resources/art/stage/palaceroom/model/palaceroom_outside101.dml"  ); 
	m_pH3DScene->CreateDml("../resources/art/stage/palaceroom/model/palaceroom_outsidetree001.dml"  );  
	m_pH3DScene->CreateDml("../resources/art/stage/palaceroom/model/palaceroom_door200.dml"  );  
	m_pH3DScene->CreateDml("../resources/art/stage/palaceroom/model/palaceroom_wall001.dml" , iMatLod , false  ); 
	m_pH3DScene->CreateDml("../resources/art/stage/palaceroom/model/palaceroom_wall004.dml" , iMatLod , false  ); 
	m_pH3DScene->CreateDml("../resources/art/stage/palaceroom/model/palaceroom_wall00301.dml" , iMatLod , false );
	m_pH3DScene->CreateDml("../resources/art/stage/palaceroom/model/palaceroom_window001.dml" , iMatLod , false  );  
	//�ݶ���Ͷ����Ӱ
	m_pH3DScene->CreateDml("../resources/art/stage/palaceroom/model/palaceroom_roof001.dml" , iMatLod , false ); 
	m_pH3DScene->CreateDml("../resources/art/stage/palaceroom/model/palaceroom_roof002.dml" , iMatLod , false ); 
	m_pH3DScene->CreateDml("../resources/art/stage/palaceroom/model/palaceroom_roof003.dml" , iMatLod , false );
}

void H3DEngineBox::_InitPostProcess( void )
{
	//��Ӻ�����Ч
	m_pH3DScene->GetH3DLevel()->AddPostProcess( "../data/product/postprocess/UE3_Uber.xml" );  
	//m_pH3DScene->GetH3DLevel()->AddPostProcess( "../data/product/postprocess/FXAA.xml");

#define SET_POSTPROCESS_PARAM(Name, InValue)	{ float Value = InValue; \
	m_pH3DScene->GetH3DLevel()->SetPostProcessParam(Name, &Value, sizeof(float)); }

#define SET_POSTPROCESS_PARAMS(Name, InValue) { float Value[4] = {InValue, InValue, InValue, InValue};	\
	m_pH3DScene->GetH3DLevel()->SetPostProcessParam(Name, &Value, sizeof(float) * 4); }

	SET_POSTPROCESS_PARAMS("SceneMidTones",1.0f);
	SET_POSTPROCESS_PARAM("SceneDesaturation",1.0f);
	SET_POSTPROCESS_PARAMS("GammaOverlayColor",0.f);
	SET_POSTPROCESS_PARAMS("GammaColorScale",1.0f);
	SET_POSTPROCESS_PARAM("DOF_Dist",5.0f);
	SET_POSTPROCESS_PARAM("DOF_Range",10.0f);
	SET_POSTPROCESS_PARAM("DOF_Inner",10.0f);
	SET_POSTPROCESS_PARAM("DOFKernal",1.0f);
	SET_POSTPROCESS_PARAM("BloomKernal",1.0f);
	SET_POSTPROCESS_PARAM("BloomThreshold",1.0f);
	SET_POSTPROCESS_PARAM("BloomBlend",1.0f);
	SET_POSTPROCESS_PARAMS("BloomColor",1.0f);
	SET_POSTPROCESS_PARAMS("ScreenShadow",0.0f);
	SET_POSTPROCESS_PARAMS("ScreenHighLight",1.0f);
	SET_POSTPROCESS_PARAM("SoftFocalRange",1.0f);
	SET_POSTPROCESS_PARAM("SoftFocalBenld",0.0f);

#undef SET_POSTPROCESS_PARAMS
#undef SET_POSTPROCESS_PARAM
}

}//namespace ZPH3D