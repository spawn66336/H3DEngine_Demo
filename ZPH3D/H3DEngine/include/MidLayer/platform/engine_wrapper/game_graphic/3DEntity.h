/**
* @file 3DEntity.h
* ����ļ�����Ҫ�����ǣ�
*			������ý�����Ļ���C3DEntity
* 
* @author �̳�(Chengchen@h3d.com.cn)
* 
* ������־
* ----------------------------------------------------
* ver 1.0.0
*       by �̳�2010.1.25
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
	ES_NONE = 0, ///<δ����
	ES_CREATE, ///<�첽������
	ES_HIDE, ///<�������������ʾ
	ES_UNLOAD, ///<�ͷ���Դ��������Դǰ
	ES_PRE, ///<Ԥ��Ⱦ
	ES_VIEW, ///<������������ʾ
	ES_NUM,
};

#define PRIORITY_COUNT 2

/**
* @brief
*		C3DEntity��ý�����Ļ��࣬
*		����IMoveEntity��IMediaEntity��ͬ������
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
	*		����ý��
	*/
	virtual void play(void) ;

	/**
	* @brief
	*		��ͣ����ý��
	*/
	virtual void pause(void) ;

	/**
	* @brief
	*		ֹͣ����ý��
	*/
	virtual void stop(void) ;

	/**
	* @brief
	*		���¿�ʼ
	*/
	virtual void reset(void) ;

	/**
	* @brief
	*		��ת��ָ��λ��
	*/
	virtual void skip_to(float time) ;

	/**
	* @brief
	*		�趨�Ƿ�ѭ������
	* @param
	*		is_loop����IN���趨ý���Ƿ�ѭ����true��ѭ����false ��ѭ��
	*/
	virtual void set_loop(bool loop) ;

	/**
	* @brief
	*		��ȡý���Ƿ�ѭ��
	* @return
	*		TRUE�� ѭ������ý��
	*		FALSE����ѭ������ý��
	*/
	virtual bool is_loop(void) ;

	/**
	* @brief
	*		�趨ý�峤��
	* @param
	*		lenght����IN��ý�峤��
	*/
	virtual void set_media_length(float length) ;

	/**
	* @brief
	*		��ȡý�峤��
	* @return
	*		ý�峤��
	*/
	virtual float media_length(void) const ;

	/**
	* @brief
	*		��ȡ��ǰ�Ĳ���ʱ��
	* @return
	*		��ǰ�Ĳ���ʱ��
	*/
	virtual float current_time(void) const ;

	/**
	* @brief
	*		�趨ý�岥���ٶ�
	* @param
	*		speed����IN��ý��Ĳ����ٶ�
	*/
	virtual void set_play_speed(float speed) ;

	/**
	* @brief
	*		��ȡý��Ĳ����ٶ�
	* @return
	*		ý��Ĳ����ٶ�
	*/
	virtual float play_speed(void) const ;

	/**
	* @brief
	*		��ȡý���״̬
	* @return
	*		����ý���״̬
	*/
	virtual MEDIA_STATUS status(void) const ;

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
	*		��ȡ�����λ�þ���
	* @return
	*		�����λ�þ���
	*/
	virtual const H3DMat4& get_location_matrix(void) const ;

	/**
	* @brief
	*		�趨�����λ�þ���
	* @param
	*		location����IN�������λ�þ���
	*/
	virtual void set_location_matrix(const H3DMat4& location) ;

	/**
	* @brief
	*		�趨�����Ƿ�ɼ�
	* @param
	*		is_visible����IN���趨�����Ƿ�ɼ�
	*/
	virtual void set_visible(bool visible);

	/**
	* @brief
	*		�ж϶����Ƿ�ɼ�
	* @return
	*		TRUE����������Ļ�Ͽɼ�
	*		FALSE����������Ļ�ϲ��ɼ�
	*/
	virtual bool is_visible();

	/**
	* @brief
	*		�趨�����Ƿ�ɼ�
	* @param
	*		is_visible����IN���趨�����Ƿ�ɼ�
	*/
	virtual void set_external_visible(bool visible);

	/**
	* @brief
	*		�ж϶����Ƿ�ɼ�
	* @return
	*		TRUE����������Ļ�Ͽɼ�
	*		FALSE����������Ļ�ϲ��ɼ�
	*/
	virtual bool is_external_visible() const;

	void update_engine_entity_visible();

	/**
	* @brief
	*		�ж϶����Ƿ�ΪҪ�������
	* @param
	*		type����IN���������
	* @return
	*		true��Ϊָ��������
	*		false����Ϊָ��������
	*/
	virtual bool is_kind_of(int obj_type);

	/**
	* @brief
	*		��ȡ�������
	* @return
	*		�������
	*/
	virtual int		type(void) const;

	/**
	* @brief
	*		��ȡ��������Ӧ��ý�����
	* @return
	*		������Ӧ��ý�����
	*/
	virtual H3DI::IMoveObject* get_IMoveObject() const;

	/**
	* @brief
	*		��ȡ��Χ��
	* @return
	*		��Χ������
	*/
	virtual BBOX get_BBox();

	/**
	* @brief
	*		ת��·�����ơ�
	*		��������ʽ��·�����ƣ���.\resource\stage\defined_object.xml��
	*		ת��Ϊ����./resource/stage/defined_object.xml��
	*
	*/
	void convert_path_string(std::string& str_path)
	{
		std::replace(str_path.begin(), str_path.end(), '\\', '/');
		std::transform(str_path.begin(), str_path.end(),  str_path.begin(), tolower);
	}

	/**
	* @brief
	*		�޸���Դ
	* @param
	*		res_name����IN��Ҫ�޸ĵ���Դ·��
	* @return
	*		true���޸ĳɹ�
	*		false���޸�ʧ��
	*/
	virtual bool change_resource(const std::string& res_name);

	/**
	* @brief
	*		�޸���Դ
	* @param
	*		object����IN��Ҫ�޸ĵ���Դ����
	* @return
	*		true���޸ĳɹ�
	*		false���޸�ʧ��
	*/
	virtual bool change_media_object(H3DI::IMoveObject* object, bool use_preproperty);

	/**
	* @brief
	*		������Դ��·����Ϣ
	* @param
	*		res_path����IN����Դ·��
	*/
	virtual void set_resource_name(const std::string& res_path);

	/**
	* @brief
	*		��ȡ��Դ��·����Ϣ
	* @return
	*		��Դ��·��
	*/
	virtual std::string resource_name() const;

	/**
	* @brief
	*		����Actor
	*/
	virtual void set_actor(Actor* actor);

	/**
	* @brief
	*		��ȡ�����λ�þ���
	* @return
	*		�����λ�þ���
	*/
	virtual const H3DMat4& get_world_location_matrix(void);

	/**
	* @brief
	*		���¼�������
	*/
	virtual void reload();
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
    bool IsMouseOnActor(int mouseX,int mouseY,int gameH,int gameW,float& distance, H3DVec4* viewPort = NULL);

	/**
	* @brief
	*		��level��ɾ����entity
	*/
	virtual void remove_from_level(CSceneEntity* level);

	virtual void setColorFactor(float* color, int size);
	virtual void set_diffuse_color_factor(float* color0, int size0, float* color1, int size1, int meshPart = -1, bool bUpdateChildren = true);

	/// ��Ⱦɫ
	/// meshPart��ʾģ�ͽ��ܱ�ɫ�������ID������һ��ģ�Ϳ�����MeshID����
	///     ����ǽ�ɫ����ʾ��װ��BodypartID��Ҽ�ID(BODYPART_NUM+�Ҽ�ID)����ֵΪ-1ʱ������ɫ��ģ�����еĲ��֡�
	/// bUpdateChildrenΪtrueʱ������ɫ��ģ�͵��ӽڵ㣨����Ҽ��ϵ���Ч��
	void ClearDiffuseColorFactor(int meshPart=-1, bool bUpdateChildren=true);

	/**
	* @brief
	*		��entity��ӽ�level
	*/
	virtual void add_to_level(CSceneEntity* level);

	/**
	* @brief
	*		������������
	* @param
	*		intervalTime����IN�������ϴθ��µ��¼����
	*/
	virtual void	update_media_object(float interval) ;

	/**
	* @brief
	*		������ʾ
	*/
	void update_appearance();

	/**
	* @brief
	*		�������ȼ����������´�update_appearance��Ч
	*/
	void appearance_priority(int priority);

	/**
	* @brief
	*		����һ֡��Ⱦ���ٸ���������BodyPart���Ҽ������Σ�
	* @param
	*		val��һ֡��Ⱦ���������������0��ʾ����֡��Ⱦ
	*/
	static void set_render_count(unsigned int val){s_render_count = val;}

	/**
	* @brief
	*		�����Ƿ�̬����
	*/
	void set_is_static(bool val);

	/**
	* @brief
	*		�Ƿ�ӱ�֡��ʼ���ƣ���update֮�����
	*/
	bool is_render();

	/**
	* @brief
	*		���ÿ��Ӿ���
	*/
	void set_visible_dist(float);
	void set_detail_actor_type(int type){m_detial_actor_type = type;};
	int get_detail_actor_type(){return m_detial_actor_type;};

	/**
	* @brief
	*		Entity�Ƿ�������
	*/
	bool is_loaded();

	/**
	* @brief
	*		�ͷ����л�����Դ
	*/
	virtual void UnloadResources();

	/**
	* @brief
	*		�������л�����Դ
	*/
	virtual void ReloadResources(bool is_sync = true, int priority = 0);

