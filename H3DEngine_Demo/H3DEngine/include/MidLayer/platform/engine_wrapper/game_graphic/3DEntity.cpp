/**
* @file 3DEntity.cpp
* 这个文件的主要功能是：媒体对象的共同基类C3DEntity的实现
* 
* @author 程陈(Chengchen@h3d.com.cn)
* 
* 更改日志
* ----------------------------------------------------
* ver 1.0.0
*       by 程陈2010.1.25
*/

#include "game_graphic_body.h"
#include "platform/common/interact_ray.h"
#include "platform/common/interact_obb.h"
#include "operation.h"

unsigned int C3DEntity::s_render_count = 1;

C3DEntity::C3DEntity(CGraphicsResMng* res_mng, const std::string& name, H3DI::IMoveObject* move_object)
: CMediaEndEntity(res_mng, name)
, m_OBB(NULL)
, m_entity_state(ES_VIEW)
, m_is_create_sync(true)
, m_is_view_sync(true)
, m_is_first_view(false)
, m_level_layer(H3DI::SL_DetailObj)
, m_priority(0)
, m_create_priority(0)
, m_is_static(false)
, m_detial_actor_type(DAT_SMALL_ACTOR)
, m_visible_dist(-1.f)
{
	assert(NULL != move_object);
	m_move_object = move_object;
	m_cur_time = 0.0f;
	m_media_length = FLT_MAX;
	m_play_speed = 1.0f;
	m_status = MEDIA_PLAYING;

	m_is_loop = false;
	m_is_visible = true;
	m_is_external_visible = true;
	m_lod_level = LL_ZERO;

	m_quat.Identity();
	m_position.Set(0.0f,0.0f,0.0f);
	m_scale.Set(1.0f,1.0f,1.0f);
	m_eula.Set(0.0f,0.0f,0.0f);
	m_location_matrix.Identity();
	move_object->SetLocationMatrix(m_location_matrix);
	m_resource_name = "";

#ifdef _LOCATION_SET_OPTIMIZE_
	m_prs_changed = false;
	m_location_matrix_changed = false;
#endif
}

C3DEntity::C3DEntity(CGraphicsResMng* res_mng, const std::string& name, const std::string& resource_name, int lod)
: CMediaEndEntity(res_mng, name)
, m_OBB(NULL)
, m_move_object(0)
, m_entity_state(ES_NONE)
, m_resource_name(resource_name)
, m_set_color_factor(false)
, m_set_diffuse_color_factor(false)
, m_is_create_sync(true)
, m_is_view_sync(true)
, m_is_first_view(false)
, m_level_layer(H3DI::SL_DetailObj)
, m_priority(0)
, m_create_priority(0)
, m_is_static(false)
, m_detial_actor_type(DAT_SMALL_ACTOR)
, m_visible_dist(-1.f)
{
	convert_path_string(m_resource_name);

	m_cur_time = 0.0f;
	m_media_length = FLT_MAX;
	m_play_speed = 1.0f;
	m_status = MEDIA_PLAYING;

	m_is_loop = false;
	m_is_visible = true;
	m_is_external_visible = true;
	m_lod_level = lod;

	m_quat.Identity();
	m_position.Set(0.0f,0.0f,0.0f);
	m_scale.Set(1.0f,1.0f,1.0f);
	m_eula.Set(0.0f,0.0f,0.0f);
	m_location_matrix.Identity();
}

C3DEntity::C3DEntity(CGraphicsResMng* res_mng, std::string name)
: CMediaEndEntity(res_mng, name)
, m_entity_state(ES_VIEW)
, m_is_create_sync(true)
, m_is_view_sync(true)
, m_is_first_view(false)
, m_level_layer(H3DI::SL_DetailObj)
, m_priority(0)
, m_create_priority(0)
, m_is_static(false)
, m_detial_actor_type(DAT_SMALL_ACTOR)
, m_visible_dist(-1.f)
{
	m_move_object = NULL;
	m_cur_time = 0.0f;
	m_media_length = FLT_MAX;
	m_play_speed = 1.0f;
	m_status = MEDIA_STOP;

	m_is_loop = false;
	m_is_visible = true;
	m_lod_level = LL_ZERO;
}

C3DEntity::~C3DEntity(void)
{
	clear_all_op();
	if (NULL != m_move_object)
	{
		m_res_mng->get_entity_factory()->release_engine_entity(m_move_object);
	}
}

