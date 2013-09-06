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
׼��һ��ʹ���˷�װ����ģ���DML�����в���ʹ�����Զ�LOD��
���ش�DML����������LOD1��Ч����
  ��Ⱦ5s
�ҵ���DMLʹ�õĲ���ʵ���ļ����޸Ĳ���ʵ���ļ��е��Զ�LOD1������
����ģ�͵�ReloadMaterial������������LOD1���鿴Ч���仯
  ��Ⱦ5s
*/

class ReloadMatLodTest1:public HippoTestCase_RefCompare
{
public:
	ReloadMatLodTest1(const char* casename):HippoTestCase_RefCompare(casename){}
	H3DI::IModel* m_pDml1;
	H3DI::IModel* m_pDml2;
	HippoLevelScene* m_pScene;
	Hippo_StateMachine stateMachine;
	bool bAutoEnd;

private:
	static HippoTestCaseBase* const testcase_;
public:
	int GetMatLod1(){return 1;}
	int GetMatLod2(){return 2;}
	virtual void CreateDml()
	{
		m_pDml1 = m_pScene->CreateDml(Make_HippoTestResource_Path("Reload����������Դ/LOD����/4015001001/4015001.dml").c_str(),GetMatLod1());
	}
	virtual void ChangeDiskMatFirst()
	{
		rename(Make_HippoTestResource_Path("Reload����������Դ/LOD����/4015001001/4015001001.mat").c_str(),
			Make_HippoTestResource_Path("Reload����������Դ/LOD����/4015001001/back.mat").c_str());
		rename(Make_HippoTestResource_Path("Reload����������Դ/LOD����/4015001001/test1.mat").c_str(),
			Make_HippoTestResource_Path("Reload����������Դ/LOD����/4015001001/4015001001.mat").c_str());
	}
	virtual void ChangeDiskMatSecond()
	{
		rename(Make_HippoTestResource_Path("Reload����������Դ/LOD����/4015001001/4015001001.mat").c_str(),
			Make_HippoTestResource_Path("Reload����������Դ/LOD����/4015001001/test1.mat").c_str());
		rename(Make_HippoTestResource_Path("Reload����������Դ/LOD����/4015001001/back.mat").c_str(),
			Make_HippoTestResource_Path("Reload����������Դ/LOD����/4015001001/4015001001.mat").c_str());
	}

	SCSTATE OnState0()
	{
		if (Spin_N_Ms(5000))
		{
			return STATE_CHANGE;
		}
		return STATE_NOT_CHANGE;
	}
	SCSTATE OnState1()
	{
		ChangeDiskMatFirst();
		m_pDml1->ReloadAll();
		ChangeDiskMatSecond();
		bAutoEnd = true;
		return STATE_CHANGE;
	}

