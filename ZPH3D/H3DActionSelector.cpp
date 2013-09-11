#include "H3DActionSelector.h"
#include "tinyxml.h" 
#include "dMathHeader.h"

namespace ZPH3D
{

	H3DActionInfo::H3DActionInfo( void )
	{ 
	}

	H3DActionInfo::H3DActionInfo( const H3DActionInfo& info )
	{
		this->operator=( info );
	}


	H3DActionInfo::~H3DActionInfo()
	{ 
	}

	H3DActionInfo& H3DActionInfo::operator=( const H3DActionInfo& rhs )
	{
		m_strID = rhs.m_strID;
		m_strFileName = rhs.m_strFileName;
		return *this;
	}



	H3DActionSelector::H3DActionSelector(void):
	m_currMaleActionIndx(0),
	m_currFemaleActionIndx(0)
	{ 
	} 

	H3DActionSelector::~H3DActionSelector(void)
	{ 
	}

	void H3DActionSelector::LoadActionsFromXMLFile( const String& filename  )
	{
		TiXmlDocument doc;
		bool bRes = doc.LoadFile( filename.c_str() ); 
		//若读取失败
		if( !bRes )
		{
			return;
		}

		TiXmlElement* pRoot=doc.RootElement();
		TiXmlElement* pActionNode=pRoot->FirstChildElement("ActionNode");
		pActionNode = pActionNode->FirstChildElement("ActionNode");
		
		while( NULL != pActionNode )
		{
			this->_ParseActionNode( pActionNode );
			pActionNode = pActionNode->NextSiblingElement("ActionNode");
		}
	}

	void H3DActionSelector::_ParseActionNode( TiXmlElement* pElem )
	{
		String strFigure = pElem->Attribute("Figure");

		bool male = false;
		if( 0 == strFigure.compare("girl") )
		{
			male = false;
		}else if( 0 ==strFigure.compare("boy") ){
			male = true;
		}else{
			return;
		}

		TiXmlElement* pItem = pElem->FirstChildElement("Item");
		while( NULL != pItem )
		{
			H3DActionInfo info;

			String tmp = pItem->Attribute("file");

			info.SetID( pItem->Attribute("id") ); 

			int pos = tmp.rfind('.');
			if( String::npos != pos )
			{ 
				int len = tmp.length();
				tmp.at(len-1) = '6';
				tmp.at(len-2) = 'c';

				info.SetFileName( tmp );

				//若动作存在则加入列表中
				if( _IsActionResourceExist( info.GetFileName() ) )
				{ 
					this->_GetActionList( male ).push_back( info );
				}

			}

			pItem = pItem->NextSiblingElement("Item");
		}
	}

	H3DActionSelector::actionList_t& H3DActionSelector::_GetActionList( const bool male )
	{
		if( male )
		{
			return m_maleActions;
		}
		return m_femaleActions;
	}

	bool H3DActionSelector::_IsActionResourceExist( const String& filename )
	{
		FILE* pFile = NULL;
		if( 0 != fopen_s( &pFile , filename.c_str() , "rb" ) )
		{
			return false;
		}
		fclose( pFile );
		pFile = NULL;
		return true;
	}

	String H3DActionSelector::GetCurrAction( const bool male )
	{
		if( _GetActionList( male ).size() )
		{ 
			String strActionID;
			if( male )
			{
				strActionID = m_maleActions.at( m_currMaleActionIndx ).GetID();
			}else{
				strActionID = m_femaleActions.at( m_currFemaleActionIndx ).GetID();
			}
			return strActionID;
		}
		return "";
	}

	void H3DActionSelector::NextAction( void )
	{
		m_currMaleActionIndx = ( m_currMaleActionIndx+1 ) % m_maleActions.size();
		m_currFemaleActionIndx = ( m_currFemaleActionIndx+1 ) % m_femaleActions.size();
	}




}