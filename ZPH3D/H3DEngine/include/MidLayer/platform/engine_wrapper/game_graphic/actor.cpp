/**
* @file Actor.cpp
* 这个文件的主要功能是：
* 
* @author 程陈(Chengchen@h3d.com.cn)
* 
* 更改日志
* ----------------------------------------------------
* ver 1.0.0
*       by 程陈2010.1.27
*/

#include "game_graphic_body.h"
#include "platform/common/interact_obb.h"
#include "platform/common/interact_ray.h"
Actor::Actor(ActorManager*	actor_manager, const char* name)
: m_name(name)
, m_visible(true)
, m_external_visible(true)
, m_actor_manager(actor_manager)
, m_OBB(NULL)
{
	m_location_matrix.Identity();
	m_world_location_matrix.Identity();

	assert(actor_manager != NULL);
	assert((NULL != name) && (strlen(name) != 0));

	m_parent = NULL;
	m_link_flag = AttributeLinkVisible | AttributeLinkLocation;

	reset_actor_iter();
	reset_control_iter();
	reset_entity_iter();

	m_quat.Identity();
	m_position.Set(0.0f,0.0f,0.0f);
	m_scale.Set(1.0f,1.0f,1.0f);
	m_eula.Set(0.0f,0.0f,0.0f);
	m_location_matrix.Identity();

#ifdef _LOCATION_SET_OPTIMIZE_
	m_prs_changed = false;
	m_location_matrix_changed = false;
#endif
}

Actor::~Actor()
{
	//删除现有的ActorControl
	clear_actor_controls();

	//删除现有的Entity
	clear_entities();

	//删除现有的子Actor
	clear_actors();

    if(m_OBB)
        delete m_OBB;
}

bool Actor::add_actor(Actor* actor)
{
	assert(NULL != actor);

	Actor* q = find_actor(actor->get_name());
	if (NULL != q)
	{
		if (q != actor)
		{
			assert(false);
			return false;
		}

		return true;
	}

	m_child_actor_collection.push_back(actor);
	actor->set_parent(this);

	reset_actor_iter();

	return true;
}

bool Actor::remove_actor(Actor* actor)
{
	assert(NULL != actor);

	Actor* p = find_actor(actor->get_name());
	if (p == actor)
	{
		m_child_actor_collection.remove(actor);
		actor->set_parent(NULL);

		reset_actor_iter();
		return true;
	}
	else
	{
		return false;
	}
}

void Actor::clear_actors()
{
	//删除现有的子Actor
	for (ActorList::iterator it = m_child_actor_collection.begin();
		it != m_child_actor_collection.end();)
	{
		delete *it++;
	}
	m_child_actor_collection.clear();

	reset_actor_iter();
}

Actor* Actor::find_actor(const char* actor_name) const
{
	ActorList::const_iterator it = std::find_if(m_child_actor_collection.begin(),m_child_actor_collection.end(), bind2nd(name_equal<Actor>(), actor_name));

	if (it != m_child_actor_collection.end()) //如果找到
	{
		return (*it);
	}

	return NULL;
}

bool Actor::add_entity(CMediaEndEntity* entity)
{
	assert(NULL != entity);

	CMediaEndEntity* q = find_entity(entity->get_name());
	if (NULL != q)
	{
		if (q != entity)
		{
			assert(false);
			return false;
		}

		return true;
	}

	m_media_end_entity_collection.push_back(entity);
	entity->set_actor(this);
	reset_entity_iter();

	return true;
}

bool Actor::remove_entity(CMediaEndEntity* entity)
{
	if ( !entity )
	{
		return false;
	}
	CMediaEndEntity* p = find_entity(entity->get_name());
	if (p == entity)
	{
		m_media_end_entity_collection.remove(entity);
		entity->set_actor(NULL);
		reset_entity_iter();
		return true;
	}
	else
	{
		return false;
	}
}

void Actor::clear_entities()
{
	for (EntityList::iterator it = m_media_end_entity_collection.begin();
		it != m_media_end_entity_collection.end();)
	{
		m_actor_manager->release_entity(*it++);
	}

	m_media_end_entity_collection.clear();
	reset_entity_iter();
}

