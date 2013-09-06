/**
* @file pawn.h
* ����ļ�����Ҫ�����ǣ�
* 
* @author �̳�(Chengchen@h3d.com.cn)
* 
* ������־
* ----------------------------------------------------
* ver 1.0.0
*       by �̳�2010.1.27
*/

#ifndef _PAWN_H_
#define _PAWN_H_

#include "game_graphic_header.h"

/**
* \brief
* Pawn
* 
* Pawn�����пɿ�ʵ��Ļ���, ������֯�˶��ý��,
* �Լ�����ý��Ļ��Ʒ�ʽ
*/
class Pawn
{
public:
    Pawn();

    virtual ~Pawn();

	/**
	* @brief
	*		��ȡ��������
	* @param
	*		name����IN�����������
	*/
	void set_name(const std::string& name);
	/**
	* @brief
	*		��ȡ��������
	* @return
	*		���������
	*/
	const std::string& get_name() const;

	/**
	* @brief
	*		����,����ʱ��Ϊ����
	* @brief
	*		����ʱ��
	*/
	void update(float t);

	/**
	* @brief
	*		����pawn
	*/
	void render(CRenderSystem& render_system);

//////////////////////////////////////////////////////////////////////////
	/**
	* @brief
	*		��ȡ����ָ��entity_name�Ŀɿ�ʵ��
	* @param
	*		entity_name����IN���ӿɿ�ʵ����������
	* @return
	*		���ز��ҵ�����������ָ�룬���û�У�����NULL
	*/
	virtual CGraphicEntity* find_entity(const std::string& entity_name) const ;

	/**
	* @brief
	*		����ӿɿ�ʵ��
	* @param
	*		entity����IN��Ҫ��ӵ��ӿɿ�ʵ�����ָ��
	* @return
	*		��ӳɹ�������true
	*		���ʧ�ܣ�����false
	*/
	virtual bool			add_entity(CGraphicEntity* entity) ;

	/**
	* @brief
	*		�Ƴ��ӿɿ�ʵ�����
	* @param
	*		entity����IN��Ҫ�Ƴ����ӿɿ�ʵ������ָ��
	* @return
	*		ɾ���ɹ�������true
	*		ɾ��ʧ�ܣ�����false	
	*/
	virtual bool			remove_entity(CGraphicEntity* entity) ;

	/**
	* @brief
	*		�Ƴ������ӿɿ�ʵ��
	*/
	virtual void clear_entities() ;

//////////////////////////////////////////////////////////////////////////
	/**
	* @brief
	*		���pawn��
	* @param
	*		pawn����IN��Ҫ��ӵ�pawn
	* @return
	*		��ӳɹ�������true
	*		���ʧ�ܣ�����false
	*/
	virtual bool add_pawn(Pawn* pawn);

	/**
	* @brief
	*		ɾ��pawn
	* @param
	*		pawn����IN��Ҫɾ����pawn
	*/
	virtual bool remove_pawn(Pawn* pawn);

	/**
	* @brief
	*	������е�pawn
	*/
	virtual void clear_pawns();


	/**
	* @brief
	*		��ȡpawn
	* @param
	*		pawn_name����IN��Ҫ��ȡ��pawn������
	*/
	virtual Pawn*	find_pawn(const std::string& pawn_name) const;
//////////////////////////////////////////////////////////////////////////
	/**
	* @brief
	*		����Pawn��PawnControl
	*/
	virtual void set_pawn_control(IPawnControl* pawn_control);
	/**
	* @brief
	*		��ȡPawn��PawnControl
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
	*		��ȡָ���б���ָ�����Ƶ�Ԫ��
	* @param
	*		element_name����IN��Ҫ��ȡ��Ԫ�ص�����
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
	EntityList			m_graphic_entity_collection; ///< �ɿ�ʵ����
	PawnList			m_child_pawn_collection;	///< ��pawn����
	PawnControlList     m_control_collection; ///< ����������

	std::string			m_name; ///< pawn������
	H3DMat4				m_loc_matrix; ///< Pawn��λ��
	bool				m_visible; ///< Pawn�Ƿ�ɼ�

	H3DVec3					m_position; ///< ���λ��
	H3DVec3					m_eula; ///< ���ŷ����
	H3DQuat					m_quat; ///< �����Ԫ��
	H3DVec3					m_scale;///< ������ţ�������Model�ж������޸ġ���ʼֵΪ(1.0f,1.0f,1.0f)
	H3DMat4					m_location_matrix; ///< λ�þ���
};

#endif //_PAWN_H_
