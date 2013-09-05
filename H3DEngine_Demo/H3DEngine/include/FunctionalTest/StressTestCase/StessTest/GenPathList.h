#include "tinystr.h"
#include "tinyxml.h"
#include "../CommonCode/HippoUtil/FileOperation.h"
#include "../CommonCode/HippoUtil/HippoGlobal.h"
#include <vector>
using namespace std;
//!这个文件主要是生成资源列表
//！第一次加载的时候没有资源列表文件此时会创建一个列表文件
//！之后在运行的时候都可以直接加载创建好的列表文件
bool GenPathList( vector<std::string>& m_pDmlPathList,std::string ErgodicDir);