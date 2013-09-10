#include "H3DDressSelector.h"

namespace ZPH3D
{

	H3DDressInfo::H3DDressInfo( const H3DDressInfo& info )
	{
		this->operator=( info );
	}

	H3DDressInfo& H3DDressInfo::operator=( const H3DDressInfo& rhs )
	{
		m_iType = rhs.m_iType;
		m_strResPath = rhs.m_strResPath;
		return *this;
	}



	H3DDressSelector::H3DDressSelector( void )
	{
		
	}


	H3DDressSelector::~H3DDressSelector(void)
	{
	}


}//namespace ZPH3D