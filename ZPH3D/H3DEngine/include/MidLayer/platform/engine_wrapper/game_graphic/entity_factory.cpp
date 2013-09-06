/**
* @file entity_factory.cpp
* 这个文件的主要功能是：
*			实现了可控实体创建和销毁的抽象类厂类CEntityFactory
* @author 程陈(Chengchen@h3d.com.cn)
* 
* 更改日志
* ----------------------------------------------------
* ver 1.0.0
*       by 程陈2010.1.26
*/

#include "game_graphic_body.h"
#include "platform/include/i_entity_pool.h"

int CEntityFactory::s_Level_no = 0;

CEntityFactory::CEntityFactory(CGraphicsResMng* res_mng)
{
	assert(NULL != res_mng);
	m_res_mng = res_mng;
	m_render_system = res_mng->get_render_system();
	assert(NULL != m_render_system);
}

CEntityFactory::~CEntityFactory()
{
	//for (size_t i = 0; i < m_create_entities.size();) //销毁Actor
	//{
	//	release(m_create_entities[i]);
	//	++i;
	//}
	Levelmap::iterator itr = m_created_level_entities.begin();
	while (itr != m_created_level_entities.end())
	{
		Levelmap::iterator tmpitr = itr;
		++itr;
		release_level_entity(tmpitr->first);
	}
}

CCameraEntity* CEntityFactory::create_camera(const std::string& camera_name)
{
	assert(camera_name.length() > 0);

	CCameraEntity* camera = new CCameraEntity(m_res_mng, camera_name, m_render_system->get_IRender()->CreateCamera());
	return camera;
}

CModelEntity* CEntityFactory::create_model(const std::string& resource_name, const std::string& model_name
										   , int lod, bool is_sync, int priority)
{
	assert(model_name.length() > 0);

	CModelEntity* model = new CModelEntity(m_res_mng, model_name, resource_name, lod);
	model->create_engine_object(is_sync, priority);

	model->set_enable(true);
	model->set_visible(true);
	model->reset();

	if (get_pool())
	{
		get_pool()->registerModel(model);
	}

	return model;
}


CSkeletonModelEntity* CEntityFactory::create_skeleton_model_entity(const std::string& resource_name, const std::string& model_name
																   , int lod, bool is_sync, int priority)
{
	assert(model_name.length() > 0);

	CSkeletonModelEntity* model = new CSkeletonModelEntity(m_res_mng, model_name, resource_name, lod);
	model->create_engine_object(is_sync, priority);

	model->set_enable(true);
	model->set_visible(true);
	model->reset();

	if (get_pool())
	{
		get_pool()->registerSkeletonModel(model);
	}

	return model;
}

AvatarEntity* CEntityFactory::create_avatar_entity(bool is_male, const std::string& avatar_name
												   , int lod)
{
	assert(avatar_name.length() > 0);

	AvatarEntity* avatar = new AvatarEntity(m_res_mng, avatar_name, is_male, lod);
	avatar->set_entity_state(ES_HIDE);

	avatar->set_enable(true);
	avatar->set_visible(true);

	if (get_pool())
	{
		get_pool()->registerAvatar(avatar);
	}

	return avatar;
}

CAvatarSkeletonModelEntity* CEntityFactory::create_avatar_skeleton_model_entity(bool is_male , const std::string& name
																				, int lod)
{
	assert(name.length() > 0);

	CAvatarSkeletonModelEntity* entity = new CAvatarSkeletonModelEntity(m_res_mng, name, is_male, lod);
	entity->set_entity_state(ES_HIDE);

	entity->set_enable(true);
	entity->set_visible(true);

	if (get_pool())
	{
		get_pool()->registerAvatarSkeletonModel(entity);
	}

	return entity;
}

CEffectEntity* CEntityFactory::create_effect(const std::string& resource_name, const std::string& effect_name
											 , int lod, bool is_sync, int priority)
{
	assert(effect_name.length() > 0);

	CEffectEntity* effect = new CEffectEntity(m_res_mng, effect_name, resource_name, lod);
	effect->create_engine_object(is_sync, priority);

	effect->set_enable(true);
	effect->set_visible(true);
	effect->reset();

	if (get_pool())
	{
		get_pool()->registerEffect(effect);
	}

	return effect;
}

