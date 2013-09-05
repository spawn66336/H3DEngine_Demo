/**
* @file wimble_prepass_light_entity.cpp
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

CWimblePrePassLightEntity::CWimblePrePassLightEntity(CGraphicsResMng* res_mng, const std::string& name, H3DI::IPrePassLight* light)
:CPrePassLightEntity(res_mng, name, light)
{
	m_is_beam_created = false;
	m_beam_location_matrix.Identity();
	m_is_beam_visible = false;
	m_beam_brightness = 1.0f;
	m_is_engine_beam_visible = false;
	m_beam_inner_radius = 0.0f;
	m_beam_inner_angle = 0.0f;
	m_beam_out_angle = 0.0f;

	create_beam(H3DI::E_H3D_BEAM_VOL);
	m_lod0_beam_type = H3DI::E_H3D_BEAM_VOL;

	set_beam_visible(false);
}

CWimblePrePassLightEntity::~CWimblePrePassLightEntity()
{
}

bool CWimblePrePassLightEntity::is_kind_of(int obj_type)
{
	if (MEDIA_END_ENTITY_TYPE_WIMBLE_PREPASS_LIGHT == obj_type)
	{
		return true;
	}

	return CPrePassLightEntity::is_kind_of(obj_type);
}

int	CWimblePrePassLightEntity::type(void)  const
{
	return MEDIA_END_ENTITY_TYPE_WIMBLE_PREPASS_LIGHT;
}

float CWimblePrePassLightEntity::inner_angle() const
{
	return m_light_cached_property.m_inner_angle;

	/*
	float inner = 0.0f;
	float outter = 0.0f;
	
	get_IPrePassLight()->GetAngle(&inner, &outter);

	return inner;
	*/
}

void CWimblePrePassLightEntity::set_inner_angle(float light_inner_angle)
{
	m_light_cached_property.m_inner_angle = light_inner_angle;
	get_IPrePassLight()->SetAngle(m_light_cached_property.m_inner_angle, m_light_cached_property.m_outter_angle);
	//Signal<WIMBLE_PREPASS_LIGHT_INNER_ANGLE_CHANGED>(this, light_inner_angle);
}

float CWimblePrePassLightEntity::outter_angle() const
{
	return m_light_cached_property.m_outter_angle;

	/*
	float inner = 0.0f;
	float outter = 0.0f;

	get_IPrePassLight()->GetAngle(&inner, &outter);

	return outter;
	*/
}

void CWimblePrePassLightEntity::set_outter_angle(float light_outter_angle)
{
	m_light_cached_property.m_outter_angle = light_outter_angle;
	get_IPrePassLight()->SetAngle(m_light_cached_property.m_inner_angle, m_light_cached_property.m_outter_angle);
	//Signal<WIMBLE_PREPASS_LIGHT_OUTTER_ANGLE_CHANGED>(this, light_outter_angle);
}

bool CWimblePrePassLightEntity::change_resource(const std::string& res_name)
{
	_asm
	{
		int 3;
	}

	return true;

	std::string input_res_name = res_name;
	convert_path_string(input_res_name);

	if(stricmp(resource_name().c_str(),input_res_name.c_str()) == 0)
	{
		return true;
	}

	H3DI::IPrePassLight* light =get_graphics_res_mng()->get_render_system()->get_IRender()->CreatePrePassLight(H3DI::LIGHT_PROJECT);
	if (NULL == light)
	{
		return false;
	}

	if (change_media_object(light, true))
	{
		set_resource_name(input_res_name);
		return true;
	}

	return change_media_object(light, true);
}

bool CWimblePrePassLightEntity::change_media_object(H3DI::IMoveObject* object, bool use_preproperty)
{
	_asm
	{
		int 3;
	}

	return true;

	H3DI::IPrePassLight* light = dynamic_cast<H3DI::IPrePassLight*>(object);
	if (NULL == light)
	{
		return false;
	}

	if (use_preproperty)
	{
		light->SetAngle(inner_angle(), outter_angle());
	}

	if (m_project_texture_name.length() > 0)
	{
		light->SetProjTextureName(m_project_texture_name.c_str());
	}

	return CPrePassLightEntity::change_media_object(object, use_preproperty);
}

