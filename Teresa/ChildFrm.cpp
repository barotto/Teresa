/*//////////////////////////////////////////////////////////////////////////////
	FILENAME:	ChildFrm.cpp
	Version:	1.0.0
	Author:		Marco Bortolin
	Created:	27/07/2001 - 23.41.40

	Copyright (C) 2001 Marco Bortolin
//////////////////////////////////////////////////////////////////////////////*/
#include "stdafx.h"
#include "Teresa.h"
#include "ChildFrm.h"
#include "TeresaDoc.h"
#include "TRSView.h"
#include "SIDView.h"
#include "TRSView.h"
#include "TermsView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CChildFrame

IMPLEMENT_DYNCREATE(CChildFrame, CMDIChildWnd)

BEGIN_MESSAGE_MAP(CChildFrame, CMDIChildWnd)
	//{{AFX_MSG_MAP(CChildFrame)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_COMMAND(ID_INPUTOK, OnInputOK)
	ON_COMMAND(ID_INPUTCANCEL, OnInputCancel)
	ON_COMMAND(ID_NUOVACOPPIA, OnNuovaCoppia)
	ON_WM_KEYDOWN()
	ON_WM_KILLFOCUS()
	ON_COMMAND(ID_TERMS_VIEW, OnTermsView)
	ON_COMMAND(ID_TRS_VIEW, OnTrsView)
	ON_COMMAND(ID_EDIT_UNDO, OnEditUndo)
	ON_UPDATE_COMMAND_UI(ID_EDIT_UNDO, OnUpdateEditUndo)
	ON_UPDATE_COMMAND_UI(ID_INPUTOK, OnUpdateInputOK)
	ON_UPDATE_COMMAND_UI(ID_INPUTCANCEL, OnUpdateInputCancel)
	ON_UPDATE_COMMAND_UI(ID_NUOVACOPPIA, OnUpdateNuovaCoppia)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChildFrame construction/destruction

CChildFrame::CChildFrame()
{
}

CChildFrame::~CChildFrame()
{
}

BOOL CChildFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	if( !CMDIChildWnd::PreCreateWindow(cs) )
		return FALSE;

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CChildFrame diagnostics

#ifdef _DEBUG
void CChildFrame::AssertValid() const
{
	CMDIChildWnd::AssertValid();
}

void CChildFrame::Dump(CDumpContext& dc) const
{
	CMDIChildWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CChildFrame message handlers

BOOL CChildFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext) 
{	
	m_wndSplitter.CreateStatic(this,1,2);

	RECT r;
	GetWindowRect(&r);
	CSize size;
	size.cx = (r.right-r.left)/2;
	size.cy = 50;
	if (!m_wndSplitter.CreateView(0,0,pContext->m_pNewViewClass, size, pContext))
	{
		TRACE0("Failed to create first pane\n");
		return FALSE;
	}
	if (!m_wndSplitter.CreateView(0,1,RUNTIME_CLASS(CTRSView),CSize(0,0),pContext))
	{
		TRACE0("Failed to create second pane\n");
		return FALSE;
	}
	SetActiveView(GetSIDView());
	return TRUE;
	
	//return CMDIChildWnd::OnCreateClient(lpcs, pContext);
}


int CChildFrame::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CMDIChildWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if(!m_wndInputbar.Create(this))	{
		ErrorExit("Impossibile creare la inputbar");
	}
	
	return 0;
}


void CChildFrame::OnSize(UINT nType, int cx, int cy) 
{
	CMDIChildWnd::OnSize(nType, cx, cy);
	
	m_wndInputbar.Resize(cx,cy);
}


