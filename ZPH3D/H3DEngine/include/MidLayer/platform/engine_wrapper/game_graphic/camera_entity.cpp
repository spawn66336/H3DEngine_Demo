/**
* @file camera_entity.cpp
* 这个文件的主要功能是：
* 
* @author 程陈(Chengchen@h3d.com.cn)
* 
* 更改日志
* ----------------------------------------------------
* ver 1.0.0
*       by 程陈2010.1.25
*/

#include "game_graphic_body.h"

CCameraEntity::CCameraEntity(CGraphicsResMng* res_mng, const std::string& entity_name, H3DI::ICamera* camera)
:C3DEntity(res_mng, entity_name, camera),m_up(0,0, 1.0),m_dof_range(1999.0f)
,m_confusion(0.0f),m_focalplane(0.0f), m_fov_adjust(true)
{
	m_render = res_mng->get_render_system()->get_IRender();
	m_camera = camera;
	//set_direction(H3DVec3(0, 1, 0));
	m_look_at = position() +H3DVec3(0, 1, 0);

	// 添加相机平滑和相机目标
	m_target_position_mng = 0;
	m_target_position = 0;

	m_smooth_type = SmoothType_NONE;
	m_target_type = TargetType_ALL_TARGET;
	m_target_id = -1;

	m_camera_position_control = new CameraPositionControl();
	m_camera_position_control->setOffsetType(m_smooth_type);

	m_fov = 45.0f;
	m_render_fov = 45.0f;
	m_ratio = 1.3333333f;
	m_fNear = 1;
	m_fFar = 1000;

   set_render_frustum(m_fov, m_ratio, m_fNear, m_fFar);

	do_update(0);
}

CCameraEntity::~CCameraEntity()
{
	delete m_camera_position_control;
}

bool CCameraEntity::is_kind_of(int obj_type)
{
	if (MEDIA_END_ENTITY_TYPE_CAMERA == obj_type)
	{
		return true;
	}

	return C3DEntity::is_kind_of(obj_type);
}

int	CCameraEntity::type(void) const
{
	return MEDIA_END_ENTITY_TYPE_CAMERA;
}

const H3DVec3& CCameraEntity::up_direction() const
{
	return m_up;
}

void CCameraEntity::set_up_direction(const H3DVec3& up)
{
	m_up = up;
	//Signal<CAMERA_ENTITY_UP_DIRECTION_CHANGED>(this, up);
}

void CCameraEntity::look_at(const H3DVec3& atPosition)
{
	// look_at(position(),atPosition,up_direction());
	m_look_at = atPosition;
}

void CCameraEntity::look_at(const H3DVec3& eyePosition, const H3DVec3& atPosition, const H3DVec3& upDirection)
{
	H3DVec3 front = eyePosition - atPosition;//z

	float len =front.Normalize();
	if (len < 0.000001f)
	{
		//atpos == eyepos
		H3DVec3 v(0,0,1);
		front = v;
	}
	//upVec = y
	H3DVec3 right = upDirection.Cross(front);//x
	float len_z = right.Normalize();

	H3DVec3 up = front.Cross(right);//y


	H3DMat3 m;m.Identity();
	m[0].x = right.x;  m[1].x = right.y;  m[2].x = right.z;
	m[0].z = up.x; m[1].z = up.y;  m[2].z = up.z;
	m[0].y = -front.x;m[1].y = -front.y;m[2].y = -front.z;


	H3DMat3 mrot = m; 
	m_position = eyePosition;
	m_look_at = atPosition;
	m_up = upDirection;
	H3DVec3 dir = direction();
	dir.Normalize();
	C3DEntity::set_rotation(quat_to_eula(mrot.ToQuat()));
	H3DVec3 dir2 = direction();
	dir2.Normalize();
	float f = dir.Dot(dir2);
	
}

void CCameraEntity::roll(float degree)
{
	H3DMat4 old_loc = get_location_matrix();
	H3DVec3 old_pos = position();
	H3DVec3 old_dir = direction();

	m_camera->Rotate(H3DVec3(0, 1, 0), degree);
	//Signal<CAMERA_ENTITY_ROLL>(this, degree);
	H3DVec3 eula = rotation();
	eula.y+=degree;
	set_rotation(eula);
}

void CCameraEntity::yaw(float degree)
{
	H3DMat4 old_loc = get_location_matrix();
	H3DVec3 old_pos = position();
	H3DVec3 old_dir = direction();

	m_camera->Rotate(H3DVec3(0, 0, 1), degree);
	//Signal<CAMERA_ENTITY_YAW>(this, degree);
	H3DVec3 eula = rotation();
	eula.z+=degree;
	set_rotation(eula);
}

