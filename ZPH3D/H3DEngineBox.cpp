
#include "H3DEngineBox.h"
#include "dMathHeader.h"
#include "H3DFPSCamera.h"
#include "engine_interface_internal.h"
#include "EffectCore_dx.h"
#include "H3DScene.h"
#include "H3DActionSelector.h"
#include "H3DDressSelector.h"


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
m_pUIShader(NULL),
m_pVB(NULL),
m_pActionSelector(NULL),
m_pDressSelector(NULL),
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

	//获取渲染器创建函数
	m_pfCreateRenderPtr = (PF_CREATERENDERPTR)GetProcAddress( m_hEngineDll , "CreateRenderer" );
	ZP_ASSERT( NULL != m_pfCreateRenderPtr );
	//获取渲染器删除函数
	m_pfDeleteRenderPtr = (PF_DELETERENDERPTR)GetProcAddress( m_hEngineDll , "DeleteRenderer" );
	ZP_ASSERT( NULL != m_pfDeleteRenderPtr );
	//获取IProxyFactory创建函数
	m_pfCreateIProxyFactoryPtr = (PF_CREATEIPROXYFACTORYPTR)GetProcAddress( m_hEngineDll , "CreateIProxyFactory");
	ZP_ASSERT( NULL != m_pfCreateIProxyFactoryPtr );

	//创建渲染器
	m_pH3DRenderer = m_pfCreateRenderPtr();
	ZP_ASSERT( NULL != m_pH3DRenderer);
	 
	
	//获得核心版本信息
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

	//创建渲染窗口
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



	m_pH3DRenderer->QueryInfo(1,(void*)(&m_isEditMode),0,0,0,0);
  
	float fColor[4]={0.0f,0.0f,0.0f,0.0f};
	m_pH3DRenderer->SetClearColor(fColor);
	m_pH3DRenderer->SetClearFlags( 
		H3DI::CLEAR_BUF_COLOR|H3DI::CLEAR_BUF_DEPTH|H3DI::CLEAR_BUF_STENCIL );

	//创建特效管理器
	m_pSpecEffectMgr=m_pH3DRenderer->CreateEffectManager();
	ZP_ASSERT( NULL != m_pSpecEffectMgr );

	//创建代理工厂
	m_pProxyFactory = m_pfCreateIProxyFactoryPtr();
	ZP_ASSERT( NULL != m_pProxyFactory );

	m_pCamera = new H3DFPSCamera; 
	m_pActionSelector = new H3DActionSelector; 
	m_pDressSelector = new H3DDressSelector;

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
	//计算宽高比
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

	ZP_SAFE_DELETE( m_pActionSelector ); 
	ZP_SAFE_DELETE( m_pDressSelector ); 
	//删除摄像机 
	ZP_SAFE_DELETE( m_pCamera );

	ZP_ASSERT( NULL != m_pfDeleteRenderPtr );
	m_pfDeleteRenderPtr();
	m_pH3DRenderer = NULL;

	ZP_ASSERT( NULL != m_pProxyFactory );
	m_pProxyFactory->Release();
	m_pProxyFactory = NULL;

	//释放初始化的Engine动态库
	BOOL bRes = FreeLibrary( m_hEngineDll );
	ZP_ASSERT( TRUE == bRes );
	m_hEngineDll = NULL;
	m_hWnd = NULL;
}

void H3DEngineBox::RenderOneFrame( void )
{
	if( NULL == m_pH3DRenderer )
		return;
	 
	//更新流失时间
	this->_FrameBegin();
	 
	//应用相机
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

	//绘制基准网格
	//_DrawHelpGrid();

	//_DrawUI();

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
	
	m_pDressSelector->LoadDressesFromXmlFile( "../resources/config/shared/item/dress.xml" );
	//m_dressSelector.PreLoadActorBodyParts( m_pH3DRenderer );
	m_pActionSelector->LoadActionsFromXMLFile( "../resources/art/role/actions/role.xml" );
	//打开动作库
	m_pH3DRenderer->OpenActionLib( "../resources/art/role/actions/role.xml" ); 
	//新建场景
	m_pH3DScene = new H3DScene("Level0" , m_pH3DRenderer  );
	
	//创建角色
	this->_InitActors();
	//创建光源
	this->_InitLights();
	//创建场景静态模型
	this->_InitDmls();
	//创建后处理特效
	this->_InitPostProcess(); 
	//初始化UI
	this->_InitUI();
	  
	m_pH3DScene->RestructScene();
}

