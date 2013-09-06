/**
* @file direction_prepass_light_entity.cpp
* 这个文件的主要功能是：
* 
* @author 程陈(Chengchen@h3d.com.cn)
* 
* 更改日志
* ----------------------------------------------------
* ver 1.0.0
*       by 程陈2010.6.9
*/

#include "game_graphic_body.h"


CDirectionPrePassLightEntity::CDirectionPrePassLightEntity(CGraphicsResMng* res_mng, const std::string& name, H3DI::IPrePassLight* light)
:CPrePassLightEntity(res_mng, name, light)
{
	m_specular_intensity = -1.0f;
	NULL;
}

CDirectionPrePassLightEntity::~CDirectionPrePassLightEntity()
{
	NULL;
}

bool CDirectionPrePassLightEntity::is_kind_of(int obj_type)
{
	if (MEDIA_END_ENTITY_TYPE_DIRECTION_PREPASS_LIGHT == obj_type)
	{
		return true; 
	}

	return CPrePassLightEntity::is_kind_of(obj_type);	
}

int CDirectionPrePassLightEntity::type(void) const
{
	return MEDIA_END_ENTITY_TYPE_DIRECTION_PREPASS_LIGHT;
	
}

void CDirectionPrePassLightEntity::draw_light_location_line(CRenderSystem& render_sys)
{
	H3DVec3 pos,scale;
	H3DQuat quat;
	H3DMat4 mat;
	mat = get_world_location_matrix();
	mat.Decompose(scale,quat,pos);

	int LineNum = 20;
	//画外径
	//float lightRange = far_range();
// 	H3DVec3 dir;
// 	get_IPrePassLight()->GetDirection(dir);
	H3DVec3 dir = quat * H3DVec3(0, -1, 0);

	//float line_length = min(far_range() / 3, 8.0);
	float line_length = 10;
	H3DVec3 end_pos = pos - dir / dir.Length() * line_length;
	/*H3DVec3 pos1 = pos + H3DVec3(0, 1, 0);
	H3DVec3 pos2 = pos + H3DVec3(0, -1, 0);
	H3DVec3 end_pos1 = pos1 - dir / dir.Length() * line_length;
	H3DVec3 end_pos2 = pos2 - dir / dir.Length() * line_length;*/

	H3DColor color(1.0f, 1.0f, 1.0f, 1.0f);

	/*render_sys.draw_light_start_point(pos, end_pos, color);
	render_sys.draw_link_line(pos, end_pos, color);
	render_sys.draw_light_end_point(pos, end_pos, color);*/

	render_sys.draw_link_line(pos, end_pos, color);
	/*render_sys.draw_link_line(pos1, end_pos1, color);
	render_sys.draw_link_line(pos2, end_pos2, color);*/
}

void CDirectionPrePassLightEntity::switch_to_lod(int lod, bool is_sync/* = true*/)
{
	CPrePassLightEntity::switch_to_lod(lod, is_sync);

	std::string strName = m_actor->get_name();
	std::transform(strName.begin(), strName.end(), strName.begin(), tolower);
	std::string::size_type pos_lod1 = strName.find("_lod1", 0);
	std::string::size_type pos_lod2 = strName.find("_lod2", 0);
	if (lod == LL_ZERO)
	{
		if (pos_lod1 != std::string::npos && pos_lod1 == (strName.length() - strlen("_lod1")))
		{
			get_IPrePassLight()->SetLightEnable(false);
		}
		else if (pos_lod2 != std::string::npos && pos_lod2 == (strName.length() - strlen("_lod2")))
		{
			get_IPrePassLight()->SetLightEnable(false);
		}
	}
	else if (lod == LL_ONE)
	{
		
		if (pos_lod1 != std::string::npos && pos_lod1 == (strName.length() - strlen("_lod1")))
		{
			get_IPrePassLight()->SetLightEnable(true);
		}
		else if (pos_lod2 != std::string::npos && pos_lod2 == (strName.length() - strlen("_lod2")))
		{
			get_IPrePassLight()->SetLightEnable(false);
		}
	}
	else if (lod == LL_TWO)
	{
		
		if (pos_lod1 != std::string::npos && pos_lod1 == (strName.length() - strlen("_lod1")))
		{
			get_IPrePassLight()->SetLightEnable(false);
		}
		else if (pos_lod2 != std::string::npos && pos_lod2 == (strName.length() - strlen("_lod2")))
		{
			get_IPrePassLight()->SetLightEnable(true);
		}
		
	}
}

void CDirectionPrePassLightEntity::set_specular_intensity(float fIntensity)
{
	m_specular_intensity = fIntensity;
	get_IPrePassLight()->SetIntensity(intensity(), m_specular_intensity);
}

void CDirectionPrePassLightEntity::set_intensity(float light_intensity)
{
	m_light_cached_property.m_intensity = light_intensity;
	m_light->SetIntensity(light_intensity, m_specular_intensity);
}