void C3DEntity::create_engine_object(bool is_sync, int priority)
{
	if (m_entity_state == ES_NONE)
	{
		assert(priority>=0 && priority<PRIORITY_COUNT);
		if (priority>=0 && priority<PRIORITY_COUNT)
		{
			m_create_priority = priority;
		}

		m_is_create_sync = is_sync;
		if (is_sync)
		{
			m_entity_state = ES_VIEW;
		}
		else
		{
			m_entity_state = ES_CREATE;
		}
		create_engine_object_impl(is_sync);
	}
}

void C3DEntity::set_entity_state(ENTITY_STATE state)
{
	m_entity_state = state;
	if (m_entity_state==ES_HIDE && m_move_object)
	{
		set_entity_flags(H3DI::I_ENTITY_HIDE, true);
	}
}

void C3DEntity::create_sync()
{
	if (!m_move_object)
	{
		Operation* pOp = m_execute_op_list[m_create_priority].front();
		m_execute_op_list[m_create_priority].pop_front();
		pOp->finish_sync();
		pOp->do_op(this);
		pOp->release();
	}
}

void C3DEntity::play(void) 
{
	set_visible(true);

	if (status() != MEDIA_PLAYING) //如果没有播放，则播放媒体
	{
		//Signal<MEDIA_MSG_PLAY>(dynamic_cast<IMediaEntity*>(this));

		m_cur_time = 0.0f;
		m_status = MEDIA_PLAYING;

	}
}

void C3DEntity::pause(void)
{
	if (status() == MEDIA_PLAYING) //如果没有进行播放动作，则暂停不起作用
	{
		//Signal<MEDIA_MSG_PAUSE>(dynamic_cast<IMediaEntity*>(this));

		m_status = MEDIA_PAUSE;
	}
}

void C3DEntity::stop(void)
{
	set_visible(false);

	if ((status() == MEDIA_PAUSE) || (status() == MEDIA_PLAYING)) //如果没有停止，则停止
	{
		//Signal<MEDIA_MSG_STOP>(dynamic_cast<IMediaEntity*>(this));

		m_status = MEDIA_STOP;
		m_cur_time = 0.0f;
	}
}

void C3DEntity::reset(void)
{
//	if (MEDIA_STOP != status()) //如果停止了，reset将不起作用
	{
		//Signal<MEDIA_MSG_RESET>(dynamic_cast<IMediaEntity*>(this));

		stop();
		play();
	}
}

void C3DEntity::skip_to(float time)
{
	assert(time <= m_media_length && time >= 0);

	float old_cur_time = m_cur_time;

	update(time - m_cur_time);
	m_cur_time = time;

	if (old_cur_time != time)
	{
		//Signal<MEDIA_MSG_CUR_TIME_CHANGED>(dynamic_cast<IMediaEntity*>(this), old_cur_time, time);
	}
}

void C3DEntity::set_loop(bool loop)
{
	m_is_loop = loop;
}

bool C3DEntity::is_loop(void)
{
	return m_is_loop;
}

void C3DEntity::set_media_length(float length)
{
	assert(length >= 0);

	float old_length = m_media_length;
	m_media_length = length;
	
	if (old_length != length)
	{
		//Signal<MEDIA_MSG_LENGTH_CHANGED>(dynamic_cast<IMediaEntity*>(this), old_length, length);
	}
}

float C3DEntity::media_length(void) const
{
	return m_media_length;
}

float C3DEntity::current_time(void) const
{
	return m_cur_time;
}

void C3DEntity::set_play_speed(float speed)
{
	assert(speed > 0);

	float old_speed = m_play_speed;
	m_play_speed = speed;

	if (old_speed != speed)
	{
		//Signal<MEDIA_MSG_SPEED_CHANGED>(dynamic_cast<IMediaEntity*>(this), old_speed, speed);
	}
}

float C3DEntity::play_speed(void) const
{
	return m_play_speed;
}

MEDIA_STATUS C3DEntity::status(void) const
{
	return m_status;
}

H3DVec3 C3DEntity::position(void) const
{
#ifdef _LOCATION_SET_OPTIMIZE_
	const_cast<C3DEntity*>(this)->apply_location_changed();
#endif
	return m_position;
}

