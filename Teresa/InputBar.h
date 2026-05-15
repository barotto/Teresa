/*//////////////////////////////////////////////////////////////////////////////
	FILENAME:	InputBar.h
	Version:	1.0.0
	Author:		Marco Bortolin
	Created:	27/07/2001 - 23.41.40

	Copyright (c) 2001 Marco Bortolin
//////////////////////////////////////////////////////////////////////////////*/

#if !defined(AFX_INPUTBAR_H__E873024F_E0B3_45C1_8324_6E1C29B16254__INCLUDED_)
#define AFX_INPUTBAR_H__E873024F_E0B3_45C1_8324_6E1C29B16254__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "InputEdit.h"

class CInputBar : public CToolBar
{

// Construction
public:
	CInputBar();

// Attributes
public:
protected:
	CInputEdit	m_edit;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CInputBar)
	//}}AFX_VIRTUAL

// Implementation
public:
	void Resize(int x, int y);
	virtual ~CInputBar();
	bool Create(CWnd* inParentWnd);
	CEdit* GetEditCtrl() { return &m_edit; }
	void PrintText(const char* inMex) { m_edit.SetWindowText(inMex); }

	// Generated message map functions
protected:
	//{{AFX_MSG(CInputBar)
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INPUTBAR_H__E873024F_E0B3_45C1_8324_6E1C29B16254__INCLUDED_)
