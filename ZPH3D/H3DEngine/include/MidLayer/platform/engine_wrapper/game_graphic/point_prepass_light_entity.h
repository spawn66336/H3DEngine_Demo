/**
* @file point_prepass_light_entity.h
* 这个文件的主要功能是：
* 
* @author 程陈(Chengchen@h3d.com.cn)
* 
* 更改日志
* ----------------------------------------------------
* ver 1.0.0
*       by 程陈2010.5.21
*/

#ifndef _POINT_PREPASS_LIGHT_ENTITY_H_
#define _POINT_PREPASS_LIGHT_ENTITY_H_

#include "game_graphic_header.h"

class CPointPrePassLightEntity
	:public CPrePassLightEntity
{
public:
	CPointPrePassLightEntity(CGraphicsResMng* res_mng, const std::string& name, H3DI::IPrePassLight* light);
	virtual ~CPointPrePassLightEntity();

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

	//绘制灯光线
	virtual void draw_light_location_line(CRenderSystem& render_sys);
private:
	FORBIDDEN_EVIL_CONSTRUCTOR(CPointPrePassLightEntity);
};

#endif //_POINT_PREPASS_LIGHT_ENTITY_H_
