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
��lod0 ����1��actor
���첽�ķ�ʽ����8��bodypart
�ڼ��صĹ����У�ÿִ֡��һ��clearmatlib
������ɺ���Ⱦ5s���۲��װЧ���Ƿ���ȷ
�ͷ�����
ִ��clearmatlib
*/

/*��ע
 ������ͨ������
*/

class AsynTestCase4 : public HippoTestCase_RefCompare
{
public:
	AsynTestCase4(const char* casename):HippoTestCase_RefCompare(casename){}
	HippoLevelScene* m_pScene;

	H3DI::IActor* m_pActor;
	std::vector<unsigned int> m_all_task;

	enum ExeState
	{
		INIT,
		//lod0����1��actor ���첽�ķ�ʽ����8��bodypart
		CREATE_ACTOR_ASYN_BPT,
		//�ڼ��صĹ����У�ÿִ֡��һ��clearmatlib,������ɺ� ���˴���
		LOADING_THEN_SETBPT,
		//��Ⱦ5s
		SPIN_5S,
		//�ͷ����ִ��clearmatlib
		RELEASE_ACTOR_CLEARMATLIB,
		TEST_OVER
	};


	ExeState m_exeState;
	
private:
	static HippoTestCaseBase* const testcase_;
public:

	virtual int GetMatLod() {return 0;}
	virtual void InitScene()
	{
		m_pScene = Hippo_GetSceneManager()->CreateEmptyLevel();
		m_exeState = INIT;
	}

	bool IsAllLoaded()
	{
		Hippo_GetIRender()->UpdateEngineThread();
		auto itr=m_all_task.begin();
		while (itr!=m_all_task.end())
		{
			unsigned int taskId=*itr;
			unsigned int  res = Hippo_GetIRender()->QueryTaskState(taskId);

			//���������Դ�ڼ��� ����false
			if (res == H3DI::TASK_WORKING)
			{
				return false;
			}
			++itr;
		}
		return true;
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
	void NextState()
	{
		int * p = (int*)(&m_exeState);
		++(*p);
	}
	void CreateActorAndBpt()
	{
		m_pActor = m_pScene->CreateActor(false,GetMatLod());
		H3DI::sCreateOp createOp;
		createOp.mat_lod = GetMatLod();
		createOp.geo_lod = GetMatLod();
		//���첽�ķ�ʽ����8��bodypart
		m_all_task.push_back(Hippo_GetIRender()->LoadBodypartAsyn("../resources/art/role/bodypart/female/hand/115002001/115002001.bpt",H3DI::ACTOR_HUMAN, createOp,0)) ;
		m_all_task.push_back(Hippo_GetIRender()->LoadBodypartAsyn("../resources/art/role/bodypart/female/trousers/116004001/116004001.bpt",H3DI::ACTOR_HUMAN, createOp,0)) ;
		m_all_task.push_back(Hippo_GetIRender()->LoadBodypartAsyn("../resources/art/role/bodypart/female/suit/119005001/1193005001/1193005001.bpt",H3DI::ACTOR_HUMAN, createOp,0)) ;
		m_all_task.push_back(Hippo_GetIRender()->LoadBodypartAsyn("../resources/art/role/bodypart/female/strippedbody/11a001.BPT",H3DI::ACTOR_HUMAN, createOp,0)) ;
		m_all_task.push_back(Hippo_GetIRender()->LoadBodypartAsyn("../resources/art/role/bodypart/female/shoe/118006001/118006001.bpt",H3DI::ACTOR_HUMAN, createOp,0)) ;
		m_all_task.push_back(Hippo_GetIRender()->LoadBodypartAsyn("../resources/art/role/bodypart/female/hair/113004001/113004001.BPT",H3DI::ACTOR_HUMAN, createOp,0)) ;
		m_all_task.push_back(Hippo_GetIRender()->LoadBodypartAsyn("../resources/art/role/bodypart/female/face/112002001/112002001.BPT",H3DI::ACTOR_HUMAN, createOp,0)) ;
		m_all_task.push_back(Hippo_GetIRender()->LoadBodypartAsyn("../resources/art/role/bodypart/female/body/114001001/114001001.BPT",H3DI::ACTOR_HUMAN, createOp,0)) ;
	}
	void LoadingThenSet()
	{

		if (!IsAllLoaded())
		{	
			//�ڼ��صĹ����У�ÿִ֡��һ��clearmatlib
			Hippo_GetIRender()->ClearMaterialLib();
		}
		else
		{
			//������ɺ� ���˴��ϲ���Ⱦ5s���۲��װЧ���Ƿ���ȷ
			auto itr=m_all_task.begin();
			while(itr!=m_all_task.end())
			{
				m_pActor->SetBodyPartAndFinishTask(*itr);
				itr=m_all_task.erase(itr);
			}
			NextState();
		}
	}

	void ReleseActor()
	{
		m_pScene->DelActor(m_pActor);
		Hippo_GetIRender()->ClearMaterialLib();
	}
	virtual bool ManualRunExec()
	{
		Hippo_GetIRender()->UpdateEngineThread();
		switch (m_exeState)
		{
		case INIT:
			NextState();
			break;
			
		case CREATE_ACTOR_ASYN_BPT://lod0����1��actor ���첽�ķ�ʽ����8��bodypart
			CreateActorAndBpt();
			NextState();
		 	break;
				
		case LOADING_THEN_SETBPT://�ڼ��صĹ����У�ÿִ֡��һ��clearmatlib,������ɺ� ���˴���
			 LoadingThenSet();
			 break;
		case SPIN_5S://��Ⱦ5s
			if (Spin_N_Ms(5000))
			{
				NextState();
			}
			break;	
		case RELEASE_ACTOR_CLEARMATLIB://�ͷ����ִ��clearmatlib
			ReleseActor();
			NextState();
			break;
		}
		return false;
	}

	virtual int OnKeyDown(unsigned int key)
	{
		if(HippoTestManager::GetInstance()->GetCurrentRunMode() )
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
ADD_TESTCASE("ASYN_LOAD_TEST",AsynTestCase4);


/*
�����߼�
ʹ��lod1������������߼���AsynTestCase4��ͬ
*/

class AsynTestCase5 : public AsynTestCase4
{
public:
	AsynTestCase5(const char* casename):AsynTestCase4(casename){}
	virtual int GetMatLod(){return 1;}
private:
	static HippoTestCaseBase* const testcase_;
};
ADD_TESTCASE("ASYN_LOAD_TEST",AsynTestCase5);

/*
�����߼�
ʹ��lod2������������߼���AsynTestCase10��ͬ
*/

class AsynTestCase6 : public AsynTestCase4
{
public:
	AsynTestCase6(const char* casename):AsynTestCase4(casename){}
	virtual int GetMatLod(){return 2;}
private:
	static HippoTestCaseBase* const testcase_;
};
ADD_TESTCASE("ASYN_LOAD_TEST",AsynTestCase6);