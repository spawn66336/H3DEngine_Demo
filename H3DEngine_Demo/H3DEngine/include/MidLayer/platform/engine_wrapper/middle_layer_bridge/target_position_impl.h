#pragma once

#include "i_target_position.h"

class ITargetPositionImpl;
class ITargetPositionMngImpl;

namespace ClientBridge
{
	public ref class TargetPositionMngImpl
	{
	public:
		TargetPositionMngImpl();
		~TargetPositionMngImpl();

		void Update(float t);
		void AddTarget(int type, int idx, void* target);
		void SetValidByIdx(int idx, bool bValid);

		void setBoneName(System::String^ boneName);

		ITargetPositionMngImpl* mITargetPositionMng;
	};
}