/*//////////////////////////////////////////////////////////////////////////////
	FILENAME:	SIDView.cpp
	Version:	1.0.0
	Author:		Marco Bortolin
	Created:	27/07/2001 - 23.38.21

	Implementazione classe CSIDView (vista principale di CTeresaDoc)
	Copyright (C) 2001 Marco Bortolin
//////////////////////////////////////////////////////////////////////////////*/
#include "stdafx.h"
#include "Teresa.h"
#include "TeresaDoc.h"
#include "SIDView.h"
#include "TRSView.h"
#include "InputBar.h"
#include "ChildFrm.h"
#include "resource.h"
#include "EquProp.h"
#include "TeoriaProp.h"
#include "OrderDlg.h"
#include "OrderProbDlg.h"
#include "MainFrm.h"
#include "HuetDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSIDView

IMPLEMENT_DYNCREATE(CSIDView, CListView)

BEGIN_MESSAGE_MAP(CSIDView, CListView)
	ON_WM_CONTEXTMENU()
	//{{AFX_MSG_MAP(CSIDView)
	ON_WM_SIZE()
	ON_WM_KEYDOWN()
	ON_NOTIFY_REFLECT(NM_CLICK, OnClick)
	ON_NOTIFY_REFLECT(NM_DBLCLK, OnDblclk)
	ON_COMMAND(ID_SIDVIEW_ELIMINA, OnSidviewElimina)
	ON_COMMAND(ID_SIDVIEW_MODIFICA, OnSidviewModifica)
	ON_UPDATE_COMMAND_UI(ID_SIDVIEW_ELIMINA, OnUpdateSidviewElimina)
	ON_UPDATE_COMMAND_UI(ID_SIDVIEW_MODIFICA, OnUpdateSidviewModifica)
	ON_COMMAND(ID_SIDVIEW_ORDER, OnSidviewOrder)
	ON_COMMAND(ID_SIDVIEW_REFRESH, OnSidviewRefresh)
	ON_COMMAND(ID_SIDVIEW_TESTORDER, OnSidviewTestorder)
	ON_UPDATE_COMMAND_UI(ID_SIDVIEW_TESTORDER, OnUpdateSidviewTestorder)
	ON_COMMAND(ID_SIDVIEW_UNIFICAZIONE, OnSidviewUnificazione)
	ON_COMMAND(ID_SIDVIEW_SWAPTERMS, OnSidviewSwapterms)
	ON_UPDATE_COMMAND_UI(ID_SIDVIEW_SWAPTERMS, OnUpdateSidviewSwapterms)
	ON_COMMAND(ID_SIDVIEW_COPPIECRITICHE, OnSidviewCoppiecritiche)
	ON_COMMAND(ID_SIDVIEW_PROPERTIES, OnSidviewProperties)
	ON_COMMAND(ID_SIDVIEW_HUET, OnSidviewHuet)
	ON_UPDATE_COMMAND_UI(ID_SIDVIEW_HUET, OnUpdateSidviewHuet)
	ON_COMMAND(ID_SIDVIEW_DELETEALL, OnSidviewDeleteall)
	ON_UPDATE_COMMAND_UI(ID_SIDVIEW_DELETEALL, OnUpdateSidviewDeleteall)
	ON_UPDATE_COMMAND_UI(ID_SIDVIEW_COPPIECRITICHE, OnUpdateSidviewCoppiecritiche)
	ON_UPDATE_COMMAND_UI(ID_SIDVIEW_UNIFICAZIONE, OnUpdateSidviewUnificazione)
	ON_COMMAND(ID_WORDPROBLEM, OnWordproblem)
	ON_UPDATE_COMMAND_UI(ID_WORDPROBLEM, OnUpdateWordproblem)
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_UPDATE_COMMAND_UI(ID_EDIT_COPY, OnUpdateEditCopy)
	ON_COMMAND(ID_EDIT_CUT, OnEditCut)
	ON_UPDATE_COMMAND_UI(ID_EDIT_CUT, OnUpdateEditCut)
	ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE, OnUpdateEditPaste)
	ON_COMMAND(ID_EDIT_PASTE, OnEditPaste)
	ON_UPDATE_COMMAND_UI(ID_SIDVIEW_PROPERTIES, OnUpdateSidviewProperties)
	ON_UPDATE_COMMAND_UI(ID_SIDVIEW_ORDER, OnUpdateSidviewOrder)
	ON_UPDATE_COMMAND_UI(ID_FILE_PRINT, OnUpdateFilePrint)
	ON_UPDATE_COMMAND_UI(ID_FILE_PRINT_PREVIEW, OnUpdateFilePrintPreview)
	ON_COMMAND(ID_TRSVIEW_DELETEALL, OnTrsviewDeleteall)
	ON_UPDATE_COMMAND_UI(ID_TRSVIEW_DELETEALL, OnUpdateTrsviewDeleteall)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CListView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CListView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CListView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSIDView construction/destruction

