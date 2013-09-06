#include "../../HippoTestManager/HippoTestManager.h"
#include "../CommonCode/HippoUtil/HippoGlobal.h"
#include "../CommonCode/HippoUtil/FileOperation.h"
#include "EffectCore_dx.h"
#include "engine_interface.h"
#include "idMath/dMathHeader.h"
#include "../../FunctionalTest/stress_test/HippoTestConfig.h"
#include "GenPathList.h"
#include "middle_layer_interface.h"
#include "../CommonCode/HippoUtil/camera/CameraBase.h"
#include <time.h>
#include <algorithm>
#include "../RandomUtil.h"
using namespace  std;
#pragma comment(lib, "winmm.lib")

namespace H3DI
{
	class IMoveObject;
	class IPrePassLight;
	class IActor;
	class IAvatarSkeletonModel;
}

const int FileLength=100;
const int ModelNum=50;
const int ModelNameLength=64;

//获得两帧的时间间隔
float GetEscapeTime()
{
	static DWORD last_t=0;
	static DWORD current_t=0;
	last_t=current_t;
	current_t=::timeGetTime();
	return (float)(current_t-last_t);
}
//某指针是否在容器中
int IsInVector(std::vector<AvatarEntity*>* vec,AvatarEntity* avatar)
{
	int idx=0;
	std::vector<AvatarEntity*>::iterator itr=vec->begin();
	for (;itr!=vec->end();itr++,idx++)
	{
		if (*itr == avatar)
			return idx;
	}
	return -1;
}

//判断是否是有效宠物bpt
bool IsValidPetBpt(std::string fileName,int& bptPos)
{
	std::string sub=fileName.substr(2,1);
	if (sub==std::string("4"))
	{
		bptPos=1;
		return true;
	}
	else if (sub==std::string("2"))
	{
		bptPos=0;
		return true;
	}
	else
	{
		return false;
	}
}

//获得随机位置
H3DVec3 GetRandomPositon()
{
	return H3DVec3(30,90,0) + H3DVec3(GetRandomInt(0,50)/4.f-6.25f, GetRandomInt(0,50)/4.f-6.25f, 0.f);
}

//随机性别
bool IsRandomMale()
{
	return GetRandomInt(0,1)==0?false:true;
}



/*测试逻辑
用例10（使用中间层）：
同步加载一个场景
创建50个人物lod0
创建50个宠物lod0
50个人物异步随机换装，换挂件
50个宠物异步随机换装，换挂件
*/
class StressTest_MidLayer_10:public HippoTestCase_RefCompare
{
public:
	StressTest_MidLayer_10(const char* casename):HippoTestCase_RefCompare(casename){}
	
	virtual void InitScene()
	{
		srand((unsigned)time(NULL));
		m_Render = Hippo_GetIRender();

		m_StageName = "Leisure";
		
		InitResourcesList();
		InitStage();
		CreateModel();
	}

	void InitStage()
	{
		m_Render->Exec(16,0,0,0,0);
		m_Render->SetFavorBinary(true);
		m_Render->OnCommandLine("SetEngineConfig OpenPhysX true1");

		char  stageFile[FileLength],staticObjectFile[FileLength],envFile[FileLength];
		sprintf(stageFile,"../resources/stage/%s/client/%s.xml",m_StageName.c_str(),m_StageName.c_str());
		sprintf(staticObjectFile,"../resources/stage/%s/ActorInfo.xml",m_StageName.c_str());
		sprintf(envFile,"../resources/stage/%s.xml",m_StageName.c_str());

		m_Stage = new Stage;
		m_Stage->initialize(m_Render,0,0);
		m_Player=dynamic_cast<StageAnimationPlayer*>(m_Stage->getPlayer());

		m_Stage->loadStageFile(stageFile,false);
		m_Player->loadStaticObject(staticObjectFile,false);
		m_Player->loadEnvironmentFile(envFile,false);
		m_Level=m_Stage->getActorMng()->get_level_entity();
		m_Factory=get_global_entity_factory(m_Render);

		CameraBase* pCam = HippoTestManager::GetInstance()->GetCamera();
		H3DVec3 CamPos(30.f,70.0f, 10.0f);
		H3DVec3 CamLook(30.f,90.f, 0.f);
		pCam->SetPos(CamPos);
		pCam->SetLookAtPos(CamLook);
	}

	virtual void InitResourcesList()
	{
		std::string LinkF=Make_X52Resources_Path("art/role/link/female").c_str();
		std::string LinkM=Make_X52Resources_Path("art/role/link/male").c_str();
		std::string BptF=Make_X52Resources_Path("art/role/bodypart/female").c_str();
		std::string BptM=Make_X52Resources_Path("art/role/bodypart/male").c_str();
		std::string LinkPetF=Make_X52Resources_Path("art/role/Pet/link/female").c_str();
		std::string LinkPetM=Make_X52Resources_Path("art/role/Pet/link/male").c_str();
		std::string BptPetF=Make_X52Resources_Path("art/role/Pet/female").c_str();
		std::string BPtPetM=Make_X52Resources_Path("art/role/Pet/male").c_str();

		FindFileRecursion(&m_LinkFList,LinkF.c_str(),"*.spe");
		FindFileRecursion(&m_LinkMList,LinkM.c_str(),"*.spe");
		FindFileRecursion(&m_BptFList,BptF.c_str(),"*.bpt");
		FindFileRecursion(&m_BptMList,BptM.c_str(),"*.bpt");
		FindFileRecursion(&m_LinkPetFList,LinkPetF.c_str(),"*.spe");
		FindFileRecursion(&m_LinkPetMList,LinkPetM.c_str(),"*.spe");
		FindFileRecursion(&m_BptPetFList,BptPetF.c_str(),"*.bpt");
		FindFileRecursion(&m_BptPetMList,BPtPetM.c_str(),"*.bpt");

	}

