/**
* @file level_entity.h
* 这个文件的主要功能是：声明引擎层的ILevel类的包装类CLevelEntity
* 
* @author 王恒(Wangheng@h3d.com.cn)
* 
* 更改日志
* ----------------------------------------------------
* ver 1.0.0
*       by 王恒2012.2.28
*/

#ifndef _SCENE_ENTITY_H_
#define _SCENE_ENTITY_H_

#include "game_graphic_header.h"

class CGraphicsResMng;
class CCameraEntity;
class C3DEntity;
enum LOD_LEVEL;

class CSceneEntity
{
public:
	CSceneEntity(CGraphicsResMng* res_mng, H3DI::IScene* scene);
	virtual ~CSceneEntity();
	/**
	* @brief
	*		设置背景光
	*/
	virtual void set_ambient_color(const H3DColor& ambient_color);

	/**
	* @brief
	*		设置人物背景光
	*/
	virtual void set_ambient_actor_color(const H3DColor& ambient_actor_color);


	/**
	* @brief
	*		获取背景光
	*/
	virtual const H3DColor& get_ambient_color() const;

	/**
	* @brief
	*		获取人物背景光
	*/
	const H3DColor& get_ambient_actor_color() const;

	/**
	* @brief
	*		设置阴影范围
	*/
	virtual void  set_shadow_scope(const float& shadow_scope);

	 /**
   * \brief
   * 设置视角摄像机
   * 
   * \param camera
   * 摄像机
   *
   * \returns
   * 老的摄像机
   */
   virtual CCameraEntity* set_render_camera(CCameraEntity* camera);

   /**
   * \brief
   * 设置视角摄像机
   * 
   * \param camera
   * 摄像机
   *
   * \returns
   * 老的摄像机
   */
   //virtual CCameraEntity* set_render_camera(const std::string& camera_name);   
   /**
   * \brief
   * 获得视角摄像机
   *
   * \returns
   * 绘制摄像机
   */
   virtual CCameraEntity* get_render_camera() const;

	virtual void add_postprocess(const char* filename);
	virtual void del_postprocess(const char* filename);
	virtual void set_postprocess_param(const char *name,void *pvalue,int sizeofvalue);

	/**
	* \brief
	* 开始渲染
	*
	* 每次渲染之前都要先调用这个函数
	*
	*/
	void begin_render();

	/**
	* \brief
	* 结束渲染
	*
	* 每次渲染结束都要先调用这个函数
	*
	*/
	void end_render();

	/**
   * @brief
   *		渲染对象
   */
	virtual void render(CRenderSystem& renderer);

	virtual void RestruceOctree(){m_scene->RestructScene();};

	/**
	* @brief
	*		设置场景LOD
	* @param
	*		lod:要设置的LOD级别
	*/
	virtual void switch_to_lod(int lod, bool is_sync = true);

	void	set_IScene(H3DI::IScene* iScene){m_scene = iScene;};
	H3DI::IScene*	get_IScene(){return m_scene;};

	void	add_to_level(CMediaEndEntity* entity);
	void	remove_from_level(CMediaEndEntity* entity);
	void	add_static_object(Actor* static_objcet);
	void	remove_static_object(Actor* static_object);
protected:

	H3DColor			m_ambient_color; ///< 背景光颜色
	H3DColor			m_ambient_actor_color; ///< 人物背景光颜色
	float				m_shadow_scope;	///<阴影范围

	std::string			m_scene_name;
	CGraphicsResMng*	m_res_mng;
	H3DI::IScene*		m_scene;
	CCameraEntity*		m_render_camera;
	int			m_scene_lod;

	typedef std::list<CMediaEndEntity*>	Entity3DList;
	typedef std::list<CMediaEndEntity*>::iterator	Entity3DListItr;

	Entity3DList	m_entitylist;	//场景中所包含的所有实体列表，主要是用于LOD切换

	Entity3DList	m_static_object_entity_list;

	FORBIDDEN_EVIL_CONSTRUCTOR(CSceneEntity);
};
#endif 
