/**
* @file GraphicsResMng.h
* ����ļ�����Ҫ�����ǣ�ͼ�ι�����
* 
* @author �̳�(Chengchen@h3d.com.cn)
* 
* ������־
* ----------------------------------------------------
* ver 1.0.0
*       by �̳�2010.1.26
*/

#ifndef _GRAPHICS_RES_MNG_H_
#define _GRAPHICS_RES_MNG_H_

#include "game_graphic_header.h"

class CRenderSystem;
class CEntityFactory;

class CGraphicsResMng
{
public:
	CGraphicsResMng();
	virtual ~CGraphicsResMng();

	/**
	* @brief 
	*     ��ʼ��ͼ����Դ��������
	*
	* @param
	*    ͼ�λ��Ʋ���Ҫ���л��ƵĴ���ָ�롣
	* @return
	*    ���س�ʼ�������
	*/
	virtual int initialize(H3DI::IRender* irender);

	/**
	* @brief 
	*     ��ȡ����ý�������೧��
	*
	* @return
	*    ָ�򴴽�ý�������೧��ָ�롣
	*/
	virtual CEntityFactory* get_entity_factory(void) ;

	/**
	* @brief 
	*     ��ȡ��Ⱦϵͳ
	*
	* @return
	*    ָ����Ⱦϵͳ��ָ�롣
	*/
	virtual CRenderSystem* get_render_system(void) ;

private:
	/**
	* @breif
	*		������Ⱦϵͳ
	* @return
	*		��Ⱦϵͳָ�룬û�з���NULL
	*/	
	virtual CRenderSystem* create_render_system();

	/**
	* @breif
	*		�ͷ���Ⱦϵͳ
	*/	
	virtual void release_render_system();

private:
	CEntityFactory*		m_entity_factory; ///< �ɿ�ʵ��Ĵ�������
	CRenderSystem*	m_render_system; ///< ͼ����Ⱦϵͳ
};

CGraphicsResMng* get_global_graphics_res_mng(H3DI::IRender* irender);

#endif //_GRAPHICS_RES_MNG_H_
