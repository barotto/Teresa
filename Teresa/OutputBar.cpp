/*//////////////////////////////////////////////////////////////////////////////
	FILENAME:	OutputBar.cpp
	Version:	1.0.0
	Author:		Marco Bortolin
	Created:	27/07/2001

	Copyright (C) 2001 Marco Bortolin
//////////////////////////////////////////////////////////////////////////////*/

#include "stdafx.h"
#include "Teresa.h"
#include "OutputBar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COutputBar

COutputBar::COutputBar()
{
}

COutputBar::~COutputBar()
{
}


BEGIN_MESSAGE_MAP(COutputBar, CSizingControlBarG)
	//{{AFX_MSG_MAP(COutputBar)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COutputBar message handlers


int COutputBar::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CSizingControlBarG::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	SetSCBStyle(GetSCBStyle()|SCBS_SHOWEDGES|SCBS_SIZECHILD);

	if (!m_wndEdit.Create(WS_CHILD|WS_VISIBLE|WS_VSCROLL|ES_MULTILINE|
		ES_WANTRETURN|ES_AUTOVSCROLL,
		CRect(0,0,0,0), this, IDW_OUTPUTBAR_EDIT))
		return -1;

	m_wndEdit.ModifyStyleEx(0, WS_EX_CLIENTEDGE);
	m_wndEdit.GetSelectionCharFormat(m_cf);

	m_szHorz.cy = 100;

	return 0;
}

void COutputBar::PrintText(CString &_text)
{
	m_wndEdit.Print(_text);
}

void COutputBar::Clear()
{
	m_wndEdit.SendMessage(WM_COMMAND,MAKEWPARAM(ID_OUTPUTEDIT_CLEAR,1),NULL);
}

void COutputBar::Copy()
{
	m_wndEdit.SendMessage(WM_COMMAND,MAKEWPARAM(ID_OUTPUTEDIT_COPY,1),NULL);
}

void COutputBar::CopyAll()
{
	m_wndEdit.SendMessage(WM_COMMAND,MAKEWPARAM(ID_OUTPUTEDIT_COPYALL,1),NULL);
}

void COutputBar::PrintSep(CString &_text, int _len, char _sep, COLORREF _color)
{
	CHARFORMAT cf;
	
	m_wndEdit.SetSel(-1,-1);
	
	ZeroMemory(&cf,sizeof(cf));
	cf.cbSize = sizeof(cf);
	cf.dwMask = CFM_COLOR|CFM_EFFECTS;
	cf.dwEffects = CFE_BOLD; 	
	cf.crTextColor = _color;	
	m_wndEdit.SetSelectionCharFormat(cf);

	CString s("\n\n");
	s += _text + "\n";
	int textlen = s.GetLength();
	char* p = s.GetBufferSetLength(textlen+_len);
	memset(p+textlen,_sep,_len);
	s.ReleaseBuffer();
	s += "\n";
	m_wndEdit.Print(s);
	
	m_wndEdit.SetSelectionCharFormat(m_cf);
}
