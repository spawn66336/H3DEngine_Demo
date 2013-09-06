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

/*�����߼�
������Դ���ü�������
׼��1��dml��Ҫ�������ʹ��1����ͼ����ͼ�����б���
����һ����ͼ�õ�itexture,�����ø�dml��diffuse��ͼ
��ʱ��diffuse��ͼ��ԭ����diffuse��ͼӦ�ò���һ��ָ��
ִ��clearmatlib
Ӧ�����б����޷����ҵ���ǰdml��diffuse��ͼ
�ͷų�����������Դ���ü�������
�Ա��������ü��������Ƿ��в�ͬ
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

		//�����µ���ͼ������ͼ���ø�ģ�͵�diffuse��ͼ
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