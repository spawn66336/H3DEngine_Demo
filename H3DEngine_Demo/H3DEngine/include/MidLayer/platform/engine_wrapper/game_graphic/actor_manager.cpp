/**
* @file actor_manager.cpp
* 这个文件的主要功能是：
* 
* @author 程陈(Chengchen@h3d.com.cn)
* 
* 更改日志
* ----------------------------------------------------
* ver 1.0.0
*       by 程陈2010.11.16
*/

#include "game_graphic_body.h"

ActorManager::ActorManager():m_is_male(false), m_pet_is_male(false)
{
	m_graphic_res_mng = NULL;
	m_modifier_factory = NULL;
	reset_iterator();

	m_def_effect_res_path = "../data/universal/asset/null.spe";
	m_def_model_res_path = "../resources/art/stage/common_object/x52_zhui_a.dml";
	m_def_actor_res_path = "../resources/config/shared/item/dress.xml";
	m_def_skeleton_model_res_path = "../resources/art/role/npc/palace_finance_001/palace_finance_001.chr";
	m_def_sound_res_path = "../resources/audio/sound/perfect.wav";
}

ActorManager::~ActorManager()
{
	remove_sysactor_from_actorlist(LL_ZERO);
	remove_sysactor_from_actorlist(LL_ONE);
	remove_sysactor_from_actorlist(LL_TWO);

	for (LodSystemActorsItr iter = m_system_actor.begin(); iter != m_system_actor.end(); ++iter)
	{
		for (std::map<std::string, Actor*>::iterator it = iter->second.begin() ; it != iter->second.end(); ++it)
		{
			if (it->second)
			{
				/*
				在remove_sysactor_from_actorlist()已经将系统物体从关卡中删除了，
				此处不用删除, wangheng 2012.07.30
				*/
				//remove_from_level(it->second);
				delete (it->second);
				it->second = NULL;
			}
		}
	}
	
	m_system_actor.clear();

	for (ActorList::iterator it = m_actor_collectioon.begin();
		it != m_actor_collectioon.end();)
	{
		remove_from_level((*it));
		delete *it++;
	}

	if (m_level)
	{
		release_level_entity();
	}
	//delete m_graphic_res_mng;
	delete m_modifier_factory;
}

bool ActorManager::initialize(H3DI::IRender* irender, H3DI::ILevel* ilevel)
{
	m_graphic_res_mng = get_global_graphics_res_mng(irender);
    
	m_modifier_factory = create_modifier_factory();
	create_level_entity(ilevel);
	return true;
}

bool ActorManager::add_actor(Actor* actor)
{
	assert(NULL != actor);
	Actor* p = find_actor(actor->get_name());
	if (p != NULL)
	{
		if (p != actor)
		{
			assert(false);
			return false;
		}

		return true;
	}

	addSystemActor(actor->get_name(), actor);

	//注掉 by wangheng 2012.06.20 灯光entity的添加放回至doPrepasslightInitialize中。
	//added by wangheng 2011.11.18，灯光entity的添加，从doPrepasslightInitialize中移到此处，需要添加下面的代码
	/*Actor::EntityList entitylist;
	if(actor->contain_entity_type_recursive(MEDIA_END_ENTITY_TYPE_PREPASS_LIGHT, entitylist))
	{
		Actor::EntityList::iterator it = entitylist.begin();
		for (; it != entitylist.end(); ++it)
		{
			m_created_light_collection.insert(std::make_pair(dynamic_cast<CPrePassLightEntity*>(*it), false));
		}
		entitylist.clear();
	}*/

	m_actor_collectioon.push_back(actor);
	add_to_level(actor);
	reset_iterator();

	return true;
}

bool ActorManager::remove_actor(Actor* actor)
{
	assert(NULL != actor);

	Actor* p = find_actor(actor->get_name());
	if (p == actor)
	{
		m_actor_collectioon.remove(actor);
		remove_from_level(actor);
		reset_iterator();
		return true;
	}
	else
	{
		return false;
	}
}

