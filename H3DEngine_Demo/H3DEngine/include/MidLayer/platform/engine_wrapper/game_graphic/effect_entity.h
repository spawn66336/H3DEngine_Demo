/**
* @file effect_entity.h
* ����ļ�����Ҫ�����ǣ�
* 
* @author �̳�(Chengchen@h3d.com.cn)
* 
* ������־
* ----------------------------------------------------
* ver 1.0.0
*       by �̳�2010.1.29
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
	*		����ý��
	*/
	virtual void play(void) ;

	/**
	* @brief
	*		ֹͣ����ý��
	*/
	virtual void stop(void) ;

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
	virtual int		type(void) const;

	/**
	* @brief
	*		��ȡ��Ӧ��ISpecialEffect����
	*/
	ISpecialEffect* get_IEffect() const;

	/**
	* @brief
	*		������Ч��͸����
	* @param
	*		transparence����IN����Ҫ���õ�͸���ȣ���Χ0��1
	*/
	void set_transparence(float new_transparence);

	/**
	* @brief
	*		��ȡ��Ч��͸����
	* @return
	*		��Ч��͸���ȣ���С0����1
	*/
	float transparence();

	/**
	* @brief
	*		�޸���Դ
	* @param
	*		res_name����IN��Ҫ�޸ĵ���Դ·��
	* @return
	*		true���޸ĳɹ�
	*		false���޸�ʧ��
	*/
	virtual bool change_resource(const std::string& res_name);

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
	*		������Ч�Ƿ�Ͷ����Ӱ
	*/
	virtual void enable_cast_shadow(bool cast_shadow);

	/**
	* @brief
	*		������Ч�Ƿ�Ͷ����Ӱ
	*/
	virtual bool is_cast_shadow_enable() const;

	/**
	@brief ����λ��ƫ��
	*/
	virtual void setPositionOffset(float x,float y,float z);

	/**
	@brief ���λ��ƫ��
	*/
	virtual void getPositionOffset(float& x,float& y,float& z);

	/**
	@brief ������Ч�Ƕȣ�����ĳЩ��Ч�������������
	*/
	void setCameraAngle(CameraInfo& camInfo , CSkeletonModelEntity* bindTarget );
	/**
	@brief �����Ƿ�ʼ�����������ת
	*/
	void enableCameraAngleAdapt(bool enable){m_cameraAngleAdapt = enable;};
	/**
	@brief �G���Ƿ�ʼ���������ѡ��
	*/
	bool isCameraAngleAdapt(){return m_cameraAngleAdapt;};
	/**
	* @brief
	*		���¿�ʼ
	*/
	virtual void reset(void) ;

	virtual void set_visible(bool visible);
	virtual void set_external_visible(bool visible);
	virtual void update_engine_entity_visible();

	virtual void switch_to_lod(int lod, bool is_sync = true);

	/**
	* @brief
	*		���ô���ЧΪ��ɫ�����Ч
	*/
	void set_is_actor_effect(bool is);

	/**
	* @brief
	*		�õ���Ч���ܳ��ȣ�����TIME_MAX��ʾѭ����Ч
	*/
	float  get_animation_length();

	/**
	* @brief
	*		�����Դ����
	*/
	void getEffectAppearance(EffectAppearance& appearance);

	virtual void UnloadResources();

private:
	/**
	* @brief
	*		������������
	* @param
	*		cur_update_time����IN�����θ��µ�ʱ��
	*		intervalTime����IN�������ϴθ��µ��¼����
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

	ISpecialEffect* m_effect; ///< ������Ӧ��ISpecialEffect����

	float m_transparence; ///< ��Ч��͸����
	bool  m_cast_shadow_enable; ///< ����ʵʱ��Ӱ

	std::map<std::string, ISpecialEffect*> m_effect_pool; ///< ��ʱʵ�ֵģ�Ŀ��������������Ч

	float m_posOffsetX;///< �������ƫ��
	float m_posOffsetY;///< �������ƫ��
	float m_posOffsetZ;///< �������ƫ��

	bool m_cameraAngleAdapt;

	//Cache������
	int		m_state;
	bool		m_is_actor_effect;

	CModelEntity* m_parent_model;

private:
	FORBIDDEN_EVIL_CONSTRUCTOR(CEffectEntity);
};


#endif //_EFFECT_ENTITY_H_
