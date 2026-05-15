/*//////////////////////////////////////////////////////////////////////////////
	FILENAME:	InputBar.cpp
	Version:	1.0.0
	Author:		Marco Bortolin
	Created:	27/07/2001

	Copyright (C) 2001 Marco Bortolin
//////////////////////////////////////////////////////////////////////////////*/

#include "stdafx.h"
#include "Teresa.h"
#include "InputBar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CInputBar

CInputBar::CInputBar()
{
}

CInputBar::~CInputBar()
{
}


BEGIN_MESSAGE_MAP(CInputBar, CToolBar)
	//{{AFX_MSG_MAP(CInputBar)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CInputBar message handlers


/*--------------------------------------------------------------------
	FUNC NAME: 	CInputBar::Create
	RETURNS: 	bool 
	THROWS: 	-

	PURPOSE/DESCRIPTION: 
	Crea la barra e tutti i suoi controlli

--------------------------------------------------------------------*/
bool CInputBar::Create(CWnd *inParentWnd)
{
	static UINT cmds[] =
	{
		ID_NUOVACOPPIA,
		ID_INPUTCANCEL,
		ID_INPUTOK,
		ID_SEPARATOR
	};

	if (!CreateEx(inParentWnd,TBSTYLE_FLAT, WS_CHILD|WS_VISIBLE|CBRS_TOP|CBRS_GRIPPER |CBRS_TOOLTIPS|CBRS_FLYBY) ||
		!LoadBitmap(IDB_INPUTBAR) ||
		!SetButtons(cmds, sizeof(cmds)/sizeof(UINT)))
	{
		TRACE0("Impossibile creare la input bar\n");
		return false;       
	}
	
	SetDlgCtrlID(IDW_INPUTBAR);

	RECT rEdit,rParent;

	inParentWnd->GetWindowRect(&rParent);

	rEdit.right = rParent.right - rParent.left - 22;
	rEdit.bottom = 20;
	rEdit.top = 2;
	rEdit.left = 78;
	//
	if(!m_edit.Create(WS_CHILD|WS_VISIBLE|WS_BORDER|ES_AUTOHSCROLL|ES_WANTRETURN, rEdit, this, IDW_INPUTBAR_EDIT))
	{
		TRACE0("Impossibile creare la input bar\n");
		return false;
	}

	m_edit.EnableWindow(FALSE);

	return true;
}


/*--------------------------------------------------------------------
	FUNC NAME: 	CInputBar::Resize
	RETURNS: 	void 
	THROWS: 	-

	PURPOSE/DESCRIPTION: 
	Ridimensiona i controlli della barra

--------------------------------------------------------------------*/
void CInputBar::Resize(int inX, int inY)
{
	int xsize = inX - (88);
	int ysize = 18;
	
	if (xsize<0)
		xsize = 0;
	
	if(m_edit.GetSafeHwnd() == NULL)
		return;
	
	m_edit.MoveWindow(78,2, xsize, ysize, TRUE);
}
