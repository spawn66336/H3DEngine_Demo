/********************************************************************
	created:	2013/03/20
	created:	20:3:2013   17:36
	filename: 	D:\Code\X52\X52_Community_Version\TestCode\Utility\CommonCode\HippoUtil\HippoTaskVisitor.h
	file path:	D:\Code\X52\X52_Community_Version\TestCode\Utility\CommonCode\HippoUtil
	file base:	HippoTaskVisitor
	file ext:	h
	author:		sssa2000
	
	purpose:	
*********************************************************************/
#pragma once

#include <functional>
#include <vector>
#include <map>
#include "engine_interface.h"
#include <boost/function.hpp>
#include <boost/bind.hpp>
using namespace std;
using namespace boost;

typedef boost::function<int (unsigned int)> TaskFinishCallback;
class HippoTaskVistor
{
public:
	HippoTaskVistor();
	~HippoTaskVistor();
	void PushLoadBodyPartTask(const char* fileName, H3DI::EActorType actorType, int matlod, int nPriority,TaskFinishCallback cb);

	int PushTaskID(unsigned int id,TaskFinishCallback cb);
	bool IsAllTaskFinished();
	void Update();

private:

	std::map<unsigned int,TaskFinishCallback> m_all_taskdata;
};
