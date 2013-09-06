#include "HippoTaskVisitor.h"
#include "HippoGlobal.h"

#include "engine_interface_internal.h"

namespace H3DI
{
	class IRender;
}
HippoTaskVistor::HippoTaskVistor()
{

}
HippoTaskVistor::~HippoTaskVistor()
{

}


void HippoTaskVistor::PushLoadBodyPartTask(const char* fileName, H3DI::EActorType actorType, int matlod, int nPriority,TaskFinishCallback cb)
{
	H3DI::sCreateOp s;
	s.mat_lod=matlod;
	unsigned int id=Hippo_GetIRender()->LoadBodypartAsyn(fileName,actorType, s,nPriority);
	m_all_taskdata[id]=cb;
}

int HippoTaskVistor::PushTaskID(unsigned int id,TaskFinishCallback cb)
{
	m_all_taskdata[id]=cb;
	return 1;
}

bool HippoTaskVistor::IsAllTaskFinished()
{
	return m_all_taskdata.empty();
}

void HippoTaskVistor::Update()
{
	Hippo_GetIRender()->UpdateEngineThread();
	auto itr=m_all_taskdata.begin();
	while(itr!=m_all_taskdata.end())
	{
		unsigned int taskid=itr->first;
		unsigned int  res = Hippo_GetIRender()->QueryTaskState(taskid);
		if (res == H3DI::TASK_OK || res == H3DI::TASK_ERROR)
		{
			TaskFinishCallback& cb=itr->second;
			cb(taskid);
			m_all_taskdata.erase(itr++);
			continue;
		}
		++itr;
	}
}