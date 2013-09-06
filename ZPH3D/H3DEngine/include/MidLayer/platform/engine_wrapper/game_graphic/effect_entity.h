/**
* @file effect_entity.h
* 这个文件的主要功能是：
* 
* @author 程陈(Chengchen@h3d.com.cn)
* 
* 更改日志
* ----------------------------------------------------
* ver 1.0.0
*       by 程陈2010.1.29
*/

#ifndef _EFFECT_ENTITY_H_
#define _EFFECT_ENTITY_H_

#include "game_graphic_header.h"
#include "3DEntity.h"

class CameraInfo;
class CSkeletonModelEntity;

class CEffectEntity
	:public C3DEntity
{
public:
	CEffectEntity(CGraphicsResMng* res_mng, const std::string& name, const std::string& resource_name, int lod);

	/**
	* @brief
	*		播放媒体
	*/
	virtual void play(void) ;

	/**
	* @brief
	*		停止播放媒体
	*/
	virtual void stop(void) ;

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
	*		获取对应的ISpecialEffect对象
	*/
	ISpecialEffect* get_IEffect() const;

	/**
	* @brief
	*		设置特效的透明度
	* @param
	*		transparence：【IN】】要设置的透明度，范围0—1
	*/
	void set_transparence(float new_transparence);

	/**
	* @brief
	*		获取特效的透明度
	* @return
	*		特效的透明度，大小0——1
	*/
	float transparence();

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
	*		设置特效是否投射阴影
	*/
	virtual void enable_cast_shadow(bool cast_shadow);

	/**
	* @brief
	*		设置特效是否投射阴影
	*/
	virtual bool is_cast_shadow_enable() const;

	/**
	@brief 设置位置偏移
	*/
	virtual void setPositionOffset(float x,float y,float z);

	/**
	@brief 获得位置偏移
	*/
	virtual void getPositionOffset(float& x,float& y,float& z);

	/**
	@brief 调整特效角度，用于某些特效朝向相机的设置
	*/
	void setCameraAngle(CameraInfo& camInfo , CSkeletonModelEntity* bindTarget );
	/**
	@brief 设置是否始终面向相机旋转
	*/
	void enableCameraAngleAdapt(bool enable){m_cameraAngleAdapt = enable;};
	/**
	@brief 臛断是否始终面向相机选择
	*/
	bool isCameraAngleAdapt(){return m_cameraAngleAdapt;};
	/**
	* @brief
	*		重新开始
	*/
	virtual void reset(void) ;

	virtual void set_visible(bool visible);
	virtual void set_external_visible(bool visible);
	virtual void update_engine_entity_visible();

	virtual void switch_to_lod(int lod, bool is_sync = true);

	/**
	* @brief
	*		设置此特效为角色相关特效
	*/
	void set_is_actor_effect(bool is);

	/**
	* @brief
	*		得到特效的总长度，返回TIME_MAX表示循环特效
	*/
	float  get_animation_length();

	/**
	* @brief
	*		获得资源描述
	*/
	void getEffectAppearance(EffectAppearance& appearance);

	virtual void UnloadResources();

private:
	/**
	* @brief
	*		更新引擎层对象
	* @param
	*		cur_update_time：【IN】本次更新的时间
	*		intervalTime：【IN】距离上次更新的事件间隔
	*/
	virtual void	update_media_object(float interval) ;


	ISpecialEffect* create_effect(const std::string& res_name);
	void	cache_effect();

protected:
	virtual ~CEffectEntity();

	virtual void create_engine_object_impl(bool is_sync);
	virtual void set_engine_object_impl(H3DI::IMoveObject* object);
	virtual void after_create_impl();

	void switch_to_lod_immediate(int lod, bool is_sync);

private:
	friend class CModelEntity;

	ISpecialEffect* m_effect; ///< 引擎层对应的ISpecialEffect对象

	float m_transparence; ///< 特效的透明度
	bool  m_cast_shadow_enable; ///< 开启实时阴影

	std::map<std::string, ISpecialEffect*> m_effect_pool; ///< 暂时实现的，目标是用来缓存特效

	float m_posOffsetX;///< 设置相对偏移
	float m_posOffsetY;///< 设置相对偏移
	float m_posOffsetZ;///< 设置相对偏移

	bool m_cameraAngleAdapt;

	//Cache的属性
	int		m_state;
	bool		m_is_actor_effect;

	CModelEntity* m_parent_model;

private:
	FORBIDDEN_EVIL_CONSTRUCTOR(CEffectEntity);
};


#endif //_EFFECT_ENTITY_H_
