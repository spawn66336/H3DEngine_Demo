#include "../../HippoTestManager/HippoTestManager.h"
#include "../CommonCode/HippoUtil/ActorUtil.h"
#include "../CommonCode/HippoUtil/HippoScene.h"
#include "../CommonCode/HippoUtil/HippoGlobal.h"
#include "../CommonCode/HippoUtil/RefCountCompare.h"
#include "engine_interface.h"
#include "engine_interface_internal.h"
#include "../idMath/dMathHeader.h"
#include "EffectCore_dx.h"
#include <algorithm>
using namespace  std;

/*测试逻辑
以lod0创建1个actor0，actor1
给actor0异步加载一件服装（task0）
task0加载完成后不设置给Actor0
执行clearmatlib
记录引用计数快照s2
给actor1同步设置同样的一件服装
将加载完的服装设置给人物actor0
记录引用计数快照s3
比较引用计数快照s2，s3，两者数量应该一样，但是某些条目的引用计数数量会差1
渲染5s，观察显示效果
释放两个人物，执行clearmatlib
*/

/*备注
测试用例没有通过
错误点：在比较引用计数的时候，三种资源信息 前后数量一致 ，资源 ref不一致
可能原因：
*/

class AsynTestCase13 :public HippoTestCase_RefCompare
{
public: 
	AsynTestCase13(const char* casename):HippoTestCase_RefCompare(casename){}

	HippoLevelScene* m_pScene;
	unsigned int task0;

	H3DI::IActor* m_pActor0;
	H3DI::IActor* m_pActor1;

	const char* bptName ;

