/**
* @file render_system.cpp
* 这个文件的主要功能是：
* 
* @author 程陈(Chengchen@h3d.com.cn)
* 
* 更改日志
* ----------------------------------------------------
* ver 1.0.0
*       by 程陈2010.1.26
*/

#include "game_graphic_body.h"

CRenderSystem::CRenderSystem(CGraphicsResMng* mng)
:m_render(NULL), m_effect_manager(NULL),
m_delete_renderer_dx(NULL), m_delete_ispecial_effect_manager(NULL), m_engine_module(NULL), m_effect_module(NULL)
, m_sound_module(NULL), m_draw_shader(NULL)
{
	assert(NULL != mng);
	m_res_mng = mng;
	m_render_camera = NULL;
	m_shader_ui_lib_path = "..\\resources\\art\\ui\\common\\Coord2.xml";

	m_ambient_color = H3DColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_ambient_actor_color = H3DColor(0.0f, 0.0f, 0.0f, 1.0f);
}

CRenderSystem::~CRenderSystem()
{
	cleanUp();
}

bool CRenderSystem::initialize(H3DI::IRender* irender)
{
	m_render = irender;
	m_effect_manager = irender->CreateEffectManager();

	//m_render->AddPostProcess("..\\resources\\media\\shaders\\PostProcess\\LightColumnEffectX52.xml");

	//m_effect_manager->setEditModel(true);
	//m_render->SetAmbientColor(m_ambient_color.getColor());

	return true;
}

int CRenderSystem::init_effectd_variable(const std::string& effectDllFile , const std::string& effectManagerCreatorName , const std::string& effectManagerDeleteName , std::string* errorMsg)
{
	/*暂时没有确定错误返回值列表，所以只定义两个常量，表示返回结果*/
	const int INT_SUCCESS_RETURN = 0;
	const int INT_ERROR_RETURN = 1;

	m_effect_module = ::LoadLibraryA(effectDllFile.c_str());
	if (m_effect_module == NULL)
	{
		*errorMsg = effectDllFile + "加载失败!";		
		return INT_ERROR_RETURN;
	}

	CreateISpecialEffectManager  createISpecialEffectManager;
	createISpecialEffectManager = (CreateISpecialEffectManager) GetProcAddress(m_effect_module, effectManagerCreatorName.c_str());
	if(createISpecialEffectManager == NULL)
	{
		*errorMsg = effectDllFile + effectManagerCreatorName + "读取失败!";
		return INT_ERROR_RETURN;
	}

	m_delete_ispecial_effect_manager =(DeleteISpecialEffectManager) GetProcAddress(m_effect_module, effectManagerDeleteName.c_str());
	if(m_delete_ispecial_effect_manager == NULL)
	{
		*errorMsg = effectDllFile + effectManagerDeleteName + "读取失败!";
		return INT_ERROR_RETURN;
	}

	m_effect_manager = createISpecialEffectManager(m_render);
	if (m_effect_manager == NULL)
	{
		*errorMsg = "创建特效引擎失败!";
		return INT_ERROR_RETURN;
	}

	return INT_SUCCESS_RETURN;
}

void CRenderSystem::set_af_mode(H3DI::EAFMode afMode)
{
	m_render->SetAFMode(afMode);
}

H3DI::EAFMode CRenderSystem::get_af_mode() const
{
    return m_render->GetAFMode();
}

void CRenderSystem::set_aa_mode(H3DI::EAAMode aaMode)
{
	m_render->SetAAMode(aaMode);
}

H3DI::EAAMode CRenderSystem::set_aa_mode() const
{
	return m_render->GetAAMode();
}

void CRenderSystem::set_clear_flags(unsigned int flags)
{
	m_render->SetClearFlags(flags);
}

unsigned int CRenderSystem::get_clear_flags() const
{
	return m_render->GetClearFlags();
}

void CRenderSystem::set_use_triangle_list(bool use)
{
	return m_render->SetUseTriList(use);
}

bool CRenderSystem::get_use_triangle_list() const
{
	return m_render->GetUseTriList();
}

void CRenderSystem::set_vetical_sync(bool veticalSync)
{
	m_render->SetVSync(veticalSync);
}

bool CRenderSystem::get_vetical_sync() const
{
	return m_render->GetVSync();
}

