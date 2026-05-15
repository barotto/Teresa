/*//////////////////////////////////////////////////////////////////////////////
	FILENAME:	Teresa.cpp
	Version:	1.0.1
	Author:		Marco Bortolin
	Created:	27/07/2001

	Copyright (C) 2001 Marco Bortolin
//////////////////////////////////////////////////////////////////////////////*/

#include "stdafx.h"
#include "Teresa.h"

#include "MainFrm.h"
#include "ChildFrm.h"
#include "TeresaDoc.h"
#include "SIDView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTeresaApp

BEGIN_MESSAGE_MAP(CTeresaApp, CWinApp)
	//{{AFX_MSG_MAP(CTeresaApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	ON_COMMAND(ID_REGISTER_FILE_TYPE, OnRegisterFileType)
	ON_COMMAND(ID_UNREGISTER_FILE_TYPE, OnUnregisterFileType)
	ON_COMMAND(ID_VERBOSE_MODE, OnVerboseMode)
	ON_UPDATE_COMMAND_UI(ID_VERBOSE_MODE, OnUpdateVerboseMode)
	//}}AFX_MSG_MAP
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
	// Standard print setup command
	ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTeresaApp construction

CTeresaApp::CTeresaApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CTeresaApp object

CTeresaApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CTeresaApp initialization

BOOL CTeresaApp::InitInstance()
{
	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	// Change the registry key under which our settings are stored.
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization.
	SetRegistryKey(_T("Teresa"));

	LoadStdProfileSettings();  // Load standard INI file options (including MRU)
	
	AfxInitRichEdit();
	
	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views.

	CMultiDocTemplate* pDocTemplate;
	pDocTemplate = new CMultiDocTemplate(
		IDR_TERESATYPE,
		RUNTIME_CLASS(CTeresaDoc),
		RUNTIME_CLASS(CChildFrame), // custom MDI child frame
		RUNTIME_CLASS(CSIDView));
	AddDocTemplate(pDocTemplate);

	// create main MDI Frame window
	CMainFrame* pMainFrame = new CMainFrame;
	if (!pMainFrame->LoadFrame(IDR_MAINFRAME))
		return FALSE;
	m_pMainWnd = pMainFrame;

	// Enable drag/drop open
	m_pMainWnd->DragAcceptFiles();

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// Dispatch commands specified on the command line
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// carico le opzioni memorizzate nel registro
	LoadSettings();

	// The main window has been initialized, so show and update it.
	pMainFrame->ShowWindow(m_nCmdShow);
	pMainFrame->UpdateWindow();

	return TRUE;
	OnContextHelp();
}


/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	CStatic	m_guiversion;
	CStatic	m_trsversion;
	CStatic	m_teresaversion;
	CEdit	m_info_ctrl;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	DDX_Control(pDX, IDC_ABOUTBOX_TERESAGUI, m_guiversion);
	DDX_Control(pDX, IDC_ABOUTBOX_TRSVERSION, m_trsversion);
	DDX_Control(pDX, IDC_ABOUTBOX_TERESAVERSION, m_teresaversion);
	DDX_Control(pDX, IDC_ABOUTBOX_EDIT, m_info_ctrl);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// App command to run the dialog
void CTeresaApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

/////////////////////////////////////////////////////////////////////////////
// CTeresaApp message handlers

void CTeresaApp::WinHelp(DWORD dwData, UINT nCmd) 
{
	if (nCmd==HELP_CONTEXT)	{
		CWinApp::WinHelp(dwData, HELP_CONTEXTPOPUP);
		return;
	}
	CWinApp::WinHelp(dwData, nCmd);
}

BOOL CAboutDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	CString text;
	HRSRC resloc = FindResource(NULL,MAKEINTRESOURCE(IDR_INFO_TEXT),"TEXT");
	HGLOBAL res = LoadResource(NULL, resloc);
 
	text = (const char*)res;
	m_info_ctrl.SetWindowText((LPCTSTR)text);

	CString teresaver, trsver, guiver;

	trsver = TRS_VERSION_STR;
	guiver.LoadString(IDS_TERESAGUI_VERSION);
	teresaver = guiver;
	teresaver += ".";
	teresaver += trsver;

