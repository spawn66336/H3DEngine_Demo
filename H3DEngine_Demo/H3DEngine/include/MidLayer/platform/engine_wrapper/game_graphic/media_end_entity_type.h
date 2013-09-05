/**
* @file media_end_entity_type.h
* 这个文件的主要功能是：图形可控实体的类型定义
* 
* @author 程陈(Chengchen@h3d.com.cn)
* 
* 更改日志
* ----------------------------------------------------
* ver 1.0.0
*       编码完成 by 程陈2010.1.8
*/

#ifndef _MEDIA_END_ENTITY_TYPE_H_
#define _MEDIA_END_ENTITY_TYPE_H_

#include "game_graphic_header.h"

/**
* @brief
*    可控实体的类型值定义
*    用于动态运行时的类型识别
*/
enum MEDIA_END_ENTITY_TYPE{
	//MEDIA_END_ENTITY_TYPE_GAME_GRAPHICS, //图形层对外层的接口
	MEDIA_END_ENTITY_TYPE_ENTITY = 1,  //可控实体
	MEDIA_END_ENTITY_TYPE_ACTOR, //Actor可控实体
	MEDIA_END_ENTITY_TYPE_ACTOR2D, //2dActor可控实体
	MEDIA_END_ENTITY_TYPE_MOVE_ENTITY, //可移动实体
	MEDIA_END_ENTITY_TYPE_MEDIA_ENTITY, //可播放实体
	MEDIA_END_ENTITY_TYPE_3D_ENTITY,  //3D可控实体
	MEDIA_END_ENTITY_TYPE_2D_ENTITY,  //2D可控实体
	MEDIA_END_ENTITY_TYPE_ACTION_ENTITY,  //动作可控实体
	MEDIA_END_ENTITY_TYPE_STATIC_FRAME_ENTITY,  //表情可控实体
	//MEDIA_END_ENTITY_TYPE_GRAPHICS_RES_MNG, //图形层的资源管理器
	MEDIA_END_ENTITY_TYPE_LEVEL , //场景可控实体
	MEDIA_END_ENTITY_TYPE_LIGHT, //灯光可控实体
	MEDIA_END_ENTITY_TYPE_PREPASS_LIGHT, //预处理的灯光可控实体
	MEDIA_END_ENTITY_TYPE_WIMBLE_PREPASS_LIGHT, //预处理的锥光源可控实体
	MEDIA_END_ENTITY_TYPE_POINT_PREPASS_LIGHT, //预处理的点光源可控实体
	MEDIA_END_ENTITY_TYPE_DIRECTION_PREPASS_LIGHT, //方向光源可控实体
	MEDIA_END_ENTITY_TYPE_CASCADE, //级联可控实体
	MEDIA_END_ENTITY_TYPE_LUMINARIE, //灯具可控实体
	MEDIA_END_ENTITY_TYPE_WIMBLE_LUMINARIE, //灯具可控实体
	MEDIA_END_ENTITY_TYPE_FLOOD_LUMINARIE, //灯具可控实体
	MEDIA_END_ENTITY_TYPE_VIRTUAL_LUMINARIE, //灯具可控实体
	MEDIA_END_ENTITY_TYPE_MODEL, //模型可控实体
	MEDIA_END_ENTITY_TYPE_AVATAR, //舞蹈者可控实体
	MEDIA_END_ENTITY_TYPE_AVATAR_GROUP, //舞蹈者组可控实体
	MEDIA_END_ENTITY_TYPE_CAMERA, //相机可控实体
	MEDIA_END_ENTITY_TYPE_EFFECT, //特效可控实体
	MEDIA_END_ENTITY_TYPE_SOUND, //声音可控实体
	MEDIA_END_ENTITY_TYPE_MUSIC, //音乐可控实体
	MEDIA_END_ENTITY_TYPE_NOTIFY_SOUND, //提示音可控实体
	MEDIA_END_ENTITY_TYPE_NOTE,  //音符可控实体
	MEDIA_END_ENTITY_TYPE_NOTE_SEG, //一段音符可控实体
	MEDIA_END_ENTITY_TYPE_NOTE_LINE, //一行音符可控实体
	MEDIA_END_ENTITY_TYPE_NOTE_ITEM,  //单个音符可控实体
	MEDIA_END_ENTITY_TYPE_ATTRIBUTE_SEQUENCE,//属性序列
	MEDIA_END_ENTITY_TYPE_FOLLOWING_BIND_ENTITY, //用于实现追光灯效果的绑定实体
	MEDIA_END_ENTITY_TYPE_SYSTEM, ///系统
	MEDIA_END_ENTITY_TYPE_LIGHTMAP_ADJUST,	//Lightmap调整对象
	MEDIA_END_ENTITY_TYPE_SKELETON_MODEL,	//skeleton_model实体

	MEDIA_END_ENTITY_TYPE_AVATAR_SKELETON_MODEL,	//avatar_skeleton_model实体
	MEDIA_END_ENTITY_TYPE_END,		//这项要始终放在最后，用于标记非扩展的MEDIA_END_ENTITY的数量
};

typedef struct _STRUCT_BBOX_ 
{
	H3DVec3 m_min_pos;
	H3DVec3 m_max_pos;
	_STRUCT_BBOX_()
	{
		m_min_pos = H3DVec3(0, 0, 0);
		m_max_pos = H3DVec3(0, 0, 0);
	}
}BBOX,*PBBOX;

#endif //_MEDIA_END_ENTITY_TYPE_H_
