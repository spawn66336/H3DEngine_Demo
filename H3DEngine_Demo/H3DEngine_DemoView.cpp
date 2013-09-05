// 这段 MFC 示例源代码演示如何使用 MFC Microsoft Office Fluent 用户界面 
// (“Fluent UI”)。该示例仅供参考，
// 用以补充《Microsoft 基础类参考》和 
// MFC C++ 库软件随附的相关电子文档。
// 复制、使用或分发 Fluent UI 的许可条款是单独提供的。
// 若要了解有关 Fluent UI 许可计划的详细信息，请访问  
// http://msdn.microsoft.com/officeui。
//
// 版权所有(C) Microsoft Corporation
// 保留所有权利。

// H3DEngine_DemoView.cpp : CH3DEngine_DemoView 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "H3DEngine_Demo.h"
#endif

#include "H3DEngine_DemoDoc.h"
#include "H3DEngine_DemoView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CH3DEngine_DemoView

IMPLEMENT_DYNCREATE(CH3DEngine_DemoView, CView)

BEGIN_MESSAGE_MAP(CH3DEngine_DemoView, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CH3DEngine_DemoView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()

// CH3DEngine_DemoView 构造/析构

CH3DEngine_DemoView::CH3DEngine_DemoView()
{
	// TODO: 在此处添加构造代码

}

CH3DEngine_DemoView::~CH3DEngine_DemoView()
{
}

BOOL CH3DEngine_DemoView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CH3DEngine_DemoView 绘制

void CH3DEngine_DemoView::OnDraw(CDC* /*pDC*/)
{
	CH3DEngine_DemoDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
}


// CH3DEngine_DemoView 打印


void CH3DEngine_DemoView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CH3DEngine_DemoView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CH3DEngine_DemoView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CH3DEngine_DemoView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}

void CH3DEngine_DemoView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CH3DEngine_DemoView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CH3DEngine_DemoView 诊断

#ifdef _DEBUG
void CH3DEngine_DemoView::AssertValid() const
{
	CView::AssertValid();
}

void CH3DEngine_DemoView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CH3DEngine_DemoDoc* CH3DEngine_DemoView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CH3DEngine_DemoDoc)));
	return (CH3DEngine_DemoDoc*)m_pDocument;
}
#endif //_DEBUG


// CH3DEngine_DemoView 消息处理程序
