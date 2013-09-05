/**
* @file camera_entity.h
* 这个文件的主要功能是：
* 
* @author 程陈(Chengchen@h3d.com.cn)
* 
* 更改日志
* ----------------------------------------------------
* ver 1.0.0
*       by 程陈2010.1.25
*/

#ifndef _CAMERA_ENTITY_H_
#define _CAMERA_ENTITY_H_

#include "game_graphic_header.h"
#include "i_target_position.h"


//参考点位移计算器
class COffsetCalculator
{
public:
	COffsetCalculator();
	virtual ~COffsetCalculator(){}
	
	/**
	* @brief
	*		添加偏移点
	*/
	virtual bool push_offset(long interval, const H3DVec3& pos);
	/**
	* @brief
	*		清除所有偏移点
	*/
	void clear_offsets();

	/**
	* @brief
	*		获取偏移
	*/
	virtual H3DVec3 get_offset();

	/**
	* @brief
	*		设置最小采样间隔
	*/
	void set_min_interval(long interval);

	/**
	* @brief
	*		设置最大采样点数目
	*/
	void set_max_count(long count);

protected:
	typedef std::list<H3DVec3> PositionList;

	PositionList m_offsetes; ///< 参考点位置集合
	long	m_min_interval; ///< 最小采样间隔
	long	m_max_node_count; ///< 最大采样点数目
};


//局内参考点平均位移计算器
class CAverageOffsetCalculator
	:public COffsetCalculator
{
public:
	virtual ~CAverageOffsetCalculator(){}
	/**
	* @brief
	*		添加偏移点
	*/
	virtual bool push_offset(long interval, const H3DVec3& pos);

	/**
	* @brief
	*		获取平均便宜点
	*/
	virtual H3DVec3 get_offset();

private:
	long m_cur_time;  ///< 当前更新时间
	long m_last_time; ///< 上次更新时间
};

