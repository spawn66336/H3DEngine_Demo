/**
* @file render_pipe_line.h
* 这个文件的主要功能是：
* 
* @author 李汕(Chengchen@h3d.com.cn)
* 
* 更改日志
* ----------------------------------------------------
* ver 1.0.0
*       by 李汕2010.1.27
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
    * 构造函数
    * 
    * \param name
    * 管道名字
    * 
    * \param type
    * 管道渲染类型
    * 
    * \param light
    * 管道渲染灯光
    */
    C2DRenderPipeline(CGraphicsResMng* resmng,const std::string& pipeline_name, H3DI::PIPE_TYPE type = H3DI::PIPE_COMMON, CLightEntity* light = NULL);
    virtual ~C2DRenderPipeline();

	/**
	* \brief
	* 绘制管道内的所有模型
	*
	* \param I_UI_Renderer
	* 绘制系统
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