void ActorManager::add_to_level(Actor* actor)
{
	if (actor)
	{
		Actor::EntityList entitylist;
		actor->contain_entity_recursive(entitylist);
		Actor::EntityList::iterator itr = entitylist.begin();
		while (itr != entitylist.end())
		{
			
			//C3DEntity* entity3d = dynamic_cast<C3DEntity*>(*itr);

			///*增加一步判断，因为系统物体并不是派生自C3DEntity;
			//目前系统物体放在这处理，以后可能会移走*/			
			//if (entity3d)
			//	m_level->add_to_level(entity3d);
			m_level->add_to_level(*itr);
			
			++itr;
		}
	}
}

void ActorManager::remove_from_level(Actor* actor)
{
	if (actor)
	{
		Actor::EntityList entitylist;
		actor->contain_entity_recursive(entitylist);
		Actor::EntityList::iterator itr = entitylist.begin();
		while (itr != entitylist.end())
		{
			//C3DEntity* entity3d = dynamic_cast<C3DEntity*>(*itr);
			///*增加一步判断，因为系统物体并不是派生自C3DEntity;
			//目前系统物体放在这处理，以后可能会移走*/
			//if (entity3d)
			//	m_level->remove_from_level(entity3d);
			m_level->remove_from_level(*itr);
			++itr;
		}
	}
}

void ActorManager::clear_actor()
{
    ActorList::iterator iter = m_actor_collectioon.begin();
    for (;iter != m_actor_collectioon.end();++iter)
    {
        assert(*iter);
        delete (*iter);
    }
    m_actor_collectioon.clear();
}

Actor* ActorManager::find_actor(const char* actor_name)
{
	ActorList::const_iterator it = std::find_if(m_actor_collectioon.begin(),m_actor_collectioon.end(), bind2nd(name_equal<Actor>(), actor_name));

	if (it != m_actor_collectioon.end()) //如果找到
	{
		return (*it);
	}

	return NULL;
;
}

void ActorManager::reset_iterator()
{
	m_iterator = m_actor_collectioon.begin();
}

Actor* ActorManager::next()
{
	Actor* p = NULL;
	if (m_iterator != m_actor_collectioon.end())
	{
		p = (*m_iterator++);
	}

	return p;
}

CMediaEndEntity* ActorManager::create_entity(const char* entitytypeid, const char* id)
{
	if (NULL == m_graphic_res_mng)
	{
		return NULL;
	}

	CMediaEndEntity* entity = NULL;
	CEntityFactory* factory = m_graphic_res_mng->get_entity_factory();

	if (stricmp(entitytypeid, "camera") == 0)
	{
		entity = factory->create_camera(id);
	}
	else if (stricmp(entitytypeid, "effect") == 0)
	{
		entity = factory->create_effect(m_def_effect_res_path, id, 0);
	}
	else if (stricmp(entitytypeid, "point_prepass_light") == 0)
	{
		entity = factory->create_point_prepass_light(id);
	}
	else if (stricmp(entitytypeid, "wimble_prepass_light") == 0)
	{
		entity = factory->create_wimble_prepass_light(id);
	}
	else if (stricmp(entitytypeid, "direction_prepass_light") == 0)
	{
		entity = factory->create_direction_prepass_light(id);
	}
	else if (stricmp(entitytypeid, "model") == 0)
	{
		entity = factory->create_model(m_def_model_res_path, id, 0);
	}
	else if (stricmp(entitytypeid, "system") == 0)
	{
		entity = factory->create_system_entity(m_level, id);
	}
	else if (stricmp(entitytypeid, "avatar") == 0)
	{
		entity = factory->create_avatar_entity(m_is_male, id, 0);
	}
	else if (stricmp(entitytypeid, "NPC") == 0)//原名过长，稍微修改了一下名字
	{
		entity = factory->create_skeleton_model_entity(m_def_skeleton_model_res_path, id, 0);
	}
	else if (stricmp(entitytypeid, "system_lightmap") == 0)
	{
		entity = factory->create_lightmap_adjust_entity(m_level, id);
	}
	else if (stricmp(entitytypeid, "pet") == 0)//原名过长，稍微修改了一下名字
	{
		entity = factory->create_avatar_skeleton_model_entity(m_pet_is_male, id, 0);
	}
	//else if (stricmp(entitytypeid, "sound") == 0)//声音
	//{
	//	entity = factory->create_sound_entity(id, m_def_sound_res_path);
	//}
	else
	{
		//是否为扩展entity类型
		entity = factory->create_extend_entity(entitytypeid, id);

		if(NULL == entity)
			return NULL;
	}

	if (NULL == entity)
	{
		return entity;
	}

	if (entity->is_kind_of(MEDIA_END_ENTITY_TYPE_PREPASS_LIGHT))
	{
		doPrepasslightInitialize(entity);
	}

	//factory->add_entity(entity);

	return entity;	
}

void ActorManager::release_entity(CMediaEndEntity* entity)
{
	if (NULL != m_graphic_res_mng)
	{
		if (entity->is_kind_of(MEDIA_END_ENTITY_TYPE_PREPASS_LIGHT))
		{
			CPrePassLightEntity* light = dynamic_cast<CPrePassLightEntity*>(entity);
			PrepassLightCollection::iterator it = m_created_light_collection.find(light);
			if (it != m_created_light_collection.end())
			{
				m_created_light_collection.erase(it);
			}
		}

		if (entity->is_added_to_level())
		{
			C3DEntity* entity3d = dynamic_cast<C3DEntity*>(entity);
			if (entity3d)
				m_level->remove_from_level(entity3d);
		}

		m_graphic_res_mng->get_entity_factory()->release(entity);
	}
}

CLevelEntity* ActorManager::create_level_entity(H3DI::ILevel* ilevel /* = NULL */)
{
	CEntityFactory* factory = m_graphic_res_mng->get_entity_factory();
	m_level = factory->create_level_entity(ilevel);
	return m_level;
}

void ActorManager::release_level_entity()
{
	CEntityFactory* factory = m_graphic_res_mng->get_entity_factory();
	factory->release_level_entity(m_level);
	m_level = NULL;
}


CLevelEntity* ActorManager::get_level_entity()
{
	return m_level;
}

void ActorManager::doPrepasslightInitialize(CMediaEndEntity* light)
{
	CPrePassLightEntity* entity = dynamic_cast<CPrePassLightEntity*>(light);
	if (NULL == entity)
	{
		return;
	}

	GlareProperty property;
	property.width = 1.0f;
	property.height = 1.0f;
	property.texture_name = "";
	property.shader_lib = "";
	property.shader_instance_name = "";
	property.glare_distance = 0.0f;
	property.glare_rotation_velocity = 0.0f;
	property.is_visible = false;
	property.is_rotate_with_camera = true;
	property.is_view_camera = true;


	for (int i = 0; i < 3; ++i)
	{
		entity->create_glare(property);
	}

	m_created_light_collection.insert(std::make_pair(entity, false));
}

CGraphicsResMng* ActorManager::get_graphic_res_mng()
{
	return m_graphic_res_mng;
}

PropertyModifierFactory* ActorManager::create_modifier_factory()
{
	return new PropertyModifierFactory();
}

PropertyModifierFactory* ActorManager::getPropertyModifierFactory()
{
	return m_modifier_factory;
}

void ActorManager::drawLightLine(CPrePassLightEntity* light)
{
	get_graphic_res_mng()->get_render_system()->drawLightLine(light);
}

void ActorManager::add_using_light(CPrePassLightEntity* light)
{
	assert(NULL != light);
	m_created_light_collection[light] = true;
}

