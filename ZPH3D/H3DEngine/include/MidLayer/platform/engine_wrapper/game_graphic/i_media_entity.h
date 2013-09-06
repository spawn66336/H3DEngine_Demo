/**
* @file i_media_entity.h
* 这个文件的主要功能是：
* 
* @author 程陈(Chengchen@h3d.com.cn)
* 
* 更改日志
* ----------------------------------------------------
* ver 1.0.0
*       by 程陈2010.1.25
*/

#ifndef _I_MEDIA_ENTITY_H_
#define _I_MEDIA_ENTITY_H_

#include "game_graphic_header.h"

enum MEDIA_STATUS
{
	MEDIA_STOP = 0,    //播放停止
	MEDIA_PLAYING = 1, //正在播放
	MEDIA_PAUSE = 2,   //暂停播放
	MEDIA_FINISH = 3  // 播放完成也会停止
};

class IMediaEntity
{
public:
	virtual ~IMediaEntity(void) {};

	/**
	* @brief
	*		播放媒体
	*/
	virtual void play(void) = 0;

	/**
	* @brief
	*		暂停播放媒体
	*/
	virtual void pause(void) = 0;

	/**
	* @brief
	*		停止播放媒体
	*/
	virtual void stop(void) = 0;

	/**
	* @brief
	*		重新开始
	*/
	virtual void reset(void) = 0;

	/**
	* @brief
	*		跳转到指定位置
	*/
	virtual void skip_to(float time) = 0;

	/**
	* @brief
	*		设定是否循环播放
	* @param
	*		is_loop：【IN】设定媒体是否循环，true：循环，false 不循环
	*/
	virtual void set_loop(bool loop) = 0;

	/**
	* @brief
	*		获取媒体是否循环
	* @return
	*		TRUE： 循环播放媒体
	*		FALSE：不循环播放媒体
	*/
	virtual bool is_loop(void) = 0;

	/**
	* @brief
	*		设定媒体长度
	* @param
	*		lenght：【IN】媒体长度
	*/
	virtual void set_media_length(float length) = 0;

	/**
	* @brief
	*		获取媒体长度
	* @return
	*		媒体长度
	*/
	virtual float media_length(void) const = 0;

	/**
	* @brief
	*		获取当前的播放时间
	* @return
	*		当前的播放时间
	*/
	virtual float current_time(void) const = 0;

	/**
	* @brief
	*		设定媒体播放速度
	* @param
	*		speed：【IN】媒体的播放速度
	*/
	virtual void set_play_speed(float speed) = 0;

	/**
	* @brief
	*		获取媒体的播放速度
	* @return
	*		媒体的播放速度
	*/
	virtual float play_speed(void) const = 0;

	/**
	* @brief
	*		获取媒体的状态
	* @return
	*		返回媒体的状态
	*/
	virtual MEDIA_STATUS status(void) const = 0;
};

#endif //_I_MEDIA_ENTITY_H_
