/**
* @file operation.h
* 这个文件的主要功能是：
*			声明了异步操作类Operation
* 
* @author 郑成俊(zhengchengjun@h3d.com.cn)
* 
* 更改日志
* ----------------------------------------------------
* ver 1.0.0
*       by 郑成俊2012.6.21
*/

#ifndef _OPERATION_H_
#define _OPERATION_H_

#include "game_graphic_header.h"

class C3DEntity;

enum OP_TYPE
{
	OPT_NULL = -1,
	OPT_CREATE = 0,	///<创建
	OPT_ACTION, ///<动作
	OPT_NEXTACTION, ///<下一个动作
	OPT_BODYPART, ///<Bodypart
	OPT_ADORNMENT, ///<挂件
	OPT_LOD, ///<材质LOD
	OPT_ATTACH, ///<挂饰
	OPT_SET_EFFECT, ///<设置特效
	OPT_SHOW, ///<显示
	OPT_NUM,
};

enum OP_STATE
{
	OPS_NULL = -1,
	OPS_WAIT = 0, ///<等待状态
	OPS_EXECUTE, ///<任务执行状态
	OPS_FINISH, ///<完成状态
	OPS_STOP, ///<中止状态
	OPS_NUM,
};

/**
* @brief
*		Operation是异步操作的基类
*/
class Operation
{
public:
	Operation(bool is_view);
	virtual ~Operation();

	void release();

	/**
	* @brief
	*		获取是否立即显示
	*/
	bool is_view(){return m_is_view;}

	/**
	* @brief
	*		获取OP类型
	*/
	OP_TYPE get_type(){return m_type;}

	/**
	* @brief
	*		获取状态
	*/
	OP_STATE get_state(){return m_state;}


	/**
	* @brief
	*		更新状态
	*/
	OP_STATE check_state();

	/**
	* @brief
	*		阻塞完成
	*/
	virtual void finish_sync(){m_state = OPS_FINISH;}

	/**
	* @brief
	*		中止
	*/
	virtual void stop(){m_state = OPS_STOP;}

	/**
	* @brief
	*		执行OP
	*/
	virtual void do_op(C3DEntity* entity){}

protected:
	virtual void to_execute(){m_state = OPS_EXECUTE;}
	virtual void check_finish(){}
	virtual void handle_error(){stop();}

protected:
	bool		m_is_view; ///< 是否立即显示
	OP_STATE		m_state; ///<当前状态
	OP_TYPE		m_type;	 ///<OP类型
};

/**
* @brief
*		TaskOperation是有异步任务的操作
*/

class TaskOperation : public Operation
{
public:
	TaskOperation(bool is_view, unsigned int priority, H3DI::IRender* irender);

	virtual void finish_sync();
	virtual void stop();

protected:
	virtual void check_finish();

protected:
	H3DI::IRender*		m_render; ///<引擎渲染对象
	unsigned int		m_task_id; ///<任务ID
	unsigned int		m_priority; ///<任务优先级
};

/**
* @brief
*		ModelOperation是异步创建模型的操作
*/
class ModelOperation : public TaskOperation
{
public:
	ModelOperation(unsigned int priority, H3DI::IRender* irender, const std::string& resource_name, int lod);

	virtual void do_op(C3DEntity* entity);
};

/**
* @brief
*		EffectOperation是异步创建特效的操作
*/
class EffectOperation : public TaskOperation
{
public:
	EffectOperation(unsigned int priority, H3DI::IRender* irender, const std::string& resource_name, int lod);

	virtual void do_op(C3DEntity* entity);
};

struct ActionOpParam
{
	int		channel_id; ///<动作通道ID
	std::string		action_name; ///<动作名
	bool		loop; ///<是否循环
	H3DI::EActorType		actor_type; ///<actor类型
	bool		is_male; ///<是否为男性
	C3DEntity*		entity;

	ActionOpParam()
		: entity(0)
	{
	}
};
/**
* @brief
*		ActionOperation是角色(宠物)异步设置动作的操作
*/
class ActionOperation : public TaskOperation
{
public:
	ActionOperation(bool is_view, unsigned int priority, H3DI::IRender* irender, const ActionOpParam& param, bool is_wait);