#ifndef NDEBUG
	trsver += " DEBUG";
	guiver += " DEBUG";
	teresaver += " DEBUG";
#endif

	m_teresaversion.SetWindowText((LPCTSTR)teresaver);
	m_trsversion.SetWindowText((LPCTSTR)trsver);
	m_guiversion.SetWindowText((LPCTSTR)guiver);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CTeresaApp::OnRegisterFileType() 
{
	//EnableShellOpen();
	//RegisterShellFileType()
	
	CString path, str;
	TCHAR longpath[_MAX_PATH];

	::GetModuleFileName(AfxGetInstanceHandle(), longpath, _MAX_PATH);
	if (::GetShortPathName(longpath, path.GetBuffer(_MAX_PATH), _MAX_PATH) == 0)
	{
		path = longpath;
	}
	path.ReleaseBuffer();

	POSITION pos = GetFirstDocTemplatePosition();
	CDocTemplate* pTemplate = GetNextDocTemplate(pos);

	CString opencommand = path;
	CString iconcommand = path;

	CString iconid;
	iconid.Format(_T(",%d"), 1);
	iconcommand += iconid;

	CString strFilterExt, strFileTypeId, strFileTypeName;
	if (pTemplate->GetDocString(strFileTypeId,
		CDocTemplate::regFileTypeId) && !strFileTypeId.IsEmpty())
	{
		// enough info to register it
		if (!pTemplate->GetDocString(strFileTypeName,CDocTemplate::regFileTypeName))
			strFileTypeName = strFileTypeId;    // use id name

		ASSERT(strFileTypeId.Find(' ') == -1);  // no spaces allowed

		// first register the type ID of our server
		if (::RegSetValue(HKEY_CLASSES_ROOT, strFileTypeId, REG_SZ,
			  strFileTypeName, lstrlen(strFileTypeName) * sizeof(TCHAR)) != ERROR_SUCCESS)
		{
			TRACE1("Warning: registration database update failed for key '%s'.\n",
				strFileTypeId);
			return;
		}

		// path\DefaultIcon = path,1
		str.Format(_T("%s\\DefaultIcon"), (LPCTSTR)strFileTypeId);
		if (::RegSetValue(HKEY_CLASSES_ROOT, str, REG_SZ,
			  iconcommand, lstrlen(iconcommand) * sizeof(TCHAR)) != ERROR_SUCCESS)
		{
			TRACE1("Warning: registration database update failed for key '%s'.\n",
				str);
			return;
		}
			
		// path\shell\open\command = path filename
		opencommand += _T(" \"%1\"");

		// path\shell\open\command = path filename
		str.Format(_T("%s\\shell\\open\\%s"), (LPCTSTR)strFileTypeId, _T("command"));
		if (::RegSetValue(HKEY_CLASSES_ROOT, str, REG_SZ,
			  opencommand, lstrlen(opencommand) * sizeof(TCHAR)) != ERROR_SUCCESS)
		{
			TRACE1("Warning: registration database update failed for key '%s'.\n",
				str);
			return;
		}
		strFilterExt = _T(".tsa");

		LONG lSize = _MAX_PATH * 2;
		LONG lResult = ::RegQueryValue(HKEY_CLASSES_ROOT, strFilterExt,
				str.GetBuffer(lSize), &lSize);
		str.ReleaseBuffer();

		if (lResult != ERROR_SUCCESS || str.IsEmpty() || str == strFileTypeId)
		{
			if (::RegSetValue(HKEY_CLASSES_ROOT, strFilterExt, REG_SZ,
			  strFileTypeId, lstrlen(strFileTypeId) * sizeof(TCHAR)) != ERROR_SUCCESS)
				return;

			str.Format(_T("%s\\ShellNew"), (LPCTSTR)strFilterExt);
					
			HKEY hKey;
			if(::RegCreateKey(HKEY_CLASSES_ROOT, str, &hKey) == ERROR_SUCCESS)
			{
				::RegSetValueEx(hKey, _T("NullFile"), 0, REG_SZ,
					(CONST BYTE*)_T(""), (lstrlen(_T("")) + 1) * sizeof(TCHAR));

				::RegCloseKey(hKey);
			}
		}
	}
}