CSIDView::CSIDView()
{
	m_inputmode = false;
	m_modifymode = false;
	m_wordpmode = false;

	m_font.CreateFont(14,8,0,0,FW_NORMAL,false,false,false,ANSI_CHARSET,
		OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY,FIXED_PITCH|FF_DONTCARE,"Courier");
}

CSIDView::~CSIDView()
{
}

BOOL CSIDView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CListView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CSIDView drawing

void CSIDView::OnDraw(CDC* pDC)
{
	CTeresaDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here
}

void CSIDView::OnInitialUpdate()
{
	CListView::OnInitialUpdate();
	
	RECT r;
	CListCtrl *pList = &GetListCtrl();
	DWORD dwStyle = GetWindowLong(pList->m_hWnd, GWL_STYLE); 
	SetWindowLong(pList->m_hWnd, GWL_STYLE, 
		(dwStyle & ~LVS_TYPEMASK) | LVS_REPORT | LVS_NOSORTHEADER); 
	GetWindowRect(&r);
	pList->InsertColumn(0,"Coppie di termini",LVCFMT_LEFT,r.right);	
	m_imgList.Create(MAKEINTRESOURCE(IDB_PALLINI),8,1,0x00FF00FF);
	pList->SetImageList(&m_imgList,LVSIL_SMALL);
	pList->SetFont(&m_font);
	Sincronizza();
}

/////////////////////////////////////////////////////////////////////////////
// CSIDView printing

BOOL CSIDView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CSIDView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CSIDView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CSIDView diagnostics

#ifdef _DEBUG
void CSIDView::AssertValid() const
{
	CListView::AssertValid();
}

void CSIDView::Dump(CDumpContext& dc) const
{
	CListView::Dump(dc);
}

CTeresaDoc* CSIDView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CTeresaDoc)));
	return (CTeresaDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CSIDView message handlers

void CSIDView::OnSize(UINT nType, int cx, int cy) 
{
	CListView::OnSize(nType, cx, cy);
	GetListCtrl().SetColumnWidth(0,LVSCW_AUTOSIZE_USEHEADER);
	POINT elem;
	if (!GetListCtrl().GetItemPosition(0,&elem)) return;
	if (elem.y>20)
		GetListCtrl().EnsureVisible(GetListCtrl().GetItemCount()-1,FALSE);
}


void CSIDView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	CChildFrame* pCframe = GetChildFrame();
	if(IsWaitingInput()) {
		pCframe->SendMessage(WM_COMMAND,MAKEWPARAM(ID_INPUTCANCEL,1),NULL);
		return;
	}

	//@@ invece di usare questo meccanismo utilizzare gli 'accelerators'
	switch(nChar) {
	case VK_DELETE:
		OnSidviewElimina();
		break;
	case VK_F2:
		OnSidviewModifica();
		break;
	case VK_F3:
		pCframe->SendMessage(WM_COMMAND,MAKEWPARAM(ID_NUOVACOPPIA,1),NULL);
		break;
	}
	CListView::OnKeyDown(nChar, nRepCnt, nFlags);
}


void CSIDView::Sincronizza()
{
	int        nItem=0, icon=0;
	string*    str;
	CTeresaDoc*  pDoc;

	pDoc = GetDocument();
	GetListCtrl().DeleteAllItems();
	str = pDoc->GetFirstItem();
	while(str != NULL) {
		switch (pDoc->GetItemCond()) {
		case CTDOCERR_GOOD: icon = 1; break;
		case CTDOCERR_SYNTAX: icon = 0; break;
		case CTDOCERR_ARITY: icon = 2; break;
		default: icon = 3; break;
		}
		GetListCtrl().InsertItem(nItem++,str->c_str(),icon);
		str = pDoc->GetNextItem();
	}
	PrintNumPairs();
}


void CSIDView::OnClick(NMHDR* pNMHDR, LRESULT* pResult) 
{
	CChildFrame* pCframe = GetChildFrame();

	if(IsModifing()) {
		pCframe->SendMessage(WM_COMMAND,MAKEWPARAM(ID_INPUTOK,1),NULL);
		DisableModifyMode();
	}
	if(IsWaitingInput()) {
		pCframe->SendMessage(WM_COMMAND,MAKEWPARAM(ID_INPUTOK,1),NULL);
		DisableInputMode();
	}
	if(IsWordPMode()) {
		pCframe->SendMessage(WM_COMMAND,MAKEWPARAM(ID_INPUTCANCEL,1),NULL);
		DisableWordPMode();
	}

	if(pResult) *pResult = 0;
}


