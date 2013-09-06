#include "../HippoTestManager/HippoTestManager.h"
#include "../CommonCode/HippoUtil/ActorUtil.h"
#include "../CommonCode/HippoUtil/HippoScene.h"
#include "../CommonCode/HippoUtil/HippoGlobal.h"
#include "../CommonCode/HippoUtil/RefCountCompare.h"
#include "engine_interface.h"
#include "engine_interface_internal.h"
#include "EffectCore_dx.h"
#include <vector>
#include <algorithm>
#include <memory>
#include "../functional_test/HippoTestConfig.h"
using namespace  std;

/*测试逻辑
创建资源引用计数快照
准备1个dml，要求材质中使用1张贴图，贴图变量有别名
载入一张贴图得到itexture,并设置给dml的diffuse贴图
此时的diffuse贴图和原来的diffuse贴图应该不是一个指针
执行clearmatlib
应该在列表中无法再找到以前dml的diffuse贴图
释放场景，创建资源引用计数快照
对比两个引用计数快照是否有不同
*/

class RunTimeTextureSet : public HippoTestCaseBase
{
public:
	RunTimeTextureSet(const char* casename):HippoTestCaseBase(casename){}
	HippoLevelScene* m_pScene;
	bool bAutoEnd;

private:
	static HippoTestCaseBase* const testcase_;
public:

	virtual void CleanUpScene()
	{
		if(m_pScene)
		{
			m_pScene->CleanScene();
			Hippo_GetSceneManager()->DelScene(m_pScene);
			m_pScene=0;
		}
	}

	virtual void InitScene()
	{
		bAutoEnd = false;
		//首先创建引用计数快照
		shared_ptr<HippoResStateSnapShot> shot1(RefCountCompare::GetResState(HIPPO_MAT_RES),[&](HippoResStateSnapShot* p){p->Release();});
		shared_ptr<HippoResStateSnapShot> shot2(RefCountCompare::GetResState(HIPPO_TPL_RES),[&](HippoResStateSnapShot* p){p->Release();});
		shared_ptr<HippoResStateSnapShot> shot3(RefCountCompare::GetResState(HIPPO_TEX_RES),[&](HippoResStateSnapShot* p){p->Release();});

		m_pScene = Hippo_GetSceneManager()->CreateEmptyLevel();

	    const char* varName = "TextureSample2D_3_TexFileName";
		const char* alias = "diffuseTexture";
		
		H3DI::IModel* m_pDml = m_pScene->CreateDml(Make_HippoTestResource_Path("art/运行期设置贴图测试/4110001001/4110001.dml").c_str());

		H3DI::INewShader* m_pNewShader = m_pDml->GetNewShaderByIndex(0);
		H3DI::IMatProxy* p=Hippo_GetIGlobalEnv()->CreateIMatProxyFromINewShader(m_pNewShader);
		H3DI::IMatLodData* lod0=p->GetLodData(0);
		//得到原始材质的diffuse的指针和路径
		H3DI::ITexture* pOrgDiffuse=lod0->GetDiffuseTexture();
		const char* tmpstr=0;
		int res=m_pNewShader->GetNodeVarValue_Str(alias,tmpstr);
		HIPPO_EXPECT_EQ(res,1);
		std::string strOrgDiffusePath=tmpstr;

		//创建新的贴图，将贴图设置给模型的diffuse贴图
		H3DI::ITexture* m_pTexture = Hippo_GetIRender()->CreateTextureFromFile("../resources/art/role/material_tpl/levels_color.tga");
		m_pNewShader->SetTexture(alias,m_pTexture);

		//比较前后两次diffusetexture，应该是不同的指针
		H3DI::ITexture* pNowDiffuse=lod0->GetDiffuseTexture();
		HIPPO_EXPECT_EQ(pOrgDiffuse==pNowDiffuse,false);

		//执行clearmatlib以后，org的texture应该被释放，即在引擎的贴图管理器中找不到该指针
		Hippo_GetIRender()->ClearMaterialLib();
		shared_ptr<H3DI::IResInfo> pAllTex(Hippo_GetIGlobalEnv()->GetAllLoadedTextureResInfo(),[&](H3DI::IResInfo* p){p->Release();});
		bool bfind=pAllTex->FindPtr(pOrgDiffuse);
		HIPPO_EXPECT_EQ(bfind,false);

		CleanUpScene();

		//再次创建引用计数快照，比较
		shared_ptr<HippoResStateSnapShot> shot11(RefCountCompare::GetResState(HIPPO_MAT_RES),[&](HippoResStateSnapShot* p){p->Release();});
		shared_ptr<HippoResStateSnapShot> shot22(RefCountCompare::GetResState(HIPPO_TPL_RES),[&](HippoResStateSnapShot* p){p->Release();});
		shared_ptr<HippoResStateSnapShot> shot33(RefCountCompare::GetResState(HIPPO_TEX_RES),[&](HippoResStateSnapShot* p){p->Release();});

		HIPPO_EXPECT_EQ(RefCountCompare::SnapShotEqual(shot1.get(),shot11.get()),true);
		HIPPO_EXPECT_EQ(RefCountCompare::SnapShotEqual(shot2.get(),shot22.get()),true);
		HIPPO_EXPECT_EQ(RefCountCompare::SnapShotEqual(shot3.get(),shot33.get()),true);
		bAutoEnd = true;

		
	}





	virtual bool AutoRunExec()
	{
		if(bAutoEnd)
		{
			return true;
		}
		return false;
	}

	virtual bool ManualRunExec()
	{
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
ADD_TESTCASE("RUN_TIME_TEXTURE_SET",RunTimeTextureSet);