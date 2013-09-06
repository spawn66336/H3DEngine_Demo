/**
* @file GraphicsResMng.cpp
* 这个文件的主要功能是：图形资源管理类
* 
* @author 程陈(Chengchen@h3d.com.cn)
* 
* 更改日志
* ----------------------------------------------------
* ver 1.0.0
*       by 程陈2010.1.26
*/

#include "game_graphic_body.h"

CGraphicsResMng::CGraphicsResMng()
{
	m_render_system = NULL;
	m_entity_factory = NULL;
}

CGraphicsResMng::~CGraphicsResMng()
{
	release_render_system();
}

int CGraphicsResMng::initialize(H3DI::IRender* irender)
{
	create_render_system();
	m_entity_factory = get_global_entity_factory(irender);

	m_render_system->initialize(irender);

	return ERROR_CLIENT_SUCCESS;

}

CEntityFactory* CGraphicsResMng::get_entity_factory(void)
{
	return m_entity_factory;
}

CRenderSystem* CGraphicsResMng::get_render_system(void)
{
	return m_render_system;
}

CRenderSystem* CGraphicsResMng::create_render_system()
{
	assert(NULL == m_render_system);
	m_render_system = new CRenderSystem(this);
	assert(NULL != m_render_system);

	return m_render_system;
}

void CGraphicsResMng::release_render_system()
{
	delete m_render_system;
}

CGraphicsResMng* get_global_graphics_res_mng(H3DI::IRender* irender)
{
	static CGraphicsResMng* graphics_res_mng = 0;
	if (!graphics_res_mng)
	{
		graphics_res_mng = new CGraphicsResMng;
		graphics_res_mng->initialize(irender);
	}
	return graphics_res_mng;
}