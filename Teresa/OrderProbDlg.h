/*//////////////////////////////////////////////////////////////////////////////
	FILENAME:	OrderProbDlg.h
	Version:	1.0.0
	Author:		Marco Bortolin
	Created:	27/07/2001 - 23.41.40

	Copyright (c) 2001 Marco Bortolin
//////////////////////////////////////////////////////////////////////////////*/

#if !defined(AFX_ORDERPROBDLG_H__C17DC054_8B27_4EF3_B427_AF882EE41C10__INCLUDED_)
#define AFX_ORDERPROBDLG_H__C17DC054_8B27_4EF3_B427_AF882EE41C10__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class COrderProbDlg : public CDialog
{
	CFont m_font;

// Construction
public:
	COrderProbDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(COrderProbDlg)
	enum { IDD = IDD_ORDER_PROB };
	CEdit	m_edit_ctrl;
	CString	m_edit;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COrderProbDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(COrderProbDlg)
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnRetry();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ORDERPROBDLG_H__C17DC054_8B27_4EF3_B427_AF882EE41C10__INCLUDED_)
