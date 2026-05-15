/*//////////////////////////////////////////////////////////////////////////////
	FILENAME:	ChildFrm.h
	Version:	1.0.0
	Author:		Marco Bortolin
	Created:	27/07/2001 - 23.41.40

	Copyright (c) 2001 Marco Bortolin
//////////////////////////////////////////////////////////////////////////////*/

#if !defined(AFX_CHILDFRM_H__322D9EF9_AB5B_44D5_88D3_FD1A67FFCCE5__INCLUDED_)
#define AFX_CHILDFRM_H__322D9EF9_AB5B_44D5_88D3_FD1A67FFCCE5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "InputBar.h"
#include "MainFrm.h"

class CChildFrame : public CMDIChildWnd
{
	DECLARE_DYNCREATE(CChildFrame)
public:
	CChildFrame();

// Attributes
public:
protected:
	CSplitterWnd m_wndSplitter;
	CInputBar	 m_wndInputbar;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CChildFrame)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void ActivateFrame(int nCmdShow = -1);
	protected:
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	//}}AFX_VIRTUAL

// Implementation
public:
	CMainFrame* GetMainFrame();
	CInputBar* GetInputBar();
	CView* GetRightView();
	CView* GetSIDView();
	bool ReplaceRightView(CRuntimeClass *pViewClass);

	virtual ~CChildFrame();

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

// Generated message map functions
protected:
	//{{AFX_MSG(CChildFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnInputOK();
	afx_msg void OnInputCancel();
	afx_msg void OnNuovaCoppia();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnTermsView();
	afx_msg void OnTrsView();
	afx_msg void OnEditUndo();
	afx_msg void OnUpdateEditUndo(CCmdUI* pCmdUI);
	afx_msg void OnUpdateInputOK(CCmdUI* pCmdUI);
	afx_msg void OnUpdateInputCancel(CCmdUI* pCmdUI);
	afx_msg void OnUpdateNuovaCoppia(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHILDFRM_H__322D9EF9_AB5B_44D5_88D3_FD1A67FFCCE5__INCLUDED_)
