/**
* @file dynamic_effect_object.h
* ����ļ�����Ҫ�����ǣ�
* 
* @author ����
* 
* ������־
* ----------------------------------------------------
* ver 1.0.0
*       by ����2010.11.16
*/
#include "middle_layer_body.h"
/// ModifierTypeID�ıȽ���
extern bool operator<(const PropertyModifierType& a, const PropertyModifierType& b);

//ȡdefault property clip����
const char* DynamicEffectObject::getDefaultPropertyName()
{
	return "__default_property_clip";
};
//ȡstage property clip����
const char* DynamicEffectObject::getStagePropertyName()
{
	return "__stage_property_clip";	
};
//ȫ��ϵͳ��������clip��ȫ�ֲ������״̬����
//DynamicEffectObject* DynamicEffectObject::s_system_object = NULL;

DynamicEffectObject::DynamicEffectObject(ActorManager* actor_manager, const char* name, const char* defined_type)
:Actor(actor_manager,name)
{
	m_defined_type = defined_type;
	//��ӿ�����
	m_main_ani_control = new DynEffObjControl(DynEffObjControl::getMainControlName());
	add_actor_control(m_main_ani_control);

	//���ȱʡ����clip
	ActorAnimationClip* default_ani_clip = new ActorAnimationClip();
	m_main_ani_control->add_animation_clip(getDefaultPropertyName(), default_ani_clip);
	//���stage����clip
	ActorAnimationClip* stage_ani_clip = new ActorAnimationClip();
	m_main_ani_control->add_animation_clip(getStagePropertyName(), stage_ani_clip);
	
	resetSubObjectIterator();

	m_object_updated = false;
};

DynamicEffectObject::~DynamicEffectObject()
{
	
};

Actor* DynamicEffectObject::find_actor_recur(const char* id)
{
	ActorID actor_id = "";
	ActorID subactor_id = id;
	int begin_index = 1;
	Actor* actor = this;
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

//ȡԤ��������
const char* DynamicEffectObject::getDefinedType()
{
	return m_defined_type.c_str();	
};
static bool createModifierRecursive(DynamicEffectObject* dynamic_effect_object,ActorAnimationClip* ani_clip,PropertyModifierFactory* modifier_factory, const DefinedItem* defined_item)
{
	std::list<DefinedItem::ValueItem*>::const_iterator iter = defined_item->m_property_list.begin();
	for(; iter != defined_item->m_property_list.end(); ++ iter)
	{
		assert(!(*iter)->m_property_modifier_type.is_curv);//ȱʡ����������
		if(!(*iter)->m_property_support.empty())
		{
			if((*iter)->m_property_support == "curve_only")//ֻ֧�����ߵĲ�����modifier
				continue;
		}
		//����modifier
		IPropertyModifier* modifier = modifier_factory->create_modifier(dynamic_effect_object,(*iter)->m_property_modifier_type);
		if(modifier)
		{
			IPropertyModifierStreamable* s_modifier = dynamic_cast<IPropertyModifierStreamable*>(modifier);
			assert(s_modifier);
			s_modifier->set_value((*iter)->m_value.c_str());//��������ֵ
			s_modifier->execute(true,0.0f);
			if(!ani_clip->add_property_modifier((*iter)->m_property_modifier_type, modifier))
			{
				modifier_factory->releaseModifier(modifier);
				return false;
			}
		}
		else
		{
			return false;
		}		
	}
	if(defined_item->m_node_type == DefinedItem::NORMAL_ENTITY)
		return true;
	else if(defined_item->m_node_type == DefinedItem::DEFINED_EFFECT_OBJECT || defined_item->m_node_type == DefinedItem::NORMAL_EFFECT_OBJECT)
	{
		std::list<DefinedItem*>::const_iterator sub_item_iter =  defined_item->m_sub_item.begin();
		for(; sub_item_iter != defined_item->m_sub_item.end(); ++ sub_item_iter)
		{
			if(!createModifierRecursive(dynamic_effect_object,ani_clip, modifier_factory, *sub_item_iter))
				return false;
		}
	}
	else
		assert(0);
	return true;
}
bool DynamicEffectObject::createAnimationClipByItem(const char* clip_name,const DefinedItem* defined_item)
{
	assert(defined_item);
	//����obj�� ani_control
	ActorAnimationControl* ani_control = getAnimationControl();
	assert(ani_control);
	//������StageAniClipͬ���� ActorAniClip
	ActorAnimationClip* ani_clip = 	ani_control->find_animation_clip(clip_name);
	assert(ani_clip);
	//��������
	if(!createModifierRecursive(this,ani_clip, m_actor_manager->getPropertyModifierFactory(),  defined_item ))
		return false;

	return true;
};
//��Ӽ�������
bool DynamicEffectObject::addSubObject(DynamicEffectObject* sub_obj)
{
	for(std::list<DynamicEffectObject*>::const_iterator iter = sub_object_list.begin();
		iter != sub_object_list.end();  ++ iter){
		if((*iter) == sub_obj)
			return false;//�Ѿ������
	}

	//��ӵ�actor�б�

	const H3DMat4& mat = this->get_world_location_matrix().Inverse() * sub_obj->get_world_location_matrix();
	add_actor(sub_obj);
	sub_obj->set_location_matrix(mat);

	sub_object_list.push_back(sub_obj);
	resetSubObjectIterator();
	return true;
};
//�Ƴ���������
bool DynamicEffectObject::removeSubObject(DynamicEffectObject* sub_obj)
{
	std::list<DynamicEffectObject*>::iterator iter = sub_object_list.begin();
	for( ; iter != sub_object_list.end();  ){
		if((*iter) == sub_obj )
		{
			//��actor�б����Ƴ�
			const H3DMat4& mat = sub_obj->get_world_location_matrix();
			remove_actor(sub_obj);
			sub_obj->set_location_matrix(mat);

			sub_object_list.erase(iter++);
			
			resetSubObjectIterator();
			return true;
		}
		else
			++iter;
	}
	return false;
};
//���ü������������
void DynamicEffectObject::resetSubObjectIterator()
{
	sub_object_iter = sub_object_list.begin();
};
//ȡ��������
DynamicEffectObject* DynamicEffectObject::nextSubObject()
{
	if(sub_object_iter != sub_object_list.end())
		return *(sub_object_iter++);
	else
		return 0;
};

void DynamicEffectObject::reload()
{
	reset_entity_iter();
	CMediaEndEntity* entity = next_entity();
	while (entity != NULL)
	{
		if (entity->is_kind_of(MEDIA_END_ENTITY_TYPE_3D_ENTITY))
		{
			C3DEntity* move_entity = dynamic_cast<C3DEntity*>(entity);
			move_entity->reload();
		}
		entity = next_entity();
	}

	reset_actor_iter();
	Actor* actor = next_actor();
	while (actor != NULL)
	{
		DynamicEffectObject* dyn_obj = dynamic_cast<DynamicEffectObject*>(actor);
		dyn_obj->reload();
	}
}
bool DynamicEffectObject::remove_entity(CMediaEndEntity* entity)
{
	CMediaEndEntity* p = find_entity(entity->get_name());
	if (p == entity)
	{
		//��ɾ����Ӧ�Ķ���������
		m_main_ani_control->reset_iterator();
		while(ActorAnimationClip* p = m_main_ani_control->next())
		{
			p->remove_property_modifier_by_entity(entity->get_name());
		}
		m_media_end_entity_collection.remove(entity);
		entity->set_actor(NULL);
		reset_entity_iter();
		return true;
	}
	else
	{
		return false;
	}
}
//������嶨��
CameraEffectObject::CameraEffectObject(ActorManager* actor_manager, const char* name, const char* defined_type)
:DynamicEffectObject(actor_manager,name,defined_type)
{
	
};
CameraEffectObject::~CameraEffectObject()
{
	
};

//��ȡ��ǰ�����Ϣ
void CameraEffectObject::getCameraInfo(CameraInfo& info)
{
	CCameraEntity* cam = getCamera();
	assert(NULL != cam);

	H3DVec3 dir = cam->direction();
	info.dir_x = dir[0]; info.dir_y = dir[1]; info.dir_z = dir[2];

	H3DVec3 pos = cam->get_cam_position();
	info.pos_x = pos[0]; info.pos_y = pos[1]; info.pos_z = pos[2];

	H3DVec3 target = cam->get_cam_lookat();
	info.target_x = target[0]; info.target_y = target[1]; info.target_z = target[2];

	H3DVec3 up = cam->up_direction();
	info.up_x = up[0]; info.up_y = up[1]; info.up_z = up[2];

	info.confusion = cam->get_confusion();
	info.dof_range = cam->get_dof_range();
	info.focalplane = cam->get_focalplane();

	cam->get_render_frustum(&info.fov, &info.ratio, &info.fNear, &info.fFar);
};

//��ȡ��ǰ���������Ϣ
const CameraEffectObject::PostProcessCollection& CameraEffectObject::getCameraPostParam()
{
	CCameraEntity* camera = getCamera();
	
	assert(NULL != camera);
	return camera->get_postprocess_param();
};

void CameraEffectObject::disableCameraPostprocess(const char* postprocess_name)
{
	CCameraEntity* camera = getCamera();
	assert(NULL != camera);
	camera->disable_camera_postprocess(postprocess_name);
}

void CameraEffectObject::setCameraPostProcessEnable(const char* post_process_name, bool enable)
{
	CCameraEntity* camera = getCamera();
	assert(NULL != camera);
	camera->set_camera_postprocess_enable(post_process_name, enable);
}

CCameraEntity* CameraEffectObject::getCamera()
{
	reset_entity_iter();

	CMediaEndEntity* entity = next_entity();
	while (NULL != entity)
	{
		if (entity->is_kind_of(MEDIA_END_ENTITY_TYPE_CAMERA))
		{
			return dynamic_cast<CCameraEntity*>(entity);
		}

		entity = next_entity();
	}

	return NULL;
}

DynEffObjControl::DynEffObjControl(const char* name)
:ActorAnimationControl(name)
{
	
};
DynEffObjControl::~DynEffObjControl()
{
	
};
//ȡdefault property clip����
const char* DynEffObjControl::getMainControlName()
{
	return "__main_control";
};

//������ʼ����״̬
void DynEffObjControl::initPlayData()
{
	clearPlayData();
	//��D��S�ϲ����õ������ܵ�ȱʡ���Լ��� P = S + (D-S) ; P�а���ͳһ��ID�� modifierӳ�䣬���ܰ���default �� stage modifier.
	//����clip
	ActorAnimationClip* default_property_ani_clip = find_animation_clip(DynamicEffectObject::getDefaultPropertyName());
	default_property_ani_clip->reset_iterator();

	ActorAnimationClip* stage_property_ani_clip = find_animation_clip(DynamicEffectObject::getStagePropertyName());
	stage_property_ani_clip->reset_iterator();
	// ��������޸�����������ָ����һ�������޸��������������Ч����0
	PropertyModifierType* modifier_type = NULL;
	while(IPropertyModifier* property_modifier = default_property_ani_clip->next(&modifier_type))
	{
		if(IPropertyModifier* stage_property_modifier = stage_property_ani_clip->find_property_modifier(*modifier_type))//���������Ƿ���stage����
		{//��stage����
			m_default_modifier_map.insert(std::pair<PropertyModifierType, IPropertyModifier*>(*modifier_type, stage_property_modifier));//����stage modifier
			stage_property_modifier->execute(true,0.0f);//����ȱʡֵ
		}
		else
		{
			m_default_modifier_map.insert(std::pair<PropertyModifierType, IPropertyModifier*>(*modifier_type, property_modifier));//����default modifier
			property_modifier->execute(true,0.0f);//����ȱʡֵ
		}
	}
	//�����ʱ��¼
	m_reset_modifier_map.clear();
	m_last_set_type_temp.clear();
	m_last_cutting_data = NULL;
};
//�������״̬
void DynEffObjControl::clearPlayData()
{
	//�ڶ�ű�����ʱ�����������������������ÿ���л��ű�ʱҪ�����
	m_default_modifier_map.clear();
	//m_opt_info_map.clear();

	m_reset_modifier_map.clear();
	m_last_set_type_temp.clear();
	m_last_cutting_data = NULL;
};

//void DynEffObjControl::clearPlayDataForScriptSwitch()
//{
//	m_reset_modifier_map.clear();
//	m_last_set_type_temp.clear();
//	m_last_cutting_data = NULL;
//}

//���и�λ���Լ���
void DynEffObjControl::calcResetModifier(const char* cutting_clipId,const char* next_clipId)
{
	//����clip
	ActorAnimationClip* actor_ani_clip = find_animation_clip(cutting_clipId); //���ұ��ε�clip
	assert(actor_ani_clip);

	if(cutting_clipId)//���״μ�¼
	{
		m_last_set_type_temp.clear();//�����¼
		//ȡ�����������ͣ���Ӽ�¼
		actor_ani_clip->reset_iterator();
		PropertyModifierType* type_temp = NULL;
		while( IPropertyModifier* modifier = actor_ani_clip->next(&type_temp))
		{
			m_last_set_type_temp.insert(type_temp);//��¼���θ��ĵ�����
		}

		std::string nextClipId;
		//�ж��Ƿ������ڵ�clip
		if(next_clipId)//
		{
			nextClipId = next_clipId;
		}
		ActorAnimationClip* nextAnimationClip = find_animation_clip(nextClipId.c_str());

		if(m_reset_modifier_map.find(std::make_pair<std::string,std::string>(cutting_clipId,nextClipId)) == m_reset_modifier_map.end())
		{
			//���һ���µ�set
			m_reset_modifier_map.insert(std::pair<std::pair<std::string,std::string>,std::set<IPropertyModifier*>>(std::make_pair<std::string,std::string>(cutting_clipId,nextClipId), std::set<IPropertyModifier*>()));
			//ȡ����ӵ�set����
			std::set<IPropertyModifier*>& ref_last_reset_modifier_set = m_reset_modifier_map[std::make_pair<std::string,std::string>(cutting_clipId,nextClipId)];

			//ȡ�ϴ����õ�����
			std::set<PropertyModifierType*>::const_iterator last_set_type_temp_iter =  m_last_set_type_temp.begin();
			for(; last_set_type_temp_iter != m_last_set_type_temp.end(); ++ last_set_type_temp_iter)
			{
				if(!nextAnimationClip || nextAnimationClip->find_property_modifier(**last_set_type_temp_iter)== NULL)//�м���򱾴β����õģ� ��¼default modifier; û�Ƚ� curve .lishan 110107
				{
					//�����ϴ����ö�����δ���õ�����.��¼ȱʡ����modifier
					std::map<PropertyModifierType, IPropertyModifier*, ModifierTypeFunctorCurveInsensitive>::const_iterator map_iter = m_default_modifier_map.find(**last_set_type_temp_iter);
					//ƽ�й� dl position ȱʡû�У�clip�У�Ŀǰ�޷��ָ� lishan 110107
					if(map_iter != m_default_modifier_map.end())
					{
						assert(map_iter != m_default_modifier_map.end());
						ref_last_reset_modifier_set.insert(map_iter->second);//��¼���ָ���modifier
					}
				}
			}
		}
	}
};
//��λ���Լ������
void DynEffObjControl::endCalcReset()
{
	//�����1��clip�Ļָ�set
	if(m_last_cutting_data !=  NULL)//���������
	{
		//���һ���µ�set
		m_reset_modifier_map.insert(std::pair< std::pair<std::string,std::string>, std::set<IPropertyModifier*> >(std::make_pair<std::string,std::string>(m_last_cutting_data->m_animation_clip_id,""), std::set<IPropertyModifier*>()));
		//ȡ����ӵ�set����
		std::set<IPropertyModifier*>& ref_last_reset_modifier_set = m_reset_modifier_map[std::make_pair<std::string,std::string>(m_last_cutting_data->m_animation_clip_id,"")];

		//ȡ�ϴ����õ�����
		std::set<PropertyModifierType*>::const_iterator last_set_type_temp_iter =  m_last_set_type_temp.begin();
		for(; last_set_type_temp_iter != m_last_set_type_temp.end(); ++last_set_type_temp_iter)
		{
			//�����ϴ����ö�����δ���õ�����.��¼ȱʡ����modifier
			std::map<PropertyModifierType, IPropertyModifier*, ModifierTypeFunctorCurveInsensitive>::const_iterator map_iter = m_default_modifier_map.find(**last_set_type_temp_iter);
			if(map_iter != m_default_modifier_map.end())//Ŀǰ�ж�Ӧ���ϵ�modifier, dl position lishan 
			{
				assert(map_iter != m_default_modifier_map.end());
				ref_last_reset_modifier_set.insert(map_iter->second);//��¼���ָ���modifier
			}
		}
		m_last_set_type_temp.clear();//�����¼
		m_last_cutting_data =  NULL;
	}
};

//void DynEffObjControl::calcOptRestModifier(StageAnimationCuttingData* first_cutting_data 
//										   , StageAnimationCuttingData* next_cutting_data)
//{
//	std::pair<StageAnimationCuttingData*, StageAnimationCuttingData*> p = make_pair(first_cutting_data, next_cutting_data);
//
//	if (m_opt_info_map.find(p) != m_opt_info_map.end())//����Ƿ��Ѿ����˸�cutting�Ե��Ż�
//		return;
//
//	ActorAnimationClip* first_clip = find_animation_clip(first_cutting_data->m_animation_clip_id.c_str()); //������һ�ű���clip
//	ActorAnimationClip* next_clip = find_animation_clip(next_cutting_data->m_animation_clip_id.c_str()); //���ҵڶ��ű���clip
//	assert(first_clip);
//	assert(next_clip);
//
//	std::set<PropertyModifierType*> first_set_type;
//	//ȡ ��һ�ű���clip����Ϣ
//	_getClipTypeSet(first_set_type, first_clip);
//
//	//�������Ż���Ϣ��Ŀǰ�ǿյ�
//	m_opt_info_map.insert(make_pair(p, std::set<IPropertyModifier*>()));
//	//��ȡ���Ż���Ϣ��value������
//	std::set<IPropertyModifier*>& ref_set = m_opt_info_map[p];
//
//	//ִ���Ż�����
//	_iterAndCalcRestModifier(true, first_set_type, next_clip, ref_set);
//}

//void DynEffObjControl::_insertResModifier( std::set<PropertyModifierType*>::const_iterator last_set_type_iter
//										  , std::set<IPropertyModifier*>& ref_set )
//{
//	//�����ϴ����ö�����δ���õ�����.��¼ȱʡ����modifier
//	std::map<PropertyModifierType, IPropertyModifier*, ModifierTypeFunctorCurveInsensitive>::const_iterator map_iter = m_default_modifier_map.find(**last_set_type_iter);
//	if(map_iter != m_default_modifier_map.end())//Ŀǰ�ж�Ӧ���ϵ�modifier, dl position lishan 
//	{
//		assert(map_iter != m_default_modifier_map.end());
//		ref_set.insert(map_iter->second);//��¼���ָ���modifier
//	}
//}

//void DynEffObjControl::_iterAndCalcRestModifier(bool is_side_by_side,  std::set<PropertyModifierType*> &set_type
//												   , ActorAnimationClip* next_clip
//												   , std::set<IPropertyModifier*>& ref_set )
//{
//	std::set<PropertyModifierType*>::const_iterator set_type_iter =  set_type.begin();
//	for(; set_type_iter != set_type.end(); ++set_type_iter)
//	{
//		if (!is_side_by_side|| next_clip->find_property_modifier(**set_type_iter)== NULL) //�õڶ��ű����������һ�ű����бȽ�
//		{
//			_insertResModifier(set_type_iter, ref_set);
//		}
//	}
//}

//void DynEffObjControl::_getClipTypeSet(std::set<PropertyModifierType*>& type_set, ActorAnimationClip* first_clip )
//{
//	first_clip->reset_iterator();
//	PropertyModifierType* type_temp = NULL;
//	while( IPropertyModifier* modifier = first_clip->next(&type_temp))
//	{
//		type_set.insert(type_temp);//��¼���θ��ĵ�����
//	}
//}

FollowLightDynamicEffectObject::FollowLightDynamicEffectObject(ActorManager* actor_manager, const char* name, const char* defined_type)
: DynamicEffectObject(actor_manager, name, defined_type)
{
	m_target_position = NULL;
}

FollowLightDynamicEffectObject::~FollowLightDynamicEffectObject()
{
	delete m_target_position;
}

void FollowLightDynamicEffectObject::update(float t)
{
	DynamicEffectObject::update(t);
	update_location(t);
}

void FollowLightDynamicEffectObject::set_follow_type(const char* follow_type)
{
	assert(NULL != follow_type);
	if (stricmp(follow_type, m_cur_follow_type.c_str()) == 0)
	{
		return ;
	}

	delete m_target_position;
	m_target_position = NULL;
	m_cur_follow_type = "";

	m_target_position = create_target_position_calculator(follow_type);
	if (NULL == m_target_position)
	{
		return;
	}

	m_cur_follow_type = follow_type;
}

CPrePassLightEntity* FollowLightDynamicEffectObject::get_light()
{
	reset_entity_iter();
	CMediaEndEntity* entity = next_entity();
	while (NULL != entity)
	{
		if (entity->is_kind_of(MEDIA_END_ENTITY_TYPE_PREPASS_LIGHT))
		{
			return dynamic_cast<CPrePassLightEntity*>(entity);
		}

		entity = next_entity();
	}

	return NULL;
}

ITargetPosition* FollowLightDynamicEffectObject::create_target_position_calculator(const char* calculator_type)
{
	if ((NULL == calculator_type) || (strlen(calculator_type) == 0))
	{
		return NULL;
	}

	if (stricmp("SingeTarget", calculator_type) == 0)
	{
		return new CSingleTargetPostion(m_actor_manager);
	}

	return NULL;
}

void FollowLightDynamicEffectObject::update_location(float t)
{
	if (NULL == m_target_position)
	{
		return;
	}
	
	CPrePassLightEntity* light = get_light();
	H3DVec3 light_pos;
	H3DVec3 light_scale;
	H3DQuat light_quat;
	H3DMat4 light_loc = light->get_world_location_matrix();
	light_loc.Decompose(light_scale, light_quat, light_pos);

	H3DVec3 target_pos = m_target_position->get_target_position();
	H3DVec3 new_dir = light_pos - target_pos;
	H3DQuat new_quat;
	new_quat.GetRotationTo(H3DVec3(0.0f,-1.0f,0.0f), new_dir);

	H3DMat4 new_light_location;
	new_light_location.Compose(light_scale, new_quat, light_pos);
	H3DMat4 trans_mat = new_light_location * light_loc.Inverse();

	set_location_matrix(trans_mat * get_location_matrix());
}

void FollowLightDynamicEffectObject::set_target_info(const char* info_name, const char* info_value)
{
	if (NULL != m_target_position)
	{
		m_target_position->set_info(info_name, info_value);
	}
}

void FollowLightDynamicEffectObject::set_target(Actor* actor)
{
	if (NULL != m_target_position)
	{
		dynamic_cast<CSingleTargetPostion*>(m_target_position)->set_target(actor);
	}
}

CSingleTargetPostion::CSingleTargetPostion(ActorManager* actor_mng)
{
	m_actor_mng = actor_mng;
	m_target = NULL;
	m_follow_actor_bone = false;
}

H3DVec3 CSingleTargetPostion::get_target_position()
{
	if (NULL != m_target)
	{
		if (!m_follow_actor_bone)
		{
			return m_target->position();
		}
		else
		{
			H3DI::ISkeletonModel* skeleton_model = dynamic_cast<H3DI::ISkeletonModel*>(m_target->get_IMoveObject());
			if (NULL != skeleton_model)
			{
				H3DI::ISkeleton* skeleton = skeleton_model->GetSkeleton();
				if (skeleton)
				{
					H3DI::IBone* bone = skeleton->GetBone("Bip01");
					assert(bone);

					const H3DMat4& mat = skeleton_model->GetRendingMatrix(bone->GetID());
					H3DVec3 offset;
					mat.GetTranslation(offset);
					return offset;
				}
			}
		}
	}
	
	return H3DVec3(0.0f, 0.0f, 0.0f);
}

void CSingleTargetPostion::set_target(Actor* actor)
{
	m_target = NULL;
	if (NULL == actor)
		return;

	actor->reset_actor_iter();
	while (CMediaEndEntity* entity = actor->next_entity())
	{
		C3DEntity* target = dynamic_cast<C3DEntity*>(entity);
		if (target)
		{
			if (dynamic_cast<H3DI::ISkeletonModel*>(target->get_IMoveObject()))
			{
				m_target = target;
				break;
			}
		}
	}
}

void CSingleTargetPostion::set_info(const char* info_name, const char* info_value)
{
	assert(NULL != info_name);
	assert(strlen(info_name) != 0);

	BaseTargetPosition::set_info(info_name, info_value);

	if (stricmp(info_name, "target") == 0)
	{
		Actor* actor = m_actor_mng->find_actor(info_value);
		set_target(actor);
	}
	else if (stricmp(info_name, "follow_bone") == 0)
	{
		m_follow_actor_bone = stricmp(info_value, "true") == 0;
	}
}


//////////////////////////////////////////////////////////////////////////
void set_light_follow_type(FollowLightDynamicEffectObject* light, const std::string& follow_type)
{
	light->set_follow_type(follow_type.c_str());
}

//׷���������Ժ���������Modifier������
IPropertyModifier* FollowTypePropertyModifierCreator::create_actor_property_modifier(Actor* actor, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv)
{
	if (is_curv)
	{
		return createNormalCurvPropertyModifier(actor, createApplier(set_light_follow_type), property_type());
	}
	else
	{
		return createPropertyModifier(actor, createApplier(set_light_follow_type), property_type());
	}
}

void set_light_target_info(FollowLightDynamicEffectObject* light, const std::string& info_name, const std::string& info_value)
{
	light->set_target_info(info_name.c_str(), info_value.c_str());
}

//׷���������Ժ���������Modifier������
IPropertyModifier* FollowLightTargetInfoPropertyModifierCreator::create_actor_property_modifier(Actor* actor, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv)
{
	std::string str = *(fixed_params.begin());
	if (is_curv)
	{
		return createNormalCurvPropertyModifier(actor, createApplier(set_light_target_info, str), property_type());
	}
	else
	{
		return createPropertyModifier(actor, createApplier(set_light_target_info, str), property_type());
	}
}

//////////////////////////////////////////////////////////////////////////

DynEffPropertyModifierFactory::DynEffPropertyModifierFactory()
{
	add_creator("Follow_Type", new FollowTypePropertyModifierCreator);
	add_creator("Target_Info", new FollowLightTargetInfoPropertyModifierCreator);
}

PropertyModifierFactory* DynEffectObjectManager::create_modifier_factory()
{
	return new DynEffPropertyModifierFactory;
}

inline std::string operator + (const std::string& first_value, const std::string& second_value)
{
	return first_value;
}
