/********************************************************************
	created:	2011/10/19
	created:	19:10:2011   0:45
	filename: 	f:\TestHippo\TestHippo\HIPPO_FrameWork\HippoFrameWork.h
	file path:	f:\TestHippo\TestHippo\HIPPO_FrameWork
	file base:	HippoFrameWork
	file ext:	h
	author:		sssa2000
	
	purpose:	
*********************************************************************/
#pragma once
#include <Windows.h>


enum CONSOLE_COLOR
{
	CC_RED,
	CC_GREEN,
	CC_BLUE,
	CC_WHITE
};

namespace H3DI
{
	class IRender;
	class IProxyFactory;
	class IGlobalEnvironment;
}
class Hippo_InputManager;
class ISpecialEffectManager;
class HippoUI;
class HippoSceneManager;
class HippoLog;

int Hippo_InitGlobal(HWND hwnd,int w,int h);
void Hippo_CleanUp();
void Hippo_WriteConsole(CONSOLE_COLOR color,char *szstr,... );
void Hippo_WriteConsoleAndLog(CONSOLE_COLOR color,char *szstr,... );

void Hippo_GetCurrentWndInfo(int & w,int& h);
int Hippo_DrawHelpGrid();
H3DI::IRender* Hippo_GetIRender();
H3DI::IProxyFactory* Hippo_GetIProxyFactory();
H3DI::IGlobalEnvironment* Hippo_GetIGlobalEnv();
ISpecialEffectManager* Hippo_GetSpeManager();
Hippo_InputManager* GetInputManager();
bool Hippo_IsEditorMode();
HippoSceneManager* Hippo_GetSceneManager();
HippoLog* Hippo_GetLog();

//以下是衡量frame time的函数
void Hippo_FrameBegin();
void Hippo_FrameEnd();
DWORD Hippo_GetFrameTime();