const ActorManager::PrepassLightCollection& ActorManager::get_light_collection()
{
	return m_created_light_collection;
}

void ActorManager::reset_light_status()
{
	for (PrepassLightCollection::iterator it = m_created_light_collection.begin(); it != m_created_light_collection.end(); ++it)
	{
		it->second = false;
	}
}

void ActorManager::set_default_resource_path(const char* default_eff_res
											 , const char* default_model_res
											 , const char* default_actor_res
											 , const char* default_skeleton_model_res)
{
	if (NULL != default_eff_res)
	{
		m_def_effect_res_path = default_eff_res;
	}

	if (NULL != default_model_res)
	{
		m_def_model_res_path = default_model_res;
	}

	if (NULL != default_actor_res)
	{
		m_def_actor_res_path = default_actor_res;
	}

	if (NULL != default_skeleton_model_res)
	{
		m_def_skeleton_model_res_path = default_skeleton_model_res;
	}
}

const char* ActorManager::getTopActorNameById(unsigned int id)
{
	reset_iterator();
	Actor* actor = next();
	while (actor)
	{
		if (actor->find_entity_recursive(id))
		{
			return actor->get_name();
		}

		actor = next();
	}

	return 0;
}

const char* ActorManager::getLodSystemActorName(LOD_LEVEL lod)
{
	switch (lod)
	{
	case LL_ZERO:
		return "x52_system_config";
	case LL_ONE:
		return "_lod1";
	case LL_TWO:
		return "_lod2";
	default:
		assert(false);
		return NULL;
	}
}

LOD_LEVEL ActorManager::isLodSystemActorName(const char* strName)
{
	assert(strName);
	std::string strActName = strName;
	std::transform(strActName.begin(), strActName.end(), strActName.begin(), tolower);

	std::string::size_type pos_lod1 = strActName.find(getLodSystemActorName(LL_ONE), 0);
	std::string::size_type pos_lod2 = strActName.find(getLodSystemActorName(LL_TWO), 0);

	if (stricmp(strActName.c_str(), getLodSystemActorName(LL_ZERO)) == 0)
		return LL_ZERO;
	else if(pos_lod1 != std::string::npos && pos_lod1 == (strActName.length() - strlen(getLodSystemActorName(LL_ONE))))
		return LL_ONE;
	else if (pos_lod2 != std::string::npos && pos_lod2 == (strActName.length() - strlen(getLodSystemActorName(LL_TWO))))
		return LL_TWO;

	return LL_NULL;
}

Actor* ActorManager::getSystemActor(const char* strName)
{
	if (NULL == strName)
		return NULL;

	for(LodSystemActorsItr iter = m_system_actor.begin(); iter != m_system_actor.end(); ++iter)
	{
		std::map<std::string, Actor*>::iterator it = iter->second.find(strName);

		if (it == iter->second.end())
			continue;
		return it->second;
	}
	return NULL;
}

bool ActorManager::addSystemActor(const char* strName, Actor* sysActor)
{
	assert(strName && sysActor);

	std::string strActName = strName;
	std::transform(strActName.begin(), strActName.end(), strActName.begin(), tolower);

	std::string::size_type pos_lod1 = strActName.find(getLodSystemActorName(LL_ONE), 0);
	std::string::size_type pos_lod2 = strActName.find(getLodSystemActorName(LL_TWO), 0);

	if (stricmp(strActName.c_str(), getLodSystemActorName(LL_ZERO)) == 0)
	{
		std::map<std::string, Actor*>::iterator itr = m_system_actor[LL_ZERO].find(strName);
		if (itr == m_system_actor[LL_ZERO].end())
			m_system_actor[LL_ZERO].insert(std::make_pair(strName, sysActor));
		else
			itr->second = sysActor;
		return true;
	}
	else if(pos_lod1 != std::string::npos && pos_lod1 == (strActName.length() - strlen(getLodSystemActorName(LL_ONE))))
	{
		std::map<std::string, Actor*>::iterator itr = m_system_actor[LL_ONE].find(strName);
		if (itr == m_system_actor[LL_ONE].end())
			m_system_actor[LL_ONE].insert(std::make_pair(strName, sysActor));
		else
			itr->second = sysActor;
		return true;
	}
	else if (pos_lod2 != std::string::npos && pos_lod2 == (strActName.length() - strlen(getLodSystemActorName(LL_TWO))))
	{
		std::map<std::string, Actor*>::iterator itr = m_system_actor[LL_TWO].find(strName);
		if (itr == m_system_actor[LL_TWO].end())
			m_system_actor[LL_TWO].insert(std::make_pair(strName, sysActor));
		else
			itr->second = sysActor;
		return true;
	}

	return false;
}

