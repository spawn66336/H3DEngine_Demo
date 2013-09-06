/*
*@file  property_helper.h
*@breif �ṩ�����ļ�����������ʱ�İ��������ľ�̬������
*@author mozisi
*/
#ifndef __PROPERTY_HELPER_H__
#define __PROPERTY_HELPER_H__
#include "middle_layer_header.h"

/*
*@breif �ṩ�����ļ�����������ʱ�İ��������ľ�̬������
*/
class CPropertyHelper
{
public:

	//ʵ�ֿ����� stage_animation_clip_mng
	static bool addPropertyModifier(PropertyModifierFactory* modifier_factory
									,BiXmlNode* propertyNode
									,const char* actor_level
									,const char* child_entity_id
									,bool is_curv
									,Actor* top_effect_obj
									,ActorAnimationClip* ani_clip);
};

#endif