void C3DEntity::set_position(const H3DVec3& pos)
{
	if (m_position == pos)
	{
		return;
	}

#ifdef _LOCATION_SET_OPTIMIZE_
	apply_location_changed();
	m_position = pos;

	m_prs_changed = true;
#else
	m_position = pos;
	set_location_matrix_inner(m_position, m_quat, m_scale);
#endif
}

H3DVec3 C3DEntity::rotation() const
{
#ifdef _LOCATION_SET_OPTIMIZE_
	apply_location_changed();
#endif
	return m_eula;
}

void C3DEntity::set_rotation(const H3DVec3& obj_rotation)
{
	if (m_eula == obj_rotation)
	{
		return;
	}

#ifdef _LOCATION_SET_OPTIMIZE_
	apply_location_changed();
	m_eula = obj_rotation;
	m_prs_changed = true;
#else
	H3DAngles angle(RAD2DEG(obj_rotation.y), RAD2DEG(obj_rotation.z), RAD2DEG(obj_rotation.x));
	m_quat = angle.ToQuat();
	//angle生成的3x3矩阵放到4x4里需要转置，四元数和3x3矩阵是一致的
	m_quat = m_quat.Inverse();
	m_eula = obj_rotation;

	set_location_matrix_inner(m_position, m_quat, m_scale);
#endif
}

void C3DEntity::set_scale(const H3DVec3& scale)
{
	assert(scale.x > 0);
	assert(scale.y > 0);
	assert(scale.z > 0);

	if (m_scale == scale)
	{
		return;
	}

#ifdef _LOCATION_SET_OPTIMIZE_
	apply_location_changed();
	m_scale = scale;
	m_prs_changed = true;
#else
	m_scale = scale;
	set_location_matrix_inner(m_position, m_quat, m_scale);
#endif
}

H3DVec3 C3DEntity::get_scale() const
{
#ifdef _LOCATION_SET_OPTIMIZE_
	const_cast<C3DEntity*>(this)->apply_location_changed();
#endif
	return m_scale;
}

const H3DMat4& C3DEntity::get_location_matrix(void) const
{
#ifdef _LOCATION_SET_OPTIMIZE_
	const_cast<C3DEntity*>(this)->apply_prs_changed();
#endif
	return m_location_matrix;
}

void C3DEntity::set_location_matrix(const H3DMat4& location)
{
#ifdef _LOCATION_SET_OPTIMIZE_
	//if (m_prs_changed && location == m_location_matrix)
	//{
	//	return;
	//}

	m_location_matrix = location;
	m_location_matrix_changed = true;
	m_prs_changed = false;
#else
	set_location_matrix_inner(location);
	location.Decompose(m_scale, m_quat, m_position);
#endif
}

void C3DEntity::set_location_matrix_inner(const H3DMat4& location)
{
	m_location_matrix = location;
	set_world_location_matrix(get_world_location_matrix());
}

void C3DEntity::set_location_matrix_inner(const H3DVec3&pos, const H3DQuat& quat, const H3DVec3& scale)
{
	H3DMat4 mat;
	mat.Compose(scale, quat, pos);

	set_location_matrix_inner(mat);
}

void C3DEntity::set_world_location_matrix(const H3DMat4& location)
{
	if (m_move_object)
	{
		m_move_object->SetLocationMatrix(location);
	}
}

const H3DMat4& C3DEntity::get_world_location_matrix(void)
{
#ifdef _LOCATION_SET_OPTIMIZE_
	apply_prs_changed();
#endif
	if (get_actor() != NULL)
	{
		m_world_location_matrix = get_actor()->get_world_location_matrix() * m_location_matrix;

		return m_world_location_matrix;
	}

	return m_location_matrix;
}

void C3DEntity::set_visible(bool visible)
{
	if (is_visible() == visible)
	{
		return;
	}

	m_is_visible = visible;
	//set_entity_flags(H3DI::I_ENTITY_HIDE, !visible);
	update_engine_entity_visible();
}

bool C3DEntity::is_visible()
{
	return m_is_visible;
}

void C3DEntity::set_external_visible(bool visible)
{
	m_is_external_visible = visible;
	update_engine_entity_visible();
}

bool C3DEntity::is_external_visible() const
{
	return m_is_external_visible;
}

void C3DEntity::update_engine_entity_visible()
{
	if (m_entity_state==ES_VIEW)
	{
		bool bVisible = is_visible() && is_external_visible();
		set_entity_flags(H3DI::I_ENTITY_HIDE, !bVisible);
	}
}

