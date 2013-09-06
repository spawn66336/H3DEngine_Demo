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

/*�����߼�
lod0����1��actor
�첽����һ����װ
������ɺ����ø�Actor
ִ��clearmatlib
��¼���ü������գ�s2��
��������ķ�װ���ø�����
��Ⱦ5s���۲���ʾЧ��
��¼���ü������գ�s3��
�Ƚ�������¼���ü�������s2��s3������Ҫ��ȫһ��
�ͷ����ִ��clearmatlib
*/

/*��ע
��������û��ͨ��
����㣺ǰ���¼���ü������գ�s2,s3 ����� �޷�ͨ������
����ԭ��
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
		//lod0����1��actor �첽����һ����װ
		CREATE_ACTOR_ASYN_BPT,
		//������ɺ����ø�Actor
		AFTER_LOAD_NO_SET,
		//ִ��clearmatlib ��¼���ü������գ�s2�� ��������ķ�װ���ø�����
		SNAPSHOT2_SET_BPT,
		SPIN_5S,
		//��¼���ü������գ�s3��//�Ƚ�������¼���ü�������s2��s3������Ҫ��ȫһ�� //�ͷ����ִ��clearmatlib
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
		//��lod0����1��actor �첽����һ����װ
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
		//��¼���ü������գ�s2��
		m_S2_Mat = RefCountCompare::GetResState(HIPPO_MAT_RES);
		m_S2_TPL = RefCountCompare::GetResState(HIPPO_TPL_RES);
		m_S2_Tex = RefCountCompare::GetResState(HIPPO_TEX_RES);
		//��������ķ�װ���ø�����
		m_pActor->SetBodyPartAndFinishTask(taskId);
		
	}
	void SnapShot3_Com_Realese()
	{
		//��¼���ü������գ�s3��
		
		m_S3_Mat = RefCountCompare::GetResState(HIPPO_MAT_RES);
		m_S3_TPL = RefCountCompare::GetResState(HIPPO_TPL_RES);
		m_S3_Tex = RefCountCompare::GetResState(HIPPO_TEX_RES);
		
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
		//�ͷ����ִ��clearmatlib
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
		case CREATE_ACTOR_ASYN_BPT: //lod0����1��actor �첽����һ����װ
			CreateActorAndLoadBpt();
			NextState();
			break;
		case AFTER_LOAD_NO_SET: //������ɺ����ø�Actor
			if (IsTaskLoaded(taskId))
			{
				NextState();
			}
			break;
		case SNAPSHOT2_SET_BPT: //ִ��clearmatlib ��¼���ü������գ�s2�� ��������ķ�װ���ø�����
			SnapShot2AndSetBpt();
			NextState();
			break;
		case  SPIN_5S:
			if (Spin_N_Ms(5000))
			{
				NextState();
			}
			break;
		case SNAPSHOT3_COM_RELEASE://��¼���ü������գ�s3��//�Ƚ�������¼���ü�������s2��s3������Ҫ��ȫһ�� //�ͷ����ִ��clearmatlib
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
�����߼�
ʹ��lod1������������߼���AsynTestCase10��ͬ
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
�����߼�
ʹ��lod2������������߼���AsynTestCase10��ͬ
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
