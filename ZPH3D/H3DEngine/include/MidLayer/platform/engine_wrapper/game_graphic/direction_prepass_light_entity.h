/**
* @file direction_prepass_light_entity.h
* 这个文件的主要功能是：
* 
* @author 程陈(Chengchen@h3d.com.cn)
* 
* 更改日志
* ----------------------------------------------------
* ver 1.0.0
*       by 程陈2010.6.9
*/

#ifndef _DIRECTION_PREPASS_LIGHT_ENTITY_H_
#define _DIRECTION_PREPASS_LIGHT_ENTITY_H_

#include "game_graphic_header.h"

class CDirectionPrePassLightEntity
	:public CPrePassLightEntity
{
public:
	CDirectionPrePassLightEntity(CGraphicsResMng* res_mng, const std::string& name, H3DI::IPrePassLight* light);
	virtual ~CDirectionPrePassLightEntity();

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

	//绘制灯光线
	virtual void draw_light_location_line(CRenderSystem& render_sys);

	virtual void switch_to_lod(int lod, bool is_sync = true);

	/*
	* @brief
	*		设置方向光的高光强度
	*/
	virtual void set_specular_intensity(float fIntensity);
	float	get_specular_intensity() const{ return m_specular_intensity;};
	/**
	* @brief
	*		设置灯光强度
	* @param
	*		light_intensity：【IN】要设置的灯光强度
	*/
	virtual void set_intensity(float light_intensity);

private:
	float		m_specular_intensity;
	FORBIDDEN_EVIL_CONSTRUCTOR(CDirectionPrePassLightEntity);

};

#endif //_DIRECTION_PREPASS_LIGHT_H_