	void CreateModel()
	{
		//push attach
		for (int i=0;i<ModelNum;i++)
		{
			bool bmale=IsRandomMale();
			char nameAvatar[ModelNameLength] = {0};
			sprintf(nameAvatar, "avatar_%d",i);
			AvatarEntity* avatar_entity = m_Factory->create_avatar_entity(bmale,nameAvatar,0);
			if (bmale)
			{
				m_AvatarCon_Male.push_back(avatar_entity);
			}
			else
			{
				m_AvatarCon_Female.push_back(avatar_entity);
			}
			avatar_entity->set_transition_time_from_current(0, 0.3);
			avatar_entity->set_fixfoot_transmode(0, false);
			avatar_entity->set_position(GetRandomPositon());
			m_Level->add_to_level(avatar_entity);
			avatar_entity->update_appearance();

			bmale=IsRandomMale();
			char namePet[ModelNameLength] = {0};
			sprintf(namePet, "avatarskel_%d",i);
			CAvatarSkeletonModelEntity* avatar_skel_entity=m_Factory->create_avatar_skeleton_model_entity(bmale,namePet,0);
			if (bmale)
			{
				m_AvaSkelCon_Boy.push_back(avatar_skel_entity);
			}
			else
			{
				m_AvaSkelCon_Girl.push_back(avatar_skel_entity);
			}
			avatar_skel_entity->set_position(GetRandomPositon());
			avatar_skel_entity->set_fixfoot_transmode(0, false);
			m_Level->add_to_level(avatar_skel_entity);
			avatar_skel_entity->update_appearance();
		}
		Hippo_WriteConsole(CC_WHITE,"创建50个人物，50个宠物");
	}

	void RandomChangeBptAdorment()//随机换装 挂件,人物 宠物 也随机
	{
	
		{	//avatar
			bool bmale=IsRandomMale();//random sex
			AvatarRandomBpt(bmale);
			bmale=IsRandomMale();
			AvatarRandomLink(bmale);
		}
		
		{	//avatarskeleton
			bool bboy=IsRandomMale();
			AvatarSkelRandomBpt(bboy);
			bboy=IsRandomMale();
			AvatarSkelRandomLink(bboy);
		}
	}

	void AvatarRandomBpt(bool bmale)
	{
		if (bmale)
		{
			int idx=GetRandomInt(0,m_AvatarCon_Male.size()-1);
			AvatarEntity* avatar=m_AvatarCon_Male.at(idx);
			int bptIdx=GetRandomInt(0,m_BptMList.size()-1);
			std::string bptMale=m_BptMList.at(bptIdx);
			Hippo_WriteConsoleAndLog(CC_WHITE,"换装%s",bptMale.c_str());
			std::string fileName;
			GetFileName(bptMale,fileName);

			if (fileName.size()<9)//如果名字不符合规范，忽略
			{
				return ;
			}

			int bptPos = atoi(fileName.substr(2, 1).c_str());
			avatar->appearance_priority(1);
			avatar->set_body_part(bptPos,bptMale,false);
			avatar->update_appearance();
		}
		else
		{
			int idx=GetRandomInt(0,m_AvatarCon_Female.size()-1);
			AvatarEntity* avatar=m_AvatarCon_Female.at(idx);
			int bptIdx=GetRandomInt(0,m_BptFList.size()-1);
			std::string bptFemale=m_BptFList.at(bptIdx);
			Hippo_WriteConsoleAndLog(CC_WHITE,"换装%s",bptFemale.c_str());
			std::string fileName;
			GetFileName(bptFemale,fileName);

			if (fileName.size()<9)//如果名字不符合规范，忽略
			{
				return ;
			}

			int bptPos = atoi(fileName.substr(2, 1).c_str());
			avatar->appearance_priority(1);
			avatar->set_body_part(bptPos,bptFemale,false);
			avatar->update_appearance();
		}
	}

	void AvatarRandomLink(bool bmale)
	{

		if (bmale)
		{
			int idx=GetRandomInt(0,m_AvatarCon_Male.size()-1);
			AvatarEntity* avatar=m_AvatarCon_Male.at(idx);
			int linkIdx=GetRandomInt(0,m_LinkMList.size()-1);
			std::string linkMale=m_LinkMList.at(linkIdx);
			Hippo_WriteConsoleAndLog(CC_WHITE,"换挂件%s",linkMale.c_str());
			std::string fileName;
			GetFileName(linkMale,fileName);
			if (fileName.size()<9)//如果名字不符合规范，忽略
			{
				return ;
			}
			int linkPos = atoi(fileName.substr(2, 2).c_str()) - 10;
			avatar->appearance_priority(1);
			avatar->setAdornment(linkPos,linkMale.c_str(),false);
			avatar->update_appearance();
		}
		else
		{
			int idx=GetRandomInt(0,m_AvatarCon_Female.size()-1);
			AvatarEntity* avatar=m_AvatarCon_Female.at(idx);
			int linkIdx=GetRandomInt(0,m_LinkFList.size()-1);
			std::string linkFemale=m_LinkFList.at(linkIdx);
			Hippo_WriteConsoleAndLog(CC_WHITE,"换挂件%s",linkFemale.c_str());
			std::string fileName;
			GetFileName(linkFemale,fileName);
			if (fileName.size()<9)//如果名字不符合规范，忽略
			{
				return ;
			}
			int linkPos = atoi(fileName.substr(2, 2).c_str())-10;
			avatar->appearance_priority(1);
			avatar->setAdornment(linkPos,linkFemale.c_str(),false);
			avatar->update_appearance();
		}
	}

	void AvatarSkelRandomBpt(bool bboy)
	{
		if (bboy)
		{
			int idx=GetRandomInt(0,m_AvaSkelCon_Boy.size()-1);
			CAvatarSkeletonModelEntity* avatarskel=m_AvaSkelCon_Boy.at(idx);
			int bptIdx=GetRandomInt(0,m_BptPetMList.size()-1);
			std::string bptBoy=m_BptPetMList.at(bptIdx);
			Hippo_WriteConsoleAndLog(CC_WHITE,"宠物换装%s",bptBoy.c_str());
			std::string fileName;
			GetFileName(bptBoy,fileName);

			int bptPos ;
			if (!IsValidPetBpt(fileName,bptPos))
				return ;
			
			avatarskel->set_body_part(bptPos,bptBoy,false);
			avatarskel->update_appearance();
		}
		else
		{
			int idx=GetRandomInt(0,m_AvaSkelCon_Girl.size()-1);
			CAvatarSkeletonModelEntity* avatarskel=m_AvaSkelCon_Girl.at(idx);
			int bptIdx=GetRandomInt(0,m_BptPetFList.size()-1);
			std::string bptGirl=m_BptPetFList.at(bptIdx);
			Hippo_WriteConsoleAndLog(CC_WHITE,"宠物换装%s",bptGirl.c_str());
			std::string fileName;
			GetFileName(bptGirl,fileName);

			int bptPos ;
			if (!IsValidPetBpt(fileName,bptPos))
				return ;

			avatarskel->set_body_part(bptPos,bptGirl,false);
			avatarskel->update_appearance();
		}
	}

