/**
* @file model_entity.cpp
* 这个文件的主要功能是：实现了引擎层的IModel类的封装类CModelEntity的定义
* 
* @author 程陈(Chengchen@h3d.com.cn)
* 
* 更改日志
* ----------------------------------------------------
* ver 1.0.0
*       by 程陈2010.1.29
*/

#include "game_graphic_body.h"
#include "operation.h"

CModelEntity::CModelEntity(CGraphicsResMng* res_mng, const std::string& name, const std::string& resource_name, int lod)
: C3DEntity(res_mng, name, resource_name, lod)
, m_model(0)
, m_fake_shadow_size(1.f)
, m_is_cast_shadow(false)
, m_is_renderin_reflection(true)
, m_is_not_write_color(false)
, m_is_ralativetoactor(false)
, m_is_hide_update(false)
, m_set_transparence(false)
, m_is_recive_fakelightspot(false)
, m_recive_fakelightspot_record(false)
{
	m_is_model_high_light = false;
	m_is_lightmap_load = false;
	m_is_lightmap_applied = false;
	NULL;
}

CModelEntity::~CModelEntity()
{
	LightMapsItr itr = m_lightmaps.begin();
	while(itr != m_lightmaps.end())
	{
		itr->second.m_pTex->DelRef();
		++itr;
	}
	m_lightmaps.clear();
	m_effect_set.clear();
}

void CModelEntity::set_fake_shadow_size(float size)
{
	assert(size > 0);

	m_fake_shadow_size = size;
	if (m_model)
	{
		m_model->SetFakeShadowSize(m_fake_shadow_size);
	}
}

float CModelEntity::get_fake_shadow_size() const
{
	return m_fake_shadow_size;
}

bool CModelEntity::is_kind_of(int obj_type)
{
	if(MEDIA_END_ENTITY_TYPE_MODEL == obj_type)
	{
		return true;
	}

	return C3DEntity::is_kind_of(obj_type);
}

int	CModelEntity::type(void) const
{
	return MEDIA_END_ENTITY_TYPE_MODEL;
}

H3DI::IModel* CModelEntity::get_IModel() const
{
	return m_model;
}

bool CModelEntity::change_resource(const std::string& res_name)
{
	std::string input_res_name = res_name;
	convert_path_string(input_res_name);

	if(stricmp(resource_name().c_str(),input_res_name.c_str()) == 0)
	{
		return true;
	}

	H3DI::IModel* model = m_res_mng->get_render_system()->get_IRender()->CreateModel(input_res_name.c_str());
	
	if (NULL == model)
	{
		return false;
	}
	if (change_media_object(model, true))
	{
		set_resource_name(input_res_name);
		update_flags();
		return true;
	}

	return false;
}

bool CModelEntity::change_media_object(H3DI::IMoveObject* object, bool use_preproperty)
{
	H3DI::IModel* model = dynamic_cast<H3DI::IModel*>(object);
	if (NULL == model)
	{
		return false;
	}

	if (use_preproperty)
	{
		model->SetFakeShadowSize(m_fake_shadow_size);
	}

	//更改资源
	m_model = model;
	m_model->SetMaterialLod((int)get_lod());

	return C3DEntity::change_media_object(object, use_preproperty);
}

H3DI::INewShader* CModelEntity::get_new_shader(const std::string& shader_name)
{
	if (m_model)
	{
		H3DI::INewShader* shader = NULL;
		for (int i = 0; i < m_model->GetNumSubModel(); i++)
		{
			shader = m_model->GetNewShaderByIndex(i);
			if (stricmp(shader->GetMaterialName(), shader_name.c_str()) == 0)
			{
				return shader;
			}
		}
	}

	return NULL;
}

bool CModelEntity::set_shader_node_value(H3DI::INewShader* shader, const std::string& shader_node_name, int value)
{
	if(shader->SetNodeVarValue_Int(shader_node_name.c_str(), value))
	{
		return true;
	}

	return false;
}

