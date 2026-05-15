/*//////////////////////////////////////////////////////////////////////////////
	FILENAME:	OrderDlg.cpp
	Version:	1.0.0
	Author:		Marco Bortolin
	Created:	27/07/2001

  Copyright (C) 2001 Marco Bortolin
//////////////////////////////////////////////////////////////////////////////*/

#include "stdafx.h"
#include "Teresa.h"
#include "OrderDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COrderDlg dialog


COrderDlg::COrderDlg(CWnd* pParent /*=NULL*/)
	: CDialog(COrderDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(COrderDlg)
	m_order = _T("");
	m_signlist = _T("");
	//}}AFX_DATA_INIT
	
	m_font.CreateFont(12,4,0,0,FW_NORMAL,false,false,false,ANSI_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY,FIXED_PITCH|FF_DONTCARE,"Courier");
	m_signature=NULL;
}


void COrderDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COrderDlg)
	DDX_Control(pDX, IDC_ORD_OP, m_ordop_ctrl);
	DDX_Control(pDX, IDC_SIGNATURE, m_signlist_ctrl);
	DDX_Control(pDX, IDC_ORDER_EDIT, m_order_ctrl);
	DDX_Text(pDX, IDC_ORDER_EDIT, m_order);
	DDX_LBString(pDX, IDC_SIGNATURE, m_signlist);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(COrderDlg, CDialog)
	//{{AFX_MSG_MAP(COrderDlg)
	ON_WM_SHOWWINDOW()
	ON_LBN_DBLCLK(IDC_SIGNATURE, OnDblclkSignature)
	ON_BN_CLICKED(IDC_GREATER, OnGreater)
	ON_BN_CLICKED(IDC_EQUAL, OnEqual)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COrderDlg message handlers

void COrderDlg::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	if (m_signature != NULL) {
		for (int i=0; i<m_signature->GetSize(); i++) {
			CString funcstr;
			funcstr.Format("%s (%d)",m_signature->GetSim(i)->c_str(),m_signature->GetArity(i));
			m_signlist_ctrl.InsertString(-1,(LPCTSTR)funcstr);
		}
	}
	m_order_ctrl.SetFont(&m_font);
	m_signlist_ctrl.SetFont(&m_font);
	m_ordop_ctrl.SetFont(&m_font);
	m_ordop_ctrl.SetWindowText(">");
}


void COrderDlg::OnDblclkSignature() 
{
	CString wndtext;
	CString func;
	CString op;
	
	m_order_ctrl.GetWindowText(wndtext);
	m_ordop_ctrl.GetWindowText(op);

	m_signlist_ctrl.GetText(m_signlist_ctrl.GetCurSel(),func);
	int pos = func.Find(" ");
	int size = func.GetLength() - pos;
	func.Delete(pos,size);
	wndtext += op;
	wndtext += func;

	if (wndtext[0]=='>' || wndtext[0]=='=')
		wndtext.Delete(0);
	
	m_order_ctrl.SetWindowText(wndtext);
}


void COrderDlg::OnGreater() 
{
	m_ordop_ctrl.SetWindowText(">");
}


void COrderDlg::OnEqual() 
{
	m_ordop_ctrl.SetWindowText("=");
}
