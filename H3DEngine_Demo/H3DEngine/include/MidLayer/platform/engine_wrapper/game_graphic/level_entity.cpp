#include "game_graphic_body.h"

CLevelEntity::CLevelEntity(CGraphicsResMng* res_mgn, H3DI::ILevel* level):CSceneEntity(res_mgn, level)
{
	m_level = level;
}

CLevelEntity::~CLevelEntity()
{
	if (m_level)
	{
		m_level->Release();
		m_level = NULL;
	}
}

void CLevelEntity::set_ILevel(H3DI::ILevel* ilevel)
{
	m_level = ilevel;
	m_scene = m_level;
}