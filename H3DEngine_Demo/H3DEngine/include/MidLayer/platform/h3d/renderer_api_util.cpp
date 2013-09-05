/*********************************************************************
Created:	2003/11/24
File name:	UIWndManager.cpp
Author:		maple

Copyright (C) 2003 - 2005 H3D, All Rights Reserved.

Purpose:	
*********************************************************************/
#define __TFILE__ _T(__FILE__)

#include <client/sys_related.h>

#include "renderer_api_util.h"

#include <client/i_log.h>
#include <client/global_share.h>
#include <client/i_input_mng.h>
#include <client/i_config.h>

#include <client_utilities/string_util.h>

#include <third_party/tinyxml/tinyxml.h>

/*
t 最终的面向用户配置

现在客户端的工程中有一个新的工程设置叫做final_release。已经把这个设置加在了需要的工程中。以后发布给用户需要使用这个设置。
所做的修改有（与release的不同点）：
	没有了stlport的动态检测
	日志只出现在最高级，其他的会被优化掉
	某些调试用功能无法使用，比如看读取时间以及资源使用
	某些机器人不能再被使用（压力机器人和UI机器人）
	某些配置命令不能在被使用，具体如下（都是调试信息）
		ShowDebugInfo
		NoAction
		LoadEff
		ShowConWin
		DoAntiTest
		NotEnterGame
		EnterSpan
		FastTimer
		MusicTimer
		MusicTimerBlend
		InputThread
		RenderHitDebug
		UseDummyInput
		renderscene
		renderui
		CareOnlyScore
		TeachingTest
		ForceSetThread
		ActorThread
		PetFunction
		NotSetAction
		TestDanceOff
		AutoFill
		sendraw
		AssertOnCheat
		LogID
		LogIDRear
		ShowActionInfo
		ShowDummyAABB
		ShowDummy
		TestDancePos
		DumpActionInfo
		TinyMode
		renderactor
		DummyGameNet
		RandSeed
		PressureTest
		TyroHelp
		TryDump
		TuningTime
		DumpLevelInfo
		TestSpan
		PressureTestCount
		ProfileSpan
		ConfigReloadSpan
		DoSwap
		ShowActiveShader
		SpanToRemoveFrameBig

final release与release在面向用户的逻辑功能上差异可以忽略。

个人建议的开发方式是：在逻辑开发阶段使用release进行测试，在接近发布以及发布阶段以final release进行测试。
另外，客户端相关的所有工程的设置必须由客户端进行修改，其他人不能修改。

如有问题，请写出意见，最后开会讨论。没有人写，就认为没有意见。

*/






BEGIN_H3D_CLIENT

void safe_check_assign_boundary(void* pos, void* beg, void* end)
{
	clAssert(pos >= beg && pos < end);
}

void safe_check_memcpy_boundary(void* des, void* org, int countinchar, void* desbeg, void* desend, void* orgbeg, void* orgend)
{
	clAssert(des >= desbeg && (void*)((char*)des + countinchar) <= desend);
	clAssert(org >= orgbeg && (void*)((char*)org + countinchar) <= orgend);
}

#ifndef _FINAL_RELEASE

void MemCheckChain::check(float k, int count)
{
	return;

	clLog(_T("check load xml\n"));

	//TiXmlDocument xml("../resources/level/dance.xml");
	//if(!xml.LoadFile())
	//		return;

	clLog(_T("after check load xml\n"));

	k *= 3;

	for(int i = 0; i < count; ++i)
	{
		MemCheck* mc = new MemCheck(k);
		m_check_buf.push_back(mc);

		ensureKeeps(m_keeps);
	}
}

#endif

void faild(const _tchar* content)
{
	if (content)
	{
		clLog(_T("faild content(%s)\n"), content);
	}
	clAssert(0);
}
H3DI::IRender* HWR::s_renderer;

int HWR::s_znear;
int HWR::s_zfar;


#ifdef RENDERCORE_NEW
bool HWR::s_is_dx9 = true;
#endif

PBufferShoot_Util::PBufferShoot_Util(H3DI::IRender* renderer, I_PBuffer* pbuffer)
{
	m_renderer = renderer;
	m_pbuffer = pbuffer;
}

