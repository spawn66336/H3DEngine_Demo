/*
*@file  avatar_skeleton_model_entity.h
*@breif 实现对引擎层的IAvatarSkeletonModel的封装
*@author mozisi
*/
#ifndef __AVATAR_SKELETON_MODEL_ENTITY_H__
#define __AVATAR_SKELETON_MODEL_ENTITY_H__
#include "game_graphic_header.h"
#include "skeleton_model_entity.h"

/*
*@breif 对对引擎层的IAvatarSkeletonModel的封装
*/
class CAvatarSkeletonModelEntity : public CSkeletonModelEntity
{
	friend class AdornmentOperation;
	friend class BodypartOperation;
public:
	CAvatarSkeletonModelEntity(CGraphicsResMng* res_mng, const std::string& name
		, bool is_male, int lod);

		/*
	*@breif 获取对应的引擎对象
	*@return 对应的引擎对象
	*/
	H3DI::IAvatarSkeletonModel* getIAvatarSkeletonModel() const;

	/**
	* @brief
	*		获取类的类型
	* @return
	*		类的类型
	*/
	virtual int type(void) const;

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
	*		设置身体部分
	* @param
	*		body_part：【IN】设定要更新的身体部位
	*		name：【IN】对应的资源名称
	*/
	virtual void set_body_part(int body_part, const std::string& name, bool is_sync = true);

	/*
	*@breif 设置挂件
	*@param ad_position 挂件位置
	*@param ad_file 挂件资源路径，如果填空指针或者空串，则表示卸载该位置的挂件
	*@return 是否成功
	*/
	virtual void setAdornment(int ad_position, const char* ad_file
		, const char* action = NULL, bool is_sync = true);

	/**
	* @brief
	*		设置是否将挂件信息更新到引擎，这个默认取消左右手的挂件与动作
	*/
	virtual void set_adornment_surpress(bool surpress);

	/**
	* @brief 设置是否将指定的挂件设置到引擎上
	*		
	*/
	virtual void set_adornment_surpress(bool surpress, H3DI::EPetAdornmentPosition ad_position);

	/**
	@brief 设置动作
	@param channel_id：通道号 action：动作名称 loop：是否循环播放
	*/
	void set_action(unsigned int channel_id, const char* action, bool loop, bool is_sync = true);

		/**
	@brief 设置下一动作
	@param channel_id：通道号 action：动作名称 loop：是否循环播放
	*/
	void  set_next_action(unsigned int channel_id, const char* action, bool loop, bool is_sync = true);

	/**
	* @brief
	*		重置挂件和动作
	*/
	virtual void reset();

	/**
	* @brief
	*		获得资源描述
	*/
	void getAvatarSkeletonModelAppearance(AvatarSkeletonAppearance& appearance);

	/**
	* @brief
	*		更新挂件
	* @param
	*		interval：【IN】距离上次更新的事件间隔，单位毫秒
	*/
	void update_adornment(float interval);

	//! 设置挂件动作
	// 参数adornmentposition：挂件部位
	// 参数actionName：挂件CHR中的动作名称
	// 参数transitionTime：从现有动作到新设动作的过渡时间，单位为秒
	// 返回值：如果成功设置了挂件中任何一个CHR的动作，返回true；如果在所有的CHR中都找不到该动作，或者对应的部位没有挂件，返回false
	bool SetAdornmentAction(int adornmentposition, const char* actionName, float transitionTime = -1.0f);

	virtual void UnloadResources();
	virtual void ReloadResources(bool is_sync = true, int priority = 0);

protected:
	virtual ~CAvatarSkeletonModelEntity();
	virtual void do_update(float interval);
	ANIMATION_CHANNEL_ID get_channel_id(int pos);

	virtual void clear_all_op();
	virtual void set_engine_object_impl(H3DI::IMoveObject* object);

	void set_body_part_immediate(int body_part, const std::string& bpt_name, bool is_sync = true);
	void set_adornment_immediate(int ad_position, const char* ad_name, const char* action = NULL, bool is_sync = true);
	void set_action_immediate(unsigned int channel_id, const char* action, bool loop, bool is_sync = true);

private:
	///< 引擎层对应的对象
	H3DI::IAvatarSkeletonModel	*m_avatar_skeleton_model;
	bool			m_is_male; /// 是否为男性

	std::string		m_adorment[H3DI::PET_ADORNMENT_NUM];
	Operation*		m_ad_op[H3DI::PET_ADORNMENT_NUM];

	std::string		m_body_part[H3DI::PET_BODYPART_NUM];
	Operation*		m_bpt_op[H3DI::PET_BODYPART_NUM];

	bool m_adornment_surpress;

private:
	FORBIDDEN_EVIL_CONSTRUCTOR(CAvatarSkeletonModelEntity);
};
#endif
