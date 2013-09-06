#include "bin_xml_converter.h"

BinXMLConvert::BinXMLConvert(const char* file) : m_file(file)
{
	m_dictbuf = new char[BinXMLReader::DICT_BUFLEN];

	memset(&m_header, 0, sizeof(m_header));
	m_header.magic_word = 'BX';
	m_header.headlen = sizeof(m_header);

	m_writer = new BinXMLWriter(BinXMLWriter::WRITE_BUFF, &m_dict);
}

BinXMLConvert::~BinXMLConvert()
{
	delete m_writer;
	delete[] m_dictbuf;
}

void BinXMLConvert::save_to_file(const char* file_name)
{
	FILE* fp = NULL;
	errno_t err = fopen_s(&fp, file_name, "wb");
	if(fp)
	{
		m_header.content_len = m_writer->position();

		fwrite(&m_header, sizeof(m_header), 1, fp);
		fwrite(m_dictbuf, m_header.dict_len, 1, fp);
		fwrite(m_writer->buffer(), m_header.content_len, 1, fp);
		fclose(fp);
	}
	else
	{
		printf("file not opened:%s", file_name);
	}
}

void BinXMLConvert::convert_attribs(BiXmlElementImpl* pElement, unsigned int indent)
{
	if ( !pElement )
		return;

	BiXmlAttribute* pAttrib=pElement->FirstAttribute();
	//printf("\n");
	while (pAttrib)
	{
//		printf( "%s: value=[%s]", pAttrib->Name(), pAttrib->Value());
		m_writer->write(BT_ATTRIBUTE);
		m_writer->write_string_only(pAttrib->Name());
		m_writer->write(pAttrib->Value());

//		int ival;
//		if (pAttrib->QueryIntValue(&ival)==BIXML_SUCCESS)
//			printf( " int=%d", ival);

//		double dval;
//		if (pAttrib->QueryDoubleValue(&dval)==BIXML_SUCCESS)
//			printf( " d=%1.1f", dval);

//		printf( "\n" );
		pAttrib=pAttrib->Next();
	}
}

void BinXMLConvert::convert_to_binary( BiXmlNodeImpl* pParent, int indent)
{
	if ( !pParent )
		return;

//	printf( "%s", getIndent(indent));

	BXML_TYPE type;
	int t = pParent->Type();
	if(t==BiXmlNodeImpl::DOCUMENT)
	{
		//printf( "Document" );
		type = BT_DOCUMENT;
		m_writer->write(type);
	}
	else if(t==BiXmlNodeImpl::ELEMENT)
	{
		//printf( "Element [%s]", pParent->Value() );
		type = BT_ELEMENT;
		m_writer->write(type);

		m_writer->write_string_only(pParent->Value());
		convert_attribs(pParent->_ToElement(), indent+1);
	}
	else if(t==BiXmlNodeImpl::TEXT)
	{
		BiXmlTextImpl* pText = pParent->_ToText();
		const char* text = pText->Value();
		m_writer->write(text);
		//printf( "Text: [%s]", pText->Value() );
	}
	else if(t==BiXmlNodeImpl::DECLARATION)
	{
		//printf( "Declaration" );
	}
	else if(t==BiXmlNodeImpl::COMMENT)
	{
		//printf( "Comment: [%s]", pParent->Value());
	}
	else if(t==BiXmlNodeImpl::UNKNOWN)
	{
		//printf( "Unknown" );
	}
//	printf( "\n" );


	//------------------------------------------
	for ( BiXmlNodeImpl* pChild = pParent->_FirstChild(); pChild != 0; pChild = pChild->_NextSibling()) 
	{
		convert_to_binary( pChild, indent+1 );
	}
	//------------------------------------------


	//write end brackets
	if(t==BiXmlNodeImpl::DOCUMENT)
	{
		type = BT_END_DOCUMENT;
		m_writer->write(type);
	}
	else if(t==BiXmlNodeImpl::ELEMENT)
	{
		type = BT_END_ELEMENT;
		m_writer->write(type);
	}
}

void BinXMLConvert::convert_to_binary(const char* file_name)
{
	//BiXmlDocumentImpl doc(m_file.c_str());
	BiXmlDocumentImpl* doc = BiXmlCreateDocument(m_file.c_str());
	bool loadOkay = doc->LoadFile();
	if (loadOkay)
	{
		//printf("\n%s:\n", m_file.c_str());
		convert_to_binary( doc, 0 );
		m_header.dict_len = m_dict.save(m_dictbuf, GetDictBuffLength() );

		save_to_file(file_name);
	}
	else
	{
		printf("Failed to load file \"%s\"\n", m_file.c_str());
	}
	BiXmlReleaseDocument(doc);
}
