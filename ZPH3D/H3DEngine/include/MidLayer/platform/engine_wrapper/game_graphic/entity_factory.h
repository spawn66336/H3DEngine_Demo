/**
* @file entity_factory.h
* ����ļ�����Ҫ�����ǣ�
*				�����˿ɿ�ʵ��Ĵ������ͷŵĳ����೧
* @author �̳�(Chengchen@h3d.com.cn)
* 
* ������־
* ----------------------------------------------------
* ver 1.0.0
*       by �̳�2010.1.26
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
*		CEntityFactory�������������ṩ���������ٿɿ�ʵ��Ľӿڣ�
*	�򻯿ɿ�ʵ��Ĵ��������ٹ���������Ѵ����Ŀɿ�ʵ�建��������
*	�����ٵ�ʱ��֤�������ڴ�й¶��״��
* 
*/
class CEntityFactory
{
public:
	/**
	* @param
	*		res_mng����IN����Դ������
	*		resmng: ��Ʒ�˴����Ʒʹ�õ���Դ���������༭��ʹ��ʱ����ֵ����NULL
	*/
	CEntityFactory(CGraphicsResMng* res_mng);
	virtual ~CEntityFactory();

	/**
	* @brief
	* ���������
	* @param
	*	camera_name����IN�����������
	*/
	virtual CCameraEntity* create_camera(const std::string& camera_name);

	/**
	* @brief
	*		������̬ģ��
	* @param
	*		resource_name����IN����Դ�ļ�·��
	*		model_name����IN��ģ������
	*/
	virtual CModelEntity* create_model(const std::string& resource_name, const std::string& model_name
		, int lod, bool is_sync = true, int priority = 0);

	/*
	*@breif ��������ģ��
	*@param resource_name����IN����Դ�ļ�·��
	*@param model_name����IN��ģ������
	*@return �����Ķ���
	*/
	virtual CSkeletonModelEntity* create_skeleton_model_entity(const std::string& resource_name, const std::string& model_name
		, int lod, bool is_sync = true, int priority = 0);
	
	/**
	* @brief
	*		����avatar
	* @param 
	*		is_male����IN���Ա��Ƿ�Ϊ����
	*		avatar_name����IN��avatar����
	*/
	virtual AvatarEntity* create_avatar_entity(bool is_male, const std::string& avatar_name
		, int lod);

	/**
	* @brief
	*		����CAvatarSkeletonModelEntity;
	* @param 
	*		is_male����IN���Ա��Ƿ�Ϊ����
	*		name����IN��avatar����
	*/
	virtual CAvatarSkeletonModelEntity* create_avatar_skeleton_model_entity(bool is_male, const std::string& name
		, int lod);

	/**
	* @brief
	*		������Ч
	* @param 
	*		resource_name����IN����Ч��Դ�ļ�·��
	*		effect_name����IN����Ч����
	*/
	virtual CEffectEntity* create_effect(const std::string& resource_name, const std::string& effect_name
		, int lod, bool is_sync = true, int priority = 0);

	/**
	* @brief
	*		����level
	* @param 
	*		level_name����IN��level����
	*/
	virtual CLevelEntity* create_level_entity(H3DI::ILevel* ilevel = NULL);

	/**
	* @brief
	*		�������Դ
	* @param
	*		light_name����IN����Դ����
	*/
	virtual CPointPrePassLightEntity* create_point_prepass_light(const std::string& light_name);

	/**
	* @brief
	*		����׶��Դ
	* @param
	*		light_name����IN����Դ����
	*/
	virtual CWimblePrePassLightEntity* create_wimble_prepass_light(const std::string& light_name);

	/**
	* @brief
	*		���������Դ
	* @param
	*		light_name����IN����Դ����
	*/
	virtual CDirectionPrePassLightEntity* create_direction_prepass_light(const std::string& light_name);

	/**
	* @brief
	*		����ϵͳ�ɿ�ʵ��
	*/
	virtual CSystemEntity* create_system_entity(CSceneEntity* scene, const char* entity_name);

	/**
	* @brief
	*		����lightmapʵ��
	*/
	virtual CLightmapAdjustEntity* create_lightmap_adjust_entity(CSceneEntity* scene, const char* entity_name);

	/**
	* @brief
	*		�ͷ�ý��
	* @param 
	*		entity����IN��Ҫ�ͷŵĿɿ�ʵ��
	*/
	void release(CMediaEndEntity* entity);

	/**
	* @brief
	*		���ý�嵽�����೧,�ⲿ��Ӧ�õ���
	* @param
	*		entity����IN��ý��
	* @return
	*		��ӳɹ� ���� true
	*		���ʧ�� ���� false
	*/
	//bool add_entity(CMediaEndEntity* entity);

	/**
	* @brief
	*		�жϿɿ�ʵ�����ೡ���Ƿ����
	* @param
	*		entity����IN��Ҫ�����жϵĿɿ�ʵ��
	* @return
	*		true�����ڿɿ�ʵ��
	*		false�������ڿɿ�ʵ��
	*/
	//bool is_exist(CMediaEndEntity* entity);

	/**
	* @brief
	*		�ͷ�����ʵ�����������cache����cache��δ�����������cache����
	* @param
	*		engine_entity �ͷŵ�ʵ��
	*/
	virtual void release_engine_entity(H3DI::IMoveObject* engine_entity);

	/*
	*@breif ��������ʵ��
	*@param name ʵ�����
	*@param res_name ��Դ·��
	*@return 
	*/
	//virtual CSoundEntity* create_sound_entity(const char* name
	//	, const string& resname);
		
	/*
	* @brief
	*	ע����չentity��creator
	* @return 
	*	�ɹ�����true��ʧ�ܷ���false
	* @param
	*	type:ע���entity creator����������
	*	creator: entity creator
	*/
	bool register_creator(const char* type, CExtendEntityCreator* creator);

	/*
	* @brief
	*	ע����չentity��creator
	* @return 
	*	�ɹ�����true��ʧ�ܷ���false
	* @param
	*	type:��ע����entity creator����������
	*/
	CExtendEntityCreator* unregister_creator(const char* type);


	/*
	* @brief
	*	ע����չentity��creator
	* @return 
	*	�ɹ�����true��ʧ�ܷ���false
	* @param
	*	type:��ע����entity creator����������
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

	//��չ
	typedef std::map<std::string, CExtendEntityCreator*> EntityCreatorMap;
	typedef std::map<std::string, CExtendEntityCreator*>::iterator EntityCreatorMapItr;
	EntityCreatorMap m_extend_entitys;
	
	//��������
	typedef std::map<int, CExtendEntityCreator*> EntityCreatorTypeMap;
	typedef std::map<int, CExtendEntityCreator*>::iterator EntityCreatorTypeMapItr;
	EntityCreatorTypeMap m_type_extend_entitys;

private:
	CGraphicsResMng* m_res_mng; ///< ��Դ������
	CRenderSystem* m_render_system; ///< ��Ⱦϵͳ

	//EntityList m_create_entities; ///< �ɿ�ʵ���б�
	Levelmap	m_created_level_entities;	//�����Ĺؿ�ʵ���б�
	// fixme: ��Ҫע�Ͳ��ϲ���uiϵͳ�У����汾ǰ
	ISpecialEffect* get_effect(const char* path);

	static int s_Level_no;
};

CEntityFactory* get_global_entity_factory(H3DI::IRender* irender);
void register_pool(I_Entity_Pool* pool);
I_Entity_Pool* get_pool();

#endif //_ENTITY_FACTORY_H_
