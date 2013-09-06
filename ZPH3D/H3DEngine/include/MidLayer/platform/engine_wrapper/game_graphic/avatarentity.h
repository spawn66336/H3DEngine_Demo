/**
* @file actor_entity.h
* 这个文件的主要功能是：
* 
* @author 程陈(Chengchen@h3d.com.cn)
* 
* 更改日志
* ----------------------------------------------------
* ver 1.0.0
*       by 程陈2010.1.29
*/

#ifndef _NEWAVATAR_ENTITY_H_
#define _NEWAVATAR_ENTITY_H_

#include "game_graphic_header.h"
#include "skeleton_model_entity.h"

class AvatarEntity
	:public CSkeletonModelEntity
{
	friend class AdornmentOperation;
	friend class BodypartOperation;
public:
	AvatarEntity(CGraphicsResMng* res_mng, const std::string& name, bool is_male, int lod);
	
	/**
	* @brief
	*		设置身体部分
	* @param
	*		body_part：【IN】设定要更新的身体部位
	*		name：【IN】对应的资源名称
	*/
	virtual void set_body_part(int body_part, const std::string& name, bool is_sync = true);

	//重载
	virtual bool change_media_object(H3DI::IMoveObject* object, bool use_preproperty);

	/*
	*@breif 给中间层用的setBodyPart
	*@param body_group 资源名称序列，用分号分割
	*/
	virtual void set_body_part_groups(const std::string& body_group);

	/*
	*@brief 设置皮肤颜色
	*@param skin_color 皮肤颜色，float[4]，表示RGBA
	*/
	virtual void set_skin_color(float* skin_colors);

	/*
	*@breif 给中间层用的改变性别的接口
	*@param is_male 要改成的性别，如果是true则为男
	*/
	virtual void change_sex(bool is_male);

	/**
	* @brief
	*		获取对应的IActor对象
	*/
	H3DI::IActor* get_IActor() const;

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
	virtual int	type(void) const;

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
	@brief 设置动作播放速度，根据BPM设置
	@return speed：速度
	*/
	void setActionSpeed(float speed)
	{
		cur_action_speed = speed;
	};

	virtual void setColorFactor(float* color, int size);

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
	*		重置挂件和动作
	*/
	virtual void reset();

	/**
	* @brief
	*		设置是否将挂件信息更新到引擎，这个默认取消左右手的挂件与动作
	*/
	virtual void set_adornment_surpress(bool surpress);

	/**
	* @brief 设置是否将指定的挂件设置到引擎上
	*		
	*/
	virtual void set_adornment_surpress(bool surpress, H3DI::EActorAdornmentPosition ad_position);

	/**
	* @brief 显示/隐藏指定的挂件
	*		
	*/
	void set_adornment_visible(bool visible, H3DI::EActorAdornmentPosition ad_position);

	/**
	* @brief
	*		获得资源描述
	*/
	void getAvatarAppearance(AvatarAppearance& appearance);

	/**
	* @brief
	*		更新挂件
	* @param
	*		interval：【IN】距离上次更新的事件间隔，单位毫秒
	*/
	void update_adornment(float interval);

	// 打开/关闭高跟鞋偏移 
	// 默认为开
	void	EnableHeelOffset(bool bEable);

	//设置人物是否使用包围体. 返回值1表示成功，小于0不成功
	int setUseCustomBoundBox(bool b);

	//! 设置挂件动作
	// 参数adornmentposition：挂件部位
	// 参数actionName：挂件CHR中的动作名称
	// 参数transitionTime：从现有动作到新设动作的过渡时间，单位为秒
	// 返回值：如果成功设置了挂件中任何一个CHR的动作，返回true；如果在所有的CHR中都找不到该动作，或者对应的部位没有挂件，返回false
	bool SetAdornmentAction(int adornmentposition, const char* actionName, float transitionTime = -1.0f);

	virtual void UnloadResources();
	virtual void ReloadResources(bool is_sync = true, int priority = 0);

protected:
	virtual ~AvatarEntity();
	virtual void do_update(float interval);
	ANIMATION_CHANNEL_ID get_channel_id(int pos);

	virtual void clear_all_op();
	virtual void set_engine_object_impl(H3DI::IMoveObject* object);

	void set_body_part_immediate(int body_part, const std::string& bpt_name, bool is_sync = true);
	void set_adornment_immediate(int ad_position, const char* ad_name, const char* action = NULL, bool is_sync = true);
	void set_action_immediate(unsigned int channel_id, const char* action, bool loop, bool is_sync = true);

private:
	H3DI::IActor* m_avatar;   //引擎层对应的舞蹈者对象
	bool			m_is_male; /// 是否为男性

	float cur_action_speed;		///<  动作播放的速度
	float action_time;			///<  当前动作时间

	std::string		m_adorment[H3DI::ACTOR_ADORNMENT_NUM];
	Operation*		m_ad_op[H3DI::ACTOR_ADORNMENT_NUM];

	std::string		m_body_part[H3DI::BODYPART_NUM];
	Operation*		m_bpt_op[H3DI::BODYPART_NUM];

	bool m_adornment_surpress;

private:
	FORBIDDEN_EVIL_CONSTRUCTOR(AvatarEntity);
};

#endif //_AVATAR_ENTITY_H_
