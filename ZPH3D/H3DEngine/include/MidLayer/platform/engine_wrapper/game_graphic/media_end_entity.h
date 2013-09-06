/**
* @file media_end_entity.h
* 这个文件的主要功能是：声明了媒体层的基类接口。
* 
* @author 程陈(Chengchen@h3d.com.cn)
* 
* 更改日志
* ----------------------------------------------------
* ver 1.0.0
*       by 程陈2010.1.25
*/

#ifndef _MEDIA_ENTITY_H_
#define _MEDIA_ENTITY_H_

#include "game_graphic_header.h"

class CGraphicsResMng;
class CRenderSystem;
class Actor;
class CSceneEntity;
/**
* @brief
*		CMediaEndEntity是可控实体的基类，
*		定义了可控实体应该具有的一些基本的行为
*/
class CMediaEndEntity
	//:public SignalDispatcher
{
public:
	CMediaEndEntity(CGraphicsResMng* mng, const std::string& entity_name);
	CMediaEndEntity();
	virtual ~CMediaEndEntity(void);

	/**
	* @brief
	*		获取指向图形绘制层资源管理器的指针
	* @return
	*		指向图形绘制层资源管理器的指针
	*/
	virtual CGraphicsResMng* get_graphics_res_mng(void) ;


	//////////////////////////////////////////////////////////////////////////
	/**
	* @brief
	*		获取类的类型
	* @return
	*		类的类型
	*/
	virtual int		type(void) const;

	/**
	* @brief
	*		获取对象名称
	* @param
	*		name：【IN】对象的名称
	*/
	virtual void		set_name(const char* obj_name) ;

	/**
	* @brief
	*		获取对象名称
	* @return
	*		对象的名称
	*/
	virtual const char*	get_name(void) const;

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

	/**
	* @brief 
	*		可控实体是否可用
	* @return 
	*		True：可控实体可用
	*		False：可控实体不可用
	*/
	virtual bool is_enable(void) const ;

	/**
	* @brief
	*		设置可控实体的状态是否可用
	* @param
	*		is_eanbel：【IN】可控实体的状态设置标志
	*/
	virtual void set_enable(bool enable) ;

	/**
	* @brief
	*		更新对象
	* @param
	*		intervalTime：【IN】距离上次更新的事件间隔，单位毫秒
	*/
	virtual void	update(float interval) ;

	/**
	* @brief
	*		渲染对象
	* @param
	*		renderSystem：【IN】绘制系统
	*/
	virtual void render(CRenderSystem& render_system);

	/**
	* @brief
	*		进行硬件选择渲染
	* @param
	*		renderSystem：【IN】绘制系统
	*		sel_line: 是否是线渲染
	* @return
	*		true：进行渲染
	*		false：不进行渲染
	*/
	virtual bool pushHardwareSelect(CRenderSystem& render_system, bool sel_line);


	/**
	* @brief
	*		设置Actor
	*/
	virtual void set_actor(Actor* actor);
	/**
	* @brief
	*		获取Actor
	*/
	virtual Actor* get_actor();


	unsigned int get_entity_id() const;

	virtual void setColorFactor(float* color, int size) {};

	/**
	* @brief
	*		设置和获取模型LOD控制， 目前中间层LOD设置仅影响模型及其子类、特效、和锥光灯
	*/
	virtual void switch_to_lod(int lod, bool is_sync = true){m_lod_level = lod;};
	LOD_LEVEL get_lod(){return (LOD_LEVEL)m_lod_level;};

	/**
	* @brief
	*		添加进level
	*/
	virtual void add_to_level(CSceneEntity* level){m_level_entity = level;};
	virtual void remove_from_level(CSceneEntity* level)
	{
		assert(level == m_level_entity);
		m_level_entity = NULL;
	};

	/**
	* @brief
	*		设置和获取模型LOD控制， 目前中间层LOD设置仅影响模型及其子类、特效、和锥光灯
	*/
	bool is_added_to_level(){return m_level_entity != 0;};

	/*
	* @brief 
	*		设置物体的GUID属性
	*/
	virtual void set_guid(const char* guid){ m_guid = guid;}
	const char* get_guid()const {return m_guid.c_str();};

	/**/
	void set_update_delta_time(float delta)
	{
		m_b_use_delta_time = true;
		m_f_delta_time = delta;
	}

	/**
	* @brief
	*		设定对象是否可见
	* @param
	*		is_visible：【IN】设定对象是否可见 ,空实现,由子类实现
	*/
	virtual void set_visible(bool visible) {};

	/*
	* @brief
	*		用于外部设定对象是否可见，该控制在属性和曲线动画控制之外，控制entity的显隐；
	*
	*/
	virtual void set_external_visible(bool visible){};

protected:
	/**
	* @brief
	*		更新对象
	* @param
	*		intervalTime：【IN】距离上次更新的事件间隔，单位毫秒
	*/
	virtual void do_update(float interval);

protected:
	std::string m_name; ///< 对象名称
	bool	m_is_enable;				///< 可控实体是否可用

	CGraphicsResMng*	m_res_mng;  ///< 指向图形资源管理器的指针

	Actor*		m_actor; ///< 所属的Actor

	unsigned int					m_entity_id;
	static	unsigned int			m_s_entity_id;

	int				m_lod_level;
	/*
	*用于表示该entity是否已经被加入到level中。
	*仅适用与CPrePassLightEntity及其子类，CModelEntity及其子类
	*特效等
	*/
	CSceneEntity*		m_level_entity;
	std::string				m_guid;		//GUID属性

	/*
	* 对于动作播放、特效播放等与时间相关的属性修改器，	
	* 在一帧更新中（设更新时间为t），对应的曲线以及属性的更新驱动了动作播放以及特效播放，
	* 对应的entity所应更新的时间应该是t1（t=t0+t1；t0表示这一帧中设置当前动作/特效之前的时间，
	* t1表示这一帧中设置当前动作/特效之后的时间）
	* 这里的t1用m_f_delta_time来表示；
	* m_b_use_delta_time用于标示是否使用m_f_delta_time更新entity，具体的逻辑处理在update()中
	*/
	bool	m_b_use_delta_time;
	float	m_f_delta_time;
protected:
//private:
	//	FORBIDDEN_EVIL_CONSTRUCTOR(CMediaEndEntity);
};

#endif //_MEDIA_ENTITY_H_
