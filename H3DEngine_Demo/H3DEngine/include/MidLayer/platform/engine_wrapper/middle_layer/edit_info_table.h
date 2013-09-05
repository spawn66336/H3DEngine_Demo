/**
* @file editor_info_table.h
* ����ļ�����Ҫ�����ǣ�
* 
* @author ����
* 
* ������־
* ----------------------------------------------------
* ver 1.0.0
*       by ����2010.11.16
*/
#ifndef _EDITOR_INFO_TABLE_H_
#define _EDITOR_INFO_TABLE_H_

#include "middle_layer_header.h"

struct DefinedItem 
{
	enum NODE_TYPE{INVALID_TYPE, DEFINED_EFFECT_OBJECT , NORMAL_EFFECT_OBJECT , NORMAL_ENTITY  };//�ڵ����Ͷ��壺 Ԥ�������壬��ͨ��̬Ч�����壬ʵ��
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
	// ֵ��
	struct ValueItem
	{
		ValueItem(){}

		ValueItem(const char* vtype, const char* value, const char* min_value, const char* max_value, const char* caption)
			: m_v_type(vtype), m_value(value), m_caption(caption), m_min_value(min_value), m_max_value(max_value)
		{
		}

		// ֵ������
		std::string m_v_type;
		
		// �����ֵ
		std::string m_value;
		// ��Сֵ
		std::string m_min_value;
		// ���ֵ
		std::string m_max_value;
		// ��ʾ������
		std::string m_caption;
		// ֧�ֵ��������� ��:���Լ����ж�֧��  property_only / curve_only
		std::string m_property_support;

		PropertyModifierType m_property_modifier_type;// �����Ƿ�Ϊ����
	};

	std::list<DefinedItem*> m_sub_item; /// ����Ʒ�б�
	std::list<DefinedItem::ValueItem*> m_property_list; /// �����б�
	std::list<std::string> m_property_order_recoder; ///��¼���ļ��м��ص����Ե�˳�� //���ò��û���ã�����ɾ��TODO

	std::string m_id; // ��ƷID��ʶ��ͬһ���ڵ��µ�����ƷID��ͬ
	std::string m_item_type; // ��Ʒ�Ļ������� 

	NODE_TYPE m_node_type;//
};

class CDefinedItemCollection
{
protected:
	std::map<std::string,DefinedItem*> m_defined_item_map;
protected:
//��������޸���
	bool addPropertyOnItem(BiXmlNode* propertyNode
								,const char* actor_level
								,const char* child_entity_id
								,bool is_curv
								,DefinedItem* defined_item);
	//�ݹ齨����̬Ч���������ʵ��
	DefinedItem*  createPropertyRecursive(BiXmlNode* xmlNode,
											 std::string& parent_actor_level 	);
	bool createObjectProperty(BiXmlNode* effectNode);
	
public:
	CDefinedItemCollection();
	virtual ~CDefinedItemCollection();
	//��ȡԤ����������Ϣ
	virtual bool load(const char* filename, bool isBinary) ;
	//ȡԤ������Ϣmap
	const std::map<std::string,DefinedItem*>& getDefinedItemMap() const {return m_defined_item_map;};
	//��������ȡԤ������Ϣ
	const DefinedItem* getDefinedItem(const char* type)  const;
	//ȡһ��������ӽڵ��������Ϣ
	const DefinedItem::ValueItem* getSubItemValue(const char* type, PropertyModifierType& modifier_type) const;
};
class CStageDefinedItemCollection: public CDefinedItemCollection
{
protected:
	bool createObjectPropertyById(BiXmlNode* effectNode);
public:
	CStageDefinedItemCollection();
	virtual ~CStageDefinedItemCollection();
	//�������ļ��ж�ȡ��������
	bool loadFromXmlNode(BiXmlNode* stageEntitys);
	const DefinedItem* getStageDefinedItem(const char* id)  const;
};
#endif //_EDITOR_INFO_TABLE_H_
