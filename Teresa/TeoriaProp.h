/*//////////////////////////////////////////////////////////////////////////////
	FILENAME:	TeoriaProp.h
	Version:	1.0.0
	Author:		Marco Bortolin
	Created:	27/07/2001 - 23.41.40

	Copyright (c) 2001 Marco Bortolin
//////////////////////////////////////////////////////////////////////////////*/

#if !defined(AFX_TEORIAPROP_H__94A8BA93_C69E_456C_B573_7A2B5DDA493C__INCLUDED_)
#define AFX_TEORIAPROP_H__94A8BA93_C69E_456C_B573_7A2B5DDA493C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CTeoriaProp : public CPropertyPage
{
	DECLARE_DYNCREATE(CTeoriaProp)
	
	CFont m_font;

// Construction
public:
	CString m_descstr;
	CTeoriaProp();
	~CTeoriaProp();

	trsSimArityPairs *m_signature;
	trsVarList *m_varlist;
	bool m_ordervalid;

// Dialog Data
	//{{AFX_DATA(CTeoriaProp)
	enum { IDD = IDD_TEORIAPROP };
	CStatic	m_numvar_ctrl;
	CListBox	m_varlist_ctrl;
	CListBox	m_funclist_ctrl;
	CEdit	m_order_ctrl;
	CStatic	m_numfunc;
	CString	m_numfunc_str;
	CString	m_order;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CTeoriaProp)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CTeoriaProp)
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnTeoriapropDesc();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	bool m_bFirstShow;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TEORIAPROP_H__94A8BA93_C69E_456C_B573_7A2B5DDA493C__INCLUDED_)
