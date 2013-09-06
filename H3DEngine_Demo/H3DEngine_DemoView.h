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

// H3DEngine_DemoView.h : CH3DEngine_DemoView ��Ľӿ�
//

#pragma once


namespace ZPH3D
{
	class H3DEngineBox;
}

class CH3DEngine_DemoView : public CView
{
protected: // �������л�����
	CH3DEngine_DemoView();
	DECLARE_DYNCREATE(CH3DEngine_DemoView)

// ����
public:
	CH3DEngine_DemoDoc* GetDocument() const;


// ��д
public:
	virtual void OnDraw(CDC* pDC);  // ��д�Ի��Ƹ���ͼ
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// ʵ��
public:
	virtual ~CH3DEngine_DemoView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected: 

	ZPH3D::H3DEngineBox* m_pH3DBox;  //��������
	BOOL m_bDragFlag;
	CPoint m_mousePoint;

public:

	void RenderOneFrame( void );

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
};

#ifndef _DEBUG  // H3DEngine_DemoView.cpp �еĵ��԰汾
inline CH3DEngine_DemoDoc* CH3DEngine_DemoView::GetDocument() const
   { return reinterpret_cast<CH3DEngine_DemoDoc*>(m_pDocument); }
#endif

