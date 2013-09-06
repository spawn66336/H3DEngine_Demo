/**
* @file property_modifier.cpp
* 这个文件的主要功能是：
* 
* @author 程陈(Chengchen@h3d.com.cn)
* 
* 更改日志
* ----------------------------------------------------
* ver 1.0.0
*       by 程陈2010.11.17
*/

#include "game_graphic_body.h"

template<>
bool is_two_bezier_point_curv_line(BezierCurvNode<float>* cur, BezierCurvNode<float>* next)
{
	bool is_line = false;
	float a = (next->m_value - cur->m_value) * 1.0 / (next->m_t - cur->m_t);
	float b = cur->m_value - a * cur->m_t;
	if (fabs(cur->m_next_value - (a * cur->m_next_t + b)) > 0.000001)
	{
		return false;
	}

	if (fabs(next->m_pre_value - (a * next->m_pre_t + b)) > 0.000001)
	{
		return false;
	}

	return true;
}

std::string InterpolatingType2Str(int type)
{
	switch(type)
	{
	case INTERPOLATINGTYPE_POINT:
		return "POINT";
		break;
	case INTERPOLATINGTYPE_LINE:
		return "LINE";
		break;
	case INTERPOLATINGTYPE_BEZIER:
		return "BEZIER";
		break;
	case INTERPOLATINGTYPE_UNKNOWN:
		return "UNKNOWN";
		break;
	default:
		break;
	}

	return "";
}

/************************************************************************/
/* 
set_value的偏特化部分
*/
/************************************************************************/
template<>
void set_value<H3DVec3, float>(H3DVec3& value, const float& elem, int index)
{
	if (index < 3)
	{
		value[index] = elem;
	}
	return ;
	switch(index)
	{
	case 0:
		value.x = elem;
		break;
	case 1:
		value.y = elem;
		break;
	case 2:
		value.z = elem;
		break;
	default:
		break;
	}
}

template<>
void set_value<H3DColor, float>(H3DColor& value, const float& elem, int index)
{
	switch(index)
	{
	case 0:
		value.setRed(elem);
		break;
	case 1:
		value.setGreen(elem);
		break;
	case 2:
		value.setBlue(elem);
		break;
	case 3:
		value.setAlpha(elem);
		break;
	default:
		break;
	}
}

//H3DVec4的偏特化
template<>
void set_value<H3DVec4, float>(H3DVec4& value, const float& elem, int index)
{
	if (index < 4)
	{
		value[index] = elem;
	}
}

//H3DMat4的偏特化
template<>
void set_value<H3DMat4, float>(H3DMat4& value, const float& elem, int index)
{
	if (index < 16)
	{
		value[index / 4][index % 4] = elem;
	}
}
//H3DColor的偏特化
template<>
void set_value<H3DMat3, float>(H3DMat3& value, const float& elem, int index){
	if (index < 9)
	{
		value[index / 3][index % 3] = elem;
	}
}

PropertyModifierFactory::PropertyModifierFactory()
{
	init_creator_map();
}

PropertyModifierFactory::~PropertyModifierFactory()
{
	for (ModifierCreatorCollection::iterator it = m_modifier_creator_map.begin();
		it != m_modifier_creator_map.end(); ++it)
	{
		if (it->second->property_type() < MEDIA_PROPERTY_TYPE_COUNT)
		{
			delete it->second;
		}
	}
}

