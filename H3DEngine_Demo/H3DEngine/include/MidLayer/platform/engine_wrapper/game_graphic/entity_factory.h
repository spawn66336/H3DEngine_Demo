/**
* @file entity_factory.h
* 这个文件的主要功能是：
*				定义了可控实体的创建和释放的抽象类厂
* @author 程陈(Chengchen@h3d.com.cn)
* 
* 更改日志
* ----------------------------------------------------
* ver 1.0.0
*       by 程陈2010.1.26
*/

#ifndef _ENTITY_FACTORY_H_
#define _ENTITY_FACTORY_H_

#include "game_graphic_header.h"

class CModelEntity;
class CCameraEntity;
class CMediaEndEntity;
class AvatarEntity;
class CLevelEntity;
class I_Entity_Pool;
class CPointPrePassLightEntity;
class CWimblePrePassLightEntity;
class CDirectionPrePassLightEntity;
class CSkeletonModelEntity;
class CAvatarSkeletonModelEntity;
//class CSoundEntity;

/**
* @brief
*		CEntityFactory的作用是用来提供创建和销毁可控实体的接口，
*	简化可控实体的创建和销毁工作。另外把创建的可控实体缓存下来，
*	在销毁的时候保证不出现内存泄露的状况
* 
*/
class CEntityFactory
{
public:
	/**
	* @param
	*		res_mng：【IN】资源管理器
	*		resmng: 产品端传入产品使用的资源管理器，编辑器使用时，该值传入NULL
	*/
	CEntityFactory(CGraphicsResMng* res_mng);
	virtual ~CEntityFactory();

	/**
	* @brief
	* 创建摄像机
	* @param
	*	camera_name：【IN】摄像机名称
	*/
	virtual CCameraEntity* create_camera(const std::string& camera_name);

	/**
	* @brief
	*		创建静态模型
	* @param
	*		resource_name：【IN】资源文件路径
	*		model_name：【IN】模型名称
	*/
	virtual CModelEntity* create_model(const std::string& resource_name, const std::string& model_name
		, int lod, bool is_sync = true, int priority = 0);

	/*
	*@breif 创建骨骼模型
	*@param resource_name：【IN】资源文件路径
	*@param model_name：【IN】模型名称
	*@return 创建的对象
	*/
	virtual CSkeletonModelEntity* create_skeleton_model_entity(const std::string& resource_name, const std::string& model_name
		, int lod, bool is_sync = true, int priority = 0);
	
	/**
	* @brief
	*		创建avatar
	* @param 
	*		is_male：【IN】性别是否为男性
	*		avatar_name：【IN】avatar名称
	*/
	virtual AvatarEntity* create_avatar_entity(bool is_male, const std::string& avatar_name
		, int lod);

	/**
	* @brief
	*		创建CAvatarSkeletonModelEntity;
	* @param 
	*		is_male：【IN】性别是否为男性
	*		name：【IN】avatar名称
	*/
	virtual CAvatarSkeletonModelEntity* create_avatar_skeleton_model_entity(bool is_male, const std::string& name
		, int lod);

	/**
	* @brief
	*		创建特效
	* @param 
	*		resource_name：【IN】特效资源文件路径
	*		effect_name：【IN】特效名称
	*/
	virtual CEffectEntity* create_effect(const std::string& resource_name, const std::string& effect_name
		, int lod, bool is_sync = true, int priority = 0);

	/**
	* @brief
	*		创建level
	* @param 
	*		level_name：【IN】level名称
	*/
	virtual CLevelEntity* create_level_entity(H3DI::ILevel* ilevel = NULL);

	/**
	* @brief
	*		创建点光源
	* @param
	*		light_name：【IN】光源名称
	*/
	virtual CPointPrePassLightEntity* create_point_prepass_light(const std::string& light_name);

	/**
	* @brief
	*		创建锥光源
	* @param
	*		light_name：【IN】光源名称
	*/
	virtual CWimblePrePassLightEntity* create_wimble_prepass_light(const std::string& light_name);

	/**
	* @brief
	*		创建方向光源
	* @param
	*		light_name：【IN】光源名称
	*/
	virtual CDirectionPrePassLightEntity* create_direction_prepass_light(const std::string& light_name);