bool CModelEntity::set_shader_node_value(H3DI::INewShader* shader, const std::string& shader_node_name, float value)
{
	if(shader->SetNodeVarValue_Float(shader_node_name.c_str(), value))
	{
		return true;
	}

	return false;
}

bool CModelEntity::set_shader_node_value(H3DI::INewShader* shader, const std::string& shader_node_name, float value1, float value2, float value3, float value4)
{
	if(shader->SetNodeVarValue_Float4(shader_node_name.c_str(), value1, value2, value3, value4))
	{
		return true;
	}

	return false;
}

bool CModelEntity::set_shader_node_value(H3DI::INewShader* shader, const std::string& shader_node_name, const std::string& value)
{
	if(shader->SetNodeVarValue_Str(shader_node_name.c_str(), value.c_str()))
	{
		return true;
	}

	return false;
}

bool CModelEntity::set_shader_node_value(H3DI::INewShader* shader, const std::string& shader_node_name, const H3DMat4& value)
{
	float mat[] = {value[0][0], value[0][1], value[0][2], value[0][3], value[1][0], value[1][1], value[1][2], value[1][3], value[2][0], value[2][1], value[2][2], value[2][3], value[3][0], value[3][1], value[3][2], value[3][3]};

	if(shader->SetNodeVarValue_Matrix44(shader_node_name.c_str(), mat))
	{
		return true;
	}

	return false;
}

bool CModelEntity::get_shader_node_value(H3DI::INewShader* shader, const std::string& shader_node_name, int& value)
{
	if (shader->GetNodeVarValue_Int(shader_node_name.c_str(), value))
	{
		return true;
	}
	return false;
}

bool CModelEntity::get_shader_node_value(H3DI::INewShader* shader, const std::string& shader_node_name, float& value)
{
	if (shader->GetNodeVarValue_Float(shader_node_name.c_str(), value))
	{
		return true;
	}
	return false;
}

bool CModelEntity::get_shader_node_value(H3DI::INewShader* shader, const std::string& shader_node_name, float& value1, float& value2, float& value3, float& value4)
{
	if (shader->GetNodeVarValue_Float4(shader_node_name.c_str(), value1, value2, value3, value4))
	{
		return true;
	}
	return false;
}

bool CModelEntity::get_shader_node_value(H3DI::INewShader* shader, const std::string& shader_node_name, std::string& value)
{
	const char* res = NULL;
	if (shader->GetNodeVarValue_Str(shader_node_name.c_str(), res))
	{
		value = res;
		return true;
	}
	return false;
}

bool CModelEntity::get_shader_node_value(H3DI::INewShader* shader, const std::string& shader_node_name, H3DMat4& value)
{
	float res[17] = {0};
	if (shader->GetNodeVarValue_Matrix44(shader_node_name.c_str(), res))
	{
		value = H3DMat4(res[0], res[1],res[2],res[3],res[4],res[5],res[6],res[7],res[8],res[9],res[10],res[11],res[12],res[13],res[14],res[15]);
		return true;
	}
	return false;
}

void CModelEntity::high_light_model(bool high_light)
{
	if (!is_visible())
	{
		return;
	}

	if (is_model_high_light() == high_light)
	{
		return;
	}

	m_is_model_high_light = high_light;
	set_entity_flags(H3DI::I_ENTITY_HIGHTLIGHT, m_is_model_high_light);
}

bool CModelEntity::is_model_high_light()
{
	return m_is_model_high_light;
}

void CModelEntity::set_waveform_var(H3DI::INewShader* shader, H3DI::WaveFormParam var_type, float value)
{
	shader->SetWaveformVar(var_type, value);
}

void CModelEntity::set_waveform_var(H3DI::INewShader* shader, H3DI::WaveFormParam var_type, float f0, float f1)
{
	shader->SetWaveformVar(var_type, f0, f1);
}

void CModelEntity::render(CRenderSystem& render_system)
{
	/*if (!is_visible())
	{
		return ;
	}

	H3DI::IRender *irender = render_system.get_IRender();
	if (is_model_high_light())
	{
		irender->AddHighLightModel(get_IModel());
	}

	get_IModel()->SetFlags(get_IModel()->GetFlags()&(~H3DI::I_ENTITY_HIDE));*/
}