void CChildFrame::OnInputOK() 
{
	CSIDView	*pView;
	CTeresaDoc	*pDoc;
	CString		inputstr,errmex;
	bool res = true;

	pView = (CSIDView*)GetSIDView();
	if( (!pView->IsWaitingInput()) && (!pView->IsModifing()) && (!pView->IsWordPMode()) ) 
		return;
	
	pDoc = (CTeresaDoc*)pView->GetDocument();
	m_wndInputbar.GetEditCtrl()->GetWindowText(inputstr);

	if(inputstr=="")
		return;

	m_wndInputbar.GetEditCtrl()->SetWindowText("");
	m_wndInputbar.GetEditCtrl()->EnableWindow(FALSE);

	if (pView->IsWordPMode()) {
		pView->WordProblem(inputstr);
		m_wndInputbar.GetEditCtrl()->EnableWindow(TRUE);
		m_wndInputbar.GetEditCtrl()->SetFocus();
		return;
	}

	pDoc->Backup();
	try {
		if (pView->IsWaitingInput()) 
			res = pDoc->InsertItem(inputstr);
		else
			res = pDoc->ModifyItem(pView->GetModItem(),inputstr);
	} 
	catch(CTeresaDocException E) {
		switch(E.m_code) {
		case (CTDOCERR_ARITY):
			// l'identita` viene comunque inserita per permettere all'utente 
			// una piu` agevole modifica della teoria.
			//errmex.LoadString(IDS_CTDOCERR_ARITY);
			//errmex += E.m_mex.c_str();	
			//MessageBox((LPCTSTR)errmex);
			break;
		case (CTDOCERR_DUP):
			// l'utente deve obbligatoriamente modificare il duplicato.
			m_wndInputbar.GetDlgItem(IDW_INPUTBAR_EDIT)->SendMessage(WM_COMMAND,
				MAKEWPARAM(ID_INPUTBAR_EDIT_IGNORAKEYUP,1),NULL);
			errmex.LoadString(IDS_CTDOCERR_DUP);
			MessageBox((LPCTSTR)errmex);
			m_wndInputbar.GetEditCtrl()->SetWindowText(inputstr);
			m_wndInputbar.GetEditCtrl()->EnableWindow(TRUE);
			m_wndInputbar.GetEditCtrl()->SetFocus();
			return;
		case (CTDOCERR_SYNTAX):
			// l'identita` viene inserita.
			break;
		}
	}	

	pDoc->SetModifiedFlag();
	pDoc->UpdateAllViews(NULL);

	if (pView->IsWaitingInput()) {
		pView->DisableInputMode();
		SendMessage(WM_COMMAND,MAKEWPARAM(ID_NUOVACOPPIA,1),NULL);
	} else {
		pView->DisableModifyMode();
		//pView->SetFocus();
	}
}


void CChildFrame::OnInputCancel() 
{
	CSIDView* pView = (CSIDView*)GetSIDView();
	
	m_wndInputbar.GetEditCtrl()->SetWindowText("");
	m_wndInputbar.GetEditCtrl()->EnableWindow(FALSE);

	if(pView->IsModifing())
		pView->DisableModifyMode();

	if(pView->IsWaitingInput())
		pView->DisableInputMode();
	
	if(pView->IsWordPMode())
		pView->DisableWordPMode();

	pView->SetFocus();
}


void CChildFrame::OnNuovaCoppia() 
{
	CSIDView* pView = (CSIDView*)GetSIDView();

	if(pView->IsWaitingInput())
		OnInputOK();

	if(pView->IsModifing())
		OnInputCancel();

	if(pView->IsWordPMode())
		pView->DisableWordPMode();

	pView->SetInputMode();

	m_wndInputbar.GetEditCtrl()->EnableWindow();
	m_wndInputbar.GetEditCtrl()->SetWindowText("");
	m_wndInputbar.GetEditCtrl()->SetFocus();
}


void CChildFrame::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	CMDIChildWnd::OnKeyDown(nChar, nRepCnt, nFlags);
}


void CChildFrame::OnKillFocus(CWnd* pNewWnd) 
{
	CMDIChildWnd::OnKillFocus(pNewWnd);
	
//	OnInputCancel();
}


CView* CChildFrame::GetSIDView()
{
	return (CView*)m_wndSplitter.GetPane(0,0);
}


CView* CChildFrame::GetRightView()
{
	return (CView*)m_wndSplitter.GetPane(0,1);
}


CInputBar* CChildFrame::GetInputBar()
{
	return &m_wndInputbar;
}


