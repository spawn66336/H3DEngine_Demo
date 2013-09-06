/**
* @file prepass_light_entity.h
* ����ļ�����Ҫ�����ǣ�
* 
* @author �̳�(Chengchen@h3d.com.cn)
* 
* ������־
* ----------------------------------------------------
* ver 1.0.0
*       by �̳�2010.5.21
*/

#ifndef _PREPASS_LIGHT_ENTITY_H_
#define _PREPASS_LIGHT_ENTITY_H_

#include "game_graphic_header.h"

//��߲����ṹ
struct GlareProperty
{
	float width; 	//�ߴ�
	float height;
	H3DColor color; 	//��ɫ
	std::string texture_name;	//��ͼ
	std::string shader_lib; 	//����lib
	std::string shader_instance_name;	//��9��ʵ����
	float glare_distance;	//����Դ�ľ���
	float glare_rotation_velocity; 	//��ת�ٶ�
	bool is_visible; 	//�Ƿ�ɼ�
	bool is_rotate_with_camera; 	//�Ƿ�����������ת
	bool is_view_camera;	//�Ƿ����������

	GlareProperty()
	{
		width = height = 1.0f;
		color = H3DColor(1.0f, 1.0f, 1.0f, 1.0f);
		glare_distance = 0.0f;
		glare_rotation_velocity = 5.0f;
		is_visible = true;
		is_rotate_with_camera = true;
		is_view_camera = true;
	}


	GlareProperty(const H3DI::tGlareProperty& glare_property)
	{
		width = glare_property.m_width;
		height = glare_property.m_height;
		color = H3DColor(glare_property.m_color);
		texture_name = (NULL != glare_property.m_pTextureName) ? glare_property.m_pTextureName : "";
		shader_lib = (NULL != glare_property.m_pShaderLib) ? glare_property.m_pShaderLib : "";
		shader_instance_name = (NULL != glare_property.m_pShaderInstanceName) ? glare_property.m_pShaderInstanceName : "";
		glare_distance = glare_property.m_disToLight;
		glare_rotation_velocity = glare_property.m_glareRotVel;
		is_visible = glare_property.m_bVisable;
		is_rotate_with_camera = glare_property.m_bRotateWithCamera;
		is_view_camera = glare_property.m_bViewCamera;
	}

	operator H3DI::tGlareProperty()
	{
		H3DI::tGlareProperty glare_property;
		glare_property.m_width = width;
		glare_property.m_height = height;
		for (int i = 0; i < 4; i++)
		{
			glare_property.m_color[i] = color.getColor()[i];
		}
		glare_property.m_pTextureName= (texture_name.length() != 0) ? texture_name.c_str() : NULL;
		glare_property.m_pShaderLib= (shader_lib.length() != 0) ? shader_lib.c_str() : NULL;
		glare_property.m_pShaderInstanceName= (shader_instance_name.length() != 0) ? shader_instance_name.c_str() : NULL;
		glare_property.m_disToLight = glare_distance;
		glare_property.m_glareRotVel = glare_rotation_velocity;
		glare_property.m_bVisable = is_visible;
		glare_property.m_bRotateWithCamera = is_rotate_with_camera;
		glare_property.m_bViewCamera = is_view_camera;

		return glare_property;
	}
};


