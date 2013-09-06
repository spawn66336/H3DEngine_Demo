#include "bin_xml.h"
#include <assert.h>
#include <algorithm>
#include <common_lib/vfs_dll/ifile.h>

/*
ClassPool<BiBxmlElementImpl, CBiboLock> elem_pool;
ClassPool<BiBxmlAttributeImpl, CBiboLock> attrib_pool;
ClassPool<BiBxmlDocumentImpl, CBiboLock> doc_pool;
*/

extern const int g_xml_use_mem_pool;

BiBxmlAttributeImpl* BiBxmlCreateAttribute()
{
	return BiBxmlFactory::CreateAttribute();
}

BiBxmlDocumentImpl* BiBxmlCreateDocument()
{
	return BiBxmlFactory::CreateDocument();
}

void BiBxmlReleaseDocument(BiBxmlDocumentImpl* doc)
{
	doc->Release();
}

BiBxmlElementImpl* BiBxmlCreateElement(const char* _value)
{
	return BiBxmlFactory::CreateElement(_value);
}

void BiBxmlReleaseElement(BiBxmlElementImpl* elem)
{
	elem->Release();
}

ClassPool<BiBxmlAttributeImpl, CBiboLock> BiBxmlFactory::m_AttributePool;
ClassPool<BiBxmlDocumentImpl, CBiboLock> BiBxmlFactory::m_DocumentPool;
ClassPool<BiBxmlElementImpl, CBiboLock> BiBxmlFactory::m_ElementPool;

BiBxmlAttributeImpl* BiBxmlFactory::CreateAttribute()
{
	if (g_xml_use_mem_pool)
	{
		return m_AttributePool.alloc();
	}
	else
	{
		return new BiBxmlAttributeImpl;
	}
	
}

BiBxmlDocumentImpl* BiBxmlFactory::CreateDocument()
{
	if (g_xml_use_mem_pool)
	{
		return m_DocumentPool.alloc();
	}
	else
	{
		return new BiBxmlDocumentImpl;
	}
	
}

BiBxmlElementImpl* BiBxmlFactory::CreateElement(const char* _value)
{
	if (g_xml_use_mem_pool)
	{
		return m_ElementPool.alloc(_value);
	}
	else
	{
		return new BiBxmlElementImpl(_value);
	}
	
}

void BiBxmlFactory::ReleaseAttribute(BiBxmlAttributeImpl* attr)
{
	if (attr)
	{
		if (g_xml_use_mem_pool)
		{
			m_AttributePool.dealloc(attr);
		}
		else
		{
			delete attr;
		}
		
	}
}

void BiBxmlFactory::ReleaseDocument(BiBxmlDocumentImpl* doc)
{
	if (doc)
	{
		if (g_xml_use_mem_pool)
		{
			m_DocumentPool.dealloc(doc);
		}
		else
		{
			delete doc;
		}
		
	}
}

void BiBxmlFactory::ReleaseElement(BiBxmlElementImpl* elem)
{
	if (elem)
	{
		if (g_xml_use_mem_pool)
		{
			m_ElementPool.dealloc(elem);
		}
		else
		{
			delete elem;
		}
	}
}
BiBxmlNodeImpl::Entity BiBxmlNodeImpl::entity[ NUM_ENTITY ] = 
{
	{ "&amp;",  5, '&' },
	{ "&lt;",   4, '<' },
	{ "&gt;",   4, '>' },
	{ "&quot;", 6, '\"' },
	{ "&apos;", 6, '\'' }
};

void BiBxmlNodeImpl::PutString( const std::string& str, std::ostream* stream )
{
	std::string buffer;
	PutString( str, &buffer );
	(*stream) << buffer;
}
void BiBxmlNodeImpl::PutString( const std::string& str, std::string* outString )
{
	int i=0;

	while( i<(int)str.length() )
	{
		unsigned char c = (unsigned char) str[i];

		if (    c == '&' 
		     && i < ( (int)str.length() - 2 )
			 && str[i+1] == '#'
			 && str[i+2] == 'x' )
		{
			// Hexadecimal character reference.
			// Pass through unchanged.
			// &#xA9;	-- copyright symbol, for example.
			//
			// The -1 is a bug fix from Rob Laveaux. It keeps
			// an overflow from happening if there is no ';'.
			// There are actually 2 ways to exit this loop -
			// while fails (error case) and break (semicolon found).
			// However, there is no mechanism (currently) for
			// this function to return an error.
			while ( i<(int)str.length()-1 )
			{
				outString->append( str.c_str() + i, 1 );
				++i;
				if ( str[i] == ';' )
					break;
			}
		}
		else if ( c == '&' )
		{
			outString->append( entity[0].str, entity[0].strLength );
			++i;
		}
		else if ( c == '<' )
		{
			outString->append( entity[1].str, entity[1].strLength );
			++i;
		}
		else if ( c == '>' )
		{
			outString->append( entity[2].str, entity[2].strLength );
			++i;
		}
		else if ( c == '\"' )
		{
			outString->append( entity[3].str, entity[3].strLength );
			++i;
		}
		else if ( c == '\'' )
		{
			outString->append( entity[4].str, entity[4].strLength );
			++i;
		}
		else if ( c < 32 )
		{
			// Easy pass at non-alpha/numeric/symbol
			// Below 32 is symbolic.
			char buf[ 32 ];
			
			sprintf( buf, "&#x%02X;", (unsigned) ( c & 0xff ) );

			//*ME:	warning C4267: convert 'size_t' to 'int'
			//*ME:	Int-Cast to make compiler happy ...
			outString->append( buf, (int)strlen( buf ) );
			++i;
		}
		else
		{
			//char realc = (char) c;
			//outString->append( &realc, 1 );
			*outString += (char) c;	// somewhat more efficient function call.
			++i;
		}
	}
}

