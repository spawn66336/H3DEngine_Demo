/**
* @file system_entity.h
* 这个文件的主要功能是：
* 
* @author 程陈(Chengchen@h3d.com.cn)
* 
* 更改日志
* ----------------------------------------------------
* ver 1.0.0
*       by 程陈2010.12.17
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
	*		设置背景光
	*/
	virtual void set_ambient_color(const H3DColor& ambient_color);

	/**
	* @brief
	*		获取背景光
	*/
	virtual H3DColor get_ambient_color() const;

	/**
	* @brief
	*		设置人物背景光
	*/
	virtual void set_ambient_actor_color(const H3DColor& ambient_color);

	/**
	* @brief
	*		获取人物背景光
	*/
	virtual H3DColor get_ambient_actor_color() const;

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

private:
	H3DColor			m_ambient_color; ///< 背景光颜色
	H3DColor			m_ambient_actor_color; ///< 人物背景光颜色
	CRenderSystem*		m_render_system; /// 媒体层的渲染系统
	CSceneEntity*		m_scene;
	
};


#endif //_SYSTEM_ENTITY_H_
