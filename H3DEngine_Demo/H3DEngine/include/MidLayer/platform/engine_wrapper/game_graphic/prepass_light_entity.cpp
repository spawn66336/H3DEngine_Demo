/**
* @file prepass_light_entity.cpp
* 这个文件的主要功能是：
* 
* @author 程陈(Chengchen@h3d.com.cn)
* 
* 更改日志
* ----------------------------------------------------
* ver 1.0.0
*       by 程陈2010.5.21
*/

#include "game_graphic_body.h"

CPrePassLightEntity::CPrePassLightEntity(CGraphicsResMng* res_mng, const std::string& name, H3DI::IPrePassLight* light)
:C3DEntity(res_mng, name, light)
{
	m_level_layer = H3DI::SL_Lights;

	m_light = light;
	m_draw_beam_line = false;
	m_is_light_enable = true;

	m_light_visible_status = true;
	m_is_light_turn_on = true;
	m_is_engine_light_enable = true; 

 	float light_color[4] = {0};
 	m_light->GetColor(light_color);
	m_light_cached_property.m_light_color = light_color;

	m_light->GetShadowColor(light_color);
	m_light_cached_property.m_shadow_color = light_color;

	m_light->GetLightShaftColor(light_color);
	m_shaft_color = light_color;

	m_light_cached_property.m_intensity = m_light->GetIntensity();
	m_shaft_intensity = m_light->GetLightShaftIntensity();
	m_light_cached_property.m_enable_shadow = m_light->GetShadowEnable();

	m_light->GetRange(&m_light_cached_property.m_near_range, &m_light_cached_property.m_far_range);

	m_light_cached_property.m_light_affect_param = m_light->GetLightAffectParam();

	m_light_cached_property.m_priority = m_light->GetPriority();

	m_light->GetAngle(&m_light_cached_property.m_inner_angle, &m_light_cached_property.m_outter_angle);

	m_shaft_threshold = m_light->GetLightShaftThreshold();
	m_shaft_depth = m_light->GetLightShaftDepth();

	m_is_entity_light_enable = m_is_engine_light_enable;
	m_is_external_visible = true;
	m_is_fake_light_enabled = false;

	m_lightmap_shadow_index = 0.0f;
	
	m_record_fake_light_spot = true;
}

CPrePassLightEntity::~CPrePassLightEntity()
{
	NULL;
}

bool CPrePassLightEntity::is_kind_of(int obj_type)
{
	if (MEDIA_END_ENTITY_TYPE_PREPASS_LIGHT == obj_type)
	{
		return true;
	}

	return C3DEntity::is_kind_of(obj_type);
}

int	CPrePassLightEntity::type(void) const
{
	return MEDIA_END_ENTITY_TYPE_PREPASS_LIGHT;
}

H3DColor CPrePassLightEntity::color() const
{
	return m_light_cached_property.m_light_color;
// 	float light_color[4] = {0};
// 	m_light->GetColor(light_color);
// 	return light_color;
}

void CPrePassLightEntity::set_color(const H3DColor& light_color)
{
	m_light_cached_property.m_light_color = light_color;
	if (m_is_engine_light_enable)
	{
		m_light->SetColor(light_color.getColor());
	}
	//Signal<PREPASS_LIGHT_COLOR_CHANGED>(this, light_color);
}

float CPrePassLightEntity::intensity() const
{
	return m_light_cached_property.m_intensity;
//	return m_light->GetIntensity();
}

void CPrePassLightEntity::set_intensity(float light_intensity)
{
	m_light_cached_property.m_intensity = light_intensity;
	m_light->SetIntensity(light_intensity);
	//Signal<PREPASS_LIGHT_INTENSITY_CHANGED>(this, light_intensity);
}

bool CPrePassLightEntity::is_shadow_enable() const
{
	return m_light_cached_property.m_enable_shadow;
//	return m_light->GetShadowEnable();
}

void CPrePassLightEntity::set_shadow_enable(bool enable_shadow)
{
	m_light_cached_property.m_enable_shadow = enable_shadow;
	m_light->SetShadowEnable(enable_shadow);
	//Signal<PREPASS_LIGHT_SHADOW_ENBALE_CHANGED>(this, enable_shadow);	
}

H3DColor CPrePassLightEntity::shadow_color() const
{
	return m_light_cached_property.m_shadow_color;
// 	float light_color[4] = {0};
// 	m_light->GetShadowColor(light_color);
// 
// 	return light_color;
}

void CPrePassLightEntity::set_shadow_color(const H3DColor& light_shadow_color)
{
	m_light_cached_property.m_shadow_color = light_shadow_color;
	m_light->SetShadowColor(light_shadow_color.getColor());
	//Signal<PREPASS_LIGHT_COLOR_CHANGED>(this, light_shadow_color);
}