void H3DEngineBox::DestroyResources( void )
{
	m_pUIShader = NULL; 
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
	int iMatLod = 0;

	//指定动作
	String strActionName = "2PMShining"; 
	if( m_pH3DRenderer->IsActionExist( strActionName.c_str() , H3DI::ACTOR_HUMAN , false ) )
	{ 
		m_pH3DRenderer->LoadAction( strActionName.c_str()  , H3DI::ACTOR_HUMAN , false );
	}

	//创建角色0
	H3DI::IActor* pActor = m_pH3DScene->CreateActor( "actor0" , false , iMatLod );  
	H3DI::IAnimationChannel* pAnimCh = 
		pActor->GetAnmChannel( 0 ); 
	pAnimCh->SetAction( strActionName.c_str() , true );    

	//创建角色1
	pActor = m_pH3DScene->CreateActor( "actor1" , false , iMatLod ); 
	pActor->SetPosition( H3DVec3(  -5.0f , 3.0f , 0.0f ) );  
	pAnimCh = 
		pActor->GetAnmChannel( 0 ); 
	pAnimCh->SetAction( strActionName.c_str() , true );   

	//创建角色2
	pActor = m_pH3DScene->CreateActor( "actor2" , true , iMatLod ); 
	pActor->SetPosition( H3DVec3(  5.0f , 3.0f , 0.0f ) );  
	pAnimCh = 
		pActor->GetAnmChannel( 0 ); 
	pAnimCh->SetAction( strActionName.c_str() , true );   

	//创建宠物
	H3DI::IAvatarSkeletonModel* pPet = m_pH3DScene->CreatePet( "pet0" , false , iMatLod );
	pPet->SetPosition( H3DVec3( 3.5f , 0.0f , 0.0f ) );
	pAnimCh =  pPet->GetAnmChannel( 0 ); 
	pAnimCh->SetAction( strActionName.c_str() , true );   

	//为角色随机挑选衣服
	m_pH3DScene->RandomActorDress( *m_pDressSelector );
}

void H3DEngineBox::_InitLights( void )
{
	//创建光源
	H3DI::IPrePassLight* pPrePassLight = m_pH3DScene->CreateLight( H3DI::AFFECT_ALL , H3DI::LIGHT_DIR ); 

	float v4LightColor[] = { 1.0f ,1.0f ,1.0f, 1.0f }; 
	float v4LightColor2[] = { 150.0f , 0.0f  ,0.0f , 1.0f }; 
	float v4ShadowColor[] = { 0.0f , 0.0f , 0.0f , 1.0f };

	H3DVec3 v3LightDir( 0.0f , 0.0f , 1.0f );
	v3LightDir.Normalize();

	pPrePassLight->SetDirection( v3LightDir );
	pPrePassLight->SetColor( v4LightColor );
	pPrePassLight->SetIntensity( 0.9f );
	pPrePassLight->SetShadowColor( v4ShadowColor );
	pPrePassLight->SetShadowEnable( true );
	pPrePassLight->SetLightEnable( true ); 

	pPrePassLight = m_pH3DScene->CreateLight( H3DI::AFFECT_ALL , H3DI::LIGHT_POINT );

	pPrePassLight->SetDirection( v3LightDir );
	pPrePassLight->SetColor( v4LightColor2 );
	pPrePassLight->SetPosition( H3DVec3( 0.0f , 5.0f , 0.0f ) ); 
	pPrePassLight->SetIntensity( 0.9f );
	pPrePassLight->SetRange( 0.0f , 10.0f );
	pPrePassLight->SetLightEnable( true ); 

	

}