bool C3DEntity::is_kind_of(int obj_type)
{
	if ((MEDIA_END_ENTITY_TYPE_3D_ENTITY == obj_type) //如果为3d可控实体
		||(MEDIA_END_ENTITY_TYPE_MOVE_ENTITY == obj_type) //如果为可移动实体
		||(MEDIA_END_ENTITY_TYPE_MEDIA_ENTITY == obj_type) //如果为可播放实体
		)
	{
		return true;
	}

	return CMediaEndEntity::is_kind_of(obj_type);
}

int C3DEntity::type() const
{
	return MEDIA_END_ENTITY_TYPE_3D_ENTITY;
}

H3DI::IMoveObject* C3DEntity::get_IMoveObject() const
{
	return m_move_object;
}
void C3DEntity::do_update(float interval)
{
	float real_time = interval * play_speed();
	if(status() == MEDIA_PLAYING)
	{
		m_cur_time += real_time;

		if (m_entity_state!=ES_UNLOAD)
		{
			update_execute_op();
			update_media_object(real_time);
		}

		if (m_cur_time >= m_media_length)
		{
			if (!m_is_loop)
			{
				m_cur_time = 0.0f;
				set_visible(false);
				m_status = MEDIA_FINISH;
				stop();

				//添加消息
				//Signal<MEDIA_MSG_FINISH>(dynamic_cast<IMediaEntity*>(this));
			}
			else
			{
				m_cur_time -= m_media_length;
			}
		}
	}

	CMediaEndEntity::do_update(real_time);
}

void C3DEntity::update_media_object(float interval)
{
	if (NULL != get_IMoveObject())
	{
		get_IMoveObject()->Update(interval / 1000.0f);
	}
}

BBOX C3DEntity::get_BBox()
{
	BBOX box;
	if (m_move_object)
	{
		float bbox[7] = {0.0f};
		m_move_object->GetAABB(bbox);
		
		box.m_min_pos = H3DVec3(bbox[0], bbox[1], bbox[2]);
		box.m_max_pos = H3DVec3(bbox[3], bbox[4], bbox[5]);
	}

	return box;
}

bool C3DEntity::change_resource(const std::string& res_name)
{
	set_resource_name(res_name);
	return true;
}

bool C3DEntity::change_media_object(H3DI::IMoveObject* object, bool use_preproperty)
{
	if (NULL == object)
	{
		return false;
	}

	if (use_preproperty)
	{
		H3DMat4 loc = get_world_location_matrix();
		object->SetLocationMatrix(loc);
	}

	//从场景中删除旧的物体
	if (is_added_to_level())
		m_level_entity->get_IScene()->DetachModel(m_move_object);

	//Clear所有OP
	clear_all_op();
	if (NULL != m_move_object)
	{
		m_res_mng->get_entity_factory()->release_engine_entity(m_move_object);
	}

	m_move_object = object;
	set_is_static(m_is_static);

	//将新的物体加入场景
	if (is_added_to_level())
	{
		m_level_entity->get_IScene()->AttachModel(m_move_object, m_level_layer);
		m_level_entity->get_IScene()->MoveModel(m_move_object);
	}

	return true;
}

//////////////////////////////////////////////////////////////////////////
H3DVec3 C3DEntity::quat_to_eula(const H3DQuat &quat )
{
	float fTx = (2.0f) * quat.x;
	float fTy = (2.0f) * quat.y;
	float fTz = (2.0f) * quat.z;
	float fTwx = fTx * quat.w;
	float fTwy = fTy * quat.w;
	float fTwz = fTz * quat.w;
	float fTxx = fTx * quat.x;
	float fTxy = fTy * quat.x;
	float fTxz = fTz * quat.x;
	float fTyy = fTy * quat.y;
	float fTyz = fTz * quat.y;
	float fTzz = fTz * quat.z;

	float _11 = (float)1.0 - (fTyy + fTzz);
	float _12 = fTxy + fTwz;
	float _13 = fTxz - fTwy;
	float _23 = fTyz + fTwx;
	float _33 = (float)1.0 - (fTxx + fTyy);

	float xAngle, yAngle, zAngle;

	if (_13 < 1.0f)
	{
		if (_13 > -1.0f)
		{
			// y_angle = asin(-r20)
			// z_angle = atan2(r10,r00)
			// x_angle = atan2(r21,r22)
			yAngle = (float)asin(-(float)_13);
			zAngle = (float)atan2((double)_12, (double)_11);
			xAngle = (float)atan2((double)_23, (double)_33);
		}
		else
		{
			// y_angle = +pi/2
			// x_angle - z_angle = atan2(r01,r02)
			// WARNING.  The solution is not unique.  Choosing x_angle = 0.
			float _21 = fTxy - fTwz;
			float _31 = fTxz + fTwy;
			yAngle = H3DMath::PI/ 2.0f;
			zAngle = -(float)atan2((double)_21, (double)_31);
			xAngle = 0.0f;
		}

	}
	else
	{
		// y_angle = -pi/2
		// x_angle + z_angle = atan2(-r01,-r02)
		// WARNING.  The solution is not unique.  Choosing x_angle = 0;
		float _21 = fTxy - fTwz;
		float _31 = fTxz + fTwy;
		yAngle = -H3DMath::PI / 2.0f;
		zAngle = (float)atan2(-(double)_21, -(double)_31);
		xAngle = 0.0f;
	}

	return H3DVec3(xAngle, yAngle, zAngle);
}