BiBxmlNodeImpl::BiBxmlNodeImpl() : 
m_parent(NULL), 
m_previous(NULL), 
m_next(NULL)
{
}
BiBxmlNodeImpl::~BiBxmlNodeImpl()
{
	ReleaseChildren();
}
void BiBxmlNodeImpl::ReleaseChildren()
{
	for(size_t i=0; i<m_children.size(); ++i)
	{
		BiBxmlNodeImpl* child = m_children[i];
		child->Release();
	}
	m_children.clear();
}

BiBxmlNodeImpl* BiBxmlNodeImpl::_FirstChild()
{
	if (!m_children.empty())
	{
		return m_children[0];
	}
	return NULL;
}

BiBxmlNodeImpl* BiBxmlNodeImpl::_FirstNonAttribute()
{
	for (size_t i=0; i<m_children.size(); ++i)
	{
		if (m_children[i]->Type() != BXML_ATTRIBUTE)
		{
			return m_children[i];
		}
	}

	return NULL;
}

const BiBxmlNodeImpl* BiBxmlNodeImpl::_FirstNonAttribute() const
{
	for (size_t i=0; i<m_children.size(); ++i)
	{
		if (m_children[i]->Type() != BXML_ATTRIBUTE)
		{
			return m_children[i];
		}
	}

	return NULL;
}

BiBxmlElementImpl* BiBxmlNodeImpl::_FirstChildElement()
{
	for (size_t i=0; i<m_children.size(); ++i)
	{
		if (m_children[i]->Type() == BXML_ELEMENT)
		{
			return dynamic_cast<BiBxmlElementImpl*>(m_children[i]);
		}
	}
	return NULL;
}
BiBxmlElementImpl* BiBxmlNodeImpl::_NextSiblingElement()
{
	BiBxmlNodeImpl* node = m_next;
	for (; node; node = node->m_next)
		if (node->Type() == BXML_ELEMENT)
			return dynamic_cast<BiBxmlElementImpl*>(node);

	return NULL;
}
void BiBxmlNodeImpl::AddChild(BiBxmlNodeImpl* child)
{
	BiBxmlNodeImpl* previous;
	if(m_children.empty())
		previous = NULL;
	else
		previous = m_children.back();

	m_children.push_back(child);

	child->m_parent = this;
	child->m_next = NULL;
	child->m_previous = previous;
	if(previous)
		previous->m_next = child;
}

void BiBxmlElementImpl::Release()
{
	//elem_pool.dealloc(this);
	BiBxmlFactory::ReleaseElement(this);
}
void BiBxmlAttributeImpl::Release()
{
	//attrib_pool.dealloc(this);
	BiBxmlFactory::ReleaseAttribute(this);
}
void BiBxmlTextImpl::StreamOut( std::ostream* stream )
{
}
void BiBxmlTextImpl::Release()
{
}
void BiBxmlDocumentImpl::Release()
{
	//doc_pool.dealloc(this);
	BiBxmlFactory::ReleaseDocument(this);
}

void BiBxmlDocumentImpl::save_attribute(BiBxmlElementImpl* pElement, unsigned int indent)
{
	if ( !pElement )
		return;

	BiBxmlAttributeImpl* pAttrib=pElement->FirstAttribute();
	//printf("\n");
	while (pAttrib)
	{
		//		printf( "%s: value=[%s]", pAttrib->Name(), pAttrib->Value());
		m_writer->write(BT_ATTRIBUTE);
		m_writer->write_string_only(pAttrib->Value());
		m_writer->write(pAttrib->AttValue());
		pAttrib = pAttrib->NextAttribute();
	}
}

