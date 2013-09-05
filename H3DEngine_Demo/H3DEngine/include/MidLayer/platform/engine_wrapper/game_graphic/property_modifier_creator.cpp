/**
* @file property_modifier_creator.cpp
* 这个文件的主要功能是：
* 
* @author 程陈(Chengchen@h3d.com.cn)
* 
* 更改日志
* ----------------------------------------------------
* ver 1.0.0
*       by 程陈2010.11.23
*/

#include "game_graphic_body.h"

template<typename ActorT, typename TClass, typename TResult, typename TPara0>
IPropertyModifier* createPercentCompositeCurvPropertyModifier(
	ActorT* owner, FuncApplierConst< TClass, TResult, TPara0>* applier, int property_type)
{
	return new PropertyCurvModifierStreambleFrame<TClass, 
		FuncApplierConst<TClass, TResult, TPara0>, PercentCompositeCurvSerializer<TPara0, float>, TPara0>(
		dynamic_cast<TClass*>(owner), applier, property_type);
}

template<typename ActorT, typename TClass, typename TResult, typename TPara0>
IPropertyModifier* createPercentCompositeCurvPropertyModifier(
	ActorT* owner, HelpFuncApplierConst< TClass, TResult, TPara0>* applier, int property_type)
{
	return new PropertyCurvModifierStreambleFrame<TClass, 
		HelpFuncApplierConst<TClass, TResult, TPara0>, PercentCompositeCurvSerializer<TPara0, float>, TPara0>(
		dynamic_cast<TClass*>(owner), applier, property_type);
}

template<typename TClass, typename TPara0, typename TResult>
GetSetApplier<TClass, TPara0>* createGetSetApplier(FuncApplierConst<TClass,TResult, TPara0>* setter, ConstFuncGetterConst<TClass, TPara0>* getter)
{
	return createGetSetApplier(dynamic_cast<Applier<TClass, TPara0>* >(setter), dynamic_cast<PropertyGetter<TClass, TPara0>* >(getter));
}

//////////////////////////////////////////////////////////////////////////
//可见性属性和属性序列Modifier生成器
IPropertyModifier* VisiblePropertyModifierCreator::create_entity_property_modifier(CMediaEndEntity* entity, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv)
{
	if (is_curv)
	{
		//创建对应的属性序列修改器;
		return createNormalCurvPropertyModifier(entity, createApplier(&C3DEntity::set_visible), property_type());
	}
	else
	{
		//创建对应的属性修改器
		return createPropertyModifier(entity, createApplier(&C3DEntity::set_visible), property_type());
	}
}

IPropertyModifier* VisiblePropertyModifierCreator::create_actor_property_modifier(Actor* actor, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv)
{
	if (is_curv)
	{
		//创建对应的属性序列修改器;
		return createNormalCurvPropertyModifier(actor, createApplier(&Actor::set_visible), property_type());
	}
	else
	{
		//创建对应的属性修改器
		return createPropertyModifier(actor, createApplier(&Actor::set_visible), property_type());
	}
}

template<typename T>
void set_location_matrix(T* actor, const H3DMat4& mat)
{
	actor->set_location_matrix(mat);
}

template<typename T>
void set_location(T* actor, const H3DMat3& mat)
{
	const H3DVec3& pos = mat[0];
	const H3DVec3& rotation = mat[1];
	const H3DVec3& scale = mat[2];

	H3DAngles angle(RAD2DEG(rotation.y), RAD2DEG(rotation.z), RAD2DEG(rotation.x));

	H3DQuat quat = angle.ToQuat();
	
	H3DMat4 loc;
	loc.Compose(scale, quat, pos);

	set_location_matrix(actor, loc);
}

template<typename T>
void set_path_point_pos(T* actor, const H3DVec3_2& new_pos)
{
	H3DMat4 loc = actor->get_location_matrix();
	H3DVec3 pos, scale;
	H3DQuat quat;
	loc.Decompose(scale, quat, pos);

	H3DVec3 tang = new_pos.m_tangent;

	if (tang.Length() > 0)
	{
		tang.Normalize();
		H3DVec3 axis = tang.Cross(H3DVec3(0.0f, -1.0f, 0.0f));
		float fCosA = tang.Dot(H3DVec3(0.0f, -1.0f, 0.0f));
		float fDegree = H3DMath::M_RAD2DEG * acosf(fCosA);

		quat.FromAngleAxis(fDegree, axis);

		H3DAngles angle = quat.ToAngles();
		angle.pitch = 0.0f;

		quat = angle.ToQuat();
		//angle生成的3x3矩阵放到4x4里需要转置，四元数和3x3矩阵是一致的
		quat = quat.Inverse();
	}

	H3DMat4 mat;
	mat.Compose(scale, quat, new_pos.m_position);
	set_location_matrix(actor, mat);
}
template<typename T>
void set_position(T* entity, const H3DVec3& new_pos)
{
	H3DMat4 loc = entity->get_location_matrix();
	H3DVec3 pos, scale;
	H3DQuat quat;
	loc.Decompose(scale, quat, pos);

	H3DMat4 mat;
	mat.Compose(scale, quat, new_pos);
	set_location_matrix(entity, mat);
}

template<typename T>
void set_rotation(T* entity, const H3DVec3& obj_rotation)
{
	H3DMat4 loc = entity->get_location_matrix();
	H3DVec3 pos, scale;
	H3DQuat quat;
	loc.Decompose(scale, quat, pos);

	H3DAngles angle(RAD2DEG(obj_rotation.y), RAD2DEG(obj_rotation.z), RAD2DEG(obj_rotation.x));
	H3DQuat new_quat = angle.ToQuat();
	//angle生成的3x3矩阵放到4x4里需要转置，四元数和3x3矩阵是一致的
	new_quat = new_quat.Inverse();

	H3DMat4 mat;
	mat.Compose(scale, new_quat, pos);
	set_location_matrix(entity, mat);
}

template<typename T>
void set_scale(T* entity, const H3DVec3& new_scale)
{
	H3DMat4 loc = entity->get_location_matrix();
	H3DVec3 pos, scale;
	H3DQuat quat;
	loc.Decompose(scale, quat, pos);

	H3DMat4 mat;
	mat.Compose(new_scale, quat, pos);
	set_location_matrix(entity, mat);
}


/**
* @brief
*		这个现在貌似是没人使用了，等过了一段时间确定没人使用就删了吧
*/
IPropertyModifier* LocationPropertyModifierCreator::create_entity_property_modifier(CMediaEndEntity* entity, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv)
{
	if (is_curv)
	{
		//创建对应的属性序列修改器;
		return createCompositeCurvPropertyModifierFloat(entity, createApplier(set_location<C3DEntity>), property_type());
	}
	else
	{
		//创建对应的属性修改器
		return createPropertyModifier(entity, createApplier(set_location<C3DEntity>), property_type());
	}
}

IPropertyModifier* LocationPropertyModifierCreator::create_actor_property_modifier(Actor* actor, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv)
{
	if (is_curv)
	{
		//创建对应的属性序列修改器;
		return createCompositeCurvPropertyModifierFloat(actor, createApplier(set_location<Actor>), property_type());
	}
	else
	{
		//创建对应的属性修改器
		return createPropertyModifier(actor, createApplier(set_location<Actor>), property_type());
	}
}

IPropertyModifier* PositionPropertyModifierCreator::create_entity_property_modifier(CMediaEndEntity* entity, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv)
{
	if (is_curv)
	{
		//createApplier(createApplier(&C3DEntity::set_position), createGetter(&C3DEntity::position));
		return createCompositeCurvPropertyModifierFloat(entity, 
			createGetSetApplier(createApplier(&C3DEntity::set_position), createGetter(&C3DEntity::position)), property_type());

		//创建对应的属性序列修改器;
//		return createCompositeCurvPropertyModifierFloat(entity, createApplier(&C3DEntity::set_position));
	}
	else
	{
		//创建对应的属性修改器
		return createPropertyModifier(entity, createApplier(&C3DEntity::set_position), property_type());
	}
}

IPropertyModifier* PositionPropertyModifierCreator::create_actor_property_modifier(Actor* actor, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv)
{
	if (is_curv)
	{
		//创建对应的属性序列修改器;
		return createCompositeCurvPropertyModifierFloat(actor, 
			createGetSetApplier(createApplier(&Actor::set_position), createGetter(&Actor::position)), property_type());
		//return createCompositeCurvPropertyModifierFloat(actor, createApplier(&Actor::set_position));
	}
	else
	{
		//创建对应的属性修改器
		return createPropertyModifier(actor, createApplier(&Actor::set_position), property_type());
	}
}

IPropertyModifier* PathPositionPropertyModifierCreator::create_entity_property_modifier(CMediaEndEntity* entity, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv)
{
	if (is_curv)
	{
		//创建对应的属性修改器
		//return createPercentCompositeCurvPropertyModifier(entity, createApplier(set_path_point_pos<C3DEntity>));
		return createPercentCompositeCurvPropertyModifier(entity, createApplier(set_position<C3DEntity>), property_type());
	}
	else
	{
		//return createPropertyModifier(entity, createApplier(set_path_point_pos<C3DEntity>));
		return createPropertyModifier(entity, createApplier(set_position<C3DEntity>), property_type());
	}

	return NULL;
}

IPropertyModifier* PathPositionPropertyModifierCreator::create_actor_property_modifier(Actor* actor, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv)
{
	if (is_curv)
	{
		//创建对应的属性序列修改器;
		return createPercentCompositeCurvPropertyModifier(actor, createApplier(set_path_point_pos<Actor>), property_type());
		//return createCompositeCurvPropertyModifierFloat(actor, createApplier(&Actor::set_position));
	}
	else
	{
		//创建对应的属性修改器
		return createPropertyModifier(actor, createApplier(&Actor::set_position), property_type());
	}
}

