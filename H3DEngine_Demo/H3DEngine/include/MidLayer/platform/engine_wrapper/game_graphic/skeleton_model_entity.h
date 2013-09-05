/*
*@file  skeleton_model_entity.h
*@breif 实现了引擎层的ISkeletonModel类的封装类CSkeletonModelEntity的定义
*@author mozisi
*/

#ifndef __SKELETON_MODEL_ENTITY_H__
#define __SKELETON_MODEL_ENTITY_H__
#include "game_graphic_header.h"
#include "model_entity.h"

typedef int ANIMATION_CHANNEL_ID;
const ANIMATION_CHANNEL_ID ANIMATION_CHANNEL_ALL = -1;
const ANIMATION_CHANNEL_ID ANIMATION_CHANNEL_ACTION = 0;
const ANIMATION_CHANNEL_ID ANIMATION_CHANNEL_EYES = 1;
const ANIMATION_CHANNEL_ID ANIMATION_CHANNEL_LEFT_HAND = 2;
const ANIMATION_CHANNEL_ID ANIMATION_CHANNEL_RIGHT_HAND = 3;
const ANIMATION_CHANNEL_ID ANIMATION_CHANNEL_COUNT = 4;

struct AttachItemInfo
{
	AttachItemInfo()
	{
		m_id	= -1;
	}
	H3DI::IAttachModelInfo	m_info;
	int		m_id;
	std::vector<std::string>	m_eff_list;
	CModelEntity*		m_entity;
};


/*
*@breif 对引擎层的ISkeletonModel的封装
*/
class CSkeletonModelEntity : public CModelEntity
{
	friend class AttachOperation;
	friend class ActionOperation;
	friend class NextActionOperation;
public:
	CSkeletonModelEntity(CGraphicsResMng* res_mng, const std::string& name, const std::string& resource_name, int lod);

	/*
	*@breif 获取对应的引擎对象
	*@return 对应的引擎对象
	*/
	H3DI::ISkeletonModel* getISkeletonModel() const;

	/*
	*@breif 更换chr资源
	*@param res_name 资源路径
	*@return 是否修改成功
	*/
	virtual bool change_resource(const std::string& res_name);

	virtual void set_action(unsigned int channel_id, const char* action, bool loop, bool is_sync = true);
	virtual void set_next_action(unsigned int channel_id, const char* action, bool loop, bool is_sync = true);

	/**
	* @brief
	*		判断频道id是否合法
	* @param
	*		channel_id：【IN】频道id
	*/
	bool is_valid_channel(ANIMATION_CHANNEL_ID channel_id) const;

	/**
	* @brief
	*		将频道设为循环
	* @param
	*		channel_id：【IN】动作序列所属频道id
	*		channel_loop：【IN】设为循环
	*/
	void set_channel_loop(ANIMATION_CHANNEL_ID channel_id, bool channel_loop);

	/*
	*@breif 获取当前动作id
	*/
	virtual const char* getActionId(unsigned int channel_id){return m_action_id[channel_id].c_str();}

	/*
	*@breif 获取当前是否是循环动作
	*/
	virtual bool getActionLoop(ANIMATION_CHANNEL_ID channel_id = ANIMATION_CHANNEL_ACTION) const {return m_action_loop[channel_id];}

	/*
	*@breif 获取下一动作id
	*/
	virtual const char* getNextActionId(ANIMATION_CHANNEL_ID channel_id = ANIMATION_CHANNEL_ACTION) const {return m_next_action_id[channel_id].c_str();}

	/*
	*@breif 获取下一动作是否是循环动作
	*/
	virtual bool getNextActionLoop(ANIMATION_CHANNEL_ID channel_id = ANIMATION_CHANNEL_ACTION) const {return m_next_action_loop[channel_id];}

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
	@brief 设置插值方式
	@param chennelid：通道号
	@param fixfoot 是否双脚固定方式
	@return 位置矩阵
	*/
	void set_fixfoot_transmode(unsigned int channel_id, bool fixfoot);

	/**
	@brief 设置插值模式
	@param chennelid：通道号 mode：模式
	*/
	virtual void set_transition_mode(unsigned int chennelid, int mode);

		/**
	@brief 取插值模式
	@param chennelid：通道号 mode：模式
	*/
    virtual int get_transition_mode(unsigned int chennelid);

		/**
	* @brief
	*		
	* @param
	*		channel_id：【IN】
	*		interval：【IN】
	*/
	virtual void set_transition_time_from_current(ANIMATION_CHANNEL_ID channel_id, float interval);

	/**
	* @brief
	*		
	* @param
	*		channel_id：【IN】
	* @return
	*
	*/
	virtual float get_transition_time_from_current(ANIMATION_CHANNEL_ID channel_id) const;

	/**
	* @brief 动作平滑时间
	*		
	* @param
	*		channel_id：【IN】
	*		interval：【IN】
	*/	
	virtual void set_transition_time_to_next(ANIMATION_CHANNEL_ID channel_id, float interval);