bool ActorManager::removeSystemActor(const char* strName)
{
	if (NULL == strName)
		return NULL;

	std::string strActName = strName;
	std::transform(strActName.begin(), strActName.end(), strActName.begin(), tolower);

	std::string::size_type pos_lod1 = strActName.find(getLodSystemActorName(LL_ONE), 0);
	std::string::size_type pos_lod2 = strActName.find(getLodSystemActorName(LL_TWO), 0);

	if (stricmp(strActName.c_str(), getLodSystemActorName(LL_ZERO)) == 0)
	{
		std::map<std::string, Actor*>::iterator itr = m_system_actor[LL_ZERO].find(strName);
		if (itr == m_system_actor[LL_ZERO].end())
			return false;

		itr->second = NULL;
		return true;
	}
	else if(pos_lod1 != std::string::npos && pos_lod1 == (strActName.length() - strlen(getLodSystemActorName(LL_ONE))))
	{
		std::map<std::string, Actor*>::iterator itr = m_system_actor[LL_ONE].find(strName);
		if (itr == m_system_actor[LL_ONE].end())
			return false;

		itr->second = NULL;
		return true;
	}
	else if (pos_lod2 != std::string::npos && pos_lod2 == (strActName.length() - strlen(getLodSystemActorName(LL_TWO))))
	{
		std::map<std::string, Actor*>::iterator itr = m_system_actor[LL_TWO].find(strName);
		if (itr == m_system_actor[LL_TWO].end())
			return false;

		itr->second = NULL;
		return true;
	}

	return false;
}

void ActorManager::remove_sysactor_from_actorlist(int lod)
{
	std::map<std::string, Actor*>::iterator it = m_system_actor[lod].begin();
	while(it != m_system_actor[lod].end())
	{
		Actor* sys_actor = find_actor(it->first.c_str());
		if (sys_actor)
		{
			remove_actor(sys_actor);
			addSystemActor(it->first.c_str(), sys_actor);
		}
		++it;
	}
}

void ActorManager::add_sysactor_to_actorlist(int lod)
{
	std::map<std::string, Actor*>::iterator it = m_system_actor[lod].begin();
	while(it != m_system_actor[lod].end())
	{
		Actor* sys_actor = it->second;
		if (sys_actor)
		{
			add_actor(sys_actor);
			removeSystemActor(sys_actor->get_name());
		}
		++it;
	}
}

void ActorManager::reset_some_sysactor_iter(int lod)
{
	m_some_lod_system_actor_itr = m_system_actor[lod].begin();
}

const char* ActorManager::next_some_sysactor_name(int lod)
{
	if (m_some_lod_system_actor_itr != m_system_actor[lod].end())
	{
		const char* strName = m_some_lod_system_actor_itr->first.c_str();
		m_some_lod_system_actor_itr++;
		return strName;
	}
	return NULL;
}

Actor* ActorManager::next_some_sysactor(int lod)
{
	if (m_some_lod_system_actor_itr != m_system_actor[lod].end())
	{
		Actor* actor = m_some_lod_system_actor_itr->second;
		m_some_lod_system_actor_itr++;
		return actor;
	}
	return NULL;
}
