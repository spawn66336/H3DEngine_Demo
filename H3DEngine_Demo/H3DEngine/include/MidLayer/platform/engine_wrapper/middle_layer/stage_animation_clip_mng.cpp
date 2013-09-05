/**
* @file stage_animation_clip_mng.h
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

StageAnimationClip::StageAnimationClip(StageAnimationClipMng* clip_mng)
:m_ani_clip_mng(clip_mng)
,m_frame_count(0)
,m_ani_clip_type(StageAnimationClip_Common)
,m_is_playing(false)
,m_current_cutting_data(NULL)
,m_next_cutting_data(NULL)
,m_total_update_time(0)
,m_begin_bar(0)
,m_is_visible(true)
{
};
StageAnimationClip::~StageAnimationClip()
{
	m_object_list.clear();
};
// ����������Ϣ������
void StageAnimationClip::resetObjectIterator() 
{
	m_object_list_iter = m_object_list.begin();
};
// ���������Ϣ��������ָ����һ��������Ϣ�����������Ч����0
DynamicEffectObject* StageAnimationClip::nextObject()
{
	if(m_object_list_iter != m_object_list.end())
		return *(m_object_list_iter++);
	else
		return 0;
};
//��������޸���
static bool addPropertyModifier(PropertyModifierFactory* modifier_factory
								,BiXmlNode* propertyNode
								,const char* actor_level
								,const char* child_entity_id
								,bool is_curv
								,DynamicEffectObject* top_effect_obj
								,ActorAnimationClip* ani_clip)
{
	//������
	const char* property_name = BiXmlHelper::get_attribute(propertyNode,"key");
	const char* fixed_parm = BiXmlHelper::get_attribute(propertyNode,"fixed_param");
	const char* value = BiXmlHelper::get_attribute(propertyNode,"value");
	if(value == NULL)
		value = BiXmlHelper::get_attribute(propertyNode,"default");//ȡȱʡֵ

	std::string curve_str;
	if(is_curv)//���н����ļ���ȡ
	{
		/*BiXmlNode* sequence_node = propertyNode->FirstChild("PropertySequence");
		if(sequence_node)
		{
			BiXmlElement * element = sequence_node->ToElement();
			if(element)
				curve_str = GameGraphicHelper::xml2Str(element);
			
		}*/
		//BiXmlNode* sequence_node = propertyNode->FirstChild();
		BiXmlElement * element = propertyNode->ToElement();
		if(element && element->GetText())
			curve_str = element->GetText();
	}

	//����modifier type
	PropertyModifierType modifier_type;
	modifier_type.child_actor_id = actor_level;//actor��Σ�ֻ������actor��
	modifier_type.child_entity_id = child_entity_id;//ʵ������

	modifier_type.property_name = property_name;//��������;
	if(fixed_parm)
		modifier_type.property_fixed_param = fixed_parm;//�̶����� /parm1/parm2/..;
	modifier_type.is_curv = is_curv;//�Ƿ�����

	//����modifier
	IPropertyModifier* modifier = modifier_factory->create_modifier(top_effect_obj,modifier_type);
	if(modifier)
	{
		IPropertyModifierStreamable* s_modifier = dynamic_cast<IPropertyModifierStreamable*>(modifier);
		assert(s_modifier);
		if(is_curv)//����
		{
			if(!curve_str.empty())
				s_modifier->set_value(curve_str.c_str());//������������
		}
		else
		{
			if(value != NULL)
			{
				s_modifier->set_value(value);//��������ֵ
				s_modifier->execute(true,0.0f);
			}
		}

		if(!ani_clip->add_property_modifier(modifier_type, modifier))
		{
			assert(0);
			modifier_factory->releaseModifier(modifier);
			return false;
		}
	}
	else
	{
		//assert(0);
		//return false;
		return true;
	}
	return true;
}
//�ݹ齨����̬Ч���������ʵ��
static bool middleLayerCreateEffectRecursive(PropertyModifierFactory* modifier_factory,
											   BiXmlNode* xmlNode,
											   DynamicEffectObject* top_effect_obj,
											   std::string& actor_level, 
											   ActorAnimationClip* ani_clip,
											   const char* effect_id)
{
	//��������
	BiXmlNode* propertyInfoNode = xmlNode->FirstChild("PropertyInfo");
	if( propertyInfoNode != NULL)
	{
		BiXmlNode* propertyNode = propertyInfoNode->FirstChild("Property");
		while(propertyNode != NULL )
		{
			//���Խ���
			if(!addPropertyModifier(modifier_factory, propertyNode, actor_level.c_str(), "",false ,top_effect_obj,ani_clip))
				return false;

			propertyNode = propertyNode->NextSibling("Property");
		}
	}
	BiXmlNode* propertyCurveInfoNode = xmlNode->FirstChild("PropertyCurveInfo");
	if(propertyCurveInfoNode != NULL )
	{
		BiXmlNode* propertyNode = propertyCurveInfoNode->FirstChild("Property");
		while(propertyNode != NULL )
		{
			//�������Խ���
			if(!addPropertyModifier(modifier_factory, propertyNode, actor_level.c_str(), "",true ,top_effect_obj,ani_clip))
				return false;
			propertyNode = propertyNode->NextSibling("Property");
		}
	}
	//������ʵ��
	BiXmlNode* entityList = xmlNode->FirstChild("EntityList");
	if(entityList!=NULL)
	{
		BiXmlNode* entityNode = entityList->FirstChild("SubEntity");
		while(entityNode != NULL)
		{
			const char* entity_name = BiXmlUtility::get_attribute(entityNode, "name");
			assert(entity_name != NULL );

			//��������
			BiXmlNode* propertyInfoNode = entityNode->FirstChild("PropertyInfo");
			if( propertyInfoNode != NULL)
			{
				BiXmlNode* propertyNode = propertyInfoNode->FirstChild("Property");
				while(propertyNode != NULL )
				{
					//���Խ���
					if(!addPropertyModifier(modifier_factory, propertyNode, actor_level.c_str(), entity_name,false ,top_effect_obj,ani_clip))
						return false;
					propertyNode = propertyNode->NextSibling("Property");
				}
			}
			BiXmlNode* propertyCurveInfoNode = entityNode->FirstChild("PropertyCurveInfo");
			if(propertyCurveInfoNode != NULL )
			{
				BiXmlNode* propertyNode = propertyCurveInfoNode->FirstChild("Property");
				while(propertyNode != NULL )
				{
					//�������Խ���
					if(!addPropertyModifier(modifier_factory, propertyNode, actor_level.c_str(), entity_name,true ,top_effect_obj,ani_clip))
						return false;
					propertyNode = propertyNode->NextSibling("Property");
				}
			}

			entityNode = entityNode->NextSibling("SubEntity");
		}
	}
	
	//������actor
	BiXmlNode* dynamicEffectObjectList = xmlNode->FirstChild("DynamicEffectObjectList");
	if(dynamicEffectObjectList != NULL)
	{
		BiXmlNode* dynamicEffectObjectNode = dynamicEffectObjectList->FirstChild("DynamicEffectObject");
		while(dynamicEffectObjectNode != NULL)
		{
			//�ݹ齨��modifier
			const char* sub_object_id = BiXmlHelper::get_attribute(dynamicEffectObjectNode,"id");
			//�����Լ��Ĳ��ID
			std::string sub_actor_level = actor_level + "/" + sub_object_id;
			
			if(!middleLayerCreateEffectRecursive(modifier_factory ,dynamicEffectObjectNode, top_effect_obj,sub_actor_level, ani_clip, effect_id))
				return false;

			dynamicEffectObjectNode = dynamicEffectObjectNode->NextSibling("DynamicEffectObject");
		}
	}
	//����������������
	BiXmlNode* linkObjectList = xmlNode->FirstChild("LinkDynamicEffectObjectList");
	if(linkObjectList != NULL)
	{
		BiXmlNode* subEffectObjectNode = linkObjectList->FirstChild("DynamicEffectObject");
		while( subEffectObjectNode != NULL)
		{
			//�ݹ齨��modifier
			const char* sub_object_id = BiXmlHelper::get_attribute(subEffectObjectNode,"id");
			assert(sub_object_id);
			//�ҵ�����������
			DynamicEffectObject* sub_obj = dynamic_cast<DynamicEffectObject*>(top_effect_obj->find_actor(sub_object_id));
			assert(sub_obj);
			//����obj�� ani_control
			ActorAnimationControl* sub_ani_control = sub_obj->getAnimationControl();
			assert(sub_ani_control);
			//������StageAniClipͬ���� ActorAniClip
			ActorAnimationClip* sub_ani_clip = sub_ani_control->find_animation_clip(effect_id);
			
			if(sub_ani_clip == NULL)
			{
				sub_ani_clip = new ActorAnimationClip();
				sub_ani_control->add_animation_clip(effect_id, sub_ani_clip);//ActorAniClip�� ��̨Ч��ͬ��
			}
			//�����Լ��Ĳ��ID
			std::string sub_actor_level ;

			//�ڼ��������Ͻ�������
			if(!middleLayerCreateEffectRecursive(modifier_factory ,subEffectObjectNode, sub_obj ,sub_actor_level, sub_ani_clip, effect_id))
				return false;

			subEffectObjectNode = subEffectObjectNode->NextSibling("DynamicEffectObject");
		}
	}
	return true;
}

bool StageAnimationClip::createDynamicEffect(BiXmlNode* effectNode)
{	//����effect�ļ���Ϣ����DynamicEffect
	BiXmlNode* item = 0;
	const char* object_id = BiXmlHelper::get_attribute(effectNode,"id");
	//�������Щ����
	DynamicEffectObject* obj = dynamic_cast<DynamicEffectObject*>(m_ani_clip_mng->getStage()->getActorMng()->find_actor(object_id));

	//���û����Ч����ƥ������壬��������ֱ�ӷ��ء�
	if (NULL == obj)
	{
		obj = dynamic_cast<DynamicEffectObject*>(m_ani_clip_mng->getStage()->getActorMng()->getSystemActor(object_id));
		if (obj == NULL)
			return true;
	}

	assert(obj != NULL);
	LOD_LEVEL lod = ActorManager::isLodSystemActorName((obj->get_name()));
	if (lod == LL_NULL)
	{
		m_object_list.push_back(obj);//���������������
	}
	else if (lod == LL_ZERO || lod == LL_ONE || lod == LL_TWO)
	{
		if (lod == getClipMng()->getStage()->getLod())
		{
			m_object_list.push_back(obj);
		}
	}
	else
	{
		assert(false);
	}

	//����obj�� ani_control
	ActorAnimationControl* ani_control = obj->getAnimationControl();
	assert(ani_control);
	//������StageAniClipͬ���� ActorAniClip
	ActorAnimationClip* ani_clip = ani_control->find_animation_clip(this->getID());
	
	if(ani_clip == NULL)
	{
		ani_clip = new ActorAnimationClip();
		ani_control->add_animation_clip(this->getID(), ani_clip);//ActorAniClip�� ��̨Ч��ͬ��
	}

	std::string actor_level;
	//����actor	
	PropertyModifierFactory* modifier_factory = m_ani_clip_mng->getStage()->getActorMng()->getPropertyModifierFactory();//
	bool ret = middleLayerCreateEffectRecursive(modifier_factory, effectNode,obj,actor_level,ani_clip, this->getID());

	return ret;
};
bool StageAnimationClip::externalCreateDynamicEffect(ActorManager* actorManager,DynamicEffectObject* effectObj, BiXmlNode* effectNode)
{	//����effect�ļ���Ϣ����DynamicEffect
	BiXmlNode* item = 0;
	const char* object_id = BiXmlHelper::get_attribute(effectNode,"id");
	//�������Щ����
	DynamicEffectObject* obj = effectObj;
	assert(obj != NULL);
	m_object_list.push_back(obj);//���������������

	//����obj�� ani_control
	ActorAnimationControl* ani_control = obj->getAnimationControl();
	assert(ani_control);
	//������StageAniClipͬ���� ActorAniClip
	ActorAnimationClip* ani_clip = ani_control->find_animation_clip(this->getID());
	
	if(ani_clip == NULL)
	{
		ani_clip = new ActorAnimationClip();
		ani_control->add_animation_clip(this->getID(), ani_clip);//ActorAniClip�� ��̨Ч��ͬ��
	}

	std::string actor_level;
	//����actor	
	PropertyModifierFactory* modifier_factory = actorManager->getPropertyModifierFactory();//
	bool ret = middleLayerCreateEffectRecursive(modifier_factory, effectNode,obj,actor_level,ani_clip, this->getID());

	return ret;
};

