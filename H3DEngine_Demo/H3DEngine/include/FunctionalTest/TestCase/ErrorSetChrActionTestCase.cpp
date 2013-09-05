#include "../HippoTestManager/HippoTestManager.h"
#include "../CommonCode/HippoUtil/HippoScene.h"
#include "../CommonCode/HippoUtil/RefCountCompare.h"
#include "../CommonCode/HippoUtil/HippoGlobal.h"
#include <iostream>
#include "idMath/dMathHeader.h"
#include "engine_interface.h"
#include "../CommonCode/HippoUtil/ActorUtil.h"
#include "log/HippoLog.h"
#include <list>
using namespace std;



/*
测试逻辑：
1、创建chr模型
2、测试不同错误情况下，程序是否崩溃
3.这里是设置chr动作的测试；重点关注的是运行过程，顺序运行程序会测试不同的用例，并将结果打印在
  控制台和日志中，所以，重点查看日志，根据日志内容了解程序运行
*/

class ErrorSetChrActionTestCase : public HippoTestCaseBase
{
public:
	ErrorSetChrActionTestCase(const char* case_name):HippoTestCaseBase(case_name){}
	HippoLevelScene* m_pScene;
	list<string> chrPathList;
	H3DI::ISkeletonModel * m_pChrModel;
	H3DI::IAnimationChannel* channel;
	bool bAutoEnd;
private:
	static HippoTestCaseBase* const testcase_;

public:
	virtual void InitScene()
	{
		bAutoEnd = false;
		//这个判断主要针对错误用例---动作列表错误---
		Hippo_WriteConsoleAndLog(CC_WHITE,"动作列表错误用例");
		if (!Hippo_GetIRender()->OpenActionLib("../resources/art/role/actions/role.xml"))
		{
			Hippo_WriteConsoleAndLog(CC_WHITE,"加载role.xml文件失败");
			m_state=CASE_END_EXECING;
		}

		m_pScene = Hippo_GetSceneManager()->CreateEmptyLevel();


		//这里测试的是：动作文件不存在，动作文件数据错误，动作文件是一个fig的情况
		//只需要生成一个chr实例，对这个实例设置3个不同的动作，这3个动作分别对应上述3中错误

		Hippo_WriteConsoleAndLog(CC_WHITE,"这里测试的是：动作文件不存在，动作文件数据错误，动作文件是一个fig----begin");

		Hippo_GetIRender()->OpenActionLib("../x52_test_resources/错误用例测试资源/设置CHR动作出错/动作文件错误/动作文件不存在_数据不正确_只是一个fig/4115001001/role.xml");  
		m_pChrModel = m_pScene->CreateChr("../x52_test_resources/错误用例测试资源/设置CHR动作出错/动作文件错误/动作文件不存在_数据不正确_只是一个fig/4115001001/4115001.chr"); 		
		CreateResultForLog();
		channel = m_pChrModel->GetAnmChannel(0);
		channel->SetTransitionTimeFromCurrent(0.0f);
		channel->SetAction("SD_122BPM_huh005_01_1",true);
		channel->SetAction("SD_122BPM_huh005_02_1",true);
		channel->SetAction("SD_122BPM_huh005_03_1",true);
		Hippo_WriteConsoleAndLog(CC_WHITE,"这里测试的是：动作文件不存在，动作文件数据错误，动作文件是一个fig----end");

		/* 
		 这里是对骨骼文件错误的测试，这里要加载3次role文件，没加载一次都会生成一个chr实例
		 然后对这个实例SetAction，这 里使用到的3个动作文件对应的3个fig文件
		 分别：fig不存在，fig格式不正确，fig命名不规范
		*/
		
		Hippo_WriteConsoleAndLog(CC_WHITE,"这里测试的是：fig不存在，fig格式不正确，fig命名不规范----begin");


		TestFigFile("../x52_test_resources/错误用例测试资源/设置CHR动作出错/动作文件错误/NodeName与骨骼不对应/骨骼文件不存在/4115001001/role.xml"
			,"../x52_test_resources/错误用例测试资源/设置CHR动作出错/动作文件错误/NodeName与骨骼不对应/骨骼文件不存在/4115001001/4115001.chr"
			,"tpose_hand");

		TestFigFile("../x52_test_resources/错误用例测试资源/设置CHR动作出错/动作文件错误/NodeName与骨骼不对应/文件格式不正确/4115001001/role.xml"
			,"../x52_test_resources/错误用例测试资源/设置CHR动作出错/动作文件错误/NodeName与骨骼不对应/文件格式不正确/4115001001/4115001.chr"
			,"tpose_hand");

		TestFigFile("../x52_test_resources/错误用例测试资源/设置CHR动作出错/动作文件错误/NodeName与骨骼不对应/NodeName命名不规范/4115001001/role.xml"
			,"../x52_test_resources/错误用例测试资源/设置CHR动作出错/动作文件错误/NodeName与骨骼不对应/NodeName命名不规范/4115001001/4115001.chr"
			,"tpose_hand");

		Hippo_WriteConsoleAndLog(CC_WHITE,"这里测试的是：fig不存在，fig格式不正确，fig命名不规范----end");

		m_pScene->RestructScene();
		bAutoEnd = true;
	}

	bool TestFigFile(string rolePaht,string ChrPath,string actionId)
	{

		Hippo_GetIRender()->OpenActionLib(rolePaht.c_str()); 
		m_pChrModel = m_pScene->CreateChr(ChrPath.c_str()); 
		channel = m_pChrModel->GetAnmChannel(0); 
		channel->SetTransitionTimeFromCurrent(0.0f);
		channel->SetAction(actionId.c_str(),true);
		CreateResultForLog();

		return true;
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

	bool CreateResultForLog()
	{
		Hippo_WriteConsoleAndLog(CC_WHITE,m_pChrModel== NULL ? "创建chr失败" : "创建chr成功");
		return true;
	}


};

ADD_TESTCASE("ERROR_RESOURCE_TEST",ErrorSetChrActionTestCase);
