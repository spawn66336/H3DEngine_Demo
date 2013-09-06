/**
* @file effect_entity.cpp
* 这个文件的主要功能是：
* 
* @author 程陈(Chengchen@h3d.com.cn)
* 
* 更改日志
* ----------------------------------------------------
* ver 1.0.0
*       by 程陈2010.1.29
*/

#include "game_graphic_body.h"
#include "../middle_layer/i_middle_layer.h"
#include "operation.h"

CEffectEntity::CEffectEntity(CGraphicsResMng* res_mng, const std::string& name, const std::string& resource_name, int lod)
: C3DEntity(res_mng, name, resource_name, lod)
, m_cameraAngleAdapt(false)
, m_effect(0)
, m_state(0)
, m_is_actor_effect(false)
, m_parent_model(0)
{
	m_level_layer = H3DI::SL_SpecialEffect;

	m_transparence = 1.0f;
	m_cast_shadow_enable = false;
	//cache_effect(); 目前阶段先不进行cache lishan 110119

	m_posOffsetX = 0;
	m_posOffsetY = 0;
	m_posOffsetZ = 0;
}

CEffectEntity::~CEffectEntity()
{
	if (m_parent_model)
	{
		m_parent_model->unset_effect(this);
	}

	m_move_object = 0;

	std::map<std::string, ISpecialEffect*>::iterator it = m_effect_pool.begin();

	for (; it != m_effect_pool.end(); ++it)
	{
		if (it->second)
		{
			it->second->release();
		}
	}
	m_effect_pool.clear();
}

void CEffectEntity::cache_effect()
{
}

std::string replace(const std::string& src, char old_ch, char new_ch)
{
	std::string path = src;
	for (int i = 0; i < path.length(); ++i)
	{
		if (old_ch == path[i])
		{
			path[i] = new_ch;
		}
	}

	return path;
}

ISpecialEffect* CEffectEntity::create_effect(const std::string& res_name)
{
	std::map<std::string, ISpecialEffect*>::iterator it = m_effect_pool.find(res_name);
	if (it != m_effect_pool.end())
	{
		return it->second;
	}

	ISpecialEffect* effect = get_graphics_res_mng()->get_render_system()->get_IRender()->CreateSpe(res_name.c_str());

	if (effect == 0)
	{
		return 0;
	}
	effect->Initialize();

	m_effect_pool[res_name] = effect;

	return effect;
}

void CEffectEntity::play()
{
	if (!(m_state&EFFECT_ACTIVATED))
	{
		m_state |= EFFECT_ACTIVATED;
		if (m_effect)
		{
			m_effect->reset();
			m_effect->setState(m_state);
		}
	}

	C3DEntity::play();
}

void CEffectEntity::stop()
{
	m_state &= ~EFFECT_ACTIVATED;
	if (m_effect)
	{
		m_effect->setState(m_state);
	}

	C3DEntity::stop();
}

void CEffectEntity::reset(void)
{
	if (m_effect)
	{
		m_effect->reset();
	}

	C3DEntity::reset();
}

void CEffectEntity::set_visible(bool visible)
{
	bool bvisible = visible && is_enable();

	if (is_visible() == bvisible)
	{
		return;
	}

	m_is_visible = visible;
	
	update_engine_entity_visible();
	/*if (bvisible)
	{
		m_state &= ~EFFECT_HIDE;
	}
	else
	{		
		m_state |= EFFECT_HIDE;
	}
	if (m_effect)
	{
		m_effect->setState(m_state);
	}*/
}
void CEffectEntity::set_external_visible(bool visible)
{
	m_is_external_visible = visible;
	update_engine_entity_visible();
}

void CEffectEntity::update_engine_entity_visible()
{
	bool bVisible = is_visible() && is_external_visible();
	if (bVisible)
	{
		m_state &= ~EFFECT_HIDE;
	}
	else
	{		
		m_state |= EFFECT_HIDE;
	}
	if (m_effect)
	{
		m_effect->setState(m_state);
	}
}


bool CEffectEntity::is_kind_of(int obj_type)
{
	if (MEDIA_END_ENTITY_TYPE_EFFECT == obj_type)
	{
		return true;
	}

	return C3DEntity::is_kind_of(obj_type);
}

int CEffectEntity::type() const
{
	return MEDIA_END_ENTITY_TYPE_EFFECT;
}