void C3DEntity::set_resource_name(const std::string& res_path)
{
	m_resource_name = res_path;
	convert_path_string(m_resource_name);
}

std::string C3DEntity::resource_name() const
{
	return m_resource_name;
}

void C3DEntity::set_actor(Actor* actor)
{
	H3DMat4 location = get_location_matrix();

	CMediaEndEntity::set_actor(actor);

	set_location_matrix(location);
}

#ifdef _LOCATION_SET_OPTIMIZE_

void C3DEntity::update_location()
{
	if (m_prs_changed)
	{
		apply_prs_changed();
	}
	else if (m_location_matrix_changed)
	{
		apply_location_changed();
	}
}

void C3DEntity::apply_location_changed()
{
	if (m_location_matrix_changed)
	{
		m_location_matrix_changed =false;
		set_location_matrix_inner(m_location_matrix);
		m_location_matrix.Decompose(m_scale, m_quat, m_position);
		//TODO 这里缺一个quat 到 欧拉的转换
	}
}

void C3DEntity::apply_prs_changed()
{
	if (m_prs_changed)
	{
		m_prs_changed = false;
		H3DAngles angle(RAD2DEG(m_eula.y), RAD2DEG(m_eula.z), RAD2DEG(m_eula.x));
		m_quat = angle.ToQuat();
		//angle生成的3x3矩阵放到4x4里需要转置，四元数和3x3矩阵是一致的
		m_quat = m_quat.Inverse();

		set_location_matrix_inner(m_position, m_quat, m_scale);
	}
}

#endif

void C3DEntity::reload()
{
	if (m_move_object)
	{
		m_move_object->ReloadAll();
	}
}

bool C3DEntity::IsRayHit( const H3DVec3& mousePos, const H3DVec3& dir, H3DVec3& hitpoint )
{
    CInteract_Ray ray(mousePos,dir);
    float fScale;
    if (Hit_None != m_OBB->Hit(ray,hitpoint,fScale))
    {
        return true;
    }
    return false;
}

void C3DEntity::SetObbPos( const H3DVec3& pos,const H3DVec3& size,const H3DVec3& offset )
{
    if(!m_OBB)
        m_OBB = new CInteract_OBB();
    H3DMat4 posTemp;
    posTemp.Identity();
    m_OBB->SetInitWorldTransform(posTemp);
    m_OBB->SetLenghtWeithHight(size.x,size.y,size.z);
    posTemp = this->get_world_location_matrix();
    posTemp[0].w = pos.x + offset.x;
    posTemp[1].w = pos.y + offset.y;
    posTemp[2].w = pos.z + offset.z;
    m_OBB->SetWorldTransform(posTemp);
}

bool C3DEntity::IsMouseOnActor( int mouseX,int mouseY,int gameH,int gameW ,float& distance, H3DVec4* viewPort /*= NULL*/ )
{
    H3DI::IRender* pIRender = m_actor->GetManager()->get_graphic_res_mng()->get_render_system()->get_IRender(); 
    int realX = mouseX;
    int realY = gameH - mouseY;
    if(NULL != viewPort)
    {
        realX = mouseX - viewPort->x;
        int tempY = gameH - viewPort->y - viewPort->z;
        realY = gameH - mouseY - tempY;
    }
    H3DVec3 raypos;
    H3DVec3 raydir;
    H3DVec3 hitPoint;
    pIRender->GetMouseRayDir(realX,realY,&raypos,&raydir);
    bool r = this->IsRayHit(raypos,raydir,hitPoint);
    H3DVec3 vLength = hitPoint - raypos;
    distance = vLength.LengthSqr();
    return r;
}

