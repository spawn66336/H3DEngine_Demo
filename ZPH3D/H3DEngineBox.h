#ifndef ZP_H3DENGINEBOX
#define ZP_H3DENGINEBOX

#include "ZPDependency.h"
#include "engine_interface.h"
#include "engine_interface_internal.h" 
#include "H3DDressSelector.h"
#include "H3DActionSelector.h"

namespace  ZPH3D
{ 

	typedef H3DI::IRender* (*PF_CREATERENDERPTR)();
	typedef void (*PF_DELETERENDERPTR)();
	typedef H3DI::IProxyFactory* (*PF_CREATEIPROXYFACTORYPTR)();

	class H3DFPSCamera;
	class H3DScene;

	class ZPEXPORT H3DEngineBox
	{
	public:
		H3DEngineBox(void);
		~H3DEngineBox(void);

		/**
		* @brief 引擎容器初始化
		*/
		void Init( const HWND hWnd );

		/**
		* @brief 初始化资源
		*/
		void InitResources( void );

		/**
		* @brief 销毁资源
		*/
		void DestroyResources( void );

		/**
		* @brief 在窗体大小变化时调用
		*/
		void Resize( void );

		/**
		* @brief 渲染一帧
		*/
		void RenderOneFrame( void );
		 
		/**
		* @brief 获得本帧流失的毫秒数
		*/
		inline unsigned int ElapseMS( void ) const { return m_uiElapseTick; }
		 
		/**
		* @brief 销毁引擎容器
		*/
		void Destroy(void);

		void RotateCameraWithUpAxis( const float thetaInRad );

		void RotateCameraWithRightAxis( const float thetaInRad );

		void RandomActorDresses( void );

		void SwitchActorAction( void );

	protected:

		void _FrameBegin( void );

		void _FrameEnd( void );

		void _DrawHelpGrid( void );

		void _SetupCamera( void );

		void _InitActors( void );

		void _InitLights( void );

		void _InitDmls( void );

		void _InitPostProcess( void );

		void _InitUI( void );

		void _PrepareUIToRender( void );

		void _DrawUI( void );

	protected:

		HWND m_hWnd;											//窗体句柄
		int		m_iWidth;											//窗口宽度
		int		m_iHeight;										//窗口高度
		HMODULE m_hEngineDll;								//引擎动态库句柄
		H3DI::IRender* m_pH3DRenderer;					//H3D渲染器
		H3DI::IProxyFactory* m_pProxyFactory;		//代理工厂
		ISpecialEffectManager* m_pSpecEffectMgr; //特效管理器
		H3DI::INewShader* m_pUIShader;
		H3DI::IVB*		m_pVB;
		H3DActionSelector m_actionSelector;
		H3DDressSelector m_dressSelector;

		bool						m_isEditMode;					//是否为编辑器模式
		H3DFPSCamera*	m_pCamera;						//当前相机
		H3DScene*			m_pH3DScene;					
		 

		unsigned int m_uiLastTick;							//上一帧起始时的时间计数
		unsigned int m_uiElapseTick;							//从上一帧到本帧流失的时间

	protected: //从引擎动态库中获得的函数
		PF_CREATERENDERPTR m_pfCreateRenderPtr;
		PF_DELETERENDERPTR  m_pfDeleteRenderPtr;
		PF_CREATEIPROXYFACTORYPTR m_pfCreateIProxyFactoryPtr;
	};

}//ZPH3D

#endif //ZP_H3DENGINEBOX