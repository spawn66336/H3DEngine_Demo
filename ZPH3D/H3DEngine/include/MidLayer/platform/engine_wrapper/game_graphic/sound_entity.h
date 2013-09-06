/*
*@file  sound_entity.h
*@breif ��װ����ʵ�����м��ʹ��
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
	*		��ȡ�������
	* @return
	*		�������
	*/
	virtual int		type(void) const;

	/**
	* @brief
	*		�ж϶����Ƿ�ΪҪ�������
	* @param
	*		obj_type����IN���������
	* @return
	*		true��Ϊָ��������
	*		false����Ϊָ��������
	*/
	virtual bool is_kind_of(int obj_type) ;

	/*
	*@breif ��������
	*@param 
	*@return 
	*/
	virtual void play(void);

	/*
	*@breif ����ֹͣ
	*@param 
	*@return 
	*/
	virtual void stop(void);

	/*
	*@breif ����Դ
	*@param res_name ��Դ��
	*@return 
	*/
	virtual bool change_resource(const std::string& res_name);

protected:

	///< ������ʵ������ʵ��,�ⲿ���룬��musicPlayer����
	H3D_CLIENT::I_MusicEntity*	m_i_music_entity;

	//std::string				m_resource_name; ///< ��Դ·��
};

#endif