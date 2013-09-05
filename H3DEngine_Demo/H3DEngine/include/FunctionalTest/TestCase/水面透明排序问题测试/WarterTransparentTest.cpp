#include "../../HippoTestManager/HippoTestManager.h"
#include "../CommonCode/HippoUtil/ActorUtil.h"
#include "../CommonCode/HippoUtil/HippoScene.h"
#include "../CommonCode/HippoUtil/HippoGlobal.h"
#include "../CommonCode/HippoUtil/RefCountCompare.h"
#include "engine_interface.h"
#include "engine_interface_internal.h"
#include "../CommonCode/HippoUtil/HippoStateMachine.h"
#include "../idMath/dMathHeader.h"
#include "EffectCore_dx.h"
#include "../../functional_test/HippoTestConfig.h"

using namespace  std;
/*测试逻辑
准备物体A（sortkey=10）、B（sortkey=60）、C（sortkey=100）、D（sortkey=101）、E（sortkey=300）、F（sortkey=301）、G（sortkey=500）。
以LOD0加载并绘制以上物体。
物体的绘制顺序必然是A、B、C、D、E、F、G
将所有物体切换到LOD1
物体的绘制顺序必然是A、B、C、D、E、F、G
将所有物体切换到LOD2
物体的绘制顺序必然是A、B、C、D、E、F、G
*/
namespace H3DI
{
	class IDrawEventListener;
	class IRenderable;
}

class CustomDrawEventCallback:public H3DI::IDrawEventListener
{
public:
	void Release(){delete this;}
	void OnFrameBegin(){}
	void OnFrameEnd(){}
	void OnDrawOP(const H3DI::IRenderable* p)
	{

	}
};


class WaterTransparentTest1 : public HippoTestCase_RefCompare,public H3DI::IDrawEventListener 
{
public:
	WaterTransparentTest1(const char* casename):HippoTestCase_RefCompare(casename){}
	HippoLevelScene* m_pScene;
	std::vector<H3DI::IRenderableProxy*> m_draw_renderables;
	bool bAutoEnd;
	std::vector<H3DI::INewShader*> m_all_modelsd;

private:
	static HippoTestCaseBase* const testcase_;
public:
	void Release(){delete this;}
	void OnFrameBegin()
	{
		auto itr=m_draw_renderables.begin();
		auto itrend=m_draw_renderables.end();
		while (itr!=itrend)
		{
			H3DI::IRenderableProxy* p=*itr;
			p->Release();
			++itr;
		}
		m_draw_renderables.clear();
	}
	void OnFrameEnd()
	{
		unsigned int sz1=m_draw_renderables.size();
		unsigned int sz2=m_all_modelsd.size();
		HIPPO_EXPECT_EQ(sz1,sz2);

		//验证绘制顺序
		unsigned int idx=0;
		auto itr=m_draw_renderables.begin();
		auto itrend=m_draw_renderables.end();
		while (itr!=itrend)
		{
			H3DI::IRenderableProxy* p=*itr;
			H3DI::INewShader* psd=p->GetNewShader();
			H3DI::INewShader* psd1=m_all_modelsd.at(idx);
			HIPPO_EXPECT_EQ(psd,psd1);
			++itr;
			++idx;
		}
	}

	void OnDrawOP(H3DI::IRenderable* p)
	{
		H3DI::IRenderableProxy* proxy=Hippo_GetIGlobalEnv()->CreateRenderableProxy(p);
		m_draw_renderables.push_back(proxy);
	}

