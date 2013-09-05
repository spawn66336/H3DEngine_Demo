/**
* @file media_end_entity_type.h
* ����ļ�����Ҫ�����ǣ�ͼ�οɿ�ʵ������Ͷ���
* 
* @author �̳�(Chengchen@h3d.com.cn)
* 
* ������־
* ----------------------------------------------------
* ver 1.0.0
*       ������� by �̳�2010.1.8
*/

#ifndef _MEDIA_END_ENTITY_TYPE_H_
#define _MEDIA_END_ENTITY_TYPE_H_

#include "game_graphic_header.h"

/**
* @brief
*    �ɿ�ʵ�������ֵ����
*    ���ڶ�̬����ʱ������ʶ��
*/
enum MEDIA_END_ENTITY_TYPE{
	//MEDIA_END_ENTITY_TYPE_GAME_GRAPHICS, //ͼ�β�����Ľӿ�
	MEDIA_END_ENTITY_TYPE_ENTITY = 1,  //�ɿ�ʵ��
	MEDIA_END_ENTITY_TYPE_ACTOR, //Actor�ɿ�ʵ��
	MEDIA_END_ENTITY_TYPE_ACTOR2D, //2dActor�ɿ�ʵ��
	MEDIA_END_ENTITY_TYPE_MOVE_ENTITY, //���ƶ�ʵ��
	MEDIA_END_ENTITY_TYPE_MEDIA_ENTITY, //�ɲ���ʵ��
	MEDIA_END_ENTITY_TYPE_3D_ENTITY,  //3D�ɿ�ʵ��
	MEDIA_END_ENTITY_TYPE_2D_ENTITY,  //2D�ɿ�ʵ��
	MEDIA_END_ENTITY_TYPE_ACTION_ENTITY,  //�����ɿ�ʵ��
	MEDIA_END_ENTITY_TYPE_STATIC_FRAME_ENTITY,  //����ɿ�ʵ��
	//MEDIA_END_ENTITY_TYPE_GRAPHICS_RES_MNG, //ͼ�β����Դ������
	MEDIA_END_ENTITY_TYPE_LEVEL , //�����ɿ�ʵ��
	MEDIA_END_ENTITY_TYPE_LIGHT, //�ƹ�ɿ�ʵ��
	MEDIA_END_ENTITY_TYPE_PREPASS_LIGHT, //Ԥ����ĵƹ�ɿ�ʵ��
	MEDIA_END_ENTITY_TYPE_WIMBLE_PREPASS_LIGHT, //Ԥ�����׶��Դ�ɿ�ʵ��
	MEDIA_END_ENTITY_TYPE_POINT_PREPASS_LIGHT, //Ԥ����ĵ��Դ�ɿ�ʵ��
	MEDIA_END_ENTITY_TYPE_DIRECTION_PREPASS_LIGHT, //�����Դ�ɿ�ʵ��
	MEDIA_END_ENTITY_TYPE_CASCADE, //�����ɿ�ʵ��
	MEDIA_END_ENTITY_TYPE_LUMINARIE, //�ƾ߿ɿ�ʵ��
	MEDIA_END_ENTITY_TYPE_WIMBLE_LUMINARIE, //�ƾ߿ɿ�ʵ��
	MEDIA_END_ENTITY_TYPE_FLOOD_LUMINARIE, //�ƾ߿ɿ�ʵ��
	MEDIA_END_ENTITY_TYPE_VIRTUAL_LUMINARIE, //�ƾ߿ɿ�ʵ��
	MEDIA_END_ENTITY_TYPE_MODEL, //ģ�Ϳɿ�ʵ��
	MEDIA_END_ENTITY_TYPE_AVATAR, //�赸�߿ɿ�ʵ��
	MEDIA_END_ENTITY_TYPE_AVATAR_GROUP, //�赸����ɿ�ʵ��
	MEDIA_END_ENTITY_TYPE_CAMERA, //����ɿ�ʵ��
	MEDIA_END_ENTITY_TYPE_EFFECT, //��Ч�ɿ�ʵ��
	MEDIA_END_ENTITY_TYPE_SOUND, //�����ɿ�ʵ��
	MEDIA_END_ENTITY_TYPE_MUSIC, //���ֿɿ�ʵ��
	MEDIA_END_ENTITY_TYPE_NOTIFY_SOUND, //��ʾ���ɿ�ʵ��
	MEDIA_END_ENTITY_TYPE_NOTE,  //�����ɿ�ʵ��
	MEDIA_END_ENTITY_TYPE_NOTE_SEG, //һ�������ɿ�ʵ��
	MEDIA_END_ENTITY_TYPE_NOTE_LINE, //һ�������ɿ�ʵ��
	MEDIA_END_ENTITY_TYPE_NOTE_ITEM,  //���������ɿ�ʵ��
	MEDIA_END_ENTITY_TYPE_ATTRIBUTE_SEQUENCE,//��������
	MEDIA_END_ENTITY_TYPE_FOLLOWING_BIND_ENTITY, //����ʵ��׷���Ч���İ�ʵ��
	MEDIA_END_ENTITY_TYPE_SYSTEM, ///ϵͳ
	MEDIA_END_ENTITY_TYPE_LIGHTMAP_ADJUST,	//Lightmap��������
	MEDIA_END_ENTITY_TYPE_SKELETON_MODEL,	//skeleton_modelʵ��

	MEDIA_END_ENTITY_TYPE_AVATAR_SKELETON_MODEL,	//avatar_skeleton_modelʵ��
	MEDIA_END_ENTITY_TYPE_END,		//����Ҫʼ�շ���������ڱ�Ƿ���չ��MEDIA_END_ENTITY������
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
