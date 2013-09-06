/**
* @file model_entity.h
* ����ļ�����Ҫ�����ǣ�����������IModel��İ�װ��CModelEntity
* 
* @author �̳�(Chengchen@h3d.com.cn)
* 
* ������־
* ----------------------------------------------------
* ver 1.0.0
*       by �̳�2010.1.29
*/

#ifndef _MODEL_ENTITY_H_
#define _MODEL_ENTITY_H_

#include "game_graphic_header.h"
#include "3DEntity.h"
#include "entity_appearance.h"

class CEffectEntity;
class CGraphicsResMng;
class CRenderSystem;
class CLevelEntity;

enum Detail_Actor_Type
{
	DAT_SMALL_ACTOR = 0x0,
	DAT_HUGE_ACTOR,
	DAT_DETAIL_ACTOR,
};

struct ModelLightMap
{
	std::string m_MapPath;	/// LIGHTMAP���·��

	float m_CoordBias[2]; /// UVƫ����Ϣ
	float m_CoordScale[2];/// UV������Ϣ

	float m_clrScale[4]; /// ��ɫ��ǿϵ��
};

/**
* @brief
*		CModelEntity���Ƕ�������IModel��İ�װ
*/
class CModelEntity
	:public C3DEntity
{
	friend class SetEffectOperation;
public:
	CModelEntity(CGraphicsResMng* res_mng, const std::string& name, const std::string& resource_name, int lod);

	/**
	* @brief
	*		����ģ�ͼ���Ӱ�Ĵ�С
	* @param
	*		size����IN��ģ�ͼ���Ӱ�Ĵ�С
	*/
	virtual void set_fake_shadow_size(float size);

	/**
	* @brief
	*		��ȡģ�ͼ���Ӱ�Ĵ�С
	* @return
	*		ģ�ͼ���Ӱ�Ĵ�С
	*/
	virtual float get_fake_shadow_size() const;

	/**
	* @brief
	*		�ж϶����Ƿ�ΪҪ�������
	* @param
	*		type����IN���������
	* @return
	*		true��Ϊָ��������
	*		false����Ϊָ��������
	*/
	virtual bool is_kind_of(int obj_type);

	/**
	* @brief
	*		��ȡ�������
	* @return
	*		�������
	*/
	virtual int		type(void) const;


	/**
	* @brief
	*		��ȡ��Ӧ�������ģ�Ͷ���
	*/
	H3DI::IModel*	get_IModel() const;

	/**
	* @brief
	*		�޸���Դ
	* @param
	*		res_name����IN��Ҫ�޸ĵ���Դ·��
	* @return
	*		true���޸ĳɹ�
	*		false���޸�ʧ��
	*/
	virtual bool change_resource(const std::string& res_name);

	/**
	* @brief
	*		�޸���Դ
	* @param
	*		object����IN��Ҫ�޸ĵ���Դ����
	* @return
	*		true���޸ĳɹ�
	*		false���޸�ʧ��
	*/
	virtual bool change_media_object(H3DI::IMoveObject* object, bool use_preproperty);

//////////////////////////////////////////////////////////////////////////
	///**
	//* @brief
	//*		��ȡģ�͵Ĳ���
	//* @param
	//*		i����IN�����ʵ�����
	//* @return
	//*		����������������
	//*/
	//virtual H3DI::INewShader* get_new_shader(unsigned int i);
	
	/**
	* @brief
	*		��ȡģ�͵Ĳ���
	* @param
	*		shader_name����IN�����ʵ�����
	* @return
	*		������������ָ��
	*/
	virtual H3DI::INewShader* get_new_shader(const std::string& shader_name);

	/**
	* @brief
	*		���ò��ʵ�����
	* @param
	*		value����IN��Ҫ���õ�ֵ
	*/
	virtual bool set_shader_node_value(H3DI::INewShader* shader, const std::string& shader_node_name, int value);
	/**
	* @brief
	*		���ò��ʵ�����
	* @param
	*		value����IN��Ҫ���õ�ֵ
	*/
	virtual bool set_shader_node_value(H3DI::INewShader* shader, const std::string& shader_node_name, float value);
	/**
	* @brief
	*		���ò��ʵ�����
	* @param
	*		value����IN��Ҫ���õ�ֵ
	*/
	virtual bool set_shader_node_value(H3DI::INewShader* shader, const std::string& shader_node_name, float value1, float value2, float value3, float value4);
	/**
	* @brief
	*		���ò��ʵ�����
	* @param
	*		value����IN��Ҫ���õ�ֵ
	*/
	virtual bool set_shader_node_value(H3DI::INewShader* shader, const std::string& shader_node_name, const std::string& value);
	/**
	* @brief
	*		���ò��ʵ�����
	* @param
	*		value����IN��Ҫ���õ�ֵ
	*/
	virtual bool set_shader_node_value(H3DI::INewShader* shader, const std::string& shader_node_name, const H3DMat4& value);

	/**
	* @brief
	*		���ò��ʵ�����
	* @param
	*		value����IN��Ҫ���õ�ֵ
	*/
	virtual bool get_shader_node_value(H3DI::INewShader* shader, const std::string& shader_node_name, int& value);
	/**
	* @brief
	*		���ò��ʵ�����
	* @param
	*		value����IN��Ҫ���õ�ֵ
	*/
	virtual bool get_shader_node_value(H3DI::INewShader* shader, const std::string& shader_node_name, float& value);
	/**
	* @brief
	*		���ò��ʵ�����
	* @param
	*		value����IN��Ҫ���õ�ֵ
	*/
	virtual bool get_shader_node_value(H3DI::INewShader* shader, const std::string& shader_node_name, float& value1, float& value2, float& value3, float& value4);
	/**
	* @brief
	*		���ò��ʵ�����
	* @param
	*		value����IN��Ҫ���õ�ֵ
	*/
	virtual bool get_shader_node_value(H3DI::INewShader* shader, const std::string& shader_node_name, std::string& value);
	/**
	* @brief
	*		���ò��ʵ�����
	* @param
	*		value����IN��Ҫ���õ�ֵ
	*/
	virtual bool get_shader_node_value(H3DI::INewShader* shader, const std::string& shader_node_name, H3DMat4& value);

	/**
	* @brief
	*		�����޺�Ƶ���������
	* @param
	*		shader����IN��Ҫ���õĲ��ʶ���ָ��
	*		var_type����IN��Ҫ���õĲ��ʱ�������
	*		value����IN��Ҫ���õĲ��ʱ�������ֵ
	*/
	virtual void set_waveform_var(H3DI::INewShader* shader, H3DI::WaveFormParam var_type, float value);

	/**
	* @brief
	*		�����޺�Ƶ�������������
	*void set_waveform_var(H3DI::INewShader* shader, const char* var_name, float value)
	*����չ��������value = f0 * value + f1
	* @param
	*		shader����IN��Ҫ���õĲ��ʶ���ָ��
	*		var_type����IN��Ҫ���õĲ��ʱ�������
	*		f0����IN��Ҫ���õĲ��ʱ�������ֵ
	*		f1����IN��Ҫ���õĲ��ʱ�������ֵ
	*/
	virtual void set_waveform_var(H3DI::INewShader* shader, H3DI::WaveFormParam var_type, float f0, float f1);
	
	/**
	* @brief
	*		��ģ����Ϊ������ʾ
	* @param
	*		high_light����IN��true��ģ�͸�����falseģ�Ͳ�����
	*/
	virtual void high_light_model(bool high_light);

	/**
	* @brief
	*		ģ���Ƿ񱻸���
	* @return
	*		true��ģ�͸���
	*		false��ģ�Ͳ�����
	*/
	virtual bool is_model_high_light();

	/**
	* @brief
	*		��Ⱦ����
	* @param
	*		renderSystem����IN������ϵͳ
	*/
	virtual void render(CRenderSystem& render_system);

	/**
	* @brief
	*		���¼���ģ��
	*/
	void reload();

	/**
	* @brief
	*		����Ӳ��ѡ����Ⱦ
	* @param
	*		renderSystem����IN������ϵͳ
	*		sel_line: �Ƿ�������Ⱦ
	* @return
	*		true��������Ⱦ
	*		false����������Ⱦ
	*/
	virtual bool pushHardwareSelect(CRenderSystem& render_system, bool sel_line);

	virtual void add_lightmapinfo(unsigned int nMeshID, ModelLightMap& lightmapinfo);
	virtual void switch_to_lod(int lod, bool is_sync = true);
	virtual void load_lightmap();
	virtual void apply_lightmap();
	virtual void cancle_lightmap();
	virtual void set_recive_fakelightspot(bool brecive);
	virtual bool get_recive_fakelightspot(){return m_is_recive_fakelightspot;};
	virtual void record_recive_fakelightspot(bool brecive);
	virtual bool get_recive_fakelightspot_record()const{return m_recive_fakelightspot_record;};
	/**
	* @brief
	*		��һ���������ø�ĳ��mesh
	* @return
	*		0 �� ʧ��
	*		���� �� �ɹ�
	*/
	virtual int set_mesh_shader_by_name(const char* mesh_name,H3DI::INewShader* p);

	/**
	* @brief
	*		��һ���������ø�ĳ��mesh
	* @return
	*		0 �� ʧ��
	*		���� �� �ɹ�
	*/
	virtual int set_mesh_shader_by_idx(int mesh_idx, H3DI::INewShader* p);

	/**
	* @brief
	*		������Ч
	* @param
	*		entity������ģ���ϵ���Ч����entity�Ѿ�����һ��ģ��A�ϣ������¹���ģ��B��֮ǰ��Ҫ����unset_effect
	*/
	void set_effect(CEffectEntity* entity);

	/**
	* @brief
	*		ȡ����Ч
	*/
	void unset_effect(CEffectEntity* entity);

	/**
	* @brief
	*		�����Դ����
	*/
	void getModelAppearance(ModelAppearance& appearance);

	/*
	*@breif ����Ӱ��
	*@param show �Ƿ񿪹�
	*/
	void setShadow(bool show);

	/*
	*@breif �����Ƿ��ڷ�������Ⱦ
	*/
	void set_renderin_reflection(bool is);

	/*
	*@breif �����Ƿ���Ⱦʱ��д��ɫ
	*/
	void set_not_write_color(bool is);

	/*
	*@breif �����Ƿ�ʹ�ý�ɫ��
	*/
	void set_ralativetoactor(bool is);

	/*
	*@breif ����͸����
	*/
	void set_transparence(float trans);

	/*
	*@breif ��֡ǿ��ͬ��
	*/
	void set_forcesyncdata();

	/*
	*@breif ����ʱ�Ƿ���£�Ĭ��Ϊ�����£�
	*/
	void set_hide_update(bool is);

	//���ܣ�����һ��������ģ���ϵİ�Χ������ײ����Ϣ
	//����
	//rayOrigin������������������ɵ����飬��ʾһ�����ߵ���ʼ��
	//rayDir������������������ɵ����飬��ʾһ�����ߵķ���
	//ModelBVTestInfo ��һ�������׵�ַ�����ڽ���ķ��ء�������ײ���İ�Χ�а��Ⱥ�˳��ID����ײ�����귵�ظ��ͻ��ˡ�����Ĵ�С����С��һ��ģ�͵İ�Χ�и�������Χ�и������������ṩһ�����еõ�
	//nResultBufferCount�������Ԫ�ظ�������ֹ�����������Խ��
	//����ֵ�����صĽ���ĸ���,���Ϊ��˵��û����ײ
	int rayTestWithBoundingVolumes(const float* rayOrigin, const float * rayDir, H3DI::ModelBoundingVolumeTestInfo *pOut, int nResultBufferCount);

	//!������ؽӿ�
	//�������ߵõ��ҽ����ݼ�
	//�ڶ���������ʾ�Ƿ���Ҫת�����߿ռ�
	int	  queryAttachPoint(H3DI::IAttachModelInfo& rayResInfo,bool bTransToLocal=true);

	/**
	* @brief
	*		����level�㣨��add_to_level֮ǰ���ò�����Ч��
	*/
	void set_level_layer(int level_layer);

	virtual void UnloadResources();
	virtual void ReloadResources(bool is_sync = true, int priority = 0);

protected:
	virtual ~CModelEntity();

	virtual void create_engine_object_impl(bool is_sync);
	virtual void set_engine_object_impl(H3DI::IMoveObject* object);
	virtual void after_create_impl();

	void set_effect_immediate(CEffectEntity* entity);
	void switch_to_lod_immediate(int lod, bool is_sync);

	void UnloadEffect();
	void ReloadEffect(bool is_sync, int priority);

	virtual void update_flags();
	
protected:
	H3DI::IModel*	m_model; ///< ������Ӧ��mode����

	std::set<CEffectEntity*> m_effect_set;

	bool			m_is_model_high_light;
	bool			m_is_lightmap_load;
	bool			m_is_lightmap_applied;
	bool			m_is_recive_fakelightspot;		//��¼���������Ƿ������˹������
	bool			m_recive_fakelightspot_record;	//��¼�����ļ����������Ƿ���ܹ������

	typedef std::map<unsigned int, ModelLightMap> LightMapInfos;
	typedef LightMapInfos::iterator LightMapInfosItr;
	LightMapInfos	m_lightmapinfos;

	typedef std::map<unsigned int, H3DI::ILightMapInfo> LightMaps;
	typedef LightMaps::iterator LightMapsItr;
	LightMaps m_lightmaps;

	//Cache������
	float		m_fake_shadow_size;
	bool		m_is_cast_shadow; //��¼�Ƿ����Ӱ��
	bool		m_is_renderin_reflection; //��¼�Ƿ��ڷ�������Ⱦ
	bool		m_is_not_write_color; //��¼�Ƿ���Ⱦʱ��д��ɫ
	bool		m_is_ralativetoactor; //��¼�Ƿ�ʹ�ý�ɫ��
	bool		m_is_hide_update; //��¼�Ƿ�����ʱ����

	bool		m_set_transparence;
	float		m_transparence; //͸����

private:
	FORBIDDEN_EVIL_CONSTRUCTOR(CModelEntity);
};

#endif //_MODEL_ENTITY_H_