void CWimblePrePassLightEntity::set_projected_texture(const std::string& texturename)
{
	if (stricmp(texturename.c_str(), m_project_texture_name.c_str()) == 0)
		return;

	m_project_texture_name = texturename;
	get_IPrePassLight()->SetProjTextureName(texturename.c_str());
}

void	CWimblePrePassLightEntity::enable_projected_texture(bool enable)
{
	/*在lod2模式下，锥光灯的SetProjTextureEnable()属性始终为false;在其他LOD级别下，随物体属性变化
	因此需要对其设置进行记录，以便在切换至LOD1和LOD0时设置。
	*/
	if (LL_TWO == get_lod())
	{
	}
	else if (LL_ZERO == get_lod() || LL_ONE == get_lod())
	{
		get_IPrePassLight()->SetProjTextureEnable(enable);
	}
	else
	{
		assert(false);
	}
	
	m_is_project_texture = enable;

	// 只有在投射disable期间改变过相应的投射纹理时，才需要设置，这里有一点多
	// 如果开始先enable后设置投射纹理，会造成设置两次投射纹理的情况
	if(enable)
	{
		get_IPrePassLight()->SetProjTextureName(m_project_texture_name.c_str());
	}
}

void CWimblePrePassLightEntity::update(float interval)
{
	CPrePassLightEntity::update(interval);
}

void CWimblePrePassLightEntity::create_beam(H3DI::EH3DBEAMTYPE beam_type)
{
	if (!is_beam_created())
	{
		m_is_beam_created = true;
		get_IPrePassLight()->CreateLightBeam(beam_type);
		if (LL_TWO == get_lod())
		{
			get_IPrePassLight()->SetBeamVisable(false);
		}
		else if (LL_ONE == get_lod() || LL_ZERO == get_lod())
		{
			get_IPrePassLight()->SetBeamVisable(m_is_engine_beam_visible);
		}
		else
		{
			assert(false);
		}
		set_beam_visible(m_is_engine_beam_visible);
		set_beam_local_matrix(m_beam_location_matrix);
		set_beam_brightness(m_beam_brightness);

		set_beam_near_range(5);
		set_beam_far_range(1);
	}
	else
	{
		set_beam_type(beam_type);
	}
}

void CWimblePrePassLightEntity::set_beam_type(H3DI::EH3DBEAMTYPE beam_type)
{
	H3DColor lightColor = color();
	float lightIntensity = intensity();
	float nearRange = near_range();
	float farRange = far_range();
	H3DI::LightAffectParam afp =  get_affect_param();
	int priority = get_priority();
	
	float innerAngle = inner_angle();
	float outterAngle = outter_angle();

	float beamNearRange = get_beam_near_range();
	float beamFarRange = get_beam_far_range();
	//float beamInnerAngle = get_beam_inner_angle();
	//float beamOutterAngle = get_beam_outter_angle();

	H3DVec3 dir;
	get_IPrePassLight()->GetDirection(dir);

	
	if (LL_TWO == get_lod())
	{
		get_IPrePassLight()->SetLightBeamType(H3DI::E_H3D_BEAM_BOARD);
	}
	else if (LL_ONE == get_lod())
	{
		get_IPrePassLight()->SetLightBeamType(H3DI::E_H3D_BEAM_BOARD);
		get_IPrePassLight()->SetBeamVisable(m_is_engine_beam_visible);
	}
	else if (LL_ZERO == get_lod())
	{
		get_IPrePassLight()->SetLightBeamType(beam_type);
		get_IPrePassLight()->SetBeamVisable(m_is_engine_beam_visible);
		m_lod0_beam_type = beam_type;
	}
	else 
	{
		assert(false);
	}
	
	set_beam_visible(m_is_beam_visible);
	set_beam_brightness(m_beam_brightness);
	set_beam_inner_radius(m_beam_inner_radius);
	set_beam_local_matrix(m_beam_location_matrix);
	
	set_color(lightColor);
	set_intensity(lightIntensity);
	set_near_range(nearRange);
	set_far_range(farRange);
	set_affect_param(afp);
	set_priority(priority);

	get_IPrePassLight()->SetAngle(innerAngle, outterAngle);
	get_IPrePassLight()->SetBeamRange(beamNearRange, beamFarRange);
	//get_IPrePassLight()->SetBeamAngle(beamInnerAngle, beamOutterAngle);
	get_IPrePassLight()->SetBeamAngle(m_beam_inner_angle, m_beam_out_angle);

	set_location_matrix(get_location_matrix());
	get_IPrePassLight()->SetDirection(dir);
}