CMediaEndEntity* Actor::find_entity(const std::string& entity_name) const
{
	EntityList::const_iterator it = std::find_if(m_media_end_entity_collection.begin(),m_media_end_entity_collection.end(), bind2nd(name_equal<CMediaEndEntity>(), entity_name));

	if (it != m_media_end_entity_collection.end()) //如果找到
	{
		return (*it);
	}

	return NULL;
}

CMediaEndEntity* Actor::find_entity_recursive(unsigned int entity_id)
{
	EntityList::iterator it = m_media_end_entity_collection.begin();
	for (; it != m_media_end_entity_collection.end(); ++it)
	{
		if ((*it)->get_entity_id() == entity_id)
		{
			return *it;
		}
	}

	ActorList::iterator actor_it = m_child_actor_collection.begin();
	for (; actor_it != m_child_actor_collection.end(); ++actor_it)
	{
		CMediaEndEntity* entity = (*actor_it)->find_entity_recursive(entity_id);
		if (entity)
		{
			return entity;
		}
	}

	return 0;
}

bool Actor::contain_entity_type_recursive(int obj_type, EntityList& entitylist)
{
	bool bRet = false;

	EntityList::iterator it = m_media_end_entity_collection.begin();
	for (; it != m_media_end_entity_collection.end(); ++it)
	{
		if ((*it)->is_kind_of(obj_type))
		{
			entitylist.push_back(*it);
			bRet = true;
		}
	}

	ActorList::iterator actor_it = m_child_actor_collection.begin();
	for (; actor_it != m_child_actor_collection.end(); ++actor_it)
	{
		if ((*actor_it)->contain_entity_type_recursive(obj_type, entitylist))
		{
			bRet = true;
		}
	}

	return bRet;
}

void Actor::set_name(const char* name)
{
	m_name = name;
}

const char* Actor::get_name() const
{
	return m_name.c_str();
}

#ifdef _LOCATION_SET_OPTIMIZE_

void Actor::update_location()
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

void Actor::apply_location_changed()
{
	if (m_location_matrix_changed)
	{
		m_location_matrix_changed =false;
		set_location_matrix_inner(m_location_matrix);
		m_location_matrix.Decompose(m_scale, m_quat, m_position);
		//TODO 这里缺一个quat 到 欧拉的转换
	}
}

void Actor::apply_prs_changed()
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

void Actor::update(float t)
{
	ActorControlList::iterator acend = m_control_collection.end();
	for (ActorControlList::iterator it = m_control_collection.begin(); it!= acend; ++it)
	{
		if ((*it)->in_charge_of_actor_update())
		{
			(*it)->update(t);
			return;
		}
	}
	
	//更新ActorControl
	for (ActorControlList::iterator it = m_control_collection.begin(); it!= acend; ++it)
	{
		(*it)->update(t);
	}

#ifdef _LOCATION_SET_OPTIMIZE_
	update_location();
#endif

	//进行子Actor的render
	ActorList::iterator caend = m_child_actor_collection.end();
	for (ActorList::iterator it = m_child_actor_collection.begin(); it!= caend; ++it)
	{
		(*it)->update(t);
	}

	for (EntityList::iterator it = m_media_end_entity_collection.begin();
		it != m_media_end_entity_collection.end(); ++it)
	{
		CMediaEndEntity* ee = *it;

		ee->update(t);

		if (ee->is_kind_of(MEDIA_END_ENTITY_TYPE_PREPASS_LIGHT))
		{
			CPrePassLightEntity* light = dynamic_cast<CPrePassLightEntity*>(ee);
			if (light->is_entity_light_enable())
			{
				//将灯光添加到起作用的prepasslight内
				m_actor_manager->add_using_light(light);
			}
		}
	}
}

void Actor::render(CRenderSystem& render_system)
{
	if (!m_visible)
	{
		return;
	}

	EntityList::iterator meend = m_media_end_entity_collection.end();
	for (EntityList::iterator it = m_media_end_entity_collection.begin();
		it != meend; ++it)
	{
		(*it)->render(render_system);
	}

	//进行子Actor的render
	ActorList::iterator acend = m_child_actor_collection.end();
	for (ActorList::iterator it = m_child_actor_collection.begin(); it!= acend; ++it)
	{
		(*it)->render(render_system);
	}
}