void CModelEntity::reload()
{
	get_IModel()->ReloadAll();
}
bool CModelEntity::pushHardwareSelect(CRenderSystem& render_system, bool sel_line)
{
	if (!m_model)
	{
		return false;
	}

	if (sel_line)
	{
		return false;
	}

	bool need_push = is_visible();
	if ((get_actor() != NULL) && (!is_enable()))
	{
		need_push = need_push || (get_actor()->get_visible()); ///针对特效做了特殊的判断
	}
	if (need_push)
	{
		unsigned int id = get_entity_id();
		render_system.get_IRender()->PushHardwareSelectObject(m_model, id);
		return true;
	}

	return false;
}

void CModelEntity::add_lightmapinfo(unsigned int nMeshID, ModelLightMap& lightmapinfo)
{
	if(m_lightmapinfos.find(nMeshID) != m_lightmapinfos.end())
		return;
	m_lightmapinfos.insert(std::make_pair(nMeshID, lightmapinfo));
}

void CModelEntity::switch_to_lod(int lod, bool is_sync)
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

void CModelEntity::load_lightmap()
{
	if (m_lightmapinfos.size() <= 0)
		return;
	if (m_is_lightmap_load)
		return;

	LightMapInfosItr itr = m_lightmapinfos.begin();
	while(itr != m_lightmapinfos.end())
	{
		H3DI::ITexture* iTex = m_res_mng->get_render_system()->get_IRender()->CreateTextureFromFile(itr->second.m_MapPath.c_str());
		iTex->AddRef();
		H3DI::ILightMapInfo iLightmap;
		iLightmap.m_pTex = iTex;
		iLightmap.m_CoordBias[0] = itr->second.m_CoordBias[0];
		iLightmap.m_CoordBias[1] = itr->second.m_CoordBias[1];
		iLightmap.m_CoordScale[0] = itr->second.m_CoordScale[0];
		iLightmap.m_CoordScale[1] = itr->second.m_CoordScale[1];
		iLightmap.m_clrScale[0] = itr->second.m_clrScale[0];
		iLightmap.m_clrScale[1] = itr->second.m_clrScale[1];
		iLightmap.m_clrScale[2] = itr->second.m_clrScale[2];

		m_lightmaps.insert(std::make_pair(itr->first, iLightmap));

		//get_IModel()->SetLightMapInfo(itr->first, iLightmap);

		itr++;
	}
	m_is_lightmap_load = true;
}

void CModelEntity::apply_lightmap()
{
	if (m_lightmaps.size() <=0)
		return;
	if (!m_is_lightmap_load)
		return;
	if (m_is_lightmap_applied)
		return;

	LightMapsItr itr = m_lightmaps.begin();
	while(itr != m_lightmaps.end())
	{
		if (m_model)
		{
			m_model->SetLightMapInfo(itr->first, itr->second);
		}
		++itr;
	}

	m_is_lightmap_applied = true;
}

void CModelEntity::cancle_lightmap()
{
	if (m_lightmaps.size() <= 0)
		return;
	if (!m_is_lightmap_applied)
		return;
	LightMapsItr itr = m_lightmaps.begin();
	while(itr != m_lightmaps.end())
	{
		H3DI::ILightMapInfo iLightmap;
		iLightmap.m_pTex = NULL;
		iLightmap.m_CoordBias[0] = itr->second.m_CoordBias[0];
		iLightmap.m_CoordBias[1] = itr->second.m_CoordBias[1];
		iLightmap.m_CoordScale[0] = itr->second.m_CoordScale[0];
		iLightmap.m_CoordScale[1] = itr->second.m_CoordScale[1];
		iLightmap.m_clrScale[0] = itr->second.m_clrScale[0];
		iLightmap.m_clrScale[1] = itr->second.m_clrScale[1];
		iLightmap.m_clrScale[2] = itr->second.m_clrScale[2];

		if (m_model)
		{
			m_model->SetLightMapInfo(itr->first, iLightmap);
		}
		++itr;
	}

	m_is_lightmap_applied = false;
}