IPropertyModifier* RotationPropertyModifierCreator::create_entity_property_modifier(CMediaEndEntity* entity, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv)
{
	if (is_curv)
	{
		//创建对应的属性序列修改器;
		return createCompositeCurvPropertyModifierFloat(entity, 
			createGetSetApplier(createApplier(&C3DEntity::set_rotation), 
			createGetter(&C3DEntity::rotation)), property_type());

//		return createCompositeCurvPropertyModifierFloat(entity, createApplier(&C3DEntity::set_rotation));
	}
	else
	{
		//创建对应的属性修改器
		return createPropertyModifier(entity, createApplier(&C3DEntity::set_rotation), property_type());
	}
}

IPropertyModifier* RotationPropertyModifierCreator::create_actor_property_modifier(Actor* actor, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv)
{
	if (is_curv)
	{
		//创建对应的属性序列修改器;
		return createCompositeCurvPropertyModifierFloat(actor, 
			createGetSetApplier(createApplier(&Actor::set_rotation), createGetter(&Actor::rotation)), property_type());

//		return createCompositeCurvPropertyModifierFloat(actor, createApplier(&Actor::set_rotation));
	}
	else
	{
		//创建对应的属性修改器
		return createPropertyModifier(actor, createApplier(&Actor::set_rotation), property_type());
	}
}

IPropertyModifier* ScalePropertyModifierCreator::create_entity_property_modifier(CMediaEndEntity* entity, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv)
{
	if (is_curv)
	{
		//创建对应的属性序列修改器;
		return createCompositeCurvPropertyModifierFloat(entity, 
			createGetSetApplier(createApplier(&C3DEntity::set_scale), 
			createGetter(&C3DEntity::get_scale)), property_type());

//		return createCompositeCurvPropertyModifierFloat(entity, createApplier(&C3DEntity::set_scale));
	}
	else
	{
		//创建对应的属性修改器
		return createPropertyModifier(entity, createApplier(&C3DEntity::set_scale), property_type());
	}
}

IPropertyModifier* ScalePropertyModifierCreator::create_actor_property_modifier(Actor* actor, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv)
{
	if (is_curv)
	{
		//创建对应的属性序列修改器;
		return createCompositeCurvPropertyModifierFloat(actor, 
			createGetSetApplier(createApplier(&Actor::set_scale), createGetter(&Actor::get_scale)), property_type());


//		return createCompositeCurvPropertyModifierFloat(actor, createApplier(&Actor::set_scale));
	}
	else
	{
		//创建对应的属性修改器
		return createPropertyModifier(actor, createApplier(&Actor::set_scale), property_type());
	}
}




//////////////////////////////////////////////////////////////////////////
//Entity的其他一些基本属性

IPropertyModifier* ResourcePropertyModifierCreator::create_entity_property_modifier(CMediaEndEntity* entity, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv)
{
	if (is_curv)
	{
		//创建对应的属性序列修改器;
		return createNormalCurvPropertyModifier(entity, createApplier(&C3DEntity::change_resource), property_type());
	}
	else
	{
		//创建对应的属性修改器
		return createPropertyModifier(entity, createApplier(&C3DEntity::change_resource), property_type());
	}
}

////////////////////////////////////////////////////////////////////////////
//相机的属性和属性序列

void set_camera_frustum(CCameraEntity* camera, const H3DVec4& value)
{
	camera->set_frustum(value[0], value[1], value[2], value[3]);
}

H3DVec4 get_camera_frustum(CCameraEntity* camera)
{
	float fov = 0;
	float ratio = 0;
	float fNear = 0;
	float fFar = 0;
	camera->get_frustum(&fov, &ratio, &fNear, &fFar);

	H3DVec4 vec(fov, ratio, fNear, fFar);

	return vec;
}

void set_camera_fov(CCameraEntity* camera, float value)
{
	float fov, ratio, fNear, fFar;
	camera->get_frustum(&fov, &ratio, &fNear, &fFar);
	fov = value;
	camera->set_frustum(fov, ratio, fNear, fFar);
}
void set_camera_ratio(CCameraEntity* camera, float value)
{
//	float fov, ratio, fNear, fFar;
//	camera->get_frustum(&fov, &ratio, &fNear, &fFar);
//	ratio = value;
//	camera->set_frustum(fov, ratio, fNear, fFar);
}
void set_camera_fNear(CCameraEntity* camera, float value)
{
	float fov, ratio, fNear, fFar;
	camera->get_frustum(&fov, &ratio, &fNear, &fFar);
	fNear = value;
	camera->set_frustum(fov, ratio, fNear, fFar);
}
void set_camera_fFar(CCameraEntity* camera, float value)
{
	float fov, ratio, fNear, fFar;
	camera->get_frustum(&fov, &ratio, &fNear, &fFar);
	fFar = value;
	camera->set_frustum(fov, ratio, fNear, fFar);
}

//Frustum属性和属性序列Modifer生成器
IPropertyModifier* CameraFrustumPropertyModifierCreator::create_entity_property_modifier(CMediaEndEntity* entity, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv)
{
	if (is_curv)
	{
		//创建对应的属性序列修改器;
		return createCompositeCurvPropertyModifierFloat(entity, 
			createGetSetApplier(createApplier(set_camera_frustum), createGetter(get_camera_frustum)), property_type());


//		return createCompositeCurvPropertyModifierFloat(entity, createApplier(set_camera_frustum));
		//return NULL;
	}
	else
	{
		//创建对应的属性修改器
		return createPropertyModifier(entity, createApplier(set_camera_frustum), property_type());
	}
	return NULL;
}

//Frustum Fov属性和属性序列Modifer生成器
IPropertyModifier* CameraFovPropertyModifierCreator::create_entity_property_modifier(CMediaEndEntity* entity, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv)
{
	if (is_curv)
	{
		//创建对应的属性序列修改器;
		return createNormalCurvPropertyModifier(entity, createApplier(set_camera_fov), property_type());
		//return NULL;
	}
	else
	{
		//创建对应的属性修改器
		return createPropertyModifier(entity, createApplier(set_camera_fov), property_type());
	}
}

//Frustum Ration属性和属性序列Modifer生成器
IPropertyModifier* CameraRatioPropertyModifierCreator::create_entity_property_modifier(CMediaEndEntity* entity, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv)
{
	if (is_curv)
	{
		//创建对应的属性序列修改器;
		return createNormalCurvPropertyModifier(entity, createApplier(set_camera_ratio), property_type());
		//return NULL;
	}
	else
	{
		//创建对应的属性修改器
		return createPropertyModifier(entity, createApplier(set_camera_ratio), property_type());
	}
}

//Frustum fNear属性和属性序列Modifer生成器
IPropertyModifier* CameraNearPlanePropertyModifierCreator::create_entity_property_modifier(CMediaEndEntity* entity, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv)
{
	if (is_curv)
	{
		//创建对应的属性序列修改器;
		return createNormalCurvPropertyModifier(entity, createApplier(set_camera_fNear), property_type());
		//return NULL;
	}
	else
	{
		//创建对应的属性修改器
		return createPropertyModifier(entity, createApplier(set_camera_fNear), property_type());
	}
}

//Frustum fFar属性和属性序列Modifer生成器
IPropertyModifier* CameraFarPlanePropertyModifierCreator::create_entity_property_modifier(CMediaEndEntity* entity, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv)
{
	if (is_curv)
	{
		//创建对应的属性序列修改器;
		return createNormalCurvPropertyModifier(entity, createApplier(set_camera_fFar), property_type());
		//return NULL;
	}
	else
	{
		//创建对应的属性修改器
		return createPropertyModifier(entity, createApplier(set_camera_fFar), property_type());
	}
}

//Focalplane属性和属性序列Modifer生成器
IPropertyModifier* CameraFocalplanePropertyModifierCreator::create_entity_property_modifier(CMediaEndEntity* entity, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv)
{
	if (is_curv)
	{
		//创建对应的属性序列修改器;
		return createNormalCurvPropertyModifier(entity, createApplier(&CCameraEntity::set_focalplane), property_type());
	}
	else
	{
		//创建对应的属性修改器
		return createPropertyModifier(entity, createApplier(&CCameraEntity::set_focalplane), property_type());
	}
}

IPropertyModifier* CameraDofPropertyModifierCreator::create_entity_property_modifier(CMediaEndEntity* entity, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv)
{
	if (is_curv)
	{
		//创建对应的属性序列修改器;
		return createNormalCurvPropertyModifier(entity, createApplier(&CCameraEntity::set_dof_range), property_type());
	}
	else
	{
		//创建对应的属性修改器
		return createPropertyModifier(entity, createApplier(&CCameraEntity::set_dof_range), property_type());
	}
}

//Confusion属性和属性序列Modifer生成器
IPropertyModifier* CameraConfusionPropertyModifierCreator::create_entity_property_modifier(CMediaEndEntity* entity, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv)
{
	if (is_curv)
	{
		//创建对应的属性序列修改器;
		return createNormalCurvPropertyModifier(entity, createApplier(&CCameraEntity::set_confusion), property_type());
	}
	else
	{
		//创建对应的属性修改器
		return createPropertyModifier(entity, createApplier(&CCameraEntity::set_confusion), property_type());
	}
}

//Lookat属性和属性序列Modifer生成器
IPropertyModifier* CameraLookatPropertyModifierCreator::create_entity_property_modifier(CMediaEndEntity* entity, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv)
{
	if (is_curv)
	{
		//创建对应的属性序列修改器;
		return createCompositeCurvPropertyModifierFloat(entity, createGetSetApplier(createApplier(&CCameraEntity::look_at), 
			createGetter(&CCameraEntity::get_look_at)), property_type());

//		return createCompositeCurvPropertyModifierFloat(entity, createApplier(&CCameraEntity::look_at));
	}
	else
	{
		//创建对应的属性修改器
		return createPropertyModifier(entity, createApplier(&CCameraEntity::look_at), property_type());
	}
}

//UpDirection属性和属性序列Modifer生成器
IPropertyModifier* CameraUpDirectionPropertyModifierCreator::create_entity_property_modifier(CMediaEndEntity* entity, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv)
{
	if (is_curv)
	{
		//创建对应的属性序列修改器;
		return createCompositeCurvPropertyModifierFloat(entity, createGetSetApplier<CCameraEntity, H3DVec3, void>(
			createApplier<CCameraEntity, void, H3DVec3>(&CCameraEntity::set_up_direction), 
			createGetter<CCameraEntity, H3DVec3>(&CCameraEntity::up_direction)), property_type());

//		return createCompositeCurvPropertyModifierFloat(entity, createApplier(&CCameraEntity::set_up_direction));
	}
	else
	{
		//创建对应的属性修改器
		return createPropertyModifier(entity, createApplier(&CCameraEntity::set_up_direction), property_type());
	}
}