//局内参考点平均位移计算器
class CFirstCuttingOffsetCalculator
	:public COffsetCalculator
{
public:
	virtual ~CFirstCuttingOffsetCalculator(){}
	/**
	* @brief
	*		添加偏移点
	*/
	virtual bool push_offset(long interval, const H3DVec3& pos);

	/**
	* @brief
	*		获取平均便宜点
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
	@brief 设置最小采样间隔
	*/
	void setMinInterval(int interval);
	/**
	@brief 设置OffsetCalculator最大采样点数目
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
	virtual int		type(void) const;

	/**
	* @brief 
	*		获取对象的方向
	* @return
	*		对象的方向
	*/
	virtual H3DVec3 direction(void) const ;
	/**
	* @brief
	*		设定对象的位置
	* @param
	*		positon：【IN】对象的位置
	*/
	virtual void set_position(const H3DVec3& pos) ;
	/**
	* \brief
	* 获得向上方向
	* 
	* \returns
	* 向上方向
	* 
	*/
	virtual const H3DVec3& up_direction() const;

	/**
	* \brief
	* 设置向上方想
	* 
	* \param up
	* 向上方向
	* 
	*/
	virtual void set_up_direction(const H3DVec3& up);

	/**
	* \brief
	* 摄像头视觉方向
	* 
	* \param atPosition
	* 摄像头视觉所看的位置
	* 
	*/
	virtual void look_at(const H3DVec3& atPosition);

	/**
	* \brief
	* 摄像头视觉色设置
	* 
	* \param eyePosition
	* 眼睛位置
	* 
	* \param atPosition
	* 眼睛看的目的地位置
	* 
	* \param upDirection
	* 向上的方向
	* 
	*/
	virtual void look_at(const H3DVec3& eyePosition, const H3DVec3& atPosition, const H3DVec3& upDirection);

	/**
	* \brief
	* 绕着前方向转
	* 
	* \param degree
	* 转的角度(0-360)
	*/
	virtual void roll(float degree);

	/**
	* \brief
	* 绕着右方向转
	* 
	* \param degree
	* 转的角度(0-360)
	*/
	virtual void yaw(float degree);

	/**
	* \brief
	* 绕着上方向转
	* 
	* \param degree
	* 转的角度(0-360)
	*/
	virtual void pitch(float degree);

	/**
	* \brief
	* 绕着轴旋转
	*
	* \param axis
	* 轴的方向
	*
	* \param degree
	* 转的角度(0-360)
	*/
	virtual void rotate(const H3DVec3& axis, float degree);

	/**
	* \brief
	* 向上下移动
	* 
	* \param step
	* 移动距离
	* 
	* 当移动距离为正的时候向上移动, 否则向下移动
	*/
	virtual void move_up(float step);

	/**
	* \brief
	* 向左右移动
	* 
	* \param step
	* 移动距离
	* 
	* 当移动距离为正的时候向右移动, 否则向左移动
	*/    
	virtual void move_right(float step);

	/**
	* \brief
	* 向前后移动
	* 
	* \param step
	* 移动距离
	* 
	* 前后移动是指摄像头方向的前后, 不是水平面的前后,
	* 当移动距离为正的时候向前移动, 否则向后移动
	*/    
	virtual void move_front(float step);

	/**
	* \brief
	* 获得view matrix
	* 
	* \returns
	* view matrix
	*/
	virtual const H3DMat4& get_view_matrix() const;

	/**
	* \brief
	* 设置可视矩阵
	* 
	* \param viewMatrix
	* view matrix
	*/
	virtual void set_view_matrix(const H3DMat4& viewMatrix);

	/**
	* \brief
	* 获得投影矩阵
	* 
	* \returns
	* 投影矩阵
	*/
	virtual const H3DMat4& get_projection_matrix() const;

	/**
	* \brief
	* 设置投影矩阵
	* 
	* \param projectionMatrix
	* projection matrix
	*/
	virtual void set_projection_matrix(const H3DMat4& projectionMatrix);

	/**
	* \brief
	* 设置平行截体
	* 
	* \param fov
	* 张角
	* 
	* \param ratio
	* 长宽比
	* 
	* \param fNear
	* 近平面
	* 
	* \param fFar
	* 远平面
	* 
	*/
	virtual void set_frustum(float fov, float ratio, float fNear, float fFar);

	/**
	* \brief
	* 设置平行截体
	* 
	* \param fov
	* 张角
	* 
	* \param ratio
	* 长宽比
	* 
	* \param fNear
	* 近平面
	* 
	* \param fFar
	* 远平面
	* 
	*/
	virtual void get_frustum(float* fov, float* ratio, float* fNear, float* fFar);

	virtual void get_render_frustum(float* fov, float* ratio, float* fNear, float* fFar);
	virtual void set_render_frustum(float fov, float ratio, float fNear, float fFar);

	/**
	*@brief 
	*		设置相机是否根据库户屏幕进行fov自动缩放
	*/
	virtual void set_is_fov_auto_adjust(bool bAutoChange);
	virtual bool get_is_fov_auto_adjust();

	///**
	//* \brief
	//* 设置对象方向
	//*
	//* \param direction
	//* 对象方向
	//*/
	//virtual void set_direction(const H3DVec3& direction);

	/**
	* \brief
	* 获得摄像头景深焦平面距离
	* 焦平面距离:原本指摄相机凸透镜的焦距,物体在某个距离下可以在摄相机上成最清晰实像。在这里是焦平面距离指这个距离
	* 
	* \returns
	* 原始焦平面距离
	*/
	virtual float get_focalplane() const;

	virtual void set_focalplane(float focalplane);

	/**
	* \brief
	* 获得摄像头景深范围
	* 景深范围:从焦平面延视线向前后两个方向延伸一段距离，在这个范围内的物体成的象都是清晰，越出这个范围的物体成的像都会在某种程度上变模糊
	* 
	* \returns
	* 原始景深范围
	*/
	virtual float get_dof_range() const;
	virtual void set_dof_range(float dofrange);

	/**
	* \brief
	* 获得摄像头弥散系数
	* 弥散系数:当物体超出景深范围后，逐渐物体的成的像逐渐向外圆形扩散，这个圆称之为弥散圆，
	* 这个圆会随着物体超出景深范围的距离的变大而变大。在摄象机的远近两个截面处圆的半径是最大的。
	* 该值为0时景深关闭
	* 
	* \returns
	* 弥散系数
	*/
	virtual float get_confusion() const;
	virtual void set_confusion(float confusion);

	/**
	* @brief
	*		获取对应的引擎层的ICamera对象
	* @return
	*		返回引擎层的ICamera对象
	*/
	H3DI::ICamera* get_ICamera() const;

	/**
	* @brief
	*		获取目标点的位置
	*/
	virtual H3DVec3 get_look_at() const;
	/**
	* @brief
	*		设定对象的欧拉角，欧拉角和max里面保持一致
	* @return
	*		返回一个h3cVec的值，分别表示绕x、y、z三个轴的旋转弧度
	*/
	virtual void set_rotation(const H3DVec3& obj_rotation);

	/**
	* @brief
	*		向相机中设置一个后处理特效的参数，如果类型是float的特效，将数值添加到H3DVec4.x就可以了
	* @return
	*		
	*/
	virtual bool set_postprocess_param(const char* postprocess,  const char* param,const H3DVec4& paramvalue );
	/**
	* @brief
	*		将相机中的后处理特效的设置给引擎
	* @return
	*		
	*/
	//virtual void apply_postprocess_param();
//private:
//	void position_changed(const H3DVec3& old_pos, const H3DVec3& new_pos);

	/**
	* @brief
	*		获取相机后处理的信息
	*/
	virtual const PostprocessCollection& get_postprocess_param() const;

	/**
	* @brief
	*		设置相机后处理的信息
	*/
	virtual void disable_camera_postprocess(const char* postprocess_name);

	/**
	* @brief
	*		设置相机后处理的信息
	*/
	virtual void set_camera_postprocess_enable(const char* postprocess, bool enable);


	// 设置获得目标位置的回调函数
	virtual void set_target_position_mng(ITargetPositionMng* target_position_mng);
	virtual H3DVec3 get_cam_position();
	virtual H3DVec3 get_cam_lookat();
	
	// 外部通知播放相机，相机内处理第一次进入
	virtual void play_camera();
	void set_smooth_type(SmoothType smooth_type);
	void set_target_type(TargetType target_type);
	void set_target_id(const TargetID& target_id);

	/**
	* @brief
	*		将entity添加进level
	*/
	virtual void add_to_level(CSceneEntity* level){m_level_entity = level;}
	virtual void remove_from_level(CSceneEntity* level)
	{
		assert(level == m_level_entity);
		m_level_entity = NULL;
	};

	/**
	@brief 设置OffsetCalculator最大采样点数目 用于相机的动态调整
	*/
	virtual void set_offset_calculator_max_count(int count);
	/**
	@brief 设置OffsetCalculator最小采样间隔 用于相机的动态调整
	*/
	virtual void set_offset_calculator_min_interval(int interval);
protected:
	/**
	* @brief
	*		更新对象
	* @param
	*		intervalTime：【IN】距离上次更新的事件间隔
	*/
	virtual void do_update(float interval);

private:
	float m_focalplane; ///< 摄像头焦深平面距离
	float m_dof_range; ///< 摄像头景深平面范围
	float m_confusion; ///< 摄像头弥散系数
	H3DVec3 m_up; ///< 摄像头的上方向
	H3DI::IRender* m_render; ///< 引擎层的渲染对象
	H3DI::ICamera*	m_camera; ///< 指向照相机对象

	H3DVec3 m_look_at; ///< 目标点

	PostprocessCollection m_postprocess_collection; ///< 后处理信息列表
	std::set<std::string> m_disabled_postprocess_list;///<后处理列表

	// 添加相机平滑与相机目标功能
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