void CModelEntity::set_recive_fakelightspot(bool brecive)
{
	if (brecive == m_is_recive_fakelightspot)
		return;

	m_is_recive_fakelightspot = brecive;
	set_entity_flags(H3DI::I_ENTITY_RECEIVE_FAKELIGHTSPOT, brecive);
}

void CModelEntity::record_recive_fakelightspot(bool brecive)
{
	if (LL_ZERO != get_lod())
		set_recive_fakelightspot(brecive);

	m_recive_fakelightspot_record = brecive;
}
int CModelEntity::set_mesh_shader_by_name(const char* mesh_name,H3DI::INewShader* p)
{
	if (m_model)
	{
		m_model->SetMeshNewShaderByName(mesh_name, p);
	}
	return 0;
}

int CModelEntity::set_mesh_shader_by_idx(int mesh_idx, H3DI::INewShader* p)
{
	if (m_model)
	{
		return m_model->SetMeshNewShaderByIndex(mesh_idx, p);
	}
	return 0;
}

void CModelEntity::set_effect(CEffectEntity* entity)
{
	if (!entity)
	{
		return;
	}

	if (entity->m_parent_model)
	{
		entity->m_parent_model->unset_effect(entity);
	}
	entity->m_parent_model = this;
	m_effect_set.insert(entity);

	if (m_entity_state!=ES_NONE && m_entity_state!=ES_UNLOAD)
	{
		set_effect_immediate(entity);
	}
	else if (m_model && entity->get_is_create_sync() && entity->get_IEffect())
	{
		middleAssert(false && "Unload状态不能设置同步创建的特效");
	}
}

void CModelEntity::unset_effect(CEffectEntity* entity)
{
	middleAlwaysAssert(entity->m_parent_model==this && "特效绑定与解绑的对象不一致");

	auto it = m_effect_set.find(entity);
	if (it!=m_effect_set.end())
	{
		m_effect_set.erase(it);
	}
	entity->m_parent_model = 0;

	if (entity->get_IEffect())
	{
		entity->get_IEffect()->unsetActor();
	}

	stop_entity_op(entity);
}

void CModelEntity::getModelAppearance(ModelAppearance& appearance)
{
	appearance.geo_lod = m_lod_level;
	appearance.mat_lod = m_lod_level;
	strcpy(appearance.res_name, m_resource_name.c_str());
}


void CModelEntity::setShadow(bool show)
{
	m_is_cast_shadow = show;
	set_entity_flags(H3DI::I_ENTITY_CAST_SHADOW, m_is_cast_shadow);
}

void CModelEntity::set_renderin_reflection(bool is)
{
	m_is_renderin_reflection = is;
	set_entity_flags(H3DI::I_ENTITY_RENDERIN_REFLECTION, m_is_renderin_reflection);
}

void CModelEntity::set_not_write_color(bool is)
{
	m_is_not_write_color = is;
	set_entity_flags(H3DI::I_ENTITY_NOT_WRITE_COLOR, m_is_not_write_color);
}

void CModelEntity::set_ralativetoactor(bool is)
{
	m_is_ralativetoactor = is;
	set_entity_flags(H3DI::I_ENTITY_RALATIVETOACTOR, m_is_ralativetoactor);
}

void CModelEntity::set_transparence(float trans)
{
	m_set_transparence = true;
	m_transparence = trans;
	if (m_model)
	{
		m_model->SetTransparence(m_transparence);
	}
}

void CModelEntity::set_forcesyncdata()
{
	set_entity_flags(H3DI::I_ENTITY_FORCESYNCDATA, true);
}

void CModelEntity::set_hide_update(bool is)
{
	m_is_hide_update = is;
	set_entity_flags(1<<19, m_is_hide_update);
}

int CModelEntity::rayTestWithBoundingVolumes(const float* rayOrigin, const float * rayDir, H3DI::ModelBoundingVolumeTestInfo *pOut, int nResultBufferCount)
{
	if (m_model)
	{
		return m_model->RayTestWithBoundingVolumes(rayOrigin, rayDir, pOut, nResultBufferCount);
	}
	else
	{
		return 0;
	}
}

