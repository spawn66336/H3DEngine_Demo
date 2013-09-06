
#include "H3DEngineBox.h"
#include "dMathHeader.h"
#include "H3DFPSCamera.h"


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
m_pLevel(NULL),
m_pActor(NULL),
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

  
	float fColor[4]={0.0f,0.0f,0.0f,0.0f};
	m_pH3DRenderer->SetClearColor(fColor);
	m_pH3DRenderer->SetClearFlags( H3DI::CLEAR_BUF_COLOR|H3DI::CLEAR_BUF_DEPTH );

	//创建特效管理器
	m_pSpecEffectMgr=m_pH3DRenderer->CreateEffectManager();
	ZP_ASSERT( NULL != m_pSpecEffectMgr );

	//创建代理工厂
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
	//计算宽高比
	float fRatio = static_cast<float>( m_iWidth ) / static_cast<float>( m_iHeight );

	if( NULL != m_pLevel )
	{
		m_pLevel->SetFrustum( fFov , fRatio , fNear , fFar  );
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

	//删除摄像机
	delete m_pCamera;
	m_pCamera = NULL;

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
	 
	m_pH3DRenderer->UpdateCpuSkin();
	m_pH3DRenderer->ForceSyncData();
	m_pH3DRenderer->UpdatePhx(m_uiElapseTick); 

	m_pLevel->Update( static_cast<float>(m_uiElapseTick) * 0.001f ); 
	m_pH3DRenderer->PushScene( m_pLevel ); 
	 
	m_pH3DRenderer->Render();

	//绘制基准网格
	this->_DrawHelpGrid();

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

	m_pCamera->Apply( m_pH3DRenderer );
	m_pCamera->Apply( m_pLevel );
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
	String strActorName = "ac2";
	H3DI::sCreateOp createOp; 
	createOp.mat_lod = 0; 
	m_pH3DRenderer->OpenActionLib( "../resources/art/role/actions/role.xml" );
	m_pLevel = m_pH3DRenderer->CreateLevel( "Level0" );

	m_pActor = 
		(H3DI::IActor*)m_pH3DRenderer->CreateActor( 
		createOp , strActorName.c_str() , false , H3DI::ACTION_UPDATE_PH );
	  
	m_pActor->SetBodyPart("../resources/art/role/bodypart/female/trousers/116006001/116006001.BPT");
	m_pActor->SetBodyPart("../resources/art/role/bodypart/female/shoe/118005001/118005001.BPT");

	m_pActor->Update( 0 );

	m_pLevel->AttachModel( (H3DI::IModel*)m_pActor , H3DI::SL_Actors );

	

	m_pLevel->RestructScene();
	 
}

void H3DEngineBox::DestroyResources( void )
{
	
	if( m_pActor )
	{
		m_pLevel->DetachModel( (H3DI::IModel*)m_pActor );
		m_pActor->Release();
		m_pActor = NULL;
	}

	m_pLevel->Release();
	m_pLevel = NULL;
}

void H3DEngineBox::RotateCameraWithUpAxis( const float thetaInRad )
{
	m_pCamera->RotateWithUp( thetaInRad );
}

void H3DEngineBox::RotateCameraWithRightAxis( const float thetaInRad )
{
	m_pCamera->RotateWithRight( thetaInRad );
}

}//namespace ZPH3D