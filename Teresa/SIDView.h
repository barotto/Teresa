/*//////////////////////////////////////////////////////////////////////////////
	FILENAME:	SIDView.h
	Version:	1.0.0
	Author:		Marco Bortolin
	Created:	27/07/2001 - 23.41.40

	Copyright (c) 2001 Marco Bortolin
//////////////////////////////////////////////////////////////////////////////*/

#if !defined(AFX_SIDVIEW_H__BADFE6E7_8124_410D_9D88_D93EB655FD7C__INCLUDED_)
#define AFX_SIDVIEW_H__BADFE6E7_8124_410D_9D88_D93EB655FD7C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <trs.h>
#include "ChildFrm.h"

class CSIDView : public CListView
{
protected: // create from serialization only
	CSIDView();
	DECLARE_DYNCREATE(CSIDView)

// Attributes
public:

protected:
	bool m_inputmode;
	bool m_modifymode;
	bool m_wordpmode;
	string m_itembuffer;
	unsigned int m_moditem;

	CImageList	m_imgList;
	CFont		m_font;

private:
	CTeresaApp* GetApp();
	void ShowOrderDlg(string _ordstr);

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSIDView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual void OnInitialUpdate(); // called first time after construct
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	//}}AFX_VIRTUAL

// Implementation
public:
	CChildFrame* GetChildFrame();
	void WordProblem(CString &_incoppia);
	CTeresaDoc* GetDocument();

	bool IsWaitingInput();
	void SetInputMode();
	void DisableInputMode();
	void DisableModifyMode();
	void SetModifyMode(unsigned int _nitem);
	unsigned int GetModItem();
	bool IsModifing();
	bool IsWordPMode() { return m_wordpmode; }
	void SetWordPMode();
	void DisableWordPMode();
	void Sincronizza();
	void PrintNumPairs();
	void TestorderVerbose(trsTermPairs *_teoria, trsOrder *_ordine);

	virtual ~CSIDView();

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	afx_msg void OnContextMenu(CWnd*, CPoint point);
	//{{AFX_MSG(CSIDView)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnClick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclk(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSidviewElimina();
	afx_msg void OnSidviewModifica();
	afx_msg void OnUpdateSidviewElimina(CCmdUI* pCmdUI);
	afx_msg void OnUpdateSidviewModifica(CCmdUI* pCmdUI);
	afx_msg void OnSidviewOrder();
	afx_msg void OnSidviewRefresh();
	afx_msg void OnSidviewTestorder();
	afx_msg void OnUpdateSidviewTestorder(CCmdUI* pCmdUI);
	afx_msg void OnSidviewUnificazione();
	afx_msg void OnSidviewSwapterms();
	afx_msg void OnUpdateSidviewSwapterms(CCmdUI* pCmdUI);
	afx_msg void OnSidviewCoppiecritiche();
	afx_msg void OnSidviewProperties();
	afx_msg void OnSidviewHuet();
	afx_msg void OnUpdateSidviewHuet(CCmdUI* pCmdUI);
	afx_msg void OnSidviewDeleteall();
	afx_msg void OnUpdateSidviewDeleteall(CCmdUI* pCmdUI);
	afx_msg void OnUpdateSidviewCoppiecritiche(CCmdUI* pCmdUI);
	afx_msg void OnUpdateSidviewUnificazione(CCmdUI* pCmdUI);
	afx_msg void OnWordproblem();
	afx_msg void OnUpdateWordproblem(CCmdUI* pCmdUI);
	afx_msg void OnEditCopy();
	afx_msg void OnUpdateEditCopy(CCmdUI* pCmdUI);
	afx_msg void OnEditCut();
	afx_msg void OnUpdateEditCut(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditPaste(CCmdUI* pCmdUI);
	afx_msg void OnEditPaste();
	afx_msg void OnUpdateSidviewProperties(CCmdUI* pCmdUI);
	afx_msg void OnUpdateSidviewOrder(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFilePrint(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFilePrintPreview(CCmdUI* pCmdUI);
	afx_msg void OnTrsviewDeleteall();
	afx_msg void OnUpdateTrsviewDeleteall(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in SIDView.cpp
inline CTeresaDoc* CSIDView::GetDocument()
   { return (CTeresaDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SIDVIEW_H__BADFE6E7_8124_410D_9D88_D93EB655FD7C__INCLUDED_)
