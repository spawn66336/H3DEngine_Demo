/**
* @file point_prepass_light_entity.h
* ����ļ�����Ҫ�����ǣ�
* 
* @author �̳�(Chengchen@h3d.com.cn)
* 
* ������־
* ----------------------------------------------------
* ver 1.0.0
*       by �̳�2010.5.21
*/

#ifndef _POINT_PREPASS_LIGHT_ENTITY_H_
#define _POINT_PREPASS_LIGHT_ENTITY_H_

#include "game_graphic_header.h"

class CPointPrePassLightEntity
	:public CPrePassLightEntity
{
public:
	CPointPrePassLightEntity(CGraphicsResMng* res_mng, const std::string& name, H3DI::IPrePassLight* light);
	virtual ~CPointPrePassLightEntity();

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

	//���Ƶƹ���
	virtual void draw_light_location_line(CRenderSystem& render_sys);
private:
	FORBIDDEN_EVIL_CONSTRUCTOR(CPointPrePassLightEntity);
};

#endif //_POINT_PREPASS_LIGHT_ENTITY_H_