void C3DEntity::remove_from_level(CSceneEntity* level)
{
	assert(level && level == m_level_entity);
	if (get_IMoveObject() && is_added_to_level())
	{
		m_level_entity->get_IScene()->DetachModel(get_IMoveObject());
	}
	m_level_entity = NULL;
}

void C3DEntity::setColorFactor(float* color, int size)
{
	if (size>4)
	{
		size = 4;
	}

	m_set_color_factor = true;
	
	m_color_factor_size = size;
	for (int i=0; i<size; ++i)
	{
		m_color_factor[i] = color[i];
	}

	if (m_move_object)
	{
		m_move_object->SetMaterialColorFactor(color, size);
	}
}

void C3DEntity::set_diffuse_color_factor(float* color0, int size0, float* color1, int size1, int meshPart, bool bUpdateChildren)
{
	if (size0>4)
	{
		size0 = 4;
	}
	if (size1>4)
	{
		size1 = 4;
	}

	m_set_diffuse_color_factor = true;

	m_diffuse_color_factor_size_0 = size0;
	for (int i=0; i<size0; ++i)
	{
		m_diffuse_color_factor_0[i] = color0[i];
	}
	m_diffuse_color_factor_size_1 = size1;
	for (int i=0; i<size1; ++i)
	{
		m_diffuse_color_factor_1[i] = color1[i];
	}
	m_mesh_part = meshPart;
	m_update_children = bUpdateChildren;

	if (m_move_object)
	{
		m_move_object->SetDiffuseColorFactor(color0, size0, color1, size1, meshPart, bUpdateChildren);
	}
}

void C3DEntity::ClearDiffuseColorFactor(int meshPart, bool bUpdateChildren)
{
	if (m_move_object)
	{
		float fC0[4] = {1.f, 1.f, 1.f, 1.f};
		float fC1[4] = {0.f, 0.f, 0.f, 1.f};
		m_move_object->SetDiffuseColorFactor(fC0, 4, fC1, 4, meshPart, bUpdateChildren);
	}
}

void C3DEntity::add_to_level(CSceneEntity* level)
{
	assert(level && m_level_entity == NULL);
	m_level_entity = level;
	if (m_move_object)
	{
		m_move_object->Update(0);
		level->get_IScene()->AttachModel(m_move_object, m_level_layer);
	}
}

void C3DEntity::update_appearance()
{
	//处理Cache列表
	if (!m_cache_op_list.empty())
	{
		bool finish = true;
		for (OpIterator it=m_cache_op_list.begin(); it!=m_cache_op_list.end(); ++it)
		{
			if ((*it)->get_state() != OPS_FINISH)
			{
				finish = false;
			}
		}

		//Cache列表全部完成直接生效
		if (finish && m_move_object)
		{
			for (OpIterator it=m_cache_op_list.begin(); it!=m_cache_op_list.end(); ++it)
			{
				(*it)->do_op(this);
				(*it)->release();	
			}
			m_cache_op_list.clear();
		}
		else
		{
			for (OpIterator it=m_cache_op_list.begin(); it!=m_cache_op_list.end(); ++it)
			{
				m_execute_op_list[m_priority].push_back(*it);
			}
			m_cache_op_list.clear();
		}
	}

	//重复调用时不产生新的显示标志位
	if (!m_execute_op_list[m_priority].empty())
	{
		Operation* op = m_execute_op_list[m_priority].back();
		if (op->get_type() != OPT_SHOW)
		{
			ShowOperation* pOp = new ShowOperation(m_is_first_view);
			m_execute_op_list[m_priority].push_back(pOp);
			m_is_first_view = false;
		}
	}

	//默认设为低优先级
	m_priority = 0;
}

void C3DEntity::appearance_priority(int priority)
{
	assert(priority>=0 && priority<PRIORITY_COUNT);
	if (priority>=0 && priority<PRIORITY_COUNT)
	{
		m_priority = priority;
	}
}