void BiBxmlDocumentImpl::save_binary(BiBxmlNodeImpl* pParent, int indent)
{
	if ( !pParent )
		return;

	//	printf( "%s", getIndent(indent));

	BXML_TYPE type;
	int t = pParent->Type();
	if(t==BiBxmlNodeImpl::BXML_DOCUMENT)
	{
		//printf( "Document" );
		type = BT_DOCUMENT;
		m_writer->write(type);
	}
	else if(t==BiBxmlNodeImpl::BXML_ELEMENT)
	{
		//printf( "Element [%s]", pParent->Value() );
		type = BT_ELEMENT;
		m_writer->write(type);

		m_writer->write_string_only(pParent->Value());
		save_attribute(dynamic_cast<BiBxmlElementImpl*>(pParent), indent+1);
		const char* text = pParent->ToElement()->GetText();
		m_writer->write(text);
		//printf( "Text: [%s]", pText->Value() );
	}
	else if(t==BiBxmlNodeImpl::BXML_TEXT)
	{
		//BiXmlText* pText = pParent->ToText();
		//const char* text = pText->Value();
		//m_writer->write(text);
		//printf( "Text: [%s]", pText->Value() );
	}
	else if(t==BiBxmlNodeImpl::BXML_DECLARATION)
	{
		//printf( "Declaration" );
	}
	else if(t==BiBxmlNodeImpl::BXML_COMMENT)
	{
		//printf( "Comment: [%s]", pParent->Value());
	}
	else if(t==BiBxmlNodeImpl::BXML_UNKNOWN)
	{
		//printf( "Unknown" );
	}
	//	printf( "\n" );


	//------------------------------------------
	for ( BiBxmlNodeImpl* pChild = pParent->_FirstChild(); pChild != 0; pChild = pChild->_NextSibling()) 
	{
		save_binary( pChild, indent+1 );
	}
	//------------------------------------------


	//write end brackets
	if(t==BiBxmlNodeImpl::BXML_DOCUMENT)
	{
		type = BT_END_DOCUMENT;
		m_writer->write(type);
	}
	else if(t==BiBxmlNodeImpl::BXML_ELEMENT)
	{
		type = BT_END_ELEMENT;
		m_writer->write(type);
	}
}

void BiBxmlDocumentImpl::save_to_file(const char* file_name)
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
		reader_print("file not opened:%s", file_name);
	}
}

void BiBxmlAttributeImpl::StreamOut( std::ostream* stream )
{
	if (m_value.find( '\"' ) != std::string::npos)
	{
		PutString( m_name, stream );
		(*stream) << "=" << "'";
		PutString( m_value, stream );
		(*stream) << "'";
	}
	else
	{
		PutString( m_name, stream );
		(*stream) << "=" << "\"";
		PutString( m_value, stream );
		(*stream) << "\"";
	}
}
BiXmlNode* BiBxmlAttributeImpl::Clone()const
{
	//BiBxmlAttributeImpl* attr = attrib_pool.alloc();
	BiBxmlAttributeImpl* attr = BiBxmlCreateAttribute();
	attr->SetValue(Value());
	attr->SetAttValue(AttValue());
	return attr;
}
BiBxmlAttributeImpl* BiBxmlAttributeImpl::NextAttribute()
{
	BiBxmlNodeImpl* node = m_next;
	while(node)
	{
		if (node->Type() == BXML_ATTRIBUTE)
		{
			BiBxmlAttributeImpl* attr = dynamic_cast<BiBxmlAttributeImpl*>(node);
			return attr;
		}
		node = node->_NextSibling();
	}

	return NULL;
}

void BiBxmlDocumentImpl::StreamOut( std::ostream* stream )
{
	BiBxmlNodeImpl* node;
	for ( node=_FirstChild(); node; node=node->_NextSibling() )
	{
		node->StreamOut( stream );

		// Special rule for streams: stop after the root element.
		// The stream in code will only read one element, so don't
		// write more than one.
		if ( node->ToElement() )
			break;
	}
}

BiBxmlDocumentImpl::BiBxmlDocumentImpl():m_dictbuf(NULL), m_writer(NULL)
{
	//m_dictbuf = new char[BinXMLReader::DICT_BUFLEN];

	memset(&m_header, 0, sizeof(m_header));
	m_header.magic_word = 'BX';
	m_header.headlen = sizeof(m_header);

	m_reader.SetOwner(this);
	//m_writer = new BinXMLWriter(BinXMLWriter::WRITE_BUFF, &m_dict);
};

BiBxmlDocumentImpl::~BiBxmlDocumentImpl()
{
	if (m_writer)
		delete m_writer;
	if (m_dictbuf)
		delete[] m_dictbuf;

	m_dictbuf = NULL;
	m_writer = NULL;
}
void PrintIndent(int depth)
{
	for(int i=0;i<depth;++i)
		reader_print("    ");
}

void BinXMLReader::ReadTextOnly(const char*& text)
{
	short index;
	m_buff->read(index);
	text = m_dict.find_string(index);
}

