/**
* @file editor_info_table.h
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
extern void divide_actor_params(const std::string& sub_actor, std::list<std::string>& actor_list);
CDefinedItemCollection::CDefinedItemCollection()
{
	
};
CDefinedItemCollection::~CDefinedItemCollection()
{
	for(std::map<std::string,DefinedItem*>::iterator pos = m_defined_item_map.begin();
		pos != m_defined_item_map.end(); ++pos)
	{
		delete pos->second;
	}
	m_defined_item_map.clear();
};
//添加属性修改器
bool CDefinedItemCollection::addPropertyOnItem(BiXmlNode* propertyNode
								,const char* actor_level
								,const char* child_entity_id
								,bool is_curv
								,DefinedItem* defined_item)
{
	//读属性
	const char* property_name0 = BiXmlHelper::get_attribute(propertyNode,"key");

	const char* fixed_parm = BiXmlHelper::get_attribute(propertyNode,"fixed_param");
	const char* value = BiXmlHelper::get_attribute(propertyNode,"value");

	const char* data_type = BiXmlHelper::get_attribute(propertyNode,"datatype");
	const char* usage = BiXmlHelper::get_attribute(propertyNode,"usage");
	//const char* default_value = BiXmlHelper::get_attribute(propertyNode,"default");
	const char* max = BiXmlHelper::get_attribute(propertyNode,"max");
	const char* min = BiXmlHelper::get_attribute(propertyNode,"min");
	const char* caption = BiXmlHelper::get_attribute(propertyNode,"caption");
	const char* property_support = BiXmlHelper::get_attribute(propertyNode,"property_support");

	std::string property_name_ext = property_name0;
	if(fixed_parm)
		property_name_ext+=fixed_parm;

	defined_item->m_property_order_recoder.push_back(property_name_ext.c_str());//添加属性名列表
	//建立新的item
	DefinedItem::ValueItem* new_item = new DefinedItem::ValueItem();
	defined_item->m_property_list.push_back(new_item);


	if(value == NULL)
		value = BiXmlHelper::get_attribute(propertyNode,"default");//取缺省值

	//设置modifier type
	//PropertyModifierType modifier_type;
	new_item->m_property_modifier_type.child_actor_id = actor_level;//actor层次，只包括子actor的
	new_item->m_property_modifier_type.child_entity_id = child_entity_id;//实体名称

	new_item->m_property_modifier_type.property_name = property_name0;//属性名称;
	if(fixed_parm)
		new_item->m_property_modifier_type.property_fixed_param = fixed_parm;//固定参数 /parm1/parm2/..;
	new_item->m_property_modifier_type.is_curv = is_curv;//是否曲线


	if(data_type)
		new_item->m_v_type = data_type;
	//if(usage)
	//	new_item->m_usage=usage;
	if(caption)
		new_item->m_caption= caption;
	if(value)
		new_item->m_value = value;
	if(max)
		new_item->m_max_value = max;
	if(min)
		new_item->m_min_value = min;

	if(property_support)
		new_item->m_property_support = property_support;

	return true;

}
//递归建立动态效果物体和子实体
DefinedItem*  CDefinedItemCollection::createPropertyRecursive(BiXmlNode* xmlNode,
											   std::string& parent_actor_level 
											   )
{
	//递归建立modifier

	//加上自己的层次ID
	std::string self_actor_level; 

	DefinedItem* actor_item = new DefinedItem();
	const char* defined_object_type = BiXmlHelper::get_attribute(xmlNode,"defined_type");
	const char* object_type = BiXmlHelper::get_attribute(xmlNode,"type");
	const char* object_id = BiXmlHelper::get_attribute(xmlNode,"id");


	//建立节点类型
	if(defined_object_type)
	{
		actor_item->m_node_type = DefinedItem::DEFINED_EFFECT_OBJECT;
		actor_item->m_item_type = defined_object_type;
	}
	else 
	{
		actor_item->m_node_type = DefinedItem::NORMAL_EFFECT_OBJECT;
		if(object_type)
			actor_item->m_item_type = object_type;
		assert(object_id);
		self_actor_level = parent_actor_level + "/" + object_id;//建立actor层次
	}

	if(object_id)
		actor_item->m_id = object_id;//

	//建立属性
	BiXmlNode* propertyInfoNode = xmlNode->FirstChild("PropertyInfo");
	if( propertyInfoNode != NULL)
	{
		BiXmlNode* propertyNode = propertyInfoNode->FirstChild("Property");
		while(propertyNode != NULL )
		{
			//属性解析
			if(!addPropertyOnItem(propertyNode, self_actor_level.c_str(), "",false , actor_item))
			{
				delete actor_item;
				return NULL;
			}

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
			const char* sub_entity_type = BiXmlHelper::get_attribute(entityNode,"type");

			//建立 属性 item
			DefinedItem* entity_item = new DefinedItem();
			entity_item->m_node_type = DefinedItem::NORMAL_ENTITY;//实体类型节点
			if(sub_entity_type)
				entity_item->m_item_type = sub_entity_type;
			entity_item->m_id = entity_name;

			actor_item->m_sub_item.push_back(entity_item);//加入队列

			//建立属性
			BiXmlNode* sub_entity_propertyInfoNode = entityNode->FirstChild("PropertyInfo");
			if( sub_entity_propertyInfoNode != NULL)
			{
				BiXmlNode* propertyNode = sub_entity_propertyInfoNode->FirstChild("Property");
				while(propertyNode != NULL )
				{
					//属性解析
					if(!addPropertyOnItem(propertyNode, self_actor_level.c_str(), entity_name,false ,entity_item))
					{
						delete actor_item;
						return NULL;
					}
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
			
			DefinedItem* sub_actor_item = createPropertyRecursive(dynamicEffectObjectNode, self_actor_level);
			assert(sub_actor_item);
			actor_item->m_sub_item.push_back(sub_actor_item);//添加到列表

			dynamicEffectObjectNode = dynamicEffectObjectNode->NextSibling("DynamicEffectObject");
		}
	}
	return actor_item;
}
bool CDefinedItemCollection::createObjectProperty(BiXmlNode* effectNode)
{	//根据effect文件信息建立DynamicEffect
	//BiXmlNode* item = 0;
	//const char* object_type = BiXmlHelper::get_attribute(effectNode,"defined_type");
	//做所有类型记录
	//m_supported_objects.push_back(object_type);
	//顶层的做些处理,添加顶层的对象到 map
	//m_object_property_map.insert(OBJECT_PROPERTY_MAP::value_type(object_type,std::map<struct PropertyModifierType , struct PropertyModifierEditInfo>()));

	//OBJECT_PROPERTY_MAP::iterator obj_iter = m_object_property_map.find(object_type);
	//建立与StageAniClip同名的 ActorAniClip
	//ActorAnimationClip* ani_clip = new ActorAnimationClip();
	//ani_control->add_animation_clip(this->getID(), ani_clip);//ActorAniClip与 舞台效果同名
	//DefinedItem* defined_item = new DefinedItem();
	//defined_item->m_type = DefinedItem::DEFINED_EFFECT_OBJECT;
	std::string actor_level;
	DefinedItem* defined_item = createPropertyRecursive(effectNode,actor_level);
	if(defined_item == NULL)
		return false;
	
	m_defined_item_map.insert(std::map<std::string,DefinedItem*>::value_type(defined_item->m_item_type,defined_item));//以TYPE为键值插入map
	return true;
};
//读取预定义物体信息
bool CDefinedItemCollection::load(const char* filename, bool isBinary) 
{
	//create dynamic effect object
	assert(!isBinary);

	//BiXmlHelper::LoadXMLDocument(filename);
	BiXmlDocument* doc = BiXmlHelper::PeekXMLDocument(filename, false);
	if(doc == NULL)
	{
		assert(0);
		return false;
	}
	// 读取序列
	BiXmlNode* stageEffectNode = doc->FirstChild("DefinedDynamicEffectObjectList");
	if (stageEffectNode == NULL)
	{
		//delete doc;
		return false;
	}

	// 读取动态效果物体的节点
	BiXmlNode* dynamicEffectObjectNode = stageEffectNode->FirstChild("DynamicEffectObject");
	while(dynamicEffectObjectNode != NULL)
	{//解析舞台效果
		if (!createObjectProperty(dynamicEffectObjectNode))
		{
			//delete doc;
			return false;
		}

		dynamicEffectObjectNode = dynamicEffectObjectNode->NextSibling("DynamicEffectObject");
	}
	//delete doc;
	return true;
};
const DefinedItem* CDefinedItemCollection::getDefinedItem(const char* type) const
{
	std::map<std::string,DefinedItem*>::const_iterator iter = m_defined_item_map.find(type);
	if(iter == m_defined_item_map.end())
		return NULL;
	return iter->second;
};

//取一个具体的子节点的信息
const DefinedItem::ValueItem* CDefinedItemCollection::getSubItemValue(const char* type, PropertyModifierType& modifier_type) const
{
	//取缺省属性
	const DefinedItem* defined_item = getDefinedItem(type);//& 取类型
	assert(defined_item);
	//先找到插入节点
	std::list<std::string> sub_actor_list;
	divide_actor_params(modifier_type.child_actor_id,sub_actor_list);
	//临时ITEM,查找actor
	const DefinedItem* defined_item_temp = defined_item;
	//取actor名称，查找actor

	for(std::list<std::string>::const_iterator sub_actor_iter = sub_actor_list.begin(); 
		sub_actor_iter != sub_actor_list.end() ; ++sub_actor_iter)
	{
		for(std::list<DefinedItem*>::const_iterator actor_iter = defined_item_temp->m_sub_item.begin(); 
			actor_iter!= defined_item_temp->m_sub_item.end(); ++ actor_iter)
		{
			if((*actor_iter)->m_node_type != DefinedItem::NORMAL_EFFECT_OBJECT)
				continue;
			if((*actor_iter)->m_id == *sub_actor_iter)//根据ID进行匹配
			{
				defined_item_temp = *actor_iter;//查找下一级actor
				break;
			}
		}
	}

	//查找entity
	for(std::list<DefinedItem*>::const_iterator entity_iter = defined_item_temp->m_sub_item.begin(); 
		entity_iter!= defined_item_temp->m_sub_item.end(); ++ entity_iter)
	{
		if((*entity_iter)->m_node_type != DefinedItem::NORMAL_ENTITY)
			continue;
		if((*entity_iter)->m_id == modifier_type.child_entity_id)//根据ID进行匹配
		{
			defined_item_temp = *entity_iter;//这个地方改变了循环的主变量，需要立即退出
			break;
		}
	}
	//return defined_item_temp;
	//匹配属性
	for(std::list<DefinedItem::ValueItem*>::const_iterator property_iter = defined_item_temp->m_property_list.begin(); 
		property_iter!= defined_item_temp->m_property_list.end(); ++ property_iter)
	{
		if((*property_iter)->m_property_modifier_type.property_name == modifier_type.property_name
			&& (*property_iter)->m_property_modifier_type.property_fixed_param == modifier_type.property_fixed_param)//比较类型相同
		{
			//if(stricmp((*property_iter)->m_value.c_str(), frame_val) == 0)//值相同
			//	remove_default_curve = true;//跳过缺省曲线
			return (*property_iter);
		}
	}
	if(true)
	{
		int i;i=0;
	}
	return NULL;
};

CStageDefinedItemCollection::CStageDefinedItemCollection()
{

}
CStageDefinedItemCollection::~CStageDefinedItemCollection()
{

}
//以ID名添加属性记录
bool CStageDefinedItemCollection::createObjectPropertyById(BiXmlNode* effectNode)
{	//根据effect文件信息建立DynamicEffect
	////////////////////////
	std::string actor_level;
	DefinedItem* defined_item = createPropertyRecursive(effectNode,actor_level);
	if(defined_item == NULL)
		return false;
	
	m_defined_item_map.insert(std::map<std::string,DefinedItem*>::value_type(defined_item->m_id,defined_item));//以ID为键值插入map
	return true;
};
//从XML节点读取节点信息
bool CStageDefinedItemCollection::loadFromXmlNode(BiXmlNode* stageEntitys)
{
	// 读取动态效果物体的节点
	BiXmlNode* dynamicEffectObjectNode = stageEntitys->FirstChild("DynamicEffectObject");
	while(dynamicEffectObjectNode != NULL)
	{//解析舞台效果
		if (!createObjectPropertyById(dynamicEffectObjectNode))
		{
			return false;
		}

		dynamicEffectObjectNode = dynamicEffectObjectNode->NextSibling("DynamicEffectObject");
	}
	return true;	
};
const DefinedItem* CStageDefinedItemCollection::getStageDefinedItem(const char* id) const
{
	return 	getDefinedItem(id);
};