	void AvatarSkelRandomLink(bool bboy)
	{
		if (bboy)
		{
			int idx=GetRandomInt(0,m_AvaSkelCon_Boy.size()-1);
			CAvatarSkeletonModelEntity* avatarskel=m_AvaSkelCon_Boy.at(idx);
			int linkIdx=GetRandomInt(0,m_LinkPetMList.size()-1);
			std::string linkBoy=m_LinkPetMList.at(linkIdx);
			Hippo_WriteConsoleAndLog(CC_WHITE,"宠物换挂件%s",linkBoy.c_str());
			std::string fileName;
			GetFileName(linkBoy,fileName);
			if (fileName.size()<10)//如果名字不符合规范，忽略
			{
				return ;
			}
			int linkPos = atoi(fileName.substr(2, 2).c_str()) - 10;
			avatarskel->setAdornment(linkPos,linkBoy.c_str(),false);
			avatarskel->update_appearance();
		}
		else
		{
			int idx=GetRandomInt(0,m_AvaSkelCon_Girl.size()-1);
			CAvatarSkeletonModelEntity* avatarskel=m_AvaSkelCon_Girl.at(idx);
			int linkIdx=GetRandomInt(0,m_LinkPetFList.size()-1);
			std::string linkGirl=m_LinkPetFList.at(linkIdx);
			Hippo_WriteConsoleAndLog(CC_WHITE,"宠物换挂件%s",linkGirl.c_str());
			std::string fileName;
			GetFileName(linkGirl,fileName);
			int linkPos = atoi(fileName.substr(2, 2).c_str()) - 10;
			if (fileName.size()<10)//如果名字不符合规范，忽略
			{
				return ;
			}
			avatarskel->setAdornment(linkPos,linkGirl.c_str(),false);
			avatarskel->update_appearance();
		}
	}

	virtual void ReleaseModel()
	{
		for(vector<AvatarEntity*>::iterator itr = m_AvatarCon_Female.begin(); itr != m_AvatarCon_Female.end(); ++itr)
		{
			if ((*itr)->is_added_to_level() && m_Level)
				m_Level->remove_from_level((*itr));
			m_Factory->release((*itr));
		}
		m_AvatarCon_Female.clear();

		for(vector<AvatarEntity*>::iterator itr = m_AvatarCon_Male.begin(); itr != m_AvatarCon_Male.end(); ++itr)
		{
			if ((*itr)->is_added_to_level() && m_Level)
				m_Level->remove_from_level((*itr));
			m_Factory->release((*itr));
		}
		m_AvatarCon_Female.clear();


		for(vector<CAvatarSkeletonModelEntity*>::iterator itr = m_AvaSkelCon_Girl.begin(); itr != m_AvaSkelCon_Girl.end(); ++itr)
		{
			if ((*itr)->is_added_to_level() && m_Level)
				m_Level->remove_from_level((*itr));
			m_Factory->release((*itr));
		}
		m_AvaSkelCon_Girl.clear();

		for(vector<CAvatarSkeletonModelEntity*>::iterator itr = m_AvaSkelCon_Boy.begin(); itr != m_AvaSkelCon_Boy.end(); ++itr)
		{
			if ((*itr)->is_added_to_level() && m_Level)
				m_Level->remove_from_level((*itr));
			m_Factory->release((*itr));
		}
		m_AvaSkelCon_Boy.clear();
	}

	void UpdateCamera()
	{
		CameraBase* pCam = HippoTestManager::GetInstance()->GetCamera();
		H3DVec3 CamPos=pCam->GetPos();
		H3DVec3 CamLook=pCam->GetViewAtDir() + CamPos; 
		H3DVec3 CamUp=pCam->GetUp();
		m_Level->get_ILevel()->SetCurrentCamera(m_Render);

		float fov,ratio,fNear,fFar;
		m_Render->GetFrustum(fov,ratio,fNear,fFar,H3DI::ECT_Render);
		m_Render->LookAt(CamPos,CamLook,CamUp);
		m_Render->UpdateCamera();

		m_Level->get_ILevel()->SetFrustum(fov,ratio,fNear,fFar);
		m_Level->get_ILevel()->LookAt(CamPos,CamLook,CamUp);
		m_Level->get_ILevel()->UpdateCamera();
	}

	virtual void UpdateModel(float t)
	{
		//更新
		if (m_Player)
		{
			m_Player->update(t);
		}

		for(vector<AvatarEntity*>::iterator itr = m_AvatarCon_Female.begin(); itr != m_AvatarCon_Female.end(); ++itr)
		{
			(*itr)->update(t);
		}

		for(vector<AvatarEntity*>::iterator itr = m_AvatarCon_Male.begin(); itr != m_AvatarCon_Male.end(); ++itr)
		{
			(*itr)->update(t);
		}


		for(vector<CAvatarSkeletonModelEntity*>::iterator itr = m_AvaSkelCon_Girl.begin(); itr != m_AvaSkelCon_Girl.end(); ++itr)
		{
			(*itr)->update(t);
		}

		for(vector<CAvatarSkeletonModelEntity*>::iterator itr = m_AvaSkelCon_Boy.begin(); itr != m_AvaSkelCon_Boy.end(); ++itr)
		{
			(*itr)->update(t);
		}
		//更新物理
		m_Render->UpdateCpuSkin();
		m_Render->ForceSyncData();	
		m_Render->UpdatePhx(t);
	}