void BinXMLReader::ReadBinXMLValue(BiBxmlNodeImpl* parent, const BXML_TYPE& type)
{
	if(type == BT_INDEXED_STRING)
	{
		const char* text;
		ReadTextOnly(text);
		
		reader_print("%s",text);
	}
	else if(type == BT_BYTE)
	{
		char data;
		m_buff->read(data);
		reader_print("%c",data);
	}
	else if(type == BT_WORD)
	{
		short data;
		m_buff->read(data);
		reader_print("%d",data);
	}
	else if(type == BT_DWORD)
	{
		int data;
		m_buff->read(data);
		reader_print("%d",data);
	}
	else if(type == BT_FLOAT)
	{
		float data;
		m_buff->read(data);
		reader_print("%f",data);
	}
	else if(type >= BT_DIGIT_BEGIN && type <= BT_DIGIT_END)
	{
		int data;
		data = type-BT_DIGIT_BEGIN;
	}
	else
	{
		m_buff->throw_exception();
	}
}
void BinXMLReader::ReadBinXMLAttributeValue(BiBxmlAttributeImpl* attribute, const BXML_TYPE& type)
{
	if(type == BT_INDEXED_STRING)
	{
		const char* text;
		ReadTextOnly(text);
		attribute->SetAttValue(text);

		reader_print("%s",text);
	}
	else if(type == BT_BYTE)
	{
		char data;
		m_buff->read(data);
		attribute->SetAttValue(&data);
		reader_print("%c",data);
	}
	else if(type == BT_WORD)
	{
		short data;
		m_buff->read(data);
		char buf[64];
		sprintf_s(buf, 64, "%d", data);
		attribute->SetAttValue(buf);

		reader_print("%d",data);
	}
	else if(type == BT_DWORD)
	{
		int data;
		m_buff->read(data);
		char buf[64];
		sprintf_s(buf, 64, "%d", data);
		attribute->SetAttValue(buf);
		reader_print("%d",data);
	}
	else if(type == BT_FLOAT)
	{
		float data;
		m_buff->read(data);

		char buf[256];
		sprintf_s(buf, 256, "%f", data);
		attribute->SetAttValue(buf);

		reader_print("%f",data);
	}
	else if(type >= BT_DIGIT_BEGIN && type <= BT_DIGIT_END)
	{
		int data;
		data = type-BT_DIGIT_BEGIN;
	}
	else
	{
		m_buff->throw_exception();
	}
}

void BinXMLReader::ReadBinXMLAttribute(BiBxmlNodeImpl* parent)
{
	//BiBxmlAttributeImpl* attr = attrib_pool.alloc();
	BiBxmlAttributeImpl* attr = BiBxmlCreateAttribute();
	parent->AddChild(attr);

	const char* name;
	ReadTextOnly(name);
	attr->SetValue(name);
	reader_print("%s=", name);

	BXML_TYPE type;
	m_buff->read(type);//value
	//ReadBinXMLValue(attr, type);
	ReadBinXMLAttributeValue(attr, type);
}

void BinXMLReader::ReadBinXMLElementValue(BiBxmlElementImpl* element, const BXML_TYPE& type)
{
	if(type == BT_INDEXED_STRING)
	{
		const char* text;
		ReadTextOnly(text);
		element->SetText(text);

		reader_print("%s",text);
	}
	else if(type == BT_BYTE)
	{
		char data;
		m_buff->read(data);
		element->SetText(&data);
		reader_print("%c",data);
	}
	else if(type == BT_WORD)
	{
		short data;
		m_buff->read(data);
		char buf[64];
		sprintf_s(buf, 64, "%d", data);
		element->SetText(buf);

		reader_print("%d",data);
	}
	else if(type == BT_DWORD)
	{
		int data;
		m_buff->read(data);
		char buf[64];
		sprintf_s(buf, 64, "%d", data);
		element->SetText(buf);
		reader_print("%d",data);
	}
	else if(type == BT_FLOAT)
	{
		float data;
		m_buff->read(data);

		char buf[256];
		sprintf_s(buf, 256, "%f", data);
		element->SetText(buf);

		reader_print("%f",data);
	}
	else if(type >= BT_DIGIT_BEGIN && type <= BT_DIGIT_END)
	{
		int data;
		data = type-BT_DIGIT_BEGIN;
	}
	else
	{
		m_buff->throw_exception();
	}
}

void BinXMLReader::ReadBinXMLElement(BiBxmlNodeImpl* parent, int depth)
{
	//BiBxmlElementImpl* elem = elem_pool.alloc();
	BiBxmlElementImpl* elem = BiBxmlCreateElement("");
	parent->AddChild(elem);

	//element name
	const char* name;
	ReadTextOnly(name);
	elem->SetValue(name);

	reader_print("\n");//PrintIndent(depth);
	reader_print("<%s>", name);
	bool has_subelm = false;

	//element content
	BXML_TYPE type;
	m_buff->read(type);
	while(type!=BT_END_ELEMENT)
	{
		if(type == BT_ELEMENT)
		{
			has_subelm = true;
			ReadBinXMLElement(elem, depth+1);
		}
		else if(type == BT_ATTRIBUTE)
		{
			ReadBinXMLAttribute(elem);
		}
		else
		{
			//ReadBinXMLValue(elem, type);
			ReadBinXMLElementValue(elem, type);
		}
		m_buff->read(type);
	}
	if(has_subelm)
	{
		reader_print("\n");PrintIndent(depth);
	}
	reader_print("</%s>", name);
}

void BinXMLReader::ReadBinXMLDocument()
{
	BXML_TYPE type;
	m_buff->read(type);
	while(type!=BT_END_DOCUMENT)
	{
		if(type == BT_ELEMENT)
		{
			if (!m_owndoc)
				return;

			ReadBinXMLElement(m_owndoc, 0);
		}
		else
		{
			m_buff->throw_exception();
		}
		m_buff->read(type);
	}
}

void BinXMLReader::ReadBinXML()
{
	BXML_TYPE type;
	m_buff->read(type);
	if(type == BT_DOCUMENT)
	{
		ReadBinXMLDocument();
	}
	else 
	{
		m_buff->throw_exception();
	}
}