	void InitModelShader(H3DI::IModel *m_pDmlA)
	{
		int num=m_pDmlA->GetNumSubModel();
		for (int i=0;i<num;++i)
		{
			H3DI::INewShader* psd=m_pDmlA->GetNewShaderByIndex(i);
			m_all_modelsd.push_back(psd);
		}
	}
	virtual void InitScene()
	{
		Hippo_GetIRender()->OnCommandLine("SetEngineConfig UseFrameProfile true");
		Hippo_GetIRender()->OnCommandLine("SetEngineConfig EnableLpp false");

		Hippo_GetIGlobalEnv()->GetDataStatisitcs()->Register_ForwardShadingDrawEvent_Listener((H3DI::IDrawEventListener*)this);
		m_pScene = Hippo_GetSceneManager()->CreateEmptyLevel();
		bAutoEnd = false;
		//以LOD0加载并绘制物体。
		H3DI::IModel *m_pDmlA = m_pScene->CreateDml(Make_HippoTestResource_Path("水面透明排序问题测试资源/用例1/4114001001/4114001.dml").c_str(),0);
		H3DI::IModel *m_pDmlB = m_pScene->CreateDml(Make_HippoTestResource_Path("水面透明排序问题测试资源/用例1/4114001002/4114001.dml").c_str(),0);
		H3DI::IModel *m_pDmlC = m_pScene->CreateDml(Make_HippoTestResource_Path("水面透明排序问题测试资源/用例1/4114004001/4114004.dml").c_str(),0);
		H3DI::IModel *m_pDmlD = m_pScene->CreateDml(Make_HippoTestResource_Path("水面透明排序问题测试资源/用例1/4114004002/4114004.dml").c_str(),0);
		H3DI::IModel *m_pDmlE = m_pScene->CreateDml(Make_HippoTestResource_Path("水面透明排序问题测试资源/用例1/4114004003/4114004.dml").c_str(),0);
		H3DI::IModel *m_pDmlF = m_pScene->CreateDml(Make_HippoTestResource_Path("水面透明排序问题测试资源/用例1/4114007001/4114007.dml").c_str(),0);
		H3DI::IModel *m_pDmlG = m_pScene->CreateDml(Make_HippoTestResource_Path("水面透明排序问题测试资源/用例1/4114007002/4114007.dml").c_str(),0);

		InitModelShader(m_pDmlA);
		InitModelShader(m_pDmlB);
		InitModelShader(m_pDmlC);
		InitModelShader(m_pDmlD);
		InitModelShader(m_pDmlE);
		InitModelShader(m_pDmlF);
		InitModelShader(m_pDmlG);

		m_pDmlA->SetPosition(H3DVec3(-1.5f,0.0f,3.0f));
		m_pDmlB->SetPosition(H3DVec3(0.0f,0.0f,3.0f));
		m_pDmlC->SetPosition(H3DVec3(1.5f,0.0f,3.0f));
		m_pDmlD->SetPosition(H3DVec3(-1.5f,0.f,1.5f));
		m_pDmlE->SetPosition(H3DVec3(0.0f,0.f,1.5f));
		m_pDmlF->SetPosition(H3DVec3(1.5f,0.f,1.5f));

		m_pScene->RestructScene();

		////将所有物体切换到LOD1
		//m_pDmlA->SetMaterialLod(1);
		//m_pDmlB->SetMaterialLod(1);
		//m_pDmlC->SetMaterialLod(1);
		//m_pDmlD->SetMaterialLod(1);
		//m_pDmlE->SetMaterialLod(1);
		//m_pDmlF->SetMaterialLod(1);
		//m_pDmlG->SetMaterialLod(1);

		////将所有物体切换到LOD2
		//m_pDmlA->SetMaterialLod(2);
		//m_pDmlB->SetMaterialLod(2);
		//m_pDmlC->SetMaterialLod(2);
		//m_pDmlD->SetMaterialLod(2);
		//m_pDmlE->SetMaterialLod(2);
		//m_pDmlF->SetMaterialLod(2);
		//m_pDmlG->SetMaterialLod(2);
		
		bAutoEnd = true;
	}

	virtual void CleanUpScene()
	{
		Hippo_GetIGlobalEnv()->GetDataStatisitcs()->UnRegister_ForwardShadingDrawEvent_Listener((H3DI::IDrawEventListener*)this);

		m_pScene->CleanScene();
		Hippo_GetSceneManager()->DelScene(m_pScene);
	}

