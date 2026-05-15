/*//////////////////////////////////////////////////////////////////////////////
	FILENAME:	InputEdit.cpp
	Version:	1.0.0
	Author:		Marco Bortolin
	Created:	02/08/2001 - 10.33.11

	Implementazione di CInputEdit (CEdit della inputbar)
	Copyright (C) 2001 Marco Bortolin
//////////////////////////////////////////////////////////////////////////////*/
#include "stdafx.h"
#include "Teresa.h"
#include "InputEdit.h"
#include "ChildFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CInputEdit

CInputEdit::CInputEdit()
{
	m_bIgnoraKeyUp = false;
}

CInputEdit::~CInputEdit()
{
}


BEGIN_MESSAGE_MAP(CInputEdit, CEdit)
	//{{AFX_MSG_MAP(CInputEdit)
	ON_WM_KEYUP()
	ON_WM_KEYDOWN()
	ON_COMMAND(ID_INPUTBAR_EDIT_IGNORAKEYUP, OnIgnoraKeyUp)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CInputEdit message handlers


void CInputEdit::EscapePressed()
{
	CChildFrame* pCframe = (CChildFrame*)GetParent()->GetParent();
	pCframe->SendMessage(WM_COMMAND,MAKEWPARAM(ID_INPUTCANCEL,1),NULL);
}


void CInputEdit::EnterPressed()
{
	CChildFrame* pCframe = (CChildFrame*)GetParent()->GetParent();
	pCframe->SendMessage(WM_COMMAND,MAKEWPARAM(ID_INPUTOK,1),NULL);
}


void CInputEdit::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	if (m_bIgnoraKeyUp) {
		m_bIgnoraKeyUp = false;
		CEdit::OnKeyUp(nChar, nRepCnt, nFlags);
		return;
	}
	if(nChar==VK_RETURN) {
		EnterPressed();
		return;
	}
	if(nChar==VK_ESCAPE) {
		EscapePressed();
		return;
	}
	CEdit::OnKeyUp(nChar, nRepCnt, nFlags);
}


void CInputEdit::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// ne' VK_ENTER ne' VK_ESCAPE vengono passati a questa funzione.
	CEdit::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CInputEdit::OnIgnoraKeyUp()
{
	m_bIgnoraKeyUp = true;
}

int CInputEdit::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CEdit::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	m_font.CreateFont(16,8,0,0,FW_NORMAL,false,false,false,ANSI_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY,FIXED_PITCH|FF_DONTCARE,"Courier");
	SetFont(&m_font);
	return 0;
}
