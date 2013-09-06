/**
* @file prepass_light_entity.h
* 这个文件的主要功能是：
* 
* @author 程陈(Chengchen@h3d.com.cn)
* 
* 更改日志
* ----------------------------------------------------
* ver 1.0.0
*       by 程陈2010.5.21
*/

#ifndef _PREPASS_LIGHT_ENTITY_H_
#define _PREPASS_LIGHT_ENTITY_H_

#include "game_graphic_header.h"

//光斑参数结构
struct GlareProperty
{
	float width; 	//尺寸
	float height;
	H3DColor color; 	//颜色
	std::string texture_name;	//贴图
	std::string shader_lib; 	//材质lib
	std::string shader_instance_name;	//材9质实例名
	float glare_distance;	//到光源的距离
	float glare_rotation_velocity; 	//旋转速度
	bool is_visible; 	//是否可见
	bool is_rotate_with_camera; 	//是否跟随摄像机旋转
	bool is_view_camera;	//是否面向摄像机

	GlareProperty()
	{
		width = height = 1.0f;
		color = H3DColor(1.0f, 1.0f, 1.0f, 1.0f);
		glare_distance = 0.0f;
		glare_rotation_velocity = 5.0f;
		is_visible = true;
		is_rotate_with_camera = true;
		is_view_camera = true;
	}


	GlareProperty(const H3DI::tGlareProperty& glare_property)
	{
		width = glare_property.m_width;
		height = glare_property.m_height;
		color = H3DColor(glare_property.m_color);
		texture_name = (NULL != glare_property.m_pTextureName) ? glare_property.m_pTextureName : "";
		shader_lib = (NULL != glare_property.m_pShaderLib) ? glare_property.m_pShaderLib : "";
		shader_instance_name = (NULL != glare_property.m_pShaderInstanceName) ? glare_property.m_pShaderInstanceName : "";
		glare_distance = glare_property.m_disToLight;
		glare_rotation_velocity = glare_property.m_glareRotVel;
		is_visible = glare_property.m_bVisable;
		is_rotate_with_camera = glare_property.m_bRotateWithCamera;
		is_view_camera = glare_property.m_bViewCamera;
	}

	operator H3DI::tGlareProperty()
	{
		H3DI::tGlareProperty glare_property;
		glare_property.m_width = width;
		glare_property.m_height = height;
		for (int i = 0; i < 4; i++)
		{
			glare_property.m_color[i] = color.getColor()[i];
		}
		glare_property.m_pTextureName= (texture_name.length() != 0) ? texture_name.c_str() : NULL;
		glare_property.m_pShaderLib= (shader_lib.length() != 0) ? shader_lib.c_str() : NULL;
		glare_property.m_pShaderInstanceName= (shader_instance_name.length() != 0) ? shader_instance_name.c_str() : NULL;
		glare_property.m_disToLight = glare_distance;
		glare_property.m_glareRotVel = glare_rotation_velocity;
		glare_property.m_bVisable = is_visible;
		glare_property.m_bRotateWithCamera = is_rotate_with_camera;
		glare_property.m_bViewCamera = is_view_camera;

		return glare_property;
	}
};


