#include "../HippoTestManager/HippoTestManager.h"
#include "../CommonCode/HippoUtil/HippoScene.h"
#include "../CommonCode/HippoUtil/RefCountCompare.h"
#include "../CommonCode/HippoUtil/HippoGlobal.h"
#include "../CommonCode/HippoUtil/ActorUtil.h"
#include "engine_interface.h"
#include "EffectCore_dx.h"
#include "../CommonCode/HippoUtil/HippoStateMachine.h"
#include <iostream>

using namespace std;

/*
测试逻辑：
如果以下操作，引擎会崩溃：
1、creatert
2、active rt
3、frame begin
4、deactive rt

如果在2，3之间发生了设备丢失，那么会导致引擎内的ScreenDepth Rt野指针。
*/

class RTtest:public HippoTestCaseBase
{
public:
	HippoLevelScene* m_pLevelScene;
	H3DI::IRenderTarget* pRT;

	RTtest(const char* case_name) : HippoTestCaseBase(case_name)
	{
		pRT=0;
	}

	//
	virtual void InitScene()
	{
		Hippo_GetIRender()->OpenActionLib("../resources/art/role/actions/role.xml");
		m_pLevelScene=Hippo_GetSceneManager()->CreateEmptyLevel();
		unsigned int w=512;
		unsigned int h=512;
		pRT=Hippo_GetIRender()->CreateRenderTarget(w,h,H3DI::FMT_RGBA8,H3DI::RT_COLOR);
	}




	void CleanUpScene()
	{
		pRT->Deactivate();
		pRT->Release();
		int w;int h;
		Hippo_GetCurrentWndInfo(w,h);
		Hippo_GetIRender()->ResizeWindowForEditor(w,h);

		m_pLevelScene->CleanScene();
		Hippo_GetSceneManager()->DelScene(m_pLevelScene);

	}
	void deactive_wrapper()
	{
		pRT->Deactivate();
	}
	void AfterRender()
	{
		__try
		{
			deactive_wrapper();
		}
		__except(1)
		{
			HIPPO_EXPECT_EQ(1,0);
			Hippo_WriteConsole(CC_RED,"deactive RT 过程发生异常");
		}


	}

	bool ManualRunExec()
	{
		pRT->Activate(0);
		if(Spin_N_Frame(250))
		{
			//::MessageBoxA(0,0,0,0);
			Hippo_GetIRender()->ResizeWindowForEditor(1440,900);
		}
		

		return true;
	}

	virtual bool AutoRunExec()
	{
		if(Spin_N_Frame(150)) //什么都不做，10s钟后切换到下一个状态
		{
			m_state=CASE_END_EXECING;
			return true;
		}
		return false;
	}

	virtual int  OnKeyDown(unsigned int key)
	{

		if (HippoTestManager::GetInstance()->GetCurrentRunMode())
		{
			switch(key)
			{
			case  VK_RETURN:
				m_state = CASE_END_EXECING;
				return 0;
			}
		}
		return -1;
	}

private:
	static HippoTestCaseBase* const testcase_;
};



ADD_TESTCASE("RTtest",RTtest)