void CRenderSystem::begin_render()
{
	//float color[] = {0.0f, 0.0f, 0.0f, 0.0f};
	//get_IRender()->SetClearColor(color);

	//get_IRender()->FrameBegin();
	//get_IRender()->ClearScreen();

#ifdef _EDITOR
	get_IRender()->PushProjectionMatrix();
	//get_IRender()->PushCamera();
	get_IRender()->PushViewMatrix();
#endif

	if (NULL != m_render_camera)
	{
		H3DVec3 position = m_render_camera->position();
		H3DVec3 direction = m_render_camera->direction();
		H3DVec3 up = m_render_camera->up_direction();
		float fov, ratio, fNear, fFar;
		m_render_camera->get_frustum(&fov, &ratio, &fNear, &fFar);
		get_IRender()->SetFrustum(fov, ratio, fNear, fFar);
		get_IRender()->LookAt(position, position + direction * 1000, up);
		get_IRender()->UpdateCamera();
	}
}

/**
* @brief
*		渲染对象
*/
void CRenderSystem::render()
{
	get_IRender()->Render();
}

void CRenderSystem::end_render()
{
#ifdef _EDITOR

	get_IRender()->PopViewMatrix();
	//get_IRender()->PopCamera();
	get_IRender()->PopProjectionMatrix();

#endif
	
}

void CRenderSystem::save_screen_to_file(const std::string& fileName)
{
	m_render->SaveScreenToFile(fileName.c_str());
}

H3DI::EAFMode CRenderSystem::get_supported_af_mode()
{
	return static_cast<H3DI::EAFMode>(m_render->GetSupportedAFMode());
}

H3DI::EAAMode CRenderSystem::get_supported_aa_mode()
{
	return static_cast<H3DI::EAAMode>(m_render->GetSupportedAAMode());
}

void CRenderSystem::set_window_mode(WINDOW_MODE windowMode)
{
	if (WINDOW_NORMAL == windowMode)
	{
		m_render->ToWindowedMode();
	}
	else
	{
		m_render->ToFullScreenMode();
	}
}

void CRenderSystem::get_screen_position(const H3DVec3& objectPosition, const H3DMat4& worldViewMat, const H3DMat4& projectionMat, 
					   unsigned int x, unsigned int y, unsigned int width, unsigned height, float minZ, float maxZ, H3DVec3& screenPosition) const
{
	m_render->GetScreenPos(objectPosition, worldViewMat, projectionMat, x, y, width, height, minZ, maxZ, screenPosition);
}

void CRenderSystem::get_ortho_projection_matrix(float left, float right, float bottom, float top, float fnear, float ffar, H3DMat4& out) const
{
	m_render->GetOrthoProjectionMatrix(left, right, bottom, top, fnear, ffar, out);
}

HWND CRenderSystem::get_window_handle() const
{
	return static_cast<HWND>(m_render->GetWindowHandle());
}

//void CRenderSystem::preloadActions(const H3DActionNameSequence& actionSequence, H3DSkeletonModel *skeletonModel)
//{
//	unsigned int count = actionSequence.size();
//	const char **actionNames = new const char *[count];
//	for (size_t i = 0; i < count; ++i)
//	{
//		actionNames[i] = actionSequence[i].c_str();
//	}
//	m_Render->PreLoadActions(actionNames, count, skeletonModel->getISkeletonModel());
//	delete [] actionNames;
//}

void CRenderSystem::clear_loaded_actions()
{
	m_render->ClearAllActions();
}

void CRenderSystem::cleanUp()
{
	if (m_draw_shader != NULL)
	{
		m_draw_shader->Release();
		m_draw_shader = NULL;
	}

	if (m_render != NULL && m_delete_renderer_dx != NULL)
	{
		//m_delete_renderer_dx();
	}
	if (m_engine_module != NULL)
	{
		::FreeLibrary(m_engine_module);
		m_engine_module = NULL;
	}
	if (NULL != m_sound_module)
	{
		::FreeLibrary(m_sound_module);
		m_sound_module = NULL;
	}
	m_render = NULL;
	m_effect_manager = NULL;
	m_delete_renderer_dx = NULL;
	m_delete_ispecial_effect_manager = NULL;
}

ISpecialEffectManager* CRenderSystem::getIEffectManager() const
{
	return m_effect_manager;
}

void CRenderSystem::setIEffectManager(ISpecialEffectManager* effectManager)
{
	m_effect_manager = effectManager;
}


H3DI::IRender* CRenderSystem::get_IRender() const
{
	return m_render;
}

void CRenderSystem::setIRender(H3DI::IRender* render)
{
	assert(NULL != render);
	m_render = render;
}