void null_func(CCameraEntity* camera, const std::string& value){}

//CameraAdjustType属性和属性序列Modifer生成器
IPropertyModifier* CameraAdjustTypePropertyModifierCreator::create_entity_property_modifier(CMediaEndEntity* entity, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv)
{
	if (!is_curv)
	{
		//创建对应的属性修改器
		return createPropertyModifier(entity, createApplier(&CCameraEntity::set_smooth_type), property_type());
	}
	
	return NULL;
}

//CameraTargetType属性和属性序列Modifer生成器
IPropertyModifier* CameraTargetTypePropertyModifierCreator::create_entity_property_modifier(CMediaEndEntity* entity, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv)
{
	if (!is_curv)
	{
		//创建对应的属性修改器
		return createPropertyModifier(entity, createApplier(&CCameraEntity::set_target_type), property_type());
	}

	return NULL;
}

//CameraTargetes属性和属性序列Modifer生成器
IPropertyModifier* CameraTargetesPropertyModifierCreator::create_entity_property_modifier(CMediaEndEntity* entity, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv)
{
	if (!is_curv)
	{
		//创建对应的属性修改器
		return createPropertyModifier(entity, createApplier(&CCameraEntity::set_target_id), property_type());
	}

	return NULL;
}

void set_camera_post_process(CCameraEntity* camera, const std::string& post_process_name, const std::string& post_process_param_name, float value)
{
	H3DVec4 vec;
	vec.x = value;
	camera->set_postprocess_param(post_process_name.c_str(), post_process_param_name.c_str(), vec);
}

void set_camera_post_process_const(CCameraEntity* camera, const std::string& post_process_name, const std::string& post_process_param_name, const H3DVec4& value)
{
	camera->set_postprocess_param(post_process_name.c_str(), post_process_param_name.c_str(), value);
}

H3DVec4 get_camera_post_process_const(CCameraEntity* camera, const std::string& post_process_name, const std::string& post_process_param)
{
	const CCameraEntity::PostprocessCollection& col = camera->get_postprocess_param();
	CCameraEntity::PostprocessCollection::const_iterator it = col.find(post_process_name);
	if(it ==  col.end())
	{
		return H3DVec4(0, 0, 0, 0);
	}

	CCameraEntity::PostprocessParamCollection::const_iterator param_it = (*it).second.find(post_process_param);

	if (param_it == (*it).second.end())
	{
		return H3DVec4(0, 0, 0, 0);
	}

	return (*param_it).second;
}

//相机后处理的属性和属性序列Modifer生成器
IPropertyModifier* CameraPostprocessPropertyModifierCreator::create_entity_property_modifier(CMediaEndEntity* entity, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv)
{
	if (is_curv)
	{
		//固定参数为 postprocess_name/postprocess_param_name/type
		std::list<ModifierFixParaInfo>::const_iterator it = fixed_params.begin();
		std::string  postprocess_param_name = *it++;
		std::string  postprocess_name = *it++;
		std::string  param_type = *it++;
		if (stricmp(param_type.c_str(), "float") == 0)
		{
			return createNormalCurvPropertyModifier(entity, createApplier(set_camera_post_process, 
				postprocess_name, postprocess_param_name), property_type());
		}
		else if ((stricmp(param_type.c_str(), "float4") == 0) || (stricmp(param_type.c_str(), "color") == 0))
		{
			return createCompositeCurvPropertyModifierFloat(entity, createGetSetApplier(createApplier(
				set_camera_post_process_const, postprocess_name, postprocess_param_name), 
				createGetter(get_camera_post_process_const, postprocess_name, postprocess_param_name)), property_type());

//			return createCompositeCurvPropertyModifierFloat(entity, createApplier(set_camera_post_process_const, postprocess_name, postprocess_param_name));
		}
	}

	return NULL;
}

//相机位置的属性和属性序列Modifer生成器
IPropertyModifier* CameraPostitionPropertyModifierCreator::create_entity_property_modifier(CMediaEndEntity* entity, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv)
{
	if (is_curv)
	{
		//创建对应的属性修改器
		return createPercentCompositeCurvPropertyModifier(entity, createApplier(set_position<C3DEntity>), property_type());
	}
	else
	{
		return createPropertyModifier(entity, createApplier(set_position<C3DEntity>), property_type());
	}

	return NULL;
}

////////////////////////////////////////////////////////////////////////////
//模型的属性和属性序列创建

void enable_model_shader(CModelEntity* model, bool value)
{
	model->setShadow(value);
}

//开启模型阴影属性和属性序列Modifer生成器
IPropertyModifier* ModelShadelEnablePropertyModifierCreator::create_entity_property_modifier(CMediaEndEntity* entity, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv)
{
	if (is_curv)
	{
		//创建对应的属性序列修改器;
		return createNormalCurvPropertyModifier(entity, createApplier(enable_model_shader), property_type());
	}
	else
	{
		//创建对应的属性修改器
		return createPropertyModifier(entity, createApplier(enable_model_shader), property_type());
	}
}

//模型高亮属性和属性序列Modifer生成器
IPropertyModifier* ModelHighLightPropertyModifierCreator::create_entity_property_modifier(CMediaEndEntity* entity, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv)
{
	if (is_curv)
	{
		//创建对应的属性序列修改器;
		return createNormalCurvPropertyModifier(entity, createApplier(&CModelEntity::high_light_model), property_type());
	}
	else
	{
		//创建对应的属性修改器
		return createPropertyModifier(entity, createApplier(&CModelEntity::high_light_model), property_type());
	}
}

template<typename T>
void set_shader_value(CModelEntity* model, H3DI::INewShader* shader, const std::string& node_name, T value)
{
	model->set_shader_node_value(shader, node_name, value);
}

template<typename T>
void set_shader_value_const(CModelEntity* model, H3DI::INewShader* shader, const std::string& node_name, const T& value)
{
	model->set_shader_node_value(shader, node_name, value);
}

template<>
void set_shader_value_const<H3DVec4>(CModelEntity* model, H3DI::INewShader* shader, const std::string& node_name, const H3DVec4& value)
{
	model->set_shader_node_value(shader, node_name, value.x, value.y, value.z, value.w);
}

//模型材质属性和属性序列Modifer生成器
IPropertyModifier* ModelShaderVarPropertyModifierCreator::create_entity_property_modifier(CMediaEndEntity* entity, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv)
{
	std::list<ModifierFixParaInfo>::const_iterator it = fixed_params.begin();
	std::string shader_name = *it++;
	std::string shader_var_name = *it++;
	std::string shader_var_type = *it++;

	H3DI::INewShader* shader = (dynamic_cast<CModelEntity*>(entity))->get_new_shader(shader_name);
	if (NULL == shader)
	{
		return NULL;
	}

	if (stricmp(shader_var_type.c_str(), "int") == 0)
	{
		if (is_curv)
		{
			return createNormalCurvPropertyModifier(entity, createApplier(set_shader_value<int>,shader, shader_var_name), property_type());
			return NULL;
		}
		else
		{
			//创建对应的属性修改器
			return createPropertyModifier(entity, createApplier(set_shader_value<int>,shader, shader_var_name), property_type());
		}
	}
	else if (stricmp(shader_var_type.c_str(), "float") == 0)
	{
		if (is_curv)
		{
			//创建对应的属性序列修改器;
			return createNormalCurvPropertyModifier(entity, createApplier(set_shader_value<float>,shader, shader_var_name), property_type());
			//return NULL;
		}
		else
		{
			//创建对应的属性修改器
			return createPropertyModifier(entity, createApplier(set_shader_value<float>,shader, shader_var_name), property_type());
		}
	}
	else if (stricmp(shader_var_type.c_str(), "float4") == 0)
	{
		if (is_curv)
		{
			//创建对应的属性序列修改器;
			return createNormalCurvPropertyModifier(entity, createApplier(set_shader_value_const<H3DVec4>,shader, 
				shader_var_name), property_type());
			//return NULL;
		}
		else
		{
			//创建对应的属性修改器
			return createPropertyModifier(entity, createApplier(set_shader_value_const<H3DVec4>,shader, 
				shader_var_name), property_type());
		}
	}
	else if (stricmp(shader_var_type.c_str(), "string") == 0)
	{
		if (is_curv)
		{
			//创建对应的属性序列修改器;
			return createNormalCurvPropertyModifier(entity, createApplier(set_shader_value_const<std::string>,
				shader, shader_var_name), property_type());
			//return NULL;
		}
		else
		{
			//创建对应的属性修改器
			return createPropertyModifier(entity, createApplier(set_shader_value_const<std::string>,
				shader, shader_var_name), property_type());
		}
	}
	else if (stricmp(shader_var_type.c_str(), "matrix44") == 0)
	{
		if (is_curv)
		{
			//创建对应的属性序列修改器;
			return createNormalCurvPropertyModifier(entity, createApplier(set_shader_value_const<H3DMat4>,
				shader, shader_var_name), property_type());
			//return NULL;
		}
		else
		{
			//创建对应的属性修改器
			return createPropertyModifier(entity, createApplier(set_shader_value_const<H3DMat4>,
				shader, shader_var_name), property_type());
		}
	}

	return NULL;
}

