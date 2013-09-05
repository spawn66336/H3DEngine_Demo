/**
* @file render_pipe_line.h
* ����ļ�����Ҫ�����ǣ�
* 
* @author ����(Chengchen@h3d.com.cn)
* 
* ������־
* ----------------------------------------------------
* ver 1.0.0
*       by ����2010.1.27
*/

#ifndef _RENDER_PIPE_LINE_2D_H_
#define _RENDER_PIPE_LINE_2D_H_

#include "game_graphic_header.h"
class CLightEntity;
class CGraphicsResMng;
/**
 * \brief
 * C2DRenderPipeline
 * 
 */
class C2DRenderPipeline: public C3DRenderPipeline,public I_Render_Pipeline
{
public:
    /**
    * \brief
    * ���캯��
    * 
    * \param name
    * �ܵ�����
    * 
    * \param type
    * �ܵ���Ⱦ����
    * 
    * \param light
    * �ܵ���Ⱦ�ƹ�
    */
    C2DRenderPipeline(CGraphicsResMng* resmng,const std::string& pipeline_name, H3DI::PIPE_TYPE type = H3DI::PIPE_COMMON, CLightEntity* light = NULL);
    virtual ~C2DRenderPipeline();

	/**
	* \brief
	* ���ƹܵ��ڵ�����ģ��
	*
	* \param I_UI_Renderer
	* ����ϵͳ
	*/
	virtual void render(I_UI_Renderer* uirenderer);

	virtual void pipelineRender(I_UI_Renderer* uirenderer) ;

	virtual bool addPostRender(I_Post_Render* postrender) ;
	virtual bool removePostRender(I_Post_Render* postrender) ;
private:
	void render_media(CGraphicEntity* media, I_UI_Renderer* uirenderer);

	std::list<I_Post_Render*> m_postrender_list;	
	CGraphicsResMng* m_resmng;
protected:
};

#endif //_RENDER_PIPE_LINE_H_