void Actor::setColorFactor(float* color, int size)
{
	EntityList::iterator meend = m_media_end_entity_collection.end();
	for (EntityList::iterator it = m_media_end_entity_collection.begin();
		it != meend; ++it)
	{
		(*it)->setColorFactor(color, size);
	}

	//进行子Actor的render
	ActorList::iterator acend = m_child_actor_collection.end();
	for (ActorList::iterator it = m_child_actor_collection.begin(); it!= acend; ++it)
	{
		(*it)->setColorFactor(color, size);
	}
}

unsigned int Actor::pushHardwareSelect(CRenderSystem& render_system, bool sel_line)
{
	if (!get_visible())
	{
		return 0;
	}

	int count = 0;

	for (EntityList::iterator it = m_media_end_entity_collection.begin();
		it != m_media_end_entity_collection.end(); ++it)
	{
		if ((*it)->pushHardwareSelect(render_system, sel_line))
			count++;
	}

	//进行子Actor的render
	for (ActorList::iterator it = m_child_actor_collection.begin(); it!= m_child_actor_collection.end(); ++it)
	{
		count += (*it)->pushHardwareSelect(render_system, sel_line);
	}

	return count;
}


const H3DMat4& Actor::get_location_matrix(void)
{
#ifdef _LOCATION_SET_OPTIMIZE_
	apply_prs_changed();
#endif

	return m_location_matrix;
}

H3DVec3 Actor::position(void) const
{
#ifdef _LOCATION_SET_OPTIMIZE_
	const_cast<Actor*>(this)->apply_location_changed();
#endif
	return m_position;
}

void Actor::set_position(const H3DVec3& pos)
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

H3DVec3 Actor::rotation() const
{
#ifdef _LOCATION_SET_OPTIMIZE_
	apply_location_changed();
#endif
	return m_eula;
}

void Actor::set_rotation(const H3DVec3& obj_rotation)
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

void Actor::set_scale(const H3DVec3& scale)
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

H3DVec3 Actor::get_scale() const
{
#ifdef _LOCATION_SET_OPTIMIZE_
	const_cast<Actor*>(this)->apply_location_changed();
#endif
	return m_scale;
}

void Actor::set_location_matrix(const H3DMat4& location)
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

void Actor::set_location_matrix_inner(const H3DMat4& location)
{
	m_location_matrix = location;

	//开始更新子Entity的位置
	for (EntityList::iterator it = m_media_end_entity_collection.begin(); it != m_media_end_entity_collection.end(); ++it)
	{
		IMoveEntity* entity = dynamic_cast<IMoveEntity*>(*it);
		/*并不是所有的实体派生自IMoveEntity，所以转型有可能失败，需要判断一下，
		比如x52_system_config实体(CSystemEntity)就会失败*/
		if (entity)
			entity->set_location_matrix(entity->get_location_matrix());
		

		C3DEntity* entity3d = dynamic_cast<C3DEntity*>(*it);
		/*和上面同理，并不是所有的entity实体都是派生自C3DEntity，所以转型有可能失败，需要判断一下*/
		if (entity3d && entity3d->is_added_to_level() && entity3d->get_IMoveObject())
			m_actor_manager->get_level_entity()->get_ILevel()->MoveModel(entity3d->get_IMoveObject());
	}

	//更新子Actor的位置
	for (ActorList::iterator it = m_child_actor_collection.begin(); it!= m_child_actor_collection.end(); ++it)
	{
		int link_flag = (*it)->get_link_flag();
		if (link_flag & AttributeLinkLocation)
		{
			(*it)->set_location_matrix((*it)->get_location_matrix());
		}
	}
}

void Actor::set_location_matrix_inner(const H3DVec3&pos, const H3DQuat& quat, const H3DVec3& scale)
{
	H3DMat4 mat;
	mat.Compose(scale, quat, pos);

	set_location_matrix_inner(mat);
}

const H3DMat4& Actor::get_world_location_matrix(void)
{
#ifdef _LOCATION_SET_OPTIMIZE_
	apply_prs_changed();
#endif
	if (get_parent() != NULL)
	{
		m_world_location_matrix = get_parent()->get_world_location_matrix() * m_location_matrix;

		return m_world_location_matrix;
	}

	return m_location_matrix;
}