CLevelEntity* CEntityFactory::create_level_entity(H3DI::ILevel* ilevel/* = NULL */)
{
	//assert(level_name.length() > 0);
	CLevelEntity* level_entity;
	EngineObjectCreateFlag eFlag = EOCF_NULL;
	if (NULL == ilevel)
	{
		
		H3DI::ILevel* level = NULL;
		//if (m_resmng)
		//{
		//	level = m_resmng->createLevel("client_level_");
		//	eFlag = EOCF_CREATED_SELF_CLIENT;
		//}
		//else
		//{
			char strLevel[64] = {0};
			sprintf(strLevel, "middle_layer_level_%3d", s_Level_no);
			level = m_render_system->get_IRender()->CreateLevel(strLevel);
			eFlag = EOCF_CREATED_SELF_MIDDLE_LAYER;
		//}

		level_entity = new CLevelEntity(m_res_mng, level);
	}
	else
	{
		level_entity = new CLevelEntity(m_res_mng, ilevel);
		eFlag = EOCF_PASSED_IN;
	}

	s_Level_no++;
	m_created_level_entities.insert(std::make_pair(level_entity, eFlag));
	return level_entity;
}

void CEntityFactory::release_level_entity(CLevelEntity* level)
{
	if (level)
	{
		Levelmap::iterator itr = m_created_level_entities.find(level);
		if (itr == m_created_level_entities.end())
			return;

		//if (EOCF_CREATED_SELF_CLIENT == itr->second)
		//{
		//	if (m_resmng)
		//	{
		//		m_resmng->releaseLevel(level->get_ILevel());
		//		level->set_ILevel(NULL);
		//	}
		//	else
		//	{
		//		assert(false);
		//	}
		//}
		//else if (EOCF_PASSED_IN == itr->second)
		if (EOCF_PASSED_IN == itr->second)
		{
			level->set_ILevel(NULL);
		}
		
		delete level;

		m_created_level_entities.erase(itr);
	}
}

ISpecialEffect* CEntityFactory::get_effect(const char* path)
{
	return m_render_system->getIEffectManager()->getSpecialEffect(path);
}

//CSoundEntity* CEntityFactory::create_sound_entity(const char* name 
//												   , const string& resname)
//{
//	assert(resname.length() > 0);
//	assert(name);
//	
//	H3D_CLIENT::LocalMusicName n;
//	n.m_name = resname;
//	
//	assert(m_res_mng->get_ui_wnd_mng()->GetMusicPlayer());
//	H3D_CLIENT::I_MusicEntity* i_music = m_res_mng->get_ui_wnd_mng()->GetMusicPlayer()
//		->createMusicEntityNotInList(name, n, H3D_CLIENT::X5Sound_Type_Effect);
//	assert(i_music);
//
//	i_music->play();
//	i_music->stop();
//
//	CSoundEntity *entity = new CSoundEntity(m_res_mng, name, i_music);
//
//	return entity;
//}

CPointPrePassLightEntity* CEntityFactory::create_point_prepass_light(const std::string& light_name)
{
	assert(light_name.length() > 0);
	
	H3DI::IPrePassLight* light = m_render_system->get_IRender()->CreatePrePassLight(H3DI::LIGHT_POINT);
	light->EnableFakeLightSpot(false);
	CPointPrePassLightEntity* entity = new CPointPrePassLightEntity(m_res_mng, light_name, light);

	return entity;
}

CWimblePrePassLightEntity* CEntityFactory::create_wimble_prepass_light(const std::string& light_name)
{
	assert(light_name.length() > 0);

	H3DI::IPrePassLight* light = m_render_system->get_IRender()->CreatePrePassLight(H3DI::LIGHT_PROJECT);
	light->EnableFakeLightSpot(false);
	CWimblePrePassLightEntity* entity = new CWimblePrePassLightEntity(m_res_mng, light_name, light);

	entity->set_inner_angle(30);
	entity->set_outter_angle(50);

	return entity;
}

CDirectionPrePassLightEntity* CEntityFactory::create_direction_prepass_light(const std::string& light_name)
{
	assert(light_name.length() > 0);

	H3DI::IPrePassLight* light = m_render_system->get_IRender()->CreatePrePassLight(H3DI::LIGHT_DIR);
	light->EnableFakeLightSpot(false);
	CDirectionPrePassLightEntity* entity = new CDirectionPrePassLightEntity(m_res_mng, light_name, light);

	return entity;
}