int	  CModelEntity::queryAttachPoint(H3DI::IAttachModelInfo& rayResInfo,bool bTransToLocal)
{
	if (m_model)
	{
		return m_model->QueryAttachPoint(rayResInfo, bTransToLocal);
	}
	else
	{
		return 0;
	}
}

void CModelEntity::set_level_layer(int level_layer)
{
	assert(level_layer>=H3DI::SL_DetailObj && level_layer<=H3DI::SL_Actors);
	m_level_layer = level_layer;
}

void CModelEntity::UnloadResources()
{
	if (m_entity_state==ES_NONE)
	{
		return;
	}

	UnloadEffect();
	C3DEntity::UnloadResources();
}

void CModelEntity::ReloadResources(bool is_sync, int priority)
{
	if (m_model && m_entity_state!=ES_NONE)
	{
		return;
	}

	C3DEntity::ReloadResources(is_sync, priority);
	ReloadEffect(is_sync, priority);
	if (!is_sync)
	{
		update_appearance();
	}
}

void CModelEntity::create_engine_object_impl(bool is_sync)
{
	if (is_sync)
	{
		H3DI::sCreateOp co;
		co.geo_lod = m_lod_level;
		co.mat_lod = m_lod_level;
		set_engine_object_impl(m_res_mng->get_render_system()->get_IRender()->CreateModel(co, m_resource_name.c_str()));
		after_create_impl();
	}
	else
	{
		ModelOperation* pOp = new ModelOperation(m_create_priority, m_res_mng->get_render_system()->get_IRender(), m_resource_name, m_lod_level);
		m_execute_op_list[m_create_priority].push_back(pOp);
	}
}

void CModelEntity::set_engine_object_impl(H3DI::IMoveObject* object)
{
	C3DEntity::set_engine_object_impl(object);
	m_model = (H3DI::IModel*)object;
}

void CModelEntity::after_create_impl()
{
	if (m_model)
	{
		C3DEntity::after_create_impl();

		update_flags();
		m_model->SetFakeShadowSize(m_fake_shadow_size);
		if (m_set_transparence)
		{
			m_model->SetTransparence(m_transparence);
		}
	}
}

void CModelEntity::set_effect_immediate(CEffectEntity* entity)
{
	//特效若为同步创建则立即生效
	if (m_model && entity->get_is_create_sync() && entity->get_IEffect())
	{
		EffectActor ea;
		ea.actor = m_model;
		ea.obj = 0;
		entity->get_IEffect()->setActor(&ea, 1);
	}
	else
	{
		SetEffectOperation* pOp = new SetEffectOperation(m_is_view_sync, this, entity);
		m_cache_op_list.push_back(pOp);

		m_entity_op_map[entity] = pOp;
	}
}

void CModelEntity::switch_to_lod_immediate(int lod, bool is_sync)
{
	if (is_sync)
	{
		finish_all_op();
		m_model->SetMaterialLod(lod);
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

void CModelEntity::UnloadEffect()
{
	for (auto it=m_effect_set.begin(); it!=m_effect_set.end(); ++it)
	{
		(*it)->UnloadResources();
	}
}

void CModelEntity::ReloadEffect(bool is_sync, int priority)
{
	for (auto it=m_effect_set.begin(); it!=m_effect_set.end(); ++it)
	{
		CEffectEntity* entity = *it;
		entity->ReloadResources(is_sync, priority);
		entity->m_is_create_sync = is_sync;
		set_effect_immediate(entity);
	}
}

void CModelEntity::update_flags()
{
	setShadow(m_is_cast_shadow);
	high_light_model(m_is_model_high_light);
	set_recive_fakelightspot(m_is_recive_fakelightspot);
	set_renderin_reflection(m_is_renderin_reflection);
	set_not_write_color(m_is_not_write_color);
	set_ralativetoactor(m_is_ralativetoactor);
	set_hide_update(m_is_hide_update);
	update_engine_entity_visible();
}