void PBufferShoot_Util::setupEnv(float clearc[])
{
	alwaysAssert(0); // 不再使用
}
// 恢复原来的绘制目标
void PBufferShoot_Util::clearEnv()
{
	alwaysAssert(0); // 不再使用
}
// 设置3D投影相机
void PBufferShoot_Util::setCamera(float fovy, float aspect, 
								  float n, float f, 
								  float eyepos[], float eyedes[], float eyeup[], unsigned short vp[])
{
	m_renderer->SetViewport(vp);

	HWR::set_depthtest(true);
	HWR::set_texture2d(true);
	HWR::load_projection_identity();

	H3DVec3 ep(eyepos[0], eyepos[1], eyepos[2]);
	H3DVec3 ed(eyedes[0], eyedes[1], eyedes[2]);
	H3DVec3 eu(eyeup[0], eyeup[1], eyeup[2]);

	// m_renderer->PushCamera();
	m_renderer->LookAt(ep, ed, eu);  // renderer->SetViewMatrix(m_mat);
	m_renderer->SetFrustum(fovy, aspect, n, f);
	m_renderer->UpdateCamera();
}
// 设置平行投影相机
void PBufferShoot_Util::setCamera(float n, float f, unsigned short vp[])
{
	m_renderer->SetViewport(vp);

	HWR::load_projection_identity();
	HWR::set_ortho(vp[0], vp[2], vp[1], vp[3], -2048, 2048);
	HWR::load_viewmatrix_identity();
}
// 取消设置的相机
void PBufferShoot_Util::desetCamera(unsigned short oldvw, unsigned short oldvh)
{
	//	unsigned short vp[] = {0, 0, oldvw, oldvh};
	//	m_renderer->SetViewport(vp); // fixme: a little waste ?

	// m_renderer->PopCamera();
	m_renderer->UpdateCamera();
}
// 使用给定shader绘制
void PBufferShoot_Util::renderMask(int ow, int oh, int mx, int my, int mw, int mh, H3DI::IShader* ms)
{
	HWR::load_projection_identity();
	HWR::set_ortho(0, ow, 0, oh, -2048, 2048);
	HWR::load_viewmatrix_identity();
	m_renderer->SetShader(ms);

	HWR::set_depthtest(false);
	HWR::set_blend(true);

	CLRect rect(mx, my + mh, mx + mw, my);
	H3DI::IVB *vb = m_renderer->GetDynamicVB();

	if(HWR::dx9())
	{
		float *pVertex = (float*)vb->Lock(H3DI::VB_VERTEX_ARRAY, H3DI::VB_POS | H3DI::VB_COLOR | H3DI::VB_UV, 36, 4);
		float alpha = 1.0f;

		*pVertex++ = rect.left;		*pVertex++ = rect.top;		*pVertex++ = 0;
		*pVertex++ = 1; *pVertex++ = 1; *pVertex++ = 1; *pVertex++ = alpha;
		*pVertex++ = 0;			*pVertex++ = 0;

		*pVertex++ = rect.left;		*pVertex++ = rect.bottom;	*pVertex++ = 0;
		*pVertex++ = 1; *pVertex++ = 1; *pVertex++ = 1; *pVertex++ = alpha;
		*pVertex++ = 0;			*pVertex++ = 1;

		*pVertex++ = rect.right;	*pVertex++ = rect.top;		*pVertex++ = 0;
		*pVertex++ = 1; *pVertex++ = 1; *pVertex++ = 1; *pVertex++ = alpha;
		*pVertex++ = 1;			*pVertex++ = 0;		

		*pVertex++ = rect.right;	*pVertex++ = rect.bottom;	*pVertex++ = 0;
		*pVertex++ = 1; *pVertex++ = 1; *pVertex++ = 1; *pVertex++ = alpha;
		*pVertex++ = 1;			*pVertex++ = 1;		
	}
	else
	{
		float *pVertex = (float *)vb->Lock(H3DI::VB_VERTEX_ARRAY, H3DI::VB_POS | H3DI::VB_COLOR | H3DI::VB_UV, 24, 4);

		// a r g b
		unsigned int c = 0xffffffff;

		*pVertex++ = rect.left;		*pVertex++ = rect.top;		*pVertex++ = 0;
		*(unsigned int*)(pVertex)++ = c;
		*pVertex++ = 0;	*pVertex++ = 0;

		*pVertex++ = rect.left;		*pVertex++ = rect.bottom;	*pVertex++ = 0;
		*(unsigned int*)(pVertex)++ = c;
		*pVertex++ = 0;			*pVertex++ = 1;

		*pVertex++ = rect.right;	*pVertex++ = rect.top;		*pVertex++ = 0;
		*(unsigned int*)(pVertex)++ = c;
		*pVertex++ = 1;			*pVertex++ = 0;		

		*pVertex++ = rect.right;	*pVertex++ = rect.bottom;	*pVertex++ = 0;
		*(unsigned int*)(pVertex)++ = c;
		*pVertex++ = 1;			*pVertex++ = 1;		
	}

	vb->UnLock( H3DI::VB_VERTEX_ARRAY );

	unsigned short *pIndex = (unsigned short*)vb->Lock(H3DI::VB_ELEMENT_ARRAY, - 1, - 1, 4);
	*pIndex++ = 0;
	*pIndex++ = 1;
	*pIndex++ = 2;
	*pIndex = 3;
	vb->UnLock(H3DI::VB_ELEMENT_ARRAY);

	m_renderer->DrawIndex(H3DI::TRIANGLE_STRIP, 4, 4, - 1, - 1, - 1, - 1);
}