bool BinXMLReader::LoadFromFile(const char* file_name)
{
	FILE* fp = NULL;
	errno_t err = fopen_s(&fp, file_name, "rb");
	if(fp)
		return LoadFromFile(fp);
	else
	{
		reader_print("file not opened:%s", file_name);
		return false;
	}
}

bool BinXMLReader::LoadFromFile(FILE* fp)
{
	if(fp)
	{
		//获得文件头
		BinXML_Header header;
		fread(&header, sizeof(header), 1, fp);

		if (header.dict_len > DICT_BUFLEN || header.dict_len < 0)
			return false;

		//申请文件中字典部分的存储空间
		if (m_dictbuf)
		{
			delete[] m_dictbuf;
			m_dictbuf = NULL;
		}
		m_dictbuf = new char[header.dict_len + 1];

		//获得字典内容
		fread(m_dictbuf, header.dict_len, 1, fp);
		m_dict.load(m_dictbuf, header.dict_len);

		//获得文件内容
		if (m_content_buf)
		{
			delete[] m_content_buf;
			m_content_buf = NULL;
		}
		m_content_buf = new char[header.content_len];
		fread(m_content_buf, header.content_len, 1, fp);

		if (m_buff)
		{
			delete[] m_buff;
			m_buff = NULL;
		}
		m_buff = new xml_buffer(m_content_buf, header.content_len);

		fclose(fp);

		//读取二进制xml
		ReadBinXML();

		if (m_dictbuf)
		{
			delete[] m_dictbuf;
			m_dictbuf = NULL;
		}
		if (m_content_buf)
		{
			delete[] m_content_buf;
			m_content_buf = NULL;
		}
		m_dict.clear();

		return true;
	}
	else
	{
		reader_print("file not opened:%s");
		return false;
	}
}

bool BinXMLReader::LoadFileFromPack(const char* filename)
{
	VFS::IFile* file = VFS::Hfopen(filename,"rb",0);
	if ( file)
	{
		long length = (long)file->GetSize();
		if ( length == 0 )
		{
			VFS::Hfclose( file );
			return false;
		}
		char* buf = new char[ length+1 ];
		buf[0] = 0;

		if ( VFS::Hfread( buf, length, 1, file ) != 1 ) {
			reader_print("open pack failed!\n");
			VFS::Hfclose(file);
			return false;
		}

		//获得文件头
		BinXML_Header header;
		memcpy(&header, buf, sizeof(header));
		//fread(&header, sizeof(header), 1, fp);

		if (header.dict_len > DICT_BUFLEN || header.dict_len < 0)
			return false;

		//申请文件中字典部分的存储空间
		if (m_dictbuf)
		{
			delete[] m_dictbuf;
			m_dictbuf = NULL;
		}
		m_dictbuf = new char[header.dict_len + 1];

		//获得字典内容
		memcpy(m_dictbuf, (buf + sizeof(header)), header.dict_len);
		//fread(m_dictbuf, header.dict_len, 1, fp);
		m_dict.load(m_dictbuf, header.dict_len);

		//获得文件内容
		if (m_content_buf)
		{
			delete[] m_content_buf;
			m_content_buf = NULL;
		}
		m_content_buf = new char[header.content_len];

		memcpy(m_content_buf, (buf + sizeof(header) + header.dict_len), header.content_len);
		//fread(m_content_buf, header.content_len, 1, fp);
		if (m_buff)
		{
			delete m_buff;
			m_buff = NULL;
		}
		m_buff = new xml_buffer(m_content_buf, header.content_len);

		delete [] buf;
		buf = 0;
		VFS::Hfclose(file);

		//读取二进制xml
		ReadBinXML();

		if (m_dictbuf)
		{
			delete[] m_dictbuf;
			m_dictbuf = NULL;
		}
		if (m_content_buf)
		{
			delete[] m_content_buf;
			m_content_buf = NULL;
		}
		m_dict.clear();
		return true;
	}
	else
	{
		reader_print("file not opened:%s", filename);
		return false;
	}
}


void BiBxmlNodeImpl::Clear()
{
	ReleaseChildren();

	m_parent = 0;
	m_next = 0;
	m_previous = 0;
	m_name.clear();
}

const BiXmlNode* BiBxmlNodeImpl::FirstChild()const
{
	return _FirstNonAttribute();
}
BiXmlNode* BiBxmlNodeImpl::FirstChild()
{
	return _FirstNonAttribute();
}

const BiXmlNode* BiBxmlNodeImpl::FirstChild( const char * value ) const
{
	for (size_t i=0; i<m_children.size(); ++i)
	{
		if (strcmp(m_children[i]->m_name.c_str(), value) == 0 &&
			m_children[i]->Type() != BXML_ATTRIBUTE)
			return m_children[i];
	}
	return NULL;
}
BiXmlNode* BiBxmlNodeImpl::FirstChild( const char * value )
{
	for (size_t i = 0; i < m_children.size(); ++i)
		if (strcmp(m_children[i]->m_name.c_str(), value) == 0 &&
			m_children[i]->Type() != BXML_ATTRIBUTE)
			return m_children[i];

	return NULL;
}

const BiXmlElement* BiBxmlNodeImpl::FirstChildElement()	const
{
	for (size_t i=0; i<m_children.size(); ++i)
	{
		if (m_children[i]->Type() == BXML_ELEMENT)
		{
			return dynamic_cast<const BiBxmlElementImpl*>(m_children[i]);
		}
	}
	return NULL;
}

