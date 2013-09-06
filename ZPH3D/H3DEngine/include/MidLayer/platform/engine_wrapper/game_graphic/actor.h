/**
* @file Actor.h
* ����ļ�����Ҫ�����ǣ�
* 
* @author �̳�(Chengchen@h3d.com.cn)
* 
* ������־
* ----------------------------------------------------
* ver 1.0.0
*       by �̳�2010.1.27
*/

#ifndef _ACTOR_H_
#define _ACTOR_H_

#include "game_graphic_header.h"

enum AttributeLinkType //������־
{
	AttributeLinkNone = 0, //���͸�Actor���κ����Թ���
	AttributeLinkLocation = 1 << 0, //������Actor��λ��
	AttributeLinkVisible =  1 << 1, //������Actor��Visible����
};

/**
* @brief
*		actor�Ƕ�̬Ч������Ļ��࣬��������˶����Entity�������
*		һ����̬Ч�����塣���Ҹ���ͨ����������Ĺ�ϵ��ʵ���˼���Ч��
* @func
*		1����֯��Entity��Actor����һ�����壬��Entity���������һ����
*		2��ʵ���˼�����Ч����ͨ����־λʵ�ֺ͸�Actor���ĸ����Թ���
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
	*		��ȡ��������
	* @param
	*		name����IN�����������
	*/
	void set_name(const char* name);
	/**
	* @brief
	*		��ȡ��������
	* @return
	*		���������
	*/
	const char* get_name() const;

	/**
	* @brief
	*		����,����ʱ��Ϊ����
	* @brief
	*		����ʱ��
	*/
	virtual void update(float t);

	/**
	* @brief
	*		����Actor
	*/
	virtual void render(CRenderSystem& render_system);
	//����Ӳ��ѡ����Ⱦ
	virtual unsigned int pushHardwareSelect(CRenderSystem& render_system, bool sel_line);

//////////////////////////////////////////////////////////////////////////
	/**
	* @brief
	*		��ȡ����ָ��entity_name�Ŀɿ�ʵ��
	* @param
	*		entity_name����IN���ӿɿ�ʵ����������
	* @return
	*		���ز��ҵ�����Entity�����ָ�룬���û�У�����NULL
	*/
	virtual CMediaEndEntity* find_entity(const std::string& entity_name) const ;

	/**
	* @brief
	*		����ӿɿ�ʵ��
	* @param
	*		entity����IN��Ҫ��ӵ��ӿɿ�ʵ�����ָ��
	* @return
	*		��ӳɹ�������true
	*		���ʧ�ܣ�����false
	*/
	virtual bool			add_entity(CMediaEndEntity* entity) ;

	/**
	* @brief
	*		�Ƴ��ӿɿ�ʵ�����
	* @param
	*		entity����IN��Ҫ�Ƴ����ӿɿ�ʵ������ָ��
	* @return
	*		ɾ���ɹ�������true
	*		ɾ��ʧ�ܣ�����false	
	*/
	virtual bool			remove_entity(CMediaEndEntity* entity) ;

	/**
	* @brief
	*		�Ƴ������ӿɿ�ʵ��
	*/
	virtual void clear_entities() ;

	/**
	* @brief
	*		����control�����ĵ�����
	*/
	virtual void reset_entity_iter();

	/**
	* @brief
	*		������һ��control
	*/
	virtual CMediaEndEntity* next_entity();

	/**
	* @brief
	*		����entity
	* @param
	*		entity_id��entity��id
	*/
	virtual CMediaEndEntity* find_entity_recursive(unsigned int entity_id);

	/**
	* @brief
	*		�Ƿ����ĳ��ָ�����͵�entity
	* @param
	*		obj_type��Ҫ����entity����
	*		entitylist:����entity���͵�entity�б�
	*/
	virtual bool contain_entity_type_recursive(int obj_type, EntityList& entitelist);

//////////////////////////////////////////////////////////////////////////
	/**
	* @brief
	*		���Actor��
	* @param
	*		Actor����IN��Ҫ��ӵ�Actor
	* @return
	*		��ӳɹ�������true
	*		���ʧ�ܣ�����false
	*/
	virtual bool add_actor(Actor* Actor);

	/**
	* @brief
	*		ɾ��Actor
	* @param
	*		Actor����IN��Ҫɾ����Actor
	*/
	virtual bool remove_actor(Actor* Actor);

	/**
	* @brief
	*	������е�Actor
	*/
	virtual void clear_actors();


	/**
	* @brief
	*		��ȡActor
	* @param
	*		Actor_name����IN��Ҫ��ȡ��Actor������
	*/
	virtual Actor*	find_actor(const char* Actor_name) const;

	/**
	* @brief
	*		����Actor�����ĵ�����
	*/
	virtual void reset_actor_iter();

	/**
	* @brief
	*		������һ��Actor
	*/
	virtual Actor* next_actor();


