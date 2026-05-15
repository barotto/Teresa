/*//////////////////////////////////////////////////////////////////////////////
	FILENAME:	TRSView.h
	Version:	1.0.0
	Author:		Marco Bortolin
	Created:	27/07/2001 - 23.41.40

	Copyright (c) 2001 Marco Bortolin
//////////////////////////////////////////////////////////////////////////////*/

#if !defined(AFX_TRSVIEW_H__010A5C44_824A_491E_8218_7BC0F21D9891__INCLUDED_)
#define AFX_TRSVIEW_H__010A5C44_824A_491E_8218_7BC0F21D9891__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TeresaDoc.h"
#include "ChildFrm.h"

class CTRSView : public CListView
{
protected:
	CTRSView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CTRSView)

// Attributes
public:
protected:
	CFont m_font;

// Operations
public:
	CChildFrame* GetChildFrame();
	void Sincronizza();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTRSView)
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual void OnInitialUpdate(); // called first time after construct
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CTRSView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	afx_msg void OnContextMenu(CWnd*, CPoint point);
	//{{AFX_MSG(CTRSView)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TRSVIEW_H__010A5C44_824A_491E_8218_7BC0F21D9891__INCLUDED_)
