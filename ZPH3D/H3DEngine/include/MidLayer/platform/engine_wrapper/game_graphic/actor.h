/**
* @file Actor.h
* 这个文件的主要功能是：
* 
* @author 程陈(Chengchen@h3d.com.cn)
* 
* 更改日志
* ----------------------------------------------------
* ver 1.0.0
*       by 程陈2010.1.27
*/

#ifndef _ACTOR_H_
#define _ACTOR_H_

#include "game_graphic_header.h"

enum AttributeLinkType //级联标志
{
	AttributeLinkNone = 0, //不和父Actor的任何属性关联
	AttributeLinkLocation = 1 << 0, //关联父Actor的位置
	AttributeLinkVisible =  1 << 1, //关联父Actor的Visible属性
};

/**
* @brief
*		actor是动态效果物体的基类，该类包含了多个子Entity，组成了
*		一个动态效果物体。并且该类通过子类包含的关系，实现了级联效果
* @func
*		1、组织子Entity，Actor代表一个物体，子Entity代表物体的一部分
*		2、实现了级联的效果。通过标志位实现和父Actor的哪个属性关联
*/
class CInteract_OBB;
class H3DI::IRender;
class Actor
{
public:

public:
	typedef std::list<CMediaEndEntity*> EntityList;
	typedef std::list<Actor*> ActorList;
	typedef std::list<IActorControl*> ActorControlList;

    Actor(ActorManager*	actor_manager, const char* name);

    virtual ~Actor();

    ActorManager* GetManager(){return m_actor_manager;}
	/**
	* @brief
	*		获取对象名称
	* @param
	*		name：【IN】对象的名称
	*/
	void set_name(const char* name);
	/**
	* @brief
	*		获取对象名称
	* @return
	*		对象的名称
	*/
	const char* get_name() const;

	/**
	* @brief
	*		更新,更新时间为毫秒
	* @brief
	*		更新时间
	*/
	virtual void update(float t);

	/**
	* @brief
	*		绘制Actor
	*/
	virtual void render(CRenderSystem& render_system);
	//进行硬件选择渲染
	virtual unsigned int pushHardwareSelect(CRenderSystem& render_system, bool sel_line);

//////////////////////////////////////////////////////////////////////////
	/**
	* @brief
	*		获取具有指定entity_name的可控实体
	* @param
	*		entity_name：【IN】子可控实体对象的名称
	* @return
	*		返回查找到的子Entity对象的指针，如果没有，返回NULL
	*/
	virtual CMediaEndEntity* find_entity(const std::string& entity_name) const ;

	/**
	* @brief
	*		添加子可控实体
	* @param
	*		entity：【IN】要添加的子可控实体对象指针
	* @return
	*		添加成功：返回true
	*		添加失败：返回false
	*/
	virtual bool			add_entity(CMediaEndEntity* entity) ;

	/**
	* @brief
	*		移除子可控实体对象
	* @param
	*		entity：【IN】要移除的子可控实体对象的指针
	* @return
	*		删除成功：返回true
	*		删除失败：返回false	
	*/
	virtual bool			remove_entity(CMediaEndEntity* entity) ;

	/**
	* @brief
	*		移除所有子可控实体
	*/
	virtual void clear_entities() ;

	/**
	* @brief
	*		重置control遍历的迭代器
	*/
	virtual void reset_entity_iter();

	/**
	* @brief
	*		查找下一个control
	*/
	virtual CMediaEndEntity* next_entity();

	/**
	* @brief
	*		查找entity
	* @param
	*		entity_id：entity的id
	*/
	virtual CMediaEndEntity* find_entity_recursive(unsigned int entity_id);

	/**
	* @brief
	*		是否包含某种指定类型的entity
	* @param
	*		obj_type：要检测的entity类型
	*		entitylist:复合entity类型的entity列表
	*/
	virtual bool contain_entity_type_recursive(int obj_type, EntityList& entitelist);

//////////////////////////////////////////////////////////////////////////
	/**
	* @brief
	*		添加Actor、
	* @param
	*		Actor：【IN】要添加的Actor
	* @return
	*		添加成功：返回true
	*		添加失败：返回false
	*/
	virtual bool add_actor(Actor* Actor);

	/**
	* @brief
	*		删除Actor
	* @param
	*		Actor：【IN】要删除的Actor
	*/
	virtual bool remove_actor(Actor* Actor);

	/**
	* @brief
	*	清除所有的Actor
	*/
	virtual void clear_actors();


