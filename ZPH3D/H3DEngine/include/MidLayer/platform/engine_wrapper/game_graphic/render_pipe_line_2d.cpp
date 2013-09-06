/**
* @file render_pipe_line.cpp
* 这个文件的主要功能是：
* 
* @author 李汕(Chengchen@h3d.com.cn)
* 
* 更改日志
* ----------------------------------------------------
* ver 1.0.0
*       by 李汕2010.1.27
*/

#include "game_graphic_body.h"

C2DRenderPipeline::C2DRenderPipeline(CGraphicsResMng* resmng,const std::string& name, H3DI::PIPE_TYPE type, CLightEntity* light)
:C3DRenderPipeline(name, type, light )
,m_resmng(resmng)
{

}

C2DRenderPipeline::~C2DRenderPipeline(void)
{
	m_postrender_list.clear();
}
void C2DRenderPipeline::pipelineRender(I_UI_Renderer* uirenderer) 
{
	H3DI::IRender* iRender = uirenderer->getRenderer();

	ISpecialEffectManager* iSEM = m_resmng->get_ui_wndmng()->getClientUI()->getEffectMng();;

	iRender->PushProjectionMatrix();
	iRender->PushCamera();
	iRender->PushViewMatrix();

	//设置投影矩阵
	H3DMat4 mat;
	iRender->GetOrthoProjectionMatrix(0,1024,0, 768, - 2048, 2048, mat);
	iRender->SetPerspective(mat);

	//设置视图矩阵
	H3DVec3 farp(4000, 4000, 4000);
	H3DVec3 tarp(0, 0, 0);
	H3DVec3 upp(0, 0, 1);
	iRender->GetRenderCamera()->SetPosition(farp);
	iRender->LookAt(farp, tarp, upp);
	iRender->UpdateCamera();

	iRender->Set2DMode(true);

	std::list<I_Post_Render*>::iterator it=m_postrender_list.begin();
	
	for(;it != m_postrender_list.end();++it)
	{
		(*it)->postRender(uirenderer);
	}

	iRender->Render2DObj();	//2d物体、特效...
	iRender->Set2DMode(false);

	iRender->PopViewMatrix();
	iRender->PopCamera();
	iRender->PopProjectionMatrix();
}

void C2DRenderPipeline::render(I_UI_Renderer* uirenderer)
{
	for (size_t i = 0; i < m_medias.size(); ++i)
	{
		render_media(m_medias[i], uirenderer);
	}
}
 
void C2DRenderPipeline::render_media(CGraphicEntity* media, I_UI_Renderer* uirenderer)
{
	if (media->is_enable() && media->is_kind_of(GRAPHIC_ENTITY_TYPE_2D_ENTITY))
	{
		CGraphic2DEntity* entity = dynamic_cast<CGraphic2DEntity*>(media);
		if (NULL != entity)
		{
			entity->render(uirenderer);		

			CGraphic2DEntity::EntityList::iterator it = media->children().begin();
			for (; it != media->children().end(); ++it)
			{
				render_media(*it, uirenderer);
			}
		}
	}
}
bool C2DRenderPipeline::addPostRender(I_Post_Render* postrender) 
{
	m_postrender_list.push_back(postrender);
	return true;
}
bool C2DRenderPipeline::removePostRender(I_Post_Render* postrender) 
{
	std::list<I_Post_Render*>::iterator it=m_postrender_list.begin();
	
	for(;it != m_postrender_list.end();++it)
	{
		if((*it) == postrender)
		{
			m_postrender_list.erase(it);
			return true;
		}
	}
	return true;
}