CCameraEntity* CRenderSystem::get_render_camera() const
{ 
	return m_render_camera; 
}

CCameraEntity* CRenderSystem::set_render_camera(CCameraEntity* camera) 
{ 
	CCameraEntity* old_camera = m_render_camera;
	m_render_camera = camera;
	return old_camera;
}


void CRenderSystem::drawLightLine(CPrePassLightEntity* media)
{
	if (media->is_kind_of(MEDIA_END_ENTITY_TYPE_WIMBLE_PREPASS_LIGHT))
	{
		CWimblePrePassLightEntity *pLight =(CWimblePrePassLightEntity*)media;
		if(pLight->get_beam_line_draw())
		{
			H3DVec3 pos,scale;
			H3DQuat quat;
			H3DMat4 mat;
			mat = pLight->get_world_location_matrix();
			mat.Decompose(scale,quat,pos);

			int LineNum = 360;
			//画外径
			float lightRange = pLight->far_range();
			float angle = pLight->outter_angle() * H3DMath::M_DEG2RAD;
			float color[]={1.0f,1.0,1.0f,1.0f};
			draw_a_wimble(angle, lightRange, quat, LineNum, pos, mat, color, m_render);
			//画内径
			lightRange = pLight->near_range();
			angle = pLight->inner_angle() * H3DMath::M_DEG2RAD;
			float color2[]={0.0f,0.0,1.0f,1.0f};
			draw_a_wimble(angle, lightRange, quat, LineNum, pos, mat, color2, m_render);
		}
	}
	else if (media->is_kind_of(MEDIA_END_ENTITY_TYPE_POINT_PREPASS_LIGHT))
	{
		CPointPrePassLightEntity *pLight =(CPointPrePassLightEntity*)media;
		if(pLight->get_beam_line_draw())
		{
			H3DVec3 pos,scale;
			H3DQuat quat;
			H3DMat4 mat;
			mat = pLight->get_world_location_matrix();
			mat.Decompose(scale,quat,pos);

			int LineNum = 20;
			//画外径
			float lightRange = pLight->far_range();

			float color[]={1.0f,1.0,1.0f,1.0f};
			draw_a_sphere(lightRange,LineNum, pos, color, m_render);
			//画内径
			lightRange = pLight->near_range();

			float color2[]={0.0f,0.0,1.0f,1.0f};
			draw_a_sphere(lightRange,LineNum, pos, color2, m_render);
		}
	}
	else if (media->is_enable() && media->is_kind_of(MEDIA_END_ENTITY_TYPE_DIRECTION_PREPASS_LIGHT))
	{
		CDirectionPrePassLightEntity* dir_light = (CDirectionPrePassLightEntity*)media;
		if (dir_light->get_beam_line_draw())
		{
			H3DVec3 pos,scale;
			H3DQuat quat;
			H3DMat4 mat;
			mat = dir_light->get_world_location_matrix();
			mat.Decompose(scale,quat,pos);

			int light_num = 20;
			float color[]={1.0f,1.0,1.0f,1.0f};

			float line_list[20];
			line_list[0] = pos.x;
			line_list[1] = pos.y;
			line_list[2] = pos.z;
// 			H3DVec3 dir;
// 			dir_light->get_IPrePassLight()->GetDirection(dir);
			H3DVec3 dir = quat * H3DVec3(0, -1, 0);
			dir = dir / dir.Length() * 200;
			line_list[3] = pos.x + dir.x;
			line_list[4] = pos.y + dir.y;
			line_list[5] = pos.z + dir.z;
			draw_line(line_list, 1, color, m_render);
		}
	}


}


