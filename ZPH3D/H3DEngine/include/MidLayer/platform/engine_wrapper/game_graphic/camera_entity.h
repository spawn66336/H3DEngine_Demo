/**
* @file camera_entity.h
* ����ļ�����Ҫ�����ǣ�
* 
* @author �̳�(Chengchen@h3d.com.cn)
* 
* ������־
* ----------------------------------------------------
* ver 1.0.0
*       by �̳�2010.1.25
*/

#ifndef _CAMERA_ENTITY_H_
#define _CAMERA_ENTITY_H_

#include "game_graphic_header.h"
#include "i_target_position.h"


//�ο���λ�Ƽ�����
class COffsetCalculator
{
public:
	COffsetCalculator();
	virtual ~COffsetCalculator(){}
	
	/**
	* @brief
	*		���ƫ�Ƶ�
	*/
	virtual bool push_offset(long interval, const H3DVec3& pos);
	/**
	* @brief
	*		�������ƫ�Ƶ�
	*/
	void clear_offsets();

	/**
	* @brief
	*		��ȡƫ��
	*/
	virtual H3DVec3 get_offset();

	/**
	* @brief
	*		������С�������
	*/
	void set_min_interval(long interval);

	/**
	* @brief
	*		��������������Ŀ
	*/
	void set_max_count(long count);

protected:
	typedef std::list<H3DVec3> PositionList;

	PositionList m_offsetes; ///< �ο���λ�ü���
	long	m_min_interval; ///< ��С�������
	long	m_max_node_count; ///< ����������Ŀ
};


//���ڲο���ƽ��λ�Ƽ�����
class CAverageOffsetCalculator
	:public COffsetCalculator
{
public:
	virtual ~CAverageOffsetCalculator(){}
	/**
	* @brief
	*		���ƫ�Ƶ�
	*/
	virtual bool push_offset(long interval, const H3DVec3& pos);

	/**
	* @brief
	*		��ȡƽ�����˵�
	*/
	virtual H3DVec3 get_offset();

private:
	long m_cur_time;  ///< ��ǰ����ʱ��
	long m_last_time; ///< �ϴθ���ʱ��
};

//���ڲο���ƽ��λ�Ƽ�����
class CFirstCuttingOffsetCalculator
	:public COffsetCalculator
{
public:
	virtual ~CFirstCuttingOffsetCalculator(){}
	/**
	* @brief
	*		���ƫ�Ƶ�
	*/
	virtual bool push_offset(long interval, const H3DVec3& pos);

	/**
	* @brief
	*		��ȡƽ�����˵�
	*/
	virtual H3DVec3 get_offset();
};

class CameraPositionControl
{
	COffsetCalculator* m_offset_calculator;
public:
	CameraPositionControl();
	~CameraPositionControl();

	void setOffsetType(SmoothType st);

	void clear_record();
	H3DVec3 update_camera_pos(long interval, const H3DVec3& campos, ITargetPosition* target_position);
	/**
	@brief ������С�������
	*/
	void setMinInterval(int interval);
	/**
	@brief ����OffsetCalculator����������Ŀ
	*/
	virtual void set_offset_calculator_max_count(int count);
};


