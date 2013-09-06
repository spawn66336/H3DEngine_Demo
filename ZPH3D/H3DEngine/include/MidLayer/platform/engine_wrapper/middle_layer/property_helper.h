/*
*@file  property_helper.h
*@breif 提供解析文件后设置属性时的帮助函数的静态类声明
*@author mozisi
*/
#ifndef __PROPERTY_HELPER_H__
#define __PROPERTY_HELPER_H__
#include "middle_layer_header.h"

/*
*@breif 提供解析文件后设置属性时的帮助函数的静态类声明
*/
class CPropertyHelper
{
public:

	//实现拷贝自 stage_animation_clip_mng
	static bool addPropertyModifier(PropertyModifierFactory* modifier_factory
									,BiXmlNode* propertyNode
									,const char* actor_level
									,const char* child_entity_id
									,bool is_curv
									,Actor* top_effect_obj
									,ActorAnimationClip* ani_clip);
};

#endif
