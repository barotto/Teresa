/*//////////////////////////////////////////////////////////////////////////////
	FILENAME:	HuetDlg.cpp
	Version:	1.0.0
	Author:		Marco Bortolin
	Created:	27/08/2001 - 12.29.42

	Copyright (C) 2001 Marco Bortolin
//////////////////////////////////////////////////////////////////////////////*/

#include "stdafx.h"
#include "teresa.h"
#include "HuetDlg.h"
#include <trs.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CHuetDlg dialog


CHuetDlg::CHuetDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CHuetDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CHuetDlg)
	//}}AFX_DATA_INIT
	m_iavvio = AfxGetApp()->LoadIcon(IDI_START);
	m_istop = AfxGetApp()->LoadIcon(IDI_STOP);
	m_fplog = NULL;
	m_mex = "Non eseguito.\n";
}


void CHuetDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CHuetDlg)
	DDX_Control(pDX, IDC_FUNCNAME, m_funcname);
	DDX_Control(pDX, IDC_EDIT_LOOPNUM, m_loopnum_ctrl);
	DDX_Control(pDX, IDC_CHECKLOG, m_checklog);
	DDX_Control(pDX, ID_HUETDLG_LOG, m_btnlog);
	DDX_Control(pDX, IDC_EDIT_TRSRULES, m_trsrules_ctrl);
	DDX_Control(pDX, IDCANCEL, m_btnexit);
	DDX_Control(pDX, IDC_EDIT_CURLOOP, m_curloop_ctrl);
	DDX_Control(pDX, IDC_PROGRESS, m_progress);
	DDX_Control(pDX, IDC_STOP, m_btnstop);
	DDX_Control(pDX, IDC_AVVIO, m_btnavvio);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CHuetDlg, CDialog)
	//{{AFX_MSG_MAP(CHuetDlg)
	ON_BN_CLICKED(IDC_AVVIO, OnAvvio)
	ON_BN_CLICKED(IDC_STOP, OnStop)
	ON_BN_CLICKED(ID_HUETDLG_LOG, OnHuetdlgLog)
	ON_BN_CLICKED(IDC_CHECKLOG, OnChecklog)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHuetDlg message handlers

void CHuetDlg::OnAvvio() 
{
	trsHuet huet;
	CString loopnum,rules;

	if (m_fplog != NULL)
		fclose(m_fplog);
	m_fplog = NULL;

	if (m_checklog.GetCheck()==1)
		m_fplog = fopen("huetlog.wri","wb");

	if (m_fplog != NULL)
		huet.SetDevices(this,this);
	else
		huet.SetDevices(NULL,this);

	m_bstop = false;
	m_blooplimit = false;
	m_ilimit = 0;

	m_loopnum_ctrl.GetWindowText(loopnum);
	sscanf((LPCTSTR)loopnum,"%d",&m_ilimit);

	m_btnlog.EnableWindow(FALSE);
	m_btnexit.EnableWindow(FALSE);
	m_btnavvio.EnableWindow(FALSE);
	m_btnstop.EnableWindow();
	try {
		m_system->ClearTrs();
		huet.Completion(*m_system,m_order);
		m_trsrules_ctrl.GetWindowText(rules);
		if (m_blooplimit) {
			m_mex = "Completamento interrotto prima del termine per raggiungimento del limite imposto.\n\
Regole nel TRS temporaneo: ";
			m_mex += rules;
			MessageBox((LPCTSTR)m_mex,NULL,MB_ICONINFORMATION|MB_OK);
		} else if (m_bstop) {
			m_mex = "Completamento interrotto prima del termine.\nRegole nel TRS temporaneo: ";
			m_mex += rules;
		} else {
			m_mex = "Completamento terminato con successo.\nRegole nel TRS: ";
			m_mex += rules;
			MessageBox((LPCTSTR)m_mex,NULL,MB_ICONINFORMATION|MB_OK);
			//SendMessage(WM_COMMAND,IDCANCEL);
		}
	} catch(trsHuetException E) {
		m_trsrules_ctrl.GetWindowText(rules);
		m_mex = "Completamento fallito :\n";
		m_mex += E.GetMsg().c_str();
		m_mex += "\nRegole nel TRS temporaneo: ";
		m_mex += rules;
		if (E.GetCode() != TRSERR_USERSTOP)
			MessageBox((LPCTSTR)m_mex,NULL,MB_ICONERROR|MB_OK);
	}

	m_btnlog.EnableWindow(m_checklog.GetCheck()==1);
	m_btnexit.EnableWindow();
	m_btnavvio.EnableWindow();
	m_btnstop.EnableWindow(FALSE);

}