void C3DEntity::update_execute_op()
{
	if (!m_first_op_list.empty())
	{
		update_first_op();
	}
	else
	{
		for (int i=PRIORITY_COUNT-1; i>=0; --i)
		{
			if (!m_execute_op_list[i].empty())
			{
				update_execute_op(m_execute_op_list[i], m_finish_op_list[i]);
			}
		}
	}
}

void C3DEntity::update_execute_op(OpList& execute_list, OpList& finish_list)
{
	Operation* pOp = 0;
	for (OpIterator it=execute_list.begin(); it!=execute_list.end();)
	{
		pOp = *it;
		OP_TYPE pType = pOp->get_type();
		OP_STATE pState = pOp->get_state();

		//显示标志位
		if (pType == OPT_SHOW)
		{
			//显示标志位不在队首不执行
			if (it != execute_list.begin())
			{
				it++;
				continue;
			}

			if (m_entity_state==ES_HIDE && ((ShowOperation*)pOp)->is_first_view())
			{
				m_first_op_list = finish_list;
				finish_list.clear();
				m_entity_state = ES_PRE;
				m_move_object->SetFakeRender(true);
			}
			else
			{
				update_finish_op(finish_list);
			}

			pOp->release();
			execute_list.erase(it++);
			continue;
		}

		if (pState == OPS_WAIT)
		{
			//材质LOD之前有未处理的OP就直接return
			if (pType==OPT_LOD && (it!=execute_list.begin() || !finish_list.empty()))
			{
				return;
			}
			else
			{
				pOp->check_state();
				++it;
				continue;
			}
		}

		pState = pOp->check_state();
		switch(pState)
		{
		case OPS_EXECUTE:
			return;
		case OPS_FINISH:
			{
				if (pOp->is_view())
				{
					pOp->do_op(this);
					pOp->release();
				}
				else
				{
					finish_list.push_back(pOp);
				}
				execute_list.erase(it++);
			}
			break;
		case OPS_STOP:
			{
				pOp->release();
				execute_list.erase(it++);
			}
			break;
		default:
			break;
		}
	}
}

void C3DEntity::update_finish_op(OpList& finish_list)
{
	Operation* pOp = 0;
	for (OpIterator it=finish_list.begin(); it!=finish_list.end(); ++it)
	{
		pOp = *it;
		if (pOp->get_state() == OPS_FINISH)
		{
			pOp->do_op(this);
		}
		pOp->release();
	}
	finish_list.clear();
}

void C3DEntity::update_first_op()
{
	if (!m_first_op_list.empty())
	{
		if (s_render_count==0)
		{
			for (OpIterator it=m_first_op_list.begin(); it!=m_first_op_list.end(); ++it)
			{
				Operation* pOp = *it;
				if (pOp->get_state() == OPS_FINISH)
				{
					pOp->do_op(this);
				}
				pOp->release();
			}
			m_first_op_list.clear();
		}
		else
		{
			OpIterator it=m_first_op_list.begin();
			for (unsigned int i=0; i<=s_render_count && it!=m_first_op_list.end(); ++i)
			{
				Operation* pOp = *it;
				if (pOp->get_state() == OPS_FINISH)
				{
					pOp->do_op(this);
				}
				pOp->release();
				it = m_first_op_list.erase(it);
			}
		}

		if (m_first_op_list.empty() && m_move_object)
		{
			m_entity_state = ES_VIEW;
			update_engine_entity_visible();
			m_move_object->SetFakeRender(false);
		}
	}
}

void C3DEntity::finish_all_op()
{
	if (m_entity_state!=ES_VIEW && m_move_object)
	{
		m_entity_state = ES_VIEW;
		update_engine_entity_visible();
	}

	for (OpIterator it=m_first_op_list.begin(); it!=m_first_op_list.end(); ++it)
	{
		Operation* pOp = *it;
		if (pOp->get_state() == OPS_FINISH)
		{
			pOp->do_op(this);
		}
		pOp->release();
	}
	m_first_op_list.clear();

	Operation* pOp = 0;
	for (int i=PRIORITY_COUNT-1; i>=0; --i)
	{
		for (OpIterator it=m_execute_op_list[i].begin(); it!=m_execute_op_list[i].end(); ++it)
		{
			pOp = *it;
			//材质LOD若处于等待状态需先处理之前的OP
			if (pOp->get_type()==OPT_LOD && pOp->get_state()==OPS_WAIT)
			{
				update_execute_op();
			}
			pOp->finish_sync();
		}
	}

	update_execute_op();
}