void PropertyModifierFactory::init_creator_map()
{
	add_creator("Visible", new VisiblePropertyModifierCreator());
	add_creator("Location", new LocationPropertyModifierCreator());
	add_creator("Position", new PositionPropertyModifierCreator());	
	add_creator("PathPosition", new PathPositionPropertyModifierCreator());
	add_creator("Rotation", new RotationPropertyModifierCreator());	
	add_creator("Scale", new ScalePropertyModifierCreator());
	add_creator("Resource", new ResourcePropertyModifierCreator());
	add_creator("Frustum", new CameraFrustumPropertyModifierCreator());
	add_creator("Fov", new CameraFovPropertyModifierCreator());
	add_creator("Ratio", new CameraRatioPropertyModifierCreator());
	add_creator("Nearplane", new CameraNearPlanePropertyModifierCreator());
	add_creator("Farplane", new CameraFarPlanePropertyModifierCreator());
	add_creator("Focalplane", new CameraFocalplanePropertyModifierCreator());
	add_creator("DofRange", new CameraDofPropertyModifierCreator());
	add_creator("confusion", new CameraConfusionPropertyModifierCreator());
	add_creator("Lookat", new CameraLookatPropertyModifierCreator());
	add_creator("Up", new CameraUpDirectionPropertyModifierCreator());
	add_creator("Camera_FovAutoChange", new CameraFovAutoChangePropertyModifierCreator());
	add_creator("Camera_Adjust_Type", new CameraAdjustTypePropertyModifierCreator());
	add_creator("Camera_Target_Type", new CameraTargetTypePropertyModifierCreator());
	add_creator("Camera_Targetes", new CameraTargetesPropertyModifierCreator());
	add_creator("Camera_PostProcess", new CameraPostprocessPropertyModifierCreator());
	add_creator("CameraPosition", new CameraPostitionPropertyModifierCreator());
	add_creator("ENABLE_MODEL_SHADOW", new ModelShadelEnablePropertyModifierCreator());
	add_creator("HIGH_LIGHT_MODEL", new ModelHighLightPropertyModifierCreator());
	add_creator("Shader_var", new ModelShaderVarPropertyModifierCreator());
	add_creator("Shader_waveform", new ModelWaveFormPropertyModifierCreator());
	add_creator("Light_Enable", new LightEnablePropertyModifierCreator());
	add_creator("Light_Color", new LightColorPropertyModifierCreator());
	add_creator("Light_intensity", new LightIntensityPropertyModifierCreator());
	add_creator("Enable_Light_Shadow", new EnableLightShadowPropertyModifierCreator());
	add_creator("Light_Shadow_Color", new LightShadowColorPropertyModifierCreator());
	add_creator("Near_Range", new LightNearRangePropertyModifierCreator());
	add_creator("Far_Range", new LightFarRangePropertyModifierCreator());
	add_creator("Light_Affect_Param", new LightAffectParamPropertyModifierCreator());
	add_creator("Light_Glare_Width", new LightGlareWidthPropertyModifierCreator());
	add_creator("Light_Glare_Height", new LightGlareHeightPropertyModifierCreator());
	add_creator("Light_Glare_Color", new LightGlareColorPropertyModifierCreator());
	add_creator("Light_Glare_Texture_Name", new LightGlareTexturePropertyModifierCreator());
	add_creator("Light_Glare_Shader_Lib", new LightGlareShaderLibPropertyModifierCreator());
	add_creator("Light_Glare_Shader_Name", new LightGlareShaderNamePropertyModifierCreator());
	add_creator("Light_Glare_Distance", new LightGlareDistancePropertyModifierCreator());
	add_creator("Light_Glare__Rotation_Velocity", new LightGlareRotationVelocityPropertyModifierCreator());
	add_creator("Light_Glare_Visible", new LightGlareVisiblePropertyModifierCreator());
	add_creator("Light_Glare_Rotate_With_Camera", new LightGlareRotateWithCameraPropertyModifierCreator());
	add_creator("Light_Glare_View_Camera", new LightGlareViewCameraPropertyModifierCreator());
	add_creator("DRAW_LIGHT_BEAM_LINE", new LightBeamLineDrawPropertyModifierCreator());
	add_creator("Set_Light_Priority", new LightPriorityPropertyModifierCreator());
	add_creator("Light_Shaft_Enable", new LightShaftEnablePropertyModifierCreator());
	add_creator("Light_Shaft_Color", new LightShaftColorPropertyModifierCreator());
	add_creator("Light_Shaft_Intensity", new LightShaftIntensityPropertyModifierCreator());
	add_creator("Light_Shaft_Threshold", new LightShaftThresholdPropertyModifierCreator());
	add_creator("Light_Shaft_Depth", new LightShaftDepthPropertyModifierCreator());
	add_creator("Inner_angle", new LightInnerAnglePropertyModifierCreator());
	add_creator("Outter_angle", new LightOutterAnglePropertyModifierCreator());
	add_creator("Enable_proj_texture", new LightProjTextureEnablePropertyModifierCreator());
	add_creator("Proj_texture", new LightProjectTextureNamePropertyModifierCreator());
	add_creator("Light_Beam_Visible", new LightBeamVisiblePropertyModifierCreator());
	add_creator("Light_Beam_Brightness", new LightBeamBrightnessPropertyModifierCreator());
	add_creator("Light_Beam_Near_Range", new LightBeamNearRangePropertyModifierCreator());
	add_creator("Light_Beam_Far_Range", new LightBeamFarRangePropertyModifierCreator());
	//add_creator("Light_Beam_Inner_Angle", new LightBeamInnerAnglePropertyModifierCreator());
	add_creator("Light_Beam_Outter_Angle", new LightBeamOutterAnglePropertyModifierCreator());
	add_creator("Fake_Light_Spot_Enable", new FakeLightPostPropertyModifierCreator());

	add_creator("Light_Beam_Type", new LightBeamTypePropertyModifierCreator());
	add_creator("Effect_transparence", new EffectTransparencePropertyModifierCreator());
	add_creator("Play", new EffectPlayPropertyModifierCreator());
	add_creator("System_Ambient_Color", new SystemAmbientColorPropertyModifierCreator());
	add_creator("system_ambientactor_color", new SystemAmbientActorColorPropertyModifierCreator());
	add_creator("Color_FactorA", new SystemLightMapColorFactorAPropertyModifierCreator());
	add_creator("Color_FactorB", new SystemLightMapColorFactorBPropertyModifierCreator());
	add_creator("Color_FactorA_Intensity", new SystemLightMapColorFactorAIntensityPropertyModifierCreator());
	add_creator("Color_FactorB_Intensity", new SystemLightMapColorFactorBIntensityPropertyModifierCreator());
	add_creator("Light_Beam_Inner_Radius", new LightBeamInnerRadiusPropertyModifierCreator());
	add_creator("Light_Post_Beam_Blur_Coeff", new LightPostBeamBlurCoeffPropertyModifierCreator());

	add_creator("IsMale", new AvatarIsMalePropertyModifierCreator());
	add_creator("Body_Part", new AvatarBodyPartPropertyModifierCreator());
	add_creator("Action", new AvatarActionPropertyModifierCreator());
	add_creator("ActionLoop", new AvatarActionLoopPropertyModifierCreator());
	add_creator("ActionList", new AvatarActionListPropertyModifierCreator());
	add_creator("ZRotation", new ZRotationByDegreePropertyModifierCreator());
	add_creator("Recive_Fake_Light_Spot", new ReciveFakeLightSpotPropertyModifierCreator());
	add_creator("Light_Specular_Intensity", new LightSpecularIntensityPropertyModifierCreator());
	add_creator("Transition_Time_From_Current", new TransitionTimeFromCurrentPropertyModifierCreator());
	add_creator("Transition_Time_To_Next", new TransitionTimeToNextPropertyModifierCreator());

	//add_creator("Sound_Play", new SoundPlayPropertyModifierCreator());
	//add_creator("Sound_Resource", new SoundResourcePropertyModifierCreator());

	//仅编辑器使用
	add_creator("Lightmap_Shadow_Index", new LightMapShadowIndexPropertyModifierCreator());
	add_creator("Lightmap_Radius", new LightMapRadiusPropertyModifierCreator());
	
}

