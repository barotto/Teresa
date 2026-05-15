/*//////////////////////////////////////////////////////////////////////////////
	FILENAME:	DescDlg.cpp
	Version:	1.0.0
	Author:		Marco Bortolin
	Created:	25/09/2001 - 12.23.49
	
	Copyright (C) 2001 Marco Bortolin
//////////////////////////////////////////////////////////////////////////////*/

#include "stdafx.h"
#include "teresa.h"
#include "DescDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDescDlg dialog


CDescDlg::CDescDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDescDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDescDlg)
	m_edit = _T("");
	//}}AFX_DATA_INIT
}


void CDescDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDescDlg)
	DDX_Control(pDX, IDC_DESC_EDIT, m_edit_ctrl);
	DDX_Text(pDX, IDC_DESC_EDIT, m_edit);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDescDlg, CDialog)
	//{{AFX_MSG_MAP(CDescDlg)
	ON_WM_SHOWWINDOW()
	ON_WM_SIZE()
	ON_COMMAND(ID_FORMAT_FONT, OnFormatFont)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDescDlg message handlers

void CDescDlg::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);

	LOGFONT lf;
	RECT r;	
	CTeresaApp* pApp = (CTeresaApp*)AfxGetApp();

	pApp->GetFont(&lf, IDC_DESC_EDIT);
	m_font.CreateFontIndirect(&lf);
	GetClientRect(&r);
	m_edit_ctrl.SetSel(-1,-1,TRUE);
	m_edit_ctrl.MoveWindow(0,0,r.right,r.bottom);
	m_edit_ctrl.SetFont(&m_font);
}

void CDescDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	RECT r;
	GetClientRect(&r);
	if (m_edit_ctrl)
		m_edit_ctrl.MoveWindow(0,0,r.right,r.bottom);
}

void CDescDlg::OnFormatFont() 
{
   CFont* pFont = m_edit_ctrl.GetFont();
   CTeresaApp* pApp = (CTeresaApp*)AfxGetApp();

   LOGFONT lf;
   if (pFont != NULL)
	   pFont->GetObject(sizeof(LOGFONT), &lf);
   else
	   ::GetObject(GetStockObject(SYSTEM_FONT), sizeof(LOGFONT), &lf);

	CFontDialog fdlg(&lf, CF_SCREENFONTS|CF_INITTOLOGFONTSTRUCT);
	if (fdlg.DoModal() == IDOK) {
		m_font.DeleteObject();
		if (m_font.CreateFontIndirect(&lf))
			m_edit_ctrl.SetFont(&m_font);
		pApp->StoreFont(&lf, IDC_DESC_EDIT);
	}	
}
