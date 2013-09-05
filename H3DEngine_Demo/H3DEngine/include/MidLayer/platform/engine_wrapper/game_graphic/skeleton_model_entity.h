/*
*@file  skeleton_model_entity.h
*@breif ʵ����������ISkeletonModel��ķ�װ��CSkeletonModelEntity�Ķ���
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
*@breif ��������ISkeletonModel�ķ�װ
*/
class CSkeletonModelEntity : public CModelEntity
{
	friend class AttachOperation;
	friend class ActionOperation;
	friend class NextActionOperation;
public:
	CSkeletonModelEntity(CGraphicsResMng* res_mng, const std::string& name, const std::string& resource_name, int lod);

	/*
	*@breif ��ȡ��Ӧ���������
	*@return ��Ӧ���������
	*/
	H3DI::ISkeletonModel* getISkeletonModel() const;

	/*
	*@breif ����chr��Դ
	*@param res_name ��Դ·��
	*@return �Ƿ��޸ĳɹ�
	*/
	virtual bool change_resource(const std::string& res_name);

	virtual void set_action(unsigned int channel_id, const char* action, bool loop, bool is_sync = true);
	virtual void set_next_action(unsigned int channel_id, const char* action, bool loop, bool is_sync = true);

	/**
	* @brief
	*		�ж�Ƶ��id�Ƿ�Ϸ�
	* @param
	*		channel_id����IN��Ƶ��id
	*/
	bool is_valid_channel(ANIMATION_CHANNEL_ID channel_id) const;

	/**
	* @brief
	*		��Ƶ����Ϊѭ��
	* @param
	*		channel_id����IN��������������Ƶ��id
	*		channel_loop����IN����Ϊѭ��
	*/
	void set_channel_loop(ANIMATION_CHANNEL_ID channel_id, bool channel_loop);

	/*
	*@breif ��ȡ��ǰ����id
	*/
	virtual const char* getActionId(unsigned int channel_id){return m_action_id[channel_id].c_str();}

	/*
	*@breif ��ȡ��ǰ�Ƿ���ѭ������
	*/
	virtual bool getActionLoop(ANIMATION_CHANNEL_ID channel_id = ANIMATION_CHANNEL_ACTION) const {return m_action_loop[channel_id];}

	/*
	*@breif ��ȡ��һ����id
	*/
	virtual const char* getNextActionId(ANIMATION_CHANNEL_ID channel_id = ANIMATION_CHANNEL_ACTION) const {return m_next_action_id[channel_id].c_str();}

	/*
	*@breif ��ȡ��һ�����Ƿ���ѭ������
	*/
	virtual bool getNextActionLoop(ANIMATION_CHANNEL_ID channel_id = ANIMATION_CHANNEL_ACTION) const {return m_next_action_loop[channel_id];}

	/**
	* @brief
	*		�޸���Դ
	* @param
	*		object����IN��Ҫ�޸ĵ���Դ����
	* @return
	*		true���޸ĳɹ�
	*		false���޸�ʧ��
	*/
	virtual bool change_media_object(H3DI::IMoveObject* object, bool use_preproperty);

	/**
	@brief ���ò�ֵ��ʽ
	@param chennelid��ͨ����
	@param fixfoot �Ƿ�˫�Ź̶���ʽ
	@return λ�þ���
	*/
	void set_fixfoot_transmode(unsigned int channel_id, bool fixfoot);

	/**
	@brief ���ò�ֵģʽ
	@param chennelid��ͨ���� mode��ģʽ
	*/
	virtual void set_transition_mode(unsigned int chennelid, int mode);

		/**
	@brief ȡ��ֵģʽ
	@param chennelid��ͨ���� mode��ģʽ
	*/
    virtual int get_transition_mode(unsigned int chennelid);

		/**
	* @brief
	*		
	* @param
	*		channel_id����IN��
	*		interval����IN��
	*/
	virtual void set_transition_time_from_current(ANIMATION_CHANNEL_ID channel_id, float interval);

	/**
	* @brief
	*		
	* @param
	*		channel_id����IN��
	* @return
	*
	*/
	virtual float get_transition_time_from_current(ANIMATION_CHANNEL_ID channel_id) const;

