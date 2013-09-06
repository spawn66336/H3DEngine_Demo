/**
* @file implement.h
* 这个文件的主要功能是：外部使用的接口文件
* 
* @author 程陈(Chengchen@h3d.com.cn)
* 
* 更改日志
* ----------------------------------------------------
* ver 1.0.0
*       by 程陈2010.1.26
*/

#ifndef _GAME_GRAPHIC_INTERFACE_H_
#define _GAME_GRAPHIC_INTERFACE_H_

#include "game_graphic_header.h"

#include "error_code.h"
#include "media_end_entity_type.h"
#include "property_type.h"

//#include "param_parse.h"
#include "system_info.h"
//#include "function_parameter.h"
#include "bezier.h"
#include "equation.h"

#include "i_move_entity.h"
#include "i_media_entity.h"

#include "media_end_entity.h"
#include "3DEntity.h"
#include "extend_entity_creator.h"
#include "entity_appearance.h"

#include "camera_entity.h"
#include "model_entity.h"
#include "effect_entity.h"
#include "skeleton_model_entity.h"
#include "avatarentity.h"
#include "avatar_skeleton_model_entity.h"
#include "system_entity.h"
#include "lightmap_adjust_entity.h"

#include "scene_entity.h"
#include "level_entity.h"

#include "prepass_light_entity.h"
#include "point_prepass_light_entity.h"
#include "wimble_prepass_light_entity.h"
#include "direction_prepass_light_entity.h"


#include "render_system.h"
#include "entity_factory.h"
#include "graphics_res_mng.h"

#include "i_actor_control.h"
#include "actor_control.h"
#include "property_modifier.h"
#include "property_modifier_creator.h"

#include "actor_manager.h"
#include "actor.h"
#include "target_position.h"

#endif //_GAME_GRAPHIC_INTERFACE_H_
