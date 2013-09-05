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

class TextureShaderConstError : public HippoTestCaseBase
{
public:
	TextureShaderConstError(const char* casename):HippoTestCaseBase(casename){}
	HippoLevelScene* m_pScene;
	bool bAutoEnd;

	H3DI::IModel* m_pModel;

	H3DI::ITexture* m_pTexture;

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
		m_pScene = Hippo_GetSceneManager()->CreateEmptyLevel();

	    const char* varName = "TextureSample2D_3_TexFileName";
		const char* alias = "diffuseTexture";
		
		m_pModel = m_pScene->CreateDml( "../resources/art/role/Link/female/4110/4110001001/4110001.dml" );

		//创建新的贴图，将贴图设置给模型的diffuse贴图
		m_pTexture = Hippo_GetIRender()->CreateTextureFromFile("../data/universal/asset/RTTLUTTable.png");
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
		const char* varName = "TextureSample2D_3_TexFileName";
		const char* alias = "diffuseTexture";

		if (HippoTestManager::GetInstance()->GetCurrentRunMode())
		{
			switch(key)
			{
			case '1':
				{
						H3DI::INewShader* m_pNewShader = m_pModel->GetNewShaderByIndex(0);
						m_pNewShader->SetTexture(varName, m_pTexture);
						Hippo_GetIRender()->ClearMaterialLib();
					return 1;
				}
			case   VK_RETURN:
				return 1;
			}
		}
		return 0;
	}
};
ADD_TESTCASE("TextureShaderConstError",TextureShaderConstError);