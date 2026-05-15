/*//////////////////////////////////////////////////////////////////////////////
	FILENAME:	DescDlg.h
	Version:	1.0.0
	Author:		Marco Bortolin
	Created:	25/09/2001 - 12.23.49
	
	Copyright (C) 2001 Marco Bortolin
//////////////////////////////////////////////////////////////////////////////*/
#if !defined(AFX_DESCDLG_H__8854CAD8_0873_4FFF_8E34_59565B309DB8__INCLUDED_)
#define AFX_DESCDLG_H__8854CAD8_0873_4FFF_8E34_59565B309DB8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CDescDlg dialog

class CDescDlg : public CDialog
{
// Construction
public:
	CDescDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDescDlg)
	enum { IDD = IDD_DESCRIZIONE };
	CEdit	m_edit_ctrl;
	CString	m_edit;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDescDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CFont m_font;

	// Generated message map functions
	//{{AFX_MSG(CDescDlg)
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnFormatFont();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DESCDLG_H__8854CAD8_0873_4FFF_8E34_59565B309DB8__INCLUDED_)
