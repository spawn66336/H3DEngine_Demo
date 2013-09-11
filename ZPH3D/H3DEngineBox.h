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
		* @brief ����������ʼ��
		*/
		void Init( const HWND hWnd );

		/**
		* @brief ��ʼ����Դ
		*/
		void InitResources( void );

		/**
		* @brief ������Դ
		*/
		void DestroyResources( void );

		/**
		* @brief �ڴ����С�仯ʱ����
		*/
		void Resize( void );

		/**
		* @brief ��Ⱦһ֡
		*/
		void RenderOneFrame( void );
		 
		/**
		* @brief ��ñ�֡��ʧ�ĺ�����
		*/
		inline unsigned int ElapseMS( void ) const { return m_uiElapseTick; }
		 
		/**
		* @brief ������������
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

		HWND m_hWnd;											//������
		int		m_iWidth;											//���ڿ��
		int		m_iHeight;										//���ڸ߶�
		HMODULE m_hEngineDll;								//���涯̬����
		H3DI::IRender* m_pH3DRenderer;					//H3D��Ⱦ��
		H3DI::IProxyFactory* m_pProxyFactory;		//������
		ISpecialEffectManager* m_pSpecEffectMgr; //��Ч������
		H3DI::INewShader* m_pUIShader;
		H3DI::IVB*		m_pVB;
		H3DActionSelector m_actionSelector;
		H3DDressSelector m_dressSelector;

		bool						m_isEditMode;					//�Ƿ�Ϊ�༭��ģʽ
		H3DFPSCamera*	m_pCamera;						//��ǰ���
		H3DScene*			m_pH3DScene;					
		 

		unsigned int m_uiLastTick;							//��һ֡��ʼʱ��ʱ�����
		unsigned int m_uiElapseTick;							//����һ֡����֡��ʧ��ʱ��

	protected: //�����涯̬���л�õĺ���
		PF_CREATERENDERPTR m_pfCreateRenderPtr;
		PF_DELETERENDERPTR  m_pfDeleteRenderPtr;
		PF_CREATEIPROXYFACTORYPTR m_pfCreateIProxyFactoryPtr;
	};

}//ZPH3D

#endif //ZP_H3DENGINEBOX