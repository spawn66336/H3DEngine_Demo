/**
* @file system_entity.cpp
* 这个文件的主要功能是：
* 
* @author 程陈(Chengchen@h3d.com.cn)
* 
* 更改日志
* ----------------------------------------------------
* ver 1.0.0
*       by 程陈2010.12.17
*/

#include "game_graphic_body.h"

CSystemEntity::CSystemEntity(CGraphicsResMng* mng, CSceneEntity* scene, const std::string& entity_name)
	: CMediaEndEntity(mng, entity_name)
{
	m_render_system = mng->get_render_system();
	m_scene = scene;
	m_ambient_color = H3DColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_ambient_actor_color = H3DColor(1.0f, 1.0f, 1.0f, 1.0f);
}



void CSystemEntity::set_ambient_actor_color(const H3DColor& ambient_color)
{
	m_ambient_actor_color = ambient_color;
	m_scene->set_ambient_actor_color(m_ambient_actor_color);
}

H3DColor CSystemEntity::get_ambient_actor_color() const
{
	return m_ambient_actor_color;
}


void CSystemEntity::set_ambient_color(const H3DColor& ambient_color)
{
	m_ambient_color = ambient_color;
	m_scene->set_ambient_color(m_ambient_color);
}

H3DColor CSystemEntity::get_ambient_color() const
{
	return m_ambient_color;
}

int	CSystemEntity::type(void) const
{
	return MEDIA_END_ENTITY_TYPE_SYSTEM;
}

bool CSystemEntity::is_kind_of(int obj_type)
{
	if (MEDIA_END_ENTITY_TYPE_SYSTEM == obj_type)
	{
		return true;
	}

	return CMediaEndEntity::is_kind_of(obj_type);
}