//void Actor::set_world_location_matrix(const H3DMat4& location)
//{
//	H3DMat4 transfer_loc = location * get_world_location_matrix().Inverse();
//
//	m_world_location_matrix = location;
//
//	//开始更新子Entity的位置
//	for (EntityList::iterator it = m_media_end_entity_collection.begin(); it != m_media_end_entity_collection.end(); ++it)
//	{
//			IMoveEntity* entity = dynamic_cast<IMoveEntity*>(*it);
//			H3DMat4 mat = transfer_loc * entity->get_location_matrix();
//			entity->set_location_matrix(mat);
//	}
//
//	//更新子Actor的位置
//	for (ActorList::iterator it = m_child_actor_collection.begin(); it!= m_child_actor_collection.end(); ++it)
//	{
//		int link_flag = (*it)->get_link_flag();
//		if (link_flag & AttributeLinkLocation)
//		{
//			H3DMat4 mat = transfer_loc * (*it)->get_world_location_matrix();
//			(*it)->set_world_location_matrix(mat);
//		}
//	}
//}

bool Actor::get_visible(void) const
{
	return m_visible;
}

void Actor::set_visible(bool visible)
{
	m_visible = visible;
	for (EntityList::iterator it = m_media_end_entity_collection.begin(); it != m_media_end_entity_collection.end(); ++it)
	{
		(*it)->set_visible(visible);
	}

	//更新子Actor的位置
	for (ActorList::iterator it = m_child_actor_collection.begin(); it!= m_child_actor_collection.end(); ++it)
	{
		(*it)->set_visible(visible);
	}
}

bool Actor::get_external_visible(void) const
{
	return m_external_visible;
}

void Actor::set_external_visible(bool visible)
{
	m_visible = visible;
	for (EntityList::iterator it = m_media_end_entity_collection.begin(); it != m_media_end_entity_collection.end(); ++it)
	{
		(*it)->set_external_visible(visible);
	}

	//更新子Actor的位置
	for (ActorList::iterator it = m_child_actor_collection.begin(); it!= m_child_actor_collection.end(); ++it)
	{
		(*it)->set_external_visible(visible);
	}
}

IActorControl* Actor::find_actor_control(const std::string& control_name) const
{
	ActorControlList::const_iterator it = std::find_if(m_control_collection.begin(),m_control_collection.end(), bind2nd(name_equal<IActorControl>(), control_name));

	if (it != m_control_collection.end()) //如果找到
	{
		return (*it);
	}

	return NULL;
}

bool Actor::add_actor_control(IActorControl* control)
{
	assert(NULL != control);

	IActorControl* q = find_actor_control(control->get_name());
	if (NULL != q)
	{
		if (q != control)
		{
			assert(false);
			return false;
		}

		return true;
	}

	m_control_collection.push_back(control);
	control->set_actor(this);

	reset_control_iter();

	return true;
}

bool Actor::remove_actor_control(IActorControl* control)
{
	assert(NULL != control);

	IActorControl* p = find_actor_control(control->get_name());
	if (p == control)
	{
		m_control_collection.remove(control);
		control->set_actor(NULL);
		reset_control_iter();
		return true;
	}
	else
	{
		return false;
	}
}

void Actor::clear_actor_controls()
{
	for (ActorControlList::iterator it = m_control_collection.begin();
		it != m_control_collection.end();)
	{
		delete *it++;
	}
	
	m_control_collection.clear();
	reset_control_iter();
}

void Actor::set_link_flag(int link_flag)
{
	m_link_flag = link_flag;
}

int Actor::get_link_flag()
{
	return m_link_flag;
}

void Actor::set_parent(Actor* parent)
{
	H3DMat4 location = get_location_matrix();
	
	m_parent = parent;
	
	set_location_matrix(location);
}

Actor* Actor::get_parent()
{
	return m_parent;
}

ActorManager* Actor::get_actor_mngr()
{
	return m_actor_manager;
}

void Actor::reset_entity_iter()
{
	m_entity_iter = m_media_end_entity_collection.begin();
}

