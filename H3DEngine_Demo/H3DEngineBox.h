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
		* @brief ����������ʼ��
		*/
		void Init( const HWND hWnd );

		/**
		* @brief �ڴ����С�仯ʱ����
		*/
		void Resize( void );

		void RenderOneFrame( void );

		/**
		* @brief ������������
		*/
		void Destroy(void);

	protected:

		HWND m_hWnd;									//������
		int		m_iWidth;									//���ڿ��
		int		m_iHeight;								//���ڸ߶�
		HMODULE m_hEngineDll;						//���涯̬����
		H3DI::IRender* m_pH3DRenderer;			//H3D��Ⱦ��
		H3DI::IProxyFactory* m_pProxyFactory; //������
		ISpecialEffectManager* m_pSpecEffectMgr; //��Ч������

	protected: //�����涯̬���л�õĺ���
		PF_CREATERENDERPTR m_pfCreateRenderPtr;
		PF_DELETERENDERPTR  m_pfDeleteRenderPtr;
		PF_CREATEIPROXYFACTORYPTR m_pfCreateIProxyFactoryPtr;
	};

}//ZPH3D

#endif //ZP_H3DENGINEBOX