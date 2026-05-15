/*//////////////////////////////////////////////////////////////////////////////
	FILENAME:	InputEdit.h
	Version:	1.0.0
	Author:		Marco Bortolin
	Created:	27/07/2001 - 23.41.40

	Copyright (c) 2001 Marco Bortolin
//////////////////////////////////////////////////////////////////////////////*/

#if !defined(AFX_INPUTEDIT_H__180C3C2B_BF4C_4F81_90EA_12267E31FB5E__INCLUDED_)
#define AFX_INPUTEDIT_H__180C3C2B_BF4C_4F81_90EA_12267E31FB5E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CInputEdit : public CEdit
{
// Construction
public:
	CInputEdit();

// Attributes
public:
protected:
	CFont		m_font;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CInputEdit)
	//}}AFX_VIRTUAL

// Implementation
public:
	void EscapePressed();
	void EnterPressed();
	virtual ~CInputEdit();

	// Generated message map functions
protected:
	//{{AFX_MSG(CInputEdit)
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnIgnoraKeyUp();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
private:
	bool m_bIgnoraKeyUp;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INPUTEDIT_H__180C3C2B_BF4C_4F81_90EA_12267E31FB5E__INCLUDED_)
