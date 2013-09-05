#include "tinyutil.h"

const char* BiXmlUtility::get_value(const char* key, BiXmlElement *item)
{
	if(BiXmlElement* subitem = item->FirstChildElement(key))
	{
		return BiXmlUtility::get_value(subitem);
	}
	else
		return 0;
}

const char* BiXmlUtility::get_value(BiXmlElement *item)
{
	return item->GetText();
	/*BiXmlNode *childNode = item->FirstChild();
	if ( childNode == NULL )
		return 0;
	BiXmlText *childText = childNode->ToText();
	if ( childText == NULL )
		return 0;

	return childText->Value();*/
}

const char* BiXmlUtility::get_attribute(BiXmlNode* node, const char* key)
{
	BiXmlElement* ele = node->ToElement();
	if(ele)
	{
		return ele->Attribute(key);
	}
	else
		return 0;
}
//16�����ַ���ת��������
static unsigned char a16ToByte(const char* str)
{
	if (!str) return 0;

	//ֻҪ��һ���ַ�Ϊ�ַ�����������ʱ������0
	if (!str[0]) return 0;
	if (!str[1]) return 0;

	unsigned char result =  0;
	//��ø�λ��ֵ
	if (str[0] >= '0' && str[0] <= '9')
	{
		result = ((str[0] - '0') << 4) & 0xF0;
	}
	else if (str[0] >= 'A' && str[0] <= 'F')
	{
		result = ((10 + str[0] - 'A') << 4) & 0xF0;
	}
	else if (str[0] >= 'a' && str[0] <= 'f')
	{
		result = ((10 + str[0] - 'a') << 4) & 0xF0;
	}
	else
	{
		return 0;
	}
	//��õ�λ��ֵ
	if (str[1] >= '0' && str[1] <= '9')
	{
		result = result | ((str[1] - '0') & 0x0F);
	}
	else if (str[1] >= 'A' && str[1] <= 'F')
	{
		result = result | ((10 + str[1] - 'A') & 0x0F);
	}
	else if (str[1] >= 'a' && str[1] <= 'f')
	{
		result = result | ((10 + str[1] - 'a') & 0x0F);
	}
	else
	{
		return 0;
	}

	return result;
}
//�ڲ������str�������ȣ�ֻ��д���������ⲿ��֤��
//str�������������3���ֽڣ���������Խ���д
//ÿ�κ������û���str[0],str[1],��д2���ַ���Ȼ����str[2]д��'\0'
static bool byteToa16(unsigned char c, char* str)
{
	if (!str) return false;

	str[0] = '0';
	str[1] = '0';
	str[2] = 0;

	unsigned char ch = (c >> 4) & 0x0F;
	unsigned char cl = c & 0x0F;
	if (ch >= 0 && ch <= 9)
	{
		str[0] = '0' + ch;
	}
	else if (ch >= 10 && ch <= 15)
	{
		str[0] = 'A' + (ch - 10);
	}

	if (cl >= 0 && cl <= 9)
	{
		str[1] = '0' + cl;
	}
	else if (cl >= 10 && cl <= 15)
	{
		str[1] = 'A' + (cl - 10);
	}
	return true;
}
static const char* DataLenAttrName = "Len";
static const char* DataNodeName = "Data";
bool BiXmlUtility::insertBinaryData(BiXmlElement* parent, const char* keyname, int len, const unsigned char* data)
{
	if (!parent) return false;
	if (!keyname) return false;
	if (!data) return false;
	if (len <= 0) return false;

	//����ڵ�
	BiXmlElement* keynode = parent->InsertEndChild(keyname)->ToElement(); 
	//д�볤������
	//BiXmlElement keynode(keyname);
	keynode->SetAttribute(DataLenAttrName, len);
	//�����м��ڴ�
	char tempbuf[256 + 2] = {0};			
	char* tempp = tempbuf;
	int   tempacc = 0;
	//д��ÿ��2������������
	for (int i = 0; i < len; i++)
	{							
		byteToa16(data[i], tempp);
		tempp += 2;
		tempacc += 2;
		if (tempacc >= 256)
		{				
			tempbuf[tempacc] = '\0';
			tempp = tempbuf;
			tempacc = 0;	

			/*BiXmlElement datanode(DataNodeName);

			BiXmlText textnode(tempbuf);
			datanode.InsertEndChild(textnode);

			keynode.InsertEndChild(datanode);*/
			BiXmlElement* datanode = keynode->InsertEndChild(DataNodeName)->ToElement();
			datanode->SetText(tempbuf);
		}
	}		
	//д��ʣ��2������������
	if (tempacc > 0)
	{
		tempbuf[tempacc] = '\0';
		tempp = tempbuf;
		tempacc = 0;

		/*BiXmlElement datanode(DataNodeName);

		BiXmlText textnode(tempbuf);
		datanode.InsertEndChild(textnode);

		keynode.InsertEndChild(datanode);*/
		BiXmlElement* datanode = keynode->InsertEndChild(DataNodeName)->ToElement();
		datanode->SetText(tempbuf);
	}
	//����ڵ�
	//parent->InsertEndChild(keynode);

	return true;
}
int BiXmlUtility::getBinaryDataLen(BiXmlNode* node)
{
	if (!node) return 0;
	BiXmlElement* ele = node->ToElement();
	if(!ele) return 0;

	int result = 0;
	ele->Attribute(DataLenAttrName, &result);
	return result;
}
bool BiXmlUtility::getBinaryData(BiXmlNode* node, int len, unsigned char* data)
{
	if (!node) return false;
	if (!data) return false;
	if (len <= 0) return false;
	//��ʱ���Դ���len���м�⡣
	//���ж�ȡ
	int tempacc = 0;
	BiXmlNode* datanode = node->FirstChild(DataNodeName);
	for (; datanode; datanode = datanode->NextSibling(DataNodeName))
	{
		BiXmlElement* ele = datanode->ToElement();
		if (!ele) continue;

		if(const char* v = get_value(ele))
		{
			const char* temp = v;
			//���2�������ݵ�2λ�Ƿ�Ϊ�ַ�����������
			//����ۻ������Ƿ񳬹�������
			while (temp[0] && temp[1] && tempacc < len)
			{
				data[tempacc] = a16ToByte(temp);
				tempacc++;
				temp += 2;
			}							
		}						
	}

	return true;
}