	virtual void ExecTestLogic()
	{
		RandomChangeBptAdorment();
	}

	void DoUpdate()
	{
		m_Render->UpdateEngineThread();
		UpdateCamera();

		float escape=GetEscapeTime();

		if (Spin_N_Ms(5000))
		{
			ExecTestLogic();
		}

		UpdateModel(escape);
	}

	void DoRender()
	{
		if (m_Player)
			m_Player->render();
	}

	virtual bool ManualRunExec()
	{
		return false;
	}
	virtual bool AutoRunExec()
	{
		return ManualRunExec();
	}

	virtual void AfterRender()
	{
		DoUpdate();
		DoRender();
	}

	virtual void CleanUpScene()
	{
		ReleaseModel();
		delete m_Stage;
		m_Stage=NULL;
		m_Player=NULL;
		m_Level=NULL;
		m_Factory=NULL;
		m_Render=NULL;
		m_Render->ClearMaterialLib();
		m_Render->ClearAllActions();
	}
private:
	static HippoTestCaseBase* const testcase_;

	std::vector<string>		m_LinkFList;
	std::vector<string>		m_LinkMList;
	std::vector<string>		m_BptFList;
	std::vector<string>		m_BptMList;

	std::vector<string>		m_BptPetFList;
	std::vector<string>		m_BptPetMList;
	std::vector<string>     m_LinkPetFList;
	std::vector<string>		m_LinkPetMList;

protected:
	std::vector<AvatarEntity*> m_AvatarCon_Male;
	std::vector<AvatarEntity*> m_AvatarCon_Female;
	std::vector<CAvatarSkeletonModelEntity*> m_AvaSkelCon_Boy;
	std::vector<CAvatarSkeletonModelEntity*> m_AvaSkelCon_Girl;

	H3DI::IRender* m_Render; 

	std::string		m_StageName;//场景文件名
	Stage* m_Stage;
	StageAnimationPlayer* m_Player;
	CLevelEntity* m_Level;
	CEntityFactory* m_Factory;

};
ADD_TESTCASE("STESS_TEST_MIDLAYER",StressTest_MidLayer_10);


/*测试逻辑
用例11（使用中间层）：
同步加载一个场景
创建50个人物lod0
创建50个宠物lod0
针对每个人，宠物执行以下动作：
异步随机换装 
异步随机换挂件
unload人物 reload人物
隐藏显示人物
异步创建1个特效绑定到人物或者宠物身上
同步创建1个特效绑定到人物或者宠物身上
异步设置动作
同步设置动作
*/

class StressTest_MidLayer_11 :public StressTest_MidLayer_10
{
public:
	StressTest_MidLayer_11(const char* casename):StressTest_MidLayer_10(casename){}


	void InitResourcesList()
	{
		std::string LinkF=Make_X52Resources_Path("art/role/link/female");
		std::string LinkM=Make_X52Resources_Path("art/role/link/male");
		std::string BptF=Make_X52Resources_Path("art/role/bodypart/female");
		std::string BptM=Make_X52Resources_Path("art/role/bodypart/male");
		std::string LinkPetF=Make_X52Resources_Path("art/role/Pet/link/female");
		std::string LinkPetM=Make_X52Resources_Path("art/role/Pet/link/male");
		std::string BptPetF=Make_X52Resources_Path("art/role/Pet/female");
		std::string BPtPetM=Make_X52Resources_Path("art/role/Pet/male");

		std::string actionFile=Make_X52Resources_Path("art/role/actions/role.xml");

		FindFileRecursion(&m_ResourcesTypeCon[LINK_ACTOR_FEMALE],LinkF.c_str(),"*.spe");
		FindFileRecursion(&m_ResourcesTypeCon[LINK_ACTOR_MALE],LinkM.c_str(),"*.spe");
		FindFileRecursion(&m_ResourcesTypeCon[BPT_ACTOR_FEMALE],BptF.c_str(),"*.bpt");
		FindFileRecursion(&m_ResourcesTypeCon[BPT_ACTOR_MALE],BptM.c_str(),"*.bpt");
		FindFileRecursion(&m_ResourcesTypeCon[LINK_PET_GIRL],LinkPetF.c_str(),"*.spe");
		FindFileRecursion(&m_ResourcesTypeCon[LINK_PET_BOY],LinkPetM.c_str(),"*.spe");
		FindFileRecursion(&m_ResourcesTypeCon[BPT_PET_GIRL],BptPetF.c_str(),"*.bpt");
		FindFileRecursion(&m_ResourcesTypeCon[BPT_PET_BOY],BPtPetM.c_str(),"*.bpt");

		ParseActionFile(actionFile);
	}

	void ParseActionFile(std::string actionFile)
	{
		TiXmlDocument docAction(actionFile.c_str());
		if (!docAction.LoadFile()) return;

		TiXmlElement* pDance = docAction.FirstChildElement("DANCE");
		if (!pDance) return;
		TiXmlElement* pActionRoot = pDance->FirstChildElement("ActionNode");
		if (!pActionRoot) return;
		TiXmlElement* pAction = pActionRoot->FirstChildElement("ActionNode");
		while (pAction)
		{
			const char* figName = pAction->Attribute("Figure");
			if (strcmp(figName, "girl") == 0)
			{
				TiXmlElement* pItem = pAction->FirstChildElement("Item");
				while (pItem)
				{
					const char* strActName = pItem->Attribute("id");
					if (strActName)
					{
						m_ActionTypeCon[ACTION_FEMALE].push_back(strActName);
					}

					pItem = pItem->NextSiblingElement("Item");
				}	
			}
			else if (strcmp(figName, "boy") == 0)
			{
				TiXmlElement* pItem = pAction->FirstChildElement("Item");
				while (pItem)
				{
					const char* strActName = pItem->Attribute("id");
					if (strActName)
					{
						m_ActionTypeCon[ACTION_MALE].push_back(strActName);
					}

					pItem = pItem->NextSiblingElement("Item");
				}	
			}
			else if (strcmp(figName, "pet_girl") == 0)
			{
				TiXmlElement* pItem = pAction->FirstChildElement("Item");
				while (pItem)
				{
					const char* strActName = pItem->Attribute("id");
					if (strActName)
					{
						m_ActionTypeCon[ACTION_PET_GIRL].push_back(strActName);
					}

					pItem = pItem->NextSiblingElement("Item");
				}
			}
			else if (strcmp(figName, "pet_boy") == 0)
			{
				TiXmlElement* pItem = pAction->FirstChildElement("Item");
				while (pItem)
				{
					const char* strActName = pItem->Attribute("id");
					if (strActName)
					{
						m_ActionTypeCon[ACTION_PET_BOY].push_back(strActName);
					}

					pItem = pItem->NextSiblingElement("Item");
				}
			}

			pAction = pAction->NextSiblingElement("ActionNode");
		}
	}
	