void CCameraEntity::pitch(float degree)
{
	H3DMat4 old_loc = get_location_matrix();
	H3DVec3 old_pos = position();
	H3DVec3 old_dir = direction();

	m_camera->Rotate(H3DVec3(1, 0, 0), degree);
	H3DVec3 eula = rotation();
	eula.x+=degree;
	set_rotation(eula);
	//Signal<CAMERA_ENTITY_PITCH>(this, degree);
}

void CCameraEntity::rotate(const H3DVec3& axis, float degree)
{
	H3DMat4 old_loc = get_location_matrix();
	H3DVec3 old_pos = position();
	H3DVec3 old_dir = direction();

	m_camera->Rotate(axis, degree);
	//Signal<CAMERA_ENTITY_ROTATE>(this, axis, degree);
	H3DQuat quat;
	quat.FromAngleAxis(degree,axis);
	m_quat = quat*m_quat;
	set_rotation(quat_to_eula(m_quat));
}

void CCameraEntity::move_up(float step)
{
	H3DMat4 old_loc = get_location_matrix();
	H3DVec3 old_pos = position();
	H3DVec3 old_dir = direction();

	m_camera->MoveUp(step);
	//Signal<ENTITY_POSITION_CHANGED>(dynamic_cast<IMoveEntity*>(this), old_pos, position());
	old_pos.z += step;
	set_position(old_pos);
	m_look_at.z += step;
}

void CCameraEntity::move_right(float step)
{
	H3DMat4 old_loc = get_location_matrix();
	H3DVec3 old_pos = position();
	H3DVec3 old_dir = direction();

	m_camera->MoveRight(step);
	//Signal<ENTITY_POSITION_CHANGED>(dynamic_cast<IMoveEntity*>(this), old_pos, position());
	old_pos.x += step;
	set_position(old_pos);
	m_look_at.x += step;
}

void CCameraEntity::move_front(float step)
{
	H3DMat4 old_loc = get_location_matrix();
	H3DVec3 old_pos = position();
	H3DVec3 old_dir = direction();

	m_camera->MoveFront(step);
	//Signal<ENTITY_POSITION_CHANGED>(dynamic_cast<IMoveEntity*>(this), old_pos, position());
	old_pos.y -= step;
	set_position(old_pos);
	m_look_at.y -= step;
}

const H3DMat4& CCameraEntity::get_view_matrix() const
{
	return m_camera->GetViewMatrix();
}

void CCameraEntity::set_view_matrix(const H3DMat4& viewMatrix)
{
	H3DI::ICamera* camera = m_render->GetRenderCamera();
	m_render->SetRenderCamera(m_camera);
	m_render->SetViewMatrix(viewMatrix);
	m_render->SetRenderCamera(camera);
}

const H3DMat4& CCameraEntity::get_projection_matrix() const
{
	return m_camera->GetProjectionMatrix();
}

void CCameraEntity::set_projection_matrix(const H3DMat4& projectionMatrix)
{
	H3DI::ICamera* camera = m_render->GetRenderCamera();
	m_render->SetRenderCamera(m_camera);
	m_render->SetPerspective(projectionMatrix);
	m_render->SetRenderCamera(camera);
}

void CCameraEntity::set_frustum(float fov, float ratio, float fNear, float fFar)
{
	unsigned short vp[4] = {0};
	m_render->GetViewPort(vp);
	if(vp[3] > 0)
	{
		float ratio2 = (float)vp[2] / (float)vp[3];
		float tg = 3.0f / 4.0f * ratio2 * tan(DEG2RAD(fov / 2));
		float newfov = RAD2DEG(atan(tg) * 2);

		m_fov = fov;
		m_ratio = ratio2;
		m_fNear = fNear;
		m_fFar = fFar;

		m_render_fov = newfov;
		
//		H3DI::ICamera* camera = m_render->GetRenderCamera();
//		m_render->SetRenderCamera(m_camera);
//		m_render->SetFrustum(newfov, ratio2, fNear, fFar);
//		m_render->SetRenderCamera(camera);
	}
}

void CCameraEntity::set_render_frustum(float fov, float ratio, float fNear, float fFar)
{
		H3DI::ICamera* camera = m_render->GetRenderCamera();
		m_render->SetRenderCamera(m_camera);
		m_render->SetFrustum(fov, ratio, fNear, fFar);
		m_render->SetRenderCamera(camera);
}

void CCameraEntity::get_frustum(float* fov, float* ratio, float* fNear, float* fFar)
{
	*fov = m_fov;
	*ratio = m_ratio;
	*fNear = m_fNear;
	*fFar = m_fFar;

	return;

	H3DI::ICamera* camera = m_render->GetRenderCamera();
	m_render->SetRenderCamera(m_camera);
	m_render->GetFrustum(*fov, *ratio, *fNear, *fFar);
	m_render->SetRenderCamera(camera);
}

