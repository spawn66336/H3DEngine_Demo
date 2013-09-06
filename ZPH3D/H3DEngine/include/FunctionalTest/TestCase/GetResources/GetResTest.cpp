#include "../../HippoTestManager/HippoTestManager.h"
#include "../CommonCode/HippoUtil/ActorUtil.h"
#include "../CommonCode/HippoUtil/HippoScene.h"
#include "../CommonCode/HippoUtil/HippoGlobal.h"
#include "../CommonCode/HippoUtil/RefCountCompare.h"
#include "engine_interface.h"
#include "engine_interface_internal.h"
#include "../CommonCode/HippoUtil/HippoStateMachine.h"
#include "EffectCore_dx.h"
#include "../idMath/dMathHeader.h"
#include <algorithm>
#include <stdio.h>
#include "GetResInterface.h"
using namespace  std;

class GetResTest: public HippoTestCaseBase
{
public:
	GetResTest(const char* casename):HippoTestCaseBase(casename){}
	HippoLevelScene* m_pScene;
	H3DI::ISkeletonModel* m_pChr;
	Hippo_StateMachine stateMachine;
	bool bAutoEnd;

private:
	static HippoTestCaseBase* const testcase_;
public:
	 
	int GetMatLod2(){return 2;}
	virtual void CreateChr()
	{
	    //bool flag =  ResUtil::GetResFromXml(); //这个函数最终要放在程序初始化处
		//const char* path=ResUtil::GetHippoTestResource_Path("Reload测试用例资源/烘焙贴图测试/114001001/114001_M.chr");
		//m_pChr = m_pScene->CreateChr(path,GetMatLod2());
	}

	
	virtual void InitScene()
	{
		bAutoEnd = false;
		m_pScene = Hippo_GetSceneManager()->CreateEmptyLevel();
		Hippo_GetIRender()->OpenActionLib("../resources/art/role/actions/role.xml");

		CreateChr();
	}
	virtual void CleanUpScene()
	{
		m_pScene->CleanScene();
		Hippo_GetSceneManager()->DelScene(m_pScene);
	}

	virtual bool ManualRunExec()
	{ 
		return false;
	}
	virtual bool AutoRunExec()
	{
		if (bAutoEnd)
		{
			return true;
		}
		return ManualRunExec();
	}

	virtual int OnKeyDown(unsigned int key)
	{
		if (HippoTestManager::GetInstance()->GetCurrentRunMode() == MANUAL_RUN)
		{
			switch (key)
			{
			case VK_RETURN:
				m_state = CASE_END_EXECING;
				return 0;
			}
		}
		return -1;
	}
};
ADD_TESTCASE("RELOAD_TESTCASE",GetResTest);