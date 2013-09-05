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
��ȡ���ü�������s1
Task0��ɺ�Ҫ���ø�����
clearmatlib
��actor0��ͬ���ķ�ʽ����lod1
��Ⱦ5s�۲�Ч��
Task0��cancel�����ȵ�������ض�����ĿΪ0��ִ����һ����
��ȡ���ü�������s2
S1�����s2һ��
�ͷ���������
*/


/*��ע
�ò�����������ͨ������
����㣺tpl��mat��������
*/

class AsynTestCase24 : public HippoTestCase_RefCompare
{
public:
public:
	AsynTestCase24(const char* casename):HippoTestCase_RefCompare(casename){}
	HippoLevelScene* m_pScene;

	H3DI::IActor* m_pActor0;
	H3DI::IActor* m_pActor1;

	const char* bptName;
	unsigned int task0;

	enum ExeState
	{
		INIT,
		//��lod0����һ��actor0 ��actor0ͬ������һ���װ+�Ҽ�
		CREATE_ACTOR_ASYN_BPT_ADORMENT,
		//���첽�ķ�ʽ��actor1�л���lod1��task0��
		ASYN_SET_LOD,
		//��ȡ���ü�������s1
		SNAPSHOT1,
		//Task0��ɺ�Ҫ���ø�����
		TASK0_LOADED_NO_SET,
		//clearmatlib  ��actor0��ͬ���ķ�ʽ����lod1
		SET_LOD1_ACTOR0,
		//��Ⱦ5s�۲�Ч��
		SPIN_5S,
		//Task0��cancel����
		CANCEL_TASK0,
		//�ȵ�������ض�����ĿΪ0��ִ����һ����
		WAIT_QUEUE_IS_ZERO,
		//��ȡ���ü�������s2   S1�����s2һ��
		SNAPSHOT2_COM,
		//�ͷ���������
		RELEASE_ACTOR,
		TEST_OVER
	};
	ExeState m_exeState;
private:
	HippoResStateSnapShot* m_S1_Mat;
	HippoResStateSnapShot* m_S1_TPL;
	HippoResStateSnapShot* m_S1_Tex;

	HippoResStateSnapShot* m_S2_Mat;
	HippoResStateSnapShot* m_S2_TPL;
	HippoResStateSnapShot* m_S2_Tex;
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
	void CreatActorAndSetBptAdorment()
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

		m_pActor0->SetAdornment((H3DI::EActorAdornmentPosition)5,"../resources/art/role/link/female/4115/4115028002/4115028002.spe");
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
				
		case CREATE_ACTOR_ASYN_BPT_ADORMENT://��lod0����һ��actor0 ��actor0ͬ������һ���װ+�Ҽ�
			CreatActorAndSetBptAdorment();
			NextState();
			break;
				
		case ASYN_SET_LOD://���첽�ķ�ʽ��actor1�л���lod1��task0��
			{
				//���첽�ķ�ʽ��actor1�л���lod1��task0��
				task0 = Hippo_GetIRender()->LoadMaterialLodAsyn(m_pActor1,GetMatLod1(),0);
				NextState();
				break;
			}
				
		case SNAPSHOT1://��ȡ���ü�������s1
				m_S1_Mat = RefCountCompare::GetResState(HIPPO_MAT_RES);
				m_S1_TPL = RefCountCompare::GetResState(HIPPO_TEX_RES);
				m_S1_Tex = RefCountCompare::GetResState(HIPPO_TPL_RES);
				NextState();
				break;
				
		case TASK0_LOADED_NO_SET://Task0��ɺ�Ҫ���ø�����
			{ 
				unsigned int res = Hippo_GetIRender()->QueryTaskState(task0);
				if (res == H3DI::TASK_OK ||res == H3DI::TASK_ERROR)
				{
					NextState();
				}
			}
			break;
				
		case SET_LOD1_ACTOR0://clearmatlib  ��actor0��ͬ���ķ�ʽ����lod1
			Hippo_GetIRender()->ClearMaterialLib();
			m_pActor0->SetMaterialLod(GetMatLod1());
			NextState();
			break;
				//��Ⱦ5s�۲�Ч��
		case SPIN_5S:
			if (Spin_N_Ms(5000))
			{
				NextState();
			}
			break;
				
		case CANCEL_TASK0://Task0��cancel����
			Hippo_GetIRender()->CancelTask(task0);
			NextState();
			break;
				
		case WAIT_QUEUE_IS_ZERO://�ȵ�������ض�����ĿΪ0��ִ����һ����
			if (0 == Hippo_GetIRender()->GetWaitingTaskCount())
			{
				NextState();
			}
			break;	
				
		case SNAPSHOT2_COM://��ȡ���ü�������s2   S1�����s2һ��
			m_S2_Mat = RefCountCompare::GetResState(HIPPO_MAT_RES);
			m_S2_TPL = RefCountCompare::GetResState(HIPPO_TEX_RES);
			m_S2_Tex = RefCountCompare::GetResState(HIPPO_TPL_RES);
			//�Ƚ�������¼���ü�������s2��s1������Ҫ��ȫһ��
			HIPPO_EXPECT_EQ(RefCountCompare::SnapShotEqual(m_S2_Mat,m_S1_Mat),true);
			HIPPO_EXPECT_EQ(RefCountCompare::SnapShotEqual(m_S2_Tex,m_S1_Tex),true);
			HIPPO_EXPECT_EQ(RefCountCompare::SnapShotEqual(m_S2_TPL,m_S1_TPL),true);

			m_S2_Mat->Release();
			m_S2_TPL->Release();
			m_S2_Tex->Release();
			m_S1_Mat->Release();
			m_S1_TPL->Release();
			m_S1_Tex->Release();
			NextState();
			break;
		case RELEASE_ACTOR://�ͷ���������
			m_pScene->DelActor(m_pActor0);
			m_pScene->DelActor(m_pActor1);
			NextState();
			break;
		}
		return false;
	}

	virtual int OnKeyDown(unsigned int key)
	{
		switch(key)
		{
		case   VK_RETURN:
			m_state = CASE_END_EXECING;
			return 1;
		}
		return 0;
	}
};
ADD_TESTCASE("ASYN_LOAD_TEST",AsynTestCase24);