	/**
	* @brief
	*		;
	* @param
	*		channel_id：【IN】
	* @return
	*		
	*/
	float get_transition_time_to_next(ANIMATION_CHANNEL_ID channel_id) const;

	/**
	@brief 取当前动作播放时间
	@param chennelid：通道号 
	@return 当前动作播放时间
	*/
	float get_cur_action_time(unsigned int channel_id);

	/**
	@brief 取当前动作引擎实际播放时间
	@param chennelid：通道号 
	@return 当前动作引擎实际播放时间
	*/
	float get_cur_real_action_time(unsigned int channel_id);

	/**
	@brief 取当前动作总长度
	@param chennelid：通道号 
	@return 当前动作总长度
	*/
	float get_cur_action_length(unsigned int channel_id);

	/**
	* @brief
	*		添加控制通道
	*/
	void add_dominating_channel(int action_channel);
	/**
	* @brief
	*		移除控制通道
	*/
	void remove_dominating_channel(int action_channel);

	/**
	* @brief
	*		重置BodyPart、挂件和动作等
	*/
	virtual void reset();

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
	*		type：【IN】类的类型
	* @return
	*		true：为指定的类型
	*		false：不为指定的类型
	*/
	virtual bool is_kind_of(int obj_type);

	/**
	* @brief 设置挂接配饰
	*		
	*/
	virtual void attach_model(const AttachItemInfo& info);

	/**
	* @brief 取消挂接配饰
	*		
	*/
	virtual void detach_model(const AttachItemInfo& info);

	/**
	* @brief 取消某个BODYPART上的所有配饰
	* @param pos 这个位置是已经通过ToEngineBodyPartPos转换为引擎使用的pos,需要外面自己先转好
	*		
	*/
	virtual void deatch_bodypart_model(int pos);

	virtual void remove_all_attached_model();

	/**
	* @brief 设置当前所有配饰的显隐
	* @param
	*		val：显示/隐藏
	*/
	void SetAllAttachedModelVisible(bool val);

	const H3DMat4* GetCurMatrix(unsigned int nBone);
	void getRootBonePos(float* pos);
	H3DI::ISkeleton* GetSkeleton();

	/**
	* @brief
	*		获得骨骼矩阵
	*/
	const H3DMat4* get_bone_matrix(const std::string& boneName);

	/**
	* @brief
	*		开启/关闭物理
	*/
	void set_actor_phy(bool enable);

	/**
	* @brief
	*		更新蒙皮到预定义姿态
	*/
	void update_skin_pose(int bpID,bool bUpdate);

	/**
	* @brief
	*		获得资源描述
	*/
	void getSkeletonModelAppearance(SkeletonModelAppearance& appearance);

	/**
	* @brief
	*		设置LightMap信息
	*/
	void initLightMapInfo(const char* path);

	virtual void switch_to_lod(int lod, bool is_sync = true);

protected:
	virtual void do_update(float interval);
	virtual void setAction(unsigned int channel_id, const char* action, bool loop);
	virtual void setNextAction(unsigned int channel_id, const char* action, bool loop);
	void add_set_actions(ANIMATION_CHANNEL_ID channel_id, const char* action, bool loop);
	void add_set_next_actions(ANIMATION_CHANNEL_ID channel_id, const char* action, bool loop);

	int find_attach_info(const AttachItemInfo& info);

	void updateLightMap();

protected:
	virtual ~CSkeletonModelEntity();

	virtual void clear_all_op();
	virtual void set_engine_object_impl(H3DI::IMoveObject* object);
	virtual void after_create_impl();

	void UpdateAttachItem();
	void UnloadAttachItem();
	void ReloadAttachItem(bool is_sync);

	AttachItemInfo* CSkeletonModelEntity::get_attach_info(int bodypart, int id);
	void attach_model_immediate(AttachItemInfo& pInfo);

protected:
	H3DI::ISkeletonModel*		m_skeleton_model; ///< 引擎层对应的对象

	std::map<int, std::vector<AttachItemInfo>>		m_attach_item_info;
	bool		m_is_all_attach_item_visible;

	std::string		m_action_id[ANIMATION_CHANNEL_COUNT]; ///< 当前动作id
	bool			m_action_loop[ANIMATION_CHANNEL_COUNT]; ///< 当前动作是否是loop
	Operation*		m_action_op;

	std::string		m_next_action_id[ANIMATION_CHANNEL_COUNT]; ///< 下一个动作id
	bool			m_next_action_loop[ANIMATION_CHANNEL_COUNT]; ///< 当前动作是否是loop
	Operation*		m_next_action_op;

	bool		m_fix_foot;
	float		m_played_time;
	float		m_transition_time_from_current;
	float		m_transition_time_to_next;

	bool		m_actor_phy;
	bool		m_has_lightmap;

private:
	FORBIDDEN_EVIL_CONSTRUCTOR(CSkeletonModelEntity);
};
#endif