void CRenderSystem::draw_a_wimble( float angle, float lightRange,  H3DQuat quat, int LineNum, H3DVec3 &pos, H3DMat4 mat, float* color, H3DI::IRender * irender )
{
	//锥底的半径
	float radius = tan(angle*0.5f)*lightRange;
	H3DVec3 dir;
	//锥的方向
	dir = quat*H3DVec3(0.0f,-1.0f,0.0f);
	int pointnum = (LineNum + 4)*2;//一个锥由4条母线和一个底组成
	float *pLineList = new float[pointnum*3];
	//取锥底的若干顶点
	for(int nLine = 0; nLine < LineNum; nLine++)
	{
		// 		//计算母线
		// 		pLineList[nLine*6] = pos.x;
		// 		pLineList[nLine*6 + 1] = pos.y;
		// 		pLineList[nLine*6 + 2] = pos.z;
		// 
		H3DVec4 vEnd(H3DMath::Cos((float)nLine/LineNum*H3DMath::PI*2.0f)*radius ,lightRange,H3DMath::Sin((float)nLine/LineNum*H3DMath::PI*2.0f)*radius,1.0f);
		H3DVec4 vEndTrans = mat*vEnd;
		// 		pLineList[nLine*6 + 3] = vEndTrans.x;
		// 		pLineList[nLine*6 + 4] = vEndTrans.y;
		// 		pLineList[nLine*6 + 5] = vEndTrans.z;

		//计算底
		pLineList[nLine*6  ] = vEndTrans.x;
		pLineList[nLine*6 + 1] = vEndTrans.y;
		pLineList[nLine*6 + 2] = vEndTrans.z;
		if(nLine)
		{
			pLineList[nLine*6 - 3 ] = vEndTrans.x;
			pLineList[nLine*6 - 2 ] = vEndTrans.y;
			pLineList[nLine*6 - 1] = vEndTrans.z;
		}
		else
		{
			pLineList[LineNum * 6  - 3] = vEndTrans.x;
			pLineList[LineNum * 6  - 2] = vEndTrans.y;
			pLineList[LineNum * 6  - 1] = vEndTrans.z;
		}
		for(int i = 0; i< 4; ++i)
		{
			//计算母线
			pLineList[(LineNum + i) * 6] = pos.x;
			pLineList[(LineNum + i) * 6 + 1] = pos.y;
			pLineList[(LineNum + i) * 6 + 2] = pos.z;

			H3DVec4 v_end(H3DMath::Cos((float)i/4.0f*H3DMath::PI*2.0f)*radius ,lightRange,H3DMath::Sin((float)i/4.0f*H3DMath::PI*2.0f)*radius,1.0f);
			H3DVec4 v_end_trans = mat*v_end;
			pLineList[(LineNum + i) * 6 + 3] = v_end_trans.x;
			pLineList[(LineNum + i) * 6 + 4] = v_end_trans.y;
			pLineList[(LineNum + i) * 6 + 5] = v_end_trans.z;
		}
	}

	draw_line(pLineList,LineNum+4,color,irender);
	delete [] pLineList;
}

void CRenderSystem::draw_a_sphere( float lightRange, int LineNum, H3DVec3 &pos, float* color, H3DI::IRender * irender )
{
	float radius = lightRange;

	int pointnum = (LineNum * LineNum) * 2 + (LineNum * (LineNum - 1)) * 2 ;
	float *pLineList = new float[pointnum*3];

	//用经线画球
	for(int y = 0; y < LineNum; ++y)
	{
		for(int x = 0; x < LineNum; ++x)
		{	
			float radiusx = H3DMath::Sin((float)y/LineNum*H3DMath::PI)*radius ;
			H3DVec3 vEnd( H3DMath::Cos((float)x/LineNum*H3DMath::PI*2.0f)*radiusx,H3DMath::Sin((float)x/LineNum*H3DMath::PI*2.0f)*radiusx,H3DMath::Cos((float)y/LineNum*H3DMath::PI)*radius);
			vEnd += pos;
			pLineList[(LineNum * y + x) * 6] = vEnd.x;
			pLineList[(LineNum * y + x) * 6 + 1] = vEnd.y;
			pLineList[(LineNum * y + x) * 6 + 2] = vEnd.z;

			radiusx = H3DMath::Sin((float)(y+1)/LineNum*H3DMath::PI)*radius ;
			vEnd.Set( H3DMath::Cos((float)x/LineNum*H3DMath::PI*2.0f)*radiusx,H3DMath::Sin((float)x/LineNum*H3DMath::PI*2.0f)*radiusx,H3DMath::Cos((float)(y+1)/LineNum*H3DMath::PI)*radius);
			vEnd += pos;
			pLineList[(LineNum * y + x) * 6 + 3] = vEnd.x;
			pLineList[(LineNum * y + x) * 6 + 4] = vEnd.y;
			pLineList[(LineNum * y + x) * 6 + 5] = vEnd.z;
		}
	}

	int offset = LineNum*LineNum*6;
	for(int y = 1; y < LineNum; ++y)
	{
		for(int x = 0; x < LineNum; ++x)
		{	
			float radiusx = H3DMath::Sin((float)y/LineNum*H3DMath::PI)*radius ;
			H3DVec3 vEnd( H3DMath::Cos((float)x/LineNum*H3DMath::PI*2.0f)*radiusx,H3DMath::Sin((float)x/LineNum*H3DMath::PI*2.0f)*radiusx,H3DMath::Cos((float)y/LineNum*H3DMath::PI)*radius);
			vEnd += pos;
			pLineList[offset+(LineNum * (y-1) + x) * 6] = vEnd.x;
			pLineList[offset+(LineNum * (y-1)  + x) * 6 + 1] = vEnd.y;
			pLineList[offset+(LineNum * (y-1)  + x) * 6 + 2] = vEnd.z;


			vEnd.Set( H3DMath::Cos((float)(x+1.0f)/LineNum*H3DMath::PI*2.0f)*radiusx,H3DMath::Sin((float)(x+1.0f)/LineNum*H3DMath::PI*2.0f)*radiusx,H3DMath::Cos((float)y/LineNum*H3DMath::PI)*radius);
			vEnd += pos;
			pLineList[offset+(LineNum * (y-1)  + x) * 6 + 3] = vEnd.x;
			pLineList[offset+(LineNum * (y-1)  + x) * 6 + 4] = vEnd.y;
			pLineList[offset+(LineNum * (y-1)  + x) * 6 + 5] = vEnd.z;
		}
	}
	draw_line(pLineList,LineNum * LineNum + LineNum * (LineNum - 1),color,irender);
	delete [] pLineList;
}