	/**
	* @brief ����ƽ��ʱ��
	*		
	* @param
	*		channel_id����IN��
	*		interval����IN��
	*/	
	virtual void set_transition_time_to_next(ANIMATION_CHANNEL_ID channel_id, float interval);

	/**
	* @brief
	*		;
	* @param
	*		channel_id����IN��
	* @return
	*		
	*/
	float get_transition_time_to_next(ANIMATION_CHANNEL_ID channel_id) const;

	/**
	@brief ȡ��ǰ��������ʱ��
	@param chennelid��ͨ���� 
	@return ��ǰ��������ʱ��
	*/
	float get_cur_action_time(unsigned int channel_id);

	/**
	@brief ȡ��ǰ��������ʵ�ʲ���ʱ��
	@param chennelid��ͨ���� 
	@return ��ǰ��������ʵ�ʲ���ʱ��
	*/
	float get_cur_real_action_time(unsigned int channel_id);

	/**
	@brief ȡ��ǰ�����ܳ���
	@param chennelid��ͨ���� 
	@return ��ǰ�����ܳ���
	*/
	float get_cur_action_length(unsigned int channel_id);

	/**
	* @brief
	*		��ӿ���ͨ��
	*/
	void add_dominating_channel(int action_channel);
	/**
	* @brief
	*		�Ƴ�����ͨ��
	*/
	void remove_dominating_channel(int action_channel);

	/**
	* @brief
	*		����BodyPart���Ҽ��Ͷ�����
	*/
	virtual void reset();

	/**
	* @brief
	*		��ȡ�������
	* @return
	*		�������
	*/
	virtual int		type(void) const;

	/**
	* @brief
	*		�ж϶����Ƿ�ΪҪ�������
	* @param
	*		type����IN���������
	* @return
	*		true��Ϊָ��������
	*		false����Ϊָ��������
	*/
	virtual bool is_kind_of(int obj_type);

	/**
	* @brief ���ùҽ�����
	*		
	*/
	virtual void attach_model(const AttachItemInfo& info);

	/**
	* @brief ȡ���ҽ�����
	*		
	*/
	virtual void detach_model(const AttachItemInfo& info);

	/**
	* @brief ȡ��ĳ��BODYPART�ϵ���������
	* @param pos ���λ�����Ѿ�ͨ��ToEngineBodyPartPosת��Ϊ����ʹ�õ�pos,��Ҫ�����Լ���ת��
	*		
	*/
	virtual void deatch_bodypart_model(int pos);

	virtual void remove_all_attached_model();

	/**
	* @brief ���õ�ǰ�������ε�����
	* @param
	*		val����ʾ/����
	*/
	void SetAllAttachedModelVisible(bool val);

	const H3DMat4* GetCurMatrix(unsigned int nBone);
	void getRootBonePos(float* pos);
	H3DI::ISkeleton* GetSkeleton();

	/**
	* @brief
	*		��ù�������
	*/
	const H3DMat4* get_bone_matrix(const std::string& boneName);

	/**
	* @brief
	*		����/�ر�����
	*/
	void set_actor_phy(bool enable);

	/**
	* @brief
	*		������Ƥ��Ԥ������̬
	*/
	void update_skin_pose(int bpID,bool bUpdate);

	/**
	* @brief
	*		�����Դ����
	*/
	void getSkeletonModelAppearance(SkeletonModelAppearance& appearance);

	/**
	* @brief
	*		����LightMap��Ϣ
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
	H3DI::ISkeletonModel*		m_skeleton_model; ///< ������Ӧ�Ķ���

	std::map<int, std::vector<AttachItemInfo>>		m_attach_item_info;
	bool		m_is_all_attach_item_visible;

	std::string		m_action_id[ANIMATION_CHANNEL_COUNT]; ///< ��ǰ����id
	bool			m_action_loop[ANIMATION_CHANNEL_COUNT]; ///< ��ǰ�����Ƿ���loop
	Operation*		m_action_op;

	std::string		m_next_action_id[ANIMATION_CHANNEL_COUNT]; ///< ��һ������id
	bool			m_next_action_loop[ANIMATION_CHANNEL_COUNT]; ///< ��ǰ�����Ƿ���loop
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
