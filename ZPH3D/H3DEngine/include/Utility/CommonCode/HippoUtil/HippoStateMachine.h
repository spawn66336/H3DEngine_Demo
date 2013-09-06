/********************************************************************
	created:	2013/03/20
	created:	20:3:2013   17:32
	filename: 	D:\Code\X52\X52_Community_Version\TestCode\Utility\CommonCode\HippoUtil\HippoStateMachine.h
	file path:	D:\Code\X52\X52_Community_Version\TestCode\Utility\CommonCode\HippoUtil
	file base:	HippoStateMachine
	file ext:	h
	author:		sssa2000
	
	purpose:	
*********************************************************************/
#pragma once

#include <functional>
#include <vector>
#include <map>
#include <boost/function.hpp>
#include <boost/bind.hpp>
using namespace std;
using namespace boost;

/*状态机
要求：状态必须是整型，会按照AddState的顺序依次执行每个状态
用户需要提供每个状态时对应的函数，并且需要负责状态如何切换
*/
enum SCSTATE
{
	STATE_NOT_CHANGE,
	STATE_CHANGE
};

typedef boost::function<SCSTATE (void)> StateCallback;
class Hippo_StateMachine
{
public:

	Hippo_StateMachine();
	~Hippo_StateMachine();
	void AddState(int state,StateCallback cb);
	void Run();
	int GetCurrentState();
	int m_current_state;
private:
	typedef std::vector<std::pair<int,StateCallback> > StateCon;
	typedef StateCon::iterator StateConItr;

	
	StateCon m_state_table;

	std::pair<int,StateCallback>* _getCurrent();

};