void CWimblePrePassLightEntity::delete_beam()
{
	if (!is_beam_created())
	{
		get_IPrePassLight()->ReleaseLightBeam();
		m_is_beam_created = false;
	}
}

bool CWimblePrePassLightEntity::is_beam_created()
{
	return m_is_beam_created;
}

const H3DMat4& CWimblePrePassLightEntity::get_beam_local_matrix() const
{
	return m_beam_location_matrix;
}

void CWimblePrePassLightEntity::set_beam_local_matrix(const H3DMat4& mat)
{
	if (is_beam_created())
	{
		get_IPrePassLight()->SetBeamLocalMat(mat);
		m_beam_location_matrix = mat;
	}
}

void CWimblePrePassLightEntity::set_beam_visible(bool beam_visible)
{
	m_is_beam_visible = beam_visible;
	update_engine_beam_visible();
}

bool CWimblePrePassLightEntity::is_beam_visible() const
{
	return m_is_beam_visible;
}

void CWimblePrePassLightEntity::set_beam_brightness(float bright_ness)
{
	if (is_beam_created())
	{
		get_IPrePassLight()->SetBeamBrightness(bright_ness);
		m_beam_brightness = bright_ness;
	}
}

float CWimblePrePassLightEntity::get_beam_brightness() const
{
	return m_beam_brightness;
}

void CWimblePrePassLightEntity::set_beam_near_range(float range)
{
	if (is_beam_created())
	{
		get_IPrePassLight()->SetBeamRange(range, get_beam_far_range());
	}
}

float CWimblePrePassLightEntity::get_beam_near_range()
{
	float fnear = 0;
	float ffar = 0;
	if (is_beam_created())
	{
		get_IPrePassLight()->GetBeamRange(&fnear, &ffar);
	}

	return fnear;
}

void CWimblePrePassLightEntity::set_beam_far_range(float range)
{
	if (is_beam_created())
	{
		get_IPrePassLight()->SetBeamRange(get_beam_near_range(), range);
	}
}

float CWimblePrePassLightEntity::get_beam_far_range()
{
	float fnear = 0;
	float ffar = 0;
	if (is_beam_created())
	{
		get_IPrePassLight()->GetBeamRange(&fnear, &ffar);
	}

	return ffar;
}

void CWimblePrePassLightEntity::set_beam_inner_angle(float inner)
{
	if (is_beam_created())
	{
		get_IPrePassLight()->SetBeamAngle(inner, m_beam_out_angle);
		m_beam_inner_angle = inner;
	}
}

float CWimblePrePassLightEntity::get_beam_inner_angle()
{
	float inner = 0;
	float outter = 0;

	if (is_beam_created())
	{
		get_IPrePassLight()->GetBeamAngle(&inner, &outter);
	}

	return inner;
}

void CWimblePrePassLightEntity::set_beam_outter_angle(float outter)
{
	if (is_beam_created())
	{
		get_IPrePassLight()->SetBeamAngle(m_beam_inner_angle, outter);
		m_beam_out_angle = outter;
	}
}

float CWimblePrePassLightEntity::get_beam_outter_angle()
{
	float inner = 0;
	float outter = 0;

	if (is_beam_created())
	{
		get_IPrePassLight()->GetBeamAngle(&inner, &outter);
	}

	return outter;
}