	virtual void ExecTestLogic()
	{
		for(vector<AvatarEntity*>::iterator itr = m_AvatarCon_Female.begin(); itr != m_AvatarCon_Female.end(); ++itr)
		{
			unsigned int operationIndex=GetRandomInt(0,OPERATION_TYPE_NUM-1);
			UpdateAvatarOperation(*itr,false,operationIndex);
		}

		for(vector<AvatarEntity*>::iterator itr = m_AvatarCon_Male.begin(); itr != m_AvatarCon_Male.end(); ++itr)
		{
			unsigned int operationIndex=GetRandomInt(0,OPERATION_TYPE_NUM-1);
			UpdateAvatarOperation(*itr,true,operationIndex);
		}


		for(vector<CAvatarSkeletonModelEntity*>::iterator itr = m_AvaSkelCon_Girl.begin(); itr != m_AvaSkelCon_Girl.end(); ++itr)
		{
			unsigned int operationIndex=GetRandomInt(0,OPERATION_TYPE_NUM-1);
			UpdateAvatarSkelOperation(*itr,false,operationIndex);
		}

		for(vector<CAvatarSkeletonModelEntity*>::iterator itr = m_AvaSkelCon_Boy.begin(); itr != m_AvaSkelCon_Boy.end(); ++itr)
		{
			unsigned int operationIndex=GetRandomInt(0,OPERATION_TYPE_NUM-1);
			UpdateAvatarSkelOperation(*itr,true,operationIndex);
		}
	}

	void UpdateAvatarOperation(AvatarEntity* avatar,bool bmale,unsigned int type)
	{
		switch (type)
		{
		case ASYN_BPT:
			AvatarRandomBpt(avatar,bmale);
			break;
		case ASYN_LINK:
			AvatarRandomLink(avatar,bmale);
			break;
		case UNLOAD_RELOAD:
			UnloadReload(avatar);
			break;
		case HIDE_SHOW:
			HideShow(avatar);
			break;
		case SYNC_SPE:
			AvatarBindEffect(avatar,bmale,true);
			break;
		case ASYN_SPE:
			AvatarBindEffect(avatar,bmale,false);
			break;
		case SYNC_ACTION:
			AvatarSetAction(avatar,bmale,true);
			break;
		case ASYN_ACTION:
			AvatarSetAction(avatar,bmale,false);
			break;
		}

	}
	void AvatarRandomBpt(AvatarEntity* avatar,bool bmale)
	{
		if (bmale)
		{
			int bptIdx=GetRandomInt(0,m_ResourcesTypeCon[BPT_ACTOR_MALE].size()-1);
			std::string bptMale=m_ResourcesTypeCon[BPT_ACTOR_MALE].at(bptIdx);
			Hippo_WriteConsoleAndLog(CC_WHITE,"%s男人换装:%s",avatar->get_name(),bptMale.c_str());
			std::string fileName;
			GetFileName(bptMale,fileName);

			if (fileName.size()<9)//如果名字不符合规范，忽略
			{
				return ;
			}

			int bptPos = atoi(fileName.substr(2, 1).c_str());
			avatar->appearance_priority(1);
			avatar->set_body_part(bptPos,bptMale,false);
			avatar->update_appearance();
		}
		else
		{
			int bptIdx=GetRandomInt(0,m_ResourcesTypeCon[BPT_ACTOR_FEMALE].size()-1);
			std::string bptFemale=m_ResourcesTypeCon[BPT_ACTOR_FEMALE].at(bptIdx);
			Hippo_WriteConsoleAndLog(CC_WHITE,"%s女人换装:%s",avatar->get_name(),bptFemale.c_str());
			std::string fileName;
			GetFileName(bptFemale,fileName);

			if (fileName.size()<9)//如果名字不符合规范，忽略
			{
				return ;
			}

			int bptPos = atoi(fileName.substr(2, 1).c_str());
			avatar->appearance_priority(1);
			avatar->set_body_part(bptPos,bptFemale,false);
			avatar->update_appearance();
		}
	}

	void AvatarRandomLink(AvatarEntity* avatar,bool bmale)
	{

		if (bmale)
		{
			int linkIdx=GetRandomInt(0,m_ResourcesTypeCon[LINK_ACTOR_MALE].size()-1);
			std::string linkMale=m_ResourcesTypeCon[LINK_ACTOR_MALE].at(linkIdx);
			Hippo_WriteConsoleAndLog(CC_WHITE,"%s男人挂件:%s",avatar->get_name(),linkMale.c_str());
			std::string fileName;
			GetFileName(linkMale,fileName);
			if (fileName.size()<9)//如果名字不符合规范，忽略
			{
				return ;
			}
			int linkPos = atoi(fileName.substr(2, 2).c_str()) - 10;
			avatar->appearance_priority(1);
			avatar->setAdornment(linkPos,linkMale.c_str(),0,false);
			avatar->update_appearance();
		}
		else
		{
			int linkIdx=GetRandomInt(0,m_ResourcesTypeCon[LINK_ACTOR_FEMALE].size()-1);
			std::string linkFemale=m_ResourcesTypeCon[LINK_ACTOR_FEMALE].at(linkIdx);
			Hippo_WriteConsoleAndLog(CC_WHITE,"%s女人挂件:%s",avatar->get_name(),linkFemale.c_str());
			std::string fileName;
			GetFileName(linkFemale,fileName);
			if (fileName.size()<9)//如果名字不符合规范，忽略lil
			{
				return ;
			}
			int linkPos = atoi(fileName.substr(2, 2).c_str())-10;
			avatar->appearance_priority(1);
			avatar->setAdornment(linkPos,linkFemale.c_str(),0,false);
			avatar->update_appearance();
		}
	}