//ConfigValue::ValueSet* Platform_Info_Log::s_info_retrive = 0;
bool Platform_Info_Log::s_info_retrive = false;

Platform_Info_Log::Platform_Info_Log(const _tchar* name)
{
#ifndef _FINAL_RELEASE
	static bool s_loaded = false;

	if(!s_loaded)
	{
		s_loaded = true;
		//s_info_retrive = g_config->getPtr(_T("PlatformInfoRetrive"), _T("Debug"));
		g_config->Get(s_info_retrive, _T("PlatformInfoRetrive"), _T("Debug"));
	}

	//if(!s_info_retrive || !s_info_retrive->b)
	if(!s_info_retrive)
		return;

	clAssert(name);
	m_name = name;

	const H3DI::IEngineMonitor* em = HWR::s_renderer->GetEngineMonitor();

	m_mem_used = em->GetAllocatedMemory();
	m_video_mem_used = em->GetAllocatedVideoMemory();
#endif
}

Platform_Info_Log::~Platform_Info_Log()
{
#ifndef _FINAL_RELEASE
	//if(!s_info_retrive || !s_info_retrive->b)
	if(!s_info_retrive)
		return;

	const H3DI::IEngineMonitor* em = HWR::s_renderer->GetEngineMonitor();

	int cmu = (int)em->GetAllocatedMemory();
	int cvmu = (int)em->GetAllocatedVideoMemory();

	int mu = cmu - (int)m_mem_used;
	int vmu = cvmu - (int)m_video_mem_used;

	float fmu = (float)mu / 1024 / 1024;
	float fvmu = (float)vmu / 1024 / 1024;

	float fcmu = (float)cmu / 1024 / 1024;
	float fcvmu = (float)cvmu / 1024 / 1024;

	clLog1(_T("mem used:%.3f vmem used:%.3f after %s with mu:%.3f vmu:%.3f \n"), fmu, fvmu, m_name.c_str(), fcmu, fcvmu);
#endif
}

Time_Info_Log::Time_Info_Log(const _tchar* name) : m_name(name)
{
#ifndef _FINAL_RELEASE
	m_org_time = g_timer->GetCurTimeMs();
#endif
}

Time_Info_Log::~Time_Info_Log()
{
#ifndef _FINAL_RELEASE
	I_Timer::MS_TYPE offset = g_timer->GetCurTimeMs() - m_org_time;
	if(offset)
		clLog1(_T("time passed :%i %s \n"), offset, m_name.c_str());
#endif
}

std::list<FrameSpanAction::FrameSpanInfo> FrameSpanAction::s_big_frame_span_info;