//////////////////////////////////////////////////////////////////////////
	
	/**
	* @brief
	*		��ȡ����ָ��control_name��IActorControl
	* @param
	*		entity_name����IN��IActorControl������
	* @return
	*		���ز��ҵ���IActorControl�����ָ�룬���û�У�����NULL
	*/
	virtual IActorControl* find_actor_control(const std::string& control_name) const ;

	/**
	* @brief
	*		���IActorControl
	* @param
	*		entity����IN��Ҫ��ӵ�IActorControl����ָ��
	* @return
	*		��ӳɹ�������true
	*		���ʧ�ܣ�����false
	*/
	virtual bool			add_actor_control(IActorControl* control) ;

	/**
	* @brief
	*		�Ƴ�IActorControl����
	* @param
	*		entity����IN��Ҫ�Ƴ���IActorControl�����ָ��
	* @return
	*		ɾ���ɹ�������true
	*		ɾ��ʧ�ܣ�����false	

	*		���棺����ӿڲ������ͷ�control���ڴ�,������øýӿ�,�ⲿ��Ҫ�ֶ��ͷ�control�ڴ�.
	*/
	virtual bool			remove_actor_control(IActorControl* control) ;

	/**
	* @brief
	*		�Ƴ�����IActorControl
	*/
	virtual void clear_actor_controls() ;

	/**
	* @brief
	*		����control�����ĵ�����
	*/
	virtual void reset_control_iter();

	/**
	* @brief
	*		������һ��control
	*/
	virtual IActorControl* next_control();
	
//////////////////////////////////////////////////////////////////////////
	/**
	* @brief
	*		��ȡ�����λ��
	* @return
	*		�����λ��
	*/
	virtual H3DVec3 position(void) const ;

	/**
	* @brief
	*		�趨�����λ��
	* @param
	*		positon����IN�������λ��
	*/
	virtual void set_position(const H3DVec3& pos) ;

	/**
	* @brief
	*		��ȡ�����ŷ���ǣ�ŷ���Ǻ�max���汣��һ��
	* @return
	*		����һ��h3cVec��ֵ���ֱ��ʾ��x��y��z���������ת����
	*/
	virtual H3DVec3 rotation() const;

	/**
	* @brief
	*		�趨�����ŷ���ǣ�ŷ���Ǻ�max���汣��һ��
	* @return
	*		����һ��h3cVec��ֵ���ֱ��ʾ��x��y��z���������ת����
	*/
	virtual void set_rotation(const H3DVec3& obj_rotation);

	/**
	* @brief
	*		����ģ����������
	* @param
	*		scale����IN��ģ������ά��������������
	*/
	virtual void set_scale(const H3DVec3& scale);

	/**
	* @brief
	*		��ȡģ�͵���������
	* @return
	*		����ģ������ά�����ϵ���������
	*/
	virtual H3DVec3 get_scale() const;

	/**
	* @brief
	*		��ȡ����ı���λ�þ���
	* @return
	*		����ı���λ�þ���
	*/
	virtual const H3DMat4& get_location_matrix(void);

	/**
	* @brief
	*		�趨����ı���λ�þ���
	* @param
	*		location����IN������ı���λ�þ���
	*/
	virtual void set_location_matrix(const H3DMat4& location);

	/**
	* @brief
	*		��ȡ���������λ�þ���
	* @return
	*		���������λ�þ���
	*/
	virtual const H3DMat4& get_world_location_matrix(void);

	///**
	//* @brief
	//*		�趨���������λ�þ���
	//* @param
	//*		location����IN�����������λ�þ���
	//*/
	//virtual void set_world_location_matrix(const H3DMat4& location);

	/**
	* @brief
	*		�趨�����Ƿ�ɼ�
	* @param
	*		is_visible����IN���趨�����Ƿ�ɼ�
	*/
	virtual bool get_visible(void) const;

	/**
	* @brief
	*		�ж϶����Ƿ�ɼ�
	* @return
	*		TRUE����������Ļ�Ͽɼ�
	*		FALSE����������Ļ�ϲ��ɼ�
	*/
	virtual void set_visible(bool visible);

	/**
	* @brief
	*		�ж϶����Ƿ�ɼ�
	* @return
	*		TRUE����������Ļ�Ͽɼ�
	*		FALSE����������Ļ�ϲ��ɼ�
	*/
	virtual void set_external_visible(bool visible);

	/**
	* @brief
	*		�趨�����Ƿ�ɼ�
	* @param
	*		is_visible����IN���趨�����Ƿ�ɼ�
	*/
	virtual bool get_external_visible(void) const;

	/**
	* @brief
	*		���ü�����־��Ϣ
	*/
	virtual void set_link_flag(int link_flag);

	/**
	* @brief
	*		��ȡ������־��Ϣ
	*/
	virtual int get_link_flag();

	/**
	* @brief
	*		���ø�Actor
	*/
	virtual void set_parent(Actor* parent);
	/**
	* @brief
	*		��ȡ��Actor
	*/
	virtual Actor* get_parent();

	virtual ActorManager* get_actor_mngr();
	//virtual void set_actor_mngr();

	/**
	* @brief
	*		��ȡ��Χ��
	* @return
	*		��Χ������
	*/
	virtual bool get_BBox(BBOX& bbox);

	virtual void setColorFactor(float* color, int size);
	
	/**
	* @brief
	*		��������������е���entity
	* @param
	*		entitylist:����entity���͵�entity�б�
	*/
	virtual bool contain_entity_recursive(EntityList& entitelist);
    
    
    /**
    	@brief   ��ȡ��Χ��
    */
    CInteract_OBB* GetObb() {return m_OBB;}

    /**
    	@brief   ���ð�Χ�� 
    */
    void SetObbPos( const H3DVec3& pos,const H3DVec3& size,const H3DVec3& offset);
 
    /**
    	@brief   �ж�����Ƿ���Actor�ϣ���Ҫ������Obb
    	@param   pos ���λ��
    	@param   I_UIWndManager * wndMgr
    	@param   H3DI::IRender * pIRender
    	@param   float & distance
    	@Returns bool
    */
    virtual bool IsMouseOnActor(int mouseX,int mouseY,int gameH,int gameW,float& distance, H3DVec4* viewPort = NULL);

	//��ȡActorAnimationControl
	virtual ActorAnimationControl* getAnimationControl();