bool CSIDView::IsWaitingInput() 
{ 
	return m_inputmode; 
}


void CSIDView::SetInputMode() 
{ 
	if (IsWaitingInput()||IsModifing()||IsWordPMode()) return;

	m_inputmode = true; 
}


void CSIDView::SetWordPMode()
{
	if (IsWaitingInput()||IsModifing()||IsWordPMode()) return;
	
	CInputBar *pInBar;
	pInBar = GetChildFrame()->GetInputBar();
	CMainFrame* pMainFrame = (CMainFrame*)::AfxGetMainWnd();
	COutputBar* pOBar = (COutputBar*)(pMainFrame->GetControlBar(IDW_OUTPUT_BAR));
	
	CString sep = GetDocument()->GetTitle();
	sep += " - Problema della parola"; 
	pOBar->PrintSep(sep,sep.GetLength());

	pInBar->GetEditCtrl()->EnableWindow();
	pInBar->GetEditCtrl()->SetWindowText("");
	pInBar->GetEditCtrl()->SetFocus();

	GetDocument()->Backup();
	m_wordpmode = true;
}


void CSIDView::DisableWordPMode()
{
	CInputBar*  pInBar = GetChildFrame()->GetInputBar();
	CMainFrame* pMainFrame = (CMainFrame*) ::AfxGetMainWnd();
	COutputBar* pOBar = (COutputBar*) (pMainFrame->GetControlBar(IDW_OUTPUT_BAR));

	pOBar->PrintText(CString("\n--- fine Problema della parola\n"));
			
	pInBar->GetEditCtrl()->EnableWindow(FALSE);
	pInBar->GetEditCtrl()->SetWindowText("");
	SetFocus();
	PrintNumPairs();

	if (GetDocument()->CanUndo())
		GetDocument()->Undo();
	m_wordpmode = false;
}


void CSIDView::SetModifyMode(unsigned int _nitem)
{
	if (IsWaitingInput()||IsModifing()||IsWordPMode()) return;

	m_moditem = _nitem;
	m_modifymode = true;
}

bool CSIDView::IsModifing()
{
	return	m_modifymode;
}

unsigned int CSIDView::GetModItem() 
{
	return m_moditem;
}

void CSIDView::DisableModifyMode()
{
	CInputBar*  pInBar = GetChildFrame()->GetInputBar();

	pInBar->GetEditCtrl()->EnableWindow(FALSE);
	pInBar->GetEditCtrl()->SetWindowText("");
	GetListCtrl().SetSelectionMark(GetModItem()+1);
	SetFocus();

	PrintNumPairs();
	m_modifymode = false;
}


void CSIDView::DisableInputMode()
{
	CInputBar*  pInBar = GetChildFrame()->GetInputBar();

	pInBar->GetEditCtrl()->EnableWindow(FALSE);
	pInBar->GetEditCtrl()->SetWindowText("");
	SetFocus();

	PrintNumPairs();
	m_inputmode = false;
}


void CSIDView::OnDblclk(NMHDR* pNMHDR, LRESULT* pResult) 
{	
	CChildFrame* pCframe = GetChildFrame();
	if(IsModifing()) {
		pCframe->SendMessage(WM_COMMAND,MAKEWPARAM(ID_INPUTOK,1),NULL);
		DisableModifyMode();
	}
	if(IsWaitingInput()) {
		pCframe->SendMessage(WM_COMMAND,MAKEWPARAM(ID_INPUTCANCEL,1),NULL);
		DisableInputMode();
	}
	if(IsWordPMode()) {
		pCframe->SendMessage(WM_COMMAND,MAKEWPARAM(ID_INPUTCANCEL,1),NULL);
		DisableWordPMode();
	}

	SendMessage(WM_COMMAND,MAKEWPARAM(ID_SIDVIEW_MODIFICA,1),NULL);
	
	*pResult = 0;
}


void CSIDView::OnContextMenu(CWnd*, CPoint point)
{
	OnClick(NULL,NULL);

	if (point.x == -1 && point.y == -1){
		CRect rect;
		GetClientRect(rect);
		ClientToScreen(rect);
		point = rect.TopLeft();
		point.Offset(5, 5);
	}

	CMenu menu;
	CChildFrame* pCframe = GetChildFrame();

	if (GetListCtrl().GetFirstSelectedItemPosition())
		VERIFY(menu.LoadMenu(IDR_POPUP_SIDVIEW_ITEM));
	else
		VERIFY(menu.LoadMenu(IDR_POPUP_SIDVIEW));

	CMenu* pPopup = menu.GetSubMenu(0);
	ASSERT(pPopup != NULL);
	CWnd* pWndPopupOwner = this;
	
	while (pWndPopupOwner->GetStyle() & WS_CHILD)
		pWndPopupOwner = pWndPopupOwner->GetParent();

	pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, pWndPopupOwner);
}


