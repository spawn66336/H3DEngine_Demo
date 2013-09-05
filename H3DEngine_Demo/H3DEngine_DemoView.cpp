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
#include "ZPDependency.h"
#include "H3DEngineBox.h"

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
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_PAINT()
END_MESSAGE_MAP()

// CH3DEngine_DemoView ����/����

CH3DEngine_DemoView::CH3DEngine_DemoView(): 
m_pH3DBox(NULL)
{ 

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
	return DoPreparePrinting(pInfo);
}

void CH3DEngine_DemoView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{ 
}

void CH3DEngine_DemoView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{ 
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
int CH3DEngine_DemoView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	//������������
	m_pH3DBox = new ZPH3D::H3DEngineBox;
	ZP_ASSERT( NULL != m_pH3DBox );
	m_pH3DBox->Init( this->GetSafeHwnd() ); 
	return 0;
}


void CH3DEngine_DemoView::OnDestroy()
{
	CView::OnDestroy(); 
	//������������
	m_pH3DBox->Destroy();
	 ZP_SAFE_DELETE( m_pH3DBox );
}


void CH3DEngine_DemoView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);
	m_pH3DBox->Resize();
}


void CH3DEngine_DemoView::OnPaint()
{
	CPaintDC dc(this);   
	this->RenderOneFrame();
}

void CH3DEngine_DemoView::RenderOneFrame( void )
{
	m_pH3DBox->RenderOneFrame();
}
