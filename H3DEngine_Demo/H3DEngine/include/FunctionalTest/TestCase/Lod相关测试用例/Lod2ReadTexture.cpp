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

/*�����߼�
����1��dmlģ�ͣ�Ҫ���dmlģ�͵Ĳ���ʹ��dsn3����ͼ�����ҽ����Զ�lod��
����1��chrģ�ͣ�Ҫ���chrģ�͵Ĳ�����mask���ʣ�ʹ��dsn3����ͼ���ҽ����Զ�lod��
��dml��chr��lod0����ʽ���������ơ���¼��ǰ��������������ͼ������
��dml��chr�л���lod2��ִ��clearmatlib����¼��ǰ��������������ͼ������
��ʱ��ͼ����Ӧ�ü���4��
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

		//����1��dmlģ��  ����1��chrģ��
		H3DVec3 posDml(-1.5f,0.f,0.f);
		H3DVec3 posChr(1.5f,0.f,0.f);
		m_pDml = m_pScene->CreateDml(Make_HippoTestResource_Path("lod2��ȡ��ͼ��������/dmlģ�͵Ĳ���ʹ��dsn3����ͼ�����ҽ����Զ�lod2/Sphere01.dml").c_str(),0);
		m_Chr = m_pScene->CreateChr(Make_HippoTestResource_Path("lod2��ȡ��ͼ��������/chrģ�͵Ĳ�����mask���ʣ�ʹ��dsn3����ͼ���ҽ����Զ�lod/114001_M.chr").c_str(),0);
		m_pDml->SetPosition(posDml);
		m_Chr->SetPosition(posChr);

		//��¼��ǰ��������������ͼ������
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


/*��ע
�ò�������ͨ��
*/