H3DI::IPrePassLight* CPrePassLightEntity::get_IPrePassLight() const
{
	return m_light;
}

bool CPrePassLightEntity::change_media_object(H3DI::IMoveObject* object, bool use_preproperty)
{
	//这个函数理论上不能调用
	_asm int 3
	H3DI::IPrePassLight* light = dynamic_cast<H3DI::IPrePassLight*>(object);
	if (NULL == light)
	{
		return false;
	}

	if (use_preproperty)
	{
		light->SetColor(color().getColor());
		light->SetIntensity(intensity());
		light->SetShadowEnable(is_shadow_enable());
		light->SetShadowColor(shadow_color().getColor());
	}

	m_light = light;

	return C3DEntity::change_media_object(object, use_preproperty);
}

float CPrePassLightEntity::near_range() const
{
	return m_light_cached_property.m_near_range;
// 	float inner = 0.0f;
// 	float outter = 0.0f;
// 
// 	get_IPrePassLight()->GetRange(&inner, &outter);
// 
// 	return inner; ;
}

void CPrePassLightEntity::set_near_range(float light_inner_radius)
{
	m_light_cached_property.m_near_range = light_inner_radius;
	get_IPrePassLight()->SetRange(light_inner_radius, m_light_cached_property.m_far_range);
	//Signal<POINT_PREPASS_LIGHT_INNER_RADIUS_CHANGED>(this, light_inner_radius);
}

float CPrePassLightEntity::far_range() const
{
	return m_light_cached_property.m_far_range;
 //	float inner = 0.0f;
 //	float outter = 0.0f;
 //
 //	get_IPrePassLight()->GetRange(&inner, &outter);

	//return outter;
}

void CPrePassLightEntity::set_far_range(float light_outter_radius)
{
	m_light_cached_property.m_far_range = light_outter_radius;
	get_IPrePassLight()->SetRange(m_light_cached_property.m_near_range, light_outter_radius);
	//Signal<POINT_PREPASS_LIGHT_OUTTER_RADIUS_CHANGED>(this, light_outter_radius);
}
void CPrePassLightEntity::set_beam_line_draw( bool is_draw )
{
	m_draw_beam_line = is_draw;
}

bool CPrePassLightEntity::get_beam_line_draw()
{
	return m_draw_beam_line;
}

void CPrePassLightEntity::set_glare_property(int index, const GlareProperty& glare_property)
{
	int glare_index = get_glare_index(index);
	if (glare_index > 0)
	{
		H3DI::tGlareProperty property = (H3DI::tGlareProperty)(*(const_cast<GlareProperty*>(&glare_property)));
		H3DI::tGlareProperty pre_property = (H3DI::tGlareProperty)(*(const_cast<GlareProperty*>(&(m_created_glares[index].second))));

		if (((property.m_pTextureName != 0) && (pre_property.m_pTextureName != 0))
			&& (stricmp(pre_property.m_pTextureName, property.m_pTextureName) == 0)
			)
		{
			property.m_pTextureName = 0;
		}
		if (((property.m_pShaderLib != 0) && (pre_property.m_pShaderLib != 0))
			&& (stricmp(pre_property.m_pShaderLib, property.m_pShaderLib) == 0)
			)		
		{
			property.m_pShaderLib = 0;
		}
		if (((property.m_pShaderInstanceName != 0) && (pre_property.m_pShaderInstanceName != 0))
			&& (stricmp(pre_property.m_pShaderInstanceName, property.m_pShaderInstanceName) == 0)
			)
		{
			property.m_pShaderInstanceName = 0;
		}

		m_created_glares[index].second = property;
		set_engine_glare_property(glare_index, property);
	}
}

GlareProperty CPrePassLightEntity::get_glare_property(int index)
{
	if (is_glare_created(index))
	{
		return m_created_glares[index].second;
	}

	return GlareProperty();
}

int CPrePassLightEntity::create_glare(const GlareProperty& glare_property)
{
	H3DI::tGlareProperty property = (H3DI::tGlareProperty)(*(const_cast<GlareProperty*>(&glare_property)));
	property.m_bVisable = property.m_bVisable && m_is_engine_light_enable;

	int index = get_IPrePassLight()->CreateGlare(property);
	if (-1 != index)
	{
		H3DI::tGlareProperty tmp_property;
		get_IPrePassLight()->GetGlareProperty(index, tmp_property);

		m_created_glares.push_back(std::make_pair<int, GlareProperty>(index, GlareProperty(tmp_property)));
	}

	return -1;
}

