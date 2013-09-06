/*
*@file  sound_entity.h
*@breif 封装声音实体在中间层使用
*@author mozisi
*/

#ifndef __SOUND_ENTITY_H__
#define __SOUND_ENTITY_H__

#include "game_graphic_header.h"

class CMediaEndEntity;

namespace H3D_CLIENT
{
	class I_MusicEntity;
}

class CSoundEntity
	: public CMediaEndEntity
{

public:
	CSoundEntity(CGraphicsResMng* res_mng, const std::string& entity_name, H3D_CLIENT::I_MusicEntity* music_entity);
	virtual ~CSoundEntity();

private:
	CSoundEntity(const CSoundEntity&);
	CSoundEntity& operator=(const CSoundEntity&);

public:
	/**
	* @brief
	*		获取类的类型
	* @return
	*		类的类型
	*/
	virtual int		type(void) const;

	/**
	* @brief
	*		判断对象是否为要求的类型
	* @param
	*		obj_type：【IN】类的类型
	* @return
	*		true：为指定的类型
	*		false：不为指定的类型
	*/
	virtual bool is_kind_of(int obj_type) ;

	/*
	*@breif 播放声音
	*@param 
	*@return 
	*/
	virtual void play(void);

	/*
	*@breif 声音停止
	*@param 
	*@return 
	*/
	virtual void stop(void);

	/*
	*@breif 换资源
	*@param res_name 资源名
	*@return 
	*/
	virtual bool change_resource(const std::string& res_name);

protected:

	///< 包含的实际声音实例,外部传入，在musicPlayer销毁
	H3D_CLIENT::I_MusicEntity*	m_i_music_entity;

	//std::string				m_resource_name; ///< 资源路径
};

#endif