void CHuetDlg::OnStop() 
{
	m_bstop = true;	
	m_btnlog.EnableWindow(m_checklog.GetCheck()==1);
	m_btnexit.EnableWindow();
	m_btnavvio.EnableWindow();
	m_btnstop.EnableWindow(FALSE);
}

BOOL CHuetDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
		
	m_funcdots.Create(MAKEINTRESOURCE(IDB_HUETFUNC),9,1,0x00FF00FF);

	m_funcname.SetIcon(m_funcdots.ExtractIcon(5));

	m_btnavvio.SetIcon(m_iavvio);
	m_btnstop.SetIcon(m_istop);
	
	m_checklog.SetCheck(0);
	m_btnlog.EnableWindow(FALSE);
	m_btnexit.EnableWindow();
	m_btnavvio.EnableWindow();
	m_btnstop.EnableWindow(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


bool CHuetDlg::Stop()
{
	MSG msg;

	while(PeekMessage(&msg,this->m_hWnd,0,0,PM_REMOVE)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return (m_bstop || m_blooplimit);
}

void CHuetDlg::SetProgressRange(int _lower, int _upper)
{
	m_progress.SetRange32(_lower,_upper);
}

void CHuetDlg::SetProgressStep(int _step)
{
	m_progress.SetStep(_step);
}

void CHuetDlg::SetProgressPos(int _pos)
{
	m_progress.SetPos(_pos);
}
	
void CHuetDlg::StepProgress()
{
	m_progress.StepIt();
	m_progress.UpdateWindow();
}

void CHuetDlg::Reset()
{
	m_progress.SetPos(0);
	m_curloop_ctrl.SetWindowText("0");
}

void CHuetDlg::PrintInt(int _vec[], int _size)
{
	CString s;

	s.Format("%d", _vec[0]);
	m_curloop_ctrl.SetWindowText(s);
	if ((!m_blooplimit) && (m_ilimit>0))
		m_blooplimit = (_vec[0] == m_ilimit);
	
	s.Format("%d",_vec[1]);
	m_trsrules_ctrl.SetWindowText(s);
}

void CHuetDlg::PrintFuncName(const char* _str)
{
	HICON oldicon;
	switch (_str[0]) {
	case 'S': // Simplify
		oldicon = m_funcname.SetIcon(m_funcdots.ExtractIcon(0));
		DestroyIcon(oldicon);
		break;
	case 'O' : //Orient
		oldicon = m_funcname.SetIcon(m_funcdots.ExtractIcon(2));
		DestroyIcon(oldicon);
		break;
	case 'L': // L-R Simplify
		oldicon = m_funcname.SetIcon(m_funcdots.ExtractIcon(3));
		DestroyIcon(oldicon);
		break;
	case 'D' :
		if (_str[2]=='l') //Delete
			oldicon = m_funcname.SetIcon(m_funcdots.ExtractIcon(1));
		else // Deduce
			oldicon = m_funcname.SetIcon(m_funcdots.ExtractIcon(4));	
		DestroyIcon(oldicon);
		break;
	}
	UpdateWindow();
}

void CHuetDlg::PrintStr(const char* _str)
{
}

trsOutputDev &CHuetDlg::operator <<(string _src)
{
	fprintf(m_fplog,_src.c_str());
	return *this;
}

trsOutputDev &CHuetDlg::operator <<(const char* _src)
{
	fprintf(m_fplog,_src);
	return *this;
}

trsOutputDev &CHuetDlg::operator <<(int _src)
{	
	fprintf(m_fplog,"%d",_src);
	return *this;
}

void CHuetDlg::OnCancel() 
{
	if (m_fplog != NULL)
		fclose(m_fplog);	

	CDialog::OnCancel();
}

void CHuetDlg::OnHuetdlgLog() 
{
	if (m_fplog != NULL) {
		fclose(m_fplog);
		m_fplog = NULL;
	}

	ShellExecute(AfxGetApp()->m_pMainWnd->m_hWnd,"open","huetlog.wri",NULL,NULL,SW_SHOWNORMAL);
}

void CHuetDlg::OnChecklog() 
{
	m_btnlog.EnableWindow(m_checklog.GetCheck()==1);
}
