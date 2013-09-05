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
��lod0����һ��actor0
��actor0ͬ������һ���װ+�Ҽ�
���첽�ķ�ʽ��actor1�л���lod1��task0��
Task0���֮ǰcancel�����ȵ�������ض�����ĿΪ0��ִ����һ����
clearmatlib
��actor0��ͬ���ķ�ʽ����lod1
��Ⱦ5s�۲�Ч��
�ͷ���������
*/

/*��ע
�ò�������ͨ������
*/

class AsynTestCase22 : public HippoTestCase_RefCompare
{
public:
public:
	AsynTestCase22(const char* casename):HippoTestCase_RefCompare(casename){}
	HippoLevelScene* m_pScene;

	H3DI::IActor* m_pActor0;
	H3DI::IActor* m_pActor1;

	const char* bptName;
	unsigned int task0;
	enum ExeState
	{
		INIT,
		//��lod0����һ��actor0  ��actor0ͬ������һ���װ+�Ҽ�
		CREATE_ACTOR_ASYN_BPT_TASK,
		//���첽�ķ�ʽ��actor1�л���lod1��task0��
		ASYN_SET_ACTOR_LOD1,
		//Task0���֮ǰcancel����
		BEFORE_CANCEL_TASK0,
		//�ȵ�������ض�����ĿΪ0��ִ����һ����
		WAIT_QUEUE_IS_ZERO,
		//clearmatlib //��actor0��ͬ���ķ�ʽ����lod1
		SET_LOD1,
		//��Ⱦ5s�۲�Ч��
		SPIN_5S,
		//�ͷ���������
		RELEASE_ACTOR,
		TEST_OVER
	};
	ExeState m_exeState;

private:
	static HippoTestCaseBase* const testcase_;
public:
	virtual void InitScene()
	{
		m_pScene = Hippo_GetSceneManager()->CreateEmptyLevel();
		m_exeState = INIT;
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

	void NextState()
	{
		int* p = (int*)(&m_exeState);
		++(*p);
	}
 
	int GetMatLod1(){return 1;}
	int GetMatLod0(){return 0;}
	void CreateAcotrAndBptAdorment()
	{
		//��lod0����һ��actor0
		H3DVec3 pos0(-1.5f,0.f,0.f);
		m_pActor0 = m_pScene->CreateActor(false,GetMatLod0());
		m_pActor0->SetPosition(pos0);

		H3DVec3 pos1(1.5f,0.f,0.f);
		m_pActor1 = m_pScene->CreateActor(false,GetMatLod0());
		m_pActor1->SetPosition(pos1);
		//actor0ͬ������һ���װ+�Ҽ�
		m_pActor0->SetAdornment((H3DI::EActorAdornmentPosition)1,"../resources/art/role/link/female/4110/4110001001/4110001001.spe");
		m_pActor0->SetAdornment((H3DI::EActorAdornmentPosition)2,"../resources/art/role/link/female/4112/4112001001/4112001001.spe");
		m_pActor0->SetAdornment((H3DI::EActorAdornmentPosition)3,"../resources/art/role/link/female/4113/4113006002/4113006002.spe");
		m_pActor0->SetAdornment((H3DI::EActorAdornmentPosition)4,"../resources/art/role/link/female/4114/4114004003/4114004003.spe");

		m_pActor0->SetAdornment((H3DI::EActorAdornmentPosition)5,"../resources/art/role/link/female/4115/4115004001/4115004001.spe");
		m_pActor0->SetAdornment((H3DI::EActorAdornmentPosition)6,"../resources/art/role/link/female/4116/4116009002/4116009002.spe");
		m_pActor0->SetAdornment((H3DI::EActorAdornmentPosition)7,"../resources/art/role/link/female/4117/4117004001/4117004001.spe");
		m_pActor0->SetAdornment((H3DI::EActorAdornmentPosition)8,"../resources/art/role/link/female/4118/4118006001/4118006001.spe");
		m_pActor0->SetAdornment((H3DI::EActorAdornmentPosition)9,"../resources/art/role/link/female/4119/4119007001/4119007001.spe");
		m_pActor0->SetAdornment((H3DI::EActorAdornmentPosition)10,"../resources/art/role/link/female/4120/4120007001/4120007001.spe");
		m_pActor0->SetAdornment((H3DI::EActorAdornmentPosition)11,"../resources/art/role/link/female/4121/4121007001/4121007001.spe");
		m_pActor0->SetAdornment((H3DI::EActorAdornmentPosition)0,"../resources/art/role/link/female/4111/4111006001/4111006001.spe");

		m_pActor0->SetBodyPart("../resources/art/role/bodypart/female/socks/117013001/117013001.bpt");
		m_pActor0->SetBodyPart("../resources/art/role/bodypart/female/body/114001001/114001001.BPT");
		m_pActor0->SetBodyPart("../resources/art/role/bodypart/female/face/112002001/112002001.BPT");
		m_pActor0->SetBodyPart("../resources/art/role/bodypart/female/hair/113004001/113004001.BPT");
		m_pActor0->SetBodyPart("../resources/art/role/bodypart/female/hand/115002001/115002001.bpt");
		m_pActor0->SetBodyPart("../resources/art/role/bodypart/female/shoe/118006001/118006001.bpt");
		m_pActor0->SetBodyPart("../resources/art/role/bodypart/female/strippedbody/11a001.BPT");
		m_pActor0->SetBodyPart("../resources/art/role/bodypart/female/suit/119005001/1193005001/1193005001.bpt");
		m_pActor0->SetBodyPart("../resources/art/role/bodypart/female/trousers/116004001/116004001.bpt");
	}
	virtual bool ManualRunExec()
	{
		Hippo_GetIRender()->UpdateEngineThread();
		switch (m_exeState)
		{
		case INIT:
			NextState();
			break;
				
		case CREATE_ACTOR_ASYN_BPT_TASK://��lod0����һ��actor0  ��actor0ͬ������һ���װ+�Ҽ�
			CreateAcotrAndBptAdorment();
			NextState();
			break;
				
		case ASYN_SET_ACTOR_LOD1://���첽�ķ�ʽ��actor1�л���lod1��task0��
			{
				task0 = Hippo_GetIRender()->LoadMaterialLodAsyn(m_pActor1,GetMatLod1(),0);
				NextState();
				break;
			}
				
		case BEFORE_CANCEL_TASK0://Task0���֮ǰcancel����
			Hippo_GetIRender()->CancelTask(task0);
			NextState();
			break;
				
		case WAIT_QUEUE_IS_ZERO://�ȵ�������ض�����ĿΪ0��ִ����һ����
			if (0 == Hippo_GetIRender()->GetWaitingTaskCount())
			{
				NextState();
			}
			break;	
				
		case SET_LOD1://clearmatlib //��actor0��ͬ���ķ�ʽ����lod1
			{
				Hippo_GetIRender()->ClearMaterialLib();
				m_pActor0->SetMaterialLod(GetMatLod1());
				NextState();
				break;
			}
				//��Ⱦ5s�۲�Ч��
			case SPIN_5S:
				if (Spin_N_Ms(5000))
				{
					NextState();
				}
				break;
				//�ͷ���������
			case RELEASE_ACTOR:
				m_pScene->DelActor(m_pActor0);
				m_pScene->DelActor(m_pActor1);
				NextState();
				break;
		}
		return  false;
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

ADD_TESTCASE("ASYN_LOAD_TEST",AsynTestCase22);