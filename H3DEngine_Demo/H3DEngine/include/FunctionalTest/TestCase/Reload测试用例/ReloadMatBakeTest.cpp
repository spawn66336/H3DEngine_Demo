#include "../../HippoTestManager/HippoTestManager.h"
#include "../CommonCode/HippoUtil/ActorUtil.h"
#include "../CommonCode/HippoUtil/HippoScene.h"
#include "../CommonCode/HippoUtil/HippoGlobal.h"
#include "../CommonCode/HippoUtil/RefCountCompare.h"
#include "engine_interface.h"
#include "engine_interface_internal.h"
#include "../CommonCode/HippoUtil/HippoStateMachine.h"
#include "EffectCore_dx.h"
#include "../idMath/dMathHeader.h"
#include <algorithm>
#include <stdio.h>
#include "../../FunctionalTest/functional_test/HippoTestConfig.h"
using namespace  std;

/*�����߼�
�����決��ͼ���޵���
׼��һ����װ��CHR����CHR�Ĳ��ʵ�LOD2û�к決��ͼ��
���ش�CHR����������LOD2��Ч����
���Ӵ�CHR�ĺ決��ͼ��
����ģ�͵�ReloadMaterial������������LOD2��Ч�����鿴Ч���仯��
*/
class ReloadMatBake1 : public HippoTestCase_RefCompare
{
public:
	ReloadMatBake1(const char* casename):HippoTestCase_RefCompare(casename){}
	HippoLevelScene* m_pScene;
	H3DI::ISkeletonModel* m_pChr;
	Hippo_StateMachine stateMachine;
	bool bAutoEnd;

private:
	static HippoTestCaseBase* const testcase_;
public:

	int GetMatLod2(){return 2;}
	virtual void CreateChr()
	{
		m_pChr = m_pScene->CreateChr(Make_HippoTestResource_Path("Reload����������Դ/�決��ͼ����/114001001/114001_M.chr").c_str(),GetMatLod2());
		ChangeDiskTexlod2First();
	}
	 void ChangeDiskTexlod2First() //Ų�ߣ��൱��ɾ��
	{
		rename(Make_HippoTestResource_Path("Reload����������Դ/�決��ͼ����/114001001/Texture/114001001_d_LOD2.dds").c_str(),
			Make_HippoTestResource_Path("Reload����������Դ/�決��ͼ����/114001001/114001001_d_LOD2.dds").c_str());
	}
	 void ChangeDiskTexlod2Second()//�������ǰ �ָ�ԭ����Դ
	{
		rename(Make_HippoTestResource_Path("Reload����������Դ/�決��ͼ����/114001001/114001001_d_LOD2.dds").c_str(),
			Make_HippoTestResource_Path("Reload����������Դ/�決��ͼ����/114001001/Texture/114001001_d_LOD2.dds").c_str());
	}

	SCSTATE OnState0()
	{
		if (Spin_N_Ms(5000))
		{
			return STATE_CHANGE;
		}
		return STATE_NOT_CHANGE;
	}
	virtual SCSTATE OnState1()
	{
		m_pChr->ReloadAll();
		ChangeDiskTexlod2Second();
		bAutoEnd = true;
		return STATE_CHANGE;
	}


	virtual void InitScene()
	{
		bAutoEnd = false;
		m_pScene = Hippo_GetSceneManager()->CreateEmptyLevel();
		Hippo_GetIRender()->OpenActionLib(Make_X52Resources_Path("art/role/actions/role.xml").c_str());
		stateMachine.AddState(0,std::tr1::bind(&ReloadMatBake1::OnState0,this));
		stateMachine.AddState(1,std::tr1::bind(&ReloadMatBake1::OnState1,this));
		stateMachine.AddState(2,std::tr1::bind(&ReloadMatBake1::OnState0,this));
		CreateChr();
	}
	virtual void CleanUpScene()
	{
		m_pScene->CleanScene();
		Hippo_GetSceneManager()->DelScene(m_pScene);
	}

	virtual bool ManualRunExec()
	{ 
		stateMachine.Run();
		return false;
	}
	virtual bool AutoRunExec()
	{
		if (bAutoEnd)
		{
			return true;
		}
		return ManualRunExec();
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
ADD_TESTCASE("RELOAD_TESTCASE",ReloadMatBake1);

/*�����߼�
�����決��ͼ���е���
�����߼�ͬ ����1
*/
class ReloadMatBake2 : public ReloadMatBake1
{
public:
	ReloadMatBake2(const char* casename): ReloadMatBake1(casename){}
	virtual void CreateChr()
	{
		ChangeDiskTexlod2First();
		m_pChr = m_pScene->CreateChr(Make_HippoTestResource_Path("Reload����������Դ/�決��ͼ����/114001001/114001_M.chr").c_str(),GetMatLod2());
		
	}
	virtual SCSTATE OnState1()
	{
		ChangeDiskTexlod2Second();
		m_pChr->ReloadAll();
		
		bAutoEnd = true;
		return STATE_CHANGE;
	}
private:
	static HippoTestCaseBase* const testcase_;
};
ADD_TESTCASE("RELOAD_TESTCASE",ReloadMatBake2);