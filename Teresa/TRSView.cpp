/*//////////////////////////////////////////////////////////////////////////////
	FILENAME:	TRSView.cpp
	Version:	1.0.0
	Author:		Marco Bortolin
	Created:	27/07/2001 - 23.41.40

	Copyright (C) 2001 Marco Bortolin, Alberto Minora
//////////////////////////////////////////////////////////////////////////////*/

#include "stdafx.h"
#include "Teresa.h"
#include "TRSView.h"
#include "TeresaDoc.h"
#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTRSView

IMPLEMENT_DYNCREATE(CTRSView, CListView)

CTRSView::CTRSView()
{
	m_font.CreateFont(14,6,0,0,FW_NORMAL,false,false,false,ANSI_CHARSET,
		OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY,FIXED_PITCH|FF_DONTCARE,"Courier");
}

CTRSView::~CTRSView()
{
}


BEGIN_MESSAGE_MAP(CTRSView, CListView)
	ON_WM_CONTEXTMENU()
	//{{AFX_MSG_MAP(CTRSView)
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTRSView drawing

void CTRSView::OnDraw(CDC* pDC)
{
	//CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}

void CTRSView::OnInitialUpdate()
{
	CListView::OnInitialUpdate();

	// TODO: You may populate your ListView with items by directly accessing
	//  its list control through a call to GetListCtrl().

	CListCtrl *pList = &GetListCtrl();

	DWORD dwStyle = GetWindowLong(pList->m_hWnd, GWL_STYLE); 
 
    SetWindowLong(pList->m_hWnd, GWL_STYLE, (dwStyle & ~LVS_TYPEMASK) | LVS_REPORT | LVS_NOSORTHEADER); 
	
	RECT r;
	GetWindowRect(&r);
	pList->InsertColumn(0,"TRS Equivalente",LVCFMT_LEFT,r.right);
	pList->SetFont(&m_font);
}

/////////////////////////////////////////////////////////////////////////////
// CTRSView diagnostics

#ifdef _DEBUG
void CTRSView::AssertValid() const
{
	CListView::AssertValid();
}

void CTRSView::Dump(CDumpContext& dc) const
{
	CListView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CTRSView message handlers

void CTRSView::Sincronizza()
{
	trsTermPairs *ptrs;
	trsTermPairs::iterator it;
	int         nItem = 0;
	CListCtrl&  oList = GetListCtrl();
	CTeresaDoc* pDoc = (CTeresaDoc*)GetDocument();

	oList.DeleteAllItems();
	ptrs = pDoc->GetTrs();
	for (it=ptrs->begin(); it != ptrs->end(); it++) {
		oList.InsertItem(nItem++,it->GetPrintable(true).c_str());
	}
}

void CTRSView::OnSize(UINT nType, int cx, int cy) 
{
	CListView::OnSize(nType, cx, cy);
	GetListCtrl().SetColumnWidth(0,LVSCW_AUTOSIZE_USEHEADER);
	POINT elem;
	if (!GetListCtrl().GetItemPosition(0,&elem)) return;
	if (elem.y>20)
		GetListCtrl().EnsureVisible(GetListCtrl().GetItemCount()-1,FALSE);
}

void CTRSView::OnContextMenu(CWnd*, CPoint point)
{
	if (point.x == -1 && point.y == -1){
		CRect rect;
		GetClientRect(rect);
		ClientToScreen(rect);

		point = rect.TopLeft();
		point.Offset(5, 5);
	}

	CMenu menu;
	VERIFY(menu.LoadMenu(IDR_POPUP_TRSVIEW));

	CMenu* pPopup = menu.GetSubMenu(0);
	ASSERT(pPopup != NULL);
	CWnd* pWndPopupOwner = this;

	while (pWndPopupOwner->GetStyle() & WS_CHILD)
		pWndPopupOwner = pWndPopupOwner->GetParent();

	pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y,
		pWndPopupOwner);
}


void CTRSView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
	Sincronizza();
}


CChildFrame* CTRSView::GetChildFrame()
{
	return (CChildFrame*)(GetParent()->GetParent()); //il primo parent e' lo splitter
}


void CTRSView::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView) 
{
	CChildFrame* pCframe = GetChildFrame();

	pCframe->SetActiveView(pCframe->GetSIDView());
	
	CListView::OnActivateView(bActivate, pActivateView, pDeactiveView);
}
