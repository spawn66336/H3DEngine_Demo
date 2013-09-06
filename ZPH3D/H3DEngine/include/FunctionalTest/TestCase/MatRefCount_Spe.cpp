#include "../HippoTestManager/HippoTestManager.h"
#include "../CommonCode/HippoUtil/ActorUtil.h"
#include "../CommonCode/HippoUtil/HippoScene.h"
#include "../CommonCode/HippoUtil/HippoGlobal.h"
#include "../CommonCode/HippoUtil/RefCountCompare.h"
#include "engine_interface.h"
#include "EffectCore_dx.h"

using namespace  std;

/*测试逻辑
得到当前引擎的引用计数快照
以lod0加载一个普通面片粒子特效
渲染5s
释放特效
得到当前引擎的引用计数快照
比较两个引擎快照是否一样
加载1个带有的模型特效
释放特效
得到当前引擎的引用计数快照
比较两个引擎快照是否一样
*/

class MatRefCountSpe :public HippoTestCaseBase
{
public:
	MatRefCountSpe(const char* casename):HippoTestCaseBase(casename){}
	HippoLevelScene* m_pScene;
	ISpecialEffect* m_pSpe;

	enum ExeState
	{
		INIT,
		SPE_LOAD_COMMEN_EFF,
		SPE_COMMEN_EFF_SPIN,
		SPE_COMMEN_EFF_RELEASE,
		SPE_COMMEN_EFF_SNAPSHOT_COM,
		SPE_LOAD_MODEL,
		SPE_MODEL_SPIN,
		SPE_MODEL_RELEASE,
		SPE_MODEL_SNAPSHOT_COM,
		TEST_OVER
	};
	ExeState m_exeState;

	struct sSnapShot
	{
		HippoResStateSnapShot* SnapShot_Mat;
		HippoResStateSnapShot* SnapShot_Tpl;
		HippoResStateSnapShot* SnapShot_Tex;
	};

	struct sSnapShot beforeSnapShot;
	struct sSnapShot afterSnapShot;


private:
	static HippoTestCaseBase* const testcase_;

public:

	virtual void InitScene()
	{
		m_pScene = Hippo_GetSceneManager()->CreateEmptyLevel();
		Hippo_GetIRender()->OpenActionLib("../resources/art/role/actions/role.xml");
		ActorUtil::InitActorDress("../resources/config/shared/item/dress.xml");
		m_pSpe = 0;
		m_exeState = INIT;
		m_pScene->RestructScene();
	}

	virtual  void CleanUpScene()
	{
		m_pScene->CleanScene();
		Hippo_GetSceneManager()->DelScene(m_pScene);
	}

	void  GetNowRefCount(sSnapShot &SnapShot)
	{
		SnapShot.SnapShot_Mat = RefCountCompare::GetResState(HIPPO_MAT_RES);
		SnapShot.SnapShot_Tpl = RefCountCompare::GetResState(HIPPO_TPL_RES);
		SnapShot.SnapShot_Tex = RefCountCompare::GetResState(HIPPO_TEX_RES);
	}

	void SnapShotCom(sSnapShot& before,sSnapShot& after)
	{
		HIPPO_EXPECT_EQ(RefCountCompare::SnapShotEqual(before.SnapShot_Mat,after.SnapShot_Mat),true);
		HIPPO_EXPECT_EQ(RefCountCompare::SnapShotEqual(before.SnapShot_Tex,after.SnapShot_Tex),true);
		HIPPO_EXPECT_EQ(RefCountCompare::SnapShotEqual(before.SnapShot_Tpl,after.SnapShot_Tpl),true);
		before.SnapShot_Mat->Release();
		before.SnapShot_Tex->Release();
		before.SnapShot_Tpl->Release();
		after.SnapShot_Mat->Release();
		after.SnapShot_Tex->Release();
		after.SnapShot_Tpl->Release();
	}

	void CreatSpe(bool bModel,int lod)
	{
		if (bModel)
		{
			m_pSpe = m_pScene->CreateSpe("../resources/art/role/link/female/4115/4115006001/4115006001.spe"/*,lod*/);
		}
		else
		{
			m_pSpe = m_pScene->CreateSpe("../resources/art/role/link/female/4115/4115014002/4115014002.spe"/*,lod*/);
		}
	}

	void DelSpe()
	{
		m_pScene->DelSpe(m_pSpe);
	}

	void NextState()
	{
		int* p  = (int*)(&m_exeState);
		++(*p);
	}

	bool Executing()
	{
		switch(m_exeState)
		{

		case  INIT:
			GetNowRefCount(beforeSnapShot);
			NextState();
			break;
		case SPE_LOAD_COMMEN_EFF:
			CreatSpe(false,0);
			NextState();
			break;
		case SPE_COMMEN_EFF_SPIN:
			if (Spin_N_Ms(5000))
			{
				NextState();
			}
			break;
		case SPE_COMMEN_EFF_RELEASE:
			DelSpe();
			GetNowRefCount(afterSnapShot);
			NextState();
			break;
		case SPE_COMMEN_EFF_SNAPSHOT_COM:
			SnapShotCom(beforeSnapShot,afterSnapShot);
			NextState();
			break;
		case SPE_LOAD_MODEL:
			GetNowRefCount(beforeSnapShot);
			CreatSpe(true,0);
			NextState();
			break;
		case SPE_MODEL_SPIN:
			if (Spin_N_Ms(5000))
			{
				NextState();
			}
			break;
		case SPE_MODEL_RELEASE:
			DelSpe();
			GetNowRefCount(afterSnapShot);
			NextState();
		case  SPE_MODEL_SNAPSHOT_COM:
			SnapShotCom(beforeSnapShot,afterSnapShot);
			NextState();
			break;
		default:
			return true;

		}
		return false;
	}

	virtual bool AutoRunExec()
	{
		if (m_exeState == TEST_OVER )
		{
			return true;
		}
		return Executing();
	}
	bool ManualRunExec()
	{
		return Executing();
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
ADD_TESTCASE("MATERIAL_TEST",MatRefCountSpe);