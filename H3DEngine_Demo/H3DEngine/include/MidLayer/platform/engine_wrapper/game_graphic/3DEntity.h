/**
* @file 3DEntity.h
* 这个文件的主要功能是：
*			声明了媒体对象的基类C3DEntity
* 
* @author 程陈(Chengchen@h3d.com.cn)
* 
* 更改日志
* ----------------------------------------------------
* ver 1.0.0
*       by 程陈2010.1.25
*/

#ifndef _3D_ENTITY_H_
#define _3D_ENTITY_H_

#include "game_graphic_header.h"
#include "media_end_entity.h"
#include "i_move_entity.h"
#include "i_media_entity.h"
#include "media_end_entity_type.h"

class CInteract_OBB;
class Operation;
class CGraphicsResMng;

enum ENTITY_STATE
{
	ES_NONE = 0, ///<未创建
	ES_CREATE, ///<异步创建中
	ES_HIDE, ///<创建后非立即显示
	ES_UNLOAD, ///<释放资源后，重载资源前
	ES_PRE, ///<预渲染
	ES_VIEW, ///<创建后立即显示
	ES_NUM,
};

#define PRIORITY_COUNT 2

/**
* @brief
*		C3DEntity是媒体对象的基类，
*		具有IMoveEntity和IMediaEntity共同的特性
*/