void CRenderSystem::draw_line( float* points, int LightNum,float *color, H3DI::IRender* irender )
{
#ifndef _EDITOR
	assert(false);    //TODO by 张首钊 需要给出画线需要的.mat文件
#endif
	irender->OpenShaderLib(m_shader_ui_lib_path.c_str());
	
	if (m_draw_shader == NULL)
		m_draw_shader = irender->GetNewShader("../data/universal/ui/Coord2.mat", "Coord2");

	if(!m_draw_shader)
	{
		return;
	}


	H3DI::IVB *vb = irender->GetDynamicVB();
	if(!vb)
	{
		return;
	}
	irender->SetNewShader(m_draw_shader);

	float *pVertex = (float *)vb->Lock(H3DI::VB_VERTEX_ARRAY, H3DI::VB_POS | H3DI::VB_COLOR, 28,LightNum*2);
	int vpos = 0;
	for (int i = 0;i < LightNum*2;i++)
	{
		pVertex[vpos++] = points[i*3];
		pVertex[vpos++] = points[i*3+1];
		pVertex[vpos++] = points[i*3+2];
		pVertex[vpos++]= color[0];
		pVertex[vpos++]= color[1];
		pVertex[vpos++]= color[2];
		pVertex[vpos++]= color[3];
	}

	vb->UnLock(H3DI::VB_VERTEX_ARRAY);

	unsigned short *pIndex = (unsigned short*)vb->Lock(H3DI::VB_ELEMENT_ARRAY, - 1, - 1,LightNum*2);

	for (int i=0; i<LightNum*2; i++)
	{
		pIndex[i] = i;
	}
	vb->UnLock( H3DI::VB_ELEMENT_ARRAY );
	irender->DrawIndexNew(H3DI::LINE_LIST,LightNum*2, LightNum*2, 0, 0, 0, 0);

}


void CRenderSystem::set_line_shader_ui_lib_path(const char* path)
{
	m_shader_ui_lib_path = path;
}