CMediaEndEntity* Actor::next_entity()
{
	if (m_entity_iter == m_media_end_entity_collection.end())
	{
		return NULL;
	}

	return *(m_entity_iter++);
}

void Actor::reset_actor_iter()
{
	m_actor_iter = m_child_actor_collection.begin();
}

Actor* Actor::next_actor()
{
	if (m_actor_iter == m_child_actor_collection.end())
	{
		return NULL;
	}

	return *(m_actor_iter++);
}

void Actor::reset_control_iter()
{
	m_control_iter = m_control_collection.begin();
}

IActorControl* Actor::next_control()
{
	if (m_control_iter == m_control_collection.end())
	{
		return NULL;
	}

	return *(m_control_iter++);
}

bool Actor::get_BBox(BBOX& bbox)
{
	bool has_bbox = false;
	BBOX box;

	//首先获取entity的bbox
	reset_entity_iter();
	while(CMediaEndEntity* entity = next_entity())
	{
		if (dynamic_cast<C3DEntity*>(entity))
		{
			BBOX q = dynamic_cast<C3DEntity*>(entity)->get_BBox();
			if (!has_bbox)
			{
				box = q;
			}
			else
			{
				box.m_min_pos.x = min(box.m_min_pos.x, q.m_min_pos.x);
				box.m_min_pos.y = min(box.m_min_pos.y, q.m_min_pos.y);
				box.m_min_pos.z = min(box.m_min_pos.z, q.m_min_pos.z);

				box.m_max_pos.x = max(box.m_max_pos.x, q.m_max_pos.x);
				box.m_max_pos.y = max(box.m_max_pos.y, q.m_max_pos.y);
				box.m_max_pos.z = max(box.m_max_pos.z, q.m_max_pos.z);
			}
			has_bbox = true;
		}
	}


	//然后获取Actor的bbox
	reset_actor_iter();

	while(Actor* a = next_actor())
	{
		BBOX q;
		if (a->get_BBox(q))
		{
			if (!has_bbox)
			{
				box = q;
			}
			else
			{
				box.m_min_pos.x = min(box.m_min_pos.x, q.m_min_pos.x);
				box.m_min_pos.y = min(box.m_min_pos.y, q.m_min_pos.y);
				box.m_min_pos.z = min(box.m_min_pos.z, q.m_min_pos.z);

				box.m_max_pos.x = max(box.m_max_pos.x, q.m_max_pos.x);
				box.m_max_pos.y = max(box.m_max_pos.y, q.m_max_pos.y);
				box.m_max_pos.z = max(box.m_max_pos.z, q.m_max_pos.z);
			}
			has_bbox = true;
		}
	}

	if (has_bbox)
	{
		bbox = box;
	}
	return has_bbox;
}

bool Actor::contain_entity_recursive(EntityList& entitylist)
{
	bool bRet = true;

	EntityList::iterator it = m_media_end_entity_collection.begin();
	for (; it != m_media_end_entity_collection.end(); ++it)
	{
		entitylist.push_back(*it);
	}

	ActorList::iterator actor_it = m_child_actor_collection.begin();
	for (; actor_it != m_child_actor_collection.end(); ++actor_it)
	{
		(*actor_it)->contain_entity_recursive(entitylist);
	}

	return bRet;
}

bool Actor::IsRayHit( const H3DVec3& pos, const H3DVec3& dir, H3DVec3& hitpoint )
{
	//不显示，就认为点不中
	if(!m_visible)
	{
		return false;
	}
    CInteract_Ray ray(pos,dir);
    float fScale;
    if (Hit_None != m_OBB->Hit(ray,hitpoint,fScale))
    {
        return true;
    }
    return false;
}

bool Actor::IsMouseOnActor( int mouseX,int mouseY,int gameH,int gameW,float& distance, H3DVec4* viewPort )
{
    H3DI::IRender* pIRender = m_actor_manager->get_graphic_res_mng()->get_render_system()->get_IRender(); 
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

void Actor::SetObbPos( const H3DVec3& pos,const H3DVec3& size,const H3DVec3& offset )
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

ActorAnimationControl* Actor::getAnimationControl()
{
	return NULL;
}
