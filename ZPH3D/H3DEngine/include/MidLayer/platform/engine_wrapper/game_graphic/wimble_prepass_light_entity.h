/**
* @file wimble_prepass_light_entity.h
* ����ļ�����Ҫ�����ǣ�
* 
* @author �̳�(Chengchen@h3d.com.cn)
* 
* ������־
* ----------------------------------------------------
* ver 1.0.0
*       by �̳�2010.5.21
*/

#ifndef _WIMBLE_PREPASS_LIGHT_ENTITY_H_
#define _WIMBLE_PREPASS_LIGHT_ENTITY_H_

#include "game_graphic_header.h"

class CFollowingBind;

class CWimblePrePassLightEntity
	:public CPrePassLightEntity
{
public:
	CWimblePrePassLightEntity(CGraphicsResMng* res_mng, const std::string& name, H3DI::IPrePassLight* light);
	virtual ~CWimblePrePassLightEntity();

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
	*		��ȡ�ƹ���ڽǶ�
	* @return
	*		�ƹ���ڽǶȣ���λΪ�Ƕ�
	*/
	virtual float inner_angle() const;

	/**
	* @brief
	*		���õƹ���ڽǶ�
	* @param
	*		light_inner_angle����IN��Ҫ���õĵƹ���ڽǶ�
	*/
	virtual void set_inner_angle(float light_inner_angle);

	/**
	* @brief
	*		��ȡ�ƹ����Ƕ�
	* @return
	*		�ƹ����Ƕȣ���λΪ�Ƕ�
	*/
	virtual float outter_angle() const;
	
	/**
	* @brief
	*		���õƹ����Ƕ�
	* @param
	*		light_outter_angle����IN���ƹ����Ƕ�
	*/
	virtual void set_outter_angle(float light_outter_angle);

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
	*		����Ͷ�����������
	* @param
	*		texturename����IN���ļ���
	*/
	virtual void set_projected_texture(const std::string& texturename);

	/**
	* @brief
	*		����Ͷ�������Ƿ����
	* @param
	*		enable����IN���Ƿ����
	*/
	virtual void	enable_projected_texture(bool enable);

	/**
	* @brief
	*		��������
	*/
	virtual void create_beam(H3DI::EH3DBEAMTYPE beam_type);

	/**
	* @brief
	*		ɾ������
	*/
	virtual void delete_beam();

	/**
	* @brief
	*		���ù�������
	* @param
	*
	*/
	virtual void set_beam_type(H3DI::EH3DBEAMTYPE beam_type);

	/**
	* @brief
	*		�ж��Ƿ񴴽��˹���	
	*/
	virtual bool is_beam_created();

	/**
	* @brief
	*		���ù����ı��ؾ���
	* @param
	*		mat����IN��Ҫ���õľ���
	*/
	virtual void set_beam_local_matrix(const H3DMat4& mat);

	/**
	*  @brief
	*		��ȡ�����ı��ؾ���
	* @return
	*		�����ı��ؾ���
	*/
	virtual const H3DMat4& get_beam_local_matrix() const;

	/**
	* @brief
	*		���ù����Ŀɼ���
	* @param
	*		beam_visible����IN���趨�����Ƿ�ɼ�
	*/
	virtual void set_beam_visible(bool beam_visible);

	/**
	* @brief
	*		�����Ƿ�ɼ�
	* @return
	*		TRUE�������ɼ�
	*		FALSE���������ɼ�
	*/
	virtual bool is_beam_visible() const;

	/**
	* @brief
	*		���ù����������̶�
	* @param
	*		bright_ness����IN�������������̶�
	*/
	virtual void set_beam_brightness(float bright_ness);

	/**
	* @brief
	*		��ȡ����������
	* @return
	*		����������
	*/
	virtual float get_beam_brightness() const;

	/**
	* @brief
	*		���ù������ھ�
	*/
	virtual void set_beam_inner_radius(float t);

	/**
	* @brief
	*		���ú��������ģ����
	*/
	virtual void set_beam_blur_coeff(float t);

	/**
	* @brief
	*		���ù����Ľ���Χ
	*/
	virtual void set_beam_near_range(float range);

	/**
	* @brief
	*		��ȡ��������Χ
	*/
	virtual float get_beam_near_range();

	/**
	* @brief
	*		���ù���Զ��Χ
	*/
	virtual void set_beam_far_range(float range);

	/**
	* @brief
	*		��ȡ����Զ��Χ
	*/
	virtual float get_beam_far_range();

	/**
	* @brief
	*		���ù����ڽǶ�
	*/
	virtual void set_beam_inner_angle(float inner);
	/**
	* @brief
	*		��ȡ�������ڽǶ�
	*/
	virtual float get_beam_inner_angle();
	/**
	* @brief
	*		���ù�������Ƕ�
	*/
	virtual void set_beam_outter_angle(float outter);
	/**
	* @brief
	*		��ȡ��������Ƕ�
	*/
	virtual float get_beam_outter_angle();

	/**
	* @brief
	*		���¶���
	* @param
	*		intervalTime����IN�������ϴθ��µ��¼����
	*/
	virtual void	update(float interval);

	/// �������߹ر�����ƹ�
	virtual void enable_engine_light(bool enable_light);

	virtual void switch_to_lod(int lod, bool is_sync = true);

protected:
	/// �������߹رչ���
	virtual void enable_engine_beam(bool enable_beam);
	//�������������ɼ�
	virtual void update_engine_beam_visible();
	//���������Ŀɼ���
	virtual bool is_engine_beam_visible();

	//��������ƹ�Ч��
	virtual void update_engine_light_enable();
	//���Ƶƹ���
	virtual void draw_light_location_line(CRenderSystem& render_sys);

private:
	H3DMat4			m_beam_location_matrix; ///< �����ı��ؾ���
	float			m_beam_brightness; ///< ����������
	bool			m_is_beam_visible; ///< �����Ƿ�ɼ�
	bool			m_is_beam_created; ///< ��������
	float			m_beam_inner_angle;	//< �����ڽ�
	float			m_beam_out_angle;	///< �������

	bool			m_is_project_texture;
	std::string		m_project_texture_name; ///< ͶӰ��ͼ����

	bool			m_is_engine_beam_visible; ///< �����Ĺ����Ƿ�ɼ�
	float			m_beam_inner_radius;
	H3DI::EH3DBEAMTYPE	m_lod0_beam_type;	///<���ڼ�¼LOD0ʱbeam type����lod�л�ʱ����;
private:
	FORBIDDEN_EVIL_CONSTRUCTOR(CWimblePrePassLightEntity);
};

#endif //_WIMBLE_PREPASS_LIGHT_ENTITY_H_