void C3DEntity::clear_all_op()
{
	for (int i=PRIORITY_COUNT-1; i>=0; --i)
	{
		for (OpIterator it=m_execute_op_list[i].begin(); it!=m_execute_op_list[i].end(); ++it)
		{
			(*it)->release();
		}
		m_execute_op_list[i].clear();

		for (OpIterator it=m_finish_op_list[i].begin(); it!=m_finish_op_list[i].end(); ++it)
		{
			(*it)->release();
		}
		m_finish_op_list[i].clear();
	}

	for (OpIterator it=m_cache_op_list.begin(); it!=m_cache_op_list.end(); ++it)
	{
		(*it)->release();
	}
	m_cache_op_list.clear();

	for (OpIterator it=m_first_op_list.begin(); it!=m_first_op_list.end(); ++it)
	{
		(*it)->release();
	}
	m_first_op_list.clear();

	m_entity_op_map.clear();
}

bool C3DEntity::op_list_empty()
{
	for (int i=PRIORITY_COUNT-1; i>=0; --i)
	{
		if (!m_execute_op_list[i].empty())
		{
			return false;
		}
	}
	return true;
}

bool C3DEntity::next_wait()
{
	if (!m_execute_op_list[m_priority].empty())
	{
		Operation* op = m_execute_op_list[m_priority].back();
		if (op->get_state() == OPS_WAIT)
		{
			return true;
		}
	}
	return false;
}

void C3DEntity::stop_entity_op(C3DEntity* entity)
{
	EntityOpIterator it = m_entity_op_map.find(entity);
	if (it != m_entity_op_map.end())
	{
		if (it->second)
		{
			it->second->stop();
		}
	}
}

void C3DEntity::remove_entity_op(C3DEntity* entity)
{
	EntityOpIterator it = m_entity_op_map.find(entity);
	if (it != m_entity_op_map.end())
	{
		m_entity_op_map.erase(it);
	}
}

void C3DEntity::set_engine_object_impl(H3DI::IMoveObject* object)
{
	m_move_object = object;
}

void C3DEntity::after_create_impl()
{
	if (m_move_object)
	{
		m_move_object->SetLocationMatrix(get_world_location_matrix());
		set_is_static(m_is_static);
		if (m_set_color_factor)
		{
			m_move_object->SetMaterialColorFactor(m_color_factor, m_color_factor_size);
		}
		if (m_set_diffuse_color_factor)
		{
			m_move_object->SetDiffuseColorFactor(m_diffuse_color_factor_0, m_diffuse_color_factor_size_0
				, m_diffuse_color_factor_1, m_diffuse_color_factor_size_1, m_mesh_part, m_update_children);
		}
		m_move_object->SetVisibleDist(m_visible_dist);

		if (is_added_to_level())
		{
			m_level_entity->get_IScene()->AttachModel(m_move_object, m_level_layer);
			m_level_entity->get_IScene()->MoveModel(m_move_object);
		}
	}
}

void C3DEntity::set_entity_flags(unsigned int flags, bool is)
{
	if (m_move_object)
	{
		if (is)
		{
			m_move_object->SetFlags(m_move_object->GetFlags() | flags);
		}
		else
		{
			m_move_object->SetFlags(m_move_object->GetFlags() & ~flags);
		}
	}
}

void C3DEntity::set_is_static(bool val)
{
	m_is_static = val;
	set_entity_flags(H3DI::I_ENTITY_STATIC, val);
}

bool C3DEntity::is_render()
{
	return m_entity_state==ES_VIEW;
}

void C3DEntity::set_visible_dist(float dist)
{
	m_visible_dist = dist;
	if (m_move_object)
	{
		m_move_object->SetVisibleDist(m_visible_dist);
	}
}

bool C3DEntity::is_loaded()
{
	return m_move_object!=0;
}

void C3DEntity::UnloadResources()
{
	if (m_entity_state==ES_NONE)
	{
		return;
	}

	clear_all_op();
	m_entity_state = ES_NONE;
	if (m_move_object)
	{
		m_res_mng->get_entity_factory()->release_engine_entity(m_move_object);
		set_engine_object_impl(0);
	}
}

void C3DEntity::ReloadResources(bool is_sync, int priority)
{
	if (m_move_object && m_entity_state!=ES_NONE)
	{
		return;
	}

	create_engine_object(is_sync, priority);
}