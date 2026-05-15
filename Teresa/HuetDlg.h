/*//////////////////////////////////////////////////////////////////////////////
	FILENAME:	HuetDlg.h
	Version:	1.0.0
	Author:		Marco Bortolin
	Created:	27/07/2001 - 23.41.40

	Copyright (c) 2001 Marco Bortolin
//////////////////////////////////////////////////////////////////////////////*/

#if !defined(AFX_HUETDLG_H__28607E2D_4825_45B7_BEBC_817BC626B806__INCLUDED_)
#define AFX_HUETDLG_H__28607E2D_4825_45B7_BEBC_817BC626B806__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <trs.h>

class CHuetDlg : public CDialog, trsFBDev, trsOutputDev
{
// Construction
public:
	CHuetDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CHuetDlg)
	enum { IDD = IDD_HUET };
	CStatic	m_funcname;
	CEdit	m_loopnum_ctrl;
	CButton	m_checklog;
	CButton	m_btnlog;
	CEdit	m_trsrules_ctrl;
	CButton	m_btnexit;
	CEdit	m_curloop_ctrl;
	CProgressCtrl	m_progress;
	CButton	m_btnstop;
	CButton	m_btnavvio;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHuetDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
public:
	trsOutputDev &operator <<(string _src);
	trsOutputDev &operator <<(const char* _src);
	trsOutputDev &operator <<(int _src);

	bool Stop();
	void SetProgressRange(int _lower, int _upper);
	void SetProgressStep(int _step);
	void SetProgressPos(int _pos);
	void StepProgress();
	
	void PrintInt(int _vec[], int _size);
	
	void PrintStr(const char* _str);
	void PrintFuncName(const char* _str);

	void Reset();

	trsSystem* m_system;
	trsOrder*  m_order;

	CString m_mex;

protected:
	HICON m_iavvio, m_istop;
	CImageList m_funcdots;
	bool m_bstop;
	bool m_blooplimit;
	int m_ilimit;
	FILE *m_fplog;


	// Generated message map functions
	//{{AFX_MSG(CHuetDlg)
	afx_msg void OnAvvio();
	afx_msg void OnStop();
	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	afx_msg void OnHuetdlgLog();
	afx_msg void OnChecklog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HUETDLG_H__28607E2D_4825_45B7_BEBC_817BC626B806__INCLUDED_)
