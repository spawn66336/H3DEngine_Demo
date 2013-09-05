#ifndef ZP_H3DENGINEBOX
#define ZP_H3DENGINEBOX


#include "engine_interface.h"
#include "engine_interface_internal.h"

namespace ZPH3D
{ 

	typedef H3DI::IRender* (*PF_CREATERENDERPTR)();
	typedef void (*PF_DELETERENDERPTR)();
	typedef H3DI::IProxyFactory* (*PF_CREATEIPROXYFACTORYPTR)();

	class H3DEngineBox
	{
	public:
		H3DEngineBox(void);
		~H3DEngineBox(void);

		/**
		* @brief 引擎容器初始化
		*/
		void Init( const HWND hWnd );

		/**
		* @brief 在窗体大小变化时调用
		*/
		void Resize( void );

		void RenderOneFrame( void );

		/**
		* @brief 销毁引擎容器
		*/
		void Destroy(void);

	protected:

		HWND m_hWnd;									//窗体句柄
		int		m_iWidth;									//窗口宽度
		int		m_iHeight;								//窗口高度
		HMODULE m_hEngineDll;						//引擎动态库句柄
		H3DI::IRender* m_pH3DRenderer;			//H3D渲染器
		H3DI::IProxyFactory* m_pProxyFactory; //代理工厂
		ISpecialEffectManager* m_pSpecEffectMgr; //特效管理器

	protected: //从引擎动态库中获得的函数
		PF_CREATERENDERPTR m_pfCreateRenderPtr;
		PF_DELETERENDERPTR  m_pfDeleteRenderPtr;
		PF_CREATEIPROXYFACTORYPTR m_pfCreateIProxyFactoryPtr;
	};

}//ZPH3D

#endif //ZP_H3DENGINEBOX