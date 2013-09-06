#include "../../HippoTestManager/HippoTestManager.h"
#include "../CommonCode/HippoUtil/ActorUtil.h"
#include "../CommonCode/HippoUtil/HippoScene.h"
#include "../CommonCode/HippoUtil/HippoGlobal.h"
#include "../CommonCode/HippoUtil/RefCountCompare.h"
#include "engine_interface.h"
#include "engine_interface_internal.h"
#include "EffectCore_dx.h"
#include "idMath/dMathHeader.h"
#include "../../FunctionalTest/functional_test/HippoTestConfig.h"
using namespace  std;

/*测试逻辑
加载1个dml模型，要求该dml模型的材质使用dsn3个贴图，并且接受自动lod。
加载1个chr模型，要求该chr模型的材质是mask材质，使用dsn3个贴图，且接受自动lod。
将dml和chr以lod0的形式读进来绘制。记录当前的引擎内所有贴图数量。
将dml和chr切换到lod2。执行clearmatlib。记录当前的引擎内所有贴图数量。
此时贴图数量应该减少4张
*/

class Lod2ReadTexture : public HippoTestCaseBase
{
public:
	Lod2ReadTexture(const char* casename) : HippoTestCaseBase(casename){}
	HippoScene* m_pScene;
	H3DI::IModel* m_pDml;
	H3DI::ISkeletonModel* m_Chr;

	bool bAutoEnd;
private:
	static HippoTestCaseBase* const testcase_;
public:
	virtual void InitScene()
	{
		bAutoEnd = false;
		m_pScene = Hippo_GetSceneManager()->CreateEmptyLevel();
		Hippo_GetIRender()->OpenActionLib(Make_X52Resources_Path("art/role/actions/role.xml").c_str());

		//加载1个dml模型  加载1个chr模型
		H3DVec3 posDml(-1.5f,0.f,0.f);
		H3DVec3 posChr(1.5f,0.f,0.f);
		m_pDml = m_pScene->CreateDml(Make_HippoTestResource_Path("lod2读取贴图测试用例/dml模型的材质使用dsn3个贴图，并且接受自动lod2/Sphere01.dml").c_str(),0);
		m_Chr = m_pScene->CreateChr(Make_HippoTestResource_Path("lod2读取贴图测试用例/chr模型的材质是mask材质，使用dsn3个贴图，且接受自动lod/114001_M.chr").c_str(),0);
		m_pDml->SetPosition(posDml);
		m_Chr->SetPosition(posChr);

		//记录当前的引擎内所有贴图数量。
		shared_ptr<HippoResStateSnapShot> Snap_Tex1(RefCountCompare::GetResState(HIPPO_TEX_RES),[&](HippoResStateSnapShot* p){p->Release();});

		m_pDml->SetMaterialLod(2);
		m_Chr->SetMaterialLod(2);

		Hippo_GetIRender()->ClearMaterialLib();

		
		shared_ptr<HippoResStateSnapShot> Snap_Tex2(RefCountCompare::GetResState(HIPPO_TEX_RES),[&](HippoResStateSnapShot* p){p->Release();});

		bool bTex = Snap_Tex1->m_res_count - Snap_Tex2->m_res_count == 4;
		HIPPO_EXPECT_EQ(bTex,true);
		bAutoEnd = true;
	}

	virtual void CleanUpScene()
	{
		m_pScene->CleanScene();
		Hippo_GetSceneManager()->DelScene(m_pScene);
	}

	virtual bool AutoRunExec()
	{
		if (bAutoEnd)
		{
			return true;
		}
		return ManualRunExec();
	}
	virtual bool ManualRunExec()
	{
		return false;
	}

	virtual int OnKeyDown(unsigned int key)
	{
		if (HippoTestManager::GetInstance()->GetCurrentRunMode())
		{
			switch (key)
			{
			case VK_RETURN:
				m_state = CASE_END_EXECING;
				return 0;
			}
		}
		return -1;
	}
};

ADD_TESTCASE("LOD_TESTCASE",Lod2ReadTexture);


/*备注
该测试用例通过
*/