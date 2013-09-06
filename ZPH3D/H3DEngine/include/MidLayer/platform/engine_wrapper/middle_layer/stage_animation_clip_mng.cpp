/**
* @file stage_animation_clip_mng.h
* 这个文件的主要功能是：
* 
* @author 李汕
* 
* 更改日志
* ----------------------------------------------------
* ver 1.0.0
*       by 李汕2010.11.16
*/
#include "middle_layer_body.h"
/// ModifierTypeID的比较器
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
// 重置物体信息迭代器
void StageAnimationClip::resetObjectIterator() 
{
	m_object_list_iter = m_object_list.begin();
};
// 获得物体信息，迭代器指向下一个物体信息，如迭代器无效返回0
DynamicEffectObject* StageAnimationClip::nextObject()
{
	if(m_object_list_iter != m_object_list.end())
		return *(m_object_list_iter++);
	else
		return 0;
};
//添加属性修改器
static bool addPropertyModifier(PropertyModifierFactory* modifier_factory
								,BiXmlNode* propertyNode
								,const char* actor_level
								,const char* child_entity_id
								,bool is_curv
								,DynamicEffectObject* top_effect_obj
								,ActorAnimationClip* ani_clip)
{
	//读属性
	const char* property_name = BiXmlHelper::get_attribute(propertyNode,"key");
	const char* fixed_parm = BiXmlHelper::get_attribute(propertyNode,"fixed_param");
	const char* value = BiXmlHelper::get_attribute(propertyNode,"value");
	if(value == NULL)
		value = BiXmlHelper::get_attribute(propertyNode,"default");//取缺省值

	std::string curve_str;
	if(is_curv)//序列进行文件读取
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

	//设置modifier type
	PropertyModifierType modifier_type;
	modifier_type.child_actor_id = actor_level;//actor层次，只包括子actor的
	modifier_type.child_entity_id = child_entity_id;//实体名称

	modifier_type.property_name = property_name;//属性名称;
	if(fixed_parm)
		modifier_type.property_fixed_param = fixed_parm;//固定参数 /parm1/parm2/..;
	modifier_type.is_curv = is_curv;//是否曲线

	//建立modifier
	IPropertyModifier* modifier = modifier_factory->create_modifier(top_effect_obj,modifier_type);
	if(modifier)
	{
		IPropertyModifierStreamable* s_modifier = dynamic_cast<IPropertyModifierStreamable*>(modifier);
		assert(s_modifier);
		if(is_curv)//曲线
		{
			if(!curve_str.empty())
				s_modifier->set_value(curve_str.c_str());//设置曲线内容
		}
		else
		{
			if(value != NULL)
			{
				s_modifier->set_value(value);//设置属性值
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
//递归建立动态效果物体和子实体
static bool middleLayerCreateEffectRecursive(PropertyModifierFactory* modifier_factory,
											   BiXmlNode* xmlNode,
											   DynamicEffectObject* top_effect_obj,
											   std::string& actor_level, 
											   ActorAnimationClip* ani_clip,
											   const char* effect_id)
{
	//建立属性
	BiXmlNode* propertyInfoNode = xmlNode->FirstChild("PropertyInfo");
	if( propertyInfoNode != NULL)
	{
		BiXmlNode* propertyNode = propertyInfoNode->FirstChild("Property");
		while(propertyNode != NULL )
		{
			//属性解析
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
			//曲线属性解析
			if(!addPropertyModifier(modifier_factory, propertyNode, actor_level.c_str(), "",true ,top_effect_obj,ani_clip))
				return false;
			propertyNode = propertyNode->NextSibling("Property");
		}
	}
	//建立子实体
	BiXmlNode* entityList = xmlNode->FirstChild("EntityList");
	if(entityList!=NULL)
	{
		BiXmlNode* entityNode = entityList->FirstChild("SubEntity");
		while(entityNode != NULL)
		{
			const char* entity_name = BiXmlUtility::get_attribute(entityNode, "name");
			assert(entity_name != NULL );

			//建立属性
			BiXmlNode* propertyInfoNode = entityNode->FirstChild("PropertyInfo");
			if( propertyInfoNode != NULL)
			{
				BiXmlNode* propertyNode = propertyInfoNode->FirstChild("Property");
				while(propertyNode != NULL )
				{
					//属性解析
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
					//曲线属性解析
					if(!addPropertyModifier(modifier_factory, propertyNode, actor_level.c_str(), entity_name,true ,top_effect_obj,ani_clip))
						return false;
					propertyNode = propertyNode->NextSibling("Property");
				}
			}

			entityNode = entityNode->NextSibling("SubEntity");
		}
	}
	
	//建立子actor
	BiXmlNode* dynamicEffectObjectList = xmlNode->FirstChild("DynamicEffectObjectList");
	if(dynamicEffectObjectList != NULL)
	{
		BiXmlNode* dynamicEffectObjectNode = dynamicEffectObjectList->FirstChild("DynamicEffectObject");
		while(dynamicEffectObjectNode != NULL)
		{
			//递归建立modifier
			const char* sub_object_id = BiXmlHelper::get_attribute(dynamicEffectObjectNode,"id");
			//加上自己的层次ID
			std::string sub_actor_level = actor_level + "/" + sub_object_id;
			
			if(!middleLayerCreateEffectRecursive(modifier_factory ,dynamicEffectObjectNode, top_effect_obj,sub_actor_level, ani_clip, effect_id))
				return false;

			dynamicEffectObjectNode = dynamicEffectObjectNode->NextSibling("DynamicEffectObject");
		}
	}
	//建立级联物体属性
	BiXmlNode* linkObjectList = xmlNode->FirstChild("LinkDynamicEffectObjectList");
	if(linkObjectList != NULL)
	{
		BiXmlNode* subEffectObjectNode = linkObjectList->FirstChild("DynamicEffectObject");
		while( subEffectObjectNode != NULL)
		{
			//递归建立modifier
			const char* sub_object_id = BiXmlHelper::get_attribute(subEffectObjectNode,"id");
			assert(sub_object_id);
			//找到级联的物体
			DynamicEffectObject* sub_obj = dynamic_cast<DynamicEffectObject*>(top_effect_obj->find_actor(sub_object_id));
			assert(sub_obj);
			//建立obj的 ani_control
			ActorAnimationControl* sub_ani_control = sub_obj->getAnimationControl();
			assert(sub_ani_control);
			//建立与StageAniClip同名的 ActorAniClip
			ActorAnimationClip* sub_ani_clip = sub_ani_control->find_animation_clip(effect_id);
			
			if(sub_ani_clip == NULL)
			{
				sub_ani_clip = new ActorAnimationClip();
				sub_ani_control->add_animation_clip(effect_id, sub_ani_clip);//ActorAniClip与 舞台效果同名
			}
			//加上自己的层次ID
			std::string sub_actor_level ;

			//在级联物体上建立属性
			if(!middleLayerCreateEffectRecursive(modifier_factory ,subEffectObjectNode, sub_obj ,sub_actor_level, sub_ani_clip, effect_id))
				return false;

			subEffectObjectNode = subEffectObjectNode->NextSibling("DynamicEffectObject");
		}
	}
	return true;
}

bool StageAnimationClip::createDynamicEffect(BiXmlNode* effectNode)
{	//根据effect文件信息建立DynamicEffect
	BiXmlNode* item = 0;
	const char* object_id = BiXmlHelper::get_attribute(effectNode,"id");
	//顶层的做些处理
	DynamicEffectObject* obj = dynamic_cast<DynamicEffectObject*>(m_ani_clip_mng->getStage()->getActorMng()->find_actor(object_id));

	//如果没有与效果相匹配的物体，则跳过，直接返回。
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
		m_object_list.push_back(obj);//建立对物体的引用
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

	//建立obj的 ani_control
	ActorAnimationControl* ani_control = obj->getAnimationControl();
	assert(ani_control);
	//建立与StageAniClip同名的 ActorAniClip
	ActorAnimationClip* ani_clip = ani_control->find_animation_clip(this->getID());
	
	if(ani_clip == NULL)
	{
		ani_clip = new ActorAnimationClip();
		ani_control->add_animation_clip(this->getID(), ani_clip);//ActorAniClip与 舞台效果同名
	}

	std::string actor_level;
	//建立actor	
	PropertyModifierFactory* modifier_factory = m_ani_clip_mng->getStage()->getActorMng()->getPropertyModifierFactory();//
	bool ret = middleLayerCreateEffectRecursive(modifier_factory, effectNode,obj,actor_level,ani_clip, this->getID());

	return ret;
};
bool StageAnimationClip::externalCreateDynamicEffect(ActorManager* actorManager,DynamicEffectObject* effectObj, BiXmlNode* effectNode)
{	//根据effect文件信息建立DynamicEffect
	BiXmlNode* item = 0;
	const char* object_id = BiXmlHelper::get_attribute(effectNode,"id");
	//顶层的做些处理
	DynamicEffectObject* obj = effectObj;
	assert(obj != NULL);
	m_object_list.push_back(obj);//建立对物体的引用

	//建立obj的 ani_control
	ActorAnimationControl* ani_control = obj->getAnimationControl();
	assert(ani_control);
	//建立与StageAniClip同名的 ActorAniClip
	ActorAnimationClip* ani_clip = ani_control->find_animation_clip(this->getID());
	
	if(ani_clip == NULL)
	{
		ani_clip = new ActorAnimationClip();
		ani_control->add_animation_clip(this->getID(), ani_clip);//ActorAniClip与 舞台效果同名
	}

	std::string actor_level;
	//建立actor	
	PropertyModifierFactory* modifier_factory = actorManager->getPropertyModifierFactory();//
	bool ret = middleLayerCreateEffectRecursive(modifier_factory, effectNode,obj,actor_level,ani_clip, this->getID());

	return ret;
};

static bool removeDynamicEffectObjectRecursive(BiXmlElement* obj, const char* id)
{
	const char* obj_id = obj->Attribute("id");

	//如果是自己，则删除并返回
	if(stricmp(obj_id, id) == 0)
	{
		BiXmlNode* parent = const_cast<BiXmlNode*>(obj->Parent());
		parent->RemoveChild(obj);
		return true;
	}

	//删除子物体
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
		//如果节点为空，则删除自己
		if (!ObjList->FirstChild())
		{
			obj->RemoveChild(ObjList);
		}
	}

	//删除级联物体
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
		//如果节点为空，则删除自己
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

		//这一步会改变dom树的树形结构，所以在执行这一步之前先去下一个兄弟节点。
		if(!removeDynamicEffectObjectRecursive(temp, id))
			return false;
	}

	bool bRet = doc->SaveFile(filename);

	XmlDocFactory::ReleaseDocument(doc);

	return bRet;
}

//解析文件,返回clip id
std::string StageAnimationClip::getFileClipId(const char* filename0, bool isBinary)
{
	//create dynamic effect object
	assert(!isBinary);
	std::string strClipId;
	//转换分隔符
	std::string fullFileName ;
	MiddleLayerFilePathHelper::convertSeperator(filename0,fullFileName);

	BiXmlHelper::LoadXMLDocument(fullFileName.c_str());
	BiXmlDocument* doc = BiXmlHelper::PeekXMLDocument(fullFileName.c_str(), true);
	if(doc == NULL)
	{
		assert(0);
		return strClipId;
	}
	// 读取序列
	BiXmlNode* stageEffectNode = doc->FirstChild("StageEffect");
	if (stageEffectNode == NULL)
	{
		//delete doc;
		XmlDocFactory::ReleaseDocument(doc);
		return strClipId;
	}
	//设置ID
	const char* id = BiXmlUtility::get_attribute(stageEffectNode, "id");
	assert(id);
	strClipId = id;

	//delete doc;
	XmlDocFactory::ReleaseDocument(doc);
	return strClipId;
};

//解析文件,返回clip type
std::string StageAnimationClip::getFileClipType(const char* filename0, bool isBinary)
{
	//create dynamic effect object
	assert(!isBinary);
	std::string strClipType;
	//转换分隔符
	std::string fullFileName ;
	MiddleLayerFilePathHelper::convertSeperator(filename0,fullFileName);

	BiXmlHelper::LoadXMLDocument(fullFileName.c_str());
	BiXmlDocument* doc = BiXmlHelper::PeekXMLDocument(fullFileName.c_str(), true);
	if(doc == NULL)
	{
		assert(0);
		return strClipType;
	}
	// 读取序列
	BiXmlNode* stageEffectNode = doc->FirstChild("StageEffect");
	if (stageEffectNode == NULL)
	{
		//delete doc;
		XmlDocFactory::ReleaseDocument(doc);
		return strClipType;
	}
	//设置类型
	const char* type = BiXmlUtility::get_attribute(stageEffectNode, "type");
	assert(type);
	strClipType = type;

	//delete doc;
	XmlDocFactory::ReleaseDocument(doc);
	return strClipType;
};

//设置相机后处理切换信息
void StageAnimationClip::setPostProcessSwitchInfo(const char* filename0)
{
	//转换分隔符
	std::string fullFileName ;
	MiddleLayerFilePathHelper::convertSeperator(filename0,fullFileName);


	BiXmlHelper::LoadXMLDocument(fullFileName.c_str());
	BiXmlDocument* doc = BiXmlHelper::PeekXMLDocument(fullFileName.c_str(), true);
	if(doc == NULL)
	{
		return ;
	}
	// 读取序列
	BiXmlNode* postSwitchNode = doc->FirstChild("PostEffectSwitch");
	if (postSwitchNode == NULL)
	{
		//delete doc;
		XmlDocFactory::ReleaseDocument(doc);
		return ;
	}
	// 读取动态效果物体的节点
	BiXmlNode* switchNode = postSwitchNode->FirstChild("Switch");
	while(switchNode != NULL)
	{//解析舞台效果
		const char* effect_name = BiXmlHelper::get_attribute(switchNode,"EffectName");
		const char* effect_valid = BiXmlHelper::get_attribute(switchNode,"Valid");
		assert(effect_name && effect_valid);
		//设置后处理开关
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
//解析文件
bool StageAnimationClip::load(const char* filename0, bool isBinary)
{
	//create dynamic effect object
	assert(!isBinary);
	if(!m_object_list.empty())
	{
		assert(0);
		return false;
	}

	//转换分隔符
	std::string fullFileName ;
	MiddleLayerFilePathHelper::convertSeperator(filename0,fullFileName);
	//取工作路径
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
	// 读取序列
	BiXmlNode* stageEffectNode = doc->FirstChild("StageEffect");
	if (stageEffectNode == NULL)
	{
		//delete doc;
		XmlDocFactory::ReleaseDocument(doc);
		return false;
	}
	//设置ID
	const char* id = BiXmlUtility::get_attribute(stageEffectNode, "id");
	assert(id);
	setID(id);
	//读取长度
	const char* stage_effect_len = BiXmlUtility::get_attribute(stageEffectNode, "len");
	m_frame_count = atoi(stage_effect_len);
	//读取类型
	const char* stage_effect_type = BiXmlUtility::get_attribute(stageEffectNode, "type");
	if(stage_effect_type != NULL)
	{
		if(stricmp(stage_effect_type, "common") == 0)
			setClipType(StageAnimationClip_Common);
		else if(stricmp(stage_effect_type, "camera") == 0)
			setClipType(StageAnimationClip_Camera);
		else
			assert(false && "StageAnimationClip parse, clip类型不支持!");
	}
	// 读取动态效果物体的节点
	BiXmlNode* dynamicEffectObjectNode = stageEffectNode->FirstChild("DynamicEffectObject");
	while(dynamicEffectObjectNode != NULL)
	{//解析舞台效果
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
	//读取舞台效果切换设置
	if(getClipType() == StageAnimationClip_Camera)
	{
		std::string strSwitchFile = filename0;

		MiddleLayerStrHelper::replaceStr(strSwitchFile,".xml","_PostEffectSwitch.xml");

		setPostProcessSwitchInfo(strSwitchFile.c_str());
	}
	//读取舞台效果切换设置over
	return true;
};
//给外部建立StageAnimationClip用，添加额外的代码 lishan 110624
bool StageAnimationClip::externalLoad(ActorManager* actorManager, DynamicEffectObject* effectObj,const char* filename0, bool isBinary)
{
	assert(!isBinary);
	if(!m_object_list.empty())
	{
		assert(0);
		return false;
	}

	//转换分隔符
	std::string fullFileName ;
	MiddleLayerFilePathHelper::convertSeperator(filename0,fullFileName);
	//取工作路径
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
	// 读取序列
	BiXmlNode* stageEffectNode = doc->FirstChild("StageEffect");
	if (stageEffectNode == NULL)
	{
		//delete doc;
		XmlDocFactory::ReleaseDocument(doc);
		return false;
	}
	//设置ID
	const char* id = BiXmlUtility::get_attribute(stageEffectNode, "id");
	assert(id);
	setID(id);
	//读取长度
	const char* stage_effect_len = BiXmlUtility::get_attribute(stageEffectNode, "len");
	m_frame_count = atoi(stage_effect_len);
	//读取类型
	const char* stage_effect_type = BiXmlUtility::get_attribute(stageEffectNode, "type");
	if(stage_effect_type != NULL)
	{
		if(stricmp(stage_effect_type, "common") == 0)
			setClipType(StageAnimationClip_Common);
		else if(stricmp(stage_effect_type, "camera") == 0)
			setClipType(StageAnimationClip_Camera);
		else
			assert(!"StageAnimationClip parse, clip类型不支持!");
	}
	// 读取动态效果物体的节点
	BiXmlNode* dynamicEffectObjectNode = stageEffectNode->FirstChild("DynamicEffectObject");
	while(dynamicEffectObjectNode != NULL)
	{//解析舞台效果
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
	//读取舞台效果切换设置
	if(getClipType() == StageAnimationClip_Camera)
	{
		std::string strSwitchFile = filename0;

		MiddleLayerStrHelper::replaceStr(strSwitchFile,".xml","_PostEffectSwitch.xml");

		setPostProcessSwitchInfo(strSwitchFile.c_str());
	}
	//读取舞台效果切换设置over
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
	//先找到插入节点
	std::list<std::string> sub_actor_list;
	divide_actor_params(modifier_type->child_actor_id,sub_actor_list);

	//查找目标actor
	BiXmlNode* target_actor_node = effect_node;
	for(std::list<std::string>::const_iterator iter = sub_actor_list.begin(); iter != sub_actor_list.end() ; ++iter)
	{
		//查找actor list节点
		BiXmlNode* actor_list_node = target_actor_node->FirstChild("DynamicEffectObjectList");
		if (actor_list_node == NULL)
		{
			//actor_list_node = new BiXmlElement("DynamicEffectObjectList");
			//target_actor_node->LinkEndChild(actor_list_node);//如果未找到，插入新节点
			actor_list_node = target_actor_node->InsertEndChild("DynamicEffectObjectList");
		}
		//查找 actor 节点
		BiXmlNode* actor_node = actor_list_node->FirstChild("DynamicEffectObject");
		while(actor_node)
		{
			const char* actor_node_id = BiXmlHelper::get_attribute(actor_node,"id");//根据actor id 查找节点
			if( (*iter) == actor_node_id)//已有节点
			{
				break;
			}
			actor_node = actor_node->NextSibling("DynamicEffectObject");//查找下一个
		}
		if( actor_node == NULL)//还没有节点，添加1个
		{
			//actor_node = new BiXmlElement("DynamicEffectObject");
			//actor_list_node->LinkEndChild(actor_node);//如果未找到，插入新节点
			actor_node = actor_list_node->InsertEndChild("DynamicEffectObject");
		}
		//进行下一级查找
		target_actor_node = actor_node;
	}
	//查找目标位置
	BiXmlNode* target_node_position = target_actor_node;//设置处理位置为actor
	if(!modifier_type->child_entity_id.empty()) 
	{
		//查找entity list节点
		BiXmlNode* entity_list_node = target_actor_node->FirstChild("EntityList");
		if (entity_list_node == NULL)
		{
			//entity_list_node = new BiXmlElement("EntityList");
			//target_actor_node->LinkEndChild(entity_list_node);//如果未找到，插入新节点
			entity_list_node = target_actor_node->InsertEndChild("EntityList");
			;//如果未找到，插入新节点
		}
		//查找 entity 节点
		BiXmlNode* entity_node = entity_list_node->FirstChild("SubEntity");
		while(entity_node)
		{
			const char* entity_node_name = BiXmlHelper::get_attribute(entity_node,"name");//根据entity name 查找节点
			if(modifier_type->child_entity_id == entity_node_name)//已有节点
			{
				break;
			}
			entity_node = entity_node->NextSibling("SubEntity");
		}
		if( entity_node == NULL)//还没有节点，添加1个
		{
			//BiXmlElement* entity_element = new BiXmlElement("SubEntity");
			BiXmlElement* entity_element = entity_list_node->InsertEndChild("SubEntity")->ToElement();
			entity_element->SetAttribute("name", modifier_type->child_entity_id.c_str());

			entity_node = entity_element;
			//entity_list_node->LinkEndChild(entity_node);//如果未找到，插入新节点
		}
		target_node_position = entity_node;//设置处理位置为entity
	}

	IPropertyModifierStreamable* s_midifier = dynamic_cast<IPropertyModifierStreamable*>(property_modifier);
	assert(s_midifier);

	const char* modifier_value = s_midifier->get_value();
	assert(modifier_value);

	bool curve_saved = false;

	//添加属性设置
	if(modifier_type->is_curv)//属性序列设置
	{
		IPropertyCurvModifierStreamable* s_curve_modifier = dynamic_cast<IPropertyCurvModifierStreamable*>(s_midifier);
		assert(s_curve_modifier);

		//取calculator
		ICalculator* calculator = s_curve_modifier->get_calculator();

		int bufsize = 0;
		float f_buf[20] = {0};
		//取帧数
		int key_frames = calculator->get_key_frames(-1 ,NULL , bufsize);

		if(bufsize == 1 && !is_camera_property && isOptimize)//单帧且非相机进行简化，后面写入属性
		{
			modifier_value = calculator->get_interpolating_value(-1,0.0f);//取单帧值
		}
		else//多帧
		{
			BiXmlNode* property_curve_info_node = target_node_position->FirstChild("PropertyCurveInfo");
			if (property_curve_info_node == NULL)
			{
				//property_curve_info_node = new BiXmlElement("PropertyCurveInfo");
				//target_node_position->LinkEndChild(property_curve_info_node);//如果未找到，插入新节点
				property_curve_info_node = target_node_position->InsertEndChild("PropertyCurveInfo");
			}
			//建立属性
			//BiXmlElement item("Property");
			BiXmlElement* item = property_curve_info_node->InsertEndChild("Property")->ToElement();//插入节点
			item->SetAttribute("key", modifier_type->property_name.c_str());

			if(!modifier_type->property_fixed_param.empty())
				item->SetAttribute("fixed_param", modifier_type->property_fixed_param.c_str());

			//填充node
			//BiXmlDocument* doc_read = XmlDocFactory::CreateDocument();
			//doc_read->ParseXmlStr(modifier_value);
			//if(doc_read->Error()) 
			//{
			//	assert(0);
			//}
			////加入节点
			//item->InsertEndChild(*(doc_read->RootElement()));

			////删除文档
			//XmlDocFactory::ReleaseDocument(doc_read);

			curve_saved = true;//多帧曲线存储了

			item->AddText(modifier_value);
			item->FirstChild()->ToText()->SetCDATA(true);

		}
	}

	if(!curve_saved)//属性或单帧曲线
	{
		BiXmlNode* property_info_node = target_node_position->FirstChild("PropertyInfo");
		if (property_info_node == NULL)
		{
			//property_info_node = new BiXmlElement("PropertyInfo");
			//target_node_position->LinkEndChild(property_info_node);//如果未找到，插入新节点
			property_info_node = target_node_position->InsertEndChild("PropertyInfo");
			//如果未找到，插入新节点
		}
		//BiXmlElement item("Property");
		BiXmlElement* item = property_info_node->InsertEndChild("Property")->ToElement();
		item->SetAttribute("key", modifier_type->property_name.c_str());
		item->SetAttribute("fixed_param", modifier_type->property_fixed_param.c_str());
		item->SetAttribute("value", modifier_value);
		//property_info_node->InsertEndChild(item);//插入节点
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
		//点光源
		if (entity->type() == MEDIA_END_ENTITY_TYPE_POINT_PREPASS_LIGHT)
		{
			strcpy(strEntityName, entity->get_name());
			return true;
		}
		//锥光源
		else if (entity->type() == MEDIA_END_ENTITY_TYPE_WIMBLE_PREPASS_LIGHT)
		{
			strcpy(strEntityName, entity->get_name());
			return true;
		}
		//方向光源
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

	//如果同名clip中没有，则先在stageclip后在defaultclip中进行查找
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
//获得某个布尔属性是否存在，和值
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

	//如果为曲线属性
	if (modifier_type->is_curv)
	{
		IPropertyCurvModifierStreamable* curve_midifier = dynamic_cast<IPropertyCurvModifierStreamable*>(property_modifier);

		bool CurvProperty = false;
		if (curve_midifier)
		{
			const char* strValue = curve_midifier->get_value();

			NormalCurvSerializer<bool> curv_serializer;

			CalculatorT<bool>* calculator = curv_serializer.serialize(strValue);

			//进行数据减肥，包括贝塞尔转线性，多帧同值合并
			calculator->simplify();

			int bufsize = 0;
			//取帧数
			int key_frames = calculator->get_key_frames(-1 ,NULL , bufsize);

			if(bufsize == 1)//单帧
			{
				const char* frame_val = calculator->get_interpolating_value(-1,0.0f);

				if (stricmp(frame_val, "true") == 0)
				{
					CurvProperty = true;
				}
			}
			else
			{
				//取关键帧数据
				CurvDataT<bool>* curve_data = dynamic_cast<CurvDataT<bool>*>(calculator->get_curv_data());
				for(CurvNodes<bool>::NodeCollection::iterator it = curve_data->m_nodes->m_nodes.begin(); 
					it != curve_data->m_nodes->m_nodes.end(); ++it)
				{
					//判断属性值
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
	//如果为非曲线属性
	else
	{
		IPropertyModifierStreamable* enable_midifier = dynamic_cast<IPropertyModifierStreamable*>(property_modifier);

		bool NoncurvProperty = false;
		if (enable_midifier)
		{
			//判断属性值
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

//根据激活属性判断是否需要对其动画进行保存
static bool canNotBeOptimized(DynamicEffectObject* dynamic_effect_obj, 
				   ActorAnimationClip* ani_clip, 
				   char* strEntityName, 
				   PropertyModifierType* pModifierType)
{
	if (ani_clip == NULL || dynamic_effect_obj == NULL)
	{
		return false;
	}

	//灯光是否可用属性
	PropertyModifierType enable_modifier_type;
	enable_modifier_type.child_actor_id = "";
	enable_modifier_type.child_entity_id = strEntityName;
	enable_modifier_type.property_fixed_param = "";
	enable_modifier_type.property_name = "Light_Enable";
	//获得曲线属性
	enable_modifier_type.is_curv = true;
	bool b_enable_curve_modifier_existed = false;
	bool b_enable_curve_modifier_value = false;
	b_enable_curve_modifier_value = getPropertyIsEnable(dynamic_effect_obj, 
		ani_clip, &enable_modifier_type, b_enable_curve_modifier_existed);
	//获得非曲线属性
	enable_modifier_type.is_curv = false;
	bool b_enable_noncurve_modifier_existed = false;
	bool b_enable_noncurve_modifier_value = false;
	b_enable_noncurve_modifier_value = getPropertyIsEnable(dynamic_effect_obj, 
		ani_clip, &enable_modifier_type, b_enable_noncurve_modifier_existed);

	//灯光是否可见属性
	PropertyModifierType visible_modifier_type;
	visible_modifier_type.child_actor_id = "";
	visible_modifier_type.child_entity_id = strEntityName;
	visible_modifier_type.property_fixed_param = "";
	visible_modifier_type.property_name = "Visible";
	//获得曲线属性
	visible_modifier_type.is_curv = true;
	bool b_visible_curve_modifier_existed = false;
	bool b_visible_curve_modifier_value = false;
	b_visible_curve_modifier_value = getPropertyIsEnable(dynamic_effect_obj, 
		ani_clip, &visible_modifier_type, b_visible_curve_modifier_existed);
	//获得非曲线属性
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

	//如果存在曲线属性，以曲线属性为准进行判断。且enable属性和visible属性均为true时才可保持该物体
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
	//判断是否相机物体
	bool is_camera_object = false;
	CameraEffectObject* camera_object = dynamic_cast<CameraEffectObject*>(dynamic_effect_object);
	if(camera_object != NULL)//相机不进行减肥操作
		is_camera_object = true;
	if (false_modifier_type->is_curv)
	{
		IPropertyCurvModifierStreamable* curve_midifier = dynamic_cast<IPropertyCurvModifierStreamable*>(false_property_modifier);

		ICalculator* calculator = curve_midifier->get_calculator();
		calculator->simplify();		
	}

	if(!generateObjectProperty(object_element, false_modifier_type, false_property_modifier,is_camera_object, isOptimize))
	{
		//错误直接返回，不保存
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
	//迭代判断某个属性关键帧是否有效
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
	//取节点
	DynamicEffectObject* dynamic_effect_object = obj; 
	//取控制器信息
	ActorAnimationControl* 	ani_control = dynamic_effect_object->getAnimationControl();
	assert(ani_control);
	//取stage clip ID 对应的 ani_clip
	ActorAnimationClip* ani_clip = ani_control->find_animation_clip(clip_id);
	if(ani_clip == NULL)//没有控制器
		return true;

	ActorAnimationClip* stage_ani_clip = ani_control->find_animation_clip(DynamicEffectObject::getStagePropertyName());
	ActorAnimationClip* default_ani_clip = ani_control->find_animation_clip(DynamicEffectObject::getDefaultPropertyName());

	//建立节点文件
	//BiXmlElement* object_element = new BiXmlElement("DynamicEffectObject");//建立节点
	//parentNode->LinkEndChild(object_element);//插入节点
	BiXmlElement* object_element = parentNode->InsertEndChild("DynamicEffectObject")->ToElement();//建立节点

	if(consistentId)
		object_element->SetAttribute("id", file_main_name);//设置ID
	else
		object_element->SetAttribute("id", dynamic_effect_object->get_name());//设置ID

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

	//取预定义信息
	const CDefinedItemCollection* defined_item_collection = ani_player->getDefinedItemCollection();
	assert(defined_item_collection);
	// 重置属性修改器迭代器
	ani_clip->reset_iterator();
	PropertyModifierType modifier_type;
	// 获得属性修改器，迭代器指向下一个属性修改器，如迭代器无效返回0

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
		//如果属性序列为空，则不进行保存，直接跳过。因为属性必然不为空，所以不用判断。
		if (modifier_type.is_curv)
		{
			IPropertyCurvModifierStreamable* s_curve_modifier = dynamic_cast<IPropertyCurvModifierStreamable*>(s_midifier);
			assert(s_curve_modifier);
			if (!curvPropertyIsValid(s_curve_modifier))
				continue;
		}

		//跳过缺省曲线标志
		bool remove_default_curve = false;

		//判断是否相机物体
		bool is_camera_object = false;
		CameraEffectObject* camera_object = dynamic_cast<CameraEffectObject*>(dynamic_effect_object);
		if(camera_object != NULL)//相机不进行减肥操作
			is_camera_object = true;

		if(! is_camera_object
			&& isOptimize
			)//进行减肥操作
		{
			if(!modifier_type.is_curv)//非曲线
			{
				IPropertyModifier* stage_modifier = stage_ani_clip->find_property_modifier(modifier_type);
				IPropertyModifierStreamable* s_stage_modifier = dynamic_cast<IPropertyModifierStreamable*>(stage_modifier);
				IPropertyModifier* default_modifier = default_ani_clip->find_property_modifier(modifier_type);
				IPropertyModifierStreamable* s_default_midifier = dynamic_cast<IPropertyModifierStreamable*>(default_modifier);
				//先和舞台属性比较
				if (stage_modifier)
				{
					if(stricmp(s_midifier->get_value(),s_stage_modifier->get_value() ) == 0)
						remove_default_curve = true;//缺省舞台属性值不记录
					else//有曲线的不记录
					{
						PropertyModifierType modifier_type_hascurve = modifier_type;
						modifier_type_hascurve.is_curv = true;
						if(IPropertyCurvModifierStreamable* curv_modifier = dynamic_cast<IPropertyCurvModifierStreamable*>(ani_clip->find_property_modifier(modifier_type_hascurve)))//同属性有曲线控制
						{
							//取calculator
							ICalculator* calculator = curv_modifier->get_calculator();
							//进行数据减肥，包括贝塞尔转线性，多帧同值合并
							calculator->simplify();

							//取帧数
							int bufsize = 0;
							int key_frames = calculator->get_key_frames(-1 ,NULL , bufsize);
							if(bufsize > 0)//没有值
							{		
								remove_default_curve = true;//不记录
							}
						}
					}
				}
				//再和缺省属性比较
				else if(default_modifier)
				{
					if(stricmp(s_midifier->get_value(),s_default_midifier->get_value() ) == 0)
						remove_default_curve = true;//缺省值不记录
					else//有曲线的不记录
					{
						PropertyModifierType modifier_type_hascurve = modifier_type;
						modifier_type_hascurve.is_curv = true;
						if(IPropertyCurvModifierStreamable* curv_modifier = dynamic_cast<IPropertyCurvModifierStreamable*>(ani_clip->find_property_modifier(modifier_type_hascurve)))//同属性有曲线控制
						{
							//取calculator
							ICalculator* calculator = curv_modifier->get_calculator();
							//进行数据减肥，包括贝塞尔转线性，多帧同值合并
							calculator->simplify();
							
							//取帧数
							int bufsize = 0;
							int key_frames = calculator->get_key_frames(-1 ,NULL , bufsize);
							if(bufsize > 0)//没有值
							{		
								remove_default_curve = true;//不记录
							}
						}
					}
				}
			}
			else
			{
				IPropertyCurvModifierStreamable* s_curve_modifier = dynamic_cast<IPropertyCurvModifierStreamable*>(s_midifier);
				assert(s_curve_modifier);
				//取calculator
				ICalculator* calculator = s_curve_modifier->get_calculator();

				//进行数据减肥，包括贝塞尔转线性，多帧同值合并
				calculator->simplify();

				//取缺省modifier
				PropertyModifierType modifier_type_notcurve = modifier_type;
				if(modifier_type_notcurve.is_curv)
					modifier_type_notcurve.is_curv = false;//根据非曲线类型查缺省值

				IPropertyModifier* stage_modifier = stage_ani_clip->find_property_modifier(modifier_type_notcurve);
				IPropertyModifierStreamable* s_stage_modifier = dynamic_cast<IPropertyModifierStreamable*>(stage_modifier);
				IPropertyModifier* default_modifier = default_ani_clip->find_property_modifier(modifier_type_notcurve);
				IPropertyModifierStreamable* s_default_midifier = dynamic_cast<IPropertyModifierStreamable*>(default_modifier);
				//判断是否是缺省值建立的曲线

				int bufsize = 0;
				float f_buf[20] = {0};
				//取帧数
				int key_frames = calculator->get_key_frames(-1 ,NULL , bufsize);

				if(bufsize == 1)//单帧
				{
					const char* frame_val = calculator->get_interpolating_value(-1,0.0f);
				
					//先于舞台缺省属性比较
					if(s_stage_modifier)
					{
						if(stricmp(s_stage_modifier->get_value(), frame_val) == 0)//值相同
							remove_default_curve = true;//跳过缺省曲线
						//在存储时会将曲线设置到属性上 
					}
					//如果舞台缺省属性不存在，则再与缺省属性比较
					else if(s_default_midifier)
					{
						if(stricmp(s_default_midifier->get_value(), frame_val) == 0)//值相同
							remove_default_curve = true;//跳过缺省曲线
						//在存储时会将曲线设置到属性上 
					}			
				}
			}
		}
		if(!remove_default_curve)//非去除曲线
		{
			//根据类型建立XML节点
			if(!generateObjectProperty(object_element, &modifier_type, property_modifier,is_camera_object, isOptimize))
			{
				//错误直接返回，不保存
				return false;
			}
		}
	}

	//添加 级联实体属性
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
	
	//加入节点
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
//存储文件
bool StageAnimationClip::save(const char* filename0, bool isBinary, bool consistentId, bool isOptimize)
{
	assert(!isBinary);//尚不支持2进制

	//转换分隔符
	std::string fullFileName ;
	MiddleLayerFilePathHelper::convertSeperator(filename0,fullFileName);
	//取工作路径
	std::string working_dir ;
	MiddleLayerFilePathHelper::getWorkingPath(filename0,working_dir);

	//取文件名称
	std::string fileMainName;

	std::string::size_type sperator_pos = fullFileName.rfind("/");
	if(sperator_pos == std::string::npos )//无分隔符号
	{
		std::string::size_type dot_pos = fullFileName.find(".");
		if(dot_pos == std::string::npos)//无.
			fileMainName = fullFileName;
		else
			fileMainName = fullFileName.substr( 0, dot_pos );
	}
	else//有分隔符号
	{
		std::string::size_type dot_pos = fullFileName.find(".", sperator_pos + 1);
		if(dot_pos ==  std::string::npos)//无.
			fileMainName = fullFileName.substr(sperator_pos + 1 );
		else
			fileMainName = fullFileName.substr(sperator_pos + 1 , dot_pos - (sperator_pos + 1));
	}


	//BiXmlDocument* doc = new BiXmlDocument(fullFileName.c_str());
	BiXmlDocument* doc = XmlDocFactory::CreateDocument();
	if (doc == NULL)
		return false;
	
	// 找到Stage节点
	BiXmlNode* stageEffect = NULL;

	//BiXmlElement* stageElement = new BiXmlElement("StageEffect");
	BiXmlElement* stageElement = doc->InsertEndChild("StageEffect")->ToElement();

	assert(!fileMainName.empty());//确认名称非空
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
		assert(!"StageAnimationClip save, clip类型不支持!");

	stageEffect = stageElement;
	//doc->LinkEndChild(stageEffect);

	StageAnimationPlayer* ani_player = dynamic_cast<StageAnimationPlayer*>(m_ani_clip_mng->getStage()->getPlayer());
	
	//处理所有的系统物体
	if(!saveClipStageSystemObjects(stageEffect,consistentId, fileMainName.c_str(), isOptimize))
	{
		XmlDocFactory::ReleaseDocument(doc);
		return false;
	}

	//处理所有物体
	std::list<DynamicEffectObject*>::const_iterator iter = m_object_list.begin();
	for(; iter != m_object_list.end(); ++ iter)
	{
		//取节点
		DynamicEffectObject* dynamic_effect_object = *iter; 
		if(!saveClipDynamicEffectObjectRecursive(stageEffect,dynamic_effect_object,this->getID(),consistentId, fileMainName.c_str(), ani_player, isOptimize))
		{
			//错误直接返回，不保存
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
	//查找clip
	ActorAnimationClip* actor_ani_clip = actor_control->find_animation_clip(this->getID());
	//播放clip
	if(actor_ani_clip)
	{
		actor_control->play(isPlay,t,actor_ani_clip);
		if(!isPlay && (m_ani_clip_type != StageAnimationClip_Camera))//停止时进行恢复
		{
			DynEffObjControl* dyn_effobj_control = dynamic_cast<DynEffObjControl*>(actor_control);
			if(m_ani_clip_type !=StageAnimationClip_Camera )//当前对应的cutting data
			{
				//取出modifier 集合
				if (m_current_cutting_data)
				{
					std::string nextClipId = "";
					if(m_next_cutting_data)
						nextClipId = m_next_cutting_data->m_animation_clip_id;
					std::map< std::pair<std::string,std::string>,std::set<IPropertyModifier*>>::const_iterator map_iter = dyn_effobj_control->m_reset_modifier_map.find(std::make_pair<std::string,std::string>(m_current_cutting_data->m_animation_clip_id,nextClipId));

					if(map_iter == dyn_effobj_control->m_reset_modifier_map.end())
						return;

					const std::set<IPropertyModifier*>& modifier_set = map_iter->second;

					//恢复缺省属性
					std::set<IPropertyModifier*>::const_iterator modifier_iter = modifier_set.begin();
					for(; modifier_iter!= modifier_set.end(); ++ modifier_iter)
					{
						(*modifier_iter)->execute(true,0.0f);//进行属性设置，恢复缺省属性
					}

					return;
				}

				//查找clip
				ActorAnimationClip* default_property_ani_clip = actor_control->find_animation_clip(obj->getDefaultPropertyName());
				//播放clip
				actor_control->play(true,0.0f,default_property_ani_clip);
				actor_control->play(false,0.0f,default_property_ani_clip);//停止

				ActorAnimationClip* stage_property_ani_clip = actor_control->find_animation_clip(obj->getStagePropertyName());
				//播放clip
				if(stage_property_ani_clip)
				{
					actor_control->play(true,0.0f,stage_property_ani_clip);
					actor_control->play(false,0.0f,stage_property_ani_clip);//停止	
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

//播放
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

//渲染
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

//进行硬件选择渲染
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

//更新, 单位毫秒
void StageAnimationClip::update(float t)
{
	std::list<DynamicEffectObject*>::const_iterator iter = m_object_list.begin();
	for(; iter != m_object_list.end(); ++ iter)
	{
		(*iter)->update(t);
		(*iter)->m_object_updated = true;//设置更新标志变量
	};

	m_total_update_time += t;
};

float StageAnimationClip::getTotalUpdateTime()
{
	return m_total_update_time;
}

void StageAnimationClip::setID(const char* id)
{
	//修改
	std::list<DynamicEffectObject*>::const_iterator iter = m_object_list.begin();
	for(; iter != m_object_list.end(); ++ iter)
	{
		ActorAnimationControl* actor_control = (*iter)->getAnimationControl();
		//查找clip
		actor_control->rename_animation_clip(getID(), id);
	}
	m_id = id;

}

//添加效果物体
void StageAnimationClip::insert(DynamicEffectObject* eff_obj)
{
	ActorAnimationControl* aac = eff_obj->getAnimationControl();
	if(!aac->find_animation_clip(this->getID())) // 添加同名的clip
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
			return;//重复添加，返回
	}
	m_object_list.push_back(eff_obj);
	resetObjectIterator() ;
};
//移除效果物体
void StageAnimationClip::remove(DynamicEffectObject* eff_obj, bool remove_clip)
{
	ActorAnimationControl* aac = eff_obj->getAnimationControl();
	aac->play(false, 0, this->getID());
	
	if(remove_clip)//进行级联时移动时不删除clip
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
//查找
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
			m_cameraPathMap[id] = strTmp;//记录镜头路径
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
	while(Actor* actor = actor_manager->next())//取所有物体
	{
		//顶层的做些处理
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
	//建立缺省属性的clip 
	StageAnimationPlayer* player = dynamic_cast<StageAnimationPlayer*>(m_stage->getPlayer());

	assert(!isBinary);
	assert(ani_clip_map.size()==0);
	assert(m_clips_path_list.size() == 0);

	//转换分隔符
	std::string strFileName ;
	MiddleLayerFilePathHelper::convertSeperator(filename,strFileName);
	
	BiXmlHelper::LoadXMLDocument(strFileName.c_str());
	BiXmlDocument* doc = BiXmlHelper::PeekXMLDocument(strFileName.c_str(), true);
	if(doc == NULL)
	{
		assert(0);
		return false;
	}

	// 读取序列
	BiXmlNode* stageRootNode = doc->FirstChild("Stage");
	if (stageRootNode == NULL)
	{
		XmlDocFactory::ReleaseDocument(doc);
		return false;
	}

	// 建立每个物体的缺省舞台效果节点
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
			//建立obj的 ani_control
			ActorAnimationControl* ani_control = main_obj->getAnimationControl();
			assert(ani_control);
			//建立与StageAniClip同名的 ActorAniClip
			ActorAnimationClip* ani_clip = 	ani_control->find_animation_clip(DynamicEffectObject::getStagePropertyName());
			assert(ani_clip);
			PropertyModifierFactory* modifier_factory = getStage()->getActorMng()->getPropertyModifierFactory();//
			//建立物体的舞台缺省属性
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
	//建立缺省属性的clip 
	StageAnimationPlayer* player = dynamic_cast<StageAnimationPlayer*>(m_stage->getPlayer());

	assert(!isBinary);
	assert(ani_clip_map.size()==0);
	assert(m_clips_path_list.size() == 0);

	//转换分隔符
	std::string strFileName ;
	MiddleLayerFilePathHelper::convertSeperator(filename,strFileName);
	//取工作路径
	std::string working_path ;
	MiddleLayerFilePathHelper::getWorkingPath(filename,working_path);

	BiXmlHelper::LoadXMLDocument(strFileName.c_str());
	BiXmlDocument* doc = BiXmlHelper::PeekXMLDocument(strFileName.c_str(), true);
	if(doc == NULL)
	{
		assert(0);
		return false;
	}

	// 读取序列
	BiXmlNode* stageRootNode = doc->FirstChild("Stage");
	if (stageRootNode == NULL)
	{
		XmlDocFactory::ReleaseDocument(doc);
		return false;
	}

	// 保存物体的舞台效果节点列表
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

//载入单独的舞台效果
bool StageAnimationClipMng::loadStageEffect(const char* stageEffect, bool isBinary)
{
	assert(stageEffect && !isBinary);

	//是否已经载入
	std::map<std::string, StageAnimationClip*>::iterator itr = ani_clip_map.find(stageEffect);
	if (itr != ani_clip_map.end())
		return false;

	//该舞台效果是否存在
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
		m_cameraPathMap[iter->first] = iter->second;//记录镜头路径
	}

	return true;
}

bool StageAnimationClipMng::load(const char* filename0, bool isBinary, I_Keep_Client_Alive* pLp)
{
	//建立缺省属性的clip 
	StageAnimationPlayer* player = dynamic_cast<StageAnimationPlayer*>(m_stage->getPlayer());

	assert(!isBinary);
	assert(ani_clip_map.size()==0);

	//转换分隔符
	std::string strFileName ;
	MiddleLayerFilePathHelper::convertSeperator(filename0,strFileName);
	//取工作路径
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

	// 读取序列
	BiXmlNode* stageRootNode = doc->FirstChild("Stage");
	if (stageRootNode == NULL)
	{
		//delete doc;
		XmlDocFactory::ReleaseDocument(doc);
		return false;
	}
	// 读取舞台动态效果物体的节点
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
			//建立obj的 ani_control
			ActorAnimationControl* ani_control = main_obj->getAnimationControl();
			assert(ani_control);
			//建立与StageAniClip同名的 ActorAniClip
			ActorAnimationClip* ani_clip = 	ani_control->find_animation_clip(DynamicEffectObject::getStagePropertyName());
			assert(ani_clip);
			
			PropertyModifierFactory* modifier_factory = getStage()->getActorMng()->getPropertyModifierFactory();//
			//建立物体的舞台缺省属性
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
	// 读取舞台效果的节点
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
	assert(!isBinary);//尚不支持2进制
	//BiXmlDocument* doc = new BiXmlDocument();
	BiXmlDocument* doc = XmlDocFactory::CreateDocument();
	if (doc == NULL)
		return false;

	//转换分隔符
	std::string strFileName ;
	MiddleLayerFilePathHelper::convertSeperator(filename0,strFileName);
	//取工作路径
	std::string working_path ;
	MiddleLayerFilePathHelper::getWorkingPath(filename0,working_path);

	doc->LoadFile(strFileName.c_str());

	// 找到Stage节点
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
	// 添加舞台效果

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
		//存储舞台效果文件
		i->second->save(fullPath.c_str(),isBinary, false, isOptimize);//传递加路径的文件名
	}

	bool ret = doc->SaveFile(strFileName.c_str());
	//delete doc;
	XmlDocFactory::ReleaseDocument(doc);
	return ret;
};

// 创建给定类型的舞台效果
StageAnimationClip* StageAnimationClipMng::createStageAnimationClip(StageAnimationClipType t, const char* clipid)
{
	StageAnimationClip* ani_clip = new StageAnimationClip(this);
	ani_clip->setID(clipid);
	ani_clip->setClipType(t);
	return ani_clip;
};
// 销毁给定的舞台效果
void StageAnimationClipMng::releaseStageAnimationClip(StageAnimationClip* stageaniclip)
{
	delete stageaniclip;
	return;
};

// 添加一个舞台效果
void StageAnimationClipMng::insert(StageAnimationClip* stageaniclip)
{
	std::map<std::string,StageAnimationClip*>::const_iterator iter = ani_clip_map.find(stageaniclip->getID());
	assert(iter == ani_clip_map.end());
	
	ani_clip_map.insert(std::make_pair(stageaniclip->getID(),stageaniclip));
	resetIterator();
};
// 删除一个舞台效果
void StageAnimationClipMng::remove(StageAnimationClip* stageaniclip)
{
	std::map<std::string,StageAnimationClip*>::iterator iter = ani_clip_map.find(stageaniclip->getID());
	
	if(iter != ani_clip_map.end())
	{
		//移除ani_clip
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
		//删除记录
		ani_clip_map.erase(iter);
		resetIterator();

		//删除lod物体上的舞台效果
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

//查找指定的舞台效果
StageAnimationClip* StageAnimationClipMng::find(const char* clipid)
{
	std::map<std::string,StageAnimationClip*>::const_iterator iter = ani_clip_map.find(clipid);
#ifdef _EDITOR
	//现在舞台效果中找，如果没有，则再在通用文件中找
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

// 重置舞台效果迭代器
void StageAnimationClipMng::resetIterator()
{
	m_clip_pos = ani_clip_map.begin();
} ;
// 获得舞台效果，迭代器指向下一个舞台效果，如迭代器无效返回0
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
	if(camera_object != NULL)//进行减肥操作
		is_camera_object = true;
	//取控制器信息
	ActorAnimationControl* 	ani_control = dynamic_effect_object->getAnimationControl();
	assert(ani_control);
	//取stage属性设置的ani_clip
	ActorAnimationClip* ani_clip = ani_control->find_animation_clip(DynamicEffectObject::getStagePropertyName());//
	if(ani_clip)
	{
		// 重置属性修改器迭代器
		ani_clip->reset_iterator();
		PropertyModifierType modifier_type;
		// 获得属性修改器，迭代器指向下一个属性修改器，如迭代器无效返回0
		while(IPropertyModifier* property_modifier = ani_clip->next(modifier_type))
		{
			IPropertyModifierStreamable* s_midifier = dynamic_cast<IPropertyModifierStreamable*>(property_modifier);
			assert(s_midifier);

			const char* value = s_midifier->get_value();
			assert(value);
			//根据类型建立XML节点
			if(!generateObjectProperty(effect_object_node, &modifier_type, property_modifier, is_camera_object))//
				return false;
		}	
	}
	return true;
};
//从关卡文件外读取舞台效果
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
		m_cameraPathMap[ani_clip->getID()] = strTmp;//记录镜头路径
	}
	return true;
};
//根据id取相机文件
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

		//加载并执行缺省效果，待完成
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
@brief 解析通用镜头定义
*/
bool StageAnimationClipMng::parseCameraFile(const char* cameraFile)
{
	// 打开关卡文件
	BiXmlDocument* doc = XmlDocFactory::CreateDocument();

	if(!doc->LoadFile(cameraFile))
	{
		XmlDocFactory::ReleaseDocument(doc);
		return false;
	}
	BiXmlNode* rootNode = doc->FirstChild("common");
	if(!rootNode)
		return false;

	// 解析相机信息
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
