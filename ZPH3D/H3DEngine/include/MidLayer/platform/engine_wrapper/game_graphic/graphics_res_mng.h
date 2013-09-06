/**
* @file GraphicsResMng.h
* 这个文件的主要功能是：图形管理类
* 
* @author 程陈(Chengchen@h3d.com.cn)
* 
* 更改日志
* ----------------------------------------------------
* ver 1.0.0
*       by 程陈2010.1.26
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
	*     初始化图形资源管理器。
	*
	* @param
	*    图形绘制层需要进行绘制的窗口指针。
	* @return
	*    返回初始化情况。
	*/
	virtual int initialize(H3DI::IRender* irender);

	/**
	* @brief 
	*     获取创建媒体对象的类厂。
	*
	* @return
	*    指向创建媒体对象的类厂的指针。
	*/
	virtual CEntityFactory* get_entity_factory(void) ;

	/**
	* @brief 
	*     获取渲染系统
	*
	* @return
	*    指向渲染系统的指针。
	*/
	virtual CRenderSystem* get_render_system(void) ;

private:
	/**
	* @breif
	*		创建渲染系统
	* @return
	*		渲染系统指针，没有返回NULL
	*/	
	virtual CRenderSystem* create_render_system();

	/**
	* @breif
	*		释放渲染系统
	*/	
	virtual void release_render_system();

private:
	CEntityFactory*		m_entity_factory; ///< 可控实体的创建工厂
	CRenderSystem*	m_render_system; ///< 图形渲染系统
};

CGraphicsResMng* get_global_graphics_res_mng(H3DI::IRender* irender);

#endif //_GRAPHICS_RES_MNG_H_
