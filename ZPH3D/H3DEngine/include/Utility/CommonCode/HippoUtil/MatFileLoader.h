/********************************************************************
	created:	2013/02/08
	created:	8:2:2013   16:18
	filename: 	D:\Code\X52\Engine_MT_Main\TestCode\Utility\CommonCode\HippoUtil\MatFileLoader.h
	file path:	D:\Code\X52\Engine_MT_Main\TestCode\Utility\CommonCode\HippoUtil
	file base:	MatFileLoader
	file ext:	h
	author:		sssa2000
	
	purpose:	
*********************************************************************/
#pragma once

namespace H3DI
{
	class IMatProxy;
}
class MatFileLoader
{
public:
	static H3DI::IMatProxy* LoadMatFromFile(const char* filename,const char* ball);

protected:
private:
};
