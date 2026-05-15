/*//////////////////////////////////////////////////////////////////////////////
	FILENAME:	OutputEdit.h
	Version:	1.0.0
	Author:		Marco Bortolin
	Created:	27/07/2001 - 23.41.40

	Copyright (c) 2001 Marco Bortolin
//////////////////////////////////////////////////////////////////////////////*/

#if !defined(AFX_OUTPUTEDIT_H__859BE20A_0D06_4AB4_ACEC_E831EA4B2D62__INCLUDED_)
#define AFX_OUTPUTEDIT_H__859BE20A_0D06_4AB4_ACEC_E831EA4B2D62__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class COutputEdit : public CRichEditCtrl, public trsOutputDev
{
	CFont m_font;

// Construction
public:
	COutputEdit();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COutputEdit)
	//}}AFX_VIRTUAL

// Implementation
public:
	void Print(CString &_str);
	void Print(const char *_str);

	virtual ~COutputEdit();
		
	void printf(const char* _format, ...);
	
	trsOutputDev &operator <<(string _src);
	trsOutputDev &operator <<(const char* _src);
	trsOutputDev &operator <<(int _src);

	// Generated message map functions
protected:
	//{{AFX_MSG(COutputEdit)
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnOutputeditClear();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnOutputeditCopy();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnOutputeditCopyall();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OUTPUTEDIT_H__859BE20A_0D06_4AB4_ACEC_E831EA4B2D62__INCLUDED_)