class CPrePassLightEntity
	: public C3DEntity
{
public:
	CPrePassLightEntity(CGraphicsResMng* res_mng, const std::string& name, H3DI::IPrePassLight* light);
	virtual ~CPrePassLightEntity();

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
	*		��ȡ�ƹ���ɫ
	* @return
	*		�ƹ���ɫ
	*/
	H3DColor color() const;

	/**
	* @brief
	*		���õƹ���ɫ
	* @param
	*		light_color����IN��Ҫ���õĵƹ���ɫ
	*/
	void set_color(const H3DColor& light_color);

	/**
	* @brief
	*		��ȡ�ƹ�ǿ��
	* @return
	*		�ƹ�ǿ��
	*/
	float intensity() const;
	
	/**
	* @brief
	*		���õƹ�ǿ��
	* @param
	*		light_intensity����IN��Ҫ���õĵƹ�ǿ��
	*/
	virtual void set_intensity(float light_intensity);

	/**
	* @brief
	*		�Ƿ�����ƹ���Ӱ
	* @return
	*		�Ƿ�����ƹ���Ӱ
	*/
	bool is_shadow_enable() const;

	/**
	* @brief
	*		�����Ƿ�����ƹ���Ӱ
	* @param
	*		enable_shadow����IN���Ƿ�����ƹ���Ӱ
	*/
	void set_shadow_enable(bool enable_shadow);

	/**
	* @brief
	*		��ȡ�ƹ���Ӱ��ɫ
	* @return
	*		�ƹ���Ӱ��ɫ
	*/
	H3DColor shadow_color() const;
	
	/**
	* @brief
	*		���õƹ���Ӱ��ɫ
	* @param
	*		light_shadow_color����IN��Ҫ���õĵƹ���Ӱ��ɫ
	*/
	void set_shadow_color(const H3DColor& light_shadow_color);

	/**
	* @brief
	*		��ȡ�����װ�ĵƹ����
	*/
	H3DI::IPrePassLight*  get_IPrePassLight() const;

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
	*		��ȡ�ƹ���ڰ뾶
	* @return
	*		�ƹ���ڰ뾶
	*/
	float near_range() const;

	/**
	* @brief
	*		���õƹ���ڰ뾶
	* @param
	*		light_inner_radius����IN��Ҫ���õĵƹ���ڰ뾶
	*/
	void set_near_range(float light_inner_radius);

	/**
	* @brief
	*		��ȡ�ƹ����뾶
	* @return
	*		�ƹ����뾶
	*/
	float far_range() const;

	/**
	* @brief
	*		���õƹ����뾶
	* @param
	*		light_outter_radius����IN���ƹ����뾶
	*/
	void set_far_range(float light_outter_radius);

	/**
	* @brief
	*		���õƹ�Ӱ�췶Χ
	*/
	virtual void set_affect_param(H3DI::LightAffectParam t);

	/**
	* @brief
	*		��ȡ�ƹ�Ӱ�췶Χ
	*/
	virtual H3DI::LightAffectParam get_affect_param();

	/**
	* @brief
	*		�����Ƿ񻭹�����Χ
	*/
	void set_beam_line_draw(bool is_draw);
	/**
	* @brief
	*		�õ��Ƿ񻭹�����Χ
	*/
	bool get_beam_line_draw();

	/**
	* @brief
	*		����һ�����
	* @param
	*		glare_property����IN���������
	*/
	virtual int create_glare(const GlareProperty& glare_property);

	/**
	* @brief
	*		ɾ�����
	* @param
	*		glare_index����IN���������
	*/
	//virtual void delete_galare(int glare_index);

	/**
	* @brief
	*		�жϹ���Ƿ񴴽�
	* @param
	*		glare_index����IN���������
	*/
	virtual bool is_glare_created(int glare_index);

	/**
	* @brief
	*		���ù������
	* @param
	*		glare_index����IN���������
	*		glare_property����IN���������
	*/
	virtual void set_glare_property(int glare_index, const GlareProperty& glare_property);

	/**
	* @brief
	*		��ȡ�������
	* @param
	*		glare_index����IN���������
	* @return
	*
	*/
	virtual GlareProperty get_glare_property(int glare_index);

	/**
	* @brief
	*		��ȡ���id
	* @param
	*		index����IN���������
	* @return
	*		���id
	*/
	virtual int get_glare_index(int index);

	/**
	* @brief
	*		�жϹ�Դ�Ƿ�������
	* @return
	*		�����ã�TRUE
	*		�������ã�FALSE
	*/
	virtual bool is_light_enable() const;

	/**
	* @brief
	*		���õƹ��Ƿ�������
	* @param
	*		light_enable����IN�����õƹ��Ƿ�������
	*/
	virtual void set_light_enable(bool light_enable);

	/**
	* @brief
	*		���õƹ��Ƿ�������
	* @param
	*		light_enable����IN�����õƹ��Ƿ�������
	*/
	virtual void set_entity_light_enable(bool light_enable);

	/**
	* @brief
	*		�жϹ�Դ�Ƿ�������
	* @return
	*		�����ã�TRUE
	*		�������ã�FALSE
	*/
	virtual bool is_entity_light_enable() const;

	/**
	* @brief
	*		��õƹ�����Լ��ϣ��Ż��ã�
	* @return
	*		�ƹ�����Լ��ϣ�ֻ������ƹ⣨�Լ����������ﹲͬ�����õĵƹ⣩��Ч
	*/
	LightCachedProperty* get_light_property() { return &m_light_cached_property; }

	/**
	* @brief
	*		���õƹ�Ŀɼ���״̬
	*/
	void set_light_visible_status(bool visible_status);

	/**
	* @brief
	*		��ȡ�ƹ�Ŀɼ���״̬
	*/
	bool get_light_visible_status() const;

	/**
	* @brief
	*		��ȡ�ƹ�����ȼ�
	*/
	int get_priority() const;

	/**
	* @brief
	*		���õƹ�����ȼ�
	*/
	void set_priority(int light_priority);

	/**
	* @brief
	*		����/�ر������
	*/
	virtual void set_light_shaft_enable(bool enable);

	/**
	* @brief
	*		�ж�������Ƿ�ʼ
	*/
	virtual bool is_light_shaft_enable();

	/**
	* @brief
	*		�����������ɫ
	*/
	virtual void set_light_shaft_color(const H3DColor& shaft_color);

	/**
	* @brief
	*		��ȡ�������ɫ
	*/
	virtual H3DColor get_light_shaft_color() const;

	/**
	* @brief
	*		���������ǿ��
	*/
	virtual void set_light_shaft_intensity(float shaft_intensity);
	/**
	* @brief
	*		��ȡ�����ǿ��
	*/
	virtual float get_light_shaft_intensity();

	/**
	* @brief
	*		��������ֵⷧ
	*/
	virtual void set_light_shaft_threshold(float threshold);

	/**
	* @brief
	*		��ȡ����ֵⷧ
	*/
	virtual float get_light_shaft_threshold();

	/**
	* @brief
	*		������������
	*/
	virtual void set_light_shaft_depth(float depth);

	/**
	* @brief
	*		��ȡ��������
	*/
	virtual float get_light_shaft_depth();

	/**
	* @brief
	*		�趨�ƹ�Ч������
	*/
	virtual void turn_on_light(bool enable_light);

	/**
	* @brief
	*		�ƹ���̨Ч���Ƿ���
	*/
	virtual bool is_light_turn_on();

	/**
	* @brief
	*		�趨�����Ƿ�ɼ�
	* @param
	*		is_visible����IN���趨�����Ƿ�ɼ�
	*/
	virtual void set_visible(bool visible);

	/**
	* @brief
	*		���ÿɿ�ʵ���״̬�Ƿ����
	* @param
	*		is_eanbel����IN���ɿ�ʵ���״̬���ñ�־
	*/
	virtual void set_enable(bool enable) ;

	/**
	* @brief
	*		����Ӳ��ѡ����Ⱦ
	* @param
	*		renderSystem����IN������ϵͳ
	*		sel_line: �Ƿ�������Ⱦ
	* @return
	*		true��������Ⱦ
	*		false����������Ⱦ
	*/
	virtual bool pushHardwareSelect(CRenderSystem& render_system, bool sel_line);
	/**
	* @brief
	*		���Ʊ�ʾ�ƹ�λ�õ���
	* @param
	*		renderSystem����IN������ϵͳ
	*/
	void draw_light_select_line(CRenderSystem& render_system);
	/**
	* @brief
	*		����ƹ��Ƿ����
	*/
	virtual bool is_engine_light_enable();

	/// �������߹ر�����ƹ�
	virtual void enable_engine_light(bool enable_light);

	virtual void switch_to_lod(int lod,bool is_sync = true);
	/**
	* @brief
	*		�ⲿ��Ϊ��Ҫ�򿪵ƹ�
	*/
	virtual void set_external_visible(bool on);

	/**
	* @brief
	*		�Ƿ��ⲿ��Ϊ��Ҫ����
	*/
	virtual bool is_external_visible() const;

	virtual void set_world_location_matrix(const H3DMat4& location);

	virtual void enable_fake_light_spot(bool bEnabled);
	virtual bool is_fake_light_spot_enable(){return m_is_fake_light_enabled;};
	void record_fake_light_spot(bool bEnable);
	bool get_record_fake_light_spot(){return m_record_fake_light_spot;};

	//���úͻ�ȡlightmap��Ӱָ�������༭��ʹ��
	virtual void set_lightmap_shadow_index(float shadow_index){m_lightmap_shadow_index = shadow_index;};
	virtual float get_lightmap_shadow_index(){return m_lightmap_shadow_index;};

	//���úͻ�ȡlightmap��Դ�뾶 �����༭��ʹ��
	virtual void set_lightmap_radius(float lightmap_radius){m_lightmap_radius = lightmap_radius;};
	virtual float get_lightmap_radius(){return m_lightmap_radius;};

	//���õƹ��GUID����
	virtual void set_guid(const char* guid)
	{
		m_guid = guid;
		m_light->SetGUID(m_guid.c_str());
	}

private:
	typedef std::vector<std::pair<int, GlareProperty> > GlareList;



protected:
	//��������ƹ�Ч��
	virtual void update_engine_light_enable();
	//���������ߵ�����
	virtual void set_engine_glare_property(int index, const H3DI::tGlareProperty& glare_property);
	//���Ƶƹ���
	virtual void draw_light_location_line(CRenderSystem& render_sys) = 0;

protected:

	GlareList	m_created_glares; ///< �����Ĺ���б�

	bool			m_is_light_enable; ///< �����ƹ�
	bool			m_light_visible_status; ///< �ƹ�ɼ�״̬
	bool			m_is_light_turn_on; ///< �ƹ�Ч���Ƿ񱻴�
	bool			m_draw_beam_line;///< ��ķ�Χ 
	bool			m_is_engine_light_enable; ///< ����ĵƹ��Ƿ��� 
	//bool			m_is_external_visible; /// �ⲿ�Ƿ��趨�򿪵ƹ�
	bool			m_is_entity_light_enable; /// �ƹ�״̬
	
	bool			m_enable_shaft; /// �Ƿ��������

	/// �ƹ����ԣ���Ϊһ���ṹ��
	LightCachedProperty m_light_cached_property;

	// �����滻Ϊ m_light_cached_property
	// H3DColor		m_color; /// �ƹ���ɫ
	// H3DColor		m_shader_color; /// ��Ӱ��ɫ
	// bool			m_enable_shadow; /// �Ƿ�����ƹ�����Ӱ
	// int				m_priority; /// �ƹ����ȼ�
	// float			m_intensity; /// �ƹ�ǿ��
	// float			m_near_range; /// �ƹ����Χ
	// float			m_far_range; /// �ƹ�Զ��Χ
	// H3DI::LightAffectParam m_affect_param; /// �ƹ�Ӱ�췶Χ

	H3DColor		m_shaft_color; /// �������ɫ
	float			m_shaft_intensity; /// �����ǿ��
	float			m_shaft_threshold; /// ����ֵⷧ
	float			m_shaft_depth; /// ��������

	
	H3DI::IPrePassLight* m_light; ///< ��Ӧ�������ƹ�ʵ��


	bool			m_is_fake_light_enabled;	//���ڼ�¼����ƹ��Ƿ�������fakelight��
	bool			m_record_fake_light_spot;	//���ڼ�¼�ⲿ��fakelightspot�����ã�

	//���༭��ʹ�á�
	float			m_lightmap_shadow_index;	
	float			m_lightmap_radius;

private:
	FORBIDDEN_EVIL_CONSTRUCTOR(CPrePassLightEntity);
};

#endif //_PREPASS_LIGHT_ENTITY_H_
