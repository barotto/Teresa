/*//////////////////////////////////////////////////////////////////////////////
	FILENAME:	OutputEdit.cpp
	Version:	1.0.0
	Author:		Marco Bortolin
	Created:	27/07/2001

	Copyright (C) 2001 Marco Bortolin
//////////////////////////////////////////////////////////////////////////////*/

#include "stdafx.h"
#include "teresa.h"
#include "OutputEdit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COutputEdit

COutputEdit::COutputEdit()
{
}

COutputEdit::~COutputEdit()
{
}


BEGIN_MESSAGE_MAP(COutputEdit, CRichEditCtrl)
	//{{AFX_MSG_MAP(COutputEdit)
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_OUTPUTEDIT_CLEAR, OnOutputeditClear)
	ON_WM_RBUTTONUP()
	ON_COMMAND(ID_OUTPUTEDIT_COPY, OnOutputeditCopy)
	ON_WM_CREATE()
	ON_COMMAND(ID_OUTPUTEDIT_COPYALL, OnOutputeditCopyall)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COutputEdit message handlers

void COutputEdit::OnContextMenu(CWnd* pWnd, CPoint point) 
{
 	if (point.x == -1 && point.y == -1){
 		CRect rect;
		GetClientRect(rect);
 		ClientToScreen(rect);
 		point = rect.TopLeft();
 		point.Offset(5, 5);
	}

 	CMenu menu;
 
 	VERIFY(menu.LoadMenu(IDR_POPUP_OUTPUTEDIT));

 	CMenu* pPopup = menu.GetSubMenu(0);
	ASSERT(pPopup != NULL);
 	CWnd* pWndPopupOwner = this;
	
 	while (pWndPopupOwner->GetStyle() & WS_CHILD)
		pWndPopupOwner = pWndPopupOwner->GetParent();

	pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, pWndPopupOwner);	
}

void COutputEdit::OnOutputeditClear() 
{
	SetWindowText("");
	UpdateWindow();	
}

void COutputEdit::OnRButtonUp(UINT nFlags, CPoint point) 
{
	RECT r;
	GetWindowRect(&r);
	
	OnContextMenu(NULL, CPoint(r.left+point.x,r.top+point.y));
	
	//CRichEditCtrl::OnRButtonUp(nFlags, point);
}

void COutputEdit::Print(CString &_str)
{
	Print((LPCTSTR)_str);
}

void COutputEdit::OnOutputeditCopy() 
{
	Copy();
}

int COutputEdit::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CRichEditCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	m_font.CreateFont(12,4,0,0,FW_NORMAL,false,false,false,ANSI_CHARSET,
		OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY,FIXED_PITCH|FF_DONTCARE,"Courier");

	SetFont(&m_font);
	
	return 0;
}


void COutputEdit::printf(const char* _format, ...) 
{
	CString str;

	va_list ap;
	va_start(ap, _format);
	str.Format(_format,ap);
	va_end(ap);
	Print(str);
};

	
trsOutputDev & COutputEdit::operator <<(string _src)
{ 
	Print(_src.c_str());
	return *this; 
}


trsOutputDev & COutputEdit::operator <<(const char* _src) 
{
	Print(_src);
	return *this; 
}


trsOutputDev & COutputEdit::operator <<(int _src)
{ 
	CString s;
	s.Format("%d",_src);
	Print(s);
	return *this; 
}

void COutputEdit::Print(const char *_str)
{
	RECT r;
	GetWindowRect(&r);

	SetSel(-1,-1);
	ReplaceSel(_str);
	int min = GetFirstVisibleLine();
	int max = GetLineCount();
	int nlines = (r.bottom-r.top)/14;

	if(max-min>nlines)
		LineScroll((max-nlines)-min-1);
	
	UpdateWindow();
}


void COutputEdit::OnOutputeditCopyall() 
{
	SetSel(0,-1);
	Copy();
	SetSel(-1,-1);
}
