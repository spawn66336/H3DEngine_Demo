// ��� MFC ʾ��Դ������ʾ���ʹ�� MFC Microsoft Office Fluent �û����� 
// (��Fluent UI��)����ʾ�������ο���
// ���Բ��䡶Microsoft ������ο����� 
// MFC C++ ������渽����ص����ĵ���
// ���ơ�ʹ�û�ַ� Fluent UI ����������ǵ����ṩ�ġ�
// ��Ҫ�˽��й� Fluent UI ��ɼƻ�����ϸ��Ϣ�������  
// http://msdn.microsoft.com/officeui��
//
// ��Ȩ����(C) Microsoft Corporation
// ��������Ȩ����

// H3DEngine_DemoView.cpp : CH3DEngine_DemoView ���ʵ��
//

#include "stdafx.h"
// SHARED_HANDLERS ������ʵ��Ԥ��������ͼ������ɸѡ�������
// ATL ��Ŀ�н��ж��壬�����������Ŀ�����ĵ����롣
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
	// ��׼��ӡ����
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CH3DEngine_DemoView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()

// CH3DEngine_DemoView ����/����

CH3DEngine_DemoView::CH3DEngine_DemoView()
{
	// TODO: �ڴ˴���ӹ������

}

CH3DEngine_DemoView::~CH3DEngine_DemoView()
{
}

BOOL CH3DEngine_DemoView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	return CView::PreCreateWindow(cs);
}

// CH3DEngine_DemoView ����

void CH3DEngine_DemoView::OnDraw(CDC* /*pDC*/)
{
	CH3DEngine_DemoDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: �ڴ˴�Ϊ����������ӻ��ƴ���
}


// CH3DEngine_DemoView ��ӡ


void CH3DEngine_DemoView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CH3DEngine_DemoView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// Ĭ��׼��
	return DoPreparePrinting(pInfo);
}

void CH3DEngine_DemoView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӷ���Ĵ�ӡǰ���еĳ�ʼ������
}

void CH3DEngine_DemoView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӵ�ӡ����е��������
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


// CH3DEngine_DemoView ���

#ifdef _DEBUG
void CH3DEngine_DemoView::AssertValid() const
{
	CView::AssertValid();
}

void CH3DEngine_DemoView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CH3DEngine_DemoDoc* CH3DEngine_DemoView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CH3DEngine_DemoDoc)));
	return (CH3DEngine_DemoDoc*)m_pDocument;
}
#endif //_DEBUG


// CH3DEngine_DemoView ��Ϣ�������
