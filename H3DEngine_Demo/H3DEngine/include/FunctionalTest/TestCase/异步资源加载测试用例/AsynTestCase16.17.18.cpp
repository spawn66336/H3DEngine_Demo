#include "../../HippoTestManager/HippoTestManager.h"
#include "../CommonCode/HippoUtil/ActorUtil.h"
#include "../CommonCode/HippoUtil/HippoScene.h"
#include "../CommonCode/HippoUtil/HippoGlobal.h"
#include "../CommonCode/HippoUtil/RefCountCompare.h"
#include "engine_interface.h"
#include "engine_interface_internal.h"
#include "EffectCore_dx.h"
#include "../idMath/dMathHeader.h"
#include <algorithm>
using namespace  std;

/*�����߼�
��lod0����1��actor0��actor1
��actor0�첽����һ����װ��task0��
task0������ɺ����ø�Actor0
ִ��clearmatlib
��¼���ü�������s2
��actor1ͬ������ͬ����һ����װ
��task0 cancel�����ȵ�������ض�����ĿΪ0��ִ����һ����
ִ��clearmatlib
��¼���ü�������s3
�Ƚ����ü�������s2��s3������Ӧ����ȫһ��
��Ⱦ5s���۲���ʾЧ��
�ͷ��������ִ��clearmatlib
*/

/*��ע
��������û��ͨ��
����㣺�ڱȽ����ü�����ʱ��tex ��tpl ǰ��������һ��
����ԭ��
*/
class AsynTestCase16 :public HippoTestCase_RefCompare
{
public: 
	AsynTestCase16(const char* casename):HippoTestCase_RefCompare(casename){}

	HippoLevelScene* m_pScene;
	unsigned int task0;

	H3DI::IActor* m_pActor0;
	H3DI::IActor* m_pActor1;

	const char* bptName ;

	HippoResStateSnapShot* m_S2_Mat;
	HippoResStateSnapShot* m_S2_TPL;
	HippoResStateSnapShot* m_S2_Tex;

	HippoResStateSnapShot* m_S3_Mat;
	HippoResStateSnapShot* m_S3_TPL;
	HippoResStateSnapShot* m_S3_Tex;

	enum ExeState
	{
		INIT,
		//��lod0����1��actor0��actor1 ��actor0�첽����һ����װ��task0��
		CREATE_ACTOR_ASYN_BPT_TASK0,
		//task0������ɺ����ø�Actor0
		AFTER_LOAD_NO_SET,
		//ִ��clearmatlib ��¼���ü������գ�s2�� ��actor1ͬ������ͬ����һ����װ
		SNAPSHOT2_SET_BPT_ACTOR1,
		//��task0 cancel��
		CANCEL_TASK0,
		//�ȵ�������ض�����ĿΪ0��ִ����һ����
		WAIT_QUEUE_IS_ZERO,
		//ִ��clearmatlib,��¼���ü������գ�s3��//�Ƚ�������¼���ü�������s2��s3����������Ӧ��һ����
		SNAPSHOT3_AND_COM,
		//��Ⱦ5s
		SPIN_5S,
		//�ͷ��������ִ��clearmatlib
		RELEASE_CLERAMATLIB,
		TEST_OVER
	};
	ExeState m_exeState;


private:
	static HippoTestCaseBase* const testcase_;

public:
	virtual int GetMatlod() {return 0;}
	virtual void InitScene()
	{
		m_pScene = Hippo_GetSceneManager()->CreateEmptyLevel();

		bptName = "../resources/art/role/bodypart/female/body/114028002/114028002.bpt";

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
		//��lod0����1��actor0��actor1	��actor0�첽����һ����װ��task0��
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
		case CREATE_ACTOR_ASYN_BPT_TASK0: ///��lod0����1��actor0��actor1 ��actor0�첽����һ����װ��task0��
			CreateActorAndLoadBpt();
			NextState();
			break;
		case AFTER_LOAD_NO_SET: //task0������ɺ����ø�Actor0
			if (IsTaskLoaded(task0))
			{
				NextState();
			}
			break;
		case SNAPSHOT2_SET_BPT_ACTOR1://ִ��clearmatlib ��¼���ü������գ�s2�� ��actor1ͬ������ͬ����һ����װ
			Hippo_GetIRender()->ClearMaterialLib();
			//��¼���ü������գ�s2��
			m_S2_Mat = RefCountCompare::GetResState(HIPPO_MAT_RES);
			m_S2_TPL = RefCountCompare::GetResState(HIPPO_TEX_RES);
			m_S2_Tex = RefCountCompare::GetResState(HIPPO_TPL_RES);
			m_pActor1->SetBodyPart(bptName);
			NextState();
			break;
		case CANCEL_TASK0://��task0 cancel��
			 Hippo_GetIRender()->CancelTask(task0);
			 NextState();
			break;
		case WAIT_QUEUE_IS_ZERO://�ȵ�������ض�����ĿΪ0��ִ����һ����
			if (0 == Hippo_GetIRender()->GetWaitingTaskCount())
			{
				NextState();
			}
			break;
		case SNAPSHOT3_AND_COM: //��¼���ü������գ�s3��//�Ƚ�������¼���ü�������s2��s3����������Ӧ��һ��
			//��¼���ü������գ�s3��
			m_S3_Mat = RefCountCompare::GetResState(HIPPO_MAT_RES);
			m_S3_Tex = RefCountCompare::GetResState(HIPPO_TEX_RES);
			m_S3_TPL = RefCountCompare::GetResState(HIPPO_TPL_RES);

			//�Ƚ�������¼���ü�������s2��s3������Ҫ��ȫһ��
			HIPPO_EXPECT_EQ(RefCountCompare::SnapShotEqual(m_S2_Mat,m_S3_Mat),true);
			HIPPO_EXPECT_EQ(RefCountCompare::SnapShotEqual(m_S2_Tex,m_S3_Tex),true);
			HIPPO_EXPECT_EQ(RefCountCompare::SnapShotEqual(m_S2_TPL,m_S3_TPL),true);

			m_S2_Mat->Release();
			m_S2_TPL->Release();
			m_S2_Tex->Release();
			m_S3_Mat->Release();
			m_S3_TPL->Release();
			m_S3_Tex->Release();
			NextState();
			break;
		case  SPIN_5S:
			if (Spin_N_Ms(5000))
			{
				NextState();
			}
			break;
		case RELEASE_CLERAMATLIB://�ͷ��������ִ��clearmatlib
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
		
		return ManualRunExec();
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
ADD_TESTCASE("ASYN_LOAD_TEST",AsynTestCase16);
/*
�����߼�
ʹ��lod1������������߼���AsynTestCase16��ͬ
*/

class AsynTestCase17 : public AsynTestCase16
{
public:
	AsynTestCase17(const char* casename):AsynTestCase16(casename){}
	virtual int GetMatlod() {return 1;}
private:
	static HippoTestCaseBase* const testcase_;
};
ADD_TESTCASE("ASYN_LOAD_TEST",AsynTestCase17);

/*
�����߼�
ʹ��lod2������������߼���AsynTestCase16��ͬ
*/

class AsynTestCase18 : public AsynTestCase16
{
public:
	AsynTestCase18(const char* casename):AsynTestCase16(casename){}
	virtual int GetMatlod() {return 2;}
private:
	static HippoTestCaseBase* const testcase_;
};
ADD_TESTCASE("ASYN_LOAD_TEST",AsynTestCase18);