bool CChildFrame::ReplaceRightView(CRuntimeClass *pViewClass)
{
	CCreateContext context;
	bool bSetActive;
	int xsize,ysize,dummy;
	       
	if ((m_wndSplitter.GetPane(0,1)->IsKindOf(pViewClass)) == TRUE)
		return false;
				    
	m_wndSplitter.GetRowInfo(0,ysize,dummy);
	m_wndSplitter.GetColumnInfo(1,xsize,dummy);

	CDocument *pDoc = ((CView *)m_wndSplitter.GetPane(0,1))->GetDocument();
	CView *pActiveView = GetActiveView();
	if (pActiveView==NULL || pActiveView==m_wndSplitter.GetPane(0,1))
		bSetActive=true;
	else
		bSetActive=false;

	pDoc->m_bAutoDelete = FALSE;    
	((CView*)m_wndSplitter.GetPane(0,1))->DestroyWindow();
	pDoc->m_bAutoDelete = TRUE;
 
	context.m_pNewViewClass = pViewClass;
	context.m_pCurrentDoc = pDoc;
	context.m_pNewDocTemplate = pDoc->GetDocTemplate();
	context.m_pLastView = NULL;
	context.m_pCurrentFrame = this;
   
	m_wndSplitter.CreateView(0,1,pViewClass,CSize(xsize,ysize),&context);
   
	CView *pNewView = (CView*)m_wndSplitter.GetPane(0,1);
   
	if (bSetActive == true)
		SetActiveView(pNewView);
   
	m_wndSplitter.RecalcLayout(); 
	m_wndSplitter.GetPane(0,1)->SendMessage(WM_PAINT);
   
	return true;
}

//@@ obsoleta
void CChildFrame::OnTermsView() 
{
	CString wndtext;

	// Al cambio della View il nome del documento sparisce dal caption della finestra;
	GetWindowText(wndtext);
	ReplaceRightView(RUNTIME_CLASS(CTermsView));
	GetRightView()->OnInitialUpdate();
	SetWindowText((LPCTSTR)wndtext);
}

//@@ obsoleta
void CChildFrame::OnTrsView() 
{
	CString wndtext;

	// stesso problema di cui sopra...
	GetWindowText(wndtext);
	ReplaceRightView(RUNTIME_CLASS(CTRSView));	
	GetRightView()->OnInitialUpdate();
	SetWindowText((LPCTSTR)wndtext);
}


void CChildFrame::ActivateFrame(int nCmdShow) 
{
	nCmdShow = SW_SHOWMAXIMIZED;
	CMDIChildWnd::ActivateFrame(nCmdShow);
}

void CChildFrame::OnEditUndo() 
{
	CSIDView	*pView;
	CTeresaDoc	*pDoc;

	pView = (CSIDView*)GetSIDView();
	pDoc = (CTeresaDoc*)pView->GetDocument();

	pDoc->Undo();
	pDoc->UpdateAllViews(NULL);
}

void CChildFrame::OnUpdateEditUndo(CCmdUI* pCmdUI) 
{
	CSIDView	*pView;
	CTeresaDoc	*pDoc;

	pView = (CSIDView*)GetSIDView();
	pDoc = (CTeresaDoc*)pView->GetDocument();
	
	if(pView->IsWaitingInput() || pView->IsModifing() || (pView->IsWordPMode())) {
		pCmdUI->Enable(FALSE);
		return;
	}
	pCmdUI->Enable(pDoc->CanUndo());
}

CMainFrame* CChildFrame::GetMainFrame()
{
	return (CMainFrame*) ::AfxGetMainWnd();
}

void CChildFrame::OnUpdateInputOK(CCmdUI* pCmdUI) 
{
	CSIDView* pView = (CSIDView*)GetSIDView();

	if((!pView->IsWaitingInput()) && (!pView->IsModifing()) && (!pView->IsWordPMode()))
		pCmdUI->Enable(FALSE);
}

void CChildFrame::OnUpdateInputCancel(CCmdUI* pCmdUI) 
{
	CSIDView* pView = (CSIDView*)GetSIDView();

	if((!pView->IsWaitingInput()) && (!pView->IsModifing()) && (!pView->IsWordPMode()))
		pCmdUI->Enable(FALSE);
}


void CChildFrame::OnUpdateNuovaCoppia(CCmdUI* pCmdUI) 
{
	CSIDView* pView = (CSIDView*)GetSIDView();

	if(pView->IsWaitingInput() || pView->IsModifing() || pView->IsWordPMode())
		pCmdUI->Enable(FALSE);
}
