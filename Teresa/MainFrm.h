/*//////////////////////////////////////////////////////////////////////////////
	FILENAME:	MainFrm.h
	Version:	1.0.0
	Author:		Marco Bortolin
	Created:	27/07/2001 - 23.41.40

	Copyright (c) 2001 Marco Bortolin
//////////////////////////////////////////////////////////////////////////////*/

#if !defined(AFX_MAINFRM_H__9EA719E7_5C30_45B4_A760_D1D19E1A1BD4__INCLUDED_)
#define AFX_MAINFRM_H__9EA719E7_5C30_45B4_A760_D1D19E1A1BD4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "OutputBar.h"

class CMainFrame : public CMDIFrameWnd
{
	DECLARE_DYNAMIC(CMainFrame)
public:
	CMainFrame();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
	CStatusBar  m_wndStatusBar;
	CToolBar    m_wndToolBar;
	COutputBar  m_wndOutputBar;

// Generated message map functions
protected:
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnViewOutputbar();
	afx_msg void OnUpdateViewOutputbar(CCmdUI* pCmdUI);
	afx_msg void OnClose();
	afx_msg void OnOutputeditClear();
	afx_msg void OnOutputeditCopy();
	afx_msg void OnUpdateOutputeditClear(CCmdUI* pCmdUI);
	afx_msg void OnUpdateOutputeditCopy(CCmdUI* pCmdUI);
	afx_msg void OnOutputeditCopyall();
	afx_msg void OnUpdateOutputeditCopyall(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__9EA719E7_5C30_45B4_A760_D1D19E1A1BD4__INCLUDED_)
