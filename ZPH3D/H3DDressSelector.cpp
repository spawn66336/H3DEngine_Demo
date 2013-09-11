#include "H3DDressSelector.h"
#include "tinyxml.h" 
#include "dMathHeader.h"

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

		//若读取失败
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
		 
		
		bool male = ( gender.compare("0") == 0 );

		if( NULL != pBodyPartList )
		{ 
			TiXmlElement* pBodyPartElem = pBodyPartList->FirstChildElement("bodypart"); 
			while( NULL != pBodyPartElem )
			{
				this->_ParseActorDress( male , pBodyPartElem );
				pBodyPartElem = pBodyPartElem->NextSiblingElement("bodypart");
			} 
		}

		if( NULL != pAdornmentList )
		{
			TiXmlElement* pAdornment = pAdornmentList->FirstChildElement("adornment");
			while( NULL != pAdornment )
			{
				this->_ParseActorAdornment( male , pAdornment );
				pAdornment = pAdornment->NextSiblingElement("adornment");
			}
		} 
	}

	void H3DDressSelector::_ParseActorDress( const bool male , TiXmlElement* pXmlElem )
	{
		H3DDressInfo newDress; 
		newDress.SetType( atoi( pXmlElem->Attribute("type") ) );
		newDress.SetPath( pXmlElem->Attribute("src") );

		//忽略素体部件
		String::size_type pos = newDress.GetPath().find("strippedbody");
		if( pos != String::npos )
		{
			return;
		} 
		//查看当前部件是否存在
		if( !_IsResourceExist( newDress.GetPath() ) )
		{
			return;
		}

		_GetActorDressTypeList( male ).at( newDress.GetType() ).push_back( newDress );
	}

	void H3DDressSelector::_ParseActorAdornment( const bool male , TiXmlElement* pXmlElem )
	{
		H3DDressInfo newDress; 
		newDress.SetType( atoi( pXmlElem->Attribute("type") ) );
		newDress.SetPath( pXmlElem->Attribute("src") ); 
		//查看当前部件是否存在
		if( !_IsResourceExist( newDress.GetPath() ) )
		{
			return;
		}

		_GetActorDressTypeList( male ).at( newDress.GetType() ).push_back( newDress ); 
	}


	bool H3DDressSelector::_IsResourceExist( const String& path )
	{
		FILE* pFile = NULL;
		if( 0 != fopen_s( &pFile , path.c_str() , "rb" ) )
		{
			return false;
		}
		fclose( pFile );
		pFile = NULL;
		return true;
	}



	H3DDressSelector::dressTypeList_t& H3DDressSelector::_GetActorDressTypeList( const bool male )
	{
		if( male )
		{
			return m_maleDressTypeList;
		}
		return m_femaleDressTypeList;
	}

	void H3DDressSelector::PreLoadActorBodyParts( H3DI::IRender* pRenderer )
	{
		H3DI::sCreateOp createOp;
		createOp.mat_lod = 0;
		dressTypeList_t::iterator itDressTypeList = _GetActorDressTypeList(true).begin();
		while(  _GetActorDressTypeList(true).end() != itDressTypeList )
		{
			if( 0 != (*itDressTypeList).size()  )
			{
				dressList_t::iterator itDress =  (*itDressTypeList).begin();
				while( itDress != (*itDressTypeList).end() )
				{
					unsigned int uiTaskID =	pRenderer->LoadBodypartAsyn( (*itDress).GetPath().c_str() , H3DI::ACTOR_HUMAN , createOp , 100 );
					++itDress;
				} 
			}
			++itDressTypeList;
		}//while(  _GetActorDressTypeList(male).end() != itDressTypeList )

		 itDressTypeList = _GetActorDressTypeList(false).begin();
		while(  _GetActorDressTypeList(false).end() != itDressTypeList )
		{
			if( 0 != (*itDressTypeList).size()  )
			{
				dressList_t::iterator itDress =  (*itDressTypeList).begin();
				while( itDress != (*itDressTypeList).end() )
				{
					unsigned int uiTaskID = pRenderer->LoadBodypartAsyn( (*itDress).GetPath().c_str() , H3DI::ACTOR_HUMAN , createOp , 100 ); 
					++itDress;
				} 
			}
			++itDressTypeList;
		}//while(  _GetActorDressTypeList(male).end() != itDressTypeList )
	
		pRenderer->DoTaskImmediately( 0 );
	}


	void H3DDressSelector::RandomActorDresses( H3DI::IActor* pActor  , bool male  )
	{ 
		idRandom2 _random( GetTickCount() );
		
		if( male )
		{
			m_currMaleDressSelection.clear();
		}

		int currType = 0;
		dressTypeList_t::iterator itDressTypeList = _GetActorDressTypeList(male).begin();
		while(  _GetActorDressTypeList(male).end() != itDressTypeList )
		{
			if( 0 != (*itDressTypeList).size()  )
			{
				size_t s= (*itDressTypeList).size();
				int rand_idx=_random.RandomInt(s); 

				//若为挂件
				if( currType >= 10 )
				{
					H3DI::EActorAdornmentPosition adornmentPos = 
						static_cast<H3DI::EActorAdornmentPosition>( currType - 10 );

					//随机决定此处是否有挂件
					int rand_vis = _random.RandomInt(2); 
					if( 1 == rand_vis )
					{
						pActor->SetAdornment(  adornmentPos ,  (*itDressTypeList).at( rand_idx ).GetPath().c_str()  );
						pActor->SetAdornmentVisibility( adornmentPos , true );
					}else{
						pActor->SetAdornmentVisibility( adornmentPos , false );
					}

				}else{
					pActor->SetBodyPart( (*itDressTypeList).at( rand_idx ).GetPath().c_str() );

					if( male )
					{
						m_currMaleDressSelection.push_back( (*itDressTypeList).at( rand_idx ) );
					}

				}
			}

			currType++;
			++itDressTypeList;
		}//while(  _GetActorDressTypeList(male).end() != itDressTypeList )
	}






}//namespace ZPH3D