#include "../HippoTestManager/HippoTestManager.h"
#include "../CommonCode/HippoUtil/HippoScene.h"
#include "../CommonCode/HippoUtil/RefCountCompare.h"
#include "../CommonCode/HippoUtil/HippoGlobal.h"
#include <iostream>
#include "idMath/dMathHeader.h"
#include "engine_interface.h"
#include "../CommonCode/HippoUtil/ActorUtil.h"

using namespace std;

/*
测试逻辑：
异步加载贴图，被cancel，应该能够正确释放
*/
class TextureLoadTest : public HippoTestCaseBase
{
public:
	HippoLevelScene* m_pScene;

	H3DI::ITexture* m_pTexture;

	unsigned int m_TaskId;

	bool m_bActorSex;

	TextureLoadTest(const char* case_name) : HippoTestCaseBase(case_name){}

	virtual bool InitTestCase()
	{
		m_pScene=Hippo_GetSceneManager()->CreateEmptyLevel();

		//create actor
		//Hippo_GetIRender()->OpenActionLib("../resources/art/role/actions/role.xml");
		//ActorUtil::InitActorDress("../resources/config/shared/item/dress.xml");
		//ActorUtil::InitPetDress("../resources/config/shared/item/petdress.xml");
		m_bActorSex=false;
	
		m_pScene->RestructScene();

		//m_TaskId = Hippo_GetIRender()->CreateTextureFromFileAsyn("../data/universal/asset/default.dds");
		m_TaskId = Hippo_GetIRender()->CreateTextureFromFileAsyn("../resources/stage/ceshi_L/lightmap/_SimpleLightMap_20.dds", 0);
		

		m_state=CASE_EXECING;
		return true;
	}

	virtual bool CleanUpTestCase()
	{
		return true;
	}

	//
	virtual void InitScene()
	{
	}

	void CleanUpScene()
	{
		m_pScene->CleanScene();
		Hippo_GetSceneManager()->DelScene(m_pScene);
	}

	virtual bool AutoRunExec()
	{
		//if(Spin(10000))
		//{
		//	m_state=CASE_END_EXECING;
		//	return true;
		//}
		return true;
	}


	virtual bool ManualRunExec()
	{
		return false;
	}


	virtual void BeforeRender()
	{
		Hippo_GetIRender()->UpdateCpuSkin();
		Hippo_GetIRender()->ForceSyncData();
		Hippo_GetIRender()->UpdatePhx();

		Hippo_GetIRender()->UpdateEngineThread();
		int state = Hippo_GetIRender()->QueryTaskState(m_TaskId);

		if (state == H3DI::TASK_OK)
		{
			Hippo_GetIRender()->Exec(99, "a", 0, NULL, 0);

			// cancel task
			Hippo_GetIRender()->CancelTask(m_TaskId);

			// task被释放，引用计数减到1
			Hippo_GetIRender()->UpdateEngineThread();
			Hippo_GetIRender()->Exec(99, "b", 0, NULL, 0);

			// 贴图被释放
			Hippo_GetIRender()->ClearMaterialLib();
			Hippo_GetIRender()->Exec(99, "c", 0, NULL, 0);
			
		}
	}


	virtual int OnKeyDown(unsigned int key)
	{
		switch( key )
		{
		case VK_RETURN:
			{
				//m_state=CASE_END_EXECING;
				return 1;
			}
		case VK_LEFT:
			{
				
				return 1;
			}
		case VK_RIGHT:
			{
				
				return 1;
			}
		case VK_DOWN:
			{
				m_pScene->CleanScene();
			}
		}
		return 0;
	}

	//!自定义的键盘消息处理函数，返回0表示对该消息不感兴趣，返回1表示已经处理，不需要默认消息处理函数再处理
	virtual int ProcessInput(unsigned int nChar, bool bKeyDown, bool bAltDown,bool bCrtlDown,bool bShiftDown, void* pUserContext )
	{
		return 0;
	}

private:
	static HippoTestCaseBase* const testcase_;
};

ADD_TESTCASE("TextureLoadTest",TextureLoadTest)