	void UnloadReload(AvatarEntity* avatar)
	{
		int index=IsInVector(&m_UnloadAvatarCon,avatar);
		if (index!=-1 &&index!=m_UnloadAvatarCon.size())
		{
			std::vector<AvatarEntity*>::iterator itr=m_UnloadAvatarCon.begin();
			while(index!=0)
			{
				index--;
				itr++;
			}
			avatar->ReloadResources(false,1);//asyn
			m_UnloadAvatarCon.erase(itr);
			Hippo_WriteConsoleAndLog(CC_WHITE,"%sReload",avatar->get_name());
		}
		else
		{
			avatar->UnloadResources();
			m_UnloadAvatarCon.push_back(avatar);
			Hippo_WriteConsoleAndLog(CC_WHITE,"%sUnload",avatar->get_name());
		}
	}

	void HideShow(AvatarEntity* avatar)
	{
		int index=IsInVector(&m_HideAvatarCon,avatar);
		if (index!=-1 &&index!=m_HideAvatarCon.size())
		{
			std::vector<AvatarEntity*>::iterator itr=m_HideAvatarCon.begin();
			while(index!=0)
			{
				index--;
				itr++;
			}
			avatar->set_visible(false);//asyn
			m_HideAvatarCon.erase(itr);
			Hippo_WriteConsoleAndLog(CC_WHITE,"%s隐藏",avatar->get_name());
		}
		else
		{
			avatar->set_visible(true);
			m_HideAvatarCon.push_back(avatar);	
			Hippo_WriteConsoleAndLog(CC_WHITE,"%s显示",avatar->get_name());
		}
	}

	void AvatarSetAction(AvatarEntity* avatar,bool bmale,bool sync)
	{
		avatar->appearance_priority(1);
		int actionIdx;
		if (bmale)
		{
			actionIdx=GetRandomInt(0,m_ActionTypeCon[ACTION_MALE].size()-1);
			Hippo_WriteConsoleAndLog(CC_WHITE,"%s男人动作:%s",avatar->get_name(), m_ActionTypeCon[ACTION_MALE].at(actionIdx).c_str());
			if (sync)
			{

				//判断是否unload
				int index=IsInVector(&m_UnloadAvatarCon,avatar);
				if (index!=-1 &&index!=m_UnloadAvatarCon.size())
				{
					return ;
				}
				avatar->set_action(0, m_ActionTypeCon[ACTION_MALE].at(actionIdx).c_str(), true, true);
			}
			else
			{
				avatar->set_action(0, m_ActionTypeCon[ACTION_MALE].at(actionIdx).c_str(), true, false);
			}
		}
		else
		{
			actionIdx=GetRandomInt(0,m_ActionTypeCon[ACTION_FEMALE].size()-1);
			Hippo_WriteConsoleAndLog(CC_WHITE,"%s女人动作%s",avatar->get_name(), m_ActionTypeCon[ACTION_FEMALE].at(actionIdx).c_str());
			if (sync)
			{
				//判断是否unload
				int index=IsInVector(&m_UnloadAvatarCon,avatar);
				if (index!=-1 &&index!=m_UnloadAvatarCon.size())
				{
					return ;
				}
				avatar->set_action(0, m_ActionTypeCon[ACTION_FEMALE].at(actionIdx).c_str(), true, true);
			}
			else
			{
				avatar->set_action(0, m_ActionTypeCon[ACTION_FEMALE].at(actionIdx).c_str(), true, false);
			}
		}
		avatar->update_appearance();
	}

	void AvatarBindEffect(AvatarEntity* avatar,bool bmale,bool sync)
	{
		//判断是否unload
		int index=IsInVector(&m_UnloadAvatarCon,avatar);
		if (index!=-1 &&index!=m_UnloadAvatarCon.size())
		{
			return ;
		}

		if (m_EffectCon.size()==50)
		{
			ReleaseEffect();
		}


		if (bmale)
		{
			int speIdx=GetRandomInt(0,m_ResourcesTypeCon[LINK_ACTOR_MALE].size()-1);
			std::string speMale=m_ResourcesTypeCon[LINK_ACTOR_MALE].at(speIdx);
			Hippo_WriteConsoleAndLog(CC_WHITE,"%s男人绑定特效：%s",avatar->get_name(),speMale.c_str());
			CEffectEntity* effect_entity;
			if (sync)
			{
			    effect_entity = m_Factory->create_effect(speMale.c_str(), "effect", true);
			}
			else
			{
				effect_entity = m_Factory->create_effect(speMale.c_str(), "effect", false);
			}
			
			m_EffectCon.push_back(effect_entity);
			m_Level->add_to_level(effect_entity);
			avatar->set_effect(effect_entity);
		}
		else
		{
			int speIdx=GetRandomInt(0,m_ResourcesTypeCon[LINK_ACTOR_FEMALE].size()-1);
			std::string speFemale=m_ResourcesTypeCon[LINK_ACTOR_FEMALE].at(speIdx);
			Hippo_WriteConsoleAndLog(CC_WHITE,"%s女人绑定特效：%s",avatar->get_name(),speFemale.c_str());
			CEffectEntity* effect_entity;
			if (sync)
			{
				effect_entity = m_Factory->create_effect(speFemale.c_str(), "effect", true);
			}
			else
			{
				effect_entity = m_Factory->create_effect(speFemale.c_str(), "effect", false);
			}

			m_EffectCon.push_back(effect_entity);
			m_Level->add_to_level(effect_entity);
			avatar->set_effect(effect_entity);
		}
	}


