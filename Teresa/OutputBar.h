/*//////////////////////////////////////////////////////////////////////////////
	FILENAME:	OutputBar.h
	Version:	1.0.0
	Author:		Marco Bortolin
	Created:	27/07/2001 - 23.41.40

	Copyright (c) 2001 Marco Bortolin
//////////////////////////////////////////////////////////////////////////////*/

#if !defined(AFX_OUTPUTBAR_H__72FF2125_B73D_46F0_8F7E_CA4A6A2D6B17__INCLUDED_)
#define AFX_OUTPUTBAR_H__72FF2125_B73D_46F0_8F7E_CA4A6A2D6B17__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "OutputEdit.h"

class COutputBar : public CSizingControlBarG
{

// Construction
public:
	COutputBar();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COutputBar)
	//}}AFX_VIRTUAL

// Implementation
public:
	void Copy();
	void CopyAll();
	void Clear();
	void PrintSep(CString &_text, int _len, char _sep='-', COLORREF _color=0x00000000);
	void PrintText(CString &_text);
	virtual ~COutputBar();
	trsOutputDev &GetOutputDev() { return m_wndEdit; }

protected:
	COutputEdit m_wndEdit;
	CHARFORMAT  m_cf;

	// Generated message map functions
protected:
	//{{AFX_MSG(COutputBar)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OUTPUTBAR_H__72FF2125_B73D_46F0_8F7E_CA4A6A2D6B17__INCLUDED_)