void CRenderSystem::draw_point(const H3DVec3& pos, float radius, const H3DColor& color)
{
	H3DI::IRender* irender = m_render;
	float points[96] = {0.0f};
	int indexs[180] = {0.0f};

	//计算顶点
	float da = (float)H3DMath::PI / 5;
	float cos_da = (float)cos(da);
	float sin_da = (float)sin(da);
	float dx1 = radius * (float)sin(H3DMath::PI / 3);
	float dy1 = 0;
	float dx2 = radius;
	float dy2 = 0;
	float z1 = pos.z + radius / 2;
	float z2 = pos.z - radius / 2;

	points[0] = pos.x;
	points[1] = pos.y;
	points[2] = pos.z + radius;
	points[93] = pos.x;
	points[94] = pos.y;
	points[95] = pos.z - radius;
	for (int i = 0; i < 10; ++i)
	{
		int index = 3 * i;

		points[index + 3] = dx1 * cos_da - dy1 * sin_da;
		points[index + 4] = dx1 * sin_da + dy1 * cos_da;
		dx1 = points[index + 3];
		dy1 = points[index + 4];
		points[index + 3] += pos.x;
		points[index + 4] += pos.y;
		points[index + 5] = z1;

		points[index + 33] = dx2 * cos_da - dy2 * sin_da;
		points[index + 34] = dx2 * sin_da + dy2 * cos_da;
		dx2 = points[index + 33];
		dy2 = points[index + 34];
		points[index + 33] += pos.x;
		points[index + 34] += pos.y;
		points[index + 35] = pos.z;

		points[index + 63] = points[index + 3];
		points[index + 64] = points[index + 4];
		points[index + 65] = z2;
	}

	//计算索引点
	for (int i = 0; i < 10; ++i)
	{
		int index = 3 * i;
		// 三角形索引
		indexs[index] = 0;
		indexs[index + 1] = (i + 1) % 10 + 1;
		indexs[index + 2] = i + 1;

		indexs[index + 30] = indexs[index + 2];
		indexs[index + 31] = indexs[index + 1];
		indexs[index + 32] = indexs[index + 31] + 10;
		indexs[index + 60] = indexs[index + 30];
		indexs[index + 61] = indexs[index + 32];
		indexs[index + 62] = indexs[index + 30] + 10;

		indexs[index + 90] = indexs[index + 62];
		indexs[index + 91] = indexs[index + 61];
		indexs[index + 92] = indexs[index + 91] + 10;
		indexs[index + 120] = indexs[index + 90];
		indexs[index + 121] = indexs[index + 92];
		indexs[index + 122] = indexs[index + 90] + 10;

		indexs[index + 150] = indexs[index + 122];
		indexs[index + 151] = indexs[index + 121];
		indexs[index + 152] = 31;
	}

	draw_index(H3DI::LINE_LIST, points, 32, indexs, 180, color);
}

void CRenderSystem::draw_index(H3DI::PRIMITIVE_TYPE type, float* points, int point_count, int* indexs, int index_count, const H3DColor& color)
{
#ifndef _EDITOR
    assert(false);    //TODO by 张首钊 2011.08.19 需要画线的.mat文件
#endif

	H3DI::IRender* irender = m_render;

	if (m_draw_shader == NULL)
		m_draw_shader = irender->GetNewShader("../data/universal/ui/Coord2.mat", "Coord2");

	if(!m_draw_shader)
	{
		return;
	}

	H3DI::IVB *vb = irender->GetDynamicVB();
	if(!vb)
	{
		return;
	}
	irender->SetNewShader(m_draw_shader);

	float *pVertex = (float *)vb->Lock(H3DI::VB_VERTEX_ARRAY, H3DI::VB_POS | H3DI::VB_COLOR, 28,point_count);
	int vpos = 0;
	for (int i = 0;i < point_count;i++)
	{
		pVertex[vpos++] = points[i*3];
		pVertex[vpos++] = points[i*3+1];
		pVertex[vpos++] = points[i*3+2];
		pVertex[vpos++]= color.getColor()[0];
		pVertex[vpos++]= color.getColor()[1];
		pVertex[vpos++]= color.getColor()[2];
		pVertex[vpos++]= color.getColor()[3];
	}

	vb->UnLock(H3DI::VB_VERTEX_ARRAY);

	unsigned short *pIndex = (unsigned short*)vb->Lock(H3DI::VB_ELEMENT_ARRAY, - 1, - 1,index_count);

	for (int i=0; i< index_count; i++)
	{
		pIndex[i] = indexs[i];
	}
	vb->UnLock( H3DI::VB_ELEMENT_ARRAY );

	irender->DrawIndexNew(type, index_count, point_count, 0, 0, 0, 0);
}

