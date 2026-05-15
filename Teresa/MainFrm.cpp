/*//////////////////////////////////////////////////////////////////////////////
	FILENAME:	MainFrm.cpp
	Version:	1.0.0
	Author:		Marco Bortolin
	Created:	27/07/2001

	Copyright (C) 2001 Marco Bortolin
//////////////////////////////////////////////////////////////////////////////*/

#include "stdafx.h"
#include "Teresa.h"

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CMDIFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_COMMAND(ID_VIEW_OUTPUTBAR, OnViewOutputbar)
	ON_UPDATE_COMMAND_UI(ID_VIEW_OUTPUTBAR, OnUpdateViewOutputbar)
	ON_WM_CLOSE()
	ON_COMMAND(ID_OUTPUTEDIT_CLEAR, OnOutputeditClear)
	ON_COMMAND(ID_OUTPUTEDIT_COPY, OnOutputeditCopy)
	ON_UPDATE_COMMAND_UI(ID_OUTPUTEDIT_CLEAR, OnUpdateOutputeditClear)
	ON_UPDATE_COMMAND_UI(ID_OUTPUTEDIT_COPY, OnUpdateOutputeditCopy)
	ON_COMMAND(ID_OUTPUTEDIT_COPYALL, OnOutputeditCopyall)
	ON_UPDATE_COMMAND_UI(ID_OUTPUTEDIT_COPYALL, OnUpdateOutputeditCopyall)
	//}}AFX_MSG_MAP
	// Global help commands
	ON_COMMAND(ID_HELP_FINDER, CMDIFrameWnd::OnHelpFinder)
	ON_COMMAND(ID_HELP, CMDIFrameWnd::OnHelp)
	ON_COMMAND(ID_CONTEXT_HELP, CMDIFrameWnd::OnContextHelp)
	ON_COMMAND(ID_DEFAULT_HELP, CMDIFrameWnd::OnHelpFinder)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here
	
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMDIFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}
	
	if (!m_wndOutputBar.Create(_T("Output"),this,IDW_OUTPUT_BAR))
	{
		TRACE0("Failed to create Output bar\n");
		return -1; // fail to create
	}
	m_wndOutputBar.SetBarStyle(m_wndOutputBar.GetBarStyle()|CBRS_TOOLTIPS|CBRS_FLYBY|CBRS_SIZE_DYNAMIC);

	m_wndOutputBar.EnableDocking(CBRS_ALIGN_ANY);
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);
	DockControlBar(&m_wndOutputBar, AFX_IDW_DOCKBAR_BOTTOM);
	m_wndOutputBar.LoadState("output");
	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CMDIFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CMDIFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CMDIFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers


void CMainFrame::OnViewOutputbar() 
{
	ShowControlBar(&m_wndOutputBar, !m_wndOutputBar.IsVisible(), FALSE);	
}

void CMainFrame::OnUpdateViewOutputbar(CCmdUI* pCmdUI) 
{
	if (m_wndOutputBar.IsVisible())
		pCmdUI->SetCheck(1);
	else
		pCmdUI->SetCheck(0);
}

void CMainFrame::OnClose() 
{
	m_wndOutputBar.SaveState("output");	
	CMDIFrameWnd::OnClose();
}

void CMainFrame::OnOutputeditClear() 
{
	m_wndOutputBar.Clear();
}

void CMainFrame::OnOutputeditCopy() 
{
	m_wndOutputBar.Copy();	
}

void CMainFrame::OnUpdateOutputeditClear(CCmdUI* pCmdUI) 
{
	if (m_wndOutputBar.IsVisible())
		pCmdUI->Enable(1);
	else
		pCmdUI->Enable(0);
}

void CMainFrame::OnUpdateOutputeditCopy(CCmdUI* pCmdUI) 
{
	if (m_wndOutputBar.IsVisible())
		pCmdUI->Enable(1);
	else
		pCmdUI->Enable(0);	
}

void CMainFrame::OnOutputeditCopyall() 
{
	m_wndOutputBar.CopyAll();		
}

void CMainFrame::OnUpdateOutputeditCopyall(CCmdUI* pCmdUI) 
{
	if (m_wndOutputBar.IsVisible())
		pCmdUI->Enable(1);
	else
		pCmdUI->Enable(0);	
}