static bool removeDynamicEffectObjectRecursive(BiXmlElement* obj, const char* id)
{
	const char* obj_id = obj->Attribute("id");

	//������Լ�����ɾ��������
	if(stricmp(obj_id, id) == 0)
	{
		BiXmlNode* parent = const_cast<BiXmlNode*>(obj->Parent());
		parent->RemoveChild(obj);
		return true;
	}

	//ɾ��������
	BiXmlElement* ObjList = obj->FirstChildElement("DynamicEffectObjectList");
	if (ObjList)
	{
		BiXmlElement* SubObj = ObjList->FirstChildElement("DynamicEffectObject");
		while(SubObj)
		{
			BiXmlElement* temp = SubObj;
			SubObj = SubObj->NextSiblingElement("DynamicEffectObject");

			if (!removeDynamicEffectObjectRecursive(temp, id))
				return false;


		}
		//����ڵ�Ϊ�գ���ɾ���Լ�
		if (!ObjList->FirstChild())
		{
			obj->RemoveChild(ObjList);
		}
	}

	//ɾ����������
	BiXmlElement* linkedObj = obj->FirstChildElement("LinkDynamicEffectObjectList");
	if (linkedObj)
	{
		BiXmlElement* SubObj = ObjList->FirstChildElement("DynamicEffectObject");
		while(linkedObj)
		{
			BiXmlElement* temp = linkedObj;
			linkedObj = linkedObj->NextSiblingElement("DynamicEffectObject");

			if (!removeDynamicEffectObjectRecursive(temp, id))
				return false;
		}
		//����ڵ�Ϊ�գ���ɾ���Լ�
		if (!linkedObj->FirstChild())
		{
			obj->RemoveChild(linkedObj);
		}
	}


	return true;
}
bool StageAnimationClip::removeDynamicEffectObjectFromFile(const char* filename, const char* id)
{
	BiXmlDocument* doc = XmlDocFactory::CreateDocument();
	if (!doc || !doc->LoadFile(filename))
	{
		XmlDocFactory::ReleaseDocument(doc);
		return false;
	}

	BiXmlElement* root = doc->RootElement();
	assert(root != NULL);

	BiXmlElement* DynamicEffObj = root->FirstChildElement("DynamicEffectObject");
	while(DynamicEffObj)
	{
		BiXmlElement* temp = DynamicEffObj;
		DynamicEffObj = DynamicEffObj->NextSiblingElement("DynamicEffectObject");

		//��һ����ı�dom�������νṹ��������ִ����һ��֮ǰ��ȥ��һ���ֵܽڵ㡣
		if(!removeDynamicEffectObjectRecursive(temp, id))
			return false;
	}

	bool bRet = doc->SaveFile(filename);

	XmlDocFactory::ReleaseDocument(doc);

	return bRet;
}

//�����ļ�,����clip id
std::string StageAnimationClip::getFileClipId(const char* filename0, bool isBinary)
{
	//create dynamic effect object
	assert(!isBinary);
	std::string strClipId;
	//ת���ָ���
	std::string fullFileName ;
	MiddleLayerFilePathHelper::convertSeperator(filename0,fullFileName);

	BiXmlHelper::LoadXMLDocument(fullFileName.c_str());
	BiXmlDocument* doc = BiXmlHelper::PeekXMLDocument(fullFileName.c_str(), true);
	if(doc == NULL)
	{
		assert(0);
		return strClipId;
	}
	// ��ȡ����
	BiXmlNode* stageEffectNode = doc->FirstChild("StageEffect");
	if (stageEffectNode == NULL)
	{
		//delete doc;
		XmlDocFactory::ReleaseDocument(doc);
		return strClipId;
	}
	//����ID
	const char* id = BiXmlUtility::get_attribute(stageEffectNode, "id");
	assert(id);
	strClipId = id;

	//delete doc;
	XmlDocFactory::ReleaseDocument(doc);
	return strClipId;
};

//�����ļ�,����clip type
std::string StageAnimationClip::getFileClipType(const char* filename0, bool isBinary)
{
	//create dynamic effect object
	assert(!isBinary);
	std::string strClipType;
	//ת���ָ���
	std::string fullFileName ;
	MiddleLayerFilePathHelper::convertSeperator(filename0,fullFileName);

	BiXmlHelper::LoadXMLDocument(fullFileName.c_str());
	BiXmlDocument* doc = BiXmlHelper::PeekXMLDocument(fullFileName.c_str(), true);
	if(doc == NULL)
	{
		assert(0);
		return strClipType;
	}
	// ��ȡ����
	BiXmlNode* stageEffectNode = doc->FirstChild("StageEffect");
	if (stageEffectNode == NULL)
	{
		//delete doc;
		XmlDocFactory::ReleaseDocument(doc);
		return strClipType;
	}
	//��������
	const char* type = BiXmlUtility::get_attribute(stageEffectNode, "type");
	assert(type);
	strClipType = type;

	//delete doc;
	XmlDocFactory::ReleaseDocument(doc);
	return strClipType;
};

//������������л���Ϣ
void StageAnimationClip::setPostProcessSwitchInfo(const char* filename0)
{
	//ת���ָ���
	std::string fullFileName ;
	MiddleLayerFilePathHelper::convertSeperator(filename0,fullFileName);


	BiXmlHelper::LoadXMLDocument(fullFileName.c_str());
	BiXmlDocument* doc = BiXmlHelper::PeekXMLDocument(fullFileName.c_str(), true);
	if(doc == NULL)
	{
		return ;
	}
	// ��ȡ����
	BiXmlNode* postSwitchNode = doc->FirstChild("PostEffectSwitch");
	if (postSwitchNode == NULL)
	{
		//delete doc;
		XmlDocFactory::ReleaseDocument(doc);
		return ;
	}
	// ��ȡ��̬Ч������Ľڵ�
	BiXmlNode* switchNode = postSwitchNode->FirstChild("Switch");
	while(switchNode != NULL)
	{//������̨Ч��
		const char* effect_name = BiXmlHelper::get_attribute(switchNode,"EffectName");
		const char* effect_valid = BiXmlHelper::get_attribute(switchNode,"Valid");
		assert(effect_name && effect_valid);
		//���ú�����
		std::list<DynamicEffectObject*>::const_iterator iter = m_object_list.begin();
		for(; iter != m_object_list.end(); ++iter)
		{
			CameraEffectObject* camera_object = dynamic_cast<CameraEffectObject*>(*iter);
			assert(camera_object);
			if(stricmp(effect_valid,"false") == 0) 
				camera_object->disableCameraPostprocess(effect_name);
		}
		switchNode = switchNode->NextSibling("Switch");
	}
	//delete doc;
	XmlDocFactory::ReleaseDocument(doc);
	return ;
};
//�����ļ�
bool StageAnimationClip::load(const char* filename0, bool isBinary)
{
	//create dynamic effect object
	assert(!isBinary);
	if(!m_object_list.empty())
	{
		assert(0);
		return false;
	}

	//ת���ָ���
	std::string fullFileName ;
	MiddleLayerFilePathHelper::convertSeperator(filename0,fullFileName);
	//ȡ����·��
	std::string working_dir ;
	MiddleLayerFilePathHelper::getWorkingPath(filename0,working_dir);

	BiXmlHelper::LoadXMLDocument(fullFileName.c_str());
	BiXmlDocument* doc = BiXmlHelper::PeekXMLDocument(fullFileName.c_str(), true);
	if(doc == NULL)
	{
		//LogError("[StageAnimationClip::load] load clip file failed:",fullFileName.c_str());
		assert(0);
		return false;
	}
	// ��ȡ����
	BiXmlNode* stageEffectNode = doc->FirstChild("StageEffect");
	if (stageEffectNode == NULL)
	{
		//delete doc;
		XmlDocFactory::ReleaseDocument(doc);
		return false;
	}
	//����ID
	const char* id = BiXmlUtility::get_attribute(stageEffectNode, "id");
	assert(id);
	setID(id);
	//��ȡ����
	const char* stage_effect_len = BiXmlUtility::get_attribute(stageEffectNode, "len");
	m_frame_count = atoi(stage_effect_len);
	//��ȡ����
	const char* stage_effect_type = BiXmlUtility::get_attribute(stageEffectNode, "type");
	if(stage_effect_type != NULL)
	{
		if(stricmp(stage_effect_type, "common") == 0)
			setClipType(StageAnimationClip_Common);
		else if(stricmp(stage_effect_type, "camera") == 0)
			setClipType(StageAnimationClip_Camera);
		else
			assert(false && "StageAnimationClip parse, clip���Ͳ�֧��!");
	}
	// ��ȡ��̬Ч������Ľڵ�
	BiXmlNode* dynamicEffectObjectNode = stageEffectNode->FirstChild("DynamicEffectObject");
	while(dynamicEffectObjectNode != NULL)
	{//������̨Ч��
		if (!createDynamicEffect(dynamicEffectObjectNode))
		{
			//delete doc;
			XmlDocFactory::ReleaseDocument(doc);
			return false;
		}

		dynamicEffectObjectNode = dynamicEffectObjectNode->NextSibling("DynamicEffectObject");
	}
	//delete doc;
	XmlDocFactory::ReleaseDocument(doc);

	resetObjectIterator() ;
	//��ȡ��̨Ч���л�����
	if(getClipType() == StageAnimationClip_Camera)
	{
		std::string strSwitchFile = filename0;

		MiddleLayerStrHelper::replaceStr(strSwitchFile,".xml","_PostEffectSwitch.xml");

		setPostProcessSwitchInfo(strSwitchFile.c_str());
	}
	//��ȡ��̨Ч���л�����over
	return true;
};
//���ⲿ����StageAnimationClip�ã���Ӷ���Ĵ��� lishan 110624
bool StageAnimationClip::externalLoad(ActorManager* actorManager, DynamicEffectObject* effectObj,const char* filename0, bool isBinary)
{
	assert(!isBinary);
	if(!m_object_list.empty())
	{
		assert(0);
		return false;
	}

	//ת���ָ���
	std::string fullFileName ;
	MiddleLayerFilePathHelper::convertSeperator(filename0,fullFileName);
	//ȡ����·��
	std::string working_dir ;
	MiddleLayerFilePathHelper::getWorkingPath(filename0,working_dir);

	BiXmlHelper::LoadXMLDocument(fullFileName.c_str());
	BiXmlDocument* doc = BiXmlHelper::PeekXMLDocument(fullFileName.c_str(), true);
	if(doc == NULL)
	{
		//LogError("[Stage::externalLoad] load file failed:",fullFileName.c_str());
		//assert(0);
		return false;
	}
	// ��ȡ����
	BiXmlNode* stageEffectNode = doc->FirstChild("StageEffect");
	if (stageEffectNode == NULL)
	{
		//delete doc;
		XmlDocFactory::ReleaseDocument(doc);
		return false;
	}
	//����ID
	const char* id = BiXmlUtility::get_attribute(stageEffectNode, "id");
	assert(id);
	setID(id);
	//��ȡ����
	const char* stage_effect_len = BiXmlUtility::get_attribute(stageEffectNode, "len");
	m_frame_count = atoi(stage_effect_len);
	//��ȡ����
	const char* stage_effect_type = BiXmlUtility::get_attribute(stageEffectNode, "type");
	if(stage_effect_type != NULL)
	{
		if(stricmp(stage_effect_type, "common") == 0)
			setClipType(StageAnimationClip_Common);
		else if(stricmp(stage_effect_type, "camera") == 0)
			setClipType(StageAnimationClip_Camera);
		else
			assert(!"StageAnimationClip parse, clip���Ͳ�֧��!");
	}
	// ��ȡ��̬Ч������Ľڵ�
	BiXmlNode* dynamicEffectObjectNode = stageEffectNode->FirstChild("DynamicEffectObject");
	while(dynamicEffectObjectNode != NULL)
	{//������̨Ч��
		if (!externalCreateDynamicEffect(actorManager, effectObj,dynamicEffectObjectNode))
		{
			//delete doc;
			XmlDocFactory::ReleaseDocument(doc);
			return false;
		}

		dynamicEffectObjectNode = dynamicEffectObjectNode->NextSibling("DynamicEffectObject");
	}
	//delete doc;
	XmlDocFactory::ReleaseDocument(doc);

	resetObjectIterator() ;
	//��ȡ��̨Ч���л�����
	if(getClipType() == StageAnimationClip_Camera)
	{
		std::string strSwitchFile = filename0;

		MiddleLayerStrHelper::replaceStr(strSwitchFile,".xml","_PostEffectSwitch.xml");

		setPostProcessSwitchInfo(strSwitchFile.c_str());
	}
	//��ȡ��̨Ч���л�����over
	return true;
}