ISpecialEffect* CEffectEntity::get_IEffect() const
{ 
	return m_effect;
}

void CEffectEntity::set_transparence(float new_transparence)
{
	assert(new_transparence >= 0.0f);
	assert(new_transparence <= 1.0f);

	m_transparence = new_transparence;
	if (m_effect)
	{
		m_effect->setTransparence(new_transparence);
	}
}

float CEffectEntity::transparence()
{
	return m_transparence;
}

bool CEffectEntity::change_resource(const std::string& res_name)
{
	std::string input_res_name = res_name;
	convert_path_string(input_res_name);

	if (stricmp(input_res_name.c_str(), m_resource_name.c_str()) == 0)
	{
		return true;
	}

	ISpecialEffect* effect = create_effect(input_res_name);
	if (!effect)
	{
		return false;
	}

	if (change_media_object(effect, true))
	{
		set_resource_name(input_res_name);
		return true;
	}
	return false;
}

bool CEffectEntity::change_media_object(H3DI::IMoveObject* object, bool use_preproperty)
{
	ISpecialEffect* effect_object = dynamic_cast<ISpecialEffect*>(object);
	if (NULL == effect_object)
	{
		return false;
	}

	if (use_preproperty)
	{
		float trans = transparence();
		effect_object->reset();
		effect_object->setState(m_state);
		effect_object->setTransparence(trans);

		effect_object->SetLocationMatrix(get_world_location_matrix());
	}

	//将粒子从现有场景中删除
	if (is_added_to_level())
		m_level_entity->get_IScene()->DetachModel(m_effect);

	m_effect = effect_object;
	m_effect->SetMaterialLod((int)get_lod());

	//将新粒子添加进现有场景
	if (is_added_to_level())
	{
		m_level_entity->get_IScene()->AttachModel(m_effect, m_level_layer);
		m_level_entity->get_IScene()->MoveModel(m_effect);
	}

	m_move_object = m_effect;

	return true;
}

void CEffectEntity::update_media_object(float interval)
{
	if (m_effect)
	{
		m_effect->update(interval/1000.0f, true);
	}
}

void CEffectEntity::enable_cast_shadow(bool cast_shadow)
{
	m_cast_shadow_enable = cast_shadow;
	if (m_effect)
	{
		m_effect->setCastShadow(cast_shadow);
	}
}

bool CEffectEntity::is_cast_shadow_enable() const
{
	return m_cast_shadow_enable;
}

void CEffectEntity::getPositionOffset(float& xoff,float& yoff,float& zoff)
{
	xoff = m_posOffsetX;
	yoff = m_posOffsetY;
	zoff = m_posOffsetZ;
}

void CEffectEntity::setPositionOffset(float xoff,float yoff,float zoff)
{
	m_posOffsetX = xoff;
	m_posOffsetY = yoff;
	m_posOffsetZ = zoff;

	H3DMat4 mat4;
	mat4.Identity();
	mat4[0][3] = m_posOffsetX;
	mat4[1][3] = m_posOffsetY;
	mat4[2][3] = m_posOffsetZ;
	set_location_matrix(mat4);
};
/**
@brief 调整特效角度，用于某些特效朝向相机的设置
*/
void CEffectEntity::setCameraAngle(CameraInfo& camInfo,CSkeletonModelEntity* bindTarget)
{
	if(!m_cameraAngleAdapt)
		return;

	H3DVec3 effpos;

	if (bindTarget->getISkeletonModel())
	{
		H3DI::ISkeleton* ts = bindTarget->getISkeletonModel()->GetSkeleton();
		if (ts)
		{
			H3DI::IBone* pBone = ts->GetRootBone();
			assert(pBone);
			const H3DMat4& mat4 = bindTarget->getISkeletonModel()->GetRendingMatrix(pBone->GetID());

			// 计算相机的右方向
			H3DVec3 camRight;
			camRight.Cross(H3DVec3(camInfo.up_x, camInfo.up_y, camInfo.up_z), H3DVec3(camInfo.dir_x, camInfo.dir_y, camInfo.dir_z));
			camRight.Normalize();
			effpos = H3DVec3(mat4[0][3],mat4[1][3],m_posOffsetZ ) + camRight * (- m_posOffsetX );
		}
	}

	// 必须是特效的世界空间坐标位置position

	// 面片中心指向相机的向量
	H3DVec3 effToCam(camInfo.pos_x - effpos.x, camInfo.pos_y - effpos.y, camInfo.pos_z-effpos.z);
	effToCam.Normalize();
	// 相机的向上方向
	H3DVec3 up(camInfo.up_x, camInfo.up_y, camInfo.up_z);
	up.Normalize();
	// 计算出面片的右方向
	H3DVec3 right;
	right.Cross(up, effToCam);
	right.Normalize();
	up.Cross(effToCam, right);
	up.Normalize();

	H3DMat4 mat(
		right.x, up.x, effToCam.x, effpos.x,
		right.y, up.y, effToCam.y, effpos.y,
		right.z, up.z, effToCam.z, effpos.z,
		0, 0, 0, 1
		);

	set_location_matrix(mat);

	if (m_effect)
	{
		m_effect->update(0);
	}
};

