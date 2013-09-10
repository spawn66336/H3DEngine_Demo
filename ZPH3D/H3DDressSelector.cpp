#include "H3DDressSelector.h"
#include "tinyxml.h" 

namespace ZPH3D
{

	H3DDressInfo::H3DDressInfo( const H3DDressInfo& info )
	{
		this->operator=( info );
	}

	H3DDressInfo& H3DDressInfo::operator=( const H3DDressInfo& rhs )
	{
		m_id = rhs.m_id;
		m_iType = rhs.m_iType;
		m_strResPath = rhs.m_strResPath;
		return *this;
	}



	H3DDressSelector::H3DDressSelector( void )
	{ 
		for( int i = 0 ; i < ACTOR_ITEM_NUM ; i++ )
		{
			m_maleDressTypeList.push_back( dressList_t() );
			m_femaleDressTypeList.push_back( dressList_t() );
		}
	} 
	H3DDressSelector::~H3DDressSelector(void)
	{
	}

	void H3DDressSelector::LoadDressesFromXmlFile( const String& filename )
	{
		TiXmlDocument doc;
		bool bRes = doc.LoadFile( filename.c_str() );

		//Èô¶ÁÈ¡Ê§°Ü
		if( !bRes )
		{
			return;
		}

		TiXmlElement* pRoot=doc.RootElement();
		TiXmlElement* pDressList=pRoot->FirstChildElement("Dress");

		while (pDressList)
		{
			this->_ParseActorDressList(pDressList);
			pDressList=pDressList->NextSiblingElement("Dress");
		}
	}

	void H3DDressSelector::_ParseActorDressList( TiXmlElement* pXmlElem )
	{
		ZP_ASSERT( NULL != pXmlElem ); 
		String id=pXmlElem->Attribute("id");
		String name=pXmlElem->Attribute("name");
		String appearancetype=pXmlElem->Attribute("appearancetype");
		String gender=pXmlElem->Attribute("gender");

		TiXmlElement* pBodyPartList=pXmlElem->FirstChildElement("bodypartlist");
		TiXmlElement* pAdornmentList=pXmlElem->FirstChildElement("adornmentlist");

		if( NULL == pBodyPartList )
		{
			return;
		}
		
		bool male = ( gender.compare("0") == 0 );

		TiXmlElement* pBodyPartElem = pBodyPartList->FirstChildElement("bodypart");

		while( NULL != pBodyPartElem )
		{
			this->_ParseActorDress( male , pBodyPartElem );
			pBodyPartElem = pBodyPartElem->NextSiblingElement("bodypart");
		}

	}

	void H3DDressSelector::_ParseActorDress( const bool male , TiXmlElement* pXmlElem )
	{
		H3DDressInfo newDress; 
		newDress.SetType( atoi( pXmlElem->Attribute("type") ) );
		newDress.SetPath( pXmlElem->Attribute("src") );
		_GetActorDressTypeList( male ).at( newDress.GetType() ).push_back( newDress );
	}

	H3DDressSelector::dressTypeList_t& H3DDressSelector::_GetActorDressTypeList( const bool male )
	{
		if( male )
		{
			return m_maleDressTypeList;
		}
		return m_femaleDressTypeList;
	}




}//namespace ZPH3D