void divide_actor_params(const std::string& sub_actor, std::list<std::string>& actor_list)
{
	std::string actor_id = "";
	int begin_index = 1;
	while (true)
	{
		if (begin_index >= sub_actor.length())
		{
			break;
		}

		int index = sub_actor.find('/', begin_index);
		if (index == std::string::npos)
		{
			actor_id = sub_actor.substr(begin_index, sub_actor.length()- begin_index);
			actor_list.push_back(actor_id);
			break;
		}
		else
		{
			actor_id = sub_actor.substr(begin_index, index - begin_index);
			actor_list.push_back(actor_id);
		}
		begin_index = index + 1;
	}
}
static bool generateObjectProperty(BiXmlNode* effect_node, PropertyModifierType* modifier_type, IPropertyModifier* property_modifier, bool is_camera_property, bool isOptimize=true)
{
	//���ҵ�����ڵ�
	std::list<std::string> sub_actor_list;
	divide_actor_params(modifier_type->child_actor_id,sub_actor_list);

	//����Ŀ��actor
	BiXmlNode* target_actor_node = effect_node;
	for(std::list<std::string>::const_iterator iter = sub_actor_list.begin(); iter != sub_actor_list.end() ; ++iter)
	{
		//����actor list�ڵ�
		BiXmlNode* actor_list_node = target_actor_node->FirstChild("DynamicEffectObjectList");
		if (actor_list_node == NULL)
		{
			//actor_list_node = new BiXmlElement("DynamicEffectObjectList");
			//target_actor_node->LinkEndChild(actor_list_node);//���δ�ҵ��������½ڵ�
			actor_list_node = target_actor_node->InsertEndChild("DynamicEffectObjectList");
		}
		//���� actor �ڵ�
		BiXmlNode* actor_node = actor_list_node->FirstChild("DynamicEffectObject");
		while(actor_node)
		{
			const char* actor_node_id = BiXmlHelper::get_attribute(actor_node,"id");//����actor id ���ҽڵ�
			if( (*iter) == actor_node_id)//���нڵ�
			{
				break;
			}
			actor_node = actor_node->NextSibling("DynamicEffectObject");//������һ��
		}
		if( actor_node == NULL)//��û�нڵ㣬���1��
		{
			//actor_node = new BiXmlElement("DynamicEffectObject");
			//actor_list_node->LinkEndChild(actor_node);//���δ�ҵ��������½ڵ�
			actor_node = actor_list_node->InsertEndChild("DynamicEffectObject");
		}
		//������һ������
		target_actor_node = actor_node;
	}
	//����Ŀ��λ��
	BiXmlNode* target_node_position = target_actor_node;//���ô���λ��Ϊactor
	if(!modifier_type->child_entity_id.empty()) 
	{
		//����entity list�ڵ�
		BiXmlNode* entity_list_node = target_actor_node->FirstChild("EntityList");
		if (entity_list_node == NULL)
		{
			//entity_list_node = new BiXmlElement("EntityList");
			//target_actor_node->LinkEndChild(entity_list_node);//���δ�ҵ��������½ڵ�
			entity_list_node = target_actor_node->InsertEndChild("EntityList");
			;//���δ�ҵ��������½ڵ�
		}
		//���� entity �ڵ�
		BiXmlNode* entity_node = entity_list_node->FirstChild("SubEntity");
		while(entity_node)
		{
			const char* entity_node_name = BiXmlHelper::get_attribute(entity_node,"name");//����entity name ���ҽڵ�
			if(modifier_type->child_entity_id == entity_node_name)//���нڵ�
			{
				break;
			}
			entity_node = entity_node->NextSibling("SubEntity");
		}
		if( entity_node == NULL)//��û�нڵ㣬���1��
		{
			//BiXmlElement* entity_element = new BiXmlElement("SubEntity");
			BiXmlElement* entity_element = entity_list_node->InsertEndChild("SubEntity")->ToElement();
			entity_element->SetAttribute("name", modifier_type->child_entity_id.c_str());

			entity_node = entity_element;
			//entity_list_node->LinkEndChild(entity_node);//���δ�ҵ��������½ڵ�
		}
		target_node_position = entity_node;//���ô���λ��Ϊentity
	}

	IPropertyModifierStreamable* s_midifier = dynamic_cast<IPropertyModifierStreamable*>(property_modifier);
	assert(s_midifier);

	const char* modifier_value = s_midifier->get_value();
	assert(modifier_value);

	bool curve_saved = false;

	//�����������
	if(modifier_type->is_curv)//������������
	{
		IPropertyCurvModifierStreamable* s_curve_modifier = dynamic_cast<IPropertyCurvModifierStreamable*>(s_midifier);
		assert(s_curve_modifier);

		//ȡcalculator
		ICalculator* calculator = s_curve_modifier->get_calculator();

		int bufsize = 0;
		float f_buf[20] = {0};
		//ȡ֡��
		int key_frames = calculator->get_key_frames(-1 ,NULL , bufsize);

		if(bufsize == 1 && !is_camera_property && isOptimize)//��֡�ҷ�������м򻯣�����д������
		{
			modifier_value = calculator->get_interpolating_value(-1,0.0f);//ȡ��ֵ֡
		}
		else//��֡
		{
			BiXmlNode* property_curve_info_node = target_node_position->FirstChild("PropertyCurveInfo");
			if (property_curve_info_node == NULL)
			{
				//property_curve_info_node = new BiXmlElement("PropertyCurveInfo");
				//target_node_position->LinkEndChild(property_curve_info_node);//���δ�ҵ��������½ڵ�
				property_curve_info_node = target_node_position->InsertEndChild("PropertyCurveInfo");
			}
			//��������
			//BiXmlElement item("Property");
			BiXmlElement* item = property_curve_info_node->InsertEndChild("Property")->ToElement();//����ڵ�
			item->SetAttribute("key", modifier_type->property_name.c_str());

			if(!modifier_type->property_fixed_param.empty())
				item->SetAttribute("fixed_param", modifier_type->property_fixed_param.c_str());

			//���node
			//BiXmlDocument* doc_read = XmlDocFactory::CreateDocument();
			//doc_read->ParseXmlStr(modifier_value);
			//if(doc_read->Error()) 
			//{
			//	assert(0);
			//}
			////����ڵ�
			//item->InsertEndChild(*(doc_read->RootElement()));

			////ɾ���ĵ�
			//XmlDocFactory::ReleaseDocument(doc_read);

			curve_saved = true;//��֡���ߴ洢��

			item->AddText(modifier_value);
			item->FirstChild()->ToText()->SetCDATA(true);

		}
	}

	if(!curve_saved)//���Ի�֡����
	{
		BiXmlNode* property_info_node = target_node_position->FirstChild("PropertyInfo");
		if (property_info_node == NULL)
		{
			//property_info_node = new BiXmlElement("PropertyInfo");
			//target_node_position->LinkEndChild(property_info_node);//���δ�ҵ��������½ڵ�
			property_info_node = target_node_position->InsertEndChild("PropertyInfo");
			//���δ�ҵ��������½ڵ�
		}
		//BiXmlElement item("Property");
		BiXmlElement* item = property_info_node->InsertEndChild("Property")->ToElement();
		item->SetAttribute("key", modifier_type->property_name.c_str());
		item->SetAttribute("fixed_param", modifier_type->property_fixed_param.c_str());
		item->SetAttribute("value", modifier_value);
		//property_info_node->InsertEndChild(item);//����ڵ�
	}
	return true;
};
static bool isSupportUnsaved(DynamicEffectObject* dynEffObj, char* strEntityName)
{
	if (!dynEffObj || !strEntityName)
	{
		return false;
	}

	dynEffObj->reset_entity_iter();
	while(CMediaEndEntity* entity = dynEffObj->next_entity())
	{
		//���Դ
		if (entity->type() == MEDIA_END_ENTITY_TYPE_POINT_PREPASS_LIGHT)
		{
			strcpy(strEntityName, entity->get_name());
			return true;
		}
		//׶��Դ
		else if (entity->type() == MEDIA_END_ENTITY_TYPE_WIMBLE_PREPASS_LIGHT)
		{
			strcpy(strEntityName, entity->get_name());
			return true;
		}
		//�����Դ
		else if (entity->type() == MEDIA_END_ENTITY_TYPE_DIRECTION_PREPASS_LIGHT)
		{
			strcpy(strEntityName, entity->get_name());
			return true;
		}
	}
	return false;
}

