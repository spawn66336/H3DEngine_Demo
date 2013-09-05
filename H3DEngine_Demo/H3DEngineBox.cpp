#include "StdAfx.h"
#include "ZPDependency.h"
#include "H3DEngineBox.h"


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
m_pfCreateIProxyFactoryPtr(NULL)
{
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

	//创建特效管理器
	m_pSpecEffectMgr=m_pH3DRenderer->CreateEffectManager();
	ZP_ASSERT( NULL != m_pSpecEffectMgr );

	//创建代理工厂
	m_pProxyFactory = m_pfCreateIProxyFactoryPtr();
	ZP_ASSERT( NULL != m_pProxyFactory );

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

	//计算宽高比
	float fRatio = static_cast<float>( m_iWidth ) / static_cast<float>( m_iHeight );

	m_pH3DRenderer->SetFrustum( 60.0f , fRatio , 2.0f , 1000.0f );

	if( NULL !=  m_pH3DRenderer )
	{
		m_pH3DRenderer->ResizeWindowForEditor( m_iWidth , m_iHeight );
	}
}

void H3DEngineBox::Destroy( void )
{
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
	m_pH3DRenderer->FrameBegin();
	
	m_pH3DRenderer->Render();

	m_pH3DRenderer->FrameEnd();
	 
	m_pH3DRenderer->SwapBuffer();
} 

}