//模型振荡器属性和属性序列Modifer生成器
IPropertyModifier* ModelWaveFormPropertyModifierCreator::create_entity_property_modifier(CMediaEndEntity* entity, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv)
{
	std::list<ModifierFixParaInfo>::const_iterator it = fixed_params.begin();
	std::string shader_name = *it++;
	std::string shader_var_type = *it++;

	H3DI::WaveFormParam var_type;
	if (stricmp(shader_var_type.c_str(), "WFP_FREQ") == 0)
	{
		var_type = H3DI::WFP_FREQ;
	}
	else if (stricmp(shader_var_type.c_str(), "WFP_RANGE") == 0)
	{
		var_type = H3DI::WFP_RANGE;
	}
	else if (stricmp(shader_var_type.c_str(), "WFP_OFFSET") == 0)
	{
		var_type = H3DI::WFP_OFFSET;
	}
	else if (stricmp(shader_var_type.c_str(), "WFP_RANGEOFFSET") == 0)
	{
		var_type = H3DI::WFP_RANGEOFFSET;
	}
	else
	{
		return NULL;
	}

	H3DI::INewShader* shader = (dynamic_cast<CModelEntity*>(entity))->get_new_shader(shader_name);
	if (NULL == shader)
	{
		return NULL;
	}


	if (is_curv)
	{
		//创建对应的属性序列修改器;
		return createNormalCurvPropertyModifier(entity, 
			createApplier(&CModelEntity::set_waveform_var, shader, var_type), property_type());
		//return NULL;
	}
	else
	{
		//创建对应的属性修改器
		return createPropertyModifier(entity, 
			createApplier(&CModelEntity::set_waveform_var, shader, var_type), property_type());
	}
}


//////////////////////////////////////////////////////////////////////////
//灯光的属性和属性序列生成器

//灯光启用属性和属性序列Modifer生成器
IPropertyModifier* LightEnablePropertyModifierCreator::create_entity_property_modifier(CMediaEndEntity* entity, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv)
{
	if (is_curv)
	{
		return createNormalCurvPropertyModifier(entity, 
			createApplier(&CPrePassLightEntity::turn_on_light), property_type());
	}
	else
	{
		//创建对应的属性修改器
		return createPropertyModifier(entity, 
			createApplier(&CPrePassLightEntity::turn_on_light), property_type());
	}
}

//光照强度属性和属性序列Modifer生成器
IPropertyModifier* LightIntensityPropertyModifierCreator::create_entity_property_modifier(CMediaEndEntity* entity, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv) 
{
	if (is_curv)
	{
		return createNormalCurvPropertyModifier(entity, 
			createApplier(&CPrePassLightEntity::set_intensity), property_type());
	}
	else
	{
		//创建对应的属性修改器
		return createPropertyModifier(entity, 
			createApplier(&CPrePassLightEntity::set_intensity), property_type());
	}
}

//光照强度属性和属性序列Modifer生成器
IPropertyModifier* LightSpecularIntensityPropertyModifierCreator::create_entity_property_modifier(CMediaEndEntity* entity, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv) 
{
	if (is_curv)
	{
		return createNormalCurvPropertyModifier(entity, 
			createApplier(&CDirectionPrePassLightEntity::set_specular_intensity), property_type());
	}
	else
	{
		//创建对应的属性修改器
		return createPropertyModifier(entity, 
			createApplier(&CDirectionPrePassLightEntity::set_specular_intensity), property_type());
	}
}

//启用灯光阴影属性和属性序列Modifer生成器
IPropertyModifier* EnableLightShadowPropertyModifierCreator::create_entity_property_modifier(CMediaEndEntity* entity, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv)
{
	if (is_curv)
	{
		return createNormalCurvPropertyModifier(entity, 
			createApplier(&CPrePassLightEntity::set_shadow_enable), property_type());
	}
	else
	{
		//创建对应的属性修改器
		return createPropertyModifier(entity, 
			createApplier(&CPrePassLightEntity::set_shadow_enable), property_type());
	}
}

////灯光阴影颜色属性和属性序列Modifer生成器
IPropertyModifier* LightShadowColorPropertyModifierCreator::create_entity_property_modifier(CMediaEndEntity* entity, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv)
{
	if (is_curv)
	{
		return createCompositeCurvPropertyModifierFloat(entity, 
			createGetSetApplier(createApplier(&CPrePassLightEntity::set_shadow_color), 
			createGetter(&CPrePassLightEntity::shadow_color)), property_type());

//		return createCompositeCurvPropertyModifierFloat(entity, createApplier(&CPrePassLightEntity::set_shadow_color));
	}
	else
	{
		//创建对应的属性修改器
		return createPropertyModifier(entity, createApplier(&CPrePassLightEntity::set_shadow_color), property_type());
	}
}

//灯光近范围属性和属性序列Modifer生成器
IPropertyModifier* LightNearRangePropertyModifierCreator::create_entity_property_modifier(CMediaEndEntity* entity, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv)
{
	if (is_curv)
	{
		return createNormalCurvPropertyModifier(entity, createApplier(&CPrePassLightEntity::set_near_range), property_type());
	}
	else
	{
		//创建对应的属性修改器
		return createPropertyModifier(entity, createApplier(&CPrePassLightEntity::set_near_range), property_type());
	}
}

//灯光远范围属性和属性序列Modifer生成器
IPropertyModifier* LightFarRangePropertyModifierCreator::create_entity_property_modifier(CMediaEndEntity* entity, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv) 
{
	if (is_curv)
	{
		return createNormalCurvPropertyModifier(entity, createApplier(&CPrePassLightEntity::set_far_range), property_type());
	}
	else
	{
		return createPropertyModifier(entity, createApplier(&CPrePassLightEntity::set_far_range), property_type());
	}
}

//灯光影响范围属性和属性序列Modifer生成器
IPropertyModifier* LightAffectParamPropertyModifierCreator::create_entity_property_modifier(CMediaEndEntity* entity, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv) 
{
	if (is_curv)
	{
		return createNormalCurvPropertyModifier(entity, createApplier(&CPrePassLightEntity::set_affect_param), property_type());
	}
	else
	{
		return createPropertyModifier(entity, createApplier(&CPrePassLightEntity::set_affect_param), property_type());
	}
}

//灯光颜色属性和属性序列
IPropertyModifier* LightColorPropertyModifierCreator::create_entity_property_modifier(CMediaEndEntity* entity, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv)
{
	if (is_curv)
	{
		//创建对应的属性序列修改器;
		return createCompositeCurvPropertyModifierFloat(entity, createGetSetApplier(
			createApplier(&CPrePassLightEntity::set_color), createGetter(&CPrePassLightEntity::color)), property_type());

// 		return createCompositeCurvPropertyModifierFloat(entity, createApplier(&CPrePassLightEntity::set_color));
	}
	else
	{
		//创建对应的属性修改器
		return createPropertyModifier(entity, createApplier(&CPrePassLightEntity::set_color), property_type());
	}

}

void set_light_glare_width(CPrePassLightEntity* light, int glare_index, float value)
{
	GlareProperty glare_property = light->get_glare_property(glare_index);

	glare_property.width = value;

	light->set_glare_property(glare_index, glare_property);
}

void set_light_glare_height(CPrePassLightEntity* light, int glare_index, float value)
{
	GlareProperty glare_property = light->get_glare_property(glare_index);

	glare_property.height = value;

	light->set_glare_property(glare_index, glare_property);
}

void set_light_glare_color(CPrePassLightEntity* light, int glare_index, const H3DColor& value)
{
	GlareProperty glare_property = light->get_glare_property(glare_index);

	glare_property.color = value;

	light->set_glare_property(glare_index, glare_property);
}

H3DColor get_light_glare_color(CPrePassLightEntity* light, int glare_index)
{
	GlareProperty glare_property = light->get_glare_property(glare_index);
	return glare_property.color;
}

void set_light_glare_texture_name(CPrePassLightEntity* light, int glare_index, const std::string& value)
{
	GlareProperty glare_property = light->get_glare_property(glare_index);

	glare_property.texture_name = value;

	light->set_glare_property(glare_index, glare_property);
}

void set_light_glare_shader_lib(CPrePassLightEntity* light, int glare_index,const std::string& value)
{
	GlareProperty glare_property = light->get_glare_property(glare_index);

	glare_property.shader_lib = value;

	light->set_glare_property(glare_index, glare_property);
}

void set_light_glare_shader_name(CPrePassLightEntity* light, int glare_index, const std::string& value)
{
	GlareProperty glare_property = light->get_glare_property(glare_index);

	glare_property.shader_instance_name = value;

	light->set_glare_property(glare_index, glare_property);
}


void set_light_glare_distance(CPrePassLightEntity* light, int glare_index, float value)
{
	GlareProperty glare_property = light->get_glare_property(glare_index);

	glare_property.glare_distance = value;

	light->set_glare_property(glare_index, glare_property);
}

void set_light_glare_rotation_velocity(CPrePassLightEntity* light, int glare_index, float value)
{
	GlareProperty glare_property = light->get_glare_property(glare_index);

	glare_property.glare_rotation_velocity = value;

	light->set_glare_property(glare_index, glare_property);
}

void set_light_glare_visible(CPrePassLightEntity* light, int glare_index, bool visible)
{
	GlareProperty glare_property = light->get_glare_property(glare_index);

	glare_property.is_visible = visible;

	light->set_glare_property(glare_index, glare_property);
}

void set_light_glare_rotate_with_camera(CPrePassLightEntity* light, int glare_index, bool value)
{
	GlareProperty glare_property = light->get_glare_property(glare_index);

	glare_property.is_rotate_with_camera = value;

	light->set_glare_property(glare_index, glare_property);
}

void set_light_glare_view_camera(CPrePassLightEntity* light, int glare_index, bool value)
{
	GlareProperty glare_property = light->get_glare_property(glare_index);

	glare_property.is_view_camera = value;

	light->set_glare_property(glare_index, glare_property);
}

//光斑宽度属性和属性序列Modifer生成器
IPropertyModifier* LightGlareWidthPropertyModifierCreator::create_entity_property_modifier(CMediaEndEntity* entity, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv)
{
	std::string str = *(fixed_params.begin());
	int index = GameGraphicHelper::str2Value<int>(str);
	if (is_curv)
	{
		return createNormalCurvPropertyModifier(entity, createApplier(set_light_glare_width, index), property_type());
	}
	else
	{
		//创建对应的属性修改器
		return createPropertyModifier(entity, createApplier(set_light_glare_width, index), property_type());
	}
}