void CCameraEntity::get_render_frustum(float* fov, float* ratio, float* fNear, float* fFar)
{
	*fov = m_fov_adjust ? m_render_fov : m_fov;
	*ratio = m_ratio;
	*fNear = m_fNear;
	*fFar = m_fFar;

	return;

	//H3DI::ICamera* camera = m_render->GetRenderCamera();
	//m_render->SetRenderCamera(m_camera);
	//m_render->GetFrustum(*fov, *ratio, *fNear, *fFar);
	//m_render->SetRenderCamera(camera);
}

void CCameraEntity::set_is_fov_auto_adjust(bool bAutoChange)
{
	m_fov_adjust = bAutoChange;
}

bool CCameraEntity::get_is_fov_auto_adjust()
{
	return m_fov_adjust;
}

float CCameraEntity::get_focalplane() const
{
	return m_focalplane;
}

void CCameraEntity::set_focalplane(float focalplane)
{
	m_focalplane = focalplane;
}

float CCameraEntity::get_dof_range() const
{
	return m_dof_range;
}

void CCameraEntity::set_dof_range(float dofrange)
{
	m_dof_range = dofrange;
}

float CCameraEntity::get_confusion() const
{
	return m_confusion;
}

void CCameraEntity::set_confusion(float confusion)
{
	m_confusion = confusion;
}

H3DI::ICamera* CCameraEntity::get_ICamera() const
{
	return m_camera;
}

H3DVec3 CCameraEntity::get_look_at() const
{
	return m_look_at;
}

H3DVec3 CCameraEntity::direction( void ) const
{
	return m_look_at-position();
}

void CCameraEntity::set_position( const H3DVec3& pos )
{
	m_position = pos;
	// look_at(pos,m_look_at,m_up);
}

void CCameraEntity::set_rotation( const H3DVec3& obj_rotation )
{
	H3DVec3 dir = direction();
	C3DEntity::set_rotation(obj_rotation );
	float length = dir.Length();
	dir = m_quat*H3DVec3(0.0f,1.0f,0.0f);
	m_look_at = dir *length + position();
}

bool CCameraEntity::set_postprocess_param(const char* postprocess,  const char* param,const H3DVec4& paramvalue )
{
	if (m_disabled_postprocess_list.find(postprocess) != m_disabled_postprocess_list.end())
	{
		return false;
	}

	if(m_postprocess_collection.find(postprocess) ==  m_postprocess_collection.end())
	{
		m_postprocess_collection.insert(std::make_pair(postprocess, PostprocessParamCollection()));
	}
	m_postprocess_collection[postprocess][param] = paramvalue;

	return true;
}

const CCameraEntity::PostprocessCollection& CCameraEntity::get_postprocess_param() const
{
	return m_postprocess_collection;
}

void CCameraEntity::disable_camera_postprocess(const char* postprocess)
{
	m_postprocess_collection.erase(postprocess);
	m_disabled_postprocess_list.insert(postprocess);
}

void CCameraEntity::set_camera_postprocess_enable(const char* postprocess, bool enable)
{
	if (enable)
	{
		m_disabled_postprocess_list.erase(postprocess);
	}
	else
	{
		m_postprocess_collection.erase(postprocess);
		m_disabled_postprocess_list.insert(postprocess);
	}
}

void CCameraEntity::set_target_position_mng(ITargetPositionMng* target_position_mng) 
{
	m_target_position_mng = target_position_mng; 
	if(target_position_mng)
		m_camera_position_control->setMinInterval(target_position_mng->getMinInterval());
	update_camera_target_position();
}

void CCameraEntity::set_smooth_type(SmoothType smooth_type)
{
	m_smooth_type = smooth_type;

	m_camera_position_control->setOffsetType(m_smooth_type);
	update_camera_target_position();
}

void CCameraEntity::set_target_type(TargetType target_type)
{
	m_target_type = target_type;
	update_camera_target_position();
}

void CCameraEntity::set_target_id(const TargetID& target_id)
{
	m_target_id = target_id;
	update_camera_target_position();
}


void CCameraEntity::update_camera_target_position()
{
	m_target_position = 0;

	if(m_target_position_mng && m_smooth_type != SmoothType_NONE)
	{
		m_target_position = m_target_position_mng->getTargetPosition(m_target_type, m_target_id);
	}

	m_camera_position_control->clear_record();
}


H3DVec3 CCameraEntity::get_cam_position()
{
	return m_cam_pos;
}

