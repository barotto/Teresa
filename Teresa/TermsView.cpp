/*//////////////////////////////////////////////////////////////////////////////
	FILENAME:	TermsView.cpp
	Version:	1.0.0
	Author:		Marco Bortolin
	Created:	13/09/2001 - 12.24.06

	@@ obsoleto
	Copyright (C) 2001 Marco Bortolin
//////////////////////////////////////////////////////////////////////////////*/

#include "stdafx.h"
#include "Teresa.h"
#include "TermsView.h"
#include "TeresaDoc.h"
#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTermsView

IMPLEMENT_DYNCREATE(CTermsView, CTreeView)

CTermsView::CTermsView()
{
	m_font.CreateFont(14,8,0,0,FW_NORMAL,false,false,false,ANSI_CHARSET,
		OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY,FIXED_PITCH|FF_DONTCARE,"Courier");
}

CTermsView::~CTermsView()
{
}


BEGIN_MESSAGE_MAP(CTermsView, CTreeView)
	ON_WM_CONTEXTMENU()
	//{{AFX_MSG_MAP(CTermsView)
	ON_WM_RBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTermsView drawing

void CTermsView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}

/////////////////////////////////////////////////////////////////////////////
// CTermsView diagnostics

#ifdef _DEBUG
void CTermsView::AssertValid() const
{
	CTreeView::AssertValid();
}

void CTermsView::Dump(CDumpContext& dc) const
{
	CTreeView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CTermsView message handlers

void CTermsView::Sincronizza()
{
	/* @@@@@@@@
	int nItem = 0;
	string		*str;
	CTeresaDoc	*pDoc;
	CTreeCtrl	*pTree;
	string		prob;

	pTree = &GetTreeCtrl();

	pDoc = (CTeresaDoc*)GetDocument();

	pTree->DeleteAllItems();

	while((str = pDoc->GetNextItem())!=NULL)
	{
		if(!TrsGetParser()->CheckString(str->c_str(),&prob))
		{
			HTREEITEM parent = pTree->InsertItem(str->c_str());
			pTree->InsertItem(prob.c_str(),parent);
		}
		else
		{
			pTree->InsertItem(str->c_str());
		}
	}
	*/
}


void CTermsView::OnContextMenu(CWnd*, CPoint point)
{
	if (point.x == -1 && point.y == -1){
		CRect rect;
		GetClientRect(rect);
		ClientToScreen(rect);

		point = rect.TopLeft();
		point.Offset(5, 5);
	}

	CMenu menu;
	VERIFY(menu.LoadMenu(IDR_POPUP_TERMSVIEW));

	CMenu* pPopup = menu.GetSubMenu(0);
	ASSERT(pPopup != NULL);
	CWnd* pWndPopupOwner = this;

	while (pWndPopupOwner->GetStyle() & WS_CHILD)
		pWndPopupOwner = pWndPopupOwner->GetParent();

	pPopup->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON,point.x,point.y,pWndPopupOwner);
}


void CTermsView::OnRButtonDown(UINT nFlags, CPoint point) 
{
	RECT r;

	// Il context-menu compare solo con un DBCLICK (why?), sono quindi costretto a richiamarlo
	// manualmente da qui:
	GetWindowRect(&r);
	OnContextMenu(NULL, CPoint(r.left+point.x,r.top+point.y));

	//CTreeView::OnRButtonDown(nFlags, point);
}


void CTermsView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
	// TODO: Add your specialized code here and/or call the base class
	Sincronizza();
}


void CTermsView::OnInitialUpdate() 
{
	CTreeView::OnInitialUpdate();
	
	DWORD dwStyle = GetWindowLong(GetTreeCtrl().m_hWnd, GWL_STYLE); 
 
    SetWindowLong(GetTreeCtrl().m_hWnd, GWL_STYLE, 
		dwStyle|TVS_HASLINES|TVS_LINESATROOT|TVS_HASBUTTONS); 

	GetTreeCtrl().SetFont(&m_font);
}
