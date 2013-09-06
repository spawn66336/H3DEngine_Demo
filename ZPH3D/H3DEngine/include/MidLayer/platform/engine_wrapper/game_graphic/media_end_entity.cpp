/**
* @file media_end_entity.cpp
* 这个文件的主要功能是：
*	  可控实体基类的实现
* 
* @author 程陈(Chengchen@h3d.com.cn)
* 
* 更改日志
* ----------------------------------------------------
* ver 1.0.0
*       by 程陈2010.1.25
*/

#include "game_graphic_body.h"

unsigned int CMediaEndEntity::m_s_entity_id = 10000;
CMediaEndEntity::CMediaEndEntity(CGraphicsResMng* mng, const std::string& entity_name)
: m_level_entity(0)
{
	assert(NULL != mng);
	assert(entity_name.length() != 0);

	m_res_mng = mng;
	m_name = entity_name;
	m_is_enable = true;

	m_actor = NULL;
	m_entity_id = m_s_entity_id++;
	m_guid = "";

	m_b_use_delta_time = false;
	m_f_delta_time = 0.0f;

	m_lod_level = LL_ZERO;
}

CMediaEndEntity::CMediaEndEntity()
: m_level_entity(0)
{
	m_res_mng = NULL;
	m_name = "";
	m_is_enable = true;
	m_actor = NULL;
	m_guid = "";

	m_b_use_delta_time = false;
	m_f_delta_time = 0.0f;

	m_lod_level = LL_ZERO;
}

CMediaEndEntity::~CMediaEndEntity(void)
{
}

CGraphicsResMng* CMediaEndEntity::get_graphics_res_mng()
{
	return m_res_mng;
}

//////////////////////////////////////////////////////////////////////////
//其他操作
int	CMediaEndEntity::type(void) const
{
	return MEDIA_END_ENTITY_TYPE_ENTITY;
}

void CMediaEndEntity::set_name(const char* obj_name)
{
	m_name = obj_name;
}

const char* CMediaEndEntity::get_name(void) const
{
	return m_name.c_str();
}

bool CMediaEndEntity::is_kind_of(int obj_type)
{
	if (MEDIA_END_ENTITY_TYPE_ENTITY == obj_type)
	{
		return true;
	}

	return false;
}

bool CMediaEndEntity::is_enable(void) const
{
	return m_is_enable;
}

void CMediaEndEntity::set_enable(bool enable)
{
	m_is_enable = enable;
}

void CMediaEndEntity::update(float interval)
{
	if(!is_enable())
	{
		return;
	}

	//Signal<ENTITY_BEGIN_UPDATE>(this, interval);

	//用于
	if (m_b_use_delta_time && m_f_delta_time <= interval)
	{
		interval = m_f_delta_time;
		m_b_use_delta_time = false;
		m_f_delta_time = 0.0f;
	}
	//每个类做自己特殊的操作
	do_update(interval);
	
	//Signal<ENTITY_END_UPDATE>(this);
}

void CMediaEndEntity::do_update(float interval)
{
	NULL;
}

void CMediaEndEntity::render(CRenderSystem& render_system)
{
	NULL;
}

bool CMediaEndEntity::pushHardwareSelect(CRenderSystem& render_system, bool sel_line)
{
	return false;
}

void CMediaEndEntity::set_actor(Actor* actor)
{
	m_actor = actor;
}

Actor* CMediaEndEntity::get_actor()
{
	return m_actor;
}

unsigned int CMediaEndEntity::get_entity_id() const
{
	return m_entity_id;
}

