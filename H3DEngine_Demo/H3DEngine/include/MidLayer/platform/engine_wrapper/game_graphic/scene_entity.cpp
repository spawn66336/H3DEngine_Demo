#include "game_graphic_body.h"

CSceneEntity::CSceneEntity(CGraphicsResMng* res_mgn, H3DI::IScene* scene):
m_render_camera(NULL)
{
	//m_level_name = strLevelName;
	m_scene_lod = LL_ZERO;
	m_res_mng = res_mgn;
	m_scene = scene;
}

CSceneEntity::~CSceneEntity()
{
	m_scene = NULL;
}
void CSceneEntity::set_ambient_color(const H3DColor& ambient_color)
{
	m_ambient_color = ambient_color;
	m_scene->SetAmbientColor(ambient_color.getColor());
}

const H3DColor& CSceneEntity::get_ambient_color() const
{
	return m_ambient_color;
}

void CSceneEntity::set_ambient_actor_color(const H3DColor& ambient_actor_color)
{
	m_ambient_actor_color = ambient_actor_color;
	m_scene->SetActorAmbientColor(m_ambient_actor_color.getColor());
}

const H3DColor& CSceneEntity::get_ambient_actor_color() const
{
	return m_ambient_actor_color;
}

void CSceneEntity::set_shadow_scope(const float& shadow_scope)
{
	m_shadow_scope = shadow_scope;
	m_scene->SetShadowFadeoutEnd(m_shadow_scope);
}

CCameraEntity* CSceneEntity::get_render_camera() const
{ 
	return m_render_camera; 
}

CCameraEntity* CSceneEntity::set_render_camera(CCameraEntity* camera) 
{ 
	CCameraEntity* old_camera = m_render_camera;
	m_render_camera = camera;
	return old_camera;
}

void CSceneEntity::add_postprocess(const char* filename)
{
	m_scene->AddPostProcess(filename);
}

void CSceneEntity::del_postprocess(const char* filename)
{
	m_scene->DelPostProcess(filename);
}

void CSceneEntity::set_postprocess_param(const char *name,void *pvalue,int sizeOfValue)
{
	m_scene->SetPostProcessParam(name, pvalue, sizeOfValue);
}

void CSceneEntity::begin_render()
{
	//渲染器准备
	H3DI::IRender* iRenderer = m_res_mng->get_render_system()->get_IRender();

	iRenderer->PushProjectionMatrix();
	//iRenderer->PushCamera();
	iRenderer->PushViewMatrix();

	if (NULL != m_render_camera)
	{
		H3DVec3 position = m_render_camera->position();
		H3DVec3 direction = m_render_camera->direction();
		H3DVec3 up = m_render_camera->up_direction();
		float fov, ratio, fNear, fFar;
		m_render_camera->get_frustum(&fov, &ratio, &fNear, &fFar);

		/*渲染相机是跟随场景的设置的，之所以需要设置渲染器的相机，是因为，线不是场景中的物体*/
		//设置渲染器的渲染相机
		iRenderer->SetFrustum(fov, ratio, fNear, fFar);
		iRenderer->LookAt(position, position + direction * 1000, up);
		iRenderer->UpdateCamera();

		//设置关卡的渲染相机
		m_scene->SetFrustum(fov, ratio, fNear, fFar);
		m_scene->LookAt(position, position + direction * 1000, up);
		m_scene->UpdateCamera();
	}
}

void CSceneEntity::render(CRenderSystem& renderer)
{
	H3DI::IRender* iRenderer = m_res_mng->get_render_system()->get_IRender();
	iRenderer->PushScene(m_scene);
	iRenderer->Render();
}

void CSceneEntity::end_render()
{
	H3DI::IRender* iRenderer = m_res_mng->get_render_system()->get_IRender();
	iRenderer->PopViewMatrix();
	//iRenderer->PopCamera();
	iRenderer->PopProjectionMatrix();
}

void CSceneEntity::switch_to_lod(int lod, bool is_sync/* = true*/)
{
	if (lod == LL_ZERO)
	{
		//m_res_mng->get_render_system()->get_IRender()->EnablePostProcess(true);
	}
	else if (lod == LL_ONE)
	{
		//m_res_mng->get_render_system()->get_IRender()->EnablePostProcess(true);
	}
	else if (lod == LL_TWO)
	{
		//m_res_mng->get_render_system()->get_IRender()->EnablePostProcess(false);
	}

	Entity3DListItr itr = m_entitylist.begin();
	while(itr != m_entitylist.end())
	{
		(*itr)->switch_to_lod(lod, is_sync);
		++itr;
	}

	m_scene_lod = lod;
}

void CSceneEntity::add_to_level(CMediaEndEntity* entity)
{
	assert(entity);

	entity->add_to_level(this);
	
	m_entitylist.push_back(entity);
}

void CSceneEntity::remove_from_level(CMediaEndEntity* entity)
{
	assert(entity);

	entity->remove_from_level(this);

	m_entitylist.remove(entity);
}

void CSceneEntity::add_static_object(Actor* static_objcet)
{
	if (static_objcet)
	{
		Actor::EntityList entitylist;
		static_objcet->contain_entity_recursive(entitylist);
		Actor::EntityList::iterator itr = entitylist.begin();
		while (itr != entitylist.end())
		{
			/*C3DEntity* entity3d = dynamic_cast<C3DEntity*>(*itr);
			add_to_level(entity3d);*/
			add_to_level(*itr);

			//m_static_object_entity_list.push_back(entity3d);
			m_static_object_entity_list.push_back(*itr);
			++itr;
		}
	}
}

void CSceneEntity::remove_static_object(Actor* static_object)
{
	if (static_object)
	{
		Actor::EntityList entitylist;
		static_object->contain_entity_recursive(entitylist);
		Actor::EntityList::iterator itr = entitylist.begin();
		while (itr != entitylist.end())
		{
			/*C3DEntity* entity3d = dynamic_cast<C3DEntity*>(*itr);
			if (!entity3d)
				assert(false);
			remove_from_level(entity3d);*/
			remove_from_level(*itr);

			//m_static_object_entity_list.remove(entity3d);
			m_static_object_entity_list.remove(*itr);
			++itr;
		}
	}
}
