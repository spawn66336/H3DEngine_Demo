// EngineShell.cpp : ���� DLL Ӧ�ó���ĵ���������
//

#include "stdafx.h"
#include "EngineShell.h"
#include "instance_pool.h"

extern "C" __declspec(dllexport) H3DI::IRender* CreateRenderer()
{
	static H3DI::SIRender s_render;
	EnginePool::init();
	return &s_render;	
};
extern "C" __declspec(dllexport)  void DeleteRenderer()
{
	
};