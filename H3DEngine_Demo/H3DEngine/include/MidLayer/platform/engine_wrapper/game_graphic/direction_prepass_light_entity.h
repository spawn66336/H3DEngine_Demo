/**
* @file direction_prepass_light_entity.h
* ����ļ�����Ҫ�����ǣ�
* 
* @author �̳�(Chengchen@h3d.com.cn)
* 
* ������־
* ----------------------------------------------------
* ver 1.0.0
*       by �̳�2010.6.9
*/

#ifndef _DIRECTION_PREPASS_LIGHT_ENTITY_H_
#define _DIRECTION_PREPASS_LIGHT_ENTITY_H_

#include "game_graphic_header.h"

class CDirectionPrePassLightEntity
	:public CPrePassLightEntity
{
public:
	CDirectionPrePassLightEntity(CGraphicsResMng* res_mng, const std::string& name, H3DI::IPrePassLight* light);
	virtual ~CDirectionPrePassLightEntity();

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

	//���Ƶƹ���
	virtual void draw_light_location_line(CRenderSystem& render_sys);

	virtual void switch_to_lod(int lod, bool is_sync = true);

	/*
	* @brief
	*		���÷����ĸ߹�ǿ��
	*/
	virtual void set_specular_intensity(float fIntensity);
	float	get_specular_intensity() const{ return m_specular_intensity;};
	/**
	* @brief
	*		���õƹ�ǿ��
	* @param
	*		light_intensity����IN��Ҫ���õĵƹ�ǿ��
	*/
	virtual void set_intensity(float light_intensity);

private:
	float		m_specular_intensity;
	FORBIDDEN_EVIL_CONSTRUCTOR(CDirectionPrePassLightEntity);

};

#endif //_DIRECTION_PREPASS_LIGHT_H_
