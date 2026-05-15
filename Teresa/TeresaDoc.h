/*//////////////////////////////////////////////////////////////////////////////
	FILENAME:	TeresaDoc.h
	Version:	1.0.0
	Author:		Marco Bortolin
	Created:	27/07/2001 - 23.41.40

	Copyright (c) 2001 Marco Bortolin
//////////////////////////////////////////////////////////////////////////////*/

#if !defined(AFX_HUETDOC_H__13836D54_B1BF_45B3_8E49_4988FCF801C6__INCLUDED_)
#define AFX_HUETDOC_H__13836D54_B1BF_45B3_8E49_4988FCF801C6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <vector>
#include <string>
#include <utility>
#include <trs.h>

class CTeresaDocException
{
public:
	CTeresaDocException(int _code, string _mex)
	{
		m_code = _code;
		m_mex = _mex;
	}
	int    m_code;
	string m_mex;
};

enum CTDOCERR {
	CTDOCERR_GOOD,
	CTDOCERR_ARITY,
	CTDOCERR_DUP,
	CTDOCERR_SYNTAX,
	CTDOCERR_BAD_ORDER,
	CTDOCERR_ERASED,
	CTDOCERR_EMPTY,
	CTDOCERR_UNK
};

class CTeresaDoc : public CDocument
{
protected: // create from serialization only
	CTeresaDoc();
	DECLARE_DYNCREATE(CTeresaDoc)

// Attributes
public:
private:
protected:
	typedef struct {
		trsRawIds::iterator it;   // iteratore che 'punta' ad una stringa della teoria
		string              str;  // stringa che contiene una descrizione della coppia
		int                 code; // codice che rappresenta la 'condizione' della coppia 
		                            //    (corretta, errore di sintassi, errore sull'arieta`).
	} it_str_code;

	trsRawIds           m_IDlist;  // la teoria in stringhe
	trsRawIds			m_IDbackup; // per l'undo
	trsRawIds			m_Trsbackup;
	string				m_ordbackup;
	vector<it_str_code> m_userpos; // vettore che rispecchia l'ordine di inserimento dell'utente

	//trsSimArityPairs  m_segnatura;
	trsSystem           *m_sys;
	
	trsRawIds::iterator m_it;
	int                 m_itempos;

	CString      m_descstr;
	trsLPOOrder  m_LPO;
	string       m_ordstr;
	string       m_ordprob;
	bool         m_badorder;
	
	bool m_bRebuildAlgebra;	
	// true = l'algebra deve essere ricostruita
	// messo a true in Modify,Remove,Insert

	trsParser m_parser;
	
	void RebuildAlgebra();
	int GetItemPos(int _pos);
	void CleanErasedPos();
	bool m_dirty;

	bool m_canundo;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTeresaDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	unsigned int GetNumItems() const;
	string*      GetItem(unsigned int _itempos=-1);
	string*      GetItemDesc(unsigned int _itempos=-1);
	int          GetItemCond(unsigned int _itempos=-1);
	string*      GetFirstItem();
	string*      GetNextItem();
	string*      GetOrderStr() { return &m_ordstr; }
	string*      GetOrderProb() { return &m_ordprob; }
	bool         IsOrderValid() { return !m_badorder; }
	//void         GetVarList(list<string>* _outlist);
	trsSystem*        GetSys();
	trsVarList*       GetVarList();
	trsSimArityPairs* GetSignature();
	trsTermPairs*     GetTheory() throw(CTeresaDocException);
	trsAlgebra*       GetAlgebra() throw(CTeresaDocException);
	trsTermPairs*     GetTrs() { return m_sys->GetTrs(); }
	trsOrder*    GetOrder() { return &m_LPO; }
	CString*     GetDescription() { return &m_descstr; }
	void         SetOrder(const char *_ordstr, bool _force=false) throw(CTeresaDocException);
	bool         InsertItem(CString &_str, int _pos=-1) throw(CTeresaDocException);
	bool         InsertItem(string &_str, int _pos=-1) throw(CTeresaDocException);
	void         RemoveItem(unsigned int _itempos);
	void         RemoveAllItems();
	bool         ModifyItem(unsigned int _itempos, CString &_str) throw(CTeresaDocException);
	bool         SwapTerms(int _itempos);
	void ClearTrs();
	void Refresh();
	void Backup();
	bool CanUndo() { return m_canundo; }
	void Undo();

	void InsertTrsRule(CString &_str);
	
	bool WordProblem(CString &_incoppia, CString &_outnorm);

	virtual ~CTeresaDoc();

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	void ReadDescription(CArchive &_ar);
	//{{AFX_MSG(CTeresaDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HUETDOC_H__13836D54_B1BF_45B3_8E49_4988FCF801C6__INCLUDED_)