BiXmlElement* BiBxmlNodeImpl::FirstChildElement()
{
	return _FirstChildElement();
}

const BiXmlElement* BiBxmlNodeImpl::FirstChildElement( const char * value ) const
{
	for (size_t i=0; i<m_children.size(); ++i)
	{
		if (m_children[i]->Type() == BXML_ELEMENT &&
			strcmp(m_children[i]->m_name.c_str(), value) == 0)
		{
			return dynamic_cast<const BiBxmlElementImpl*>(m_children[i]);
		}
	}
	return NULL;
}

BiXmlElement* BiBxmlNodeImpl::FirstChildElement( const char * value )
{
	for (size_t i=0; i<m_children.size(); ++i)
	{
		if (m_children[i]->Type() == BXML_ELEMENT &&
			strcmp(m_children[i]->m_name.c_str(), value) == 0)
		{
			return dynamic_cast<BiBxmlElementImpl*>(m_children[i]);
		}
	}
	return NULL;
}

const BiXmlDocument* BiBxmlNodeImpl::GetDocument() const
{
	const BiBxmlNodeImpl* node = this;
	for (;node; node = node->m_parent)
	{
		if (node->Type() == BXML_DOCUMENT)
			return dynamic_cast<const BiBxmlDocumentImpl*>(node);
	}

	return NULL;
}

BiXmlDocument* BiBxmlNodeImpl::GetDocument()
{
	BiBxmlNodeImpl* node = this;
	for (;node; node = node->m_parent)
	{
		if (node->Type() == BXML_DOCUMENT)
			return dynamic_cast<BiBxmlDocumentImpl*>(node);
	}

	return NULL;
}

BiXmlNode* BiBxmlNodeImpl::InsertEndChild( const BiXmlNode& addThis )
{
	BiXmlNode* node= addThis.Clone();

	return LinkEndChild(node);
}

BiXmlNode* BiBxmlNodeImpl::InsertEndChild( const char* name )
{
	//BiBxmlElementImpl* elem = elem_pool.alloc();
	BiBxmlElementImpl* elem = BiBxmlCreateElement("");
	elem->SetValue(name);
	BiBxmlNodeImpl* node = elem;
	return LinkEndChild(node);
}

BiXmlNode* BiBxmlNodeImpl::LinkEndChild( BiXmlNode* addThis )
{
	assert(addThis->Parent() == 0 || addThis->Parent() == this);
	assert(addThis->GetDocument() == NULL || addThis->GetDocument() == GetDocument());

	BiBxmlNodeImpl* node = dynamic_cast<BiBxmlNodeImpl*>(addThis);
	AddChild(node);
	return addThis;

}
const BiXmlNode* BiBxmlNodeImpl::NextSibling( ) const
{
	const BiBxmlNodeImpl* node = m_next;
	for (; node; node = node->m_next)
		if (node->Type() != BXML_ATTRIBUTE)
			return node;

	return NULL;
}

BiXmlNode* BiBxmlNodeImpl::NextSibling( )
{
	BiBxmlNodeImpl* node = m_next;
	for (; node; node = node->m_next)
		if (node->Type() != BXML_ATTRIBUTE)
			return node;

	return NULL;
}
const BiXmlNode* BiBxmlNodeImpl::NextSibling( const char * _value) const
{
	const BiBxmlNodeImpl* node = m_next;
	for (; node; node = node->m_next)
		if (strcmp(node->m_name.c_str(), _value) == 0)
			return node;

	return NULL;
}

BiXmlNode* BiBxmlNodeImpl::NextSibling( const char * _value)
{
	BiBxmlNodeImpl* node = m_next;
	for (; node; node = node->m_next)
		if (strcmp(node->m_name.c_str(), _value) == 0)
			return node;

	return NULL;
}

const BiXmlElement* BiBxmlNodeImpl::NextSiblingElement() const
{
	const BiBxmlNodeImpl* node = m_next;
	for (; node; node = node->m_next)
		if (node->Type() == BXML_ELEMENT)
			return dynamic_cast<const BiBxmlElementImpl*>(node);

	return NULL;
}

BiXmlElement* BiBxmlNodeImpl::NextSiblingElement()
{
	BiBxmlNodeImpl* node = m_next;
	for (; node; node = node->m_next)
		if (node->Type() == BXML_ELEMENT)
			return dynamic_cast<BiBxmlElementImpl*>(node);

	return NULL;
}