	virtual void do_op(C3DEntity* entity);

protected:
	virtual void to_execute();
	virtual void handle_error();

protected:
	ActionOpParam		m_param;
};

/**
* @brief
*		NextActionOperation是角色(宠物)异步设置下一个动作的操作
*/
class NextActionOperation : public ActionOperation
{
public:
	NextActionOperation(bool is_view, unsigned int priority, H3DI::IRender* irender, const ActionOpParam& param, bool is_wait);

	virtual void do_op(C3DEntity* entity);

protected:
	virtual void handle_error();
};

struct BodypartOpParam
{
	H3DI::EActorType		actor_type; ///<actor类型
	int		body_part;
	std::string		resource_name; ///<资源名
	int		lod;
	C3DEntity*		entity;

	BodypartOpParam()
		: entity(0)
	{
	}
};

/**
* @brief
*		BodypartOperation是角色(宠物)异步设置动作的操作
*/
class BodypartOperation : public TaskOperation
{
public:
	BodypartOperation(bool is_view, unsigned int priority, H3DI::IRender* irender, const BodypartOpParam& param, bool is_wait);

	virtual void do_op(C3DEntity* entity);

protected:
	virtual void to_execute();
	virtual void handle_error();

protected:
	BodypartOpParam		m_param;
};

struct AdornmentOpParam
{
	std::string		resource_name; ///<资源名
	int		lod;
	int		adornment_pos; ///<挂件位置
	std::string		action_name; ///<动作名
	H3DI::EActorType		actor_type; ///<actor类型
	bool		is_male; ///<是否为男性
	unsigned int		action_task_id;
	C3DEntity*		entity;

	AdornmentOpParam()
		: action_task_id(0)
		, entity(0)
	{
	}
};
/**
* @brief
*		AdornmentOperation是角色(宠物)异步设置挂件的操作
*/
class AdornmentOperation : public TaskOperation
{
public:
	AdornmentOperation(bool is_view, unsigned int priority, H3DI::IRender* irender, const AdornmentOpParam& param, bool is_wait);

	virtual void finish_sync();
	virtual void stop();
	virtual void do_op(C3DEntity* entity);

protected:
	virtual void to_execute();
	virtual void check_finish();
	virtual void handle_error();

protected:
	AdornmentOpParam		m_param;
};

/**
* @brief
*		LodOperation是设置材质LOD的操作
*/
class LodOperation : public TaskOperation
{
public:
	LodOperation(bool is_view, H3DI::IRender* irender, int lod, C3DEntity* entity, bool is_wait);

	virtual void do_op(C3DEntity* entity);

protected:
	virtual void to_execute();

protected:
	int		m_lod; ///<LOD级别
	C3DEntity*		m_entity;
};

/**
* @brief
*		EntityOperation是等待其他Entity加载完成的操作
*/
class EntityOperation : public Operation
{
public:
	EntityOperation(bool is_view, C3DEntity* main_entity, C3DEntity* entity);

	virtual void finish_sync();
	virtual void stop();

	/**
	* @brief
	*		获取Entity
	*/
	C3DEntity* get_entity(){return m_entity;}

protected:
	virtual void check_finish();

protected:
	C3DEntity*		m_main_entity;
	C3DEntity*		m_entity; ///<其他Entity
};

struct AttachOpParam
{
	int		bodypart;
	int		id;
};
/**
* @brief
*		AttachOperation是设置挂饰的操作
*/
class AttachOperation : public EntityOperation
{
public:
	AttachOperation(bool is_view, C3DEntity* main_entity, C3DEntity* entity, const AttachOpParam& param);

	virtual void do_op(C3DEntity* entity);

protected:
	AttachOpParam		m_param;
};

/**
* @brief
*		SetEffectOperation是设置特效的操作
*/
class SetEffectOperation : public EntityOperation
{
public:
	SetEffectOperation(bool is_view, C3DEntity* main_entity, C3DEntity* entity);

	virtual void do_op(C3DEntity* entity);
};

/**
* @brief
*		ShowOperation是显示标志位
*/
class ShowOperation : public Operation
{
public:
	ShowOperation(bool first_view = false);

	bool is_first_view(){return m_is_first_view;}

protected:
	bool		m_is_first_view;
};

#endif //_OPERATION_H_