void CSIDView::OnSidviewElimina() 
{
	CTeresaDoc	*pDoc;
	int			nItem;
	
	pDoc = GetDocument();	
	pDoc->Backup();

	nItem = GetListCtrl().GetNextItem(-1, LVNI_SELECTED);
	while(nItem>=0) {
		pDoc->RemoveItem(nItem);
		nItem = GetListCtrl().GetNextItem(nItem, LVNI_SELECTED);
	}
	
	Sincronizza();	
	pDoc->UpdateAllViews(this);
}


void CSIDView::OnSidviewModifica() 
{
	CString		itemText;
	CInputBar  *pInBar;
	
	pInBar = GetChildFrame()->GetInputBar();

	POSITION pos = GetListCtrl().GetFirstSelectedItemPosition();
	if(pos != NULL)	{
		int nItem = GetListCtrl().GetNextSelectedItem(pos);
		itemText = GetListCtrl().GetItemText(nItem,0);
		SetModifyMode(nItem);
		pInBar->PrintText((LPCTSTR)itemText);
		pInBar->GetEditCtrl()->EnableWindow();
		pInBar->GetEditCtrl()->SetFocus();
	}
}


void CSIDView::OnUpdateSidviewElimina(CCmdUI* pCmdUI) 
{
	if(IsModifing()||IsWaitingInput()||IsWordPMode()) {
		pCmdUI->Enable(FALSE);
		return;
	}
	if(GetListCtrl().GetFirstSelectedItemPosition()==NULL)
		pCmdUI->Enable(FALSE);
	else
		pCmdUI->Enable(TRUE);
}


void CSIDView::OnUpdateSidviewModifica(CCmdUI* pCmdUI) 
{
	if(IsModifing()||IsWaitingInput()||IsWordPMode()) {
		pCmdUI->Enable(FALSE);
		return;
	}
	if(GetListCtrl().GetFirstSelectedItemPosition()==NULL)
		pCmdUI->Enable(FALSE);
	else
		pCmdUI->Enable(TRUE);	
}


void CSIDView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
	Sincronizza();
}


void CSIDView::OnSidviewOrder() 
{
	ShowOrderDlg(*GetDocument()->GetOrderStr());
}


void CSIDView::OnSidviewRefresh() 
{
	GetDocument()->Refresh();
	GetDocument()->UpdateAllViews(this);
	Sincronizza();
}


void CSIDView::ShowOrderDlg(string _ordstr)
{
	COrderDlg dlg;

	dlg.m_signature = GetDocument()->GetSignature();
	dlg.m_order = _ordstr.c_str();
	if (dlg.DoModal()==IDOK) {
		try {
			GetDocument()->SetOrder((LPCTSTR)dlg.m_order);
		} catch(CTeresaDocException E) {
			COrderProbDlg probdlg;
			probdlg.m_edit = E.m_mex.c_str();
			switch (probdlg.DoModal()) {
			case IDOK:
				GetDocument()->SetOrder((LPCTSTR)dlg.m_order,true);
				break;
			case IDRETRY:
				ShowOrderDlg((LPCTSTR)dlg.m_order);
				break;
			default:
				break;
			}
		}
	}
}

void CSIDView::OnSidviewTestorder() 
{
	CMainFrame* pMainFrame = (CMainFrame*)::AfxGetMainWnd();

    COutputBar* pOBar = (COutputBar*)(pMainFrame->GetControlBar(IDW_OUTPUT_BAR));
	
	trsTermPairs* pTeoria;
	trsOrder* pOrder = GetDocument()->GetOrder();
	try {
		pTeoria = GetDocument()->GetTheory();
	} catch(CTeresaDocException E) {
		MessageBox("Impossibile procedere al test dell'ordine.\n\
Nella teoria inserita sono presenti degli errori,\ncorreggerli e riprovare.",
			NULL,MB_OK|MB_ICONWARNING);
		return;
	}
	CString sep(GetDocument()->GetTitle());
	sep += " - Prova ordine"; 
	pOBar->PrintSep(sep,sep.GetLength());
	
	if (GetApp()->IsVerbose()) {
		TestorderVerbose(pTeoria, pOrder);
		return;
	}

	trsTermPairs::iterator it;
	it = pTeoria->begin();
	while (it != pTeoria->end()) {
		trsTerm* s = it->GetLeft();
		trsTerm* t = it->GetRight();
		CString outstr;
		bool ls=false, gr=false, eq=false;
		try {
			gr = pOrder->Greater(s,t);
		} catch(...) {}
		if (!gr) {
			try {
				ls = pOrder->Less(s,t);
			} catch(...) {}
		}
		if ((!gr) && (!ls)) {
			try {
				eq = pOrder->Equal(s,t);
			} catch(...) {}
		}
		outstr = s->GetPrintable().c_str();
		if (gr) outstr += " > ";
		else if (ls) outstr += " < ";
		else if (eq) outstr += " = ";
		else outstr += " ? ";
		outstr += t->GetPrintable().c_str();
		outstr += "\n";
		pOBar->PrintText(outstr);
		
		it++;
	}	
}

