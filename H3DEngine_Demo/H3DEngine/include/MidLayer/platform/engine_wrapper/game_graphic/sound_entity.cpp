/*
*@file  sound_entity.cpp
*@breif 封装声音实体在中间层使用
*@author mozisi
*/
#include "game_graphic_body.h"
#include <platform/client_frame/include/client/i_music_player.h>

CSoundEntity::CSoundEntity(CGraphicsResMng* res_mng
, const std::string& entity_name
, H3D_CLIENT::I_MusicEntity* music_entity)
:CMediaEndEntity(res_mng, entity_name)
,m_i_music_entity(music_entity)
{
	lightAssert(m_i_music_entity);
}

CSoundEntity::~CSoundEntity()
{
	lightAssert(m_res_mng->get_ui_wnd_mng()->GetMusicPlayer());

	m_res_mng->get_ui_wnd_mng()->GetMusicPlayer()->releaseMusicEntityNotInList(m_i_music_entity);
	m_i_music_entity = NULL;
}

int CSoundEntity::type() const
{
	return MEDIA_END_ENTITY_TYPE_SOUND;
}

bool CSoundEntity::is_kind_of(int obj_type)
{
	if (MEDIA_END_ENTITY_TYPE_SOUND == obj_type)
	{
		return true;
	}

	return false;
}

void CSoundEntity::play()
{
	m_i_music_entity->play();
}

void CSoundEntity::stop()
{
	m_i_music_entity->stop();
}

bool CSoundEntity::change_resource(const std::string& res_name)
{
	if (stricmp(m_i_music_entity->getResName().c_str(), res_name.c_str()) == 0)
	{
		return true;
	}
	
	lightAssert(m_res_mng->get_ui_wnd_mng()->GetMusicPlayer());
	H3D_CLIENT::LocalMusicName n;
	n.m_name = res_name;
	H3D_CLIENT::I_MusicEntity* new_entity = m_res_mng->get_ui_wnd_mng()->GetMusicPlayer()
		->createMusicEntityNotInList(m_name.c_str(), n, m_i_music_entity->getType());

	if (new_entity == NULL)
	{
		return false;
	}

	m_res_mng->get_ui_wnd_mng()->GetMusicPlayer()->releaseMusicEntityNotInList(m_i_music_entity);
	m_i_music_entity = new_entity;

	return true;
}