//void CPrePassLightEntity::delete_galare(int index)
//{
//	int glare_index = get_glare_index(index);
//	if (glare_index > 0)
//	{
//		get_IPrePassLight()->ReleaseGlare(glare_index);
//		GlareList::iterator it = m_created_glares.begin();
//		
//		m_created_glares.remove(glare_index);
//	}
//}

bool CPrePassLightEntity::is_glare_created(int glare_index)
{
	/*
	GlareList::iterator it = std::find(m_created_glares.begin(), m_created_glares.end(), glare_index);

	if (it != m_created_glares.end())
	{
		return true;
	}
	*/
	if ((glare_index >= m_created_glares.size())
		|| (glare_index < 0)
		)
	{
		return false;
	}
	else
	{
		return true;
	}

	return false;
}

int CPrePassLightEntity::get_glare_index(int index)
{
	if (is_glare_created(index))
	{
		return m_created_glares[index].first;
	}

	return -1;
}

bool CPrePassLightEntity::is_light_enable() const
{
	return m_is_light_enable;
}

void CPrePassLightEntity::set_entity_light_enable(bool light_enable)
{
	m_is_engine_light_enable = light_enable;
}

bool CPrePassLightEntity::is_entity_light_enable() const
{
	return m_is_entity_light_enable;
}

void CPrePassLightEntity::set_light_visible_status(bool visible_status)
{
	m_light_visible_status = visible_status;
}

bool CPrePassLightEntity::get_light_visible_status() const
{
	return m_light_visible_status;
}

int CPrePassLightEntity::get_priority() const
{
	return m_light_cached_property.m_priority;
//	return get_IPrePassLight()->GetPriority();
}

void CPrePassLightEntity::set_priority(int light_priority)
{
	m_light_cached_property.m_priority = light_priority;
	get_IPrePassLight()->SetPriority(light_priority);
}

bool CPrePassLightEntity::is_light_shaft_enable()
{
	return m_enable_shaft;
//	return get_IPrePassLight()->GetLightShaftEnable();
}

void CPrePassLightEntity::set_light_shaft_enable(bool enable)
{
	m_enable_shaft = enable;
	get_IPrePassLight()->SetLightShaftEnable(enable);
}

H3DColor CPrePassLightEntity::get_light_shaft_color() const
{
	return m_shaft_color;
// 	float shaft_color[4] = {0};
// 	get_IPrePassLight()->GetLightShaftColor(shaft_color);
// 	return H3DColor(shaft_color);
}

void CPrePassLightEntity::set_light_shaft_color(const H3DColor& shaft_color)
{
	m_shaft_color = shaft_color;
	get_IPrePassLight()->SetLightShaftColor(shaft_color.getColor());
}

float CPrePassLightEntity::get_light_shaft_intensity()
{
	return m_shaft_intensity;
//	return get_IPrePassLight()->GetLightShaftIntensity();
}

void CPrePassLightEntity::set_light_shaft_intensity(float shaft_intensity)
{
	m_shaft_intensity = shaft_intensity;
	get_IPrePassLight()->SetLightShaftIntensity(shaft_intensity);
}

float CPrePassLightEntity::get_light_shaft_threshold()
{
	return m_shaft_threshold;
	//	return get_IPrePassLight()->GetLightShaftThreshold();
}

void CPrePassLightEntity::set_light_shaft_threshold(float threshold)
{
	m_shaft_threshold = threshold;
	get_IPrePassLight()->SetLightShaftThreshold(threshold);
}

float CPrePassLightEntity::get_light_shaft_depth()
{
	return m_shaft_depth;
	//	return get_IPrePassLight()->GetLightShaftDepth();
}

void CPrePassLightEntity::set_light_shaft_depth(float depth)
{
	get_IPrePassLight()->SetLightShaftDepth(depth);
}

void CPrePassLightEntity::set_enable(bool enable)
{
	C3DEntity::set_enable(enable);
	update_engine_light_enable();
}

void CPrePassLightEntity::set_visible(bool visible)
{
	//C3DEntity::set_visible(visible);
	if (is_visible() == visible)
	{
		return;
	}

	m_is_visible = visible;
	update_engine_light_enable();
}


void CPrePassLightEntity::set_external_visible(bool visible)
{
	m_is_external_visible = visible;
	update_engine_light_enable();
}

bool CPrePassLightEntity::is_external_visible() const
{
	return m_is_external_visible;
}

