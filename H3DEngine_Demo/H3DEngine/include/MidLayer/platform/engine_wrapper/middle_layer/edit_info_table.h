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
#ifndef _EDITOR_INFO_TABLE_H_
#define _EDITOR_INFO_TABLE_H_

#include "middle_layer_header.h"

struct DefinedItem 
{
	enum NODE_TYPE{INVALID_TYPE, DEFINED_EFFECT_OBJECT , NORMAL_EFFECT_OBJECT , NORMAL_ENTITY  };//节点类型定义： 预定义物体，普通动态效果物体，实体
	~DefinedItem()
	{
		for(std::list<DefinedItem::ValueItem*> ::iterator property_iter = m_property_list.begin();
			property_iter != m_property_list.end(); ++ property_iter)
		{
			delete *property_iter;
		}
		for(std::list<DefinedItem*>::iterator pos = m_sub_item.begin();
			pos != m_sub_item.end(); ++pos)
		{
			delete *pos;
		}
	}
	DefinedItem()
	{
		m_node_type = INVALID_TYPE;
	}
	// 值项
	struct ValueItem
	{
		ValueItem(){}

		ValueItem(const char* vtype, const char* value, const char* min_value, const char* max_value, const char* caption)
			: m_v_type(vtype), m_value(value), m_caption(caption), m_min_value(min_value), m_max_value(max_value)
		{
		}

		// 值的类型
		std::string m_v_type;
		
		// 具体的值
		std::string m_value;
		// 最小值
		std::string m_min_value;
		// 最大值
		std::string m_max_value;
		// 显示的名字
		std::string m_caption;
		// 支持的属性类型 空:属性及序列都支持  property_only / curve_only
		std::string m_property_support;

		PropertyModifierType m_property_modifier_type;// 包括是否为曲线
	};

	std::list<DefinedItem*> m_sub_item; /// 子物品列表
	std::list<DefinedItem::ValueItem*> m_property_list; /// 属性列表
	std::list<std::string> m_property_order_recoder; ///记录从文件中加载的属性的顺序 //这个貌似没有用，可以删除TODO

	std::string m_id; // 物品ID标识，同一根节点下的子物品ID不同
	std::string m_item_type; // 物品的基本类型 

	NODE_TYPE m_node_type;//
};

class CDefinedItemCollection
{
protected:
	std::map<std::string,DefinedItem*> m_defined_item_map;
protected:
//添加属性修改器
	bool addPropertyOnItem(BiXmlNode* propertyNode
								,const char* actor_level
								,const char* child_entity_id
								,bool is_curv
								,DefinedItem* defined_item);
	//递归建立动态效果物体和子实体
	DefinedItem*  createPropertyRecursive(BiXmlNode* xmlNode,
											 std::string& parent_actor_level 	);
	bool createObjectProperty(BiXmlNode* effectNode);
	
public:
	CDefinedItemCollection();
	virtual ~CDefinedItemCollection();
	//读取预定义物体信息
	virtual bool load(const char* filename, bool isBinary) ;
	//取预定义信息map
	const std::map<std::string,DefinedItem*>& getDefinedItemMap() const {return m_defined_item_map;};
	//根据类型取预定义信息
	const DefinedItem* getDefinedItem(const char* type)  const;
	//取一个具体的子节点的属性信息
	const DefinedItem::ValueItem* getSubItemValue(const char* type, PropertyModifierType& modifier_type) const;
};
class CStageDefinedItemCollection: public CDefinedItemCollection
{
protected:
	bool createObjectPropertyById(BiXmlNode* effectNode);
public:
	CStageDefinedItemCollection();
	virtual ~CStageDefinedItemCollection();
	//从物体文件中读取物体属性
	bool loadFromXmlNode(BiXmlNode* stageEntitys);
	const DefinedItem* getStageDefinedItem(const char* id)  const;
};
#endif //_EDITOR_INFO_TABLE_H_