void CSIDView::OnUpdateSidviewTestorder(CCmdUI* pCmdUI) 
{
	if(IsModifing()||IsWaitingInput()||IsWordPMode()) {
		pCmdUI->Enable(FALSE);
		return;
	}
	pCmdUI->Enable((GetDocument()->IsOrderValid())
		&&
		(GetDocument()->GetNumItems()>0));
}

void CSIDView::OnSidviewUnificazione() 
{	
	CMainFrame* pMainFrame = (CMainFrame*) ::AfxGetMainWnd();

    COutputBar* pOBar = (COutputBar*) (pMainFrame->GetControlBar(IDW_OUTPUT_BAR));
	
	trsTermPairs* pTeoria;
	try {
		pTeoria = GetDocument()->GetTheory();
	} catch(CTeresaDocException E) {
		MessageBox("Impossibile procedere all'unificazione.\n\
Nella teoria inserita sono presenti degli errori,\ncorreggerli e riprovare.",
			NULL,MB_OK|MB_ICONWARNING);
		return;
	}

	trsOutputDev &outdev = pOBar->GetOutputDev();
	trsUnify unif;
	trsSubst subst;

	if (GetApp()->IsVerbose())
		unif.SetDevices(&outdev,NULL);

	CString sep(GetDocument()->GetTitle());
	sep += " - Unificazione"; 
	pOBar->PrintSep(sep,sep.GetLength());

	try {
		unif.Unify(*pTeoria,subst);
	} catch(trsUnifyException E) {
		outdev << E.GetMsg();
		return;
	}
	if (GetApp()->IsVerbose())
		outdev << "\n*** Risultato ***\n\n" << subst;
	else
		outdev << subst;
}

void CSIDView::OnSidviewSwapterms() 
{
	int nItem;
	
	GetDocument()->Backup();

	nItem = GetListCtrl().GetNextItem(-1, LVNI_SELECTED);
	while(nItem>=0) {
		if (!GetDocument()->SwapTerms(nItem)) {
			CString mex;
			mex = "Impossibile invertire\n";
			mex += GetListCtrl().GetItemText(nItem,0);
			mex += "\nNella coppia sono presenti degli errori, correggerli e riprovare.";
			MessageBox((LPCTSTR)mex, NULL, MB_OK|MB_ICONWARNING);
		}
		nItem = GetListCtrl().GetNextItem(nItem, LVNI_SELECTED);
	}
	Sincronizza();
	GetDocument()->UpdateAllViews(this);
}

void CSIDView::OnUpdateSidviewSwapterms(CCmdUI* pCmdUI) 
{
	CListCtrl *pList = &GetListCtrl();
	if(IsModifing()||IsWaitingInput()||IsWordPMode()) {
		pCmdUI->Enable(FALSE);
		return;
	}
	if(pList->GetFirstSelectedItemPosition()==NULL)
		pCmdUI->Enable(FALSE);
	else
		pCmdUI->Enable(TRUE);	
}