void H3DEngineBox::_InitDmls( void )
{
	int iMatLod = 0;

	float FloorPlane[] = { 0.0f , 0.0f , 1.0f , 0.0f };

	//创建静态模型 
	m_pH3DScene->CreateDml("../resources/art/stage/palaceroom/model/palaceroom_carpet001.dml"   );   
	m_pH3DScene->CreateDml("../resources/art/stage/palaceroom/model/palaceroom_crown001.dml"  );   

	H3DI::IModel* pFloor001 = 
		m_pH3DScene->CreateDml("../resources/art/stage/palaceroom/model/palaceroom_floor001.dml"  );    


	m_pH3DScene->CreateDml("../resources/art/stage/palaceroom/model/palaceroom_floor002.dml"  ); 
	m_pH3DScene->CreateDml("../resources/art/stage/palaceroom/model/palaceroom_outside101.dml"  ); 
	m_pH3DScene->CreateDml("../resources/art/stage/palaceroom/model/palaceroom_outsidetree001.dml"  );  
	m_pH3DScene->CreateDml("../resources/art/stage/palaceroom/model/palaceroom_door200.dml"  );  
	m_pH3DScene->CreateDml("../resources/art/stage/palaceroom/model/palaceroom_wall001.dml" , iMatLod , false  ); 
	m_pH3DScene->CreateDml("../resources/art/stage/palaceroom/model/palaceroom_wall004.dml" , iMatLod , false  ); 
	m_pH3DScene->CreateDml("../resources/art/stage/palaceroom/model/palaceroom_wall00301.dml" , iMatLod , false );
	m_pH3DScene->CreateDml("../resources/art/stage/palaceroom/model/palaceroom_window001.dml" , iMatLod , false  );  
	//屋顶不投射阴影
	m_pH3DScene->CreateDml("../resources/art/stage/palaceroom/model/palaceroom_roof001.dml" , iMatLod , false ); 
	m_pH3DScene->CreateDml("../resources/art/stage/palaceroom/model/palaceroom_roof002.dml" , iMatLod , false ); 
	m_pH3DScene->CreateDml("../resources/art/stage/palaceroom/model/palaceroom_roof003.dml" , iMatLod , false );


}