class CPrePassLightEntity
	: public C3DEntity
{
public:
	CPrePassLightEntity(CGraphicsResMng* res_mng, const std::string& name, H3DI::IPrePassLight* light);
	virtual ~CPrePassLightEntity();

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
	*		获取灯光颜色
	* @return
	*		灯光颜色
	*/
	H3DColor color() const;

	/**
	* @brief
	*		设置灯光颜色
	* @param
	*		light_color：【IN】要设置的灯光颜色
	*/
	void set_color(const H3DColor& light_color);

	/**
	* @brief
	*		获取灯光强度
	* @return
	*		灯光强度
	*/
	float intensity() const;
	
	/**
	* @brief
	*		设置灯光强度
	* @param
	*		light_intensity：【IN】要设置的灯光强度
	*/
	virtual void set_intensity(float light_intensity);

	/**
	* @brief
	*		是否允许灯光阴影
	* @return
	*		是否允许灯光阴影
	*/
	bool is_shadow_enable() const;

	/**
	* @brief
	*		设置是否允许灯光阴影
	* @param
	*		enable_shadow：【IN】是否允许灯光阴影
	*/
	void set_shadow_enable(bool enable_shadow);

	/**
	* @brief
	*		获取灯光阴影颜色
	* @return
	*		灯光阴影颜色
	*/
	H3DColor shadow_color() const;
	
	/**
	* @brief
	*		设置灯光阴影颜色
	* @param
	*		light_shadow_color：【IN】要设置的灯光阴影颜色
	*/
	void set_shadow_color(const H3DColor& light_shadow_color);

	/**
	* @brief
	*		获取引擎封装的灯光对象
	*/
	H3DI::IPrePassLight*  get_IPrePassLight() const;

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

	
	/**
	* @brief
	*		获取灯光的内半径
	* @return
	*		灯光的内半径
	*/
	float near_range() const;

	/**
	* @brief
	*		设置灯光的内半径
	* @param
	*		light_inner_radius：【IN】要设置的灯光的内半径
	*/
	void set_near_range(float light_inner_radius);

	/**
	* @brief
	*		获取灯光的外半径
	* @return
	*		灯光的外半径
	*/
	float far_range() const;

	/**
	* @brief
	*		设置灯光的外半径
	* @param
	*		light_outter_radius：【IN】灯光的外半径
	*/
	void set_far_range(float light_outter_radius);

	/**
	* @brief
	*		设置灯光影响范围
	*/
	virtual void set_affect_param(H3DI::LightAffectParam t);

	/**
	* @brief
	*		获取灯光影响范围
	*/
	virtual H3DI::LightAffectParam get_affect_param();

	/**
	* @brief
	*		设置是否画光柱范围
	*/
	void set_beam_line_draw(bool is_draw);
	/**
	* @brief
	*		得到是否画光柱范围
	*/
	bool get_beam_line_draw();

	/**
	* @brief
	*		创建一个光斑
	* @param
	*		glare_property：【IN】光斑属性
	*/
	virtual int create_glare(const GlareProperty& glare_property);

	/**
	* @brief
	*		删除光斑
	* @param
	*		glare_index：【IN】光斑索引
	*/
	//virtual void delete_galare(int glare_index);

	/**
	* @brief
	*		判断光斑是否创建
	* @param
	*		glare_index：【IN】光斑索引
	*/
	virtual bool is_glare_created(int glare_index);

	/**
	* @brief
	*		设置光斑属性
	* @param
	*		glare_index：【IN】光斑索引
	*		glare_property：【IN】光斑属性
	*/
	virtual void set_glare_property(int glare_index, const GlareProperty& glare_property);

	/**
	* @brief
	*		获取光斑属性
	* @param
	*		glare_index：【IN】光斑索引
	* @return
	*
	*/
	virtual GlareProperty get_glare_property(int glare_index);

	/**
	* @brief
	*		获取光斑id
	* @param
	*		index：【IN】光斑索引
	* @return
	*		光斑id
	*/
	virtual int get_glare_index(int index);

	/**
	* @brief
	*		判断光源是否起作用
	* @return
	*		起作用：TRUE
	*		不起作用：FALSE
	*/
	virtual bool is_light_enable() const;

	/**
	* @brief
	*		设置灯光是否起作用
	* @param
	*		light_enable：【IN】设置灯光是否起作用
	*/
	virtual void set_light_enable(bool light_enable);

	/**
	* @brief
	*		设置灯光是否起作用
	* @param
	*		light_enable：【IN】设置灯光是否起作用
	*/
	virtual void set_entity_light_enable(bool light_enable);

	/**
	* @brief
	*		判断光源是否起作用
	* @return
	*		起作用：TRUE
	*		不起作用：FALSE
	*/
	virtual bool is_entity_light_enable() const;

	/**
	* @brief
	*		获得灯光的属性集合（优化用）
	* @return
	*		灯光的属性集合，只对人物灯光（以及场景与人物共同起作用的灯光）有效
	*/
	LightCachedProperty* get_light_property() { return &m_light_cached_property; }

	/**
	* @brief
	*		设置灯光的可见性状态
	*/
	void set_light_visible_status(bool visible_status);

	/**
	* @brief
	*		获取灯光的可见性状态
	*/
	bool get_light_visible_status() const;

	/**
	* @brief
	*		获取灯光的优先级
	*/
	int get_priority() const;

	/**
	* @brief
	*		设置灯光的优先级
	*/
	void set_priority(int light_priority);

	/**
	* @brief
	*		开启/关闭体积光
	*/
	virtual void set_light_shaft_enable(bool enable);

	/**
	* @brief
	*		判断体积光是否开始
	*/
	virtual bool is_light_shaft_enable();

	/**
	* @brief
	*		设置体积光颜色
	*/
	virtual void set_light_shaft_color(const H3DColor& shaft_color);

	/**
	* @brief
	*		获取体积光颜色
	*/
	virtual H3DColor get_light_shaft_color() const;

	/**
	* @brief
	*		设置体积光强度
	*/
	virtual void set_light_shaft_intensity(float shaft_intensity);
	/**
	* @brief
	*		获取体积光强度
	*/
	virtual float get_light_shaft_intensity();

	/**
	* @brief
	*		设置体积光阀值
	*/
	virtual void set_light_shaft_threshold(float threshold);

	/**
	* @brief
	*		获取体积光阀值
	*/
	virtual float get_light_shaft_threshold();

	/**
	* @brief
	*		设置体积光深度
	*/
	virtual void set_light_shaft_depth(float depth);

	/**
	* @brief
	*		获取体积光深度
	*/
	virtual float get_light_shaft_depth();

	/**
	* @brief
	*		设定灯光效果可用
	*/
	virtual void turn_on_light(bool enable_light);

	/**
	* @brief
	*		灯光舞台效果是否开启
	*/
	virtual bool is_light_turn_on();

	/**
	* @brief
	*		设定对象是否可见
	* @param
	*		is_visible：【IN】设定对象是否可见
	*/
	virtual void set_visible(bool visible);

	/**
	* @brief
	*		设置可控实体的状态是否可用
	* @param
	*		is_eanbel：【IN】可控实体的状态设置标志
	*/
	virtual void set_enable(bool enable) ;

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
	*		绘制表示灯光位置的线
	* @param
	*		renderSystem：【IN】绘制系统
	*/
	void draw_light_select_line(CRenderSystem& render_system);
	/**
	* @brief
	*		引擎灯光是否可用
	*/
	virtual bool is_engine_light_enable();

	/// 开启或者关闭引擎灯光
	virtual void enable_engine_light(bool enable_light);

	virtual void switch_to_lod(int lod,bool is_sync = true);
	/**
	* @brief
	*		外部认为需要打开灯光
	*/
	virtual void set_external_visible(bool on);

	/**
	* @brief
	*		是否外部认为需要代开
	*/
	virtual bool is_external_visible() const;

	virtual void set_world_location_matrix(const H3DMat4& location);

	virtual void enable_fake_light_spot(bool bEnabled);
	virtual bool is_fake_light_spot_enable(){return m_is_fake_light_enabled;};
	void record_fake_light_spot(bool bEnable);
	bool get_record_fake_light_spot(){return m_record_fake_light_spot;};

	//设置和获取lightmap阴影指数，仅编辑器使用
	virtual void set_lightmap_shadow_index(float shadow_index){m_lightmap_shadow_index = shadow_index;};
	virtual float get_lightmap_shadow_index(){return m_lightmap_shadow_index;};

	//设置和获取lightmap光源半径 ，仅编辑器使用
	virtual void set_lightmap_radius(float lightmap_radius){m_lightmap_radius = lightmap_radius;};
	virtual float get_lightmap_radius(){return m_lightmap_radius;};

	//设置灯光的GUID属性
	virtual void set_guid(const char* guid)
	{
		m_guid = guid;
		m_light->SetGUID(m_guid.c_str());
	}

private:
	typedef std::vector<std::pair<int, GlareProperty> > GlareList;



protected:
	//开启引擎灯光效果
	virtual void update_engine_light_enable();
	//设置引擎光斑的属性
	virtual void set_engine_glare_property(int index, const H3DI::tGlareProperty& glare_property);
	//绘制灯光线
	virtual void draw_light_location_line(CRenderSystem& render_sys) = 0;

protected:

	GlareList	m_created_glares; ///< 创建的光斑列表

	bool			m_is_light_enable; ///< 开启灯光
	bool			m_light_visible_status; ///< 灯光可见状态
	bool			m_is_light_turn_on; ///< 灯光效果是否被打开
	bool			m_draw_beam_line;///< 光的范围 
	bool			m_is_engine_light_enable; ///< 引擎的灯光是否开启 
	//bool			m_is_external_visible; /// 外部是否设定打开灯光
	bool			m_is_entity_light_enable; /// 灯光状态
	
	bool			m_enable_shaft; /// 是否开启体积光

	/// 灯光属性，变为一个结构体
	LightCachedProperty m_light_cached_property;

	// 均被替换为 m_light_cached_property
	// H3DColor		m_color; /// 灯光颜色
	// H3DColor		m_shader_color; /// 阴影颜色
	// bool			m_enable_shadow; /// 是否允许灯光有阴影
	// int				m_priority; /// 灯光优先级
	// float			m_intensity; /// 灯光强度
	// float			m_near_range; /// 灯光近范围
	// float			m_far_range; /// 灯光远范围
	// H3DI::LightAffectParam m_affect_param; /// 灯光影响范围

	H3DColor		m_shaft_color; /// 体积光颜色
	float			m_shaft_intensity; /// 体积光强度
	float			m_shaft_threshold; /// 体积光阀值
	float			m_shaft_depth; /// 体积光深度

	
	H3DI::IPrePassLight* m_light; ///< 对应的引擎层灯光实体


	bool			m_is_fake_light_enabled;	//用于记录引擎灯光是否设置了fakelight；
	bool			m_record_fake_light_spot;	//用于记录外部对fakelightspot的设置；

	//仅编辑器使用。
	float			m_lightmap_shadow_index;	
	float			m_lightmap_radius;

private:
	FORBIDDEN_EVIL_CONSTRUCTOR(CPrePassLightEntity);
};

#endif //_PREPASS_LIGHT_ENTITY_H_
