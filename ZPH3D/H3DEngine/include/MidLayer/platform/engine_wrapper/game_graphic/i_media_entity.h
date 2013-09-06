/**
* @file i_media_entity.h
* ����ļ�����Ҫ�����ǣ�
* 
* @author �̳�(Chengchen@h3d.com.cn)
* 
* ������־
* ----------------------------------------------------
* ver 1.0.0
*       by �̳�2010.1.25
*/

#ifndef _I_MEDIA_ENTITY_H_
#define _I_MEDIA_ENTITY_H_

#include "game_graphic_header.h"

enum MEDIA_STATUS
{
	MEDIA_STOP = 0,    //����ֹͣ
	MEDIA_PLAYING = 1, //���ڲ���
	MEDIA_PAUSE = 2,   //��ͣ����
	MEDIA_FINISH = 3  // �������Ҳ��ֹͣ
};

class IMediaEntity
{
public:
	virtual ~IMediaEntity(void) {};

	/**
	* @brief
	*		����ý��
	*/
	virtual void play(void) = 0;

	/**
	* @brief
	*		��ͣ����ý��
	*/
	virtual void pause(void) = 0;

	/**
	* @brief
	*		ֹͣ����ý��
	*/
	virtual void stop(void) = 0;

	/**
	* @brief
	*		���¿�ʼ
	*/
	virtual void reset(void) = 0;

	/**
	* @brief
	*		��ת��ָ��λ��
	*/
	virtual void skip_to(float time) = 0;

	/**
	* @brief
	*		�趨�Ƿ�ѭ������
	* @param
	*		is_loop����IN���趨ý���Ƿ�ѭ����true��ѭ����false ��ѭ��
	*/
	virtual void set_loop(bool loop) = 0;

	/**
	* @brief
	*		��ȡý���Ƿ�ѭ��
	* @return
	*		TRUE�� ѭ������ý��
	*		FALSE����ѭ������ý��
	*/
	virtual bool is_loop(void) = 0;

	/**
	* @brief
	*		�趨ý�峤��
	* @param
	*		lenght����IN��ý�峤��
	*/
	virtual void set_media_length(float length) = 0;

	/**
	* @brief
	*		��ȡý�峤��
	* @return
	*		ý�峤��
	*/
	virtual float media_length(void) const = 0;

	/**
	* @brief
	*		��ȡ��ǰ�Ĳ���ʱ��
	* @return
	*		��ǰ�Ĳ���ʱ��
	*/
	virtual float current_time(void) const = 0;

	/**
	* @brief
	*		�趨ý�岥���ٶ�
	* @param
	*		speed����IN��ý��Ĳ����ٶ�
	*/
	virtual void set_play_speed(float speed) = 0;

	/**
	* @brief
	*		��ȡý��Ĳ����ٶ�
	* @return
	*		ý��Ĳ����ٶ�
	*/
	virtual float play_speed(void) const = 0;

	/**
	* @brief
	*		��ȡý���״̬
	* @return
	*		����ý���״̬
	*/
	virtual MEDIA_STATUS status(void) const = 0;
};

#endif //_I_MEDIA_ENTITY_H_