void CEffectEntity::switch_to_lod(int lod, bool is_sync)
{
	//LOD相同直接return
	if (m_lod_level == lod)
	{
		return;
	}

	C3DEntity::switch_to_lod(lod);

	if (m_entity_state!=ES_NONE && m_entity_state!=ES_UNLOAD)
	{
		switch_to_lod_immediate(lod, is_sync);
	}
}

void CEffectEntity::set_is_actor_effect(bool is)
{
	m_is_actor_effect = is;
	if (m_effect)
	{
		m_effect->SetIsActorEffect(m_is_actor_effect);
	}
}

float  CEffectEntity::get_animation_length()
{
	if (m_effect)
	{
		return m_effect->getAnimationLength();
	}
	return 0.f;
}

void CEffectEntity::getEffectAppearance(EffectAppearance& appearance)
{
	appearance.geo_lod = m_lod_level;
	appearance.mat_lod = m_lod_level;
	strcpy(appearance.res_name, m_resource_name.c_str());
}

void CEffectEntity::create_engine_object_impl(bool is_sync)
{
	if (is_sync)
	{
		H3DI::sCreateOp co;
		co.geo_lod = m_lod_level;
		co.mat_lod = m_lod_level;
		set_engine_object_impl(m_res_mng->get_render_system()->get_IRender()->CreateSpe(m_resource_name.c_str()));
		after_create_impl();
	}
	else
	{
		EffectOperation* pOp = new EffectOperation(m_create_priority, m_res_mng->get_render_system()->get_IRender(), m_resource_name, m_lod_level);
		m_execute_op_list[m_create_priority].push_back(pOp);
	}
}

void CEffectEntity::set_engine_object_impl(H3DI::IMoveObject* object)
{
	C3DEntity::set_engine_object_impl(object);
	m_effect = (ISpecialEffect*)object;
	m_effect_pool[m_resource_name] = m_effect;//加入管理
}

void CEffectEntity::after_create_impl()
{
	if (m_effect)
	{
		C3DEntity::after_create_impl();

		m_effect->setCastShadow(m_cast_shadow_enable);
		m_effect->setTransparence(m_transparence);
		m_effect->setState(m_state);
		m_effect->SetIsActorEffect(m_is_actor_effect);

		m_effect->Initialize();
		m_effect->reset();
		m_effect->update(m_cur_time/1000.f, true);
	}
}

void CEffectEntity::switch_to_lod_immediate(int lod, bool is_sync)
{
	if (is_sync)
	{
		finish_all_op();
		m_effect->SetMaterialLod(lod);
	}
	else
	{
		//若有未处理完的OP，先触发显示，并且材质LOD的OP一上来处于等待状态
		bool is_wait = !m_execute_op_list[0].empty() || !m_finish_op_list[0].empty();
		LodOperation* pOp = new LodOperation(m_is_view_sync, m_res_mng->get_render_system()->get_IRender(), lod, this, is_wait);
		m_execute_op_list[0].push_back(pOp);

		if (!m_is_view_sync)
		{
			update_appearance();
		}
	}
}

void CEffectEntity::UnloadResources()
{
	if (m_entity_state==ES_NONE)
	{
		return;
	}

	clear_all_op();
	m_entity_state = ES_NONE;
	if (m_effect)
	{
		m_effect->unsetActor();
		m_move_object = 0;
		m_effect = 0;

		for (std::map<std::string, ISpecialEffect*>::iterator it = m_effect_pool.begin(); it != m_effect_pool.end(); ++it)
		{
			if (it->second)
			{
				it->second->release();
			}
		}
		m_effect_pool.clear();
	}
}