static int s_pretime = - 1;
static _tstring s_pre_name;

void FrameSpanAction::resetSpan()
{
	s_pretime = g_timer->GetCurTimeMs();
	s_pre_name.clear();
}

FrameSpanAction::FrameSpanAction(const _tchar* info)
{

	static bool s_show_big_span = false;
	static bool s_first = true;
	
	if(s_first)
	{
		s_first = false;
		g_config->Get(s_show_big_span, _T("ShowBigSpan"), _T("Debug"));
	}
	


	// fixme: this should be a function to add to here
	g_InputDeviceMng->CaptureInput();

#ifndef _FINAL_RELEASE
	const _tchar* tinfo = info;

	if(s_pretime != - 1)
	{
		int pret = s_pretime;
		int curt = g_timer->GetCurTimeMs();
		int offset = curt - s_pretime;
		s_pretime = curt;

		if(offset >= s_span_tiny)
		{
			if(offset >= s_span_big)
			{
#ifdef _BotStage
				clLog(_T("frame span big: %s -> %s %i \n"), s_pre_name.c_str(), info, offset);
#else
	#ifndef _FINAL_RELEASE
				clLog(_T("frame span big: %s -> %s %i \n"), s_pre_name.c_str(), info, offset);
	#else // still a little over head ???
				// clLog(_T("frame span big: %s -> %s %i \n"), s_pre_name.c_str(), info, offset);
	#endif
#endif
				if(s_show_big_span)
				{
					FrameSpanInfo info;
					info.m_beg_t = pret;
					info.m_span = offset;

					toString(info.m_name, 512, _T("%s->%s:%i"), s_pre_name.c_str(), tinfo, offset);

					s_big_frame_span_info.push_front(info);

					const int BUF_MAX_SIZE = 1024;
					if(s_big_frame_span_info.size() >= BUF_MAX_SIZE)
					{
						std::list<FrameSpanInfo>::iterator backitr = s_big_frame_span_info.end();
						for(int i = 0; i < BUF_MAX_SIZE / 2; ++i)
							--backitr;

						s_big_frame_span_info.erase(backitr, s_big_frame_span_info.end());
					}
				}
			}
			else
#ifdef _BotStage
				clLog(_T("%s -> %s %i \n"), s_pre_name.c_str(), info, offset);
#else
	#ifndef _FINAL_RELEASE
			clLog2(_T("%s -> %s %i \n"), s_pre_name.c_str(), info, offset);
	#else // still a little over head ???
			;	
	#endif
#endif
		}
	}
	else // do init
	{
		s_pretime = g_timer->GetCurTimeMs();

		s_span_tiny = 5;
		s_span_big = 30;

		g_config->Get(s_span_tiny, _T("FrameSpanTiny"), _T("Debug"));
		g_config->Get(s_span_big, _T("FrameSpanBig"), _T("Debug"));
	}

	s_pre_name = info;

#endif
}

int FrameSpanAction::s_span_tiny;
int FrameSpanAction::s_span_big;

TimeSpanCheck::TimeSpanCheck(const _tchar* str)
{
#ifndef _FINAL_RELEASE
	clAssert(str);
	m_info = str;
	m_beg_t = g_timer->GetCurTimeMs();
#endif
}

void TimeSpanCheck::doCheck(int span)
{
#ifndef _FINAL_RELEASE

	I_Timer::MS_TYPE ct = g_timer->GetCurTimeMs();
	if(ct - m_beg_t > span)
	{
		clLog2(_T("span check %s -> %i bigger:%i \n"), m_info.c_str(), ct - m_beg_t, ct - m_beg_t - span);
	}
#endif
}