	/**
	* @brief
	*		获取Actor
	* @param
	*		Actor_name：【IN】要获取的Actor的名称
	*/
	virtual Actor*	find_actor(const char* Actor_name) const;

	/**
	* @brief
	*		重置Actor遍历的迭代器
	*/
	virtual void reset_actor_iter();

	/**
	* @brief
	*		查找下一个Actor
	*/
	virtual Actor* next_actor();


//////////////////////////////////////////////////////////////////////////
	
	/**
	* @brief
	*		获取具有指定control_name的IActorControl
	* @param
	*		entity_name：【IN】IActorControl的名称
	* @return
	*		返回查找到的IActorControl对象的指针，如果没有，返回NULL
	*/
	virtual IActorControl* find_actor_control(const std::string& control_name) const ;

	/**
	* @brief
	*		添加IActorControl
	* @param
	*		entity：【IN】要添加的IActorControl对象指针
	* @return
	*		添加成功：返回true
	*		添加失败：返回false
	*/
	virtual bool			add_actor_control(IActorControl* control) ;

	/**
	* @brief
	*		移除IActorControl对象
	* @param
	*		entity：【IN】要移除的IActorControl对象的指针
	* @return
	*		删除成功：返回true
	*		删除失败：返回false	

	*		警告：这个接口不负责释放control的内存,如果调用该接口,外部需要手动释放control内存.
	*/
	virtual bool			remove_actor_control(IActorControl* control) ;

	/**
	* @brief
	*		移除所有IActorControl
	*/
	virtual void clear_actor_controls() ;

	/**
	* @brief
	*		重置control遍历的迭代器
	*/
	virtual void reset_control_iter();

	/**
	* @brief
	*		查找下一个control
	*/
	virtual IActorControl* next_control();
	
//////////////////////////////////////////////////////////////////////////
	/**
	* @brief
	*		获取对象的位置
	* @return
	*		对象的位置
	*/
	virtual H3DVec3 position(void) const ;

	/**
	* @brief
	*		设定对象的位置
	* @param
	*		positon：【IN】对象的位置
	*/
	virtual void set_position(const H3DVec3& pos) ;

	/**
	* @brief
	*		获取对象的欧拉角，欧拉角和max里面保持一致
	* @return
	*		返回一个h3cVec的值，分别表示绕x、y、z三个轴的旋转弧度
	*/
	virtual H3DVec3 rotation() const;

	/**
	* @brief
	*		设定对象的欧拉角，欧拉角和max里面保持一致
	* @return
	*		返回一个h3cVec的值，分别表示绕x、y、z三个轴的旋转弧度
	*/
	virtual void set_rotation(const H3DVec3& obj_rotation);

	/**
	* @brief
	*		设置模型伸缩比例
	* @param
	*		scale：【IN】模型在三维方向上伸缩比例
	*/
	virtual void set_scale(const H3DVec3& scale);

	/**
	* @brief
	*		获取模型的伸缩比例
	* @return
	*		返回模型在三维方向上的伸缩比例
	*/
	virtual H3DVec3 get_scale() const;

	/**
	* @brief
	*		获取对象的本地位置矩阵
	* @return
	*		对象的本地位置矩阵
	*/
	virtual const H3DMat4& get_location_matrix(void);

	/**
	* @brief
	*		设定对象的本地位置矩阵
	* @param
	*		location：【IN】对象的本地位置矩阵
	*/
	virtual void set_location_matrix(const H3DMat4& location);

	/**
	* @brief
	*		获取对象的世界位置矩阵
	* @return
	*		对象的世界位置矩阵
	*/
	virtual const H3DMat4& get_world_location_matrix(void);

	///**
	//* @brief
	//*		设定对象的世界位置矩阵
	//* @param
	//*		location：【IN】对象的世界位置矩阵
	//*/
	//virtual void set_world_location_matrix(const H3DMat4& location);

	/**
	* @brief
	*		设定对象是否可见
	* @param
	*		is_visible：【IN】设定对象是否可见
	*/
	virtual bool get_visible(void) const;

	/**
	* @brief
	*		判断对象是否可见
	* @return
	*		TRUE：对象在屏幕上可见
	*		FALSE：对象在屏幕上不可见
	*/
	virtual void set_visible(bool visible);

	/**
	* @brief
	*		判断对象是否可见
	* @return
	*		TRUE：对象在屏幕上可见
	*		FALSE：对象在屏幕上不可见
	*/
	virtual void set_external_visible(bool visible);

	/**
	* @brief
	*		设定对象是否可见
	* @param
	*		is_visible：【IN】设定对象是否可见
	*/
	virtual bool get_external_visible(void) const;

