/**
* @file level_entity.h
* ����ļ�����Ҫ�����ǣ�����������ILevel��İ�װ��CLevelEntity
* 
* @author ����(Wangheng@h3d.com.cn)
* 
* ������־
* ----------------------------------------------------
* ver 1.0.0
*       by ����2012.2.28
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
	*		���ñ�����
	*/
	virtual void set_ambient_color(const H3DColor& ambient_color);

	/**
	* @brief
	*		�������ﱳ����
	*/
	virtual void set_ambient_actor_color(const H3DColor& ambient_actor_color);


	/**
	* @brief
	*		��ȡ������
	*/
	virtual const H3DColor& get_ambient_color() const;

	/**
	* @brief
	*		��ȡ���ﱳ����
	*/
	const H3DColor& get_ambient_actor_color() const;

	/**
	* @brief
	*		������Ӱ��Χ
	*/
	virtual void  set_shadow_scope(const float& shadow_scope);

	 /**
   * \brief
   * �����ӽ������
   * 
   * \param camera
   * �����
   *
   * \returns
   * �ϵ������
   */
   virtual CCameraEntity* set_render_camera(CCameraEntity* camera);

   /**
   * \brief
   * �����ӽ������
   * 
   * \param camera
   * �����
   *
   * \returns
   * �ϵ������
   */
   //virtual CCameraEntity* set_render_camera(const std::string& camera_name);   
   /**
   * \brief
   * ����ӽ������
   *
   * \returns
   * ���������
   */
   virtual CCameraEntity* get_render_camera() const;

	virtual void add_postprocess(const char* filename);
	virtual void del_postprocess(const char* filename);
	virtual void set_postprocess_param(const char *name,void *pvalue,int sizeofvalue);

	/**
	* \brief
	* ��ʼ��Ⱦ
	*
	* ÿ����Ⱦ֮ǰ��Ҫ�ȵ����������
	*
	*/
	void begin_render();

	/**
	* \brief
	* ������Ⱦ
	*
	* ÿ����Ⱦ������Ҫ�ȵ����������
	*
	*/
	void end_render();

	/**
   * @brief
   *		��Ⱦ����
   */
	virtual void render(CRenderSystem& renderer);

	virtual void RestruceOctree(){m_scene->RestructScene();};

	/**
	* @brief
	*		���ó���LOD
	* @param
	*		lod:Ҫ���õ�LOD����
	*/
	virtual void switch_to_lod(int lod, bool is_sync = true);

	void	set_IScene(H3DI::IScene* iScene){m_scene = iScene;};
	H3DI::IScene*	get_IScene(){return m_scene;};

	void	add_to_level(CMediaEndEntity* entity);
	void	remove_from_level(CMediaEndEntity* entity);
	void	add_static_object(Actor* static_objcet);
	void	remove_static_object(Actor* static_object);
protected:

	H3DColor			m_ambient_color; ///< ��������ɫ
	H3DColor			m_ambient_actor_color; ///< ���ﱳ������ɫ
	float				m_shadow_scope;	///<��Ӱ��Χ

	std::string			m_scene_name;
	CGraphicsResMng*	m_res_mng;
	H3DI::IScene*		m_scene;
	CCameraEntity*		m_render_camera;
	int			m_scene_lod;

	typedef std::list<CMediaEndEntity*>	Entity3DList;
	typedef std::list<CMediaEndEntity*>::iterator	Entity3DListItr;

	Entity3DList	m_entitylist;	//������������������ʵ���б���Ҫ������LOD�л�

	Entity3DList	m_static_object_entity_list;

	FORBIDDEN_EVIL_CONSTRUCTOR(CSceneEntity);
};
#endif 