//光斑高度属性和属性序列Modifer生成器
IPropertyModifier* LightGlareHeightPropertyModifierCreator::create_entity_property_modifier(CMediaEndEntity* entity, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv)
{
	std::string str = *(fixed_params.begin());
	int index = GameGraphicHelper::str2Value<int>(str);
	if (is_curv)
	{
		return createNormalCurvPropertyModifier(entity, createApplier(set_light_glare_height, index), property_type());
	}
	else
	{
		//创建对应的属性修改器
		return createPropertyModifier(entity, createApplier(set_light_glare_height, index), property_type());
	}
}

//光斑颜色属性和属性序列Modifer生成器
IPropertyModifier* LightGlareColorPropertyModifierCreator::create_entity_property_modifier(CMediaEndEntity* entity, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv)
{
	std::string str = *(fixed_params.begin());
	int index = GameGraphicHelper::str2Value<int>(str);
	if (is_curv)
	{
		return createCompositeCurvPropertyModifierFloat(entity, createGetSetApplier(
			createApplier(set_light_glare_color, index), createGetter(get_light_glare_color, index)), property_type());

//		return createCompositeCurvPropertyModifierFloat(entity, createApplier(set_light_glare_color, index));
	}
	else
	{
		//创建对应的属性修改器
		return createPropertyModifier(entity, createApplier(set_light_glare_color, index), property_type());
	}
}

//光斑贴图路径属性和属性序列Modifer生成器
IPropertyModifier* LightGlareTexturePropertyModifierCreator::create_entity_property_modifier(CMediaEndEntity* entity, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv)
{
	std::string str = *(fixed_params.begin());
	int index = GameGraphicHelper::str2Value<int>(str);
	if (is_curv)
	{
		return createNormalCurvPropertyModifier(entity, createApplier(set_light_glare_texture_name, index), property_type());
	}
	else
	{
		//创建对应的属性修改器
		return createPropertyModifier(entity, createApplier(set_light_glare_texture_name, index), property_type());
	}
}

 
 //光斑贴图路径属性和属性序列Modifer生成器
IPropertyModifier* LightGlareShaderLibPropertyModifierCreator::create_entity_property_modifier(CMediaEndEntity* entity, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv)
{
	std::string str = *(fixed_params.begin());
	int index = GameGraphicHelper::str2Value<int>(str);
	if (is_curv)
	{
		return createNormalCurvPropertyModifier(entity, createApplier(set_light_glare_shader_lib, index), property_type());
	}
	else
	{
		//创建对应的属性修改器
		return createPropertyModifier(entity, createApplier(set_light_glare_shader_lib, index), property_type());
	}
}

//光斑材质名称属性和属性序列Modifer生成器
IPropertyModifier* LightGlareShaderNamePropertyModifierCreator::create_entity_property_modifier(CMediaEndEntity* entity, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv)
{
	std::string str = *(fixed_params.begin());
	int index = GameGraphicHelper::str2Value<int>(str);
	if (is_curv)
	{
		return createNormalCurvPropertyModifier(entity, createApplier(set_light_glare_shader_name, index), property_type());
	}
	else
	{
		//创建对应的属性修改器
		return createPropertyModifier(entity, createApplier(set_light_glare_shader_name, index), property_type());
	}
}

//光斑距离属性和属性序列Modifer生成器
IPropertyModifier* LightGlareDistancePropertyModifierCreator::create_entity_property_modifier(CMediaEndEntity* entity, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv)
{
	std::string str = *(fixed_params.begin());
	int index = GameGraphicHelper::str2Value<int>(str);
	if (is_curv)
	{
		return createNormalCurvPropertyModifier(entity, createApplier(set_light_glare_distance, index), property_type());
	}
	else
	{
		//创建对应的属性修改器
		return createPropertyModifier(entity, createApplier(set_light_glare_distance, index), property_type());
	}
}

// //光斑旋转速度属性和属性序列Modifer生成器
IPropertyModifier* LightGlareRotationVelocityPropertyModifierCreator::create_entity_property_modifier(CMediaEndEntity* entity, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv)
{
	std::string str = *(fixed_params.begin());
	int index = GameGraphicHelper::str2Value<int>(str);
	if (is_curv)
	{
		return createNormalCurvPropertyModifier(entity, createApplier(set_light_glare_rotation_velocity, index), property_type());
	}
	else
	{
		//创建对应的属性修改器
		return createPropertyModifier(entity, createApplier(set_light_glare_rotation_velocity, index), property_type());
	}
}

IPropertyModifier* LightGlareVisiblePropertyModifierCreator ::create_entity_property_modifier(CMediaEndEntity* entity, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv) 
{
	std::string str = *(fixed_params.begin());
	int index = GameGraphicHelper::str2Value<int>(str);
	if (is_curv)
	{
		return createNormalCurvPropertyModifier(entity, createApplier(set_light_glare_visible, index), property_type());
	}
	else
	{
		//创建对应的属性修改器
		return createPropertyModifier(entity, createApplier(set_light_glare_visible, index), property_type());
	}
}

/////光斑跟随摄像机转动属性和属性序列Modifer生成器
IPropertyModifier* LightGlareRotateWithCameraPropertyModifierCreator::create_entity_property_modifier(CMediaEndEntity* entity, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv)
{
	std::string str = *(fixed_params.begin());
	int index = GameGraphicHelper::str2Value<int>(str);
	if (is_curv)
	{
		return createNormalCurvPropertyModifier(entity, createApplier(set_light_glare_rotate_with_camera, index), property_type());
	}
	else
	{
		//创建对应的属性修改器
		return createPropertyModifier(entity, createApplier(set_light_glare_rotate_with_camera, index), property_type());
	}
}

////光斑面向摄像机属性和属性序列Modifer生成器
IPropertyModifier* LightGlareViewCameraPropertyModifierCreator::create_entity_property_modifier(CMediaEndEntity* entity, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv)
{
	std::string str = *(fixed_params.begin());
	int index = GameGraphicHelper::str2Value<int>(str);
	if (is_curv)
	{
		return createNormalCurvPropertyModifier(entity, createApplier(set_light_glare_view_camera, index), property_type());
	}
	else
	{
		//创建对应的属性修改器
		return createPropertyModifier(entity, createApplier(set_light_glare_view_camera, index), property_type());
	}
}

////光柱线绘制属性和属性序列Modifer生成器
IPropertyModifier* LightBeamLineDrawPropertyModifierCreator::create_entity_property_modifier(CMediaEndEntity* entity, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv)
{
	if (is_curv)
	{
		return createNormalCurvPropertyModifier(entity, createApplier(&CPrePassLightEntity::set_beam_line_draw), property_type());
	}
	else
	{
		//创建对应的属性修改器
		return createPropertyModifier(entity, createApplier(&CPrePassLightEntity::set_beam_line_draw), property_type());
	}
}


////LightPriority属性和属性序列Modifer生成器
IPropertyModifier* LightPriorityPropertyModifierCreator::create_entity_property_modifier(CMediaEndEntity* entity, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv)
{
	if (is_curv)
	{
		return createNormalCurvPropertyModifier(entity, createApplier(&CPrePassLightEntity::set_priority), property_type());
	}
	else
	{
		//创建对应的属性修改器
		return createPropertyModifier(entity, createApplier(&CPrePassLightEntity::set_priority), property_type());
	}
}

//体积光开启属性和属性序列Modifer生成器
IPropertyModifier* LightShaftEnablePropertyModifierCreator::create_entity_property_modifier(CMediaEndEntity* entity, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv)
{
	if (is_curv)
	{
		return createNormalCurvPropertyModifier(entity, createApplier(&CPrePassLightEntity::set_light_shaft_enable), property_type());
	}
	else
	{
		//创建对应的属性修改器
		return createPropertyModifier(entity, createApplier(&CPrePassLightEntity::set_light_shaft_enable), property_type());
	}
}

//体积光颜色属性和属性序列Modifer生成器
IPropertyModifier* LightShaftColorPropertyModifierCreator::create_entity_property_modifier(CMediaEndEntity* entity, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv)
{
	if (is_curv)
	{
		return createCompositeCurvPropertyModifierFloat(entity, createGetSetApplier(
			createApplier(&CPrePassLightEntity::set_light_shaft_color), 
			createGetter(&CPrePassLightEntity::get_light_shaft_color)), property_type());

//		return createCompositeCurvPropertyModifierFloat(entity, createApplier(&CPrePassLightEntity::set_light_shaft_color));
	}
	else
	{
		//创建对应的属性修改器
		return createPropertyModifier(entity, createApplier(&CPrePassLightEntity::set_light_shaft_color), property_type());
	}
}

//体积光强度属性和属性序列Modifer生成器
IPropertyModifier* LightShaftIntensityPropertyModifierCreator::create_entity_property_modifier(CMediaEndEntity* entity, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv)
{
	if (is_curv)
	{
		return createNormalCurvPropertyModifier(entity, 
			createApplier(&CPrePassLightEntity::set_light_shaft_intensity), property_type());
	}
	else
	{
		//创建对应的属性修改器
		return createPropertyModifier(entity, 
			createApplier(&CPrePassLightEntity::set_light_shaft_intensity), property_type());
	}
}

//体积光阀值属性和属性序列Modifer生成器
IPropertyModifier* LightShaftThresholdPropertyModifierCreator::create_entity_property_modifier(CMediaEndEntity* entity, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv)
{
	if (is_curv)
	{
		return createNormalCurvPropertyModifier(entity, 
			createApplier(&CPrePassLightEntity::set_light_shaft_threshold), property_type());
	}
	else
	{
		//创建对应的属性修改器
		return createPropertyModifier(entity, 
			createApplier(&CPrePassLightEntity::set_light_shaft_threshold), property_type());
	}
}

//体积光深度属性和属性序列Modifer生成器
IPropertyModifier* LightShaftDepthPropertyModifierCreator::create_entity_property_modifier(CMediaEndEntity* entity, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv)
{
	if (is_curv)
	{
		return createNormalCurvPropertyModifier(entity, 
			createApplier(&CPrePassLightEntity::set_light_shaft_depth), property_type());
	}
	else
	{
		//创建对应的属性修改器
		return createPropertyModifier(entity, createApplier(&CPrePassLightEntity::set_light_shaft_depth), property_type());
	}
}


