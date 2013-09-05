/**
* @file system_entity.h
* ����ļ�����Ҫ�����ǣ�
* 
* @author �̳�(Chengchen@h3d.com.cn)
* 
* ������־
* ----------------------------------------------------
* ver 1.0.0
*       by �̳�2010.12.17
*/

#ifndef _SYSTEM_ENTITY_H_
#define _SYSTEM_ENTITY_H_

#include "game_graphic_header.h"

class CMediaEndEntity;
class CRenderSystem;
class CSceneEntity;

class CSystemEntity : public CMediaEndEntity
{
public:
	CSystemEntity(CGraphicsResMng* mng, CSceneEntity* scene, const std::string& entity_name);

	/**
	* @brief
	*		���ñ�����
	*/
	virtual void set_ambient_color(const H3DColor& ambient_color);

	/**
	* @brief
	*		��ȡ������
	*/
	virtual H3DColor get_ambient_color() const;

	/**
	* @brief
	*		�������ﱳ����
	*/
	virtual void set_ambient_actor_color(const H3DColor& ambient_color);

	/**
	* @brief
	*		��ȡ���ﱳ����
	*/
	virtual H3DColor get_ambient_actor_color() const;

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

private:
	H3DColor			m_ambient_color; ///< ��������ɫ
	H3DColor			m_ambient_actor_color; ///< ���ﱳ������ɫ
	CRenderSystem*		m_render_system; /// ý������Ⱦϵͳ
	CSceneEntity*		m_scene;
	
};


#endif //_SYSTEM_ENTITY_H_
