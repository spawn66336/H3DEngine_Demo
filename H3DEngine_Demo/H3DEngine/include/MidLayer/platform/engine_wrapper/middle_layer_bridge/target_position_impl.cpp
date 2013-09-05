#include "Stdafx.h"
#include "i_target_position_impl.h"
#include "target_position_impl.h"
#include "../middle_layer/middle_layer_interface.h"
#include "middle_layer_bridge.h"

ClientBridge::TargetPositionMngImpl::TargetPositionMngImpl()
{
	mITargetPositionMng = new ITargetPositionMngImpl();
}

ClientBridge::TargetPositionMngImpl::~TargetPositionMngImpl()
{
	delete mITargetPositionMng;
}

void ClientBridge::TargetPositionMngImpl::Update( float t )
{
}

void ClientBridge::TargetPositionMngImpl::AddTarget( int type, int idx, void* target )
{
	mITargetPositionMng->AddTarget((TargetType)type, idx, (H3DI::ISkeletonModel*)target);
}

void ClientBridge::TargetPositionMngImpl::SetValidByIdx(int idx, bool bValid)
{
	mITargetPositionMng->SetValidByIdx(idx, bValid);
}

void ClientBridge::TargetPositionMngImpl::setBoneName( System::String^ boneName )
{
	mITargetPositionMng->SetBoneName(std::string(StringItemEx(boneName)));
}
