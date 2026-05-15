/*//////////////////////////////////////////////////////////////////////////////
	FILENAME:	TermsView.h
	Version:	1.0.0
	Author:		Marco Bortolin
	Created:	27/07/2001 - 23.41.40

	Copyright (c) 2001 Marco Bortolin
//////////////////////////////////////////////////////////////////////////////*/

#if !defined(AFX_TERMSVIEW_H__2DFEEAD9_353C_43B5_98DE_C154E78B5265__INCLUDED_)
#define AFX_TERMSVIEW_H__2DFEEAD9_353C_43B5_98DE_C154E78B5265__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CTermsView : public CTreeView
{
protected:
	CTermsView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CTermsView)

// Attributes
public:
protected:
	CFont m_font;

// Operations
public:
	void Sincronizza();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTermsView)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CTermsView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	afx_msg void OnContextMenu(CWnd*, CPoint point);
	//{{AFX_MSG(CTermsView)
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TERMSVIEW_H__2DFEEAD9_353C_43B5_98DE_C154E78B5265__INCLUDED_)