void CRenderSystem::draw_light_start_point(const H3DVec3& beg_pos, const H3DVec3& end_pos, const H3DColor& color)
{
	H3DI::IRender* irender = m_render;
	H3DVec3 dir = end_pos - beg_pos;
	float radius = 0.5;
	dir = dir / dir.Length();

	H3DVec3 normal = dir;
	H3DVec3 center = beg_pos;
	float length = 2 * radius;
	H3DVec3 center2 = get_point_disN_to_point(center, normal, length);

	int pointcount = 360;
	float points1[1080] = {0};
	int indexs1[361] = {0};
	float points2 [1080] = {0};
	int indexs2[361] = {0};
	float points3[24] = {0};
	int i = 0;
	for (int t = 0; t < pointcount; t++)
	{
		float factor = 0;
		H3DVec3 u;
		if (abs(normal.x) > abs(normal.y))
		{
			factor = 1.0f / (float)sqrt(normal.x * normal.x + normal.z * normal.z);
			u.x = -normal.z * factor;
			u.y = 0;
			u.z = normal.x * factor;
		}
		else
		{
			factor = 1.0f / (float)sqrt(normal.y * normal.y + normal.z * normal.z);
			u.x = 0;
			u.y = normal.z * factor;
			u.z = -normal.y * factor;
		}

		u.Normalize();
		H3DVec3 v = normal.Cross(u);
		v.Normalize();

		points1[t * 3] = (float)(center.x + (radius * cos(H3DMath::PI * t / 180.0f)) * u.x + (radius * sin(H3DMath::PI * t / 180.0f)) * v.x);
		points1[t * 3 + 1] = (float)(center.y + (radius * cos(H3DMath::PI * t / 180.0f)) * u.y + (radius * sin(H3DMath::PI * t / 180.0f)) * v.y);
		points1[t * 3 + 2] = (float)(center.z + (radius * cos(H3DMath::PI * t / 180.0f)) * u.z + (radius * sin(H3DMath::PI * t / 180.0f)) * v.z);

		points2[t * 3] = (float)(center2.x + (radius * cos(H3DMath::PI * t / 180.0f)) * u.x + (radius * sin(H3DMath::PI * t / 180.0f)) * v.x);
		points2[t * 3 + 1] = (float)(center2.y + (radius * cos(H3DMath::PI * t / 180.0f)) * u.y + (radius * sin(H3DMath::PI * t / 180.0f)) * v.y);
		points2[t * 3 + 2] = (float)(center2.z + (radius * cos(H3DMath::PI * t / 180.0f)) * u.z + (radius * sin(H3DMath::PI * t / 180.0f)) * v.z);


		indexs1[t] = t;
		indexs2[t] = t;

		if (t % 90 == 0)
		{
			points3[i * 3] = points1[t * 3];
			points3[i * 3 + 1] = points1[t * 3 + 1];
			points3[i * 3 + 2] = points1[t * 3 + 2];

			points3[12 + i * 3] = points2[t * 3];
			points3[12 + i * 3 + 1] = points2[t * 3 + 1];
			points3[12 + i * 3 + 2] = points2[t * 3 + 2];

			i++;
		}
	}
	indexs1[pointcount] = 0;
	indexs2[pointcount] = 0;
	int indexs3[8] = { 0, 4, 1, 5, 2, 6, 3, 7 };
	draw_index(H3DI::LINE_STRIP, points1, pointcount, indexs1, pointcount + 1,color);
	draw_index(H3DI::LINE_STRIP, points2, pointcount, indexs2, pointcount + 1, color);
	draw_index(H3DI::LINE_LIST, points3, 8, indexs3, 8, color);
}

void CRenderSystem::draw_light_end_point(const H3DVec3& beg_pos, const H3DVec3& end_pos, const H3DColor& color)
{
	H3DI::IRender* irender = m_render;
	H3DVec3 dir = end_pos - beg_pos;
	float radius = 0.25;
	dir = dir / dir.Length();

	H3DVec3 center = end_pos;
	H3DVec3 normal = dir * -1;
	float length = radius * 2;

	float points[24] = {0.0f};

	radius = radius * (sqrtf(2));

	H3DVec3 center2 = get_point_disN_to_point(center, normal, length);
	if (normal.Length() != 0)
	{
		normal.Normalize();
	}

	for (int t = 0; t < 4; t++)
	{
		float factor = 0;
		H3DVec3 u;
		if (abs(normal.x) > abs(normal.y))
		{
			factor = 1.0f / (float)sqrt(normal.x * normal.x + normal.z * normal.z);
			u.x = -normal.z * factor;
			u.y = 0;
			u.z = normal.x * factor;
		}
		else
		{
			factor = 1.0f / (float)sqrt(normal.y * normal.y + normal.z * normal.z);
			u.x = 0;
			u.y = normal.z * factor;
			u.z = -normal.y * factor;
		}
		u.Normalize();
		H3DVec3 v = normal.Cross(u);
		v.Normalize();

		points[t * 3] = (float)(center.x + (radius * cos(H3DMath::PI * (t * 90 + 45) / 180.0f)) * u.x + (radius * sin(H3DMath::PI * (t * 90 + 45) / 180.0f)) * v.x);
		points[t * 3 + 1] = (float)(center.y + (radius * cos(H3DMath::PI * (t * 90 + 45) / 180.0f)) * u.y + (radius * sin(H3DMath::PI * (t * 90 + 45) / 180.0f)) * v.y);
		points[t * 3 + 2] = (float)(center.z + (radius * cos(H3DMath::PI * (t * 90 + 45) / 180.0f)) * u.z + (radius * sin(H3DMath::PI * (t * 90 + 45) / 180.0f)) * v.z);

		points[12 + t * 3] = (float)(center2.x + (radius * cos(H3DMath::PI * (t * 90 + 45) / 180.0f)) * u.x + (radius * sin(H3DMath::PI * (t * 90 + 45) / 180.0f)) * v.x);
		points[12 + t * 3 + 1] = (float)(center2.y + (radius * cos(H3DMath::PI * (t * 90 + 45) / 180.0f)) * u.y + (radius * sin(H3DMath::PI * (t * 90 + 45) / 180.0f)) * v.y);
		points[12 + t * 3 + 2] = (float)(center2.z + (radius * cos(H3DMath::PI * (t * 90 + 45) / 180.0f)) * u.z + (radius * sin(H3DMath::PI * (t * 90 + 45) / 180.0f)) * v.z);

	}

	int indexs[36] = { 0, 1, 2, 0, 2, 3, 4, 5, 6, 4, 6, 7, 0, 1, 5, 0, 5, 4, 1, 2, 6, 1, 6, 5, 2, 3, 7, 2, 7, 6, 3, 0, 4, 3, 4, 7 };

	draw_index(H3DI::TRIANGLE_LIST, points, 8, indexs, 36, color);
}

