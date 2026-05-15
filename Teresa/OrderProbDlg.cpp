/*//////////////////////////////////////////////////////////////////////////////
	FILENAME:	OrderProbDlg.cpp
	Version:	1.0.0
	Author:		Marco Bortolin
	Created:	27/07/2001

  Copyright (C) 2001 Marco Bortolin
//////////////////////////////////////////////////////////////////////////////*/

#include "stdafx.h"
#include "teresa.h"
#include "OrderProbDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COrderProbDlg dialog


COrderProbDlg::COrderProbDlg(CWnd* pParent /*=NULL*/)
	: CDialog(COrderProbDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(COrderProbDlg)
	m_edit = _T("");
	//}}AFX_DATA_INIT

	m_font.CreateFont(12,4,0,0,FW_NORMAL,false,false,false,ANSI_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY,FIXED_PITCH|FF_DONTCARE,"Courier");
}


void COrderProbDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COrderProbDlg)
	DDX_Control(pDX, IDC_ORDER_PROB_EDIT, m_edit_ctrl);
	DDX_Text(pDX, IDC_ORDER_PROB_EDIT, m_edit);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(COrderProbDlg, CDialog)
	//{{AFX_MSG_MAP(COrderProbDlg)
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDRETRY, OnRetry)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COrderProbDlg message handlers


void COrderProbDlg::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	m_edit_ctrl.SetFont(&m_font,FALSE);	
}

void COrderProbDlg::OnRetry() 
{
	EndDialog(IDRETRY);
	DestroyWindow();
}
