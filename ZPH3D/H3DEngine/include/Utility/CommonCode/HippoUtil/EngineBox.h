/********************************************************************
	created:	2011/10/20
	created:	20:10:2011   23:35
	filename: 	f:\TestHippo\TestHippo\EngineBox.h
	file path:	f:\TestHippo\TestHippo
	file base:	EngineBox
	file ext:	h
	author:		sssa2000
	
	purpose:	
*********************************************************************/
#pragma once

#include <Windows.h>

namespace H3DI
{
	class IRender;
	class IActor;
	class IModel;
	class ILevel;
	class IGlobalEnvironment;
	class IProxyFactory;
};

typedef H3DI::IRender* (*CreateRenderPtr)();
typedef void (*DeleteRenderPtr)();
typedef H3DI::IProxyFactory* (*CreateIProxyFactoryPtr)();
class ISpecialEffectManager;

class EngineBox
{
public:
	EngineBox();
	~EngineBox();

	int Init(HWND hwnd,unsigned int width,unsigned int height);
	H3DI::IRender* m_pRenderer;
	H3DI::IProxyFactory* m_pIDecoratorFactory;
	ISpecialEffectManager* m_pSpeManager;
	bool m_bIsEditorMode;
	int m_width;
	int m_height;
private:

	CreateRenderPtr CreateRendererDX;
	DeleteRenderPtr DeleteRendererDX;
	CreateIProxyFactoryPtr fCreateIProxyFactoryPtr;
	HMODULE m_dll;



};
