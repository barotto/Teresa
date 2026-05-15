/*//////////////////////////////////////////////////////////////////////////////
	FILENAME:	EquProp.h
	Version:	1.0.0
	Author:		Marco Bortolin
	Created:	27/07/2001 - 23.41.40

	Copyright (c) 2001 Marco Bortolin
//////////////////////////////////////////////////////////////////////////////*/

#if !defined(AFX_EQUPROP_H__FC506615_1000_4981_9174_D0AB9271DB6A__INCLUDED_)
#define AFX_EQUPROP_H__FC506615_1000_4981_9174_D0AB9271DB6A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <trs.h>

class CEquProp : public CPropertyPage
{
	DECLARE_DYNCREATE(CEquProp)

private:
	void BuildTree(HTREEITEM _parent, trsTerm *_term);
	
	CFont m_font;
	bool m_bFirstShow;
	trsParser m_parser;
	trsSystem m_sys;

// Construction
public:
	string m_equation;

	CEquProp();
	~CEquProp();

// Dialog Data
	//{{AFX_DATA(CEquProp)
	enum { IDD = IDD_EQUPROP };
	CEdit	m_text_ctrl;
	CTreeCtrl	m_tree;
	CString	m_text;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CEquProp)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CEquProp)
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EQUPROP_H__FC506615_1000_4981_9174_D0AB9271DB6A__INCLUDED_)