void PropertyModifierFactory::add_creator(const ActorPropertyType& propertyid, IPropertyModifierCreator* creator)
{
	ModifierCreatorCollectionItr itr = m_modifier_creator_map.find(GameGraphicHelper::to_lower(propertyid));

	if (itr != m_modifier_creator_map.end())
		return;

	m_modifier_creator_map[GameGraphicHelper::to_lower(propertyid)] = creator;
	m_modifier_creator_type_map[creator->property_type()] = creator;
}

void PropertyModifierFactory::remove_creator(const ActorPropertyType& propertyid)
{
	ModifierCreatorCollectionItr itr = m_modifier_creator_map.find(GameGraphicHelper::to_lower(propertyid));

	if (itr == m_modifier_creator_map.end())
		return;
	ModifierCreatorTypeMapItr iter = m_modifier_creator_type_map.find(itr->second->property_type());

	if (iter == m_modifier_creator_type_map.end())
	{
		assert(false);
	}

	m_modifier_creator_map.erase(itr);
	m_modifier_creator_type_map.erase(iter);

}

IPropertyModifier* PropertyModifierFactory::create_modifier(Actor* actor, const PropertyModifierType& type)
{
	IPropertyModifierCreator* creator = NULL;

	std::string pn = GameGraphicHelper::to_lower(type.property_name);

	if (m_modifier_creator_map.find(pn) != m_modifier_creator_map.end())
	{
		creator = m_modifier_creator_map[pn];
	}

	if (NULL != creator)
	{
		return creator->create_modifier(actor, type);
	}

	return NULL;
}

