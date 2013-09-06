#include "../HippoTestManager/HippoTestManager.h"
#include "../CommonCode/HippoUtil/HippoScene.h"
#include "../CommonCode/HippoUtil/RefCountCompare.h"
#include "../CommonCode//HippoUtil/HippoGlobal.h"
#include "../CommonCode/HippoUtil/ActorUtil.h"
#include "engine_interface.h"
#include "EffectCore_dx.h"
#include <iostream>
using namespace std;

/*测试逻辑
 1.以lod0创建actor，渲染五秒钟后释放，比较前后引用计数
 2.以lod1创建actor，渲染五秒钟后释放，比较前后引用计数
 3.以lod2创建actor，渲染五秒钟后释放，比较前后引用计数
*/
class MatRefCountActor : public HippoTestCaseBase
{
public:
	MatRefCountActor(const char* case_name):HippoTestCaseBase(case_name){}

	HippoLevelScene* m_pScene;
	H3DI::IActor* m_pActor;
	bool m_bActorSex;

	struct sSnapShot
	{
		HippoResStateSnapShot* SnapShot_Mat;
		HippoResStateSnapShot* SnapShot_Tpl;
		HippoResStateSnapShot* SnapShot_Tex;
	};

	struct sSnapShot beforeSnapShot;
	struct sSnapShot afterSnapShot;

	enum ExeState
	{
		INIT,

		ACTOR_LOD0,
		ACTOR_LOD0_SPIN,
		ACTOR_LOD0_DEL,

		SNAPSHOT_LOD0_COM,

		ACTOR_LOD1,
		ACTOR_LOD1_SPIN,
		ACTOR_LOD1_DEL,

		SNAPSHOT_LOD1_COM,

		ACTOR_LOD2,
		ACTOR_LOD2_SPIN,
		ACTOR_LOD2_DEL,

		SNAPSHOT_LOD2_COM,

		TEST_OVER
	};


	ExeState m_exeState;

private:
	static HippoTestCaseBase* const testcase_;

public:
	virtual void InitScene()
	{
		m_pScene = Hippo_GetSceneManager()->CreateEmptyLevel();
		Hippo_GetIRender()->OpenActionLib("../resources/art/role/actions/role.xml");
		ActorUtil::InitActorDress("../resources/config/shared/item/dress.xml");
		m_bActorSex = false;

		m_exeState = INIT;

		m_pActor = 0;
		m_pScene->RestructScene();
	}

	virtual void CleanUpScene()
	{
		m_pScene->CleanScene();
		Hippo_GetSceneManager()->DelScene(m_pScene);
	}

	void GetNowRefCount(sSnapShot &snapshort)
	{
		snapshort.SnapShot_Mat = RefCountCompare::GetResState(HIPPO_MAT_RES);
		snapshort.SnapShot_Tex = RefCountCompare::GetResState(HIPPO_TEX_RES);
		snapshort.SnapShot_Tpl = RefCountCompare::GetResState(HIPPO_TPL_RES);
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

	void CreateActor(int lod)
	{
		m_pActor = m_pScene->CreateActor(m_bActorSex,lod);
		
	}

	void DelActor()
	{
		m_pScene->DelActor(m_pActor);
		Hippo_GetIRender()->ClearMaterialLib();
	}

	void NextState()
	{
		int* p = (int*)(&m_exeState);
		++(*p);
	}


	bool Executing()
	{
		switch(m_exeState)
		{
		case INIT:
			GetNowRefCount(beforeSnapShot);
			NextState();
			break;
		case ACTOR_LOD0:
			CreateActor(0);
			NextState();
			break;
		case ACTOR_LOD0_SPIN:
			if (Spin_N_Frame(150))
			{
				NextState();
			}
			break;
		case ACTOR_LOD0_DEL:
			DelActor();
			GetNowRefCount(afterSnapShot);
			NextState();
			break;
		case SNAPSHOT_LOD0_COM:
			SnapShotCom(beforeSnapShot,afterSnapShot);
			NextState();
			break;

		case ACTOR_LOD1:
			GetNowRefCount(beforeSnapShot);
			CreateActor(1);
			NextState();
			break;
		case ACTOR_LOD1_SPIN:
			if (Spin_N_Frame(150))
			{
				NextState();
			}
			break;
		case ACTOR_LOD1_DEL:
			DelActor();
			GetNowRefCount(afterSnapShot);
			NextState();
			break;
		case SNAPSHOT_LOD1_COM:
			SnapShotCom(beforeSnapShot,afterSnapShot);
			NextState();
			break;

		case ACTOR_LOD2:
			GetNowRefCount(beforeSnapShot);
			CreateActor(2);
			NextState();
			break;
		case ACTOR_LOD2_SPIN:
			if (Spin_N_Frame(150))
			{
				NextState();
			}
			break;
		case ACTOR_LOD2_DEL:
			DelActor();
			GetNowRefCount(afterSnapShot);
			NextState();
			break;
		case SNAPSHOT_LOD2_COM:
			SnapShotCom(beforeSnapShot,afterSnapShot);
			NextState();
			break;
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

	virtual bool ManualRunExec()
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

ADD_TESTCASE("MATERIAL_TEST",MatRefCountActor);