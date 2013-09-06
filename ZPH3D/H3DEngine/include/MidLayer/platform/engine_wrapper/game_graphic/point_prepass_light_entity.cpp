/**
* @file point_prepass_light_entity.cpp
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

CPointPrePassLightEntity::CPointPrePassLightEntity(CGraphicsResMng* res_mng, const std::string& name, H3DI::IPrePassLight* light)
:CPrePassLightEntity(res_mng, name, light)
{
	NULL;
}

CPointPrePassLightEntity::~CPointPrePassLightEntity()
{
	NULL;
}

bool CPointPrePassLightEntity::is_kind_of(int obj_type)
{
	if (MEDIA_END_ENTITY_TYPE_POINT_PREPASS_LIGHT == obj_type)
	{
		return true;
	}

	return CPrePassLightEntity::is_kind_of(obj_type);
}

int	CPointPrePassLightEntity::type(void) const
{
	return MEDIA_END_ENTITY_TYPE_POINT_PREPASS_LIGHT;
}

bool CPointPrePassLightEntity::change_resource(const std::string& res_name)
{
	std::string input_res_name = res_name;
	convert_path_string(input_res_name);

	if(stricmp(resource_name().c_str(),input_res_name.c_str()) == 0)
	{
		return true;
	}

	H3DI::IPrePassLight* light =get_graphics_res_mng()->get_render_system()->get_IRender()->CreatePrePassLight(H3DI::LIGHT_POINT);
	if (NULL == light)
	{
		return false;
	}

	if (change_media_object(light, true))
	{
		set_resource_name(input_res_name);
		return true;
	}

	return false;
}

bool CPointPrePassLightEntity::change_media_object(H3DI::IMoveObject* object, bool use_preproperty)
{
	H3DI::IPrePassLight* light = dynamic_cast<H3DI::IPrePassLight*>(object);
	if (NULL == light)
	{
		return false;
	}

	if (use_preproperty)
	{
		light->SetRange(near_range(), far_range());
	}

	return CPrePassLightEntity::change_media_object(object, use_preproperty);
}

void CPointPrePassLightEntity::draw_light_location_line(CRenderSystem& render_sys)
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

	float line_length = min(far_range() / 3, 8.0);
	H3DVec3 end_pos = pos - dir / dir.Length() * line_length;

	H3DColor color(1.0f, 1.0f, 1.0f, 1.0f);

	render_sys.draw_point(pos, 0.5, color);
}