	virtual bool ManualRunExec()
	{
		return false;
	}
	virtual bool AutoRunExec()
	{
		if (bAutoEnd)
		{
			return true;
		}
		return false;
	}
	virtual int OnKeyDown(unsigned int key)
	{
		if (HippoTestManager::GetInstance()->GetCurrentRunMode())
		{
			switch(key)
			{
			case  VK_RETURN:
				m_state = CASE_END_EXECING;
				return 0;
			}
		}
		return -1;
	}
};
ADD_TESTCASE("WATER_TRANSPARENT_SORT_TEST",WaterTransparentTest1);

/*测试逻辑
准备物体A（sortkey=10，但是是后处理后绘制）、B（sortkey=60，但是是景深后绘制）、C（sortkey=100）、D（sortkey=101，但是是后处理后绘制）、E（sortkey=300）、F（sortkey=301，但是是景深后绘制）、G（sortkey=500）
以LOD0加载并打开景深，绘制以上物体。
物体的绘制顺序必然是C、E、G、B、F、A、D
关闭景深，保留tonemapping
物体的绘制顺序必然是B、C、E、F、G、A、D
关闭所有后处理
物体的绘制顺序必然是A、B、C、D、E、F、G
*/

class WaterTransparentTest2 : public WaterTransparentTest1
{
public:
	WaterTransparentTest2(const char* casename):WaterTransparentTest1(casename){}
private:
	static HippoTestCaseBase* const testcase_;
public:
	virtual void InitScene()
	{


		m_pScene = Hippo_GetSceneManager()->CreateEmptyLevel();
		bAutoEnd = false;
		//	以LOD0加载并打开景深，绘制以上物体

		Hippo_GetIRender()->OnCommandLine("SetEngineConfig usePostProcess true");
		m_pScene->GetH3DScene()->AddPostProcess("..\\data\\product\\postprocess\\ue3_uber.xml");

		H3DI::IModel *m_pDmlA = m_pScene->CreateDml(Make_HippoTestResource_Path("水面透明排序问题测试资源/用例2/4114001001/4114001.dml").c_str(),0);
		H3DI::IModel *m_pDmlB = m_pScene->CreateDml(Make_HippoTestResource_Path("水面透明排序问题测试资源/用例2/4114001002/4114001.dml").c_str(),0);
		H3DI::IModel *m_pDmlC = m_pScene->CreateDml(Make_HippoTestResource_Path("水面透明排序问题测试资源/用例2/4114004001/4114004.dml").c_str(),0);
		H3DI::IModel *m_pDmlD = m_pScene->CreateDml(Make_HippoTestResource_Path("水面透明排序问题测试资源/用例2/4114004002/4114004.dml").c_str(),0);
		H3DI::IModel *m_pDmlE = m_pScene->CreateDml(Make_HippoTestResource_Path("水面透明排序问题测试资源/用例2/4114004003/4114004.dml").c_str(),0);
		H3DI::IModel *m_pDmlF = m_pScene->CreateDml(Make_HippoTestResource_Path("水面透明排序问题测试资源/用例2/4114007001/4114007.dml").c_str(),0);
		H3DI::IModel *m_pDmlG = m_pScene->CreateDml(Make_HippoTestResource_Path("水面透明排序问题测试资源/用例2/4114007002/4114007.dml").c_str(),0);

		m_pDmlA->SetPosition(H3DVec3(-1.5f,0.0f,3.0f));
		m_pDmlB->SetPosition(H3DVec3(0.0f,0.0f,3.0f));
		m_pDmlC->SetPosition(H3DVec3(1.5f,0.0f,3.0f));
		m_pDmlD->SetPosition(H3DVec3(-1.5f,0.f,1.5f));
		m_pDmlE->SetPosition(H3DVec3(0.0f,0.f,1.5f));
		m_pDmlF->SetPosition(H3DVec3(1.5f,0.f,1.5f));

		m_pScene->RestructScene();

		//关闭景深，保留tonemapping
		m_pScene->GetH3DScene()->DelPostProcess(NULL);

		//	关闭所有后处理
		Hippo_GetIRender()->OnCommandLine("SetEngineConfig usePostProcess false");

		bAutoEnd = true;
	}
};
ADD_TESTCASE("WATER_TRANSPARENT_SORT_TEST",WaterTransparentTest2);