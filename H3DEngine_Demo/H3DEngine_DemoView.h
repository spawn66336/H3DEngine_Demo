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

// H3DEngine_DemoView.h : CH3DEngine_DemoView 类的接口
//

#pragma once


namespace ZPH3D
{
	class H3DEngineBox;
}

class CH3DEngine_DemoView : public CView
{
protected: // 仅从序列化创建
	CH3DEngine_DemoView();
	DECLARE_DYNCREATE(CH3DEngine_DemoView)

// 特性
public:
	CH3DEngine_DemoDoc* GetDocument() const;


// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 实现
public:
	virtual ~CH3DEngine_DemoView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected: 

	ZPH3D::H3DEngineBox* m_pH3DBox;  //引擎容器
	BOOL m_bDragFlag;
	CPoint m_mousePoint;

public:

	void RenderOneFrame( void );

protected:

// 生成的消息映射函数
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

#ifndef _DEBUG  // H3DEngine_DemoView.cpp 中的调试版本
inline CH3DEngine_DemoDoc* CH3DEngine_DemoView::GetDocument() const
   { return reinterpret_cast<CH3DEngine_DemoDoc*>(m_pDocument); }
#endif

