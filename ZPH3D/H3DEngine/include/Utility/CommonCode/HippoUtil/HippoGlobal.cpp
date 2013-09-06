#include "HippoGlobal.h"
#include "EngineBox.h"
#include "HelpeDraw.h"
#include "input/Hippo_InputManager.h"
#include "HippoScene.h"
#include "log/HippoLog.h"
#include "engine_interface.h"
#include "engine_interface_internal.h"
#include <stdio.h>
//#include <memory>

EngineBox* g_engine_box=0;
Hippo_InputManager* gInputManager=0;
HANDLE m_console_handle;
HippoSceneManager* gSceneManager=0;
HippoLog* gLog=0;
DWORD gLastTime=0;
DWORD gFrameTimeInMs=0;

int Hippo_InitGlobal(HWND hwnd,int w,int h)
{
	static bool _bInited=false;
	if(!_bInited)
	{
		_bInited=true;
		if (!gInputManager)
		{
			gInputManager=new Hippo_InputManager;
			gInputManager->Init();
		}
		if(!g_engine_box)
		{
			g_engine_box=new EngineBox;
			g_engine_box->Init(hwnd,w,h);
		}

		::AllocConsole();
		m_console_handle=::GetStdHandle(STD_OUTPUT_HANDLE);
	}

	return 1;
}

void Hippo_GetCurrentWndInfo(int & w,int& h)
{
	w=g_engine_box->m_width;
	h=g_engine_box->m_height;
}

void Hippo_CleanUp()
{
	delete g_engine_box;
	delete gInputManager;
	::FreeConsole();
	if(gSceneManager)
		delete gSceneManager;
	if(gLog)
		delete gLog;
}
HippoLog* Hippo_GetLog()
{
	if(!gLog)
	{
		gLog=new HippoLog();
	}
	return gLog;
}
HippoSceneManager* Hippo_GetSceneManager()
{
	if(!gSceneManager)
	{
		gSceneManager=new HippoSceneManager;
	}
	return gSceneManager;
}

bool Hippo_IsEditorMode()
{
	return g_engine_box->m_bIsEditorMode;
}
H3DI::IRender* Hippo_GetIRender()
{
	return g_engine_box->m_pRenderer;
}
H3DI::IProxyFactory* Hippo_GetIProxyFactory()
{
	return g_engine_box->m_pIDecoratorFactory;
}
H3DI::IGlobalEnvironment* Hippo_GetIGlobalEnv()
{
	return g_engine_box->m_pIDecoratorFactory->GetGlobalEnv();
}
ISpecialEffectManager* Hippo_GetSpeManager()
{
	return g_engine_box->m_pSpeManager;
}

Hippo_InputManager* GetInputManager()
{
	return gInputManager;
}

//!绘制地面网格
int Hippo_DrawHelpGrid()
{
	HelpeDraw::DrawHelpGird(g_engine_box->m_pRenderer);
	return 1;
}


inline WORD MakeColor(CONSOLE_COLOR color)
{
	WORD res=0;
	if (color==CC_RED)
	{
		res=FOREGROUND_RED|FOREGROUND_INTENSITY;
	}
	else if (color==CC_GREEN)
	{
		res=FOREGROUND_GREEN|FOREGROUND_INTENSITY;
	}
	else if (color==CC_BLUE)
	{
		res=FOREGROUND_BLUE|FOREGROUND_INTENSITY;
	}
	else if (color==CC_WHITE)
	{
		res=FOREGROUND_RED|FOREGROUND_GREEN|FOREGROUND_RED|FOREGROUND_INTENSITY;
	}
	return res;
}

void Hippo_WriteConsole(CONSOLE_COLOR color,char *szstr,... )
{
	DWORD numWritten = 0;
	static char szParsedString[1024];

	va_list va;
	va_start( va, szstr );
	vsprintf_s( szParsedString,szstr, va );
	va_end( va );

	SetConsoleTextAttribute(m_console_handle, MakeColor(color));

	WriteConsoleA(m_console_handle,szParsedString, (DWORD)strlen(szParsedString),&numWritten,0);
	WriteConsoleA(m_console_handle,"\n", 1,&numWritten,0);
}

void Hippo_WriteConsoleAndLog(CONSOLE_COLOR color,char *szstr,... )
{
	DWORD numWritten = 0;
	static char szParsedString[1024];

	va_list va;
	va_start( va, szstr );
	vsprintf_s( szParsedString,szstr, va );
	va_end( va );

	SetConsoleTextAttribute(m_console_handle, MakeColor(color));

	WriteConsoleA(m_console_handle,szParsedString, (DWORD)strlen(szParsedString),&numWritten,0);
	WriteConsoleA(m_console_handle,"\n", 1,&numWritten,0);

	Hippo_GetLog()->Log(szParsedString);
}


void Hippo_FrameBegin()
{
	gLastTime=GetTickCount();
}
void Hippo_FrameEnd()
{
	DWORD t=GetTickCount();
	gFrameTimeInMs=t-gLastTime;
}
DWORD Hippo_GetFrameTime()
{
	return gFrameTimeInMs;
}