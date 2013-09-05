/**
* @file lightmap_adjust_entity.h
* 这个文件的主要功能是：
* 
* @author 王恒(wangheng@h3d.com.cn)
* 
* 更改日志
* ----------------------------------------------------
* ver 1.0.0
*       by 王恒2012.3.19
*/

#ifndef _LIGHTMAP_ADJUST_ENTITY_H_
#define _LIGHTMAP_ADJUST_ENTITY_H_

#include "game_graphic_header.h"

class CMediaEndEntity;
class CRenderSystem;
class CSceneEntity;

class CLightmapAdjustEntity : public CMediaEndEntity
{
public:
	CLightmapAdjustEntity(CGraphicsResMng* mng, CSceneEntity* scene, const std::string& entity_name);

	/**
	* @brief
	*		设置Lightmap参数
	*/
	virtual void apply_lightmap_color_factor();
	virtual void set_lightmap_color_factor_a(const H3DColor& a);
	virtual void set_lightmap_color_factor_b(const H3DColor& b);
	virtual void set_lightmap_color_factor_a_intensity(const float fIntensity) ;
	virtual void set_lightmap_color_factor_b_intensity(const float fIntensity) ;

	/**
	* @brief
	*		获取Lightmap参数
	*/
	virtual H3DColor get_lightmap_color_factor_a()const{return m_lightmap_factor_a;};
	virtual H3DColor get_lightmap_color_factor_b()const{return m_lightmap_factor_b;};
	virtual float get_lightmap_color_factor_a_intensity()const{return m_lightmap_factor_a_intensity;};
	virtual float get_lightmap_color_factor_b_intensity()const{return m_lightmap_factor_b_intensity;};

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
	H3DColor			m_lightmap_factor_a;
	float				m_lightmap_factor_a_intensity;
	H3DColor			m_lightmap_factor_b;
	float				m_lightmap_factor_b_intensity;
	CSceneEntity*		m_scene;
	
};


#endif //_SYSTEM_ENTITY_H_