void CWimblePrePassLightEntity::update_engine_beam_visible()
{
	//bool engine_beam_visible = is_visible() && is_enable() && is_light_enable() && is_light_turn_on() && is_beam_visible();
	bool engine_beam_visible = m_is_entity_light_enable && m_is_external_visible && is_beam_visible();
	enable_engine_beam(engine_beam_visible);
}

bool CWimblePrePassLightEntity::is_engine_beam_visible()
{
	return m_is_engine_beam_visible;
}

void CWimblePrePassLightEntity::enable_engine_beam(bool enable_beam)
{
	//bool engine_beam_visible = is_visible() && is_enable() && is_light_enable() && is_light_turn_on() && is_beam_visible() && enable_beam;
	bool engine_beam_visible = m_is_entity_light_enable && m_is_external_visible && is_beam_visible();
	if ((engine_beam_visible != m_is_engine_beam_visible) && is_beam_created())
	{
		m_is_engine_beam_visible = engine_beam_visible;
		if (LL_TWO == get_lod())
		{
		}
		else if (LL_ZERO == get_lod() || LL_ONE == get_lod())
		{
			get_IPrePassLight()->SetBeamVisable(engine_beam_visible);
		}
		else
		{
			assert(false);
		}
		
	}
}

void CWimblePrePassLightEntity::enable_engine_light(bool enable_light)
{
	CPrePassLightEntity::enable_engine_light(enable_light);
	enable_engine_beam(enable_light);
}

void CWimblePrePassLightEntity::switch_to_lod(int lod, bool is_sync/* = true*/)
{
	CPrePassLightEntity::switch_to_lod(lod, is_sync);

	if (LL_ZERO == lod)
	{
		set_beam_type(m_lod0_beam_type);
		get_IPrePassLight()->SetBeamVisable(m_is_engine_beam_visible);
		get_IPrePassLight()->SetProjTextureEnable(m_is_project_texture);
	}
	else if (LL_ONE == lod)
	{
		set_beam_type(H3DI::E_H3D_BEAM_BOARD);
		get_IPrePassLight()->SetBeamVisable(m_is_engine_beam_visible);
		get_IPrePassLight()->SetProjTextureEnable(m_is_project_texture);

		if (false == get_record_fake_light_spot())
			get_IPrePassLight()->SetProjTextureEnable(false);
	}
	else if (LL_TWO == lod)
	{
		get_IPrePassLight()->SetBeamVisable(false);
		get_IPrePassLight()->SetProjTextureEnable(false);
	}

	m_lod_level = lod;
}

//NOTICE : 这个可以不用
void CWimblePrePassLightEntity::update_engine_light_enable()
{
	CPrePassLightEntity::update_engine_light_enable();
	//update_engine_beam_visible();
}

void CWimblePrePassLightEntity::set_beam_inner_radius(float t)
{
	if (is_beam_created())
	{
		get_IPrePassLight()->SetBeamInnerRadius(t);
		m_beam_inner_radius = t;
	}
}

void CWimblePrePassLightEntity::set_beam_blur_coeff(float t)
{
	if (is_beam_created())
	{
		get_IPrePassLight()->SetBeamBlurCoeff(t);
	}
}

void CWimblePrePassLightEntity::draw_light_location_line(CRenderSystem& render_sys)
{
	H3DVec3 pos,scale;
	H3DQuat quat;
	H3DMat4 mat;
	mat = get_world_location_matrix();
	mat.Decompose(scale,quat,pos);

	int LineNum = 20;
	//画外径
	float lightRange = far_range();
// 	H3DVec3 dir;
// 	get_IPrePassLight()->GetDirection(dir);
	H3DVec3 dir = quat * H3DVec3(0, -1, 0);

	float line_length = min(far_range() / 3, 8.0f);
	H3DVec3 end_pos = pos - dir / dir.Length() * line_length;

	H3DColor color(1.0f, 1.0f, 1.0f, 1.0f);

	render_sys.draw_light_start_point(pos, end_pos, color);
	render_sys.draw_link_line(pos, end_pos, color);
	render_sys.draw_light_end_point(pos, end_pos, color);
}
