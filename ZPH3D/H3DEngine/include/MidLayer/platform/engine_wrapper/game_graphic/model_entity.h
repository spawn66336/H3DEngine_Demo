/**
* @file model_entity.h
* 这个文件的主要功能是：声明引擎层的IModel类的包装类CModelEntity
* 
* @author 程陈(Chengchen@h3d.com.cn)
* 
* 更改日志
* ----------------------------------------------------
* ver 1.0.0
*       by 程陈2010.1.29
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
	std::string m_MapPath;	/// LIGHTMAP存放路径

	float m_CoordBias[2]; /// UV偏移信息
	float m_CoordScale[2];/// UV缩放信息

	float m_clrScale[4]; /// 颜色增强系数
};

/**
* @brief
*		CModelEntity类是对引擎层的IModel类的包装
*/
class CModelEntity
	:public C3DEntity
{
	friend class SetEffectOperation;
public:
	CModelEntity(CGraphicsResMng* res_mng, const std::string& name, const std::string& resource_name, int lod);

	/**
	* @brief
	*		设置模型假阴影的大小
	* @param
	*		size：【IN】模型假阴影的大小
	*/
	virtual void set_fake_shadow_size(float size);

	/**
	* @brief
	*		获取模型假阴影的大小
	* @return
	*		模型假阴影的大小
	*/
	virtual float get_fake_shadow_size() const;

	/**
	* @brief
	*		判断对象是否为要求的类型
	* @param
	*		type：【IN】类的类型
	* @return
	*		true：为指定的类型
	*		false：不为指定的类型
	*/
	virtual bool is_kind_of(int obj_type);

	/**
	* @brief
	*		获取类的类型
	* @return
	*		类的类型
	*/
	virtual int		type(void) const;


	/**
	* @brief
	*		获取对应的引擎层模型对象
	*/
	H3DI::IModel*	get_IModel() const;

	/**
	* @brief
	*		修改资源
	* @param
	*		res_name：【IN】要修改的资源路径
	* @return
	*		true：修改成功
	*		false：修改失败
	*/
	virtual bool change_resource(const std::string& res_name);

	/**
	* @brief
	*		修改资源
	* @param
	*		object：【IN】要修改的资源对象
	* @return
	*		true：修改成功
	*		false：修改失败
	*/
	virtual bool change_media_object(H3DI::IMoveObject* object, bool use_preproperty);

//////////////////////////////////////////////////////////////////////////
	///**
	//* @brief
	//*		获取模型的材质
	//* @param
	//*		i：【IN】材质的索引
	//* @return
	//*		材质引擎对象的索引
	//*/
	//virtual H3DI::INewShader* get_new_shader(unsigned int i);
	
	/**
	* @brief
	*		获取模型的材质
	* @param
	*		shader_name：【IN】材质的名称
	* @return
	*		材质引擎对象的指针
	*/
	virtual H3DI::INewShader* get_new_shader(const std::string& shader_name);

	/**
	* @brief
	*		设置材质的内容
	* @param
	*		value：【IN】要设置的值
	*/
	virtual bool set_shader_node_value(H3DI::INewShader* shader, const std::string& shader_node_name, int value);
	/**
	* @brief
	*		设置材质的内容
	* @param
	*		value：【IN】要设置的值
	*/
	virtual bool set_shader_node_value(H3DI::INewShader* shader, const std::string& shader_node_name, float value);
	/**
	* @brief
	*		设置材质的内容
	* @param
	*		value：【IN】要设置的值
	*/
	virtual bool set_shader_node_value(H3DI::INewShader* shader, const std::string& shader_node_name, float value1, float value2, float value3, float value4);
	/**
	* @brief
	*		设置材质的内容
	* @param
	*		value：【IN】要设置的值
	*/
	virtual bool set_shader_node_value(H3DI::INewShader* shader, const std::string& shader_node_name, const std::string& value);
	/**
	* @brief
	*		设置材质的内容
	* @param
	*		value：【IN】要设置的值
	*/
	virtual bool set_shader_node_value(H3DI::INewShader* shader, const std::string& shader_node_name, const H3DMat4& value);

	/**
	* @brief
	*		设置材质的内容
	* @param
	*		value：【IN】要设置的值
	*/
	virtual bool get_shader_node_value(H3DI::INewShader* shader, const std::string& shader_node_name, int& value);
	/**
	* @brief
	*		设置材质的内容
	* @param
	*		value：【IN】要设置的值
	*/
	virtual bool get_shader_node_value(H3DI::INewShader* shader, const std::string& shader_node_name, float& value);
	/**
	* @brief
	*		设置材质的内容
	* @param
	*		value：【IN】要设置的值
	*/
	virtual bool get_shader_node_value(H3DI::INewShader* shader, const std::string& shader_node_name, float& value1, float& value2, float& value3, float& value4);
	/**
	* @brief
	*		设置材质的内容
	* @param
	*		value：【IN】要设置的值
	*/
	virtual bool get_shader_node_value(H3DI::INewShader* shader, const std::string& shader_node_name, std::string& value);
	/**
	* @brief
	*		设置材质的内容
	* @param
	*		value：【IN】要设置的值
	*/
	virtual bool get_shader_node_value(H3DI::INewShader* shader, const std::string& shader_node_name, H3DMat4& value);

	/**
	* @brief
	*		设置霓虹灯的振荡器参数
	* @param
	*		shader：【IN】要设置的材质对象指针
	*		var_type：【IN】要设置的材质变量类型
	*		value：【IN】要设置的材质变量属性值
	*/
	virtual void set_waveform_var(H3DI::INewShader* shader, H3DI::WaveFormParam var_type, float value);

	/**
	* @brief
	*		设置霓虹灯的振荡器参数，是
	*void set_waveform_var(H3DI::INewShader* shader, const char* var_name, float value)
	*的扩展，在这里value = f0 * value + f1
	* @param
	*		shader：【IN】要设置的材质对象指针
	*		var_type：【IN】要设置的材质变量类型
	*		f0：【IN】要设置的材质变量属性值
	*		f1：【IN】要设置的材质变量属性值
	*/
	virtual void set_waveform_var(H3DI::INewShader* shader, H3DI::WaveFormParam var_type, float f0, float f1);
	
	/**
	* @brief
	*		将模型设为高亮显示
	* @param
	*		high_light：【IN】true：模型高亮，false模型不高亮
	*/
	virtual void high_light_model(bool high_light);

	/**
	* @brief
	*		模型是否被高亮
	* @return
	*		true：模型高亮
	*		false：模型不高亮
	*/
	virtual bool is_model_high_light();

	/**
	* @brief
	*		渲染对象
	* @param
	*		renderSystem：【IN】绘制系统
	*/
	virtual void render(CRenderSystem& render_system);

	/**
	* @brief
	*		重新加载模型
	*/
	void reload();

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
	*		将一个材质设置给某个mesh
	* @return
	*		0 ： 失败
	*		其他 ： 成功
	*/
	virtual int set_mesh_shader_by_name(const char* mesh_name,H3DI::INewShader* p);

	/**
	* @brief
	*		将一个材质设置给某个mesh
	* @return
	*		0 ： 失败
	*		其他 ： 成功
	*/
	virtual int set_mesh_shader_by_idx(int mesh_idx, H3DI::INewShader* p);

	/**
	* @brief
	*		设置特效
	* @param
	*		entity：挂在模型上的特效。若entity已经挂在一个模型A上，在重新挂在模型B上之前需要调用unset_effect
	*/
	void set_effect(CEffectEntity* entity);

	/**
	* @brief
	*		取消特效
	*/
	void unset_effect(CEffectEntity* entity);

	/**
	* @brief
	*		获得资源描述
	*/
	void getModelAppearance(ModelAppearance& appearance);

	/*
	*@breif 开关影子
	*@param show 是否开关
	*/
	void setShadow(bool show);

	/*
	*@breif 设置是否在反射中渲染
	*/
	void set_renderin_reflection(bool is);

	/*
	*@breif 设置是否渲染时不写颜色
	*/
	void set_not_write_color(bool is);

	/*
	*@breif 设置是否使用角色光
	*/
	void set_ralativetoactor(bool is);

	/*
	*@breif 设置透明度
	*/
	void set_transparence(float trans);

	/*
	*@breif 本帧强制同步
	*/
	void set_forcesyncdata();

	/*
	*@breif 隐藏时是否更新（默认为不更新）
	*/
	void set_hide_update(bool is);

	//功能：计算一条射线与模型上的包围盒相碰撞的信息
	//参数
	//rayOrigin：传入三个浮点数组成的数组，表示一条射线的起始点
	//rayDir：传入三个浮点数组成的数组，表示一条射线的方向
	//ModelBVTestInfo ：一个数组首地址，用于结果的返回。所有碰撞到的包围盒按先后顺序将ID和碰撞点坐标返回给客户端。数组的大小不能小于一个模型的包围盒个数，包围盒个数由引擎再提供一个接中得到
	//nResultBufferCount：数组的元素个数，防止引擎填充数据越界
	//返回值：返回的结果的个数,如果为，说明没有碰撞
	int rayTestWithBoundingVolumes(const float* rayOrigin, const float * rayDir, H3DI::ModelBoundingVolumeTestInfo *pOut, int nResultBufferCount);

	//!挂饰相关接口
	//根据射线得到挂接数据集
	//第二个参数表示是否需要转换射线空间
	int	  queryAttachPoint(H3DI::IAttachModelInfo& rayResInfo,bool bTransToLocal=true);

	/**
	* @brief
	*		设置level层（在add_to_level之前设置才能生效）
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
	H3DI::IModel*	m_model; ///< 引擎层对应的mode对象

	std::set<CEffectEntity*> m_effect_set;

	bool			m_is_model_high_light;
	bool			m_is_lightmap_load;
	bool			m_is_lightmap_applied;
	bool			m_is_recive_fakelightspot;		//记录引擎物体是否被设置了光斑属性
	bool			m_recive_fakelightspot_record;	//记录配置文件引擎物体是否接受光斑属性

	typedef std::map<unsigned int, ModelLightMap> LightMapInfos;
	typedef LightMapInfos::iterator LightMapInfosItr;
	LightMapInfos	m_lightmapinfos;

	typedef std::map<unsigned int, H3DI::ILightMapInfo> LightMaps;
	typedef LightMaps::iterator LightMapsItr;
	LightMaps m_lightmaps;

	//Cache的属性
	float		m_fake_shadow_size;
	bool		m_is_cast_shadow; //记录是否产生影子
	bool		m_is_renderin_reflection; //记录是否在反射中渲染
	bool		m_is_not_write_color; //记录是否渲染时不写颜色
	bool		m_is_ralativetoactor; //记录是否使用角色光
	bool		m_is_hide_update; //记录是否隐藏时更新

	bool		m_set_transparence;
	float		m_transparence; //透明度

private:
	FORBIDDEN_EVIL_CONSTRUCTOR(CModelEntity);
};

#endif //_MODEL_ENTITY_H_