	/**
	* @brief
	*		设置级联标志信息
	*/
	virtual void set_link_flag(int link_flag);

	/**
	* @brief
	*		获取级联标志信息
	*/
	virtual int get_link_flag();

	/**
	* @brief
	*		设置父Actor
	*/
	virtual void set_parent(Actor* parent);
	/**
	* @brief
	*		获取父Actor
	*/
	virtual Actor* get_parent();

	virtual ActorManager* get_actor_mngr();
	//virtual void set_actor_mngr();

	/**
	* @brief
	*		获取包围盒
	* @return
	*		包围盒坐标
	*/
	virtual bool get_BBox(BBOX& bbox);

	virtual void setColorFactor(float* color, int size);
	
	/**
	* @brief
	*		获得所包含的所有的子entity
	* @param
	*		entitylist:复合entity类型的entity列表
	*/
	virtual bool contain_entity_recursive(EntityList& entitelist);
    
    
    /**
    	@brief   获取包围盒
    */
    CInteract_OBB* GetObb() {return m_OBB;}

    /**
    	@brief   设置包围盒 
    */
    void SetObbPos( const H3DVec3& pos,const H3DVec3& size,const H3DVec3& offset);
 
    /**
    	@brief   判断鼠标是否在Actor上，需要先设置Obb
    	@param   pos 鼠标位置
    	@param   I_UIWndManager * wndMgr
    	@param   H3DI::IRender * pIRender
    	@param   float & distance
    	@Returns bool
    */
    virtual bool IsMouseOnActor(int mouseX,int mouseY,int gameH,int gameW,float& distance, H3DVec4* viewPort = NULL);

	//获取ActorAnimationControl
	virtual ActorAnimationControl* getAnimationControl();
protected:
    
    /**
    	@brief   判断一条射线是否会命中宠物上
    	@param   mousePos 鼠标位置
    	@param   dir 射线方向
    	@param   hitpoint 这是一个返回值，标识命中点位置
    	@Returns bool true = 击中；false = 未击中
    */
    bool IsRayHit(const H3DVec3& mousePos, const H3DVec3& dir, H3DVec3& hitpoint);
		/**
		* @brief
		*		设定对象的位置矩阵
		* @param
		*		location：【IN】对象的位置矩阵
		* @comment
		*		这个接口是为了效率写的和set_location_matrix功能一样
		*/
		virtual void set_location_matrix_inner(const H3DMat4& location) ;

		/**
		* @brief
		*		设定对象的位置矩阵
		* @param
		*		location：【IN】对象的位置矩阵
		* @comment
		*		这个接口是为了效率写的和set_location_matrix功能一样
		*/
		virtual void set_location_matrix_inner(const H3DVec3&pos, const H3DQuat& quat, const H3DVec3& scale) ;

#ifdef _LOCATION_SET_OPTIMIZE_
		/**
		* @brief
		*		更新位置
		*/
		virtual void update_location();

		/**
		* @brief
		*		应用位置矩阵的改变
		*/
		virtual void apply_location_changed();
		/**
		* @brief
		*		应用位置改变
		*/
		virtual void apply_prs_changed();
#endif

protected:
	ActorManager*		m_actor_manager; //actor所属的actor_manger
	EntityList			m_media_end_entity_collection; ///< 可控实体结婚
	EntityList::iterator m_entity_iter;
	ActorList			m_child_actor_collection;	///< 子Actor集合
	ActorList::iterator m_actor_iter;
	ActorControlList     m_control_collection; ///< 控制器集合
	ActorControlList::iterator m_control_iter;
    CInteract_OBB*      m_OBB;  //包围盒，用于判断鼠标是否选中
	Actor*				m_parent;

	std::string			m_name; ///< Actor的名称
	bool				m_visible; ///< Actor是否可见
	bool				m_external_visible;
	int					m_link_flag; ///< 级联标志

	H3DVec3					m_position; ///< 相对位置
	H3DVec3					m_eula; ///< 相对欧拉角
	H3DQuat					m_quat; ///< 相对四元素
	H3DVec3					m_scale;///< 相对缩放，除了在Model中都不可修改。初始值为(1.0f,1.0f,1.0f)

	H3DMat4				m_location_matrix; ///< 位置矩阵
	H3DMat4				m_world_location_matrix; ///< 世界位置矩阵

#ifdef _LOCATION_SET_OPTIMIZE_
	bool				m_prs_changed; //位置，旋转，缩放发生了变化
	bool				m_location_matrix_changed; //位置矩阵发生改变了
#endif
};

#endif //_ACTOR_H_
