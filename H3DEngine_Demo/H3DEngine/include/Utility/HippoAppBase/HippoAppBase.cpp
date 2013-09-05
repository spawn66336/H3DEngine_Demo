#include "HippoAppBase.h"
#include <Windows.h>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>

using namespace std;
using namespace boost;


#pragma comment(lib, "winmm.lib")

HippoAppBase::HippoAppBase(const char* name,unsigned int w,unsigned int h,unsigned int left,unsigned int top)
{
	m_pWnd=boost::make_shared<HippoWindow>(name,w,h,left,top);
}

HippoAppBase::~HippoAppBase()
{
	
}
void HippoAppBase::Init()
{

	InitApp();
}

void HippoAppBase::Shutdown()
{
	ShutdownApp();
}

void HippoAppBase::StartRun()
{
	bool bGotMsg;
	MSG msg;
	msg.message = WM_NULL;
	PeekMessage( &msg, NULL, 0U, 0U, PM_NOREMOVE );

	while( WM_QUIT != msg.message )
	{
		// Use PeekMessage() so we can use idle time to render the scene. 
		bGotMsg = ( PeekMessage( &msg, NULL, 0U, 0U, PM_REMOVE ) != 0 );
		if( bGotMsg )
		{
			TranslateMessage( &msg );
			DispatchMessage( &msg );
		}
		else
		{
			// Render a frame during idle time (no messages are waiting)
			static DWORD current_t=0;
			static DWORD last_t=0;
			last_t=current_t;
			current_t=timeGetTime();
			DWORD escape=current_t-last_t;

			Update(escape);
			Render(escape);
		}
	}
}