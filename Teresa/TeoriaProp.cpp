/*//////////////////////////////////////////////////////////////////////////////
	FILENAME:	TeoriaProp.cpp
	Version:	1.0.0
	Author:		Marco Bortolin
	Created:	09/08/2001 - 23.40.34

	Implementazione di CTeoriaProp (CPropertyPage della teoria)
	Copyright (C) 2001 Marco Bortolin
//////////////////////////////////////////////////////////////////////////////*/

#include "stdafx.h"
#include "Teresa.h"
#include "TeoriaProp.h"
#include "DescDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTeoriaProp property page

IMPLEMENT_DYNCREATE(CTeoriaProp, CPropertyPage)

CTeoriaProp::CTeoriaProp() : CPropertyPage(CTeoriaProp::IDD)
{
	//{{AFX_DATA_INIT(CTeoriaProp)
	m_numfunc_str = _T("");
	m_order = _T("");
	//}}AFX_DATA_INIT

	m_font.CreateFont(12,4,0,0,FW_NORMAL,false,false,false,ANSI_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY,FIXED_PITCH|FF_DONTCARE,"Courier");


	m_ordervalid = false;
	m_bFirstShow = true;
	m_signature = NULL;
	m_varlist = NULL;
}

CTeoriaProp::~CTeoriaProp()
{
}

void CTeoriaProp::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTeoriaProp)
	DDX_Control(pDX, IDC_NUMVAR, m_numvar_ctrl);
	DDX_Control(pDX, IDC_VARLIST, m_varlist_ctrl);
	DDX_Control(pDX, IDC_FUNCLIST, m_funclist_ctrl);
	DDX_Control(pDX, IDC_ORDER, m_order_ctrl);
	DDX_Control(pDX, IDC_NUMFUNC, m_numfunc);
	DDX_Text(pDX, IDC_NUMFUNC, m_numfunc_str);
	DDX_Text(pDX, IDC_ORDER, m_order);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTeoriaProp, CPropertyPage)
	//{{AFX_MSG_MAP(CTeoriaProp)
	ON_WM_SHOWWINDOW()
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_TEORIAPROP_DESC, OnTeoriapropDesc)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTeoriaProp message handlers

void CTeoriaProp::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CPropertyPage::OnShowWindow(bShow, nStatus);
	CString str;

	if (m_bFirstShow) {
		//m_funclist.SendMessage(LB_SETCOLUMNWIDTH,80);
		m_funclist_ctrl.SetFont(&m_font);
		m_numfunc.SetFont(&m_font);;
		m_numfunc.SetWindowText("0");
		
		m_order_ctrl.SetFont(&m_font);
		
		m_varlist_ctrl.SetFont(&m_font);
		m_numvar_ctrl.SetFont(&m_font);;
		m_numvar_ctrl.SetWindowText("0");

		if (m_signature != NULL) {
			for (int i=0; i<m_signature->GetSize(); i++) {
				str.Format("%s (%d)",m_signature->GetSim(i)->c_str(),m_signature->GetArity(i));
				m_funclist_ctrl.InsertString(-1,(LPCTSTR)str);
			}
			str.Format(" %d",m_signature->GetSize());
			m_numfunc.SetWindowText((LPCTSTR)str);
		}

		if (m_varlist != NULL) {
			trsVarList::iterator it;
			int cnt = 0;
			it = m_varlist->begin();
			while (it != m_varlist->end()) {
				m_varlist_ctrl.InsertString(-1,(*it)->GetPrintable().c_str());
				cnt++;
				it++;
			}
			str.Format(" %d",cnt);
			m_numvar_ctrl.SetWindowText((LPCTSTR)str);
		}
		/*
		if (!m_ordervalid) 
		{
			CDC* pDC = m_order_ctrl.GetWindowDC();
			pDC->SetTextColor(RGB(255,0,0));
			RECT r;
			m_order_ctrl.GetClientRect(&r);
			pDC->DrawText("ciao",4,&r,DT_LEFT);
			m_order_ctrl.ReleaseDC(pDC);
			m_order_ctrl.InvalidateRgn(NULL);
			m_order_ctrl.RedrawWindow();
		}
		*/
		m_bFirstShow = false;
	}
}

HBRUSH CTeoriaProp::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CPropertyPage::OnCtlColor(pDC, pWnd, nCtlColor);
	
	if (GetDlgItem(IDC_ORDER)==pWnd)
    {
        if (!m_ordervalid)
            pDC->SetTextColor( RGB(255,0,0) );
    }

	return hbr;
}

void CTeoriaProp::OnTeoriapropDesc() 
{
	CDescDlg descdlg;
	descdlg.m_edit = m_descstr;
	if (descdlg.DoModal()==IDOK) {
		m_descstr = descdlg.m_edit;
	}
}