class C3DEntity
	:public CMediaEndEntity
	,public virtual IMoveEntity
	,public virtual IMediaEntity
{
	friend class ModelOperation;
	friend class EffectOperation;
	friend class EntityOperation;
	friend class CSkeletonModelEntity;
	friend class CEffectEntity;
	friend class CEntityFactory;
public:
	typedef std::list<Operation*> OpList;
	typedef std::list<Operation*>::iterator OpIterator;

	typedef std::map<C3DEntity*, Operation*> EntityOpMap;
	typedef EntityOpMap::iterator EntityOpIterator;

public:
	C3DEntity(CGraphicsResMng* res_mng, const std::string& entity_name, H3DI::IMoveObject* move_object);
	C3DEntity(CGraphicsResMng* res_mng, const std::string& name, const std::string& resource_name, int lod);
	virtual ~C3DEntity(void);

	/**
	* @brief
	*		播放媒体
	*/
	virtual void play(void) ;

	/**
	* @brief
	*		暂停播放媒体
	*/
	virtual void pause(void) ;

	/**
	* @brief
	*		停止播放媒体
	*/
	virtual void stop(void) ;

	/**
	* @brief
	*		重新开始
	*/
	virtual void reset(void) ;

	/**
	* @brief
	*		跳转到指定位置
	*/
	virtual void skip_to(float time) ;

	/**
	* @brief
	*		设定是否循环播放
	* @param
	*		is_loop：【IN】设定媒体是否循环，true：循环，false 不循环
	*/
	virtual void set_loop(bool loop) ;

	/**
	* @brief
	*		获取媒体是否循环
	* @return
	*		TRUE： 循环播放媒体
	*		FALSE：不循环播放媒体
	*/
	virtual bool is_loop(void) ;

	/**
	* @brief
	*		设定媒体长度
	* @param
	*		lenght：【IN】媒体长度
	*/
	virtual void set_media_length(float length) ;

	/**
	* @brief
	*		获取媒体长度
	* @return
	*		媒体长度
	*/
	virtual float media_length(void) const ;

	/**
	* @brief
	*		获取当前的播放时间
	* @return
	*		当前的播放时间
	*/
	virtual float current_time(void) const ;

	/**
	* @brief
	*		设定媒体播放速度
	* @param
	*		speed：【IN】媒体的播放速度
	*/
	virtual void set_play_speed(float speed) ;

	/**
	* @brief
	*		获取媒体的播放速度
	* @return
	*		媒体的播放速度
	*/
	virtual float play_speed(void) const ;

	/**
	* @brief
	*		获取媒体的状态
	* @return
	*		返回媒体的状态
	*/
	virtual MEDIA_STATUS status(void) const ;

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
	*		获取对象的位置矩阵
	* @return
	*		对象的位置矩阵
	*/
	virtual const H3DMat4& get_location_matrix(void) const ;

	/**
	* @brief
	*		设定对象的位置矩阵
	* @param
	*		location：【IN】对象的位置矩阵
	*/
	virtual void set_location_matrix(const H3DMat4& location) ;

	/**
	* @brief
	*		设定对象是否可见
	* @param
	*		is_visible：【IN】设定对象是否可见
	*/
	virtual void set_visible(bool visible);

	/**
	* @brief
	*		判断对象是否可见
	* @return
	*		TRUE：对象在屏幕上可见
	*		FALSE：对象在屏幕上不可见
	*/
	virtual bool is_visible();

	/**
	* @brief
	*		设定对象是否可见
	* @param
	*		is_visible：【IN】设定对象是否可见
	*/
	virtual void set_external_visible(bool visible);

	/**
	* @brief
	*		判断对象是否可见
	* @return
	*		TRUE：对象在屏幕上可见
	*		FALSE：对象在屏幕上不可见
	*/
	virtual bool is_external_visible() const;

	void update_engine_entity_visible();

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
	*		获取在引擎层对应的媒体对象
	* @return
	*		引擎层对应的媒体对象
	*/
	virtual H3DI::IMoveObject* get_IMoveObject() const;

	/**
	* @brief
	*		获取包围盒
	* @return
	*		包围盒坐标
	*/
	virtual BBOX get_BBox();

	/**
	* @brief
	*		转换路径名称。
	*		将这种形式的路径名称：“.\resource\stage\defined_object.xml”
	*		转换为：“./resource/stage/defined_object.xml”
	*
	*/
	void convert_path_string(std::string& str_path)
	{
		std::replace(str_path.begin(), str_path.end(), '\\', '/');
		std::transform(str_path.begin(), str_path.end(),  str_path.begin(), tolower);
	}

	/**
	* @brief
	*		修改资源
	* @param
	*		res_name：【IN】要修改的资源路径
	* @return
	*		true：修改成功
	*		false：修改失败
	*/
	virtual bool change_resource(const std::string& res_name);

	/**
	* @brief
	*		修改资源
	* @param
	*		object：【IN】要修改的资源对象
	* @return
	*		true：修改成功
	*		false：修改失败
	*/
	virtual bool change_media_object(H3DI::IMoveObject* object, bool use_preproperty);

	/**
	* @brief
	*		设置资源的路径信息
	* @param
	*		res_path：【IN】资源路径
	*/
	virtual void set_resource_name(const std::string& res_path);

	/**
	* @brief
	*		获取资源的路径信息
	* @return
	*		资源的路径
	*/
	virtual std::string resource_name() const;

	/**
	* @brief
	*		设置Actor
	*/
	virtual void set_actor(Actor* actor);

	/**
	* @brief
	*		获取对象的位置矩阵
	* @return
	*		对象的位置矩阵
	*/
	virtual const H3DMat4& get_world_location_matrix(void);

	/**
	* @brief
	*		重新加载物体
	*/
	virtual void reload();
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
    bool IsMouseOnActor(int mouseX,int mouseY,int gameH,int gameW,float& distance, H3DVec4* viewPort = NULL);

	/**
	* @brief
	*		从level中删除该entity
	*/
	virtual void remove_from_level(CSceneEntity* level);

	virtual void setColorFactor(float* color, int size);
	virtual void set_diffuse_color_factor(float* color0, int size0, float* color1, int size1, int meshPart = -1, bool bUpdateChildren = true);

	/// 清染色
	/// meshPart表示模型接受变色的网格的ID（对于一般模型可以是MeshID），
	///     如果是角色，表示服装的BodypartID或挂件ID(BODYPART_NUM+挂件ID)，当值为-1时，将变色此模型所有的部分。
	/// bUpdateChildren为true时，将变色此模型的子节点（例如挂件上的特效）
	void ClearDiffuseColorFactor(int meshPart=-1, bool bUpdateChildren=true);

	/**
	* @brief
	*		将entity添加进level
	*/
	virtual void add_to_level(CSceneEntity* level);

	/**
	* @brief
	*		更新引擎层对象
	* @param
	*		intervalTime：【IN】距离上次更新的事件间隔
	*/
	virtual void	update_media_object(float interval) ;

	/**
	* @brief
	*		触发显示
	*/
	void update_appearance();

	/**
	* @brief
	*		设置优先级，持续到下次update_appearance有效
	*/
	void appearance_priority(int priority);

	/**
	* @brief
	*		设置一帧渲染多少个新增对象（BodyPart、挂件、配饰）
	* @param
	*		val：一帧渲染新增对象的数量，0表示不隔帧渲染
	*/
	static void set_render_count(unsigned int val){s_render_count = val;}

	/**
	* @brief
	*		设置是否静态物体
	*/
	void set_is_static(bool val);

	/**
	* @brief
	*		是否从本帧开始绘制，在update之后调用
	*/
	bool is_render();

	/**
	* @brief
	*		设置可视距离
	*/
	void set_visible_dist(float);
	void set_detail_actor_type(int type){m_detial_actor_type = type;};
	int get_detail_actor_type(){return m_detial_actor_type;};

	/**
	* @brief
	*		Entity是否加载完成
	*/
	bool is_loaded();

	/**
	* @brief
	*		释放所有绘制资源
	*/
	virtual void UnloadResources();

	/**
	* @brief
	*		重载所有绘制资源
	*/
	virtual void ReloadResources(bool is_sync = true, int priority = 0);

protected:

    /**
    	@brief   判断一条射线是否会命中宠物上
    	@param   mousePos 鼠标位置
    	@param   dir 射线方向
    	@param   hitpoint 这是一个返回值，标识命中点位置
    	@Returns bool true = 击中；false = 未击中
    */
    bool IsRayHit(const H3DVec3& mousePos, const H3DVec3& dir, H3DVec3& hitpoint);

protected:
	C3DEntity(CGraphicsResMng* res_mng, std::string name); //just for CSoundEntity

	//把一个四元素转成欧拉角
	H3DVec3 quat_to_eula(const H3DQuat &quat );

	/**
	* @brief
	*		更新对象
	* @param
	*		intervalTime：【IN】距离上次更新的事件间隔
	*/
	virtual void do_update(float interval);

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

	/**
	* @brief
	*		设定对象的位置矩阵
	* @param
	*		location：【IN】对象的位置矩阵
	*/
	virtual void set_world_location_matrix(const H3DMat4& location) ;

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
	void create_engine_object(bool is_sync, int priority);
	void set_entity_state(ENTITY_STATE state);
	void create_sync();
	bool get_is_create_sync(){return m_is_create_sync;}

	void update_execute_op();
	void update_execute_op(OpList& execute_list, OpList& finish_list);
	void update_finish_op(OpList& finish_list);
	void update_first_op();
	void finish_all_op();
	virtual void clear_all_op();
	bool op_list_empty();
	bool next_wait();
	void stop_entity_op(C3DEntity* entity);
	void remove_entity_op(C3DEntity* entity);

	virtual void create_engine_object_impl(bool is_sync){}
	virtual void set_engine_object_impl(H3DI::IMoveObject* object);
	virtual void after_create_impl();

	void set_entity_flags(unsigned int flags, bool is);

protected:
	H3DI::IMoveObject*		m_move_object; ///< 引擎库对应的对象
	ENTITY_STATE		m_entity_state;
    CInteract_OBB*      m_OBB;      //用于检测鼠标选中
	float					m_cur_time; ///< 当前播放时间
	float					m_media_length; ///< 媒体的长度
	float					m_play_speed;  ///< 媒体的播放速度
	bool					m_is_loop;	///<　设定是否循环播放
	bool					m_is_visible; ///< 设定对象在屏幕上是否显示
	bool					m_is_external_visible;

	MEDIA_STATUS			m_status; ///< 媒体所处的状态

	H3DVec3					m_position; ///< 相对位置
	H3DVec3					m_eula; ///< 相对欧拉角
	H3DQuat					m_quat; ///< 相对四元素
	H3DVec3					m_scale;///< 相对缩放，除了在Model中都不可修改。初始值为(1.0f,1.0f,1.0f)
	H3DMat4					m_location_matrix; ///< 位置矩阵
	H3DMat4					m_world_location_matrix; ///< 世界坐标矩阵

	std::string				m_resource_name; ///< 资源路径

	int					m_level_layer; ///<Level的层

#ifdef _LOCATION_SET_OPTIMIZE_
	bool				m_prs_changed; //位置，旋转，缩放发生了变化
	bool				m_location_matrix_changed; //位置矩阵发生改变了
#endif

protected:
	unsigned int	m_priority;
	unsigned int	m_create_priority;
	OpList		m_execute_op_list[PRIORITY_COUNT];
	OpList		m_finish_op_list[PRIORITY_COUNT];
	OpList		m_cache_op_list;
	OpList		m_first_op_list;
	static unsigned int s_render_count;
	bool			m_is_create_sync;
	bool			m_is_view_sync;
	bool			m_is_first_view;
	EntityOpMap		m_entity_op_map;

	//Cache的属性
	bool		m_set_color_factor;
	float		m_color_factor[4];
	int		m_color_factor_size;
	
	bool		m_set_diffuse_color_factor;
	float		m_diffuse_color_factor_0[4];
	int		m_diffuse_color_factor_size_0;
	float		m_diffuse_color_factor_1[4];
	int		m_diffuse_color_factor_size_1;
	int		m_mesh_part;
	bool		m_update_children;

	bool		m_is_static;

	int		m_detial_actor_type; ///<物体的细节类型
	float		m_visible_dist;	///<物体的消失距离

//缺少属性队列部分	
private:
	FORBIDDEN_EVIL_CONSTRUCTOR(C3DEntity);
};

#endif //_D_ENTITY_H_