void CSIDView::OnSidviewCoppiecritiche() 
{
	CMainFrame* pMainFrame = (CMainFrame*) ::AfxGetMainWnd();

	COutputBar* pOBar = (COutputBar*) (pMainFrame->GetControlBar(IDW_OUTPUT_BAR));
	
	trsTermPairs* pTeoria;
	trsAlgebra* pAlgebra;
	try {
		pTeoria = GetDocument()->GetTheory();
		pAlgebra = GetDocument()->GetAlgebra();
	} catch(CTeresaDocException E) {
		MessageBox("Impossibile procedere al calcolo delle coppie critiche.\n\
Nella teoria inserita sono presenti degli errori,\ncorreggerli e riprovare.",
			NULL,MB_OK|MB_ICONWARNING);
		return;
	}

	trsOutputDev &outdev = pOBar->GetOutputDev();

	CString sep(GetDocument()->GetTitle());
	sep += " - Coppie critiche"; 
	pOBar->PrintSep(sep,sep.GetLength());

	trsCriticalPairs cp;
	trsTermPairs cplist;
		
	if (GetApp()->IsVerbose())
		cp.SetDevices(&outdev,NULL);
	
	// backup necessario poiche` potrebbero essere create nuove variabili 
	// durante il processo di unrenaming.
	GetDocument()->Backup();

	try {
		cp.CriticalPairs(*pTeoria,cplist,*pAlgebra,true);
	} catch (trsException E) {
		outdev << "\nERRORE Coppie critiche:\n  \"" << E.GetMsg() << "\"\n\n";
		GetDocument()->Undo();
		return;
	}
	if (GetApp()->IsVerbose())
		outdev << "\n*** Risultato ***\n\n";

	if (cplist.empty()) {
		outdev << "Nessuna coppia critica trovata.\n";
		GetDocument()->Undo();
		return;
	}
	
	trsTermPairs::iterator it;
	for (it = cplist.begin(); it != cplist.end(); it++)
		outdev << "< " << it->GetLeft()->GetPrintable() 
		<< " , " << it->GetRight()->GetPrintable() << " >\n";

	// prima di Undo() bisogna cancellare la lista di coppie critiche
	cplist.clear();

	GetDocument()->Undo();
}

void CSIDView::OnSidviewProperties() 
{
	CListCtrl	*pList;
	CEquProp	eprop;
	CTeoriaProp	tprop;
	CTeresaDoc& doc = *GetDocument();
	int res;
	CString* pDesc = doc.GetDescription();

	CPropertySheet	sheet("Proprietà");
	sheet.m_psh.dwFlags |= PSH_NOAPPLYNOW;

	pList = &GetListCtrl();

	tprop.m_descstr = *pDesc;
	tprop.m_signature = doc.GetSignature();
	tprop.m_varlist = doc.GetVarList();
	tprop.m_ordervalid = doc.IsOrderValid();
	if (doc.IsOrderValid())
		tprop.m_order = doc.GetOrderStr()->c_str();
	else
		tprop.m_order = doc.GetOrderProb()->c_str();

	POSITION pos = pList->GetFirstSelectedItemPosition();
	if(pos != NULL) {
		int nItem = pList->GetNextSelectedItem(pos);		
		eprop.m_equation = doc.GetItem(nItem)->c_str();
		eprop.m_text = doc.GetItemDesc(nItem)->c_str();
		sheet.AddPage(&eprop);
		sheet.AddPage(&tprop);
		res = sheet.DoModal();
	} else {
		sheet.AddPage(&tprop);
		res = sheet.DoModal();
	}
	if (res == IDOK) {
		*pDesc = tprop.m_descstr;
		doc.SetModifiedFlag();
	}
}

void CSIDView::OnSidviewHuet() 
{
	CHuetDlg hdlg;
	CMainFrame* pMainFrame = (CMainFrame*) ::AfxGetMainWnd();
    COutputBar* pOBar = (COutputBar*) (pMainFrame->GetControlBar(IDW_OUTPUT_BAR));

	try {
		hdlg.m_system = GetDocument()->GetSys();
		hdlg.m_order = GetDocument()->GetOrder();
	} catch(CTeresaDocException E) {
		MessageBox("Impossibile completare.\n\
Nella teoria sono presenti degli errori,\ncorreggerli e riprovare.",
			NULL,MB_OK|MB_ICONWARNING);
		return;
	}

	hdlg.DoModal();

	CString sep(GetDocument()->GetTitle());
	sep += " - Completamento"; 
	pOBar->PrintSep(sep,sep.GetLength());
	
	pOBar->PrintText(hdlg.m_mex);

	GetDocument()->SetModifiedFlag();
	GetDocument()->UpdateAllViews(this);
}


void CSIDView::OnUpdateSidviewHuet(CCmdUI* pCmdUI) 
{
	if(IsModifing()||IsWaitingInput()||IsWordPMode()) {
		pCmdUI->Enable(FALSE);
		return;
	}
	pCmdUI->Enable((GetDocument()->IsOrderValid())
		&&
		(GetDocument()->GetNumItems()>0));
}


void CSIDView::OnSidviewDeleteall() 
{
	GetDocument()->Backup();
	GetDocument()->RemoveAllItems();
	Sincronizza();
	GetDocument()->UpdateAllViews(this);
}


void CSIDView::OnUpdateSidviewDeleteall(CCmdUI* pCmdUI) 
{
	if(IsModifing()||IsWaitingInput()||IsWordPMode()) {
		pCmdUI->Enable(FALSE);
		return;
	}
	pCmdUI->Enable(GetListCtrl().GetItemCount());
}


