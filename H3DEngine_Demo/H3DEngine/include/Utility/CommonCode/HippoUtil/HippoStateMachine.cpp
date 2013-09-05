#include "HippoStateMachine.h"


Hippo_StateMachine::Hippo_StateMachine()
{
	m_current_state=0;
}
Hippo_StateMachine::~Hippo_StateMachine()
{

}

void Hippo_StateMachine::AddState(int state,StateCallback cb)
{
	m_state_table.push_back(std::make_pair(state,cb));
}

void Hippo_StateMachine::Run()
{
	std::pair<int,StateCallback>* p=_getCurrent();
	if(!p)
		return;

	SCSTATE s=p->second();
	if(s==STATE_CHANGE)
	{
		m_current_state+=1;
	}
}

std::pair<int,StateCallback>* Hippo_StateMachine::_getCurrent()
{
	if(m_state_table.empty() || m_current_state>=(int)(m_state_table.size()))
		return 0;

	std::pair<int,StateCallback>* single=&(m_state_table.at(m_current_state));
	return single;
}

int Hippo_StateMachine::GetCurrentState()
{
	std::pair<int,StateCallback>* p=_getCurrent();
	if(!p)
		return -1;
	return p->first;
}