CSystemEntity* CEntityFactory::create_system_entity(CSceneEntity* scene, const char* entity_name)
{
	CSystemEntity* entity = new CSystemEntity(m_res_mng, scene, entity_name);
	return entity;
}

CLightmapAdjustEntity* CEntityFactory::create_lightmap_adjust_entity(CSceneEntity* scene, const char* entity_name)
{
	CLightmapAdjustEntity* entity = new CLightmapAdjustEntity(m_res_mng, scene, entity_name);
	return entity;
}
//bool CEntityFactory::add_entity(CMediaEndEntity* entity)
//{
//	EntityList::iterator it = std::find(m_create_entities.begin(), m_create_entities.end(), entity);
//	if (it != m_create_entities.end())
//	{
//		return true; //如果已经添加了，就不在添加
//	}
//	
//	m_create_entities.push_back(entity);
//	return true;
//}

void CEntityFactory::release(CMediaEndEntity* entity)
{
	if (get_pool())
	{
		switch (entity->type())
		{
		case MEDIA_END_ENTITY_TYPE_MODEL:
			get_pool()->unregisterModel((CModelEntity*)entity);
			break;
		case MEDIA_END_ENTITY_TYPE_SKELETON_MODEL:
			get_pool()->unregisterSkeletonModel((CSkeletonModelEntity*)entity);
			break;
		case MEDIA_END_ENTITY_TYPE_AVATAR:
			get_pool()->unregisterAvatar((AvatarEntity*)entity);
			break;
		case MEDIA_END_ENTITY_TYPE_AVATAR_SKELETON_MODEL:
			get_pool()->unregisterAvatarSkeletonModel((CAvatarSkeletonModelEntity*)entity);
			break;
		case MEDIA_END_ENTITY_TYPE_EFFECT:
			get_pool()->unregisterEffect((CEffectEntity*)entity);
			break;
		default:
			break;
		}
	}

	if (entity->type() < MEDIA_END_ENTITY_TYPE_END)
	{
		delete entity;
		return;
	}

	EntityCreatorTypeMapItr itr = m_type_extend_entitys.find(entity->type());
	if (itr != m_type_extend_entitys.end())
	{
		itr->second->release_entity(entity);
	}
	else 
	{
		//调用顺序错误，该entity的creator在entity释放之前被销毁了
		assert(false);
	}
		
}

void CEntityFactory::release_engine_entity(H3DI::IMoveObject* engine_entity)
{
	engine_entity->Release();
}

//bool CEntityFactory::is_exist(CMediaEndEntity* entity)
//{
//	std::vector<CMediaEndEntity*>::iterator it = std::find(m_create_entities.begin(), m_create_entities.end(), entity);
//	if (it == m_create_entities.end())
//	{
//		return false;
//	}
//
//	return true;
//}
bool CEntityFactory::register_creator(const char* type, CExtendEntityCreator* creator)
{
	assert(type && creator);

	EntityCreatorMapItr itr = m_extend_entitys.find(type);

	if (itr != m_extend_entitys.end())
		return true;

	m_extend_entitys.insert(std::make_pair(type, creator));
	m_type_extend_entitys.insert(std::make_pair(creator->type(), creator));

	return true;
}

CExtendEntityCreator* CEntityFactory::unregister_creator(const char* type)
{
	assert(type);

	EntityCreatorMapItr itr = m_extend_entitys.find(type);

	if (itr == m_extend_entitys.end())
		return NULL;

	EntityCreatorTypeMapItr typeitr = m_type_extend_entitys.find(itr->second->type());

	CExtendEntityCreator* creator = itr->second;

	m_extend_entitys.erase(itr);	
	m_type_extend_entitys.erase(typeitr);

	return creator;
}

CMediaEndEntity* CEntityFactory::create_extend_entity(const char* type, const char* id)
{
	EntityCreatorMapItr itr = m_extend_entitys.find(type);

	if (itr == m_extend_entitys.end())
		return NULL;

	return itr->second->create_entity(m_res_mng, id);
}

CEntityFactory* get_global_entity_factory(H3DI::IRender* irender)
{
	static CEntityFactory* entity_factory = 0;
	if (!entity_factory && irender)
	{
		entity_factory = new CEntityFactory(get_global_graphics_res_mng(irender));
	}
	return entity_factory;
}

I_Entity_Pool* g_global_pool = 0;
void register_pool(I_Entity_Pool* pool)
{
	g_global_pool = pool;
}

I_Entity_Pool* get_pool()
{
	return g_global_pool;
}