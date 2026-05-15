/*//////////////////////////////////////////////////////////////////////////////
	FILENAME:	EquProp.cpp
	Version:	1.0.0
	Author:		Marco Bortolin
	Created:	09/08/2001 - 23.52.26

	Implementazione di CEquProp (CPropertyPage di una equazione - coppia term)
	Copyright (C) 2001 Marco Bortolin
//////////////////////////////////////////////////////////////////////////////*/
#include "stdafx.h"
#include "Teresa.h"
#include "EquProp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEquProp property page

IMPLEMENT_DYNCREATE(CEquProp, CPropertyPage)

CEquProp::CEquProp() : CPropertyPage(CEquProp::IDD)
{
	//{{AFX_DATA_INIT(CEquProp)
	m_text = _T("");
	//}}AFX_DATA_INIT
	
	m_bFirstShow = true;
}

CEquProp::~CEquProp()
{
}

void CEquProp::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEquProp)
	DDX_Control(pDX, IDC_EQUPROP_EDIT, m_text_ctrl);
	DDX_Control(pDX, IDC_EQUPROP_TREE, m_tree);
	DDX_Text(pDX, IDC_EQUPROP_EDIT, m_text);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CEquProp, CPropertyPage)
	//{{AFX_MSG_MAP(CEquProp)
	ON_WM_SHOWWINDOW()
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEquProp message handlers


void CEquProp::BuildTree(HTREEITEM _parent, trsTerm *_term)
{	
	if(_term == NULL) return;
	
	HTREEITEM root;
	trsTerm* t=NULL;
	
	trsArgList::iterator argit;
	
	root = m_tree.InsertItem(_term->GetString()->c_str(),_parent);
	if(_term->IsFunc()) {
		trsFunc* func = (trsFunc*)_term;
		t = func->GetFirstArg(argit);
		while (t != NULL) {
			BuildTree(root,t);
			t = func->GetNextArg(argit);
		}
	}
}


void CEquProp::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CPropertyPage::OnShowWindow(bShow, nStatus);

	if (m_bFirstShow) {
		m_font.CreateFont(8,2,0,0,FW_NORMAL,false,false,false,ANSI_CHARSET,
			OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,
			DEFAULT_QUALITY,FIXED_PITCH|FF_DONTCARE,"Courier");	

		trsTermPair tp;
				
		try {
			//@@
			m_parser.GetPair(m_equation.c_str(),&tp,m_sys.GetAlgebra());
			BuildTree(TVI_ROOT,tp.GetLeft());
			BuildTree(TVI_ROOT,tp.GetRight());
		} catch(...) {}

		m_text_ctrl.SetFont(&m_font);
		m_tree.SetFont(&m_font);
		m_tree.SetFocus();
		m_bFirstShow = false;
	}
}

int CEquProp::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CPropertyPage::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	return 0;
}
