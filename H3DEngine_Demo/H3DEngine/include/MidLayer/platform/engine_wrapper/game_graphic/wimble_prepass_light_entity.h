/**
* @file wimble_prepass_light_entity.h
* 这个文件的主要功能是：
* 
* @author 程陈(Chengchen@h3d.com.cn)
* 
* 更改日志
* ----------------------------------------------------
* ver 1.0.0
*       by 程陈2010.5.21
*/

#ifndef _WIMBLE_PREPASS_LIGHT_ENTITY_H_
#define _WIMBLE_PREPASS_LIGHT_ENTITY_H_

#include "game_graphic_header.h"

class CFollowingBind;

class CWimblePrePassLightEntity
	:public CPrePassLightEntity
{
public:
	CWimblePrePassLightEntity(CGraphicsResMng* res_mng, const std::string& name, H3DI::IPrePassLight* light);
	virtual ~CWimblePrePassLightEntity();

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
	*		获取灯光的内角度
	* @return
	*		灯光的内角度，单位为角度
	*/
	virtual float inner_angle() const;

	/**
	* @brief
	*		设置灯光的内角度
	* @param
	*		light_inner_angle：【IN】要设置的灯光的内角度
	*/
	virtual void set_inner_angle(float light_inner_angle);

	/**
	* @brief
	*		获取灯光的外角度
	* @return
	*		灯光的外角度，单位为角度
	*/
	virtual float outter_angle() const;
	
	/**
	* @brief
	*		设置灯光的外角度
	* @param
	*		light_outter_angle：【IN】灯光的外角度
	*/
	virtual void set_outter_angle(float light_outter_angle);

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

	/**
	* @brief
	*		设置投射纹理的名称
	* @param
	*		texturename：【IN】文件名
	*/
	virtual void set_projected_texture(const std::string& texturename);

	/**
	* @brief
	*		设置投射纹理是否可用
	* @param
	*		enable：【IN】是否可用
	*/
	virtual void	enable_projected_texture(bool enable);

	/**
	* @brief
	*		创建光柱
	*/
	virtual void create_beam(H3DI::EH3DBEAMTYPE beam_type);

	/**
	* @brief
	*		删除光柱
	*/
	virtual void delete_beam();

	/**
	* @brief
	*		设置光柱类型
	* @param
	*
	*/
	virtual void set_beam_type(H3DI::EH3DBEAMTYPE beam_type);

	/**
	* @brief
	*		判断是否创建了光柱	
	*/
	virtual bool is_beam_created();

	/**
	* @brief
	*		设置光柱的本地矩阵
	* @param
	*		mat：【IN】要设置的矩阵
	*/
	virtual void set_beam_local_matrix(const H3DMat4& mat);

	/**
	*  @brief
	*		获取光柱的本地矩阵
	* @return
	*		光柱的本地矩阵
	*/
	virtual const H3DMat4& get_beam_local_matrix() const;

	/**
	* @brief
	*		设置光柱的可见性
	* @param
	*		beam_visible：【IN】设定光柱是否可见
	*/
	virtual void set_beam_visible(bool beam_visible);

	/**
	* @brief
	*		光柱是否可见
	* @return
	*		TRUE：光柱可见
	*		FALSE：光柱不可见
	*/
	virtual bool is_beam_visible() const;

	/**
	* @brief
	*		设置光柱的明暗程度
	* @param
	*		bright_ness：【IN】光柱的明暗程度
	*/
	virtual void set_beam_brightness(float bright_ness);

	/**
	* @brief
	*		获取光柱的亮度
	* @return
	*		光柱的亮度
	*/
	virtual float get_beam_brightness() const;

	/**
	* @brief
	*		设置光柱的内径
	*/
	virtual void set_beam_inner_radius(float t);

	/**
	* @brief
	*		设置后处理光柱的模糊度
	*/
	virtual void set_beam_blur_coeff(float t);

	/**
	* @brief
	*		设置光柱的近范围
	*/
	virtual void set_beam_near_range(float range);

	/**
	* @brief
	*		获取光柱近范围
	*/
	virtual float get_beam_near_range();

	/**
	* @brief
	*		设置光柱远范围
	*/
	virtual void set_beam_far_range(float range);

	/**
	* @brief
	*		获取光柱远范围
	*/
	virtual float get_beam_far_range();

	/**
	* @brief
	*		设置光柱内角度
	*/
	virtual void set_beam_inner_angle(float inner);
	/**
	* @brief
	*		获取光柱的内角度
	*/
	virtual float get_beam_inner_angle();
	/**
	* @brief
	*		设置光柱的外角度
	*/
	virtual void set_beam_outter_angle(float outter);
	/**
	* @brief
	*		获取光柱的外角度
	*/
	virtual float get_beam_outter_angle();

	/**
	* @brief
	*		更新对象
	* @param
	*		intervalTime：【IN】距离上次更新的事件间隔
	*/
	virtual void	update(float interval);

	/// 开启或者关闭引擎灯光
	virtual void enable_engine_light(bool enable_light);

	virtual void switch_to_lod(int lod, bool is_sync = true);

protected:
	/// 开启或者关闭光柱
	virtual void enable_engine_beam(bool enable_beam);
	//设置引擎层光柱可见
	virtual void update_engine_beam_visible();
	//引擎层光柱的可见性
	virtual bool is_engine_beam_visible();

	//开启引擎灯光效果
	virtual void update_engine_light_enable();
	//绘制灯光线
	virtual void draw_light_location_line(CRenderSystem& render_sys);

private:
	H3DMat4			m_beam_location_matrix; ///< 光柱的本地矩阵
	float			m_beam_brightness; ///< 光柱的亮度
	bool			m_is_beam_visible; ///< 光柱是否可见
	bool			m_is_beam_created; ///< 创建光柱
	float			m_beam_inner_angle;	//< 光柱内角
	float			m_beam_out_angle;	///< 光柱外角

	bool			m_is_project_texture;
	std::string		m_project_texture_name; ///< 投影贴图名称

	bool			m_is_engine_beam_visible; ///< 引擎层的光柱是否可见
	float			m_beam_inner_radius;
	H3DI::EH3DBEAMTYPE	m_lod0_beam_type;	///<用于记录LOD0时beam type。供lod切换时调用;
private:
	FORBIDDEN_EVIL_CONSTRUCTOR(CWimblePrePassLightEntity);
};

#endif //_WIMBLE_PREPASS_LIGHT_ENTITY_H_