	enum ExeState
	{
		INIT,
		//以lod0创建1个actor0，actor1 给actor0异步加载一件服装（task0）
		CREATE_ACTOR_ASYN_BPT_TASK0,
		//task0加载完成后不设置给Actor0
		AFTER_LOAD_NO_SET,
		//执行clearmatlib 记录引用计数快照（s2） 给actor1同步设置同样的一件服装 将加载完的服装设置给人物actor0
		SNAPSHOT2_SET_BPT_ACTOR0,
		//记录引用计数快照（s3）//比较两个记录引用计数快照s2，s3，两者数量应该一样，但是某些条目的引用计数数量会差1
		SNAPSHOT3_AND_COM,
		SPIN_5S,
		//释放两个人物，执行clearmatlib
		RELEASE_CLERAMATLIB,
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
	virtual int GetMatlod() {return 0;}
	virtual void InitScene()
	{
		m_pScene = Hippo_GetSceneManager()->CreateEmptyLevel();
		bptName = "../resources/art/role/bodypart/female/suit/119005001/1193005001/1193005001.bpt";

		m_exeState = INIT;
	}
	bool IsTaskLoaded(unsigned int task)
	{
		Hippo_GetIRender()->UpdateEngineThread();
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
		H3DVec3 pos0(-1.5f,0.f,0.f);
		H3DVec3 pos1(1.5f,0.f,0.f);
		//以lod0创建1个actor0，actor1	给actor0异步加载一件服装（task0）
		m_pActor0 = m_pScene->CreateActor(false,GetMatlod());
		m_pActor1 = m_pScene->CreateActor(false,GetMatlod());
		m_pActor0->SetPosition(pos0);
		m_pActor1->SetPosition(pos1);
		H3DI::sCreateOp createop;
		createop.mat_lod = GetMatlod();
		createop.geo_lod = GetMatlod();
		task0 = Hippo_GetIRender()->LoadBodypartAsyn(bptName,H3DI::ACTOR_HUMAN,createop,0);
	}
 
 
	virtual bool ManualRunExec()
	{
		switch (m_exeState)
		{
		case INIT: 
			NextState();
			break;
		case CREATE_ACTOR_ASYN_BPT_TASK0: ///以lod0创建1个actor0，actor1 给actor0异步加载一件服装（task0）
			CreateActorAndLoadBpt();
			NextState();
			break;
		case AFTER_LOAD_NO_SET: //task0加载完成后不设置给Actor0
			if (IsTaskLoaded(task0))
			{
				NextState();
			}
			break;
		case SNAPSHOT2_SET_BPT_ACTOR0: //执行clearmatlib 记录引用计数快照（s2）  给actor1同步设置同样的一件服装 将加载完的服装设置给人物actor0
			Hippo_GetIRender()->ClearMaterialLib();
			//记录引用计数快照（s2）
			m_S2_Mat = RefCountCompare::GetResState(HIPPO_MAT_RES);
			m_S2_TPL = RefCountCompare::GetResState(HIPPO_TPL_RES);
			m_S2_Tex = RefCountCompare::GetResState(HIPPO_TEX_RES);
			m_pActor1->SetBodyPart(bptName);
			m_pActor0->SetBodyPartAndFinishTask(task0);
			NextState();
			break;
		case SNAPSHOT3_AND_COM: //记录引用计数快照（s3）//比较两个记录引用计数快照s2，s3，两者数量应该一样，但是某些条目的引用计数数量会差1
			//记录引用计数快照（s3）
			{
				m_S3_Mat = RefCountCompare::GetResState(HIPPO_MAT_RES);
				m_S3_Tex = RefCountCompare::GetResState(HIPPO_TEX_RES);
				m_S3_TPL = RefCountCompare::GetResState(HIPPO_TPL_RES);

				//比较两个记录引用计数快照s2，s3
				bool bMat = RefCountCompare::SnapShotEqual(m_S2_Mat,m_S3_Mat) || m_S2_Mat->m_res_count-1==m_S3_Mat->m_res_count;
				bool bTex = RefCountCompare::SnapShotEqual(m_S2_Tex,m_S3_Tex) || m_S2_Tex->m_res_count-1==m_S3_Tex->m_res_count;
				bool bTpl = RefCountCompare::SnapShotEqual(m_S2_TPL,m_S3_TPL) || m_S2_TPL->m_res_count-1==m_S3_TPL->m_res_count;
				HIPPO_EXPECT_EQ(bMat,true);
				HIPPO_EXPECT_EQ(bTex,true);
				HIPPO_EXPECT_EQ(bTpl,true);

				m_S2_Mat->Release();
				m_S2_TPL->Release();
				m_S2_Tex->Release();
				m_S3_Mat->Release();
				m_S3_TPL->Release();
				m_S3_Tex->Release();
				NextState();
			}
			  break;
		case  SPIN_5S:
			if (Spin_N_Ms(5000))
			{
				NextState();
			}
			break;
		case RELEASE_CLERAMATLIB://释放两个人物，执行clearmatlib
			m_pScene->DelActor(m_pActor0);
			m_pScene->DelActor(m_pActor1);
			Hippo_GetIRender()->ClearMaterialLib();
			NextState();
			break;
		}
		return false;
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
ADD_TESTCASE("ASYN_LOAD_TEST",AsynTestCase13);


/*
测试逻辑
使用lod1创建人物，其他逻辑和AsynTestCase13相同
*/

class AsynTestCase14 : public AsynTestCase13
{
public:
	AsynTestCase14(const char* casename):AsynTestCase13(casename){}
	virtual int GetMatlod() {return 1;}
private:
	static HippoTestCaseBase* const testcase_;
};
ADD_TESTCASE("ASYN_LOAD_TEST",AsynTestCase14);

/*
测试逻辑
使用lod2创建人物，其他逻辑和AsynTestCase13相同
*/

class AsynTestCase15 : public AsynTestCase13
{
public:
	AsynTestCase15(const char* casename):AsynTestCase13(casename){}
	virtual int GetMatlod() {return 2;}
private:
	static HippoTestCaseBase* const testcase_;
};
ADD_TESTCASE("ASYN_LOAD_TEST",AsynTestCase15);

/*备注
测试用例没有通过
可能原因：setbodypartandfinishtask 修改了某类资源的数量 如：113001，设置前为14 设置后为12
*/