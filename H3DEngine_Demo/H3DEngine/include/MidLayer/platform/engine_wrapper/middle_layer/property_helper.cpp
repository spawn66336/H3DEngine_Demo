/*
*@file  property_helper.cpp
*@breif �ṩ�����ļ�����������ʱ�İ��������ľ�̬��
*@author mozisi
*/

#include "middle_layer_body.h"
#include "property_helper.h"

bool CPropertyHelper::addPropertyModifier(PropertyModifierFactory* modifier_factory 
												 ,BiXmlNode* propertyNode 
												 ,const char* actor_level 
												 ,const char* child_entity_id 
												 ,bool is_curv 
												 ,Actor* top_effect_obj 
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
		if(element)
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