void CSIDView::PrintNumPairs()
{
	CTeresaDoc	*pDoc;
	CInputBar	*pInBar;
	
	pDoc = GetDocument();
	pInBar = (CInputBar*)(GetChildFrame()->GetDlgItem(IDW_INPUTBAR));

	char buf[32];
	sprintf(buf,"coppie: %d",pDoc->GetNumItems());
	pInBar->PrintText(buf);
}

void CSIDView::OnUpdateSidviewCoppiecritiche(CCmdUI* pCmdUI) 
{
	if(IsModifing()||IsWaitingInput()||IsWordPMode()) {
		pCmdUI->Enable(FALSE);
		return;
	}
	pCmdUI->Enable(GetDocument()->GetNumItems() > 0);	
}

void CSIDView::OnUpdateSidviewUnificazione(CCmdUI* pCmdUI) 
{
	if(IsModifing()||IsWaitingInput()||IsWordPMode()) {
		pCmdUI->Enable(FALSE);
		return;
	}
	pCmdUI->Enable(GetDocument()->GetNumItems() > 0);	
}


void CSIDView::OnWordproblem() 
{
	if (m_wordpmode)
		DisableWordPMode();
	else
		SetWordPMode();
}


void CSIDView::OnUpdateWordproblem(CCmdUI* pCmdUI) 
{
	CTeresaDoc	*pDoc;

	pDoc = GetDocument();
	
	if (IsWordPMode())
		pCmdUI->SetCheck(1);
	else
		pCmdUI->SetCheck(0);
	
	if(IsModifing() || IsWaitingInput())
		pCmdUI->Enable(FALSE);
	
	if (pDoc->GetTrs()->empty()) {
		pCmdUI->SetCheck(0);
		pCmdUI->Enable(FALSE);
	}
}

void CSIDView::WordProblem(CString &_incoppia)
{
	CTeresaDoc	*pDoc;
	bool        ret;
	CString     normal;

	pDoc = GetDocument();
	CMainFrame* pMainFrame = (CMainFrame*)::AfxGetMainWnd();
    COutputBar* pOBar = (COutputBar*)(pMainFrame->GetControlBar(IDW_OUTPUT_BAR));

	try {
		ret = pDoc->WordProblem(_incoppia,normal);
	} catch (CTeresaDocException E) {
		pOBar->GetOutputDev() << E.m_mex;
		return;
	}

	pOBar->GetOutputDev() << "\ncoppia       : " << (LPCTSTR)_incoppia << 
		"\nnormalizzata : " << (LPCTSTR)normal << "\nrisultato    : ";
	if (ret)
		pOBar->GetOutputDev() << "VERO\n";
	else
		pOBar->GetOutputDev() << "FALSO\n";
}

void CSIDView::OnEditCopy() 
{
	CTeresaDoc	*pDoc;
	int			nItem;
	
	pDoc = GetDocument();
	CString data("");

	nItem = GetListCtrl().GetNextItem(-1, LVNI_SELECTED);
	while(nItem>=0) {
		data += pDoc->GetItem(nItem)->c_str();
		data += "\r\n";
		nItem = GetListCtrl().GetNextItem(nItem, LVNI_SELECTED);
	}
	OpenClipboard();
	EmptyClipboard();
	SetClipboardData(CF_TEXT,(void*)((LPCTSTR)data));
    CloseClipboard();
}

void CSIDView::OnUpdateEditCopy(CCmdUI* pCmdUI) 
{
	if(IsModifing()||IsWaitingInput()||IsWordPMode()) {
		pCmdUI->Enable(FALSE);
		return;
	}
	if(GetListCtrl().GetFirstSelectedItemPosition()==NULL)
		pCmdUI->Enable(FALSE);
}

void CSIDView::OnEditCut() 
{
	CTeresaDoc	*pDoc;
	int			nItem;
	
	pDoc = GetDocument();
	CString data("");

	pDoc->Backup();
	nItem = GetListCtrl().GetNextItem(-1, LVNI_SELECTED);
	while(nItem>=0) {
		data += GetListCtrl().GetItemText(nItem,0);
		data += "\r\n";
		pDoc->RemoveItem(nItem);
		nItem = GetListCtrl().GetNextItem(nItem, LVNI_SELECTED);
	}
	OpenClipboard();
	EmptyClipboard();
	SetClipboardData(CF_TEXT,(void*)((LPCTSTR)data));
    CloseClipboard();	
	Sincronizza();
	pDoc->UpdateAllViews(NULL);
}

void CSIDView::OnUpdateEditCut(CCmdUI* pCmdUI) 
{
	if(IsModifing()||IsWaitingInput()||IsWordPMode()) {
		pCmdUI->Enable(FALSE);
		return;
	}
	if(GetListCtrl().GetFirstSelectedItemPosition()==NULL)
		pCmdUI->Enable(FALSE);
}