	/**
	* @brief
	*		创建系统可控实体
	*/
	virtual CSystemEntity* create_system_entity(CSceneEntity* scene, const char* entity_name);

	/**
	* @brief
	*		创建lightmap实体
	*/
	virtual CLightmapAdjustEntity* create_lightmap_adjust_entity(CSceneEntity* scene, const char* entity_name);

	/**
	* @brief
	*		释放媒体
	* @param 
	*		entity：【IN】要释放的可控实体
	*/
	void release(CMediaEndEntity* entity);

	/**
	* @brief
	*		添加媒体到抽象类厂,外部不应该调用
	* @param
	*		entity：【IN】媒体
	* @return
	*		添加成功 返回 true
	*		添加失败 返回 false
	*/
	//bool add_entity(CMediaEndEntity* entity);

	/**
	* @brief
	*		判断可控实体在类场内是否存在
	* @param
	*		entity：【IN】要进行判断的可控实体
	* @return
	*		true：存在可控实体
	*		false：不存在可控实体
	*/
	//bool is_exist(CMediaEndEntity* entity);

	/**
	* @brief
	*		释放引擎实例，如果存在cache池且cache池未满，则将其放入cache池中
	* @param
	*		engine_entity 释放的实例
	*/
	virtual void release_engine_entity(H3DI::IMoveObject* engine_entity);

	/*
	*@breif 创建声音实体
	*@param name 实体别名
	*@param res_name 资源路径
	*@return 
	*/
	//virtual CSoundEntity* create_sound_entity(const char* name
	//	, const string& resname);
		
	/*
	* @brief
	*	注册扩展entity的creator
	* @return 
	*	成功返回true，失败返回false
	* @param
	*	type:注册的entity creator的类型名称
	*	creator: entity creator
	*/
	bool register_creator(const char* type, CExtendEntityCreator* creator);

	/*
	* @brief
	*	注销扩展entity的creator
	* @return 
	*	成功返回true，失败返回false
	* @param
	*	type:待注销的entity creator的类型名称
	*/
	CExtendEntityCreator* unregister_creator(const char* type);


	/*
	* @brief
	*	注销扩展entity的creator
	* @return 
	*	成功返回true，失败返回false
	* @param
	*	type:待注销的entity creator的类型名称
	*/
	CMediaEndEntity* create_extend_entity(const char* type, const char* id);

	virtual void release_level_entity(CLevelEntity* level);
	enum EngineObjectCreateFlag
	{
		EOCF_NULL = -1,
		EOCF_CREATED_SELF_MIDDLE_LAYER = 0x0,
		EOCF_CREATED_SELF_CLIENT,
		EOCF_PASSED_IN,
	};

private:
	typedef std::vector<CMediaEndEntity*> EntityList;
	typedef std::map<CLevelEntity*, EngineObjectCreateFlag>	Levelmap;

	typedef std::map<std::string, H3DI::ITexture*> TextureMap;
	typedef TextureMap::iterator TextureMapitr;
	TextureMap m_texture_map;

	//扩展
	typedef std::map<std::string, CExtendEntityCreator*> EntityCreatorMap;
	typedef std::map<std::string, CExtendEntityCreator*>::iterator EntityCreatorMapItr;
	EntityCreatorMap m_extend_entitys;
	
	//用于销毁
	typedef std::map<int, CExtendEntityCreator*> EntityCreatorTypeMap;
	typedef std::map<int, CExtendEntityCreator*>::iterator EntityCreatorTypeMapItr;
	EntityCreatorTypeMap m_type_extend_entitys;

private:
	CGraphicsResMng* m_res_mng; ///< 资源管理器
	CRenderSystem* m_render_system; ///< 渲染系统

	//EntityList m_create_entities; ///< 可控实体列表
	Levelmap	m_created_level_entities;	//创建的关卡实体列表
	// fixme: 需要注释并合并到ui系统中，封测版本前
	ISpecialEffect* get_effect(const char* path);

	static int s_Level_no;
};

CEntityFactory* get_global_entity_factory(H3DI::IRender* irender);
void register_pool(I_Entity_Pool* pool);
I_Entity_Pool* get_pool();

#endif //_ENTITY_FACTORY_H_