	void UpdateAvatarSkelOperation(CAvatarSkeletonModelEntity* avatarSkel,bool bboy,unsigned int type)
	{
		switch (type)
		{
		case ASYN_BPT:
			AvatarSkelRandomBpt(avatarSkel,bboy);
			break;
		case ASYN_LINK:
			AvatarSkelRandomLink(avatarSkel,bboy);
			break;
		case SYNC_SPE:
			AvatarSkelBindEffect(avatarSkel,bboy,true);
			break;
		case ASYN_SPE:
			AvatarSkelBindEffect(avatarSkel,bboy,false);
			break;
		case SYNC_ACTION:
			AvatarSkelSetAction(avatarSkel,bboy,true);
			break;
		case ASYN_ACTION:
			AvatarSkelSetAction(avatarSkel,bboy,false);
			break;
		}
	}

	void AvatarSkelRandomBpt(CAvatarSkeletonModelEntity* avatarskel,bool bboy)
	{
		if (bboy)
		{
			int bptIdx=GetRandomInt(0,m_ResourcesTypeCon[BPT_PET_BOY].size()-1);
			std::string bptBoy=m_ResourcesTypeCon[BPT_PET_BOY].at(bptIdx);
			Hippo_WriteConsoleAndLog(CC_WHITE,"%s男宠换装：%s",avatarskel->get_name(),bptBoy.c_str());
			std::string fileName;
			GetFileName(bptBoy,fileName);

			int bptPos ;
			if (!IsValidPetBpt(fileName,bptPos))
				return ;

			avatarskel->set_body_part(bptPos,bptBoy,false);
			avatarskel->update_appearance();
		}
		else
		{
			int bptIdx=GetRandomInt(0,m_ResourcesTypeCon[BPT_PET_BOY].size()-1);
			std::string bptGirl=m_ResourcesTypeCon[BPT_PET_BOY].at(bptIdx);
			Hippo_WriteConsoleAndLog(CC_WHITE,"%s女宠换装：%s",avatarskel->get_name(),bptGirl.c_str());
			std::string fileName;
			GetFileName(bptGirl,fileName);

			int bptPos ;
			if (!IsValidPetBpt(fileName,bptPos))
				return ;

			avatarskel->set_body_part(bptPos,bptGirl,false);
			avatarskel->update_appearance();
		}
	}
	
	void AvatarSkelRandomLink(CAvatarSkeletonModelEntity* avatarskel,bool bboy)
	{
		if (bboy)
		{
			int linkIdx=GetRandomInt(0,m_ResourcesTypeCon[LINK_PET_BOY].size()-1);
			std::string linkBoy=m_ResourcesTypeCon[LINK_PET_BOY].at(linkIdx);
			Hippo_WriteConsoleAndLog(CC_WHITE,"%s男宠挂件:%s",avatarskel->get_name(),linkBoy.c_str());
			std::string fileName;
			GetFileName(linkBoy,fileName);
			if (fileName.size()<10)//如果名字不符合规范，忽略
			{
				return ;
			}
			int linkPos = atoi(fileName.substr(2, 2).c_str()) - 10;
			avatarskel->setAdornment(linkPos,linkBoy.c_str(),false);
			avatarskel->update_appearance();
		}
		else
		{
			int linkIdx=GetRandomInt(0,m_ResourcesTypeCon[LINK_PET_GIRL].size()-1);
			std::string linkGirl=m_ResourcesTypeCon[LINK_PET_GIRL].at(linkIdx);
			Hippo_WriteConsoleAndLog(CC_WHITE,"%s女宠挂件：%s",avatarskel->get_name(),linkGirl.c_str());
			std::string fileName;
			GetFileName(linkGirl,fileName);
			int linkPos = atoi(fileName.substr(2, 2).c_str()) - 10;
			if (fileName.size()<10)//如果名字不符合规范，忽略
			{
				return ;
			}
			avatarskel->setAdornment(linkPos,linkGirl.c_str(),false);
			avatarskel->update_appearance();
		}
	}

	void AvatarSkelSetAction(CAvatarSkeletonModelEntity* avatarskel,bool bboy,bool sync)
	{
		avatarskel->appearance_priority(1);
		int actionIdx;
		if (bboy)
		{
			actionIdx=GetRandomInt(0,m_ActionTypeCon[ACTION_PET_BOY].size()-1);
			Hippo_WriteConsoleAndLog(CC_WHITE,"%s男宠动作：%s",avatarskel->get_name(),m_ActionTypeCon[ACTION_PET_BOY].at(actionIdx).c_str());
			if (sync)
			{
				avatarskel->set_action(0, m_ActionTypeCon[ACTION_PET_BOY].at(actionIdx).c_str(), true, true);
			}
			else
			{
				avatarskel->set_action(0, m_ActionTypeCon[ACTION_PET_BOY].at(actionIdx).c_str(), true, false);
			}
		}
		else
		{
			actionIdx=GetRandomInt(0,m_ActionTypeCon[ACTION_PET_GIRL].size()-1);
			Hippo_WriteConsoleAndLog(CC_WHITE,"%s女宠动作：%s",avatarskel->get_name(),m_ActionTypeCon[ACTION_PET_GIRL].at(actionIdx).c_str());
			if (sync)
			{
				avatarskel->set_action(0, m_ActionTypeCon[ACTION_PET_GIRL].at(actionIdx).c_str(), true, true);
			}
			else
			{
				avatarskel->set_action(0, m_ActionTypeCon[ACTION_PET_GIRL].at(actionIdx).c_str(), true, false);
			}
		}
		avatarskel->update_appearance();
	}

	void AvatarSkelBindEffect(CAvatarSkeletonModelEntity* avatarskel,bool bmale,bool sync)
	{
		if (m_EffectCon.size()==50)
		{
			ReleaseEffect();
		}

		if (bmale)
		{
			int speIdx=GetRandomInt(0,m_ResourcesTypeCon[LINK_PET_BOY].size()-1);
			std::string speMale=m_ResourcesTypeCon[LINK_PET_BOY].at(speIdx);
			Hippo_WriteConsoleAndLog(CC_WHITE,"%s男宠绑定特效：%s",avatarskel->get_name(),speMale.c_str());
			CEffectEntity* effect_entity;
			if (sync)
			{
				effect_entity = m_Factory->create_effect(speMale.c_str(), "effect", true);
			}
			else
			{
				effect_entity = m_Factory->create_effect(speMale.c_str(), "effect", false);
			}

			m_EffectCon.push_back(effect_entity);
			m_Level->add_to_level(effect_entity);
			avatarskel->set_effect(effect_entity);
		}
		else
		{
			int speIdx=GetRandomInt(0,m_ResourcesTypeCon[LINK_PET_GIRL].size()-1);
			std::string speFemale=m_ResourcesTypeCon[LINK_PET_GIRL].at(speIdx);
			Hippo_WriteConsoleAndLog(CC_WHITE,"%s女宠绑定特效：%s",avatarskel->get_name(),speFemale.c_str());
			CEffectEntity* effect_entity;
			if (sync)
			{
				effect_entity = m_Factory->create_effect(speFemale.c_str(), "effect", true);
			}
			else
			{
				effect_entity = m_Factory->create_effect(speFemale.c_str(), "effect", false);
			}

			m_EffectCon.push_back(effect_entity);
			m_Level->add_to_level(effect_entity);
			avatarskel->set_effect(effect_entity);
		}
	}