////////////////////////////////////////////////////////////////////////////
////锥光源支持的属性和属性序列

//锥光源的内角度属性和属性序列Modifer生成器
IPropertyModifier* LightInnerAnglePropertyModifierCreator::create_entity_property_modifier(CMediaEndEntity* entity, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv)
{
	if (is_curv)
	{
		return createNormalCurvPropertyModifier(entity, 
			createApplier(&CWimblePrePassLightEntity::set_inner_angle), property_type());
	}
	else
	{
		//创建对应的属性修改器
		return createPropertyModifier(entity, 
			createApplier(&CWimblePrePassLightEntity::set_inner_angle), property_type());
	}
}

////锥光源的外角度属性和属性序列Modifer生成器
IPropertyModifier* LightOutterAnglePropertyModifierCreator::create_entity_property_modifier(CMediaEndEntity* entity, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv)
{
	if (is_curv)
	{
		return createNormalCurvPropertyModifier(entity, 
			createApplier(&CWimblePrePassLightEntity::set_outter_angle), property_type());
	}
	else
	{
		//创建对应的属性修改器
		return createPropertyModifier(entity, 
			createApplier(&CWimblePrePassLightEntity::set_outter_angle), property_type());
	}
}

////灯光投影启用属性和属性序列Modifer生成器
IPropertyModifier* LightProjTextureEnablePropertyModifierCreator::create_entity_property_modifier(CMediaEndEntity* entity, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv)
{
	if (is_curv)
	{
		return createNormalCurvPropertyModifier(entity, 
			createApplier(&CWimblePrePassLightEntity::enable_projected_texture), property_type());
	}
	else
	{
		//创建对应的属性修改器
		return createPropertyModifier(entity, 
			createApplier(&CWimblePrePassLightEntity::enable_projected_texture), property_type());
	}
}

//灯光投影贴图属性和属性序列Modifer生成器
IPropertyModifier* LightProjectTextureNamePropertyModifierCreator::create_entity_property_modifier(CMediaEndEntity* entity, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv)
{
	if (is_curv)
	{
		return createNormalCurvPropertyModifier(entity, 
			createApplier(&CWimblePrePassLightEntity::set_projected_texture), property_type());
	}
	else
	{
		//创建对应的属性修改器
		return createPropertyModifier(entity, 
			createApplier(&CWimblePrePassLightEntity::set_projected_texture), property_type());
	}
}

//光柱可见性属性和属性序列Modifer生成器
IPropertyModifier* LightBeamVisiblePropertyModifierCreator::create_entity_property_modifier(CMediaEndEntity* entity, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv)
{
	if (is_curv)
	{
		return createNormalCurvPropertyModifier(entity, 
			createApplier(&CWimblePrePassLightEntity::set_beam_visible), property_type());
	}
	else
	{
		//创建对应的属性修改器
		return createPropertyModifier(entity, 
			createApplier(&CWimblePrePassLightEntity::set_beam_visible), property_type());
	}
}

////光柱的亮度属性和属性序列Modifer生成器
IPropertyModifier* LightBeamBrightnessPropertyModifierCreator::create_entity_property_modifier(CMediaEndEntity* entity, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv)
{
	if (is_curv)
	{
		return createNormalCurvPropertyModifier(entity, 
			createApplier(&CWimblePrePassLightEntity::set_beam_brightness), property_type());
	}
	else
	{
		//创建对应的属性修改器
		return createPropertyModifier(entity, 
			createApplier(&CWimblePrePassLightEntity::set_beam_brightness), property_type());
	}
}

IPropertyModifier* LightBeamTypePropertyModifierCreator::create_entity_property_modifier(CMediaEndEntity* entity, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv)
{
	if (is_curv)
	{
		return createNormalCurvPropertyModifier(entity, 
			createApplier(&CWimblePrePassLightEntity::set_beam_type), property_type());
	}
	else
	{
		//创建对应的属性修改器
		return createPropertyModifier(entity, 
			createApplier(&CWimblePrePassLightEntity::set_beam_type), property_type());
	}
}

IPropertyModifier* LightBeamInnerRadiusPropertyModifierCreator::create_entity_property_modifier(CMediaEndEntity* entity, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv)
{
	if (is_curv)
	{
		return createNormalCurvPropertyModifier(entity, 
			createApplier(&CWimblePrePassLightEntity::set_beam_inner_radius), property_type());
	}
	else
	{
		//创建对应的属性修改器
		return createPropertyModifier(entity, 
			createApplier(&CWimblePrePassLightEntity::set_beam_inner_radius), property_type());
	}
}

//光柱的近范围属性和属性序列
IPropertyModifier* LightBeamNearRangePropertyModifierCreator::create_entity_property_modifier(CMediaEndEntity* entity, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv)
{
	if (is_curv)
	{
		return createNormalCurvPropertyModifier(entity, 
			createApplier(&CWimblePrePassLightEntity::set_beam_near_range), property_type());
	}
	else
	{
		//创建对应的属性修改器
		return createPropertyModifier(entity, 
			createApplier(&CWimblePrePassLightEntity::set_beam_near_range), property_type());
	}
}

//光柱的远范围属性和属性序列
IPropertyModifier* LightBeamFarRangePropertyModifierCreator::create_entity_property_modifier(CMediaEndEntity* entity, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv)
{
	if (is_curv)
	{
		return createNormalCurvPropertyModifier(entity, 
			createApplier(&CWimblePrePassLightEntity::set_beam_far_range), property_type());
	}
	else
	{
		//创建对应的属性修改器
		return createPropertyModifier(entity, 
			createApplier(&CWimblePrePassLightEntity::set_beam_far_range), property_type());
	}
}

//光柱的内角度属性和属性序列
IPropertyModifier* LightBeamInnerAnglePropertyModifierCreator::create_entity_property_modifier(CMediaEndEntity* entity, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv)
{
	if (is_curv)
	{
		return createNormalCurvPropertyModifier(entity, 
			createApplier(&CWimblePrePassLightEntity::set_beam_inner_angle), property_type());
	}
	else
	{
		//创建对应的属性修改器
		return createPropertyModifier(entity, 
			createApplier(&CWimblePrePassLightEntity::set_beam_inner_angle), property_type());
	}
}


//光柱的外角度属性和属性序列
IPropertyModifier* LightBeamOutterAnglePropertyModifierCreator::create_entity_property_modifier(CMediaEndEntity* entity, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv)
{
	if (is_curv)
	{
		return createNormalCurvPropertyModifier(entity, 
			createApplier(&CWimblePrePassLightEntity::set_beam_outter_angle), property_type());
	}
	else
	{
		//创建对应的属性修改器
		return createPropertyModifier(entity, 
			createApplier(&CWimblePrePassLightEntity::set_beam_outter_angle), property_type());
	}
}


IPropertyModifier* LightPostBeamBlurCoeffPropertyModifierCreator::create_entity_property_modifier(CMediaEndEntity* entity, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv)
{
	if (is_curv)
	{
		return createNormalCurvPropertyModifier(entity, 
			createApplier(&CWimblePrePassLightEntity::set_beam_blur_coeff), property_type());
	}
	else
	{
		//创建对应的属性修改器
		return createPropertyModifier(entity, 
			createApplier(&CWimblePrePassLightEntity::set_beam_blur_coeff), property_type());
	}
}

IPropertyModifier* FakeLightPostPropertyModifierCreator::create_entity_property_modifier(CMediaEndEntity* entity, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv)
{
	if (is_curv)
	{
		return createNormalCurvPropertyModifier(entity, 
			createApplier(&CPrePassLightEntity::record_fake_light_spot), property_type());
	}
	else
	{
		//创建对应的属性修改器
		return createPropertyModifier(entity, 
			createApplier(&CPrePassLightEntity::record_fake_light_spot), property_type());
	}
}

//////////////////////////////////////////////////////////////////////////
//////特效透明度属性和属性序列Modifer生成器
IPropertyModifier* EffectTransparencePropertyModifierCreator::create_entity_property_modifier(CMediaEndEntity* entity, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv)
{
	if (is_curv)
	{
		return createNormalCurvPropertyModifier(entity, 
			createApplier(&CEffectEntity::set_transparence), property_type());
	}
	else
	{
		//创建对应的属性修改器
		return createPropertyModifier(entity, 
			createApplier(&CEffectEntity::set_transparence), property_type());
	}
}

void play_effect(CEffectEntity* eff, bool value)
{
	if (value)
	{
		eff->play();
	}
	else
	{
		eff->stop();
	}
}

void play_effect_t(CEffectEntity* eff, TimeRelatedValueT<bool> value)
{
	if (value.val)
	{
		eff->play();
		if (is_greate_equal(value.delta_time, 0.0f))
		{
			eff->set_update_delta_time(value.delta_time);
		}
	}
	else
	{
		eff->stop();
	}
}

//////特效透明度属性和属性序列Modifer生成器
IPropertyModifier* EffectPlayPropertyModifierCreator::create_entity_property_modifier(CMediaEndEntity* entity, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv)
{
	if (is_curv)
	{
		//return createNormalCurvPropertyModifier(entity, createApplier(play_effect));
		return createNormalCurvPropertyModifierTimeRelated(entity, createApplier(play_effect_t), property_type());
	}
	else
	{
		//创建对应的属性修改器
		return createPropertyModifier(entity, createApplier(play_effect), property_type());
	}
}


//////系统背景光颜色属性和属性序列Modifer生成器
IPropertyModifier* SystemAmbientActorColorPropertyModifierCreator::create_entity_property_modifier(CMediaEndEntity* entity, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv)
{
	if (is_curv)
	{
		return createCompositeCurvPropertyModifierFloat(entity, 
			createGetSetApplier(createApplier(&CSystemEntity::set_ambient_actor_color), 
			createGetter(&CSystemEntity::get_ambient_actor_color)), property_type());
	}
	else
	{
		//创建对应的属性修改器
		return createPropertyModifier(entity, 
			createApplier(&CSystemEntity::set_ambient_actor_color), property_type());
	}
}


