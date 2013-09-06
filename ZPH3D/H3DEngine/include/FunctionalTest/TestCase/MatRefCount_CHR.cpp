#include "../HippoTestManager/HippoTestManager.h"
#include "../CommonCode/HippoUtil/HippoScene.h"
#include "../CommonCode/HippoUtil/RefCountCompare.h"
#include "../CommonCode/HippoUtil/HippoGlobal.h"
#include "../CommonCode/HippoUtil/ActorUtil.h"
#include "engine_interface.h"
#include "EffectCore_dx.h"
#include <iostream>

using namespace std;

/*测试逻辑
 1.以lod0创建Chr，渲染五秒钟后释放，比较前后引用计数
 2.以lod1创建Chr，渲染五秒钟后释放，比较前后引用计数
 3.以lod2创建Chr，渲染五秒钟后释放，比较前后引用计数
*/
class MatRefCountChr : public HippoTestCaseBase
{
public:
	MatRefCountChr(const char* case_name) : HippoTestCaseBase(case_name){}
	HippoLevelScene* m_pScene;
	H3DI::ISkeletonModel* m_pChr;

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

		CHR_LOD0,
		CHR_LOD0_SPIN,
		CHR_LOD0_DEL,

		SNAPSHOT_LOD0_COM,

		CHR_LOD1,
		CHR_LOD1_SPIN,
		CHR_LOD1_DEL,

		SNAPSHOT_LOD1_COM,

		CHR_LOD2,
		CHR_LOD2_SPIN,
		CHR_LOD2_DEL,

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
		m_exeState = INIT;
		m_pChr = 0;

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

	void CreateChr(int lod)
	{
		m_pChr = m_pScene->CreateChr("../resources/art/role/bodypart/female/body/114001001/114001_M.chr",lod);
	}

	void DeleteChr()
	{
		m_pScene->DelChr(m_pChr);
		Hippo_GetIRender()->ClearMaterialLib();
	}


	void NextState()
	{
		int* p=(int*)(&m_exeState);
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
		case CHR_LOD0:
			CreateChr(0);
			NextState();
			break;
		case  CHR_LOD0_SPIN:
			if (Spin_N_Frame(150))
			{
				NextState();
			}
			break;

		case CHR_LOD0_DEL:
			DeleteChr();
			GetNowRefCount(afterSnapShot);
			NextState();
			break;

		case SNAPSHOT_LOD0_COM:
			SnapShotCom(beforeSnapShot,afterSnapShot);
			NextState();
			break;

		case CHR_LOD1:
			GetNowRefCount(beforeSnapShot);
			CreateChr(1);
			NextState();
			break;
		case CHR_LOD1_SPIN:
			if (Spin_N_Frame(150))
			{
				NextState();
			}
			break;
		case CHR_LOD1_DEL:
			DeleteChr();
			GetNowRefCount(afterSnapShot);
			NextState();
			break;

		case SNAPSHOT_LOD1_COM:
			SnapShotCom(beforeSnapShot,afterSnapShot);
			NextState();
			break;

		case CHR_LOD2:
			GetNowRefCount(beforeSnapShot);
			CreateChr(2);
			NextState();
			break;
		case CHR_LOD2_SPIN:
			if (Spin_N_Frame(150))
			{
				NextState();
			}
			break;
		case CHR_LOD2_DEL:
			DeleteChr();
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

ADD_TESTCASE("MATERIAL_TEST",MatRefCountChr);