protected:
    
    /**
    	@brief   �ж�һ�������Ƿ�����г�����
    	@param   mousePos ���λ��
    	@param   dir ���߷���
    	@param   hitpoint ����һ������ֵ����ʶ���е�λ��
    	@Returns bool true = ���У�false = δ����
    */
    bool IsRayHit(const H3DVec3& mousePos, const H3DVec3& dir, H3DVec3& hitpoint);
		/**
		* @brief
		*		�趨�����λ�þ���
		* @param
		*		location����IN�������λ�þ���
		* @comment
		*		����ӿ���Ϊ��Ч��д�ĺ�set_location_matrix����һ��
		*/
		virtual void set_location_matrix_inner(const H3DMat4& location) ;

		/**
		* @brief
		*		�趨�����λ�þ���
		* @param
		*		location����IN�������λ�þ���
		* @comment
		*		����ӿ���Ϊ��Ч��д�ĺ�set_location_matrix����һ��
		*/
		virtual void set_location_matrix_inner(const H3DVec3&pos, const H3DQuat& quat, const H3DVec3& scale) ;

#ifdef _LOCATION_SET_OPTIMIZE_
		/**
		* @brief
		*		����λ��
		*/
		virtual void update_location();

		/**
		* @brief
		*		Ӧ��λ�þ���ĸı�
		*/
		virtual void apply_location_changed();
		/**
		* @brief
		*		Ӧ��λ�øı�
		*/
		virtual void apply_prs_changed();
#endif

protected:
	ActorManager*		m_actor_manager; //actor������actor_manger
	EntityList			m_media_end_entity_collection; ///< �ɿ�ʵ����
	EntityList::iterator m_entity_iter;
	ActorList			m_child_actor_collection;	///< ��Actor����
	ActorList::iterator m_actor_iter;
	ActorControlList     m_control_collection; ///< ����������
	ActorControlList::iterator m_control_iter;
    CInteract_OBB*      m_OBB;  //��Χ�У������ж�����Ƿ�ѡ��
	Actor*				m_parent;

	std::string			m_name; ///< Actor������
	bool				m_visible; ///< Actor�Ƿ�ɼ�
	bool				m_external_visible;
	int					m_link_flag; ///< ������־

	H3DVec3					m_position; ///< ���λ��
	H3DVec3					m_eula; ///< ���ŷ����
	H3DQuat					m_quat; ///< �����Ԫ��
	H3DVec3					m_scale;///< ������ţ�������Model�ж������޸ġ���ʼֵΪ(1.0f,1.0f,1.0f)

	H3DMat4				m_location_matrix; ///< λ�þ���
	H3DMat4				m_world_location_matrix; ///< ����λ�þ���

#ifdef _LOCATION_SET_OPTIMIZE_
	bool				m_prs_changed; //λ�ã���ת�����ŷ����˱仯
	bool				m_location_matrix_changed; //λ�þ������ı���
#endif
};

#endif //_ACTOR_H_