protected:

    /**
    	@brief   �ж�һ�������Ƿ�����г�����
    	@param   mousePos ���λ��
    	@param   dir ���߷���
    	@param   hitpoint ����һ������ֵ����ʶ���е�λ��
    	@Returns bool true = ���У�false = δ����
    */
    bool IsRayHit(const H3DVec3& mousePos, const H3DVec3& dir, H3DVec3& hitpoint);

protected:
	C3DEntity(CGraphicsResMng* res_mng, std::string name); //just for CSoundEntity

	//��һ����Ԫ��ת��ŷ����
	H3DVec3 quat_to_eula(const H3DQuat &quat );

	/**
	* @brief
	*		���¶���
	* @param
	*		intervalTime����IN�������ϴθ��µ��¼����
	*/
	virtual void do_update(float interval);

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

	/**
	* @brief
	*		�趨�����λ�þ���
	* @param
	*		location����IN�������λ�þ���
	*/
	virtual void set_world_location_matrix(const H3DMat4& location) ;

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
	H3DI::IMoveObject*		m_move_object; ///< ������Ӧ�Ķ���
	ENTITY_STATE		m_entity_state;
    CInteract_OBB*      m_OBB;      //���ڼ�����ѡ��
	float					m_cur_time; ///< ��ǰ����ʱ��
	float					m_media_length; ///< ý��ĳ���
	float					m_play_speed;  ///< ý��Ĳ����ٶ�
	bool					m_is_loop;	///<���趨�Ƿ�ѭ������
	bool					m_is_visible; ///< �趨��������Ļ���Ƿ���ʾ
	bool					m_is_external_visible;

	MEDIA_STATUS			m_status; ///< ý��������״̬

	H3DVec3					m_position; ///< ���λ��
	H3DVec3					m_eula; ///< ���ŷ����
	H3DQuat					m_quat; ///< �����Ԫ��
	H3DVec3					m_scale;///< ������ţ�������Model�ж������޸ġ���ʼֵΪ(1.0f,1.0f,1.0f)
	H3DMat4					m_location_matrix; ///< λ�þ���
	H3DMat4					m_world_location_matrix; ///< �����������

	std::string				m_resource_name; ///< ��Դ·��

	int					m_level_layer; ///<Level�Ĳ�

#ifdef _LOCATION_SET_OPTIMIZE_
	bool				m_prs_changed; //λ�ã���ת�����ŷ����˱仯
	bool				m_location_matrix_changed; //λ�þ������ı���
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

	//Cache������
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

	int		m_detial_actor_type; ///<�����ϸ������
	float		m_visible_dist;	///<�������ʧ����

//ȱ�����Զ��в���	
private:
	FORBIDDEN_EVIL_CONSTRUCTOR(C3DEntity);
};

#endif //_D_ENTITY_H_
