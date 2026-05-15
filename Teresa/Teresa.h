/*//////////////////////////////////////////////////////////////////////////////
	FILENAME:	Teresa.h
	Version:	1.0.0
	Author:		Marco Bortolin
	Created:	27/07/2001 - 23.41.40

	Copyright (c) 2001 Marco Bortolin
//////////////////////////////////////////////////////////////////////////////*/

#if !defined(AFX_HUETCOMPLETION_H__618BC899_B460_4526_9506_2FD5E0C3D0E1__INCLUDED_)
#define AFX_HUETCOMPLETION_H__618BC899_B460_4526_9506_2FD5E0C3D0E1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols
#include <trs.h>

class CTeresaApp : public CWinApp
{

public:
	void StoreFont(const LOGFONT *_plf, int _wndid=0);
	void GetFont(LOGFONT *_plf, int _wndid=0);
	void WriteSettings();
	bool IsVerbose() { return m_verbose; }
	CTeresaApp();

	void Shutdown()
	{	
		::SendMessage(GetMainWnd()->m_hWnd,WM_CLOSE,0,0);
	}

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTeresaApp)
	public:
	virtual BOOL InitInstance();
	virtual void WinHelp(DWORD dwData, UINT nCmd = HELP_CONTEXT);
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CTeresaApp)
	afx_msg void OnAppAbout();
	afx_msg void OnRegisterFileType();
	afx_msg void OnUnregisterFileType();
	afx_msg void OnVerboseMode();
	afx_msg void OnUpdateVerboseMode(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	bool m_verbose;
	LOGFONT m_defFont;
	void GetProfileFont(LPCTSTR _szSec, int _wndid, LOGFONT* _plf);
	void WriteProfileFont(LPCTSTR _szSec, int _wndid, const LOGFONT* _plf);

protected:
	void LoadSettings();
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HUETCOMPLETION_H__618BC899_B460_4526_9506_2FD5E0C3D0E1__INCLUDED_)