const BiXmlElement* BiBxmlNodeImpl::NextSiblingElement( const char * _value) const
{
	const BiBxmlNodeImpl* node = m_next;
	for (; node; node = node->m_next)
		if (node->Type() == BXML_ELEMENT &&
			strcmp(node->m_name.c_str(), _value) == 0)
			return dynamic_cast<const BiBxmlElementImpl*>(node);

	return NULL;
}
BiXmlElement* BiBxmlNodeImpl::NextSiblingElement( const char * _value)
{
	BiBxmlNodeImpl* node = m_next;
	for (; node; node = node->m_next)
		if (node->Type() == BXML_ELEMENT &&
			strcmp(node->m_name.c_str(), _value) == 0)
			return dynamic_cast<BiBxmlElementImpl*>(node);

	return NULL;
}
const BiXmlNode* BiBxmlNodeImpl::IterateChildren( const char * value, const BiXmlNode* previous ) const
{
	if ( !previous )
	{
		return FirstChild( value );
	}
	else
	{
		assert( previous->Parent() == this );
		return previous->NextSibling( value );
	}
}
BiXmlNode* BiBxmlNodeImpl::IterateChildren( const char * value, BiXmlNode* previous )
{
	if ( !previous )
	{
		return FirstChild( value );
	}
	else
	{
		assert( previous->Parent() == this );
		return previous->NextSibling( value );
	}
}
//TODO: 待
bool BiBxmlNodeImpl::RemoveChild( BiXmlNode* removeThis )
{
	BiBxmlNodeImpl* node = dynamic_cast<BiBxmlNodeImpl*>(removeThis);
	if ( node->m_parent != this )
	{	
		assert( 0 );
		return false;
	}

	std::vector<BiBxmlNodeImpl*>::iterator itr = std::find(m_children.begin(), m_children.end(), node);
	if (itr == m_children.end())
	{
		assert(0);
	}
	m_children.erase(itr);

	if ( node->m_next )
		node->m_next->m_previous = node->m_previous;
	

	if ( node->m_previous )
		node->m_previous->m_next = node->m_next;

	node->Release();
	return true;
}
/////----------------------------Element-----------
/////----------------------------------------------

void BiBxmlElementImpl::StreamOut( std::ostream* stream )
{
	(*stream) << "<" << m_name;

	BiBxmlAttributeImpl* attrib;
	for ( attrib = FirstAttribute(); attrib; attrib = attrib->NextAttribute())
	{	
		(*stream) << " ";
		attrib->StreamOut( stream );
	}

	// If this node has children, give it a closing tag. Else
	// make it an empty tag.
	BiBxmlElementImpl* node;
	if ( m_children.size() > 0 || m_strText.length() > 0 )
	{ 		
		(*stream) << ">";

		for ( node = _FirstChildElement(); node; node = node->_NextSiblingElement() )
		{
			node->StreamOut( stream );
		}

		PutString( m_strText, stream );

		(*stream) << "</" << m_name << ">";
	}
	else
	{
		(*stream) << " />";
	}
}
BiBxmlElementImpl::BiBxmlElementImpl()
{
}

BiBxmlElementImpl::BiBxmlElementImpl(const char* name)
{
	m_name = name;
}

BiXmlNode* BiBxmlElementImpl::Clone()const
{
	//BiBxmlElementImpl* elem = elem_pool.alloc();
	BiBxmlElementImpl* elem = BiBxmlCreateElement("");
	elem->SetValue(Value());
	elem->SetText(GetText());
	for (size_t i=0; i<m_children.size(); ++i)
	{
		elem->LinkEndChild(m_children[i]->Clone());
	}
	BiBxmlNodeImpl* node = elem;
	return node;
}
const char* BiBxmlElementImpl::Attribute(const char* name)const
{
	for (size_t i=0; i<m_children.size(); ++i)
	{
		if (m_children[i]->Type() == BXML_ATTRIBUTE && 
			strcmp(m_children[i]->Value(), name) == 0)
		{
			BiBxmlAttributeImpl* attri = dynamic_cast<BiBxmlAttributeImpl*>(m_children[i]);
			return attri->AttValue();
		}
	}
	return NULL;
}
const char* BiBxmlElementImpl::Attribute(const char* name, int* nValue)const
{
	for (size_t i=0; i<m_children.size(); ++i)
	{
		if (m_children[i]->Type() == BXML_ATTRIBUTE && 
			strcmp(m_children[i]->Value(), name) == 0)
		{
			BiBxmlAttributeImpl* attri = dynamic_cast<BiBxmlAttributeImpl*>(m_children[i]);
			*nValue = atoi(attri->AttValue());
			return attri->AttValue();
		}
	}
	return NULL;
}
const char* BiBxmlElementImpl::Attribute(const char* name, double* d)const
{
	for (size_t i=0; i<m_children.size(); ++i)
	{
		if (m_children[i]->Type() == BXML_ATTRIBUTE && 
			strcmp(m_children[i]->Value(), name) == 0)
		{
			BiBxmlAttributeImpl* attri = dynamic_cast<BiBxmlAttributeImpl*>(m_children[i]);
			*d = atof(attri->AttValue());
			return attri->AttValue();
		}
	}
	return NULL;
}

const char* BiBxmlElementImpl::GetText() const
{
	return m_strText.c_str();
}
bool BiBxmlElementImpl::SetText(const char* strText)
{
	m_strText = strText;
	return true;
}
bool BiBxmlElementImpl::AddText(const char* strText)
{
	m_strText = strText;
	return true;
}

