/**
* @file actor_entity.h
* ����ļ�����Ҫ�����ǣ�
* 
* @author �̳�(Chengchen@h3d.com.cn)
* 
* ������־
* ----------------------------------------------------
* ver 1.0.0
*       by �̳�2010.1.29
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
	*		�������岿��
	* @param
	*		body_part����IN���趨Ҫ���µ����岿λ
	*		name����IN����Ӧ����Դ����
	*/
	virtual void set_body_part(int body_part, const std::string& name, bool is_sync = true);

	//����
	virtual bool change_media_object(H3DI::IMoveObject* object, bool use_preproperty);

	/*
	*@breif ���м���õ�setBodyPart
	*@param body_group ��Դ�������У��÷ֺŷָ�
	*/
	virtual void set_body_part_groups(const std::string& body_group);

	/*
	*@brief ����Ƥ����ɫ
	*@param skin_color Ƥ����ɫ��float[4]����ʾRGBA
	*/
	virtual void set_skin_color(float* skin_colors);

	/*
	*@breif ���м���õĸı��Ա�Ľӿ�
	*@param is_male Ҫ�ĳɵ��Ա������true��Ϊ��
	*/
	virtual void change_sex(bool is_male);

	/**
	* @brief
	*		��ȡ��Ӧ��IActor����
	*/
	H3DI::IActor* get_IActor() const;

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
	*		��ȡ�������
	* @return
	*		�������
	*/
	virtual int	type(void) const;

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
	@brief ���ö��������ٶȣ�����BPM����
	@return speed���ٶ�
	*/
	void setActionSpeed(float speed)
	{
		cur_action_speed = speed;
	};

	virtual void setColorFactor(float* color, int size);

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
	*		���ùҼ��Ͷ���
	*/
	virtual void reset();

	/**
	* @brief
	*		�����Ƿ񽫹Ҽ���Ϣ���µ����棬���Ĭ��ȡ�������ֵĹҼ��붯��
	*/
	virtual void set_adornment_surpress(bool surpress);

	/**
	* @brief �����Ƿ�ָ���ĹҼ����õ�������
	*		
	*/
	virtual void set_adornment_surpress(bool surpress, H3DI::EActorAdornmentPosition ad_position);

	/**
	* @brief ��ʾ/����ָ���ĹҼ�
	*		
	*/
	void set_adornment_visible(bool visible, H3DI::EActorAdornmentPosition ad_position);

	/**
	* @brief
	*		�����Դ����
	*/
	void getAvatarAppearance(AvatarAppearance& appearance);

	/**
	* @brief
	*		���¹Ҽ�
	* @param
	*		interval����IN�������ϴθ��µ��¼��������λ����
	*/
	void update_adornment(float interval);

	// ��/�رո߸�Ьƫ�� 
	// Ĭ��Ϊ��
	void	EnableHeelOffset(bool bEable);

	//���������Ƿ�ʹ�ð�Χ��. ����ֵ1��ʾ�ɹ���С��0���ɹ�
	int setUseCustomBoundBox(bool b);

	//! ���ùҼ�����
	// ����adornmentposition���Ҽ���λ
	// ����actionName���Ҽ�CHR�еĶ�������
	// ����transitionTime�������ж��������趯���Ĺ���ʱ�䣬��λΪ��
	// ����ֵ������ɹ������˹Ҽ����κ�һ��CHR�Ķ���������true����������е�CHR�ж��Ҳ����ö��������߶�Ӧ�Ĳ�λû�йҼ�������false
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
	H3DI::IActor* m_avatar;   //������Ӧ���赸�߶���
	bool			m_is_male; /// �Ƿ�Ϊ����

	float cur_action_speed;		///<  �������ŵ��ٶ�
	float action_time;			///<  ��ǰ����ʱ��

	std::string		m_adorment[H3DI::ACTOR_ADORNMENT_NUM];
	Operation*		m_ad_op[H3DI::ACTOR_ADORNMENT_NUM];

	std::string		m_body_part[H3DI::BODYPART_NUM];
	Operation*		m_bpt_op[H3DI::BODYPART_NUM];

	bool m_adornment_surpress;

private:
	FORBIDDEN_EVIL_CONSTRUCTOR(AvatarEntity);
};

#endif //_AVATAR_ENTITY_H_
