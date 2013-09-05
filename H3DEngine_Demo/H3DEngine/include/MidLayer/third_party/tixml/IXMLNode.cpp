#include "IXMLNode.h"
#include "./tinyxml/tinyxml.h"
#include "./tinyxml/tinystr.h"
#include "./binxml/bin_xml.h"
#include <sstream>
#include <iostream>
extern const int g_xml_use_mem_pool = 0;

BiXmlDocument* XmlDocFactory::CreateDocument(DocType dt /*= DT_XML*/)
{
	if (DT_XML == dt)
	{
		//BiXmlDocumentImpl* doc = new BiXmlDocumentImpl;
		BiXmlDocumentImpl* doc = BiXmlCreateDocument();
		return doc;
	}
	else if (DT_BIN == dt)
	{
		//TiBxmlDocumentImpl* doc = new TiBxmlDocumentImpl;
		BiBxmlDocumentImpl* doc = BiBxmlCreateDocument();
		return doc;
	}
	return NULL;
}

bool XmlDocFactory::ReleaseDocument(BiXmlDocument* doc)
{
	bool bRet = true;
	if (doc == NULL)
		return bRet;
	if (DT_XML == doc->GetXmlType())
	{
		BiXmlDocumentImpl* docimp = dynamic_cast<BiXmlDocumentImpl*>(doc);
		//delete docimp;
		BiXmlReleaseDocument(docimp);
	}
	else if (DT_BIN == doc->GetXmlType())
	{
		BiBxmlDocumentImpl* docimp = dynamic_cast<BiBxmlDocumentImpl*>(doc);
		//delete docimp;
		BiBxmlReleaseDocument(docimp);
	}
	doc = NULL;
	return bRet;
}
BiXmlElement* XmlDocFactory::CreateElement(const char* strName, DocType dt /*= DT_XML*/)
{
	if (!strName)
		return NULL;
	if (DT_XML == dt)
	{
		//BiXmlElementImpl* elem = new BiXmlElementImpl(strName);
		BiXmlElementImpl* elem = BiXmlCreateElement(strName);
		return elem;
	}
	else if (DT_BIN == dt)
	{
		//BiBxmlElementImpl* elem = new TiBxmlElementImpl(strName);
		BiBxmlElementImpl* elem = BiBxmlCreateElement(strName);
		return elem;
	}
	return NULL;
}

bool XmlDocFactory::ReleaseElement(BiXmlElement* elem)
{
	bool bRet = true;
	if (elem == NULL)
		return bRet;
	if (DT_XML == elem->GetXmlType())
	{
		BiXmlElementImpl* elemimp = dynamic_cast<BiXmlElementImpl*>(elem);
		//delete elemimp;
		BiXmlReleaseElement(elemimp);
	}
	else if (DT_BIN == elem->GetXmlType())
	{
		BiBxmlElementImpl* elemimp = dynamic_cast<BiBxmlElementImpl*>(elem);
		//delete elemimp;
		BiBxmlReleaseElement(elemimp);
	}
	elem = NULL;
	return bRet;
}

std::string & operator<< (std::string& out, const BiXmlElement* base )
{
	BiXmlElement* node = const_cast<BiXmlElement*>(base);
	if (DT_BIN == node->GetXmlType())
	{
		BiBxmlElementImpl* elem = dynamic_cast<BiBxmlElementImpl*>(node);
		if (!node)
			return out;

		std::ostringstream os_stream( std::ostringstream::out );
		elem->StreamOut( &os_stream );
		out.append( os_stream.str());
		return out;
	}
	else if ( DT_XML == node->GetXmlType() )
	{
		const BiXmlElementImpl* elem = dynamic_cast<const BiXmlElementImpl*>(base);
		if (!node)
			return out;

		//std::ostringstream os_stream( std::ostringstream::out );
#ifdef BIXML_USE_STL

		std::ostringstream os_stream( std::ostringstream::out );
		elem->StreamOut( &os_stream );
		out.append( os_stream.str());
		return out;
#else		
		BiXmlOutStream XmlOut;
		//BIXML_OSTREAM XmlOut;
		elem->StreamOut( &XmlOut );
		out.append( XmlOut.c_str(), XmlOut.length());
		return out;
#endif
		
	}
	else
		return out;
}
