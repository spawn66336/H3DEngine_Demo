/**
* @file i_move_entity.h
* ����ļ�����Ҫ�����ǣ�
* 
* @author �̳�(Chengchen@h3d.com.cn)
* 
* ������־
* ----------------------------------------------------
* ver 1.0.0
*       by �̳�2010.1.25
*/

#ifndef _I_MOVE_ENTITY_H_
#define _I_MOVE_ENTITY_H_

#include "game_graphic_header.h"

class IMoveEntity
{
public:
	virtual ~IMoveEntity() {};

	/**
	* @brief
	*		��ȡ�����λ��
	* @return
	*		�����λ��
	*/
	virtual H3DVec3 position(void) const = 0;

	/**
	* @brief
	*		�趨�����λ��
	* @param
	*		positon����IN�������λ��
	*/
	virtual void set_position(const H3DVec3& pos) = 0;

	///**
	//* @brief 
	//*		��ȡ����ķ���
	//* @return
	//*		����ķ���
	//*/
	//virtual H3DVec3 direction(void) const = 0;

	///**
	//* @brief
	//*		�趨����ķ���
	//* @param
	//*		direction����IN������ķ���
	//*/
	//virtual void set_direction(const H3DVec3& dir) = 0;

	/**
	* @brief
	*		��ȡ�����λ�þ���
	* @return
	*		�����λ�þ���
	*/
	virtual const H3DMat4& get_location_matrix(void) const = 0;

	/**
	* @brief
	*		�趨�����λ�þ���
	* @param
	*		location����IN�������λ�þ���
	*/
	virtual void set_location_matrix(const H3DMat4& location) = 0;

	/**
	* @brief
	*		��ȡ�����ŷ���ǣ�ŷ���Ǻ�max���汣��һ��
	* @return
	*		����һ��h3cVec��ֵ���ֱ��ʾ��x��y��z���������ת����
	*/
	virtual H3DVec3 rotation() const = 0;

	/**
	* @brief
	*		�趨�����ŷ���ǣ�ŷ���Ǻ�max���汣��һ��
	* @return
	*		����һ��h3cVec��ֵ���ֱ��ʾ��x��y��z���������ת����
	*/
	virtual void set_rotation(const H3DVec3& obj_rotation) = 0;
};

#endif //_I_MOVE_ENTITY_H_