void H3DEngineBox::_InitPostProcess( void )
{
	//添加后处理特效
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

void H3DEngineBox::RandomActorDresses( void )
{
	m_pH3DScene->RandomActorDress( *m_pDressSelector );
}

void H3DEngineBox::SwitchActorAction( void )
{
	String strMaleAction = m_pActionSelector->GetCurrAction( true );
	String strFemaleAction = m_pActionSelector->GetCurrAction( false );

	m_pH3DRenderer->LoadAction( strMaleAction.c_str() , H3DI::ACTOR_HUMAN , true );
	m_pH3DRenderer->LoadAction( strFemaleAction.c_str() , H3DI::ACTOR_HUMAN , false ); 
	m_pH3DScene->SwitchActorAction( *m_pActionSelector ); 
	m_pActionSelector->NextAction();

}

void H3DEngineBox::_InitUI( void )
{
	//m_pUIShader = 
	//	m_pH3DRenderer->GetNewShader( 
	//	"../resources/art/effect/ui/lib/x52_ingame_st_bk3.mat" , 
	//	"x52_ingame_st_bk3" );

	//m_pUIShader = 
	//	m_pH3DRenderer->GetNewShader( 
	//	"../resources/art/effect/ui/lib/x52_ingame_st_bk.mat" ,
	//	"x52_ingame_st_bk" );

	m_pUIShader = 
		m_pH3DRenderer->GetNewShader( 
		"../resources/art/ui/zpui/1440_900_qixi_loading.mat" ,
		"1440_900_qixi_loading" );

	//m_pUIShader = 
	//	m_pH3DRenderer->GetNewShader( 
	//	"../resources/art/ui/hall/1440_900_x52_ui_hallback.mat" ,
	//	"1440_900_x52_ui_hallback" );

	//m_pUIShader = 
	//	m_pH3DRenderer->GetNewShader( 
	//	"../resources/art/role/bodypart/female/body/114004001/114004001.mat" , 
	//	"114004001" );

	//m_pUIShader = 
	//	m_pH3DRenderer->GetNewShader( 
	//	"../resources/art/role/link/female/4110/4110001001/4110001001.mat" , 
	//	"4110001001" );

	ZP_ASSERT( NULL != m_pUIShader );

	m_pVB = m_pH3DRenderer->GetDynamicVB();
	bool bCreateVBRes = m_pVB->CreateBuffer( 36 * 8192, 4096, - 1, - 1, 0, 0 );
	ZP_ASSERT( true == bCreateVBRes ); 
}

 
void H3DEngineBox::_DrawUI( void )
{
	H3DMat4 oldProjMat;
	H3DMat4 oldViewMat;
	m_pH3DRenderer->GetPerspective( oldProjMat );
	m_pH3DRenderer->GetViewMatrix( oldViewMat );

	H3DMat4 orthoMat;
	H3DMat4 newViewMat;

	newViewMat.Identity();
	m_pH3DRenderer->GetOrthoProjectionMatrix( 
		0.0f , static_cast<float>( m_iWidth ) , 
		static_cast<float>( m_iHeight ) , 0.0f , 
		-1.0f , 1.0f , orthoMat ); 

	m_pH3DRenderer->SetPerspective( orthoMat );
	m_pH3DRenderer->SetViewMatrix( newViewMat );  

	if( m_pUIShader )
	{   
		_PrepareUI(); 
		m_pH3DRenderer->SetNewShader( m_pUIShader );
		m_pH3DRenderer->DrawIndexNew( H3DI::TRIANGLE_LIST , 6 , 4 , 0 , 24 , 12 , 24 );     
	}
	 
	m_pH3DRenderer->SetPerspective( oldProjMat );
	m_pH3DRenderer->SetViewMatrix( oldViewMat ); 
}

 
void H3DEngineBox::_PrepareUI( void )
{
	float points[12] = {
		0,0,0, 
		0,400,0, 
		400,400,0, 
		400,0,0 };

		float color[12] = { 1,1,1, 1,1,1, 1,1,1, 1,1,1 };

		float uv[8] = {
			0,0,
			0,1,
			1,1,
			1,0
		};

		unsigned short index[6] = {0,1,2,0,2,3};

		float* pVertex =
			(float*)m_pVB->Lock( 
			H3DI::VB_VERTEX_ARRAY , H3DI::VB_POS|H3DI::VB_COLOR|H3DI::VB_UV , 36 , 4 );

		int i = 0;
		for( int iVert = 0 ; iVert < 4 ; iVert++ )
		{
			pVertex[i++] = points[3*iVert];
			pVertex[i++] = points[3*iVert+1];
			pVertex[i++] = points[3*iVert+2];

			pVertex[i++] = color[3*iVert];
			pVertex[i++] = color[3*iVert+1];
			pVertex[i++] = color[3*iVert+2];  
		 
			pVertex[i++]= 1.0f; 
			pVertex[i++] = uv[2*iVert]; 
			pVertex[i++] = uv[2*iVert+1]; 
		}

		m_pVB->UnLock(H3DI::VB_VERTEX_ARRAY); 

		unsigned short* pIndex = 
			(unsigned short*)m_pVB->Lock(H3DI::VB_ELEMENT_ARRAY,-1,-1,6);

		for (int iIndx= 0; iIndx<6 ;iIndx++)
		{
			pIndex[iIndx] = index[iIndx];
		}

		m_pVB->UnLock(H3DI::VB_ELEMENT_ARRAY);
}

}//namespace ZPH3D