	virtual void ReleaseModel()
	{
		for(std::vector<AvatarEntity*>::iterator itr = m_AvatarCon_Female.begin(); itr != m_AvatarCon_Female.end(); ++itr)
		{
			if ((*itr)->is_added_to_level() && m_Level)
				m_Level->remove_from_level((*itr));
			m_Factory->release((*itr));
		}
		m_AvatarCon_Female.clear();

		for(std::vector<AvatarEntity*>::iterator itr = m_AvatarCon_Male.begin(); itr != m_AvatarCon_Male.end(); ++itr)
		{
			if ((*itr)->is_added_to_level() && m_Level)
				m_Level->remove_from_level((*itr));
			m_Factory->release((*itr));
		}
		m_AvatarCon_Female.clear();


		for(std::vector<CAvatarSkeletonModelEntity*>::iterator itr = m_AvaSkelCon_Girl.begin(); itr != m_AvaSkelCon_Girl.end(); ++itr)
		{
			if ((*itr)->is_added_to_level() && m_Level)
				m_Level->remove_from_level((*itr));
			m_Factory->release((*itr));
		}
		m_AvaSkelCon_Girl.clear();

		for(std::vector<CAvatarSkeletonModelEntity*>::iterator itr = m_AvaSkelCon_Boy.begin(); itr != m_AvaSkelCon_Boy.end(); ++itr)
		{
			if ((*itr)->is_added_to_level() && m_Level)
				m_Level->remove_from_level((*itr));
			m_Factory->release((*itr));
		}
		m_AvaSkelCon_Boy.clear();

		ReleaseEffect();
	}

	void ReleaseEffect()
	{
		for (std::vector<CEffectEntity*>::iterator itr=m_EffectCon.begin();itr!=m_EffectCon.end();++itr)
		{
			if ((*itr)->is_added_to_level() && m_Level)
				m_Level->remove_from_level((*itr));
			m_Factory->release((*itr));
		}
		m_EffectCon.clear();
	}

	virtual void UpdateModel(float t)
	{
		//更新
		if (m_Player)
		{
			m_Player->update(t);
		}

		for(vector<AvatarEntity*>::iterator itr = m_AvatarCon_Female.begin(); itr != m_AvatarCon_Female.end(); ++itr)
		{
			(*itr)->update(t);
		}

		for(vector<AvatarEntity*>::iterator itr = m_AvatarCon_Male.begin(); itr != m_AvatarCon_Male.end(); ++itr)
		{
			(*itr)->update(t);
		}


		for(vector<CAvatarSkeletonModelEntity*>::iterator itr = m_AvaSkelCon_Girl.begin(); itr != m_AvaSkelCon_Girl.end(); ++itr)
		{
			(*itr)->update(t);
		}

		for(vector<CAvatarSkeletonModelEntity*>::iterator itr = m_AvaSkelCon_Boy.begin(); itr != m_AvaSkelCon_Boy.end(); ++itr)
		{
			(*itr)->update(t);
		}

		for (vector<CEffectEntity*>::iterator itr=m_EffectCon.begin();itr!=m_EffectCon.end();++itr)
		{
			(*itr)->update(t);
		}

		//更新物理
		m_Render->UpdateCpuSkin();
		m_Render->ForceSyncData();	
		m_Render->UpdatePhx(t);
	}

private:
	static HippoTestCaseBase* const testcase_;

	enum OPERATION_TYPE
	{
		ASYN_BPT=0,//异步随机换装
		ASYN_LINK,//异步随机换挂件
		UNLOAD_RELOAD,//unload人物 reload人物：注意 这个操作不对宠物
		HIDE_SHOW,//隐藏显示人物 注意 这个操作不对宠物
		ASYN_SPE, //异步创建1个特效绑定到人物或者宠物身上
		SYNC_SPE, //同步创建1个特效绑定到人物或者宠物身上
		ASYN_ACTION,//异步设置动作
		SYNC_ACTION,//同步设置动作
		OPERATION_TYPE_NUM
	};
    std::vector<std::string> m_OperationTypeCon[OPERATION_TYPE_NUM];


	enum RESOURCES_TYPE
	{
		//服装
		BPT_ACTOR_FEMALE,
		BPT_ACTOR_MALE,
		BPT_PET_BOY,
		BPT_PET_GIRL,
		//挂件
		LINK_ACTOR_FEMALE,
		LINK_ACTOR_MALE,
		LINK_PET_BOY,
		LINK_PET_GIRL,
		//特效
		SPE_BIND,
		RESOURCE_TYPE_NUM
	};
	std::vector<std::string> m_ResourcesTypeCon[RESOURCE_TYPE_NUM];

	enum ACTION_TYPE
	{
		ACTION_FEMALE=0,
		ACTION_MALE,
		ACTION_PET_GIRL,
		ACTION_PET_BOY,
		ACTION_TYPE_NUM
	};
	std::vector<std::string> m_ActionTypeCon[ACTION_TYPE_NUM];

protected:
	std::vector<CEffectEntity*> m_EffectCon;
	std::vector<AvatarEntity*> m_UnloadAvatarCon;
	std::vector<AvatarEntity*> m_HideAvatarCon;
};
ADD_TESTCASE("STESS_TEST_MIDLAYER",StressTest_MidLayer_11);