//////系统背景光颜色属性和属性序列Modifer生成器
IPropertyModifier* SystemAmbientColorPropertyModifierCreator::create_entity_property_modifier(CMediaEndEntity* entity, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv)
{
	if (is_curv)
	{
		return createCompositeCurvPropertyModifierFloat(entity, 
			createGetSetApplier(createApplier(&CSystemEntity::set_ambient_color), 
			createGetter(&CSystemEntity::get_ambient_color)), property_type());

//		return createCompositeCurvPropertyModifierFloat(entity, createApplier(&CSystemEntity::set_ambient_color));
	}
	else
	{
		//创建对应的属性修改器
		return createPropertyModifier(entity, createApplier(&CSystemEntity::set_ambient_color), property_type());
	}
}

///Lightmap影响因子A的颜色
IPropertyModifier* SystemLightMapColorFactorAPropertyModifierCreator::create_entity_property_modifier(CMediaEndEntity* entity, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv)
{
	if(is_curv)
	{
		return createCompositeCurvPropertyModifierFloat(entity, 
			createGetSetApplier(createApplier(&CLightmapAdjustEntity::set_lightmap_color_factor_a), 
			createGetter(&CLightmapAdjustEntity::get_lightmap_color_factor_a)), property_type());
	}
	else
	{
		return createPropertyModifier(entity, 
			createApplier(&CLightmapAdjustEntity::set_lightmap_color_factor_a), property_type());
	}
}

///Lightmap影响因子B的颜色
IPropertyModifier* SystemLightMapColorFactorBPropertyModifierCreator::create_entity_property_modifier(CMediaEndEntity* entity, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv)
{
	if(is_curv)
	{
		return createCompositeCurvPropertyModifierFloat(entity, 
			createGetSetApplier(createApplier(&CLightmapAdjustEntity::set_lightmap_color_factor_b), 
			createGetter(&CLightmapAdjustEntity::get_lightmap_color_factor_b)), property_type());
	}
	else
	{
		return createPropertyModifier(entity, 
			createApplier(&CLightmapAdjustEntity::set_lightmap_color_factor_b), property_type());
	}
}
///Lightmap影响因子A的颜色强度
IPropertyModifier* SystemLightMapColorFactorAIntensityPropertyModifierCreator::create_entity_property_modifier(CMediaEndEntity* entity, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv)
{
	if(is_curv)
	{
		return createNormalCurvPropertyModifier(entity, 
			createApplier(&CLightmapAdjustEntity::set_lightmap_color_factor_a_intensity), property_type());
	}
	else
	{
		return createPropertyModifier(entity, 
			createApplier(&CLightmapAdjustEntity::set_lightmap_color_factor_a_intensity), property_type());
	}
}
///Lightmap影响因子B的颜色强度
IPropertyModifier* SystemLightMapColorFactorBIntensityPropertyModifierCreator::create_entity_property_modifier(CMediaEndEntity* entity, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv)
{
	if(is_curv)
	{
		return createNormalCurvPropertyModifier(entity, 
			createApplier(&CLightmapAdjustEntity::set_lightmap_color_factor_b_intensity), property_type());
	}
	else
	{
		return createPropertyModifier(entity, 
			createApplier(&CLightmapAdjustEntity::set_lightmap_color_factor_b_intensity), property_type());
	}
}
/////相机fov是否根据客户端屏幕自动进行调整属性modifier生成器
IPropertyModifier* CameraFovAutoChangePropertyModifierCreator::create_entity_property_modifier(CMediaEndEntity* entity, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv)
{
	if (!is_curv)
	{
		return createPropertyModifier(entity, createApplier(&CCameraEntity::set_is_fov_auto_adjust), property_type());
	}
	return NULL;
}

IPropertyModifier *AvatarIsMalePropertyModifierCreator::create_entity_property_modifier(CMediaEndEntity* entity
																				  , const ActorPropertyType& type
																				  , const std::list<ModifierFixParaInfo>& fixed_params
																				  , bool is_curv)
{
	if (is_curv)
	{
		//创建对应的属性序列修改器;
		return createNormalCurvPropertyModifier(entity, createApplier(&AvatarEntity::change_sex), property_type());
	}
	else
	{
		//创建对应的属性修改器
		return createPropertyModifier(entity, createApplier(&AvatarEntity::change_sex), property_type());
	}
}

void _set_avatar_action_id_t(AvatarEntity* avatar, TimeRelatedValueT<std::string> value)
{
	avatar->set_action(ANIMATION_CHANNEL_ACTION, value.val.c_str(), avatar->getActionLoop());

	if (is_greate_equal(value.delta_time, 0.0f))
	{
		avatar->set_update_delta_time(value.delta_time);
	}
}

void _set_avatar_action_id(AvatarEntity* avatar, const std::string& value)
{
	avatar->set_action(ANIMATION_CHANNEL_ACTION, value.c_str(), avatar->getActionLoop());
}

void _set_avatar_action_loop(AvatarEntity* avatar, bool value)
{
	avatar->set_action(ANIMATION_CHANNEL_ACTION, avatar->getActionId(ANIMATION_CHANNEL_ACTION), value);
	avatar->set_next_action(ANIMATION_CHANNEL_ACTION, avatar->getActionId(ANIMATION_CHANNEL_ACTION), value);
}

IPropertyModifier *AvatarActionPropertyModifierCreator::create_entity_property_modifier(CMediaEndEntity* entity
																						, const ActorPropertyType& type
																						, const std::list<ModifierFixParaInfo>& fixed_params
																				  , bool is_curv)
{
	if (is_curv)
	{
		//创建对应的属性序列修改器;
		return createNormalCurvPropertyModifierTimeRelated(entity, 
			createApplier(_set_avatar_action_id_t), property_type());
	}
	else
	{
		//创建对应的属性修改器
		return createPropertyModifier(entity, createApplier(_set_avatar_action_id), property_type());
	}
}

IPropertyModifier *AvatarActionLoopPropertyModifierCreator::create_entity_property_modifier(CMediaEndEntity* entity
																						, const ActorPropertyType& type
																						, const std::list<ModifierFixParaInfo>& fixed_params
																				  , bool is_curv)
{
	if (is_curv)
	{
		//创建对应的属性序列修改器;
		return createNormalCurvPropertyModifier(entity, createApplier(_set_avatar_action_loop), property_type());
	}
	else
	{
		//创建对应的属性修改器
		return createPropertyModifier(entity, createApplier(_set_avatar_action_loop), property_type());
	}
}

void _split_string_to_vector( std::vector<std::string> &dest_vector, const std::string &source_string )
{
	dest_vector.clear();

	unsigned int ch_start = 0; //分割起始点
	unsigned int ch_length = 0; // 每次分割长度
	unsigned int ch_pos = 0; //分割符所在位置
	for(; ch_start < source_string.size(); ch_start += ch_length)
	{
		ch_pos = source_string.find_first_of(";", ch_start);//查找分号分割符所在位置

		if (ch_pos == source_string.npos)
		{
			dest_vector.push_back(source_string.substr(ch_start, source_string.size() - ch_start));
			ch_length = source_string.size() - ch_start;
		} 
		else
		{
			dest_vector.push_back(source_string.substr(ch_start, ch_pos - ch_start));
			ch_length = ch_pos - ch_start;
		}

		ch_length += 1; //分割符占1个字节
	}
}

/*
*@breif 从目标字符串里获取动作列表，目标字符串已“；”号为分割
*@param action_list 分割结果
*@param source_string 被分割对象
*/
void _get_acition_list(std::vector<std::string>& action_list, const std::string& source_string)
{
	_split_string_to_vector(action_list, source_string);
}

void _set_avatar_action_list(AvatarEntity* avatar, const std::string& value)
{
	std::vector<std::string> action_list;
	_get_acition_list(action_list, value);

	if (action_list.size() > 0 && stricmp(avatar->getActionId(ANIMATION_CHANNEL_ACTION), action_list[0].c_str()) != 0)
	{
		avatar->set_action(ANIMATION_CHANNEL_ACTION, action_list[0].c_str(), true);
	}
		
	if (action_list.size() >= 2)
	{
		avatar->set_next_action(ANIMATION_CHANNEL_ACTION, action_list[1].c_str(), true);
	}	
}

void _set_avatar_action_list_t(AvatarEntity* avatar, TimeRelatedValueT<std::string> value)
{
	std::vector<std::string> action_list;
	_get_acition_list(action_list, value.val);

	if (action_list.size() > 0 && stricmp(avatar->getActionId(ANIMATION_CHANNEL_ACTION), action_list[0].c_str()) != 0)
	{
		avatar->set_action(ANIMATION_CHANNEL_ACTION, action_list[0].c_str(), true);
	}

	if (action_list.size() >= 2)
	{
		avatar->set_next_action(ANIMATION_CHANNEL_ACTION, action_list[1].c_str(), true);
	}

	if (is_greate_equal(value.delta_time, 0.0f))
	{
		avatar->set_update_delta_time(value.delta_time);
	}
}


void _set_npc_action_list(CSkeletonModelEntity* npc, const std::string& value)
{
	std::vector<std::string> action_list;
	_get_acition_list(action_list, value);

	if (action_list.size() >= 0 && stricmp(npc->getActionId(ANIMATION_CHANNEL_ACTION), action_list[0].c_str()) != 0)
	{
		npc->set_action(ANIMATION_CHANNEL_ACTION, action_list[0].c_str(), true);
	}

	if (action_list.size() >= 2)
	{
		npc->set_next_action(ANIMATION_CHANNEL_ACTION, action_list[1].c_str(), true);
	}	
}

void _set_npc_action_list_t(CSkeletonModelEntity* npc, TimeRelatedValueT<std::string> value)
{
	std::vector<std::string> action_list;
	_get_acition_list(action_list, value.val);

	if (action_list.size() >= 0 && stricmp(npc->getActionId(ANIMATION_CHANNEL_ACTION), action_list[0].c_str()) != 0)
	{
		npc->set_action(ANIMATION_CHANNEL_ACTION, action_list[0].c_str(), true);
	}

	if (action_list.size() >= 2)
	{
		npc->set_next_action(ANIMATION_CHANNEL_ACTION, action_list[1].c_str(), true);
	}

	if (is_greate_equal(value.delta_time, 0.0f))
	{
		npc->set_update_delta_time(value.delta_time);
	}
}

