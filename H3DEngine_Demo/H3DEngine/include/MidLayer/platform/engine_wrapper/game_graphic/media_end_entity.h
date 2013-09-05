/**
* @file media_end_entity.h
* ����ļ�����Ҫ�����ǣ�������ý���Ļ���ӿڡ�
* 
* @author �̳�(Chengchen@h3d.com.cn)
* 
* ������־
* ----------------------------------------------------
* ver 1.0.0
*       by �̳�2010.1.25
*/

#ifndef _MEDIA_ENTITY_H_
#define _MEDIA_ENTITY_H_

#include "game_graphic_header.h"

class CGraphicsResMng;
class CRenderSystem;
class Actor;
class CSceneEntity;
/**
* @brief
*		CMediaEndEntity�ǿɿ�ʵ��Ļ��࣬
*		�����˿ɿ�ʵ��Ӧ�þ��е�һЩ��������Ϊ
*/
class CMediaEndEntity
	//:public SignalDispatcher
{
public:
	CMediaEndEntity(CGraphicsResMng* mng, const std::string& entity_name);
	CMediaEndEntity();
	virtual ~CMediaEndEntity(void);

	/**
	* @brief
	*		��ȡָ��ͼ�λ��Ʋ���Դ��������ָ��
	* @return
	*		ָ��ͼ�λ��Ʋ���Դ��������ָ��
	*/
	virtual CGraphicsResMng* get_graphics_res_mng(void) ;


	//////////////////////////////////////////////////////////////////////////
	/**
	* @brief
	*		��ȡ�������
	* @return
	*		�������
	*/
	virtual int		type(void) const;

	/**
	* @brief
	*		��ȡ��������
	* @param
	*		name����IN�����������
	*/
	virtual void		set_name(const char* obj_name) ;

	/**
	* @brief
	*		��ȡ��������
	* @return
	*		���������
	*/
	virtual const char*	get_name(void) const;

	/**
	* @brief
	*		�ж϶����Ƿ�ΪҪ�������
	* @param
	*		obj_type����IN���������
	* @return
	*		true��Ϊָ��������
	*		false����Ϊָ��������
	*/
	virtual bool is_kind_of(int obj_type) ;

	/**
	* @brief 
	*		�ɿ�ʵ���Ƿ����
	* @return 
	*		True���ɿ�ʵ�����
	*		False���ɿ�ʵ�岻����
	*/
	virtual bool is_enable(void) const ;

	/**
	* @brief
	*		���ÿɿ�ʵ���״̬�Ƿ����
	* @param
	*		is_eanbel����IN���ɿ�ʵ���״̬���ñ�־
	*/
	virtual void set_enable(bool enable) ;

	/**
	* @brief
	*		���¶���
	* @param
	*		intervalTime����IN�������ϴθ��µ��¼��������λ����
	*/
	virtual void	update(float interval) ;

	/**
	* @brief
	*		��Ⱦ����
	* @param
	*		renderSystem����IN������ϵͳ
	*/
	virtual void render(CRenderSystem& render_system);

	/**
	* @brief
	*		����Ӳ��ѡ����Ⱦ
	* @param
	*		renderSystem����IN������ϵͳ
	*		sel_line: �Ƿ�������Ⱦ
	* @return
	*		true��������Ⱦ
	*		false����������Ⱦ
	*/
	virtual bool pushHardwareSelect(CRenderSystem& render_system, bool sel_line);


	/**
	* @brief
	*		����Actor
	*/
	virtual void set_actor(Actor* actor);
	/**
	* @brief
	*		��ȡActor
	*/
	virtual Actor* get_actor();


	unsigned int get_entity_id() const;

	virtual void setColorFactor(float* color, int size) {};

	/**
	* @brief
	*		���úͻ�ȡģ��LOD���ƣ� Ŀǰ�м��LOD���ý�Ӱ��ģ�ͼ������ࡢ��Ч����׶���
	*/
	virtual void switch_to_lod(int lod, bool is_sync = true){m_lod_level = lod;};
	LOD_LEVEL get_lod(){return (LOD_LEVEL)m_lod_level;};

	/**
	* @brief
	*		��ӽ�level
	*/
	virtual void add_to_level(CSceneEntity* level){m_level_entity = level;};
	virtual void remove_from_level(CSceneEntity* level)
	{
		assert(level == m_level_entity);
		m_level_entity = NULL;
	};

	/**
	* @brief
	*		���úͻ�ȡģ��LOD���ƣ� Ŀǰ�м��LOD���ý�Ӱ��ģ�ͼ������ࡢ��Ч����׶���
	*/
	bool is_added_to_level(){return m_level_entity != 0;};

	/*
	* @brief 
	*		���������GUID����
	*/
	virtual void set_guid(const char* guid){ m_guid = guid;}
	const char* get_guid()const {return m_guid.c_str();};

	/**/
	void set_update_delta_time(float delta)
	{
		m_b_use_delta_time = true;
		m_f_delta_time = delta;
	}

	/**
	* @brief
	*		�趨�����Ƿ�ɼ�
	* @param
	*		is_visible����IN���趨�����Ƿ�ɼ� ,��ʵ��,������ʵ��
	*/
	virtual void set_visible(bool visible) {};

	/*
	* @brief
	*		�����ⲿ�趨�����Ƿ�ɼ����ÿ��������Ժ����߶�������֮�⣬����entity��������
	*
	*/
	virtual void set_external_visible(bool visible){};

protected:
	/**
	* @brief
	*		���¶���
	* @param
	*		intervalTime����IN�������ϴθ��µ��¼��������λ����
	*/
	virtual void do_update(float interval);

protected:
	std::string m_name; ///< ��������
	bool	m_is_enable;				///< �ɿ�ʵ���Ƿ����

	CGraphicsResMng*	m_res_mng;  ///< ָ��ͼ����Դ��������ָ��

	Actor*		m_actor; ///< ������Actor

	unsigned int					m_entity_id;
	static	unsigned int			m_s_entity_id;

	int				m_lod_level;
	/*
	*���ڱ�ʾ��entity�Ƿ��Ѿ������뵽level�С�
	*��������CPrePassLightEntity�������࣬CModelEntity��������
	*��Ч��
	*/
	CSceneEntity*		m_level_entity;
	std::string				m_guid;		//GUID����

	/*
	* ���ڶ������š���Ч���ŵ���ʱ����ص������޸�����	
	* ��һ֡�����У������ʱ��Ϊt������Ӧ�������Լ����Եĸ��������˶��������Լ���Ч���ţ�
	* ��Ӧ��entity��Ӧ���µ�ʱ��Ӧ����t1��t=t0+t1��t0��ʾ��һ֡�����õ�ǰ����/��Ч֮ǰ��ʱ�䣬
	* t1��ʾ��һ֡�����õ�ǰ����/��Ч֮���ʱ�䣩
	* �����t1��m_f_delta_time����ʾ��
	* m_b_use_delta_time���ڱ�ʾ�Ƿ�ʹ��m_f_delta_time����entity��������߼�������update()��
	*/
	bool	m_b_use_delta_time;
	float	m_f_delta_time;
protected:
//private:
	//	FORBIDDEN_EVIL_CONSTRUCTOR(CMediaEndEntity);
};

#endif //_MEDIA_ENTITY_H_