	virtual void InitScene()
	{
		bAutoEnd = false;
		m_pScene = Hippo_GetSceneManager()->CreateEmptyLevel();

		stateMachine.AddState(0,std::tr1::bind(&ReloadMatLodTest1::OnState0,this));
		stateMachine.AddState(1,std::tr1::bind(&ReloadMatLodTest1::OnState1,this));
		stateMachine.AddState(2,std::tr1::bind(&ReloadMatLodTest1::OnState0,this));
		CreateDml();
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
ADD_TESTCASE("RELOAD_TESTCASE",ReloadMatLodTest1);


/*�����߼�
׼��һ��DML�����в���ʹ�����Զ�LOD��
���ش�DML����������LOD1��LOD2��Ч����
  ��Ⱦ5s
�ҵ���DMLʹ�õĲ���ʵ���ļ������Ӵ˲���ʵ�����ֶ�LOD1��LOD2��
����ģ�͵�ReloadMaterial������������LOD1��LOD2��Ч�����鿴Ч���仯��
  ��Ⱦ5s
*/
class ReloadMatLodTest2 : public ReloadMatLodTest1
{
public:
	ReloadMatLodTest2(const char* casename):ReloadMatLodTest1(casename){}
	virtual void CreateDml()
	{
		m_pDml1 = m_pScene->CreateDml(Make_HippoTestResource_Path("Reload����������Դ/LOD����/4015001001/4015001.dml").c_str(),GetMatLod1());
		m_pDml2 = m_pScene->CreateDml(Make_HippoTestResource_Path("Reload����������Դ/LOD����/4015001001/4015001.dml").c_str(),GetMatLod2());
		m_pDml1->SetPosition(H3DVec3(-3.0f,.0f,.0f));
		m_pDml2->SetPosition(H3DVec3(3.0f,.0f,.0f));

	}
	virtual void ChangeDiskMatFirst()
	{
		rename(Make_HippoTestResource_Path("Reload����������Դ/LOD����/4015001001/4015001001.mat").c_str(),
			Make_HippoTestResource_Path("Reload����������Դ/LOD����/4015001001/back.mat").c_str());
		rename(Make_HippoTestResource_Path("Reload����������Դ/LOD����/4015001001/test2.mat").c_str(),
			Make_HippoTestResource_Path("Reload����������Դ/LOD����/4015001001/4015001001.mat").c_str());
	}
	virtual void ChangeDiskMatSecond()
	{
		rename(Make_HippoTestResource_Path("Reload����������Դ/LOD����/4015001001/4015001001.mat").c_str(),
			Make_HippoTestResource_Path("Reload����������Դ/LOD����/4015001001/test2.mat").c_str());
		rename(Make_HippoTestResource_Path("Reload����������Դ/LOD����/4015001001/back.mat").c_str(),
			Make_HippoTestResource_Path("Reload����������Դ/LOD����/4015001001/4015001001.mat").c_str());
	}
private:
	static HippoTestCaseBase* const testcase_;
};
ADD_TESTCASE("RELOAD_TESTCASE",ReloadMatLodTest2);

/*�����߼�
׼��һ��DML�����в���ʹ�����ֶ�LOD1���ֶ�LOD2��
���ش�DML����������LOD1��LOD2��Ч����
  ��Ⱦ5s
�ҵ���DMLʹ�õĲ���ʵ���ļ����޸��ֶ�LOD1��LOD2�Ĳ���ʵ��������Value��
����ģ�͵�ReloadMaterial������������LOD1��LOD2��Ч�����鿴Ч���仯��
  ��Ⱦ5s
*/

class ReloadMatLodTest3 : public ReloadMatLodTest1
{
public:
	ReloadMatLodTest3(const char* casename):ReloadMatLodTest1(casename){}
	virtual void CreateDml()
	{
		m_pDml1 = m_pScene->CreateDml(Make_HippoTestResource_Path("Reload����������Դ/LOD����/4015001002/4015001.dml").c_str(),GetMatLod1());
		m_pDml2 = m_pScene->CreateDml(Make_HippoTestResource_Path("Reload����������Դ/LOD����/4015001002/4015001.dml").c_str(),GetMatLod2());
		m_pDml1->SetPosition(H3DVec3(-3.0f,.0f,.0f));
		m_pDml2->SetPosition(H3DVec3(3.0f,.0f,.0f));

	}
	virtual void ChangeDiskMatFirst()
	{
		rename(Make_HippoTestResource_Path("Reload����������Դ/LOD����/4015001002/4015001001.mat").c_str(),
			Make_HippoTestResource_Path("Reload����������Դ/LOD����/4015001002/back.mat").c_str());
		rename(Make_HippoTestResource_Path("Reload����������Դ/LOD����/4015001002/changed.mat").c_str(),
			Make_HippoTestResource_Path("Reload����������Դ/LOD����/4015001002/4015001001.mat").c_str());
	}
	virtual void ChangeDiskMatSecond()
	{
		rename(Make_HippoTestResource_Path("Reload����������Դ/LOD����/4015001002/4015001001.mat").c_str(),
			Make_HippoTestResource_Path("Reload����������Դ/LOD����/4015001002/changed.mat").c_str());
		rename(Make_HippoTestResource_Path("Reload����������Դ/LOD����/4015001002/back.mat").c_str(),
			Make_HippoTestResource_Path("Reload����������Դ/LOD����/4015001002/4015001001.mat").c_str());
	}
private:
	static HippoTestCaseBase* const testcase_;
};
ADD_TESTCASE("RELOAD_TESTCASE",ReloadMatLodTest3);

/*�����߼�
׼��һ��DML�����в���ʹ�����ֶ�LOD1���ֶ�LOD2��
���ش�DML����������LOD1��LOD2��Ч����
  ��Ⱦ5s
  �ҵ���DMLʹ�õĲ���ʵ���ļ������ֶ�LODɾ������Ϊ�Զ�LOD��
  ����ģ�͵�ReloadMaterial������������LOD1��LOD2��Ч�����鿴Ч���仯��
  ��Ⱦ5s

  ����ͬ��
*/
