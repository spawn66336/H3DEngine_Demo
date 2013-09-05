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
1.��lod0����1��actor�첽����һ����װ
2.���ع�����cancel���ü������񣬵ȵ�������ض�����ĿΪ0��ִ����һ����
3.ִ��clearmatlib
*/

/*��ע
 ������ͨ������
*/


class AsynTestCase1 : public HippoTestCase_RefCompare
{
public:
	AsynTestCase1(const char* casename):HippoTestCase_RefCompare(casename){}
	HippoLevelScene* m_pScene;

	H3DI::IActor* m_pActor;
	std::vector<unsigned int> m_all_task;
	bool bAutoEnd; //�Զ����н�����־

private:
	static HippoTestCaseBase* const testcase_;
public:

	virtual int GetMatLod(){return 0;}

	virtual void InitScene()
	{
		m_pScene = Hippo_GetSceneManager()->CreateEmptyLevel();
		m_pActor = m_pScene->CreateActor(false,GetMatLod());
		//�첽����һ����װ
		H3DI::sCreateOp createOp;
		createOp.mat_lod = GetMatLod();
		createOp.geo_lod = GetMatLod();
		const char* bptName = "../resources/art/role/bodypart/female/suit/119005001/1193005001/1193005001.bpt";
		m_all_task.push_back(Hippo_GetIRender()->LoadBodypartAsyn(bptName,H3DI::ACTOR_HUMAN, createOp,0)) ;
		bAutoEnd = false;
		
	}

	virtual void CleanUpScene()
	{
		m_pScene->CleanScene();
		Hippo_GetSceneManager()->DelScene(m_pScene);
	}

	virtual bool ManualRunExec()
	{
		//���ع�����cancel���ü�������
		Hippo_GetIRender()->UpdateEngineThread();
		auto itr=m_all_task.begin();
		while (itr!=m_all_task.end())
		{
			unsigned int taskId=*itr;
			//unsigned int  res = Hippo_GetIRender()->QueryTaskState(taskId);
			Hippo_GetIRender()->CancelTask(taskId);
			itr=m_all_task.erase(itr);
		}

		//�ȵ�������ض�����ĿΪ0��ִ��ClearMaterialLib,
		if (0 == Hippo_GetIRender()->GetWaitingTaskCount())
		{
			Hippo_GetIRender()->ClearMaterialLib();
			bAutoEnd = true;//�Զ����н�����־
		}
		return false;
	}

	virtual bool AutoRunExec()
	{
		if (bAutoEnd)
		{
			return true;
		}
		ManualRunExec();
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
ADD_TESTCASE("ASYN_LOAD_TEST",AsynTestCase1);

/*
�����߼�
ʹ��lod1������������߼���AsynTestCase1��ͬ
*/

class AsynTestCase2 : public AsynTestCase1
{
public:
	AsynTestCase2(const char* casename):AsynTestCase1(casename){}
	virtual int GetMatLod(){return 1;}
private:
	static HippoTestCaseBase* const testcase_;
};

ADD_TESTCASE("ASYN_LOAD_TEST",AsynTestCase2);


/*
�����߼�
ʹ��lod2������������߼���AsynTestCase1��ͬ
*/
class AsynTestCase3 : public AsynTestCase1
{
public:
	AsynTestCase3(const char* casename):AsynTestCase1(casename){}
	virtual int GetMatLod(){return 2;}
private:
	static HippoTestCaseBase* const testcase_;
};

ADD_TESTCASE("ASYN_LOAD_TEST",AsynTestCase3);