void CTeresaApp::OnUnregisterFileType() 
{
	UnregisterShellFileTypes();	
}


void CTeresaApp::LoadSettings()
{
	int verbose;

	verbose = GetProfileInt("Settings", "verbose", 0);

	if (verbose == 1)
		m_verbose = true;
	else
		m_verbose = false;
	
	GetProfileFont("Font", 0, &m_defFont);
}

void CTeresaApp::WriteSettings()
{
	if (m_verbose==true)
		WriteProfileInt("Settings", "verbose", 1);
	else
		WriteProfileInt("Settings", "verbose", 0);
			
	WriteProfileFont("Font", 0, &m_defFont);
}

void CTeresaApp::OnVerboseMode() 
{
	m_verbose = !m_verbose;	
}

void CTeresaApp::OnUpdateVerboseMode(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(m_verbose);
}

int CTeresaApp::ExitInstance() 
{
	WriteSettings();	
	return CWinApp::ExitInstance();
}


void CTeresaApp::GetProfileFont(LPCTSTR _szSec, int _wndid, LOGFONT* _plf)
{
	CString sec;
	sec.Format("%s_%d", _szSec, _wndid);

	memset(_plf,0,sizeof(LOGFONT));

	_plf->lfHeight = GetProfileInt((LPCTSTR)sec, "Height", 10);
	_plf->lfWidth = GetProfileInt((LPCTSTR)sec, "Width", 4);
	_plf->lfWeight = GetProfileInt((LPCTSTR)sec, "Weight", FW_REGULAR);
	_plf->lfItalic = (BYTE)GetProfileInt((LPCTSTR)sec, "Italic", 0);
	_plf->lfUnderline = (BYTE)GetProfileInt((LPCTSTR)sec, "Underline", 0);
	_plf->lfPitchAndFamily = (BYTE)GetProfileInt((LPCTSTR)sec, "PitchAndFamily", FF_DONTCARE);
	_plf->lfCharSet = (BYTE)GetProfileInt((LPCTSTR)sec, "CharSet", DEFAULT_CHARSET);
	CString strFont = GetProfileString((LPCTSTR)sec, "FaceName", "Courier");
	lstrcpyn((TCHAR*)_plf->lfFaceName, (LPCTSTR)strFont, strFont.GetLength()+1);
	//_plf->lfFaceName[sizeof _plf->lfFaceName-1] = 0;
}


void CTeresaApp::WriteProfileFont(LPCTSTR _szSec, int _wndid, const LOGFONT* _plf)
{
	CString sec;
	sec.Format("%s_%d", _szSec, _wndid);

	WriteProfileInt((LPCTSTR)sec, "Height", _plf->lfHeight);
	WriteProfileInt((LPCTSTR)sec, "Width", _plf->lfWidth);
	WriteProfileInt((LPCTSTR)sec, "Weight", _plf->lfWeight);
	WriteProfileInt((LPCTSTR)sec, "Italic", _plf->lfItalic);
	WriteProfileInt((LPCTSTR)sec, "Underline", _plf->lfUnderline);
	WriteProfileInt((LPCTSTR)sec, "PitchAndFamily", _plf->lfPitchAndFamily);
	WriteProfileInt((LPCTSTR)sec, "CharSet", _plf->lfCharSet);
	WriteProfileString((LPCTSTR)sec, "FaceName", (LPCTSTR)_plf->lfFaceName);
}

void CTeresaApp::GetFont(LOGFONT *_plf, int _wndid)
{
	GetProfileFont("Font",_wndid,_plf);
}

void CTeresaApp::StoreFont(const LOGFONT *_plf, int _wndid)
{
	WriteProfileFont("Font",_wndid,_plf);
}