void BiBxmlElementImpl::SetAttribute( const char* name, const char * _value )
{
	for (size_t i=0; i<m_children.size(); ++i)
	{
		if (m_children[i]->Type() == BXML_ATTRIBUTE && 
			strcmp(m_children[i]->Value(), name) == 0)
		{
			BiBxmlAttributeImpl* attri = dynamic_cast<BiBxmlAttributeImpl*>(m_children[i]);
			attri->SetAttValue(_value);
			return;
		}
	}
	//BiBxmlAttributeImpl* attrib = attrib_pool.alloc();
	BiBxmlAttributeImpl* attrib = BiBxmlCreateAttribute();
	attrib->SetValue(name);
	attrib->SetAttValue(_value);
	
	AddChild(attrib);
}
void BiBxmlElementImpl::SetAttribute( const char * name, int value )
{
	char buf[64];
	sprintf_s( buf, 64, "%d", value );
	SetAttribute(name, buf);
}
void BiBxmlElementImpl::SetDoubleAttribute( const char * name, double value )
{
	char buf[256];
	sprintf_s( buf, 256, "%f", value );
	SetAttribute(name, buf);
}

int BiBxmlElementImpl::QueryIntAttribute( const char* name, int* _value ) const
{
	for (size_t i=0; i<m_children.size(); ++i)
	{
		if (m_children[i]->Type() == BXML_ATTRIBUTE && 
			strcmp(m_children[i]->Value(), name) == 0)
		{
			BiBxmlAttributeImpl* attri = dynamic_cast<BiBxmlAttributeImpl*>(m_children[i]);
			if ( sscanf_s( attri->AttValue(), "%d", _value ) == 1 )
				return 0;
			return -2;
		}
	}
	return -1;
}
int BiBxmlElementImpl::QueryFloatAttribute( const char* name, float* _value ) const
{
	for (size_t i=0; i<m_children.size(); ++i)
	{
		if (m_children[i]->Type() == BXML_ATTRIBUTE && 
			strcmp(m_children[i]->Value(), name) == 0)
		{
			BiBxmlAttributeImpl* attri = dynamic_cast<BiBxmlAttributeImpl*>(m_children[i]);
			if ( sscanf_s( attri->AttValue(), "%f", _value ) == 1 )
				return 0;
			return -2;
		}
	}
	return -1;
}

BiBxmlAttributeImpl* BiBxmlElementImpl::FirstAttribute()
{
	for (size_t i=0; i<m_children.size(); ++i)
	{
		if (m_children[i]->Type() == BXML_ATTRIBUTE)
		{
			BiBxmlAttributeImpl* attri = dynamic_cast<BiBxmlAttributeImpl*>(m_children[i]);
			return attri;
		}
	}
	return NULL;
}


////////------------------------------------document--------------
////////-----------------------------------------------------------
BiXmlNode* BiBxmlDocumentImpl::Clone()const 
{
	//BiBxmlDocumentImpl* doc = doc_pool.alloc();
	BiBxmlDocumentImpl* doc = BiBxmlCreateDocument();
	doc->SetValue(Value());
	for (size_t i=0; i<m_children.size(); ++i)
	{
		doc->LinkEndChild(m_children[i]->Clone());
	}
	BiBxmlNodeImpl* node = doc;
	return node;
}

//先从pack中加载，如果pack中没有则再从文件系统中加载
bool BiBxmlDocumentImpl::LoadFile( const char* filename, BiXmlEncoding encoding/* = BIXML_ENCODING_UNKNOWN */)
{
	if (m_reader.LoadFileFromPack(filename))
		return true;
	return m_reader.LoadFromFile(filename);
}
bool BiBxmlDocumentImpl::LoadFile( FILE* file, BiXmlEncoding encoding/* = BIXML_ENCODING_UNKNOWN*/)
{
	return m_reader.LoadFromFile(file);
}
const BiXmlElement* BiBxmlDocumentImpl::RootElement() const
{
	return BiBxmlNodeImpl::FirstChildElement();
}
BiXmlElement* BiBxmlDocumentImpl::RootElement()
{
	return BiBxmlNodeImpl::FirstChildElement();
}

bool BiBxmlDocumentImpl::SaveFile( const char * filename)
{
	if (!m_dictbuf)
		m_dictbuf = new char[BinXMLReader::DICT_BUFLEN];

	if (!m_writer)
	{
		//保存二进制xml时由于不知道含有的字符串数量，所以初始化为最大字典长度
		m_dict.init(string_dictionary::MAX_STRING);
		m_writer = new BinXMLWriter(BinXMLWriter::WRITE_BUFF, &m_dict);
	}

	if (m_writer && m_dictbuf)
	{
		save_binary(this, 0);
		m_header.dict_len = m_dict.save(m_dictbuf, GetDictBuffLength() );
		save_to_file(filename);
		return true;
	}
	
	return false;
}

//暂不支持错误描述
const char * BiBxmlDocumentImpl::ErrorDesc() const
{
	return NULL;
}

//二进制xml没有声明部分
bool BiBxmlDocumentImpl::InsertDeclaration(const char* _version, const char* _encoding, const char* _standalone)
{
	return true;
}

const char* BiBxmlDocumentImpl::ParseXmlStr(const char* strContent)
{
	return NULL;
}
const char* BiBxmlDocumentImpl::Parse(const char* strContent)
{
	return ParseXmlStr(strContent);
}

bool BiBxmlDocumentImpl::Error()const
{
	return false;
}