/*
*@file  avatar_skeleton_model_entity.h
*@breif ʵ�ֶ�������IAvatarSkeletonModel�ķ�װ
*@author mozisi
*/
#ifndef __AVATAR_SKELETON_MODEL_ENTITY_H__
#define __AVATAR_SKELETON_MODEL_ENTITY_H__
#include "game_graphic_header.h"
#include "skeleton_model_entity.h"

/*
*@breif �Զ�������IAvatarSkeletonModel�ķ�װ
*/
class CAvatarSkeletonModelEntity : public CSkeletonModelEntity
{
	friend class AdornmentOperation;
	friend class BodypartOperation;
public:
	CAvatarSkeletonModelEntity(CGraphicsResMng* res_mng, const std::string& name
		, bool is_male, int lod);

		/*
	*@breif ��ȡ��Ӧ���������
	*@return ��Ӧ���������
	*/
	H3DI::IAvatarSkeletonModel* getIAvatarSkeletonModel() const;

	/**
	* @brief
	*		��ȡ�������
	* @return
	*		�������
	*/
	virtual int type(void) const;

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
	* @brief
	*		�������岿��
	* @param
	*		body_part����IN���趨Ҫ���µ����岿λ
	*		name����IN����Ӧ����Դ����
	*/
	virtual void set_body_part(int body_part, const std::string& name, bool is_sync = true);

	/*
	*@breif ���ùҼ�
	*@param ad_position �Ҽ�λ��
	*@param ad_file �Ҽ���Դ·����������ָ����߿մ������ʾж�ظ�λ�õĹҼ�
	*@return �Ƿ�ɹ�
	*/
	virtual void setAdornment(int ad_position, const char* ad_file
		, const char* action = NULL, bool is_sync = true);

	/**
	* @brief
	*		�����Ƿ񽫹Ҽ���Ϣ���µ����棬���Ĭ��ȡ�������ֵĹҼ��붯��
	*/
	virtual void set_adornment_surpress(bool surpress);

	/**
	* @brief �����Ƿ�ָ���ĹҼ����õ�������
	*		
	*/
	virtual void set_adornment_surpress(bool surpress, H3DI::EPetAdornmentPosition ad_position);

	/**
	@brief ���ö���
	@param channel_id��ͨ���� action���������� loop���Ƿ�ѭ������
	*/
	void set_action(unsigned int channel_id, const char* action, bool loop, bool is_sync = true);

		/**
	@brief ������һ����
	@param channel_id��ͨ���� action���������� loop���Ƿ�ѭ������
	*/
	void  set_next_action(unsigned int channel_id, const char* action, bool loop, bool is_sync = true);

	/**
	* @brief
	*		���ùҼ��Ͷ���
	*/
	virtual void reset();

	/**
	* @brief
	*		�����Դ����
	*/
	void getAvatarSkeletonModelAppearance(AvatarSkeletonAppearance& appearance);

	/**
	* @brief
	*		���¹Ҽ�
	* @param
	*		interval����IN�������ϴθ��µ��¼��������λ����
	*/
	void update_adornment(float interval);

	//! ���ùҼ�����
	// ����adornmentposition���Ҽ���λ
	// ����actionName���Ҽ�CHR�еĶ�������
	// ����transitionTime�������ж��������趯���Ĺ���ʱ�䣬��λΪ��
	// ����ֵ������ɹ������˹Ҽ����κ�һ��CHR�Ķ���������true����������е�CHR�ж��Ҳ����ö��������߶�Ӧ�Ĳ�λû�йҼ�������false
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
	///< ������Ӧ�Ķ���
	H3DI::IAvatarSkeletonModel	*m_avatar_skeleton_model;
	bool			m_is_male; /// �Ƿ�Ϊ����

	std::string		m_adorment[H3DI::PET_ADORNMENT_NUM];
	Operation*		m_ad_op[H3DI::PET_ADORNMENT_NUM];

	std::string		m_body_part[H3DI::PET_BODYPART_NUM];
	Operation*		m_bpt_op[H3DI::PET_BODYPART_NUM];

	bool m_adornment_surpress;

private:
	FORBIDDEN_EVIL_CONSTRUCTOR(CAvatarSkeletonModelEntity);
};
#endif