void CRenderSystem::draw_link_line(const H3DVec3& beg_pos, const H3DVec3& end_pos, const H3DColor& color)
{
	H3DI::IRender* irender = m_render;
	float point[6] = {beg_pos.x, beg_pos.y, beg_pos.z, end_pos.x, end_pos.y, end_pos.z};
	int index[2] = {0, 1};
	draw_index(H3DI::LINE_LIST, point, 2, index, 2, color);
}

H3DVec3 CRenderSystem::get_point_disN_to_point(const H3DVec3& start, const H3DVec3& diretion, float dis)
{
	float x = 0, y = 0, z = 0;
	int index = 0;
	float m = 0, n = 0;
	float tmp = 0;
	H3DVec3 dir = diretion;
	if (dis == 0)
	{
		return H3DVec3(start);
	}
	else if (dis < 0)
	{
		dis = -dis;
		dir = dir * -1;
	}
	if (dir.z != 0)
	{
		m = dir.z;
		n = start.z;
		index = 1;
	}
	else if (dir.x != 0)
	{
		m = dir.x;
		n = start.x;
		index = 2;
	}
	else if (dir.y != 0)
	{
		m = dir.y;
		n = start.y;
		index = 3;
	}
	else
	{
		return H3DVec3();
	}

	tmp = dis * dis * m * m;
	tmp /= (dir.x * dir.x + dir.y * dir.y + dir.z * dir.z);
	tmp = (float)sqrt(tmp);
	if (tmp * m < 0)
	{
		tmp = -tmp;
	}
	tmp += n;

	if (index == 1)
	{
		z = tmp;
		if (dir.x != 0)
		{
			x = dir.x * (z - start.z) / dir.z + start.x;
		}
		else
		{
			x = start.x;
		}
		if (dir.y != 0)
		{
			y = dir.y * (z - start.z) / dir.z + start.y;
		}
		else
		{
			y = start.y;
		}
	}
	if (index == 2)
	{
		x = tmp;
		if (dir.y != 0)
		{
			y = dir.y * (x - start.x) / dir.x + start.y;
		}
		else
		{
			y = start.y;
		}
		if (dir.z != 0)
		{
			z = dir.z * (x - start.x) / dir.x + start.z;
		}
		else
		{
			z = start.z;
		}
	}
	if (index == 3)
	{
		y = tmp;
		if (dir.x != 0)
		{
			x = dir.x * (y - start.y) / dir.y + start.x;
		}
		else
		{
			x = start.x;
		}
		if (dir.z != 0)
		{
			z = dir.z * (y - start.y) / dir.y + start.z;
		}
		else
		{
			z = start.z;
		}
	}
	return H3DVec3(x, y, z);
}

void CRenderSystem::log(const char* szString, ...)
{
	static char szParsedString[260];

	//参数字符串
	va_list va;
	va_start( va, szString );
	vsprintf_s( szParsedString, szString, va );
	va_end( va );

	m_render->OutPutLogInfo(H3DI::MODEL_INFO, H3DI::OutPut_File, szParsedString);
}