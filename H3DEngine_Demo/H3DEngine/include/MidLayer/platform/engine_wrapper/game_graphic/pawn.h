/**
* @file pawn.h
* 这个文件的主要功能是：
* 
* @author 程陈(Chengchen@h3d.com.cn)
* 
* 更改日志
* ----------------------------------------------------
* ver 1.0.0
*       by 程陈2010.1.27
*/

#ifndef _PAWN_H_
#define _PAWN_H_

#include "game_graphic_header.h"

/**
* \brief
* Pawn
* 
* Pawn是所有可控实体的基类, 该类组织了多个媒体,
* 以及各种媒体的绘制方式
*/
class Pawn
{
public:
    Pawn();

    virtual ~Pawn();

	/**
	* @brief
	*		获取对象名称
	* @param
	*		name：【IN】对象的名称
	*/
	void set_name(const std::string& name);
	/**
	* @brief
	*		获取对象名称
	* @return
	*		对象的名称
	*/
	const std::string& get_name() const;

	/**
	* @brief
	*		更新,更新时间为毫秒
	* @brief
	*		更新时间
	*/
	void update(float t);

	/**
	* @brief
	*		绘制pawn
	*/
	void render(CRenderSystem& render_system);

//////////////////////////////////////////////////////////////////////////
	/**
	* @brief
	*		获取具有指定entity_name的可控实体
	* @param
	*		entity_name：【IN】子可控实体对象的名称
	* @return
	*		返回查找到的子类对象的指针，如果没有，返回NULL
	*/
	virtual CGraphicEntity* find_entity(const std::string& entity_name) const ;

	/**
	* @brief
	*		添加子可控实体
	* @param
	*		entity：【IN】要添加的子可控实体对象指针
	* @return
	*		添加成功：返回true
	*		添加失败：返回false
	*/
	virtual bool			add_entity(CGraphicEntity* entity) ;

	/**
	* @brief
	*		移除子可控实体对象
	* @param
	*		entity：【IN】要移除的子可控实体对象的指针
	* @return
	*		删除成功：返回true
	*		删除失败：返回false	
	*/
	virtual bool			remove_entity(CGraphicEntity* entity) ;

	/**
	* @brief
	*		移除所有子可控实体
	*/
	virtual void clear_entities() ;

//////////////////////////////////////////////////////////////////////////
	/**
	* @brief
	*		添加pawn、
	* @param
	*		pawn：【IN】要添加的pawn
	* @return
	*		添加成功：返回true
	*		添加失败：返回false
	*/
	virtual bool add_pawn(Pawn* pawn);

	/**
	* @brief
	*		删除pawn
	* @param
	*		pawn：【IN】要删除的pawn
	*/
	virtual bool remove_pawn(Pawn* pawn);

	/**
	* @brief
	*	清除所有的pawn
	*/
	virtual void clear_pawns();


	/**
	* @brief
	*		获取pawn
	* @param
	*		pawn_name：【IN】要获取的pawn的名称
	*/
	virtual Pawn*	find_pawn(const std::string& pawn_name) const;
//////////////////////////////////////////////////////////////////////////
	/**
	* @brief
	*		设置Pawn的PawnControl
	*/
	virtual void set_pawn_control(IPawnControl* pawn_control);
	/**
	* @brief
	*		获取Pawn的PawnControl
	*/
	virtual IPawnControl* get_pawn_control();
//////////////////////////////////////////////////////////////////////////
	virtual H3DVec3 get_position(void) const;

	virtual void set_position(const H3DVec3& pos);
	virtual H3DVec3 get_rotation(void) const;
	virtual void set_rotation(const H3DVec3& rotation);
	virtual H3DVec3 get_scale(void) const;
	virtual void set_scale(const H3DVec3& scale);

	virtual const H3DMat4& get_location_matrix(void) const;
	virtual void set_location_matrix(const H3DMat4& location);

	virtual bool get_visible(void) const;
	virtual void set_visible(bool visible);

public:
	typedef std::list<CGraphicEntity*> EntityList;
	typedef std::list<Pawn*> PawnList;
	typedef std::list<IPawnControl*> PawnControlList;

private:
	/**
	* @brief
	*		获取指定列表中指定名称的元素
	* @param
	*		element_name：【IN】要获取的元素的名称
	*/
	template<class T>
	struct name_equal
		: public std::binary_function<T*, std::string, bool>
	{	// functor for operator>
		bool operator()(T* obj, std::string obj_name) const
		{
			assert(NULL != obj);
			return (obj->get_name() ==  obj_name);
		}
	};

protected:
	EntityList			m_graphic_entity_collection; ///< 可控实体结婚
	PawnList			m_child_pawn_collection;	///< 子pawn集合
	PawnControlList     m_control_collection; ///< 控制器集合

	std::string			m_name; ///< pawn的名称
	H3DMat4				m_loc_matrix; ///< Pawn的位置
	bool				m_visible; ///< Pawn是否可见

	H3DVec3					m_position; ///< 相对位置
	H3DVec3					m_eula; ///< 相对欧拉角
	H3DQuat					m_quat; ///< 相对四元素
	H3DVec3					m_scale;///< 相对缩放，除了在Model中都不可修改。初始值为(1.0f,1.0f,1.0f)
	H3DMat4					m_location_matrix; ///< 位置矩阵
};

#endif //_PAWN_H_
