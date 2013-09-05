#include "../../HippoTestManager/HippoTestManager.h"
#include "../CommonCode/HippoUtil/ActorUtil.h"
#include "../CommonCode/HippoUtil/HippoScene.h"
#include "../CommonCode/HippoUtil/HippoGlobal.h"
#include "../CommonCode/HippoUtil/RefCountCompare.h"
#include "engine_interface.h"
#include "engine_interface_internal.h"
#include "EffectCore_dx.h"
#include <algorithm>
using namespace  std;

/*测试逻辑
lod0创建1个actor
异步加载一件服装
加载完成后不设置给Actor
执行clearmatlib
记录引用计数快照（s2）
将加载完的服装设置给人物
渲染5s，观察显示效果
记录引用计数快照（s3）
比较两个记录引用计数快照s2，s3，必须要完全一样
释放人物，执行clearmatlib
*/

/*备注
测试用例没有通过
出错点：前后记录引用计数快照，s2,s3 不相等 无法通过断言
可能原因：
*/

class AsynTestCase10 : public HippoTestCase_RefCompare
{
public:
	AsynTestCase10(const char* casename):HippoTestCase_RefCompare(casename){}
	HippoLevelScene* m_pScene;

	H3DI::IActor* m_pActor;
	unsigned int taskId;

	enum ExeState
	{
		INIT,
		//lod0创建1个actor 异步加载一件服装
		CREATE_ACTOR_ASYN_BPT,
		//加载完成后不设置给Actor
		AFTER_LOAD_NO_SET,
		//执行clearmatlib 记录引用计数快照（s2） 将加载完的服装设置给人物
		SNAPSHOT2_SET_BPT,
		SPIN_5S,
		//记录引用计数快照（s3）//比较两个记录引用计数快照s2，s3，必须要完全一样 //释放人物，执行clearmatlib
		SNAPSHOT3_COM_RELEASE,
		TEST_OVER
	};
	ExeState m_exeState;

	HippoResStateSnapShot* m_S2_Mat;
	HippoResStateSnapShot* m_S2_TPL;
	HippoResStateSnapShot* m_S2_Tex;

	HippoResStateSnapShot* m_S3_Mat;
	HippoResStateSnapShot* m_S3_TPL;
	HippoResStateSnapShot* m_S3_Tex;
	

private:
	static HippoTestCaseBase* const testcase_;
public:

	virtual int GetMatLod(){return 0;}

	virtual void InitScene()
	{
		m_pScene = Hippo_GetSceneManager()->CreateEmptyLevel();
		ActorUtil::InitActorDress("../resources/config/shared/item/dress.xml");

		m_exeState =INIT;

	}
	bool IsTaskLoaded(unsigned int task)
	{
		unsigned int  res = Hippo_GetIRender()->QueryTaskState(task);
		if (res == H3DI::TASK_OK || res == H3DI::TASK_ERROR)
		{
			return true;
		}
		return false;
	}

	void NextState()
	{
		int* p = (int*)(&m_exeState);
		++(*p);
	}