void CPrePassLightEntity::enable_engine_light(bool enable_light)
{
	if (enable_light != m_is_engine_light_enable)
	{
		m_is_engine_light_enable = enable_light;

		//m_light->SetLightEnable(is_on);
 		if (enable_light)
 		{
 			m_light->SetColor(m_light_cached_property.m_light_color.getColor());
 		}
 		else
		{
			m_light->SetColor(H3DColor(0, 0, 0, 0).getColor());
		}
		for (int i = 0; i < m_created_glares.size(); ++i)
		{
			GlareProperty property = m_created_glares[i].second;

			H3DI::tGlareProperty glare_property = 
				(H3DI::tGlareProperty)(*(const_cast<GlareProperty*>(&property)));
			glare_property.m_bVisable = property.is_visible && enable_light;
			glare_property.m_pTextureName = 0;
			glare_property.m_pShaderLib = 0;
			glare_property.m_pShaderInstanceName = 0;
			set_engine_glare_property(m_created_glares[i].first, glare_property);
		}
	}
}

void CPrePassLightEntity::switch_to_lod(int lod, bool is_sync/* = true*/)
{

	if (lod == LL_ZERO)
	{
		get_IPrePassLight()->SetLightEnable(true);
		enable_fake_light_spot(false);
	}
	else if (lod == LL_ONE)
	{
		get_IPrePassLight()->SetLightEnable(false);

		enable_fake_light_spot(get_record_fake_light_spot());
	}
	else if (lod == LL_TWO)
	{
		get_IPrePassLight()->SetLightEnable(false);
		enable_fake_light_spot(false);
	}

	m_lod_level = lod;
}

void CPrePassLightEntity::enable_fake_light_spot(bool bEnabled)
{
	/*if (LL_ZERO == get_lod())
	{
		assert(!get_IPrePassLight()->GetFakeLightSpot());
	}
	else if (LL_ONE == get_lod() || LL_TWO == get_lod())
	{
		get_IPrePassLight()->EnableFakeLightSpot(bEnabled);
	}*/
	get_IPrePassLight()->EnableFakeLightSpot(bEnabled);
	m_is_fake_light_enabled = bEnabled;
}

void CPrePassLightEntity::record_fake_light_spot(bool bEnable)
{
	if (LL_ONE == get_lod())
		enable_fake_light_spot(bEnable);

	m_record_fake_light_spot = bEnable;
}

void CPrePassLightEntity::update_engine_light_enable()
{
	bool is_on = is_visible() && is_enable() && is_light_enable() && is_light_turn_on();
	m_is_entity_light_enable = is_on;
	enable_engine_light(is_on && m_is_external_visible);
}

bool CPrePassLightEntity::is_engine_light_enable()
{
	return m_is_engine_light_enable && m_is_entity_light_enable && m_is_external_visible;
}

void CPrePassLightEntity::set_light_enable(bool light_enable)
{
	m_is_light_enable = light_enable;
	update_engine_light_enable();
}

void CPrePassLightEntity::set_engine_glare_property(int index, const H3DI::tGlareProperty& glare_property)
{
	H3DI::tGlareProperty* property = const_cast<H3DI::tGlareProperty*>(&glare_property);
	property->m_bVisable = property->m_bVisable && is_visible() && is_enable() && is_light_enable() && is_light_turn_on() ;

	get_IPrePassLight()->SetGlareProperty(index, glare_property);
}

void CPrePassLightEntity::turn_on_light(bool enable_light)
{
	m_is_light_turn_on = enable_light;
	update_engine_light_enable();
}

bool CPrePassLightEntity::is_light_turn_on()
{
	return m_is_light_turn_on;
}

void CPrePassLightEntity::set_affect_param(H3DI::LightAffectParam t)
{
	m_light_cached_property.m_light_affect_param = t;
	m_light->SetLightAffectParam(t);
}

H3DI::LightAffectParam CPrePassLightEntity::get_affect_param()
{
	return (H3DI::LightAffectParam)m_light_cached_property.m_light_affect_param;
//	return m_light->GetLightAffectParam();
}

bool CPrePassLightEntity::pushHardwareSelect(CRenderSystem& render_system, bool sel_line)
{
	if (!sel_line)
	{
		return false;
	}

	//开始灯光绘制
	if (is_visible())
	{
		unsigned int id = get_entity_id();
		render_system.get_IRender()->PushSelectID(id);
		draw_light_location_line(render_system);
		return true;
	}

	return false;
}

void CPrePassLightEntity::draw_light_select_line(CRenderSystem& render_system)
{
	if (is_enable() && is_visible())
	{
		draw_light_location_line(render_system);
	}
}

void CPrePassLightEntity::set_world_location_matrix(const H3DMat4& location)
{
	C3DEntity::set_world_location_matrix(location);
	memcpy(m_light_cached_property.m_location, location.ToFloatPtr(), sizeof(m_light_cached_property.m_location));
}