static IPropertyModifier* find_dyneffobj_clip_propertymodifier(
	DynamicEffectObject* dynamic_effect_object, 
	ActorAnimationClip* ani_clip, 
	PropertyModifierType* modifier_type)
{
	if (dynamic_effect_object == NULL ||
		ani_clip == NULL ||
		modifier_type == NULL)
	{
		return NULL;
	}

	IPropertyModifier* property_modifier = ani_clip->find_property_modifier(*modifier_type);

	//���ͬ��clip��û�У�������stageclip����defaultclip�н��в���
	if (!property_modifier)
	{
		ActorAnimationControl* anim_Control = dynamic_effect_object->getAnimationControl();

		ActorAnimationClip* stage_clip = anim_Control->find_animation_clip(DynamicEffectObject::getStagePropertyName());
		property_modifier = stage_clip->find_property_modifier(*modifier_type);

		if (!property_modifier)
		{
			ActorAnimationClip* default_clip = anim_Control->find_animation_clip(DynamicEffectObject::getDefaultPropertyName());
			property_modifier = default_clip->find_property_modifier(*modifier_type);
		}
	}

	return property_modifier;
}
//���ĳ�����������Ƿ���ڣ���ֵ
static bool getPropertyIsEnable(DynamicEffectObject* dynamic_effect_object, 
								ActorAnimationClip* ani_clip, 
								PropertyModifierType* modifier_type,
								bool& isExisted)
{
	isExisted = false;
	if (dynamic_effect_object == NULL ||
		ani_clip == NULL ||
		modifier_type == NULL)
	{
		return NULL;
	}

	IPropertyModifier* property_modifier = find_dyneffobj_clip_propertymodifier(dynamic_effect_object, ani_clip, modifier_type);

	//���Ϊ��������
	if (modifier_type->is_curv)
	{
		IPropertyCurvModifierStreamable* curve_midifier = dynamic_cast<IPropertyCurvModifierStreamable*>(property_modifier);

		bool CurvProperty = false;
		if (curve_midifier)
		{
			const char* strValue = curve_midifier->get_value();

			NormalCurvSerializer<bool> curv_serializer;

			CalculatorT<bool>* calculator = curv_serializer.serialize(strValue);

			//�������ݼ��ʣ�����������ת���ԣ���ֵ֡ͬ�ϲ�
			calculator->simplify();

			int bufsize = 0;
			//ȡ֡��
			int key_frames = calculator->get_key_frames(-1 ,NULL , bufsize);

			if(bufsize == 1)//��֡
			{
				const char* frame_val = calculator->get_interpolating_value(-1,0.0f);

				if (stricmp(frame_val, "true") == 0)
				{
					CurvProperty = true;
				}
			}
			else
			{
				//ȡ�ؼ�֡����
				CurvDataT<bool>* curve_data = dynamic_cast<CurvDataT<bool>*>(calculator->get_curv_data());
				for(CurvNodes<bool>::NodeCollection::iterator it = curve_data->m_nodes->m_nodes.begin(); 
					it != curve_data->m_nodes->m_nodes.end(); ++it)
				{
					//�ж�����ֵ
					CurvNode<bool>* node = dynamic_cast<CurvNode<bool>*>(*it);
					if (node->m_value == true)
					{
						CurvProperty = true;
						break;
					}
				}

				delete curve_data;
			}

			delete calculator;

			isExisted = true;
			return CurvProperty;
		}
	}
	//���Ϊ����������
	else
	{
		IPropertyModifierStreamable* enable_midifier = dynamic_cast<IPropertyModifierStreamable*>(property_modifier);

		bool NoncurvProperty = false;
		if (enable_midifier)
		{
			//�ж�����ֵ
			const char* strValue = enable_midifier->get_value();
			if (stricmp(strValue, "true") == 0)
			{
				NoncurvProperty = true;
			}
			isExisted = true;
			return NoncurvProperty;
		}
	}

	return false;
}

static void copyPropertyModifierType(PropertyModifierType* dest, PropertyModifierType* src)
{
	if (dest == NULL || src == NULL)
		return;
	dest->child_actor_id = src->child_actor_id;
	dest->child_entity_id = src->child_entity_id;
	dest->property_fixed_param = src->property_fixed_param;
	dest->property_name = src->property_name;
	dest->is_curv = src->is_curv;
}

//���ݼ��������ж��Ƿ���Ҫ���䶯�����б���
static bool canNotBeOptimized(DynamicEffectObject* dynamic_effect_obj, 
				   ActorAnimationClip* ani_clip, 
				   char* strEntityName, 
				   PropertyModifierType* pModifierType)
{
	if (ani_clip == NULL || dynamic_effect_obj == NULL)
	{
		return false;
	}

	//�ƹ��Ƿ��������
	PropertyModifierType enable_modifier_type;
	enable_modifier_type.child_actor_id = "";
	enable_modifier_type.child_entity_id = strEntityName;
	enable_modifier_type.property_fixed_param = "";
	enable_modifier_type.property_name = "Light_Enable";
	//�����������
	enable_modifier_type.is_curv = true;
	bool b_enable_curve_modifier_existed = false;
	bool b_enable_curve_modifier_value = false;
	b_enable_curve_modifier_value = getPropertyIsEnable(dynamic_effect_obj, 
		ani_clip, &enable_modifier_type, b_enable_curve_modifier_existed);
	//��÷���������
	enable_modifier_type.is_curv = false;
	bool b_enable_noncurve_modifier_existed = false;
	bool b_enable_noncurve_modifier_value = false;
	b_enable_noncurve_modifier_value = getPropertyIsEnable(dynamic_effect_obj, 
		ani_clip, &enable_modifier_type, b_enable_noncurve_modifier_existed);

	//�ƹ��Ƿ�ɼ�����
	PropertyModifierType visible_modifier_type;
	visible_modifier_type.child_actor_id = "";
	visible_modifier_type.child_entity_id = strEntityName;
	visible_modifier_type.property_fixed_param = "";
	visible_modifier_type.property_name = "Visible";
	//�����������
	visible_modifier_type.is_curv = true;
	bool b_visible_curve_modifier_existed = false;
	bool b_visible_curve_modifier_value = false;
	b_visible_curve_modifier_value = getPropertyIsEnable(dynamic_effect_obj, 
		ani_clip, &visible_modifier_type, b_visible_curve_modifier_existed);
	//��÷���������
	visible_modifier_type.is_curv = false;
	bool b_visible_noncurve_modifier_existed = false;
	bool b_visible_noncurve_modifier_value = false;
	b_visible_noncurve_modifier_value = getPropertyIsEnable(dynamic_effect_obj, 
		ani_clip, &visible_modifier_type, b_visible_noncurve_modifier_existed);
	
	if (!b_enable_curve_modifier_value && b_enable_curve_modifier_existed)
	{
		copyPropertyModifierType(pModifierType, &enable_modifier_type);
		pModifierType->is_curv = true;
	}
	else if (!b_visible_curve_modifier_value && b_visible_curve_modifier_existed)
	{

		copyPropertyModifierType(pModifierType, &visible_modifier_type);
		pModifierType->is_curv = true;
	}
	else if (!b_enable_noncurve_modifier_value && b_enable_noncurve_modifier_existed)
	{
		copyPropertyModifierType(pModifierType, &enable_modifier_type);
		pModifierType->is_curv = false;
	}
	else if (!b_visible_noncurve_modifier_value && b_visible_noncurve_modifier_existed)
	{
		copyPropertyModifierType(pModifierType, &visible_modifier_type);
		*pModifierType = visible_modifier_type;
		pModifierType->is_curv = false;
	}

	//��������������ԣ�����������Ϊ׼�����жϡ���enable���Ժ�visible���Ծ�Ϊtrueʱ�ſɱ��ָ�����
	if (b_enable_curve_modifier_existed && b_visible_curve_modifier_existed)
	{
		return b_enable_curve_modifier_value && b_visible_curve_modifier_value;
	}
	else if (b_enable_curve_modifier_existed && !b_visible_curve_modifier_existed)
	{
		return b_enable_curve_modifier_value && b_visible_noncurve_modifier_value;
	}
	else if (!b_enable_curve_modifier_existed && b_visible_curve_modifier_existed)
	{
		return b_enable_noncurve_modifier_value && b_visible_curve_modifier_value;
	}
	else
	{
		return b_enable_noncurve_modifier_value && b_visible_noncurve_modifier_value;
	}
}
static bool saveSinglePropertyObject(BiXmlNode* object_element, 
									 DynamicEffectObject* dynamic_effect_object ,
									 ActorAnimationClip* ani_clip,
									 PropertyModifierType* false_modifier_type,
									 bool isOptimize=true)
{
	IPropertyModifier* false_property_modifier = find_dyneffobj_clip_propertymodifier(dynamic_effect_object, ani_clip,  false_modifier_type);
	//�ж��Ƿ��������
	bool is_camera_object = false;
	CameraEffectObject* camera_object = dynamic_cast<CameraEffectObject*>(dynamic_effect_object);
	if(camera_object != NULL)//��������м��ʲ���
		is_camera_object = true;
	if (false_modifier_type->is_curv)
	{
		IPropertyCurvModifierStreamable* curve_midifier = dynamic_cast<IPropertyCurvModifierStreamable*>(false_property_modifier);

		ICalculator* calculator = curve_midifier->get_calculator();
		calculator->simplify();		
	}

	if(!generateObjectProperty(object_element, false_modifier_type, false_property_modifier,is_camera_object, isOptimize))
	{
		//����ֱ�ӷ��أ�������
		return false;
	}
	return true;
}

static bool curvPropertyIsValid(IPropertyCurvModifierStreamable* s_curve_modifier)
{
	assert(s_curve_modifier);

	int frame_size = 0;
	bool bValid = false;
	int nIndex = -1;
	//�����ж�ĳ�����Թؼ�֡�Ƿ���Ч
	while(frame_size != -1)
	{
		int bufsize = 0;
		frame_size = s_curve_modifier->get_calculator()->get_key_frames(nIndex, NULL, bufsize);
		if (bufsize > 0)
		{
			bValid = true;
			break;
		}
		nIndex++;
	}
	return bValid;
}