class CCameraEntity
	:public C3DEntity
{
public:
	typedef std::map<std::string,H3DVec4> PostprocessParamCollection;
	typedef std::map<std::string, PostprocessParamCollection> PostprocessCollection;

	CCameraEntity(CGraphicsResMng* res_mng, const std::string& entity_name, H3DI::ICamera* camera);
	virtual ~CCameraEntity();

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
	*		��ȡ����ķ���
	* @return
	*		����ķ���
	*/
	virtual H3DVec3 direction(void) const ;
	/**
	* @brief
	*		�趨�����λ��
	* @param
	*		positon����IN�������λ��
	*/
	virtual void set_position(const H3DVec3& pos) ;
	/**
	* \brief
	* ������Ϸ���
	* 
	* \returns
	* ���Ϸ���
	* 
	*/
	virtual const H3DVec3& up_direction() const;

	/**
	* \brief
	* �������Ϸ���
	* 
	* \param up
	* ���Ϸ���
	* 
	*/
	virtual void set_up_direction(const H3DVec3& up);

	/**
	* \brief
	* ����ͷ�Ӿ�����
	* 
	* \param atPosition
	* ����ͷ�Ӿ�������λ��
	* 
	*/
	virtual void look_at(const H3DVec3& atPosition);

	/**
	* \brief
	* ����ͷ�Ӿ�ɫ����
	* 
	* \param eyePosition
	* �۾�λ��
	* 
	* \param atPosition
	* �۾�����Ŀ�ĵ�λ��
	* 
	* \param upDirection
	* ���ϵķ���
	* 
	*/
	virtual void look_at(const H3DVec3& eyePosition, const H3DVec3& atPosition, const H3DVec3& upDirection);

	/**
	* \brief
	* ����ǰ����ת
	* 
	* \param degree
	* ת�ĽǶ�(0-360)
	*/
	virtual void roll(float degree);

	/**
	* \brief
	* �����ҷ���ת
	* 
	* \param degree
	* ת�ĽǶ�(0-360)
	*/
	virtual void yaw(float degree);

	/**
	* \brief
	* �����Ϸ���ת
	* 
	* \param degree
	* ת�ĽǶ�(0-360)
	*/
	virtual void pitch(float degree);

	/**
	* \brief
	* ��������ת
	*
	* \param axis
	* ��ķ���
	*
	* \param degree
	* ת�ĽǶ�(0-360)
	*/
	virtual void rotate(const H3DVec3& axis, float degree);

	/**
	* \brief
	* �������ƶ�
	* 
	* \param step
	* �ƶ�����
	* 
	* ���ƶ�����Ϊ����ʱ�������ƶ�, ���������ƶ�
	*/
	virtual void move_up(float step);

	/**
	* \brief
	* �������ƶ�
	* 
	* \param step
	* �ƶ�����
	* 
	* ���ƶ�����Ϊ����ʱ�������ƶ�, ���������ƶ�
	*/    
	virtual void move_right(float step);

	/**
	* \brief
	* ��ǰ���ƶ�
	* 
	* \param step
	* �ƶ�����
	* 
	* ǰ���ƶ���ָ����ͷ�����ǰ��, ����ˮƽ���ǰ��,
	* ���ƶ�����Ϊ����ʱ����ǰ�ƶ�, ��������ƶ�
	*/    
	virtual void move_front(float step);

	/**
	* \brief
	* ���view matrix
	* 
	* \returns
	* view matrix
	*/
	virtual const H3DMat4& get_view_matrix() const;

	/**
	* \brief
	* ���ÿ��Ӿ���
	* 
	* \param viewMatrix
	* view matrix
	*/
	virtual void set_view_matrix(const H3DMat4& viewMatrix);

	/**
	* \brief
	* ���ͶӰ����
	* 
	* \returns
	* ͶӰ����
	*/
	virtual const H3DMat4& get_projection_matrix() const;

	/**
	* \brief
	* ����ͶӰ����
	* 
	* \param projectionMatrix
	* projection matrix
	*/
	virtual void set_projection_matrix(const H3DMat4& projectionMatrix);

	/**
	* \brief
	* ����ƽ�н���
	* 
	* \param fov
	* �Ž�
	* 
	* \param ratio
	* �����
	* 
	* \param fNear
	* ��ƽ��
	* 
	* \param fFar
	* Զƽ��
	* 
	*/
	virtual void set_frustum(float fov, float ratio, float fNear, float fFar);

	/**
	* \brief
	* ����ƽ�н���
	* 
	* \param fov
	* �Ž�
	* 
	* \param ratio
	* �����
	* 
	* \param fNear
	* ��ƽ��
	* 
	* \param fFar
	* Զƽ��
	* 
	*/
	virtual void get_frustum(float* fov, float* ratio, float* fNear, float* fFar);

	virtual void get_render_frustum(float* fov, float* ratio, float* fNear, float* fFar);
	virtual void set_render_frustum(float fov, float ratio, float fNear, float fFar);

	/**
	*@brief 
	*		��������Ƿ���ݿ⻧��Ļ����fov�Զ�����
	*/
	virtual void set_is_fov_auto_adjust(bool bAutoChange);
	virtual bool get_is_fov_auto_adjust();

	///**
	//* \brief
	//* ���ö�����
	//*
	//* \param direction
	//* ������
	//*/
	//virtual void set_direction(const H3DVec3& direction);

	/**
	* \brief
	* �������ͷ���ƽ�����
	* ��ƽ�����:ԭ��ָ�����͹͸���Ľ���,������ĳ�������¿�����������ϳ�������ʵ���������ǽ�ƽ�����ָ�������
	* 
	* \returns
	* ԭʼ��ƽ�����
	*/
	virtual float get_focalplane() const;

	virtual void set_focalplane(float focalplane);

	/**
	* \brief
	* �������ͷ���Χ
	* ���Χ:�ӽ�ƽ����������ǰ��������������һ�ξ��룬�������Χ�ڵ�����ɵ�����������Խ�������Χ������ɵ��񶼻���ĳ�̶ֳ��ϱ�ģ��
	* 
	* \returns
	* ԭʼ���Χ
	*/
	virtual float get_dof_range() const;
	virtual void set_dof_range(float dofrange);

	/**
	* \brief
	* �������ͷ��ɢϵ��
	* ��ɢϵ��:�����峬�����Χ��������ĳɵ���������Բ����ɢ�����Բ��֮Ϊ��ɢԲ��
	* ���Բ���������峬�����Χ�ľ���ı���������������Զ���������洦Բ�İ뾶�����ġ�
	* ��ֵΪ0ʱ����ر�
	* 
	* \returns
	* ��ɢϵ��
	*/
	virtual float get_confusion() const;
	virtual void set_confusion(float confusion);

	/**
	* @brief
	*		��ȡ��Ӧ��������ICamera����
	* @return
	*		����������ICamera����
	*/
	H3DI::ICamera* get_ICamera() const;

	/**
	* @brief
	*		��ȡĿ����λ��
	*/
	virtual H3DVec3 get_look_at() const;
	/**
	* @brief
	*		�趨�����ŷ���ǣ�ŷ���Ǻ�max���汣��һ��
	* @return
	*		����һ��h3cVec��ֵ���ֱ��ʾ��x��y��z���������ת����
	*/
	virtual void set_rotation(const H3DVec3& obj_rotation);

	/**
	* @brief
	*		�����������һ��������Ч�Ĳ��������������float����Ч������ֵ��ӵ�H3DVec4.x�Ϳ�����
	* @return
	*		
	*/
	virtual bool set_postprocess_param(const char* postprocess,  const char* param,const H3DVec4& paramvalue );
	/**
	* @brief
	*		������еĺ�����Ч�����ø�����
	* @return
	*		
	*/
	//virtual void apply_postprocess_param();
//private:
//	void position_changed(const H3DVec3& old_pos, const H3DVec3& new_pos);

	/**
	* @brief
	*		��ȡ����������Ϣ
	*/
	virtual const PostprocessCollection& get_postprocess_param() const;

	/**
	* @brief
	*		��������������Ϣ
	*/
	virtual void disable_camera_postprocess(const char* postprocess_name);

	/**
	* @brief
	*		��������������Ϣ
	*/
	virtual void set_camera_postprocess_enable(const char* postprocess, bool enable);


	// ���û��Ŀ��λ�õĻص�����
	virtual void set_target_position_mng(ITargetPositionMng* target_position_mng);
	virtual H3DVec3 get_cam_position();
	virtual H3DVec3 get_cam_lookat();
	
	// �ⲿ֪ͨ�������������ڴ����һ�ν���
	virtual void play_camera();
	void set_smooth_type(SmoothType smooth_type);
	void set_target_type(TargetType target_type);
	void set_target_id(const TargetID& target_id);

	/**
	* @brief
	*		��entity��ӽ�level
	*/
	virtual void add_to_level(CSceneEntity* level){m_level_entity = level;}
	virtual void remove_from_level(CSceneEntity* level)
	{
		assert(level == m_level_entity);
		m_level_entity = NULL;
	};

	/**
	@brief ����OffsetCalculator����������Ŀ ��������Ķ�̬����
	*/
	virtual void set_offset_calculator_max_count(int count);
	/**
	@brief ����OffsetCalculator��С������� ��������Ķ�̬����
	*/
	virtual void set_offset_calculator_min_interval(int interval);
protected:
	/**
	* @brief
	*		���¶���
	* @param
	*		intervalTime����IN�������ϴθ��µ��¼����
	*/
	virtual void do_update(float interval);

private:
	float m_focalplane; ///< ����ͷ����ƽ�����
	float m_dof_range; ///< ����ͷ����ƽ�淶Χ
	float m_confusion; ///< ����ͷ��ɢϵ��
	H3DVec3 m_up; ///< ����ͷ���Ϸ���
	H3DI::IRender* m_render; ///< ��������Ⱦ����
	H3DI::ICamera*	m_camera; ///< ָ�����������

	H3DVec3 m_look_at; ///< Ŀ���

	PostprocessCollection m_postprocess_collection; ///< ������Ϣ�б�
	std::set<std::string> m_disabled_postprocess_list;///<�����б�

	// ������ƽ�������Ŀ�깦��
	ITargetPositionMng* m_target_position_mng;
	ITargetPosition* m_target_position;

	CameraPositionControl* m_camera_position_control;
	H3DVec3 m_cam_pos;
	H3DVec3 m_cam_lookat;

	SmoothType m_smooth_type;
	TargetType m_target_type;
	TargetID m_target_id;

	void update_camera_target_position();

	float m_fov, m_ratio, m_fNear, m_fFar;
	float m_render_fov;
	bool  m_fov_adjust;

private:
	FORBIDDEN_EVIL_CONSTRUCTOR(CCameraEntity);
};

#endif //_CAMERA_ENTITY_H_