IPropertyModifier *AvatarActionListPropertyModifierCreator::create_entity_property_modifier(AvatarEntity* entity
																							, const ActorPropertyType& type
																							, const std::list<ModifierFixParaInfo>& fixed_params
																							, bool is_curv)
{
	if (is_curv)
	{
		//创建对应的属性序列修改器;
		return createNormalCurvPropertyModifierTimeRelated(entity, createApplier(_set_avatar_action_list_t), property_type());
	}
	else
	{
		//创建对应的属性修改器
		return createPropertyModifier(entity, createApplier(_set_avatar_action_list), property_type());
	}
}

IPropertyModifier *AvatarActionListPropertyModifierCreator::create_entity_property_modifier(CMediaEndEntity* entity
																							, const ActorPropertyType& type
																							, const std::list<ModifierFixParaInfo>& fixed_params
																							, bool is_curv)
{
	AvatarEntity* a = dynamic_cast<AvatarEntity*>(entity);
	if (a != NULL)
	{
		return create_entity_property_modifier(a, type, fixed_params, is_curv);
	}
	else
	{
		CSkeletonModelEntity* s = dynamic_cast<CSkeletonModelEntity*>(entity);
		if (s != NULL)
		{
			return create_entity_property_modifier(s, type, fixed_params, is_curv);
		}
	}
	return NULL;
}

IPropertyModifier *AvatarActionListPropertyModifierCreator::create_entity_property_modifier(CSkeletonModelEntity* entity
																							, const ActorPropertyType& type
																							, const std::list<ModifierFixParaInfo>& fixed_params
																							, bool is_curv)
{
	if (is_curv)
	{
		//创建对应的属性序列修改器;
		return createNormalCurvPropertyModifierTimeRelated(entity, createApplier(_set_npc_action_list_t), property_type());
	}
	else
	{
		//创建对应的属性修改器
		return createPropertyModifier(entity, createApplier(_set_npc_action_list), property_type());
	}
}

IPropertyModifier* AvatarBodyPartPropertyModifierCreator::create_entity_property_modifier(CMediaEndEntity* entity
																						  , const ActorPropertyType& type
																						  , const std::list<ModifierFixParaInfo>& fixed_params
																						  , bool is_curv)
{
	if (is_curv)
	{
		//创建对应的属性序列修改器;
		return createNormalCurvPropertyModifier(entity, createApplier(&AvatarEntity::set_body_part_groups), property_type());
	}
	else
	{
		//创建对应的属性修改器
		return createPropertyModifier(entity, createApplier(&AvatarEntity::set_body_part_groups), property_type());
	}
}

void _set_rotation_by_z(C3DEntity* entity, float value)
{
	entity->set_rotation(H3DVec3(0.0f, 0.0f, DEG2RAD(value)));
}

IPropertyModifier* ZRotationByDegreePropertyModifierCreator::create_entity_property_modifier(CMediaEndEntity* entity, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv)
{
	if (is_curv)
	{
		//创建对应的属性序列修改器;
		return createNormalCurvPropertyModifier(entity, createApplier(_set_rotation_by_z), property_type());
	}
	else
	{
		//创建对应的属性修改器
		return createPropertyModifier(entity, createApplier(_set_rotation_by_z), property_type());
	}
}

IPropertyModifier *ReciveFakeLightSpotPropertyModifierCreator::create_entity_property_modifier(CMediaEndEntity* entity
																						, const ActorPropertyType& type
																						, const std::list<ModifierFixParaInfo>& fixed_params
																				  , bool is_curv)
{
	if (is_curv)
	{
		//创建对应的属性序列修改器;
		return createNormalCurvPropertyModifier(entity, 
			createApplier(&CModelEntity::record_recive_fakelightspot), property_type());
	}
	else
	{
		//创建对应的属性修改器
		return createPropertyModifier(entity, 
			createApplier(&CModelEntity::record_recive_fakelightspot), property_type());
	}
}
//
//void play_sound(CSoundEntity* eff, bool value)
//{
//	if (value)
//	{
//		eff->play();
//	}
//	else
//	{
//		eff->stop();
//	}
//}
//
//IPropertyModifier* SoundPlayPropertyModifierCreator::create_entity_property_modifier(CMediaEndEntity* entity, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv)
//{
//	if (is_curv)
//	{
//		return createNormalCurvPropertyModifier(entity, createApplier(play_sound), property_type());
//	}
//	else
//	{
//		//创建对应的属性修改器
//		return createPropertyModifier(entity, createApplier(play_sound), property_type());
//	}
//}
//
//IPropertyModifier* SoundResourcePropertyModifierCreator::create_entity_property_modifier(CMediaEndEntity* entity, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv)
//{
//	if (is_curv)
//	{
//		//创建对应的属性序列修改器;
//		return createNormalCurvPropertyModifier(entity, createApplier(&CSoundEntity::change_resource), property_type());
//	}
//	else
//	{
//		//创建对应的属性修改器
//		return createPropertyModifier(entity, createApplier(&CSoundEntity::change_resource), property_type());
//	}
//}
void _set_skeleton_model_transition_time_from_current(CSkeletonModelEntity* skeleton_model, float fTime)
{
	skeleton_model->set_transition_time_from_current(ANIMATION_CHANNEL_ACTION, fTime);
}
IPropertyModifier* TransitionTimeFromCurrentPropertyModifierCreator::create_entity_property_modifier(CMediaEndEntity* entity, 
	const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv)
{
	if (is_curv)
	{
		return createNormalCurvPropertyModifier(entity, 
			createApplier(_set_skeleton_model_transition_time_from_current), property_type());
	}
	else
	{
		return createPropertyModifier(entity, 
			createApplier(_set_skeleton_model_transition_time_from_current), property_type());
	}
}
void _set_skeleton_model_transition_time_to_next(CSkeletonModelEntity* skeleton_model, float fTime)
{
	skeleton_model->set_transition_time_to_next(ANIMATION_CHANNEL_ACTION, fTime);
}
IPropertyModifier* TransitionTimeToNextPropertyModifierCreator::create_entity_property_modifier(CMediaEndEntity* entity, 
	const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv)
{
	if (is_curv)
	{
		return createNormalCurvPropertyModifier(entity, 
			createApplier(_set_skeleton_model_transition_time_to_next), property_type());
	}
	else
	{
		return createPropertyModifier(entity, 
			createApplier(_set_skeleton_model_transition_time_to_next), property_type());
	}
}
//lightmap阴影指数
IPropertyModifier* LightMapShadowIndexPropertyModifierCreator::create_entity_property_modifier(CMediaEndEntity* entity, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv)
{
	if (is_curv)
	{
		return createNormalCurvPropertyModifier(entity, 
			createApplier(&CPrePassLightEntity::set_lightmap_shadow_index), property_type());
	}
	else
	{
		//创建对应的属性修改器
		return createPropertyModifier(entity, 
			createApplier(&CPrePassLightEntity::set_lightmap_shadow_index), property_type());
	}
}

//lightmap光源半径
IPropertyModifier* LightMapRadiusPropertyModifierCreator::create_entity_property_modifier(CMediaEndEntity* entity, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv)
{
	if (is_curv)
	{
		return createNormalCurvPropertyModifier(entity, 
			createApplier(&CPrePassLightEntity::set_lightmap_radius), property_type());
	}
	else
	{
		//创建对应的属性修改器
		return createPropertyModifier(entity, 
			createApplier(&CPrePassLightEntity::set_lightmap_radius), property_type());
	}
}

//////////////////////////////////////////////////////////////////////////
//useless,这只不过是个trick
//inline std::string operator - (const std::string& first_value, const std::string& second_value)
//{
//	return first_value;
//}
//
//inline std::string operator + (const std::string& first_value, const std::string& second_value)
//{
//	return first_value;
//}
//
//template<typename T>
//inline std::string operator * (const std::string& first_value, T second_value)
//{
//	return first_value;
//}
//
//inline std::string operator / (const std::string& first_value, const std::string& second_value)
//{
//	return first_value;
//}
inline std::string operator + (const std::string& first_value, const std::string& second_value)
{
	return first_value;
}

template<typename T>
inline H3DI::EH3DBEAMTYPE operator * (const H3DI::EH3DBEAMTYPE& first_value, T second_value)
{
	return first_value;
}

inline H3DI::EH3DBEAMTYPE operator - (const H3DI::EH3DBEAMTYPE& first_value, const H3DI::EH3DBEAMTYPE& second_value)
{
	return first_value;
}

inline H3DI::EH3DBEAMTYPE operator + (const H3DI::EH3DBEAMTYPE& first_value, const H3DI::EH3DBEAMTYPE& second_value)
{
	return first_value;
}


template<typename T>
inline H3DI::LightAffectParam operator * (const H3DI::LightAffectParam& first_value, T second_value)
{
	return first_value;
}

inline H3DI::LightAffectParam operator - (const H3DI::LightAffectParam& first_value, const H3DI::LightAffectParam& second_value)
{
	return first_value;
}

inline H3DI::LightAffectParam operator + (const H3DI::LightAffectParam& first_value, const H3DI::LightAffectParam& second_value)
{
	return first_value;
}

inline H3DVec3_2 operator * (const H3DVec3_2& first_value, const float& second_value)
{
	H3DVec3_2 vec;
	vec.m_position = first_value.m_position * second_value;
	vec.m_tangent  = first_value.m_tangent * second_value;
	return vec;
}

inline H3DVec3_2 operator - (const H3DVec3_2& first_value, const H3DVec3_2& second_value)
{
	H3DVec3_2 vec;
	vec.m_position = first_value.m_position - second_value.m_position;
	vec.m_tangent = first_value.m_tangent - second_value.m_tangent;
	return vec;
}

inline H3DVec3_2 operator + (const H3DVec3_2& first_value, const H3DVec3_2& second_value)
{
	H3DVec3_2 vec;
	vec.m_position = first_value.m_position + second_value.m_position;
	vec.m_tangent = first_value.m_tangent + second_value.m_tangent;
	return vec;
}