bool PropertyModifierFactory::releaseModifier(IPropertyModifier* modifier)
{
	IPropertyModifierCreator* creator = NULL;
	ModifierCreatorTypeMapItr itr = m_modifier_creator_type_map.find(modifier->type()) ;

	if (itr == m_modifier_creator_type_map.end())
		return false;

	return itr->second->release_modifier(modifier);
}

IPropertyModifier* PropertyModifierCreator::create_modifier(Actor* actor, const PropertyModifierType& type)
{
	Actor* owner_actor = find_actor(actor, type.child_actor_id);
	if (NULL == owner_actor)
	{
		return NULL;
	}

	CMediaEndEntity* sub_entity = find_entity(owner_actor, type.child_entity_id);

	if ((type.child_entity_id.length() > 0) && (NULL == sub_entity))
	{
		return NULL;
	}

	//分割固定参数
	std::list<std::string> params;
	divide_fixed_params(type.property_fixed_param, params);

	if (NULL == sub_entity)
	{
		return create_actor_property_modifier(owner_actor, type.property_name, params, type.is_curv);
	}
	else
	{
		return create_entity_property_modifier(sub_entity, type.property_name, params, type.is_curv);
	}
}

Actor* PropertyModifierCreator::find_actor(Actor* actor, const ActorID& subactor_id)
{
	ActorID actor_id = "";
	int begin_index = 1;
	while (true)
	{
		if ((begin_index >= subactor_id.length()) || (NULL == actor))
		{
			return actor;
		}

		int index = subactor_id.find('/', begin_index);
		if (index == std::string::npos)
		{
			actor_id = subactor_id.substr(begin_index, subactor_id.length()- begin_index);
			return actor->find_actor(actor_id.c_str());
		}
		else
		{
			actor_id = subactor_id.substr(begin_index, index - begin_index);
			actor = actor->find_actor(actor_id.c_str());
		}
		begin_index = index + 1;
	}

	return NULL;
}

CMediaEndEntity* PropertyModifierCreator::find_entity(Actor* owner, const SubEntityID& entity_id)
{
	if (entity_id.length() == 0)
	{
		return NULL;
	}

	return owner->find_entity(entity_id.c_str());
}

void PropertyModifierCreator::divide_fixed_params(const ModifierFixParaInfo& fixed_params, std::list<std::string>& params)
{
	std::string actor_id = "";
	int begin_index = 0;
	while (true)
	{
		if (begin_index >= fixed_params.length())
		{
			break;
		}

		int index = fixed_params.find('/', begin_index);
		if (index == std::string::npos)
		{
			actor_id = fixed_params.substr(begin_index, fixed_params.length()- begin_index);
			params.push_back(actor_id);
			break;
		}
		else
		{
			actor_id = fixed_params.substr(begin_index, index - begin_index);
			params.push_back(actor_id);
		}
		begin_index = index + 1;
	}
}

bool PropertyModifierCreator::release_modifier(IPropertyModifier* property)
{
	if (!property)
		return false;

	if (property->type() == property_type())
	{
		delete property;
		return true;
	}

	return false;
}

IPropertyModifier* PropertyModifierCreator::create_entity_property_modifier(CMediaEndEntity* entity, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv) 
{
	return NULL;
}

IPropertyModifier* PropertyModifierCreator::create_actor_property_modifier(Actor* entity, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv)
{
	return NULL;
}

