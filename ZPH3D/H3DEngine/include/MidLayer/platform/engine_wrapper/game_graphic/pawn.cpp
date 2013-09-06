/**
* @file pawn.cpp
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

Pawn::Pawn()
: m_name(""), m_visible(true)
{
	m_quat.Identity();
	m_position.Set(0.0f,0.0f,0.0f);
	m_scale.Set(1.0f,1.0f,1.0f);
	m_eula.Set(0.0f,0.0f,0.0f);
	m_location_matrix.Identity();
}

Pawn::~Pawn()
{
	//删除现有的子Pawn
	//删除现有的PawnControl
	//删除现有的Entity
}

bool Pawn::add_pawn(Pawn* pawn)
{
	assert(NULL != pawn);

	Pawn* q = find_pawn(pawn->get_name());
	if (NULL != q)
	{
		if (q != pawn)
		{
			assert(false);
			return false;
		}

		return true;
	}

	m_child_pawn_collection.push_back(pawn);

	return true;
}

bool Pawn::remove_pawn(Pawn* pawn)
{
	assert(NULL != pawn);

	Pawn* p = find_pawn(pawn->get_name());
	if (p == pawn)
	{
		m_child_pawn_collection.remove(pawn);
	}

	return true;
}

void Pawn::clear_pawns()
{
	m_child_pawn_collection.clear();
}

Pawn* Pawn::find_pawn(const std::string& pawn_name) const
{
	PawnList::const_iterator it = std::find_if(m_child_pawn_collection.begin(),m_child_pawn_collection.end(), bind2nd(name_equal<Pawn>(), pawn_name));

	if (it != m_child_pawn_collection.end()) //如果找到
	{
		return (*it);
	}

	return NULL;
}

bool Pawn::add_entity(CGraphicEntity* entity)
{
	assert(NULL != entity);

	CGraphicEntity* q = find_entity(entity->get_name());
	if (NULL != q)
	{
		if (q != entity)
		{
			assert(false);
			return false;
		}

		return true;
	}

	m_graphic_entity_collection.push_back(entity);

	return true;
}

bool Pawn::remove_entity(CGraphicEntity* entity)
{
	if (find_entity(entity->get_name()) == NULL)
	{
		return true;
	}
	m_graphic_entity_collection.remove(entity);

	return true;
}

void Pawn::clear_entities()
{
	m_graphic_entity_collection.clear();
}

CGraphicEntity* Pawn::find_entity(const std::string& entity_name) const
{
	EntityList::const_iterator it = std::find_if(m_graphic_entity_collection.begin(),m_graphic_entity_collection.end(), bind2nd(name_equal<CGraphicEntity>(), entity_name));

	if (it != m_graphic_entity_collection.end()) //如果找到
	{
		return (*it);
	}

	return NULL;
}

void Pawn::set_name(const std::string& name)
{
	m_name = name;
}

const std::string& Pawn::get_name() const
{
	return m_name;
}

void Pawn::update(float t)
{
	for (EntityList::iterator it = m_graphic_entity_collection.begin();
		it != m_graphic_entity_collection.end(); ++it)
	{
		(*it)->update(t);
	}

	//更新PawnControl
	for (PawnControlList::iterator it = m_control_collection.begin(); it!= m_control_collection.end(); ++it)
	{
		(*it)->update(t);
	}

	//进行子Pawn的update
}

void Pawn::render(CRenderSystem& render_system)
{
	if (!get_visible())
	{
		return;
	}

	for (EntityList::iterator it = m_graphic_entity_collection.begin();
		it != m_graphic_entity_collection.end(); ++it)
	{
		(*it)->render(render_system);
	}

	//进行子Pawn的render
}


H3DVec3 Pawn::get_position(void) const
{
	return m_position;
}

void Pawn::set_position(const H3DVec3& pos)
{
	H3DMat4 mat;
	m_position = pos;
	mat.Compose(m_scale, m_quat, m_position);
	set_location_matrix(mat);
}

H3DVec3 Pawn::get_rotation(void) const
{
	return m_eula;
}

void Pawn::set_rotation(const H3DVec3& rotation)
{
	H3DAngles angle(RAD2DEG(rotation.y), RAD2DEG(rotation.z), RAD2DEG(rotation.x));
	m_quat = angle.ToQuat();
	//angle生成的3x3矩阵放到4x4里需要转置，四元数和3x3矩阵是一致的
	m_quat = m_quat.Inverse();
	m_eula = rotation;
	H3DMat4 mat;
	mat.Compose(m_scale, m_quat, m_position);
	set_location_matrix(mat);
}

H3DVec3 Pawn::get_scale(void) const
{
	return m_scale;
}

void Pawn::set_scale(const H3DVec3& scale)
{
	H3DMat4 mat;
	m_scale = scale;
	mat.Compose(m_scale, m_quat, m_position);
	set_location_matrix(mat);
}

const H3DMat4& Pawn::get_location_matrix(void) const
{
	return m_loc_matrix;
}

void Pawn::set_location_matrix(const H3DMat4& location)
{
	H3DMat4 transfer_loc = location * get_location_matrix().Inverse();

	m_loc_matrix = location;

	//开始更新子实体的位置
	for (EntityList::iterator it = m_graphic_entity_collection.begin(); it != m_graphic_entity_collection.end(); ++it)
	{
			IMoveEntity* entity = dynamic_cast<IMoveEntity*>(*it);
			H3DMat4 mat = transfer_loc * entity->get_location_matrix();
			entity->set_location_matrix(mat);
	}
}

bool Pawn::get_visible(void) const
{
	return m_visible;
}

void Pawn::set_visible(bool visible)
{
	m_visible = visible;
}

void Pawn::set_pawn_control(IPawnControl* pawn_control)
{
	;
}

IPawnControl* Pawn::get_pawn_control()
{
	return NULL;
}