void CSIDView::OnUpdateEditPaste(CCmdUI* pCmdUI) 
{
	if(IsModifing()||IsWaitingInput()||IsWordPMode()) {
		pCmdUI->Enable(FALSE);
		return;
	}
	OpenClipboard();
	if(GetClipboardData(CF_TEXT) == NULL)
		pCmdUI->Enable(FALSE);
	CloseClipboard();
}

void CSIDView::OnEditPaste() 
{
	char *clipdata, *str, *coppia;

	GetDocument()->Backup();

	OpenClipboard();
	clipdata = (char*)GetClipboardData(CF_TEXT);
	str = new char[strlen(clipdata)+1];
	strcpy(str,clipdata);
	CloseClipboard();

	coppia = strtok(str,"\r\n");
	while (coppia != NULL) {
		try {
			GetDocument()->InsertItem(string(coppia));
		} catch(CTeresaDocException) {}
		coppia = strtok(NULL,"\r\n");
	}
	delete[] str;
	Sincronizza();
	GetDocument()->UpdateAllViews(this);
}

void CSIDView::OnUpdateSidviewProperties(CCmdUI* pCmdUI) 
{
	if(IsModifing()||IsWaitingInput()||IsWordPMode())
		pCmdUI->Enable(FALSE);
}

void CSIDView::OnUpdateSidviewOrder(CCmdUI* pCmdUI) 
{
	if(IsModifing()||IsWaitingInput()||IsWordPMode())
		pCmdUI->Enable(FALSE);
}


CChildFrame* CSIDView::GetChildFrame()
{
	//il primo parent e' lo splitter
	return (CChildFrame*)(GetParent()->GetParent()); 
}

CTeresaApp* CSIDView::GetApp()
{
	return (CTeresaApp*)AfxGetApp();
}

//@@
void CSIDView::OnUpdateFilePrint(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(FALSE);	
}
//@@
void CSIDView::OnUpdateFilePrintPreview(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(FALSE);	
}

void CSIDView::TestorderVerbose(trsTermPairs *_teoria, trsOrder *_ordine)
{
	CMainFrame* pMainFrame = (CMainFrame*)::AfxGetMainWnd();
    COutputBar* pOBar = (COutputBar*)(pMainFrame->GetControlBar(IDW_OUTPUT_BAR));
	trsOutputDev &outdev = pOBar->GetOutputDev();
	_ordine->SetDevices(&outdev,NULL);

	trsTermPairs::iterator it;
	it = _teoria->begin();
	while (it != _teoria->end()) {
		trsTerm* s = it->GetLeft();
		trsTerm* t = it->GetRight();
		string str1, str2;
		CString outstr;
		str1 = s->GetPrintable();
		str2 = t->GetPrintable();
		bool ls=false, gr=false;
		outstr.Format("\nConfronto i termini %s e %s\n",str1.c_str(),str2.c_str());
		pOBar->PrintText(outstr);
		try {
			outstr.Format("??? %s > %s ???\n",str1.c_str(),str2.c_str());
			pOBar->PrintText(outstr);
			if (!(gr = _ordine->Greater(s,t))) {
				outstr.Format("--- %s non e` maggiore di %s ---\n",str1.c_str(),str2.c_str());
				pOBar->PrintText(outstr);
			}
		} catch(trsException E) {
			outdev << E.GetMsg() << "\n";
		}

		if (!gr) {
			try {
				outstr.Format("??? %s < %s ? ???\n",str1.c_str(),str2.c_str());
				pOBar->PrintText(outstr);
				if (!(ls = _ordine->Less(s,t))) {
					outstr.Format("--- %s non e` minore di %s ---\n",str1.c_str(),str2.c_str());
					pOBar->PrintText(outstr);
				}
			} catch(trsException E) {
				outdev << E.GetMsg() << "\n";
			}
		}
		if (!ls && !gr) {
			try {
				outstr.Format("??? %s = %s ???\n",str1.c_str(),str2.c_str());
				pOBar->PrintText(outstr);
				if (!_ordine->Equal(s,t)) {
					outstr.Format("--- %s e %s non sono uguali. ---\n",str1.c_str(),str2.c_str());
					pOBar->PrintText(outstr);
				}
			} catch(trsException E) {
				outdev << E.GetMsg() << "\n";
			}
		}
		it++;
	}	
	_ordine->SetDevices(NULL,NULL);
}

void CSIDView::OnTrsviewDeleteall() 
{
	GetDocument()->Backup();
	GetDocument()->ClearTrs();
	GetDocument()->UpdateAllViews(this);
}

void CSIDView::OnUpdateTrsviewDeleteall(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(!GetDocument()->GetTrs()->empty());	
}
