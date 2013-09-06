/**
* @file system_entity.cpp
* 这个文件的主要功能是：
* 
* @author 程陈(Chengchen@h3d.com.cn)
* 
* 更改日志
* ----------------------------------------------------
* ver 1.0.0
*       by 程陈2010.12.17
*/

#include "game_graphic_body.h"

CLightmapAdjustEntity::CLightmapAdjustEntity(CGraphicsResMng* mng, CSceneEntity* scene, const std::string& entity_name)
	: CMediaEndEntity(mng, entity_name)
{
	m_scene = scene;
	m_lightmap_factor_a = H3DColor(1, 1, 1, 1);
	m_lightmap_factor_b = H3DColor(1, 1, 1, 1);
	m_lightmap_factor_a_intensity = 1.0f;
	m_lightmap_factor_b_intensity = 1.0f;
}



void CLightmapAdjustEntity::apply_lightmap_color_factor()
{
	H3DColor a = m_lightmap_factor_a * m_lightmap_factor_a_intensity;
	H3DColor b = m_lightmap_factor_b * m_lightmap_factor_b_intensity;

	m_scene->get_IScene()->SetLightmapColorFactor(a.getColor(), b.getColor());
}
void CLightmapAdjustEntity::set_lightmap_color_factor_a(const H3DColor& a)
{
	m_lightmap_factor_a = a;
	apply_lightmap_color_factor();
}
void CLightmapAdjustEntity::set_lightmap_color_factor_b(const H3DColor& b)
{
	m_lightmap_factor_b = b;
	apply_lightmap_color_factor();
}

void CLightmapAdjustEntity::set_lightmap_color_factor_a_intensity(float fIntensity) 
{
	m_lightmap_factor_a_intensity = fIntensity;
	apply_lightmap_color_factor();
}

void CLightmapAdjustEntity::set_lightmap_color_factor_b_intensity(float fIntensity) 
{
	m_lightmap_factor_b_intensity = fIntensity;
	apply_lightmap_color_factor();
}

int	CLightmapAdjustEntity::type(void) const
{
	return MEDIA_END_ENTITY_TYPE_LIGHTMAP_ADJUST;
}

bool CLightmapAdjustEntity::is_kind_of(int obj_type)
{
	if (MEDIA_END_ENTITY_TYPE_LIGHTMAP_ADJUST == obj_type)
	{
		return true;
	}

	return CMediaEndEntity::is_kind_of(obj_type);
}