	void CreateActorAndLoadBpt()
	{
		//以lod0创建1个actor 异步加载一件服装
		m_pActor = m_pScene->CreateActor(false,GetMatLod());
		H3DI::sCreateOp createOp;
		createOp.mat_lod = GetMatLod();
		createOp.geo_lod = GetMatLod();
		const char* name = "../resources/art/role/bodypart/female/suit/119005001/1193005001/1193005001.bpt";
		taskId =Hippo_GetIRender()->LoadBodypartAsyn(name,H3DI::ACTOR_HUMAN, createOp,0) ;
	}
	void SnapShot2AndSetBpt()
	{
		Hippo_GetIRender()->ClearMaterialLib();
		//记录引用计数快照（s2）
		m_S2_Mat = RefCountCompare::GetResState(HIPPO_MAT_RES);
		m_S2_TPL = RefCountCompare::GetResState(HIPPO_TPL_RES);
		m_S2_Tex = RefCountCompare::GetResState(HIPPO_TEX_RES);
		//将加载完的服装设置给人物
		m_pActor->SetBodyPartAndFinishTask(taskId);
		
	}
	void SnapShot3_Com_Realese()
	{
		//记录引用计数快照（s3）
		
		m_S3_Mat = RefCountCompare::GetResState(HIPPO_MAT_RES);
		m_S3_TPL = RefCountCompare::GetResState(HIPPO_TPL_RES);
		m_S3_Tex = RefCountCompare::GetResState(HIPPO_TEX_RES);
		
		//比较两个记录引用计数快照s2，s3，必须要完全一样
		HIPPO_EXPECT_EQ(RefCountCompare::SnapShotEqual(m_S2_Mat,m_S3_Mat),true);
		HIPPO_EXPECT_EQ(RefCountCompare::SnapShotEqual(m_S2_Tex,m_S3_Tex),true);
		HIPPO_EXPECT_EQ(RefCountCompare::SnapShotEqual(m_S2_TPL,m_S3_TPL),true);

		m_S2_Mat->Release();
		m_S2_TPL->Release();
		m_S2_Tex->Release();
		m_S3_Mat->Release();
		m_S3_TPL->Release();
		m_S3_Tex->Release();
		//释放人物，执行clearmatlib
		m_pScene->DelActor(m_pActor);
		Hippo_GetIRender()->ClearMaterialLib();		
	}
	virtual void CleanUpScene()
	{
		m_pScene->CleanScene();
		Hippo_GetSceneManager()->DelScene(m_pScene);
	}

	virtual bool AutoRunExec()
	{

		if (m_exeState == TEST_OVER)
		{
			return true;
		}
		ManualRunExec();
		return false;
	}

	virtual bool ManualRunExec()
	{
		Hippo_GetIRender()->UpdateEngineThread();
		switch (m_exeState)
		{
		case INIT: 
			NextState();
			break;
		case CREATE_ACTOR_ASYN_BPT: //lod0创建1个actor 异步加载一件服装
			CreateActorAndLoadBpt();
			NextState();
			break;
		case AFTER_LOAD_NO_SET: //加载完成后不设置给Actor
			if (IsTaskLoaded(taskId))
			{
				NextState();
			}
			break;
		case SNAPSHOT2_SET_BPT: //执行clearmatlib 记录引用计数快照（s2） 将加载完的服装设置给人物
			SnapShot2AndSetBpt();
			NextState();
			break;
		case  SPIN_5S:
			if (Spin_N_Ms(5000))
			{
				NextState();
			}
			break;
		case SNAPSHOT3_COM_RELEASE://记录引用计数快照（s3）//比较两个记录引用计数快照s2，s3，必须要完全一样 //释放人物，执行clearmatlib
			SnapShot3_Com_Realese();
			NextState();
			break;
		}
		return false;
	}

	virtual int OnKeyDown(unsigned int key)
	{
		if (HippoTestManager::GetInstance()->GetCurrentRunMode())
		{
			switch(key)
			{
			case   VK_RETURN:
				m_state = CASE_END_EXECING;
				return 1;
			}
		}
		return 0;
	}
};
ADD_TESTCASE("ASYN_LOAD_TEST",AsynTestCase10);


/*
测试逻辑
使用lod1创建人物，其他逻辑和AsynTestCase10相同
*/

class AsynTestCase11 : public AsynTestCase10
{
public:
	AsynTestCase11(const char* casename):AsynTestCase10(casename){}
	virtual int GetMatLod(){return 1;}
private:
	static HippoTestCaseBase* const testcase_;
};
ADD_TESTCASE("ASYN_LOAD_TEST",AsynTestCase11);

/*
测试逻辑
使用lod2创建人物，其他逻辑和AsynTestCase10相同
*/

class AsynTestCase12 : public AsynTestCase10
{
public:
	AsynTestCase12(const char* casename):AsynTestCase10(casename){}
	virtual int GetMatLod(){return 2;}
private:
	static HippoTestCaseBase* const testcase_;
};
ADD_TESTCASE("ASYN_LOAD_TEST",AsynTestCase12);
