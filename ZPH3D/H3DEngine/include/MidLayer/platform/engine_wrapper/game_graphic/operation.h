/**
* @file operation.h
* ����ļ�����Ҫ�����ǣ�
*			�������첽������Operation
* 
* @author ֣�ɿ�(zhengchengjun@h3d.com.cn)
* 
* ������־
* ----------------------------------------------------
* ver 1.0.0
*       by ֣�ɿ�2012.6.21
*/

#ifndef _OPERATION_H_
#define _OPERATION_H_

#include "game_graphic_header.h"

class C3DEntity;

enum OP_TYPE
{
	OPT_NULL = -1,
	OPT_CREATE = 0,	///<����
	OPT_ACTION, ///<����
	OPT_NEXTACTION, ///<��һ������
	OPT_BODYPART, ///<Bodypart
	OPT_ADORNMENT, ///<�Ҽ�
	OPT_LOD, ///<����LOD
	OPT_ATTACH, ///<����
	OPT_SET_EFFECT, ///<������Ч
	OPT_SHOW, ///<��ʾ
	OPT_NUM,
};

enum OP_STATE
{
	OPS_NULL = -1,
	OPS_WAIT = 0, ///<�ȴ�״̬
	OPS_EXECUTE, ///<����ִ��״̬
	OPS_FINISH, ///<���״̬
	OPS_STOP, ///<��ֹ״̬
	OPS_NUM,
};

/**
* @brief
*		Operation���첽�����Ļ���
*/
class Operation
{
public:
	Operation(bool is_view);
	virtual ~Operation();

	void release();

	/**
	* @brief
	*		��ȡ�Ƿ�������ʾ
	*/
	bool is_view(){return m_is_view;}

	/**
	* @brief
	*		��ȡOP����
	*/
	OP_TYPE get_type(){return m_type;}

	/**
	* @brief
	*		��ȡ״̬
	*/
	OP_STATE get_state(){return m_state;}


	/**
	* @brief
	*		����״̬
	*/
	OP_STATE check_state();

	/**
	* @brief
	*		�������
	*/
	virtual void finish_sync(){m_state = OPS_FINISH;}

	/**
	* @brief
	*		��ֹ
	*/
	virtual void stop(){m_state = OPS_STOP;}

	/**
	* @brief
	*		ִ��OP
	*/
	virtual void do_op(C3DEntity* entity){}

protected:
	virtual void to_execute(){m_state = OPS_EXECUTE;}
	virtual void check_finish(){}
	virtual void handle_error(){stop();}

protected:
	bool		m_is_view; ///< �Ƿ�������ʾ
	OP_STATE		m_state; ///<��ǰ״̬
	OP_TYPE		m_type;	 ///<OP����
};

/**
* @brief
*		TaskOperation�����첽����Ĳ���
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
	H3DI::IRender*		m_render; ///<������Ⱦ����
	unsigned int		m_task_id; ///<����ID
	unsigned int		m_priority; ///<�������ȼ�
};

/**
* @brief
*		ModelOperation���첽����ģ�͵Ĳ���
*/
class ModelOperation : public TaskOperation
{
public:
	ModelOperation(unsigned int priority, H3DI::IRender* irender, const std::string& resource_name, int lod);

	virtual void do_op(C3DEntity* entity);
};

/**
* @brief
*		EffectOperation���첽������Ч�Ĳ���
*/
class EffectOperation : public TaskOperation
{
public:
	EffectOperation(unsigned int priority, H3DI::IRender* irender, const std::string& resource_name, int lod);

	virtual void do_op(C3DEntity* entity);
};

struct ActionOpParam
{
	int		channel_id; ///<����ͨ��ID
	std::string		action_name; ///<������
	bool		loop; ///<�Ƿ�ѭ��
	H3DI::EActorType		actor_type; ///<actor����
	bool		is_male; ///<�Ƿ�Ϊ����
	C3DEntity*		entity;

	ActionOpParam()
		: entity(0)
	{
	}
};
/**
* @brief
*		ActionOperation�ǽ�ɫ(����)�첽���ö����Ĳ���
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
*		NextActionOperation�ǽ�ɫ(����)�첽������һ�������Ĳ���
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
	H3DI::EActorType		actor_type; ///<actor����
	int		body_part;
	std::string		resource_name; ///<��Դ��
	int		lod;
	C3DEntity*		entity;

	BodypartOpParam()
		: entity(0)
	{
	}
};

/**
* @brief
*		BodypartOperation�ǽ�ɫ(����)�첽���ö����Ĳ���
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
	std::string		resource_name; ///<��Դ��
	int		lod;
	int		adornment_pos; ///<�Ҽ�λ��
	std::string		action_name; ///<������
	H3DI::EActorType		actor_type; ///<actor����
	bool		is_male; ///<�Ƿ�Ϊ����
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
*		AdornmentOperation�ǽ�ɫ(����)�첽���ùҼ��Ĳ���
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
*		LodOperation�����ò���LOD�Ĳ���
*/
class LodOperation : public TaskOperation
{
public:
	LodOperation(bool is_view, H3DI::IRender* irender, int lod, C3DEntity* entity, bool is_wait);

	virtual void do_op(C3DEntity* entity);

protected:
	virtual void to_execute();

protected:
	int		m_lod; ///<LOD����
	C3DEntity*		m_entity;
};

/**
* @brief
*		EntityOperation�ǵȴ�����Entity������ɵĲ���
*/
class EntityOperation : public Operation
{
public:
	EntityOperation(bool is_view, C3DEntity* main_entity, C3DEntity* entity);

	virtual void finish_sync();
	virtual void stop();

	/**
	* @brief
	*		��ȡEntity
	*/
	C3DEntity* get_entity(){return m_entity;}

protected:
	virtual void check_finish();

protected:
	C3DEntity*		m_main_entity;
	C3DEntity*		m_entity; ///<����Entity
};

struct AttachOpParam
{
	int		bodypart;
	int		id;
};
/**
* @brief
*		AttachOperation�����ù��εĲ���
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
*		SetEffectOperation��������Ч�Ĳ���
*/
class SetEffectOperation : public EntityOperation
{
public:
	SetEffectOperation(bool is_view, C3DEntity* main_entity, C3DEntity* entity);

	virtual void do_op(C3DEntity* entity);
};

/**
* @brief
*		ShowOperation����ʾ��־λ
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