H3DVec3 CCameraEntity::get_cam_lookat()
{
	return m_cam_lookat;
}

void CCameraEntity::play_camera()
{
	m_camera_position_control->clear_record();
}

void CCameraEntity::do_update(float interval)
{
	H3DVec3 adjustp;
	if(m_camera_position_control)
	{
		adjustp = m_camera_position_control->update_camera_pos(interval * 1000, position(), m_target_position);
	}
	m_cam_pos = position() + adjustp;
	m_cam_lookat = get_look_at() + adjustp;
}

void CCameraEntity::set_offset_calculator_max_count( int count )
{
	if(m_camera_position_control)
		m_camera_position_control->set_offset_calculator_max_count(count);
}

void CCameraEntity::set_offset_calculator_min_interval( int interval )
{
	if(m_camera_position_control)
		m_camera_position_control->setMinInterval(interval);
}
COffsetCalculator::COffsetCalculator()
: m_min_interval(50)
, m_max_node_count(50)
{
	NULL;
}

bool COffsetCalculator::push_offset(long interval, const H3DVec3& pos)
{
	return false;
}

void COffsetCalculator::clear_offsets()
{
	m_offsetes.clear();
}

H3DVec3 COffsetCalculator::get_offset()
{
	return H3DVec3(0.0f, 0.0f, 0.0f);
}

void COffsetCalculator::set_min_interval(long interval)
{
	m_min_interval = interval;
}

void COffsetCalculator::set_max_count(long count)
{
	m_max_node_count = count;
}


//////////////////////////////////////////////////////////////////////////
bool CFirstCuttingOffsetCalculator::push_offset(long interval, const H3DVec3& pos)
{		
	if (m_offsetes.empty())
		m_offsetes.push_back(pos);

	return true;
}

H3DVec3 CFirstCuttingOffsetCalculator::get_offset()
{
	H3DVec3 average(0.0f, 0.0f, 0.0f);
	for (PositionList::iterator it = m_offsetes.begin(); it != m_offsetes.end(); ++it)
	{
		average += (*it);
	}
	
	average.z = 0.0f;
	return average;
}

bool CAverageOffsetCalculator::push_offset(long interval, const H3DVec3& pos)
{
	if (m_offsetes.size() == 0)
	{
		m_offsetes.push_back(pos);
		m_cur_time = 0;
		m_last_time = 0;
		return true;
	}

	m_cur_time += interval;

	if ((m_cur_time - m_last_time) >= m_min_interval)
	{
		m_offsetes.push_back(pos);
		while (m_offsetes.size() > m_max_node_count)
		{
			m_offsetes.pop_front();
		}

		m_last_time += m_min_interval;

		return true;
	}

	return false;
}


H3DVec3 CAverageOffsetCalculator::get_offset()
{
	H3DVec3 average(0.0f, 0.0f, 0.0f);
	for (PositionList::iterator it = m_offsetes.begin(); it != m_offsetes.end(); ++it)
	{
		average += (*it);
	}
	
	if (m_offsetes.size() > 1)
	{
		average /= m_offsetes.size();
	}

	average.z = 0.0f;

	return average;
}

CameraPositionControl::CameraPositionControl()
{
	m_offset_calculator = new CAverageOffsetCalculator();
}

void CameraPositionControl::setOffsetType(SmoothType st)
{
	delete m_offset_calculator;

	if(st == SmoothType_ON_ENTER)
		m_offset_calculator = new CFirstCuttingOffsetCalculator();
	else
		m_offset_calculator = new CAverageOffsetCalculator();
}

CameraPositionControl::~CameraPositionControl()
{
	delete m_offset_calculator;
}

void CameraPositionControl::clear_record()
{
	m_offset_calculator->clear_offsets();
}

H3DVec3 CameraPositionControl::update_camera_pos(long interval, const H3DVec3& campos, ITargetPosition* target_position)
{
	if(!target_position)
		return H3DVec3(0, 0, 0);

	if(!target_position)
	{
		//LogInfo("[CameraPositionControl::update_camera_pos] target_position invalid");
	}

	//获取目标点
	H3DVec3 target_pos = target_position->get_target_position();

	H3DVec3 ave_target_pos;
	if(m_offset_calculator)
	{
		//将目标点的位置push到参考点计算器
		m_offset_calculator->push_offset(interval, target_pos);
		//获取平均点
		ave_target_pos = m_offset_calculator->get_offset();
	}

	return ave_target_pos;
}
void CameraPositionControl::setMinInterval(int interval)
{
	m_offset_calculator->set_min_interval(interval);	
};

void CameraPositionControl::set_offset_calculator_max_count( int count )
{
	m_offset_calculator->set_max_count(count);
}