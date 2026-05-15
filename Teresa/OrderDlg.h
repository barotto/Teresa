/*//////////////////////////////////////////////////////////////////////////////
	FILENAME:	OrderDlg.h
	Version:	1.0.0
	Author:		Marco Bortolin
	Created:	27/07/2001 - 23.41.40

	Copyright (c) 2001 Marco Bortolin
//////////////////////////////////////////////////////////////////////////////*/

#if !defined(AFX_ORDERDLG_H__B359720D_5E82_4F83_92EE_0C0D38D552EB__INCLUDED_)
#define AFX_ORDERDLG_H__B359720D_5E82_4F83_92EE_0C0D38D552EB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class COrderDlg : public CDialog
{
	CFont m_font;

// Construction
public:
	COrderDlg(CWnd* pParent = NULL);   // standard constructor
	trsSimArityPairs *m_signature;

// Dialog Data
	//{{AFX_DATA(COrderDlg)
	enum { IDD = IDD_ORDER };
	CStatic	m_ordop_ctrl;
	CListBox	m_signlist_ctrl;
	CEdit	m_order_ctrl;
	CString	m_order;
	CString	m_signlist;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COrderDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(COrderDlg)
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnDblclkSignature();
	afx_msg void OnGreater();
	afx_msg void OnEqual();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ORDERDLG_H__B359720D_5E82_4F83_92EE_0C0D38D552EB__INCLUDED_)
