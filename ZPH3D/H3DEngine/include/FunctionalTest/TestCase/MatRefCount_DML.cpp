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
 1.以lod0创建dml，渲染五秒钟后释放，比较前后引用计数
 2.以lod1创建dml，渲染五秒钟后释放，比较前后引用计数
 3.以lod2创建dml，渲染五秒钟后释放，比较前后引用计数
*/
class MatRefCountDml : public HippoTestCaseBase
{
public:
	MatRefCountDml(const char* case_name) : HippoTestCaseBase(case_name){}
	HippoLevelScene* m_pScene;
	H3DI::IModel* m_pDml;


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
		//lod0的方式创建1个dml
		DML_LOD0,
		DML_LOD0_SPIN,
		DML_LOD0_DEL,

		SNAPSHOT_LOD0_COM,

		DML_LOD1,
		DML_LOD1_SPIN,
		DML_LOD1_DEL,

		SNAPSHOT_LOD1_COM,

		//lod2的方式创建该模型
		DML_LOD2,
		DML_LOD2_SPIN,
		DML_LOD2_DEL,

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
		m_exeState = INIT;
		m_pDml = 0;

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
		//string str= RefCountCompare::SnapShotEqual(before,after) == true? state+"引用计数相等" : state+"引用计数不等";
		//Hippo_WriteConsoleAndLog(CC_WHITE,"% s",str.c_str());
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
	void CreateDml(int lod)
	{
		m_pDml = m_pScene->CreateDml("../resources/art/role/link/female/4110/4110001001/4110001.dml",lod);
	}

	void DeleteDml()
	{
		m_pScene->DelDml(m_pDml);
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
		case DML_LOD0:
			CreateDml(0);
			NextState();
			break;
		case  DML_LOD0_SPIN:
			if (Spin_N_Frame(150))
			{
				NextState();
			}
			break;

		case DML_LOD0_DEL:
			DeleteDml();
			GetNowRefCount(afterSnapShot);
			NextState();
			break;

		case SNAPSHOT_LOD0_COM:
			SnapShotCom(beforeSnapShot,afterSnapShot);
			NextState();
			break;

		case DML_LOD1:
			GetNowRefCount(beforeSnapShot);
			CreateDml(1);
			NextState();
			break;
		case DML_LOD1_SPIN:
			if (Spin_N_Frame(150))
			{
				NextState();
			}
			break;
		case DML_LOD1_DEL:
			DeleteDml();
			GetNowRefCount(afterSnapShot);
			NextState();
			break;

		case SNAPSHOT_LOD1_COM:
			SnapShotCom(beforeSnapShot,afterSnapShot);
			NextState();
			break;

		case DML_LOD2:
			GetNowRefCount(beforeSnapShot);
			CreateDml(2);
			NextState();
			break;
		case DML_LOD2_SPIN:
			if (Spin_N_Frame(150))
			{
				NextState();
			}
			break;
		case DML_LOD2_DEL:
			DeleteDml();
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

ADD_TESTCASE("MATERIAL_TEST",MatRefCountDml);