static bool saveClipDynamicEffectObjectRecursive(BiXmlNode* parentNode, DynamicEffectObject* obj ,const char* clip_id ,  bool consistentId ,const char* file_main_name, StageAnimationPlayer* ani_player, bool isOptimize=true)
{
	if( obj == NULL )
		return false;
	//ȡ�ڵ�
	DynamicEffectObject* dynamic_effect_object = obj; 
	//ȡ��������Ϣ
	ActorAnimationControl* 	ani_control = dynamic_effect_object->getAnimationControl();
	assert(ani_control);
	//ȡstage clip ID ��Ӧ�� ani_clip
	ActorAnimationClip* ani_clip = ani_control->find_animation_clip(clip_id);
	if(ani_clip == NULL)//û�п�����
		return true;

	ActorAnimationClip* stage_ani_clip = ani_control->find_animation_clip(DynamicEffectObject::getStagePropertyName());
	ActorAnimationClip* default_ani_clip = ani_control->find_animation_clip(DynamicEffectObject::getDefaultPropertyName());

	//�����ڵ��ļ�
	//BiXmlElement* object_element = new BiXmlElement("DynamicEffectObject");//�����ڵ�
	//parentNode->LinkEndChild(object_element);//����ڵ�
	BiXmlElement* object_element = parentNode->InsertEndChild("DynamicEffectObject")->ToElement();//�����ڵ�

	if(consistentId)
		object_element->SetAttribute("id", file_main_name);//����ID
	else
		object_element->SetAttribute("id", dynamic_effect_object->get_name());//����ID

	char strEntityName[256];
	memset(strEntityName, 0, 255);
	
	PropertyModifierType false_modifier_type;
	if (isOptimize && isSupportUnsaved(dynamic_effect_object, strEntityName))
	{
		if (!canNotBeOptimized(dynamic_effect_object, ani_clip, strEntityName, &false_modifier_type))
		{
			return saveSinglePropertyObject(object_element, dynamic_effect_object, ani_clip, &false_modifier_type, true);
		}
	}

	//ȡԤ������Ϣ
	const CDefinedItemCollection* defined_item_collection = ani_player->getDefinedItemCollection();
	assert(defined_item_collection);
	// ���������޸���������
	ani_clip->reset_iterator();
	PropertyModifierType modifier_type;
	// ��������޸�����������ָ����һ�������޸��������������Ч����0

	while(IPropertyModifier* property_modifier = ani_clip->next(modifier_type))
	{
		IPropertyModifierStreamable* s_midifier = dynamic_cast<IPropertyModifierStreamable*>(property_modifier);
		assert(s_midifier);

		/*const char* value = s_midifier->get_value();

		if(!value)
		{
			continue;
		}

		assert(value);*/
		//
		//�����������Ϊ�գ��򲻽��б��棬ֱ����������Ϊ���Ա�Ȼ��Ϊ�գ����Բ����жϡ�
		if (modifier_type.is_curv)
		{
			IPropertyCurvModifierStreamable* s_curve_modifier = dynamic_cast<IPropertyCurvModifierStreamable*>(s_midifier);
			assert(s_curve_modifier);
			if (!curvPropertyIsValid(s_curve_modifier))
				continue;
		}

		//����ȱʡ���߱�־
		bool remove_default_curve = false;

		//�ж��Ƿ��������
		bool is_camera_object = false;
		CameraEffectObject* camera_object = dynamic_cast<CameraEffectObject*>(dynamic_effect_object);
		if(camera_object != NULL)//��������м��ʲ���
			is_camera_object = true;

		if(! is_camera_object
			&& isOptimize
			)//���м��ʲ���
		{
			if(!modifier_type.is_curv)//������
			{
				IPropertyModifier* stage_modifier = stage_ani_clip->find_property_modifier(modifier_type);
				IPropertyModifierStreamable* s_stage_modifier = dynamic_cast<IPropertyModifierStreamable*>(stage_modifier);
				IPropertyModifier* default_modifier = default_ani_clip->find_property_modifier(modifier_type);
				IPropertyModifierStreamable* s_default_midifier = dynamic_cast<IPropertyModifierStreamable*>(default_modifier);
				//�Ⱥ���̨���ԱȽ�
				if (stage_modifier)
				{
					if(stricmp(s_midifier->get_value(),s_stage_modifier->get_value() ) == 0)
						remove_default_curve = true;//ȱʡ��̨����ֵ����¼
					else//�����ߵĲ���¼
					{
						PropertyModifierType modifier_type_hascurve = modifier_type;
						modifier_type_hascurve.is_curv = true;
						if(IPropertyCurvModifierStreamable* curv_modifier = dynamic_cast<IPropertyCurvModifierStreamable*>(ani_clip->find_property_modifier(modifier_type_hascurve)))//ͬ���������߿���
						{
							//ȡcalculator
							ICalculator* calculator = curv_modifier->get_calculator();
							//�������ݼ��ʣ�����������ת���ԣ���ֵ֡ͬ�ϲ�
							calculator->simplify();

							//ȡ֡��
							int bufsize = 0;
							int key_frames = calculator->get_key_frames(-1 ,NULL , bufsize);
							if(bufsize > 0)//û��ֵ
							{		
								remove_default_curve = true;//����¼
							}
						}
					}
				}
				//�ٺ�ȱʡ���ԱȽ�
				else if(default_modifier)
				{
					if(stricmp(s_midifier->get_value(),s_default_midifier->get_value() ) == 0)
						remove_default_curve = true;//ȱʡֵ����¼
					else//�����ߵĲ���¼
					{
						PropertyModifierType modifier_type_hascurve = modifier_type;
						modifier_type_hascurve.is_curv = true;
						if(IPropertyCurvModifierStreamable* curv_modifier = dynamic_cast<IPropertyCurvModifierStreamable*>(ani_clip->find_property_modifier(modifier_type_hascurve)))//ͬ���������߿���
						{
							//ȡcalculator
							ICalculator* calculator = curv_modifier->get_calculator();
							//�������ݼ��ʣ�����������ת���ԣ���ֵ֡ͬ�ϲ�
							calculator->simplify();
							
							//ȡ֡��
							int bufsize = 0;
							int key_frames = calculator->get_key_frames(-1 ,NULL , bufsize);
							if(bufsize > 0)//û��ֵ
							{		
								remove_default_curve = true;//����¼
							}
						}
					}
				}
			}
			else
			{
				IPropertyCurvModifierStreamable* s_curve_modifier = dynamic_cast<IPropertyCurvModifierStreamable*>(s_midifier);
				assert(s_curve_modifier);
				//ȡcalculator
				ICalculator* calculator = s_curve_modifier->get_calculator();

				//�������ݼ��ʣ�����������ת���ԣ���ֵ֡ͬ�ϲ�
				calculator->simplify();

				//ȡȱʡmodifier
				PropertyModifierType modifier_type_notcurve = modifier_type;
				if(modifier_type_notcurve.is_curv)
					modifier_type_notcurve.is_curv = false;//���ݷ��������Ͳ�ȱʡֵ

				IPropertyModifier* stage_modifier = stage_ani_clip->find_property_modifier(modifier_type_notcurve);
				IPropertyModifierStreamable* s_stage_modifier = dynamic_cast<IPropertyModifierStreamable*>(stage_modifier);
				IPropertyModifier* default_modifier = default_ani_clip->find_property_modifier(modifier_type_notcurve);
				IPropertyModifierStreamable* s_default_midifier = dynamic_cast<IPropertyModifierStreamable*>(default_modifier);
				//�ж��Ƿ���ȱʡֵ����������

				int bufsize = 0;
				float f_buf[20] = {0};
				//ȡ֡��
				int key_frames = calculator->get_key_frames(-1 ,NULL , bufsize);

				if(bufsize == 1)//��֡
				{
					const char* frame_val = calculator->get_interpolating_value(-1,0.0f);
				
					//������̨ȱʡ���ԱȽ�
					if(s_stage_modifier)
					{
						if(stricmp(s_stage_modifier->get_value(), frame_val) == 0)//ֵ��ͬ
							remove_default_curve = true;//����ȱʡ����
						//�ڴ洢ʱ�Ὣ�������õ������� 
					}
					//�����̨ȱʡ���Բ����ڣ�������ȱʡ���ԱȽ�
					else if(s_default_midifier)
					{
						if(stricmp(s_default_midifier->get_value(), frame_val) == 0)//ֵ��ͬ
							remove_default_curve = true;//����ȱʡ����
						//�ڴ洢ʱ�Ὣ�������õ������� 
					}			
				}
			}
		}
		if(!remove_default_curve)//��ȥ������
		{
			//�������ͽ���XML�ڵ�
			if(!generateObjectProperty(object_element, &modifier_type, property_modifier,is_camera_object, isOptimize))
			{
				//����ֱ�ӷ��أ�������
				return false;
			}
		}
	}

	//��� ����ʵ������
	dynamic_effect_object->resetSubObjectIterator();
	DynamicEffectObject* sub_obj = dynamic_effect_object->nextSubObject();
	if(sub_obj)
	{
		BiXmlElement* sub_obj_list = object_element->InsertEndChild("LinkDynamicEffectObjectList")->ToElement();
		while(sub_obj)
		{
			if(!saveClipDynamicEffectObjectRecursive(sub_obj_list, sub_obj, clip_id, consistentId , file_main_name, ani_player))
				return false;
			
			sub_obj = obj->nextSubObject();
		}
	}
	
	//����ڵ�
	return true;
};
bool StageAnimationClip::saveClipStageSystemObjects(BiXmlNode* parentNode, bool consistentId ,
								const char* file_main_name, bool isOptimize)
{
	ActorManager * act_mng = getClipMng()->getStage()->getActorMng();
	StageAnimationPlayer* ani_player = dynamic_cast<StageAnimationPlayer*>(
		m_ani_clip_mng->getStage()->getPlayer());

	act_mng->reset_some_sysactor_iter(LL_ZERO);
	while(Actor* actor = act_mng->next_some_sysactor(LL_ZERO))
	{
		DynamicEffectObject* obj = dynamic_cast<DynamicEffectObject*>(actor);
		if(obj != NULL )
		{
			if(!saveClipDynamicEffectObjectRecursive(parentNode,obj,this->getID(), 
				consistentId, file_main_name, ani_player, isOptimize))
				return false;
		}
	}

	act_mng->reset_some_sysactor_iter(LL_ONE);
	while(Actor* actor = act_mng->next_some_sysactor(LL_ONE))
	{
		DynamicEffectObject* obj = dynamic_cast<DynamicEffectObject*>(actor);
		if(obj != NULL )
		{
			if(!saveClipDynamicEffectObjectRecursive(parentNode,obj,this->getID(), 
				consistentId, file_main_name, ani_player, isOptimize))
				return false;
		}
	}

	act_mng->reset_some_sysactor_iter(LL_TWO);
	while(Actor* actor = act_mng->next_some_sysactor(LL_TWO))
	{
		DynamicEffectObject* obj = dynamic_cast<DynamicEffectObject*>(actor);
		if(obj != NULL )
		{
			if(!saveClipDynamicEffectObjectRecursive(parentNode,obj,this->getID(), 
				consistentId, file_main_name, ani_player, isOptimize))
				return false;
		}
	}
	return true;
}
//�洢�ļ�
bool StageAnimationClip::save(const char* filename0, bool isBinary, bool consistentId, bool isOptimize)
{
	assert(!isBinary);//�в�֧��2����

	//ת���ָ���
	std::string fullFileName ;
	MiddleLayerFilePathHelper::convertSeperator(filename0,fullFileName);
	//ȡ����·��
	std::string working_dir ;
	MiddleLayerFilePathHelper::getWorkingPath(filename0,working_dir);

	//ȡ�ļ�����
	std::string fileMainName;

	std::string::size_type sperator_pos = fullFileName.rfind("/");
	if(sperator_pos == std::string::npos )//�޷ָ�����
	{
		std::string::size_type dot_pos = fullFileName.find(".");
		if(dot_pos == std::string::npos)//��.
			fileMainName = fullFileName;
		else
			fileMainName = fullFileName.substr( 0, dot_pos );
	}
	else//�зָ�����
	{
		std::string::size_type dot_pos = fullFileName.find(".", sperator_pos + 1);
		if(dot_pos ==  std::string::npos)//��.
			fileMainName = fullFileName.substr(sperator_pos + 1 );
		else
			fileMainName = fullFileName.substr(sperator_pos + 1 , dot_pos - (sperator_pos + 1));
	}


	//BiXmlDocument* doc = new BiXmlDocument(fullFileName.c_str());
	BiXmlDocument* doc = XmlDocFactory::CreateDocument();
	if (doc == NULL)
		return false;
	
	// �ҵ�Stage�ڵ�
	BiXmlNode* stageEffect = NULL;

	//BiXmlElement* stageElement = new BiXmlElement("StageEffect");
	BiXmlElement* stageElement = doc->InsertEndChild("StageEffect")->ToElement();

	assert(!fileMainName.empty());//ȷ�����Ʒǿ�
	stageElement->SetAttribute("id", fileMainName.c_str());

	stageElement->SetAttribute("len", this->getFrameCount());
	if(this->getClipType() == StageAnimationClip_Common)
	{
		stageElement->SetAttribute("type", "common");
	}
	else if(this->getClipType() == StageAnimationClip_Camera)
	{
		stageElement->SetAttribute("type", "camera");
	}
	else
		assert(!"StageAnimationClip save, clip���Ͳ�֧��!");

	stageEffect = stageElement;
	//doc->LinkEndChild(stageEffect);

	StageAnimationPlayer* ani_player = dynamic_cast<StageAnimationPlayer*>(m_ani_clip_mng->getStage()->getPlayer());
	
	//�������е�ϵͳ����
	if(!saveClipStageSystemObjects(stageEffect,consistentId, fileMainName.c_str(), isOptimize))
	{
		XmlDocFactory::ReleaseDocument(doc);
		return false;
	}

	//������������
	std::list<DynamicEffectObject*>::const_iterator iter = m_object_list.begin();
	for(; iter != m_object_list.end(); ++ iter)
	{
		//ȡ�ڵ�
		DynamicEffectObject* dynamic_effect_object = *iter; 
		if(!saveClipDynamicEffectObjectRecursive(stageEffect,dynamic_effect_object,this->getID(),consistentId, fileMainName.c_str(), ani_player, isOptimize))
		{
			//����ֱ�ӷ��أ�������
			XmlDocFactory::ReleaseDocument(doc);
			return false;
		}
	}

	bool ret=doc->SaveFile(fullFileName.c_str());
	//delete doc;
	XmlDocFactory::ReleaseDocument(doc);
	return ret;
};

void StageAnimationClip::playObj(DynamicEffectObject* obj, bool isPlay, float t)
{
	ActorAnimationControl* actor_control = obj->getAnimationControl();
	//����clip
	ActorAnimationClip* actor_ani_clip = actor_control->find_animation_clip(this->getID());
	//����clip
	if(actor_ani_clip)
	{
		actor_control->play(isPlay,t,actor_ani_clip);
		if(!isPlay && (m_ani_clip_type != StageAnimationClip_Camera))//ֹͣʱ���лָ�
		{
			DynEffObjControl* dyn_effobj_control = dynamic_cast<DynEffObjControl*>(actor_control);
			if(m_ani_clip_type !=StageAnimationClip_Camera )//��ǰ��Ӧ��cutting data
			{
				//ȡ��modifier ����
				if (m_current_cutting_data)
				{
					std::string nextClipId = "";
					if(m_next_cutting_data)
						nextClipId = m_next_cutting_data->m_animation_clip_id;
					std::map< std::pair<std::string,std::string>,std::set<IPropertyModifier*>>::const_iterator map_iter = dyn_effobj_control->m_reset_modifier_map.find(std::make_pair<std::string,std::string>(m_current_cutting_data->m_animation_clip_id,nextClipId));

					if(map_iter == dyn_effobj_control->m_reset_modifier_map.end())
						return;

					const std::set<IPropertyModifier*>& modifier_set = map_iter->second;

					//�ָ�ȱʡ����
					std::set<IPropertyModifier*>::const_iterator modifier_iter = modifier_set.begin();
					for(; modifier_iter!= modifier_set.end(); ++ modifier_iter)
					{
						(*modifier_iter)->execute(true,0.0f);//�����������ã��ָ�ȱʡ����
					}

					return;
				}

				//����clip
				ActorAnimationClip* default_property_ani_clip = actor_control->find_animation_clip(obj->getDefaultPropertyName());
				//����clip
				actor_control->play(true,0.0f,default_property_ani_clip);
				actor_control->play(false,0.0f,default_property_ani_clip);//ֹͣ

				ActorAnimationClip* stage_property_ani_clip = actor_control->find_animation_clip(obj->getStagePropertyName());
				//����clip
				if(stage_property_ani_clip)
				{
					actor_control->play(true,0.0f,stage_property_ani_clip);
					actor_control->play(false,0.0f,stage_property_ani_clip);//ֹͣ	
				}
			}
		}
	};

	obj->resetSubObjectIterator();
	while(DynamicEffectObject* subObj = obj->nextSubObject())
	{
		playObj(subObj, isPlay, t);
	}
}

//����
void StageAnimationClip::play(bool isPlay, float t)
{
	m_is_playing = isPlay;
	m_total_update_time = t;
	//if(isPlay)
	//	clLog("-----------clip %s begin ------------\n",this->getID());
	//else
	//	clLog("-----------clip %s end ------------\n",this->getID());
	std::list<DynamicEffectObject*>::const_iterator iter = m_object_list.begin();
	for(; iter != m_object_list.end(); ++ iter)
	{
		playObj(*iter, isPlay, t);
	};
};

bool StageAnimationClip::isPlaying()
{
	return m_is_playing;
}

//��Ⱦ
void StageAnimationClip::render()
{
	CRenderSystem* render_system =  m_ani_clip_mng->getStage()->getActorMng()->get_graphic_res_mng()->get_render_system();;

	std::list<DynamicEffectObject*>::const_iterator iter = m_object_list.begin();
	for(; iter != m_object_list.end(); ++ iter)
	{
		(*iter)->render(*render_system);
	}
};

void StageAnimationClip::set_visible(bool visible)
{
	std::list<DynamicEffectObject*>::const_iterator iter = m_object_list.begin();
	for(; iter != m_object_list.end(); ++ iter)
	{
		(*iter)->set_visible(visible);
	}
	m_is_visible = visible;
}

bool StageAnimationClip::is_visible()
{
	return m_is_visible;
}

//����Ӳ��ѡ����Ⱦ
unsigned int StageAnimationClip::pushHardwareSelect(bool sel_line)
{
	int count = 0;
	CRenderSystem* render_system =  m_ani_clip_mng->getStage()->getActorMng()->get_graphic_res_mng()->get_render_system();;

	std::list<DynamicEffectObject*>::const_iterator iter = m_object_list.begin();
	for(; iter != m_object_list.end(); ++ iter)
	{
		count += (*iter)->pushHardwareSelect(*render_system, sel_line);
	}

	return count;
};

//����, ��λ����
void StageAnimationClip::update(float t)
{
	std::list<DynamicEffectObject*>::const_iterator iter = m_object_list.begin();
	for(; iter != m_object_list.end(); ++ iter)
	{
		(*iter)->update(t);
		(*iter)->m_object_updated = true;//���ø��±�־����
	};

	m_total_update_time += t;
};

float StageAnimationClip::getTotalUpdateTime()
{
	return m_total_update_time;
}

void StageAnimationClip::setID(const char* id)
{
	//�޸�
	std::list<DynamicEffectObject*>::const_iterator iter = m_object_list.begin();
	for(; iter != m_object_list.end(); ++ iter)
	{
		ActorAnimationControl* actor_control = (*iter)->getAnimationControl();
		//����clip
		actor_control->rename_animation_clip(getID(), id);
	}
	m_id = id;

}

//���Ч������
void StageAnimationClip::insert(DynamicEffectObject* eff_obj)
{
	ActorAnimationControl* aac = eff_obj->getAnimationControl();
	if(!aac->find_animation_clip(this->getID())) // ���ͬ����clip
	{
		aac->add_animation_clip(this->getID(), new ActorAnimationClip());
		if (isPlaying())
			aac->play(true, 0, this->getID());
	}
	
	if (eff_obj->get_parent())
	{
		return;
	}

	std::list<DynamicEffectObject*>::const_iterator iter =  m_object_list.begin();
	for(; iter != m_object_list.end(); ++iter )
	{
		if(*iter == eff_obj)
			return;//�ظ���ӣ�����
	}
	m_object_list.push_back(eff_obj);
	resetObjectIterator() ;
};
//�Ƴ�Ч������
void StageAnimationClip::remove(DynamicEffectObject* eff_obj, bool remove_clip)
{
	ActorAnimationControl* aac = eff_obj->getAnimationControl();
	aac->play(false, 0, this->getID());
	
	if(remove_clip)//���м���ʱ�ƶ�ʱ��ɾ��clip
	{
		aac->remove_animation_clip(this->getID());
	}

	std::list<DynamicEffectObject*>::iterator iter =  m_object_list.begin();
	for(; iter != m_object_list.end(); ++iter )
	{
		if(*iter == eff_obj)
		{
			m_object_list.erase(iter);
			resetObjectIterator() ;
			return;
		}
	};
};
//����
DynamicEffectObject* StageAnimationClip::findEffectObject(const char* obj_id)
{
	std::list<DynamicEffectObject*>::const_iterator iter =  m_object_list.begin();
	for(; iter != m_object_list.end(); ++iter )
	{
		if( stricmp((*iter)->get_name(), obj_id) == 0)
		{
			return (*iter);
		}
	};
	return NULL;
};

StageAnimationClipMng::StageAnimationClipMng(IStage* stage)
:m_stage(stage)
{
	resetIterator();
};
StageAnimationClipMng::~StageAnimationClipMng()
{
	std::map<std::string,StageAnimationClip*>::iterator iter = ani_clip_map.begin();
	for(;iter != ani_clip_map.end(); ++iter)
	{
		delete iter->second;
	}
	ani_clip_map.clear();

#ifdef _EDITOR

	std::map<std::string,StageAnimationClip*>::iterator iter_common = m_common_camera_clips.begin();
	for(;iter_common != m_common_camera_clips.end(); ++iter_common)
	{
		delete iter_common->second;
	}
	m_common_camera_clips.clear();

	std::map<std::string,CameraEffectObject*>::iterator iter_cam = m_common_cameras.begin();
	for(;iter_cam != m_common_cameras.end(); ++iter_cam)
	{
		delete iter_cam->second;
	}
	m_common_cameras.clear();
#endif
	m_cameraPathMap.clear();
	m_clips_path_list.clear();
};

Stage* StageAnimationClipMng::getStage()
{
	return dynamic_cast<Stage*>(m_stage);
};

bool StageAnimationClipMng::parseStageEffect(BiXmlNode* rootNode, bool isBinary, const std::string& working_dir, I_Keep_Client_Alive* pLp)
{	
	BiXmlNode* item = 0;
	while (item = rootNode->IterateChildren("StageEffect", item))
	{
		if ( pLp )
		{
			pLp->update();
		}
		const char* id = BiXmlUtility::get_attribute(item, "id");
		if (id == NULL)
		{
			return false;
		}

		const char* FilePath = BiXmlUtility::get_attribute(item, "FilePath");
		if (FilePath == NULL)
		{
			return false;
		}
		//process create
		//create clip
		StageAnimationClip* ani_clip = new StageAnimationClip(this);

		std::string strTmp = working_dir + "/" +FilePath; 
		if(!ani_clip->load(strTmp.c_str(),isBinary))
		{
			delete ani_clip;
			return false;
		}
		//add to mng
		ani_clip_map.insert(std::make_pair(ani_clip->getID(),ani_clip));
		if(ani_clip->getClipType() == StageAnimationClip_Camera)
		{
			m_cameraPathMap[id] = strTmp;//��¼��ͷ·��
		}
	}
	return true;	
};

bool StageAnimationClipMng::createStagePropertyClip(const char* default_clip_name,const CDefinedItemCollection* defined_item_collection)
{
	std::string str_clip_name = default_clip_name;
	Stage* stage = dynamic_cast<Stage*>(m_stage);

	ActorManager* actor_manager =getStage()->getActorMng();
	actor_manager->reset_iterator();
	while(Actor* actor = actor_manager->next())//ȡ��������
	{
		//�������Щ����
		DynamicEffectObject* dynamic_effect_object = dynamic_cast<DynamicEffectObject*>(actor);
		assert(dynamic_effect_object);
		const DefinedItem* defined_item = defined_item_collection->getDefinedItem(dynamic_effect_object->get_name());
		assert(defined_item);
		if(!dynamic_effect_object->createAnimationClipByItem(default_clip_name,defined_item))
			return false;
	}
	return true;
}

void StageAnimationClipMng::removeSystemActorClip(LOD_LEVEL ll, const char* clipid)
{
	ActorManager* act_mng = m_stage->getActorMng();
	act_mng->reset_some_sysactor_iter(ll);
	while(Actor* act = act_mng->next_some_sysactor(ll))
	{
		DynamicEffectObject* dyn_obj = dynamic_cast<DynamicEffectObject*>(act);
		if (dyn_obj)
		{
			ActorAnimationControl* aac = dyn_obj->getAnimationControl();
			ActorAnimationClip* ani_clip = aac->find_animation_clip(clipid);
			if(ani_clip)
			{
				ani_clip->play(false,0.0f);
				aac->remove_animation_clip(clipid);
			}
		}
	}
}

void StageAnimationClipMng::renameSystemActorClip(
	LOD_LEVEL ll, const char* clipid, const char* newname)
{
	ActorManager* act_mng = m_stage->getActorMng();
	act_mng->reset_some_sysactor_iter(ll);
	while(Actor* act = act_mng->next_some_sysactor(ll))
	{
		DynamicEffectObject* dyn_obj = dynamic_cast<DynamicEffectObject*>(act);
		if (dyn_obj)
		{
			ActorAnimationControl* aac = dyn_obj->getAnimationControl();
			aac->rename_animation_clip(clipid, newname);
		}
	}
}

bool StageAnimationClipMng::loadDefaultStageEffects(const char* filename, bool isBinary)
{
	//����ȱʡ���Ե�clip 
	StageAnimationPlayer* player = dynamic_cast<StageAnimationPlayer*>(m_stage->getPlayer());

	assert(!isBinary);
	assert(ani_clip_map.size()==0);
	assert(m_clips_path_list.size() == 0);

	//ת���ָ���
	std::string strFileName ;
	MiddleLayerFilePathHelper::convertSeperator(filename,strFileName);
	
	BiXmlHelper::LoadXMLDocument(strFileName.c_str());
	BiXmlDocument* doc = BiXmlHelper::PeekXMLDocument(strFileName.c_str(), true);
	if(doc == NULL)
	{
		assert(0);
		return false;
	}

	// ��ȡ����
	BiXmlNode* stageRootNode = doc->FirstChild("Stage");
	if (stageRootNode == NULL)
	{
		XmlDocFactory::ReleaseDocument(doc);
		return false;
	}

	// ����ÿ�������ȱʡ��̨Ч���ڵ�
	BiXmlNode* dynamicEffectNodes = stageRootNode->FirstChild("SubEntitys");
	if (dynamicEffectNodes != NULL)
	{
		BiXmlNode* item = 0;
		while (item = dynamicEffectNodes->IterateChildren("DynamicEffectObject", item))
		{
			const char* id = BiXmlHelper::get_attribute(item,"id");
			assert(id);

			DynamicEffectObject* main_obj = dynamic_cast<DynamicEffectObject*>(getStage()->getActorMng()->find_actor(id));
			assert(main_obj);

			std::string actor_level;
			//����obj�� ani_control
			ActorAnimationControl* ani_control = main_obj->getAnimationControl();
			assert(ani_control);
			//������StageAniClipͬ���� ActorAniClip
			ActorAnimationClip* ani_clip = 	ani_control->find_animation_clip(DynamicEffectObject::getStagePropertyName());
			assert(ani_clip);
			PropertyModifierFactory* modifier_factory = getStage()->getActorMng()->getPropertyModifierFactory();//
			//�����������̨ȱʡ����
			bool ret = middleLayerCreateEffectRecursive(modifier_factory,
				item,
				main_obj,
				actor_level, 
				ani_clip,
				DynamicEffectObject::getStagePropertyName());
			if(!ret)
			{
				XmlDocFactory::ReleaseDocument(doc);
				return false;
			}

		}
	}

	XmlDocFactory::ReleaseDocument(doc);
	return true;
}

bool StageAnimationClipMng::loadEffectPathList(const char* filename, bool isBinary)
{
	//����ȱʡ���Ե�clip 
	StageAnimationPlayer* player = dynamic_cast<StageAnimationPlayer*>(m_stage->getPlayer());

	assert(!isBinary);
	assert(ani_clip_map.size()==0);
	assert(m_clips_path_list.size() == 0);

	//ת���ָ���
	std::string strFileName ;
	MiddleLayerFilePathHelper::convertSeperator(filename,strFileName);
	//ȡ����·��
	std::string working_path ;
	MiddleLayerFilePathHelper::getWorkingPath(filename,working_path);

	BiXmlHelper::LoadXMLDocument(strFileName.c_str());
	BiXmlDocument* doc = BiXmlHelper::PeekXMLDocument(strFileName.c_str(), true);
	if(doc == NULL)
	{
		assert(0);
		return false;
	}

	// ��ȡ����
	BiXmlNode* stageRootNode = doc->FirstChild("Stage");
	if (stageRootNode == NULL)
	{
		XmlDocFactory::ReleaseDocument(doc);
		return false;
	}

	// �����������̨Ч���ڵ��б�
	BiXmlNode* stageEffectNode = stageRootNode->FirstChild("StageEffectList");
	if (stageEffectNode != NULL)
	{
		BiXmlNode* item = 0;
		while (item = stageEffectNode->IterateChildren("StageEffect", item))
		{
			const char* id = BiXmlUtility::get_attribute(item, "id");
			if (id == NULL)
			{
				XmlDocFactory::ReleaseDocument(doc);
				return false;
			}

			const char* FilePath = BiXmlUtility::get_attribute(item, "FilePath");
			if (FilePath == NULL)
			{
				XmlDocFactory::ReleaseDocument(doc);
				return false;
			}

			std::string strTmp = working_path + "/" +FilePath; 
			m_clips_path_list.insert(std::make_pair(id, strTmp));
		}
	}

	XmlDocFactory::ReleaseDocument(doc);
	resetIterator();
	return true;	
}

//���뵥������̨Ч��
bool StageAnimationClipMng::loadStageEffect(const char* stageEffect, bool isBinary)
{
	assert(stageEffect && !isBinary);

	//�Ƿ��Ѿ�����
	std::map<std::string, StageAnimationClip*>::iterator itr = ani_clip_map.find(stageEffect);
	if (itr != ani_clip_map.end())
		return false;

	//����̨Ч���Ƿ����
	std::map<std::string, std::string>::iterator iter = m_clips_path_list.find(stageEffect);
	if (iter == m_clips_path_list.end())
		return false;

	StageAnimationClip* ani_clip = new StageAnimationClip(this);

	if(!ani_clip->load(iter->second.c_str(),isBinary))
	{
		delete ani_clip;
		return false;
	}

	ani_clip_map.insert(std::make_pair(ani_clip->getID(),ani_clip));
	if(ani_clip->getClipType() == StageAnimationClip_Camera)
	{
		m_cameraPathMap[iter->first] = iter->second;//��¼��ͷ·��
	}

	return true;
}

bool StageAnimationClipMng::load(const char* filename0, bool isBinary, I_Keep_Client_Alive* pLp)
{
	//����ȱʡ���Ե�clip 
	StageAnimationPlayer* player = dynamic_cast<StageAnimationPlayer*>(m_stage->getPlayer());

	assert(!isBinary);
	assert(ani_clip_map.size()==0);

	//ת���ָ���
	std::string strFileName ;
	MiddleLayerFilePathHelper::convertSeperator(filename0,strFileName);
	//ȡ����·��
	std::string working_path ;
	MiddleLayerFilePathHelper::getWorkingPath(filename0,working_path);

	BiXmlHelper::LoadXMLDocument(strFileName.c_str());
	BiXmlDocument* doc = BiXmlHelper::PeekXMLDocument(strFileName.c_str(), true);
	if(doc == NULL)
	{
		//LogError("[StageAnimationClipMng::load] load clip file failed:",strFileName.c_str());
		assert(0);
		return false;
	}

	// ��ȡ����
	BiXmlNode* stageRootNode = doc->FirstChild("Stage");
	if (stageRootNode == NULL)
	{
		//delete doc;
		XmlDocFactory::ReleaseDocument(doc);
		return false;
	}
	// ��ȡ��̨��̬Ч������Ľڵ�
	BiXmlNode* dynamicEffectNodes = stageRootNode->FirstChild("SubEntitys");
	if (dynamicEffectNodes != NULL)
	{
		BiXmlNode* item = 0;
		while (item = dynamicEffectNodes->IterateChildren("DynamicEffectObject", item))
		{
			if ( pLp )
			{
				pLp->update();
			}
			const char* id = BiXmlHelper::get_attribute(item,"id");
			assert(id);

			DynamicEffectObject* main_obj = dynamic_cast<DynamicEffectObject*>(getStage()->getActorMng()->find_actor(id));
			assert(main_obj);

			std::string actor_level;
			//����obj�� ani_control
			ActorAnimationControl* ani_control = main_obj->getAnimationControl();
			assert(ani_control);
			//������StageAniClipͬ���� ActorAniClip
			ActorAnimationClip* ani_clip = 	ani_control->find_animation_clip(DynamicEffectObject::getStagePropertyName());
			assert(ani_clip);
			
			PropertyModifierFactory* modifier_factory = getStage()->getActorMng()->getPropertyModifierFactory();//
			//�����������̨ȱʡ����
			bool ret = middleLayerCreateEffectRecursive(modifier_factory,
											   item,
											   main_obj,
											   actor_level, 
											   ani_clip,
											   DynamicEffectObject::getStagePropertyName());
			if(!ret)
			{
				//delete doc;
				XmlDocFactory::ReleaseDocument(doc);
				return false;
			}

		}
	}
	// ��ȡ��̨Ч���Ľڵ�
	BiXmlNode* stageEffectNode = stageRootNode->FirstChild("StageEffectList");
	if (stageEffectNode != NULL)
	{
		if (!parseStageEffect(stageEffectNode, isBinary, working_path, pLp))
		{
			//delete doc;
			XmlDocFactory::ReleaseDocument(doc);
			return false;
		}
	}

	//delete doc;
	XmlDocFactory::ReleaseDocument(doc);
	resetIterator();
	return true;	
};
bool StageAnimationClipMng::save(const char* filename0, bool isBinary, bool isOptimize)
{
	assert(!isBinary);//�в�֧��2����
	//BiXmlDocument* doc = new BiXmlDocument();
	BiXmlDocument* doc = XmlDocFactory::CreateDocument();
	if (doc == NULL)
		return false;

	//ת���ָ���
	std::string strFileName ;
	MiddleLayerFilePathHelper::convertSeperator(filename0,strFileName);
	//ȡ����·��
	std::string working_path ;
	MiddleLayerFilePathHelper::getWorkingPath(filename0,working_path);

	doc->LoadFile(strFileName.c_str());

	// �ҵ�Stage�ڵ�
	BiXmlNode* stageNode = doc->FirstChild("Stage");
	if (stageNode == NULL)
	{
		/*stageNode = new BiXmlElement("Stage");
		doc->LinkEndChild(stageNode);*/
		stageNode = doc->InsertEndChild("Stage");
		
	}

	BiXmlNode* stageEffectNodes = stageNode->FirstChild("StageEffectList");
	if (stageEffectNodes == NULL)
	{
		/*stageEffectNodes = new BiXmlElement("StageEffectList");
		stageNode->LinkEndChild(stageEffectNodes);*/
		stageEffectNodes = stageNode->InsertEndChild("StageEffectList");
		
	}
	stageEffectNodes->Clear();
	// �����̨Ч��

	for (std::map<std::string,StageAnimationClip*>::iterator i = ani_clip_map.begin(); i != ani_clip_map.end(); ++i)
	{
		//BiXmlElement item("StageEffect");
		BiXmlElement* item = stageEffectNodes->InsertEndChild("StageEffect")->ToElement();

		item->SetAttribute("id", i->first.c_str());
		std::string strPath = i->first.c_str();
		strPath += ".xml";
		item->SetAttribute("FilePath", strPath.c_str());  

		//stageEffectNodes->InsertEndChild(item);

		std::string fullPath = working_path + "/" + strPath;
		//�洢��̨Ч���ļ�
		i->second->save(fullPath.c_str(),isBinary, false, isOptimize);//���ݼ�·�����ļ���
	}

	bool ret = doc->SaveFile(strFileName.c_str());
	//delete doc;
	XmlDocFactory::ReleaseDocument(doc);
	return ret;
};

// �����������͵���̨Ч��
StageAnimationClip* StageAnimationClipMng::createStageAnimationClip(StageAnimationClipType t, const char* clipid)
{
	StageAnimationClip* ani_clip = new StageAnimationClip(this);
	ani_clip->setID(clipid);
	ani_clip->setClipType(t);
	return ani_clip;
};
// ���ٸ�������̨Ч��
void StageAnimationClipMng::releaseStageAnimationClip(StageAnimationClip* stageaniclip)
{
	delete stageaniclip;
	return;
};

// ���һ����̨Ч��
void StageAnimationClipMng::insert(StageAnimationClip* stageaniclip)
{
	std::map<std::string,StageAnimationClip*>::const_iterator iter = ani_clip_map.find(stageaniclip->getID());
	assert(iter == ani_clip_map.end());
	
	ani_clip_map.insert(std::make_pair(stageaniclip->getID(),stageaniclip));
	resetIterator();
};
// ɾ��һ����̨Ч��
void StageAnimationClipMng::remove(StageAnimationClip* stageaniclip)
{
	std::map<std::string,StageAnimationClip*>::iterator iter = ani_clip_map.find(stageaniclip->getID());
	
	if(iter != ani_clip_map.end())
	{
		//�Ƴ�ani_clip
		stageaniclip->resetObjectIterator();
		while(DynamicEffectObject* eff_obj = stageaniclip->nextObject())
		{
			ActorAnimationControl* aac = eff_obj->getAnimationControl();
			ActorAnimationClip* ani_clip = aac->find_animation_clip(stageaniclip->getID());
			if(ani_clip)
			{
				ani_clip->play(false,0.0f);
				aac->remove_animation_clip(stageaniclip->getID());
			}
		}
		//ɾ����¼
		ani_clip_map.erase(iter);
		resetIterator();

		//ɾ��lod�����ϵ���̨Ч��
		removeSystemActorClip(LL_ZERO, stageaniclip->getID());
		removeSystemActorClip(LL_ONE, stageaniclip->getID());
		removeSystemActorClip(LL_TWO, stageaniclip->getID());
		return;
	}
}

void StageAnimationClipMng::renameClip(StageAnimationClip* clip, const char* newname)
{
	std::map<std::string,StageAnimationClip*>::iterator iter = ani_clip_map.find(clip->getID());
	std::string oldname = clip->getID();
	if(iter != ani_clip_map.end())
	{
		ani_clip_map.erase(iter);
		resetIterator();
		clip->setID(newname);
		renameSystemActorClip(LL_ZERO, oldname.c_str(), newname);
		renameSystemActorClip(LL_ONE, oldname.c_str(), newname);
		renameSystemActorClip(LL_TWO, oldname.c_str(), newname);
		insert(clip);
	}
}

//����ָ������̨Ч��
StageAnimationClip* StageAnimationClipMng::find(const char* clipid)
{
	std::map<std::string,StageAnimationClip*>::const_iterator iter = ani_clip_map.find(clipid);
#ifdef _EDITOR
	//������̨Ч�����ң����û�У�������ͨ���ļ�����
	if(iter == ani_clip_map.end())
	{
		std::map<std::string,StageAnimationClip*>::const_iterator iter_common = m_common_camera_clips.find(clipid);
		if (iter_common == m_common_camera_clips.end())
		{
			return NULL;
		}
		return iter_common->second;
	}
#else

	if(iter == ani_clip_map.end())
		return NULL;

#endif
	

	return iter->second;
};

// ������̨Ч��������
void StageAnimationClipMng::resetIterator()
{
	m_clip_pos = ani_clip_map.begin();
} ;
// �����̨Ч����������ָ����һ����̨Ч�������������Ч����0
StageAnimationClip* StageAnimationClipMng::next()
{
	if(m_clip_pos != ani_clip_map.end())
	{
		StageAnimationClip* ret_clip = m_clip_pos->second;
		m_clip_pos++;
		return ret_clip;
	}
	else
		return 0;
};

bool StageAnimationClipMng::generateStagePropertyInfo(BiXmlNode* effect_object_node, DynamicEffectObject* dynamic_effect_object  )
{ 
	bool is_camera_object = false;
	CameraEffectObject* camera_object = dynamic_cast<CameraEffectObject*>(dynamic_effect_object);
	if(camera_object != NULL)//���м��ʲ���
		is_camera_object = true;
	//ȡ��������Ϣ
	ActorAnimationControl* 	ani_control = dynamic_effect_object->getAnimationControl();
	assert(ani_control);
	//ȡstage�������õ�ani_clip
	ActorAnimationClip* ani_clip = ani_control->find_animation_clip(DynamicEffectObject::getStagePropertyName());//
	if(ani_clip)
	{
		// ���������޸���������
		ani_clip->reset_iterator();
		PropertyModifierType modifier_type;
		// ��������޸�����������ָ����һ�������޸��������������Ч����0
		while(IPropertyModifier* property_modifier = ani_clip->next(modifier_type))
		{
			IPropertyModifierStreamable* s_midifier = dynamic_cast<IPropertyModifierStreamable*>(property_modifier);
			assert(s_midifier);

			const char* value = s_midifier->get_value();
			assert(value);
			//�������ͽ���XML�ڵ�
			if(!generateObjectProperty(effect_object_node, &modifier_type, property_modifier, is_camera_object))//
				return false;
		}	
	}
	return true;
};
//�ӹؿ��ļ����ȡ��̨Ч��
bool StageAnimationClipMng::loadEffect(const char* fileName, bool isBinary)
{
	//process create
	//create clip
	StageAnimationClip* ani_clip = new StageAnimationClip(this);

	std::string strTmp = fileName; 
	if(!ani_clip->load(strTmp.c_str(),isBinary))
	{
		delete ani_clip;
		return false;
	}
	//add to mng
	ani_clip_map.insert(std::make_pair(ani_clip->getID(),ani_clip));
	if(ani_clip->getClipType() == StageAnimationClip_Camera)
	{
		m_cameraPathMap[ani_clip->getID()] = strTmp;//��¼��ͷ·��
	}
	return true;
};
//����idȡ����ļ�
const char* StageAnimationClipMng::getCameraFile(const char* cameraName)
{
	std::map<std::string,std::string>::const_iterator iter = m_cameraPathMap.find(cameraName);
	if(iter == m_cameraPathMap.end())
		return NULL;
	return iter->second.c_str();
};

#ifdef _EDITOR
	
bool StageAnimationClipMng::loadCommonCamera()
{
	std::map<std::string, std::string>::iterator itr = m_common_camera_pathes.begin();
	while(itr != m_common_camera_pathes.end())
	{
		StageAnimationClip* ani_clip = new StageAnimationClip(this);
		CameraEffectObject* camera_effect_object = 	dynamic_cast<CameraEffectObject*>
			(getStage()->createDefinedObject(itr->first.c_str(), "x52_Camera"));
		
		if(!camera_effect_object)
			return false;

		//���ز�ִ��ȱʡЧ���������
		if(!ani_clip->externalLoad(getStage()->getActorMng(), 
			camera_effect_object, itr->second.c_str(), false))
		{
			return false;
		}

		m_common_cameras.insert(std::make_pair(itr->first, camera_effect_object));
		m_common_camera_clips.insert(std::make_pair(itr->first, ani_clip));

		++itr;
	}

	return true;
}
/**
@brief ����ͨ�þ�ͷ����
*/
bool StageAnimationClipMng::parseCameraFile(const char* cameraFile)
{
	// �򿪹ؿ��ļ�
	BiXmlDocument* doc = XmlDocFactory::CreateDocument();

	if(!doc->LoadFile(cameraFile))
	{
		XmlDocFactory::ReleaseDocument(doc);
		return false;
	}
	BiXmlNode* rootNode = doc->FirstChild("common");
	if(!rootNode)
		return false;

	// ���������Ϣ
	BiXmlNode* item = 0;
	while (item = rootNode->IterateChildren("camitem", item))
	{
		const char* id = BiXmlUtility::get_attribute(item, "id");
		if (id == NULL)
		{
			return false;
		}
		const char* script = BiXmlUtility::get_attribute(item, "script");
		if(script == NULL)
			return false;

		m_common_camera_pathes[id] = script;
	}
	return true;
}

CameraEffectObject* StageAnimationClipMng::getCommonCamera(const char* strCameraName)
{
	std::map<std::string,CameraEffectObject*>::iterator itr = m_common_cameras.find(strCameraName);
	if ( itr != m_common_cameras.end())
		return itr->second;
	return NULL;
}

StageAnimationClip* StageAnimationClipMng::getCommonCameraClip(const char* strClipName)
{
	std::map<std::string,StageAnimationClip*>::iterator itr = m_common_camera_clips.find(strClipName);
	if (itr != m_common_camera_clips.end())
		return itr->second;
	return NULL;
}


#endif