/* 
t 帧消耗时间的统计方法(非fanal release版本有效)
FrameSpanAction的构造函数会将本次构造的时间和上次的构造时间进行对比，如果时间超过了FrameSpanTiny，则进行日志输出，如果时间超过了FrameSpanBig除了日志输出外还要记录在对应信息buffer中，供屏幕显示。
使用clFrameSpan宏来进行消耗时间的检测，和直接调用FrameSpanAction的构造函数的效果一样。
如果设置了ShowBigSpan，屏幕上会显示超过了FrameSpanBig的内容．消失时间可以通过SpanToRemoveFrameBig来控制（单位毫秒）。
对于已经知道的必然超出FrameSpanBig的内容，可以在下一个FrameSpanAction的构造函数调用前，运行FrameSpanAction的静态函数resetSpan，这样可以不显示上一个FrameSpanAction到resetSpan之间的所花的时间记录。
比如绘制调试信息DrawDebugInfo需要花很长时间，那么下面的代码必然导致时间过长的报告：
clFrameSpan(dostuff A)
DrawDebugInfo
clFrameSpan(dostuff B)
如果DrawDebugInfo在发布时不需要关心消耗时间，那么我们可以调用resetSpan，这样就不会针对DrawDebugInfo进行检测了：
clFrameSpan(dostuff A)
DrawDebugInfo
FrameSpanAction::resetSpan();
clFrameSpan(dostuff B)
FrameSpanAction只有在star client中才能发挥完全的功能．

t 细微的操作的检测方法
为了更细致的检查问题，需要使用TimeSpanCheck．在某个操作的前面构造一个TimeSpanCheck对象，然后在操作完毕后，调用对象的doCheck来检测操作时间是否操过了给定的最大时间．如果操过最大时间，会将信息输出到日志中．
如为了检测操作Ａ的运行时间是否超过３：
TimeSpanCheck at(_T("check A"));
A();
at.doCheck(3);
这个功能可以在包括了renderer_api_util的各个模块中使用．

t 局内帧稳定开发的方法
１把局内所有消耗时间超过１００ｍｓ的操作完全去掉。
２把局内消耗时间可能操过３０ｍｓ的操作尽量的去掉。
３掌握所有消耗时间操过３毫秒的操作，需要知道为什么会这样。
para DummyGameNet true TinyMode true DummyPCount 6 ShowBigSpan true FrameSpanBig 25 FrameSpanTiny 10 Sleep false

引擎的调试方法
	(0, "MainLoop");
	(1, "CPU : EntityUpdate");
	(2, "CPU : Visibility");
	(3, "CPU : RenderEntities");
	(4, "CPU : LevelCulling");
	(5, "CPU : RenderLevel");
	(6, "GPU : Present");
	(7, "CPU : PipelineSorting");
	(8, "CPU : UseOctreeNodeList");
	(9, "CPU : LevelCullByLight");
	(10, "CPU : ProcessRenderOP");
	(11, "CPU : SceneNodeFrustumCulling");
	(12, "CPU : LevelFrustumCulling");
	(13, "CPU : UseSectionList");
	(14, "CPU : OctreeFrustumCulling");
	(15, "GPU : VertexBufferOps");
	(16, "GPU : ApplyRenderState");
	(17, "GPU : UpdateGPUParam");
	(18, "GPU : BeginEndPass");
	(19, "CPU : Sphere<->AABB");
	(20, "CPU : Sphere<->Frustum");
	(21, "CPU : AABB<->Frustum");
	(22, "Mem : BuildMeshIndices");
	(23, "Mem : ModifyPolygonFlags");
	(24, "CPU : OctreeNodeCullByLight");
	(25, "CPU : DIP");
	(26, "Mem : FillDrawIndexVB");
	(27, "CPU : Skinning");
	(28, "CPU : AmbientScene");
	(29, "CPU : Client");
	(30, "CPU : SetStreamSource");
	(31, "CPU : SetVSPS");
	(32, "CPU : SetRS");
	(33, "CPU : SetStageState");
	(34, "CPU : SetVertexDecl");
	(35, "CPU : SetIndices");
	(36, "CPU : SetSamplerState");
	(37, "CPU : SetTexture");
	(38, "CPU : FrameBeginEnd");
	(39, "GPU : BeginEndScene");
	(40, "CPU : RestoreRenderState");
	(41, "CPU : Set Vertex Decl");
	(42, "CPU : Index Data PreCache");
	(43, "CPU : Phx Update");
	(44, "CPU : Copy Backbuffer");
	(45, "CPU : DownSampling");
	(46, "CPU : Gaussian Blur");
	(47, "CPU : Apply Blur");


*/

END_H3D_CLIENT