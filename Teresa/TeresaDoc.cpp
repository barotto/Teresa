/*//////////////////////////////////////////////////////////////////////////////
	FILENAME:	TeresaDoc.cpp
	Version:	1.0.0
	Author:		Marco Bortolin
	Created:	27/07/2001

	Copyright (C) 2001 Marco Bortolin
//////////////////////////////////////////////////////////////////////////////*/

#include "stdafx.h"
#include "Teresa.h"

#include "TeresaDoc.h"
#include <algorithm>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
	

/////////////////////////////////////////////////////////////////////////////
// CTeresaDoc

IMPLEMENT_DYNCREATE(CTeresaDoc, CDocument)

BEGIN_MESSAGE_MAP(CTeresaDoc, CDocument)
	//{{AFX_MSG_MAP(CTeresaDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTeresaDoc construction/destruction

CTeresaDoc::CTeresaDoc()
{
	// TODO: add one-time construction code here
	m_itempos = -1;
	m_bRebuildAlgebra = false;
	m_sys = new trsSystem;
	m_badorder = true;
	m_ordstr = "";
	m_descstr = "";
	m_dirty = false;
	m_canundo = false;
}

CTeresaDoc::~CTeresaDoc()
{
	delete m_sys;
}

BOOL CTeresaDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CTeresaDoc serialization

void CTeresaDoc::Serialize(CArchive& ar)
{
	string *str;
	CString reading_str;

	if (ar.IsStoring())
	{
		//@@ non usare GetNextItem()
		ar.WriteString("[TEORIA]\n");
		while((str = GetNextItem())!=NULL) {
			ar.WriteString(str->c_str());
			ar.WriteString("\n");
		}
		if (!m_ordstr.empty()) {
			ar.WriteString("\n[ORDINE]\n");
			ar.WriteString(m_ordstr.c_str());
			ar.WriteString("\n");
		}
		if (!m_sys->GetTrs()->empty()) {
			ar.WriteString("\n[TRS]\n");
			trsTermPairs::iterator it;
			for (it=m_sys->GetTrs()->begin(); it != m_sys->GetTrs()->end(); it++) {
				ar.WriteString(it->GetString().c_str());
				ar.WriteString("\n");
			}
		}
		if (!m_descstr.IsEmpty()) {
			ar.WriteString("\n[DESCRIZIONE]\n");
			//ar << m_descstr;
			ar.WriteString((LPCTSTR)m_descstr);
		}
	}
	else
	{
		//@@ creare un parser degno!
		while(ar.ReadString(reading_str)) {
			if (strcmp((LPCTSTR)reading_str,"[TEORIA]")==0)
				continue;
			if (strcmp((LPCTSTR)reading_str,"[ORDINE]")==0) {
				if(ar.ReadString(reading_str))
					SetOrder((LPCTSTR)reading_str,true);
			} else if (strcmp((LPCTSTR)reading_str,"[TRS]")==0) {
				while(ar.ReadString(reading_str)) {
					if (strcmp((LPCTSTR)reading_str,"[DESCRIZIONE]")==0) {
						ReadDescription(ar);
						return;
					}
					try {
						InsertTrsRule(reading_str);
					} catch(CTeresaDocException E) {
						AfxMessageBox(E.m_mex.c_str(), MB_ICONWARNING|MB_OK);
						return;
					}
				}
			} else if (strcmp((LPCTSTR)reading_str,"[DESCRIZIONE]")==0) {
				//ar >> m_descstr;
				ReadDescription(ar);
				return;
			} else {
				try{
					InsertItem(reading_str);
				} catch(...) {}
			}
		}
	}
}


void CTeresaDoc::ReadDescription(CArchive &_ar)
{
	CString reading_str;

	while (_ar.ReadString(reading_str)) {
		m_descstr += reading_str;
		m_descstr += "\r\n";
	}
}

/////////////////////////////////////////////////////////////////////////////
// CTeresaDoc diagnostics

#ifdef _DEBUG
void CTeresaDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CTeresaDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CTeresaDoc commands


/*- CTeresaDoc::RebuildAlgebra ---------------------------------------


--------------------------------------------------------------------*/
void CTeresaDoc::RebuildAlgebra()
{
	trsRawIds trsbackup;
	trsTermPairs::iterator it;

	// prima di tutto salviamo un eventuale TRS calcolato in precedenza
	// per poi tentare di ripristinarlo.
	for (it=m_sys->GetTrs()->begin(); it!=m_sys->GetTrs()->end(); it++)
		trsbackup.push_back(it->GetString());

	// eliminiamo il system corrotto
	delete m_sys;
	m_sys = new trsSystem;

	// ricostruiamo l'algebra per le sole coppie corrette
	for (int i=0; i < m_userpos.size(); i++) {
		if (m_userpos[i].code == CTDOCERR_GOOD) {
			try {
				m_parser.GetPair((*m_userpos[i].it).c_str(), NULL,
					m_sys->GetAlgebra());
			} catch(...) {}
		}
	}

	// ripristiniamo il vecchio TRS gia` calcolato
	try {
		m_parser.GetPairs(&trsbackup,m_sys->GetTrs(),m_sys->GetAlgebra());
	} catch(...) {
		// qualora ci fossero dei problemi elimina il TRS e dimenticatene
		m_sys->ClearTrs();
	}

	m_bRebuildAlgebra = false;
}


/*- CTeresaDoc::InsertItem -------------------------------------------

	Se _pos >= 0 allora sostituisce l'elemento in posizione _pos con _str
	_pos e` zero-based
--------------------------------------------------------------------*/
bool CTeresaDoc::InsertItem(CString &_str, int _pos) throw(CTeresaDocException)
{
	string s((LPCTSTR)_str);
	return InsertItem(s,_pos);
}
bool CTeresaDoc::InsertItem(string &_str, int _pos) throw(CTeresaDocException)
{
	it_str_code tri;
	trsRawIds::iterator it;

	CleanErasedPos();
	
	// rimuovo tutti gli spazi (non ammessi) dalla stringa inserita dall'utente
	string::iterator its;
	its = _str.begin();
	while (its!=_str.end()) {
		if (*its == ' ') {
			its = _str.erase(its);
			continue;
		}
		its++;
	}
	if (_str.empty())
		return false;

	// se l'algebra e` stata invalidata la devo ricostruire
	if (m_bRebuildAlgebra)
		RebuildAlgebra();

	if ( find(m_IDlist.begin(),m_IDlist.end(),_str) != m_IDlist.end()) {
		// la stringa e` gia` stata inserita (duplicato) percio` esco.
		throw CTeresaDocException(CTDOCERR_DUP,"Duplicato");
	}

	//la inserisco nella lista
	if (_pos<0) {
		it = m_IDlist.end();
	} else {
		it = m_IDlist.begin();
		advance(it,_pos);
	}
	it = m_IDlist.insert(it,_str);
	SetModifiedFlag();

	// la stringa e` nuova quindi provo a trasformarla in un trsTermPair
	try 
	{
		m_parser.GetPair(_str.c_str(), NULL, m_sys->GetAlgebra());
	} 
	catch (trsParserException P) 
	{
		// errore sintattico; mantengo la stringa in memoria, 
		// invalido l'algebra e lancio un errore.
		tri.it = it;
		tri.str = P.GetMsg();
		tri.code = CTDOCERR_SYNTAX;
		if (_pos>=0)
			m_userpos[_pos] = tri;
		else 
			m_userpos.insert(m_userpos.end(),tri);

		// bisogna ricostruire l'algebra poiche` durante il parsing
		// potrebbero essere state inserite delle funzioni nella segnatura
		// (o variabili o costanti) appartenenti ad una prima parte 
		// corretta della coppia.
		m_bRebuildAlgebra = true;
		throw CTeresaDocException(CTDOCERR_SYNTAX,P.GetMsg());
	} 
	catch (trsSAException SA) 
	{
		// errore sull'arieta` delle funzioni in essa contenute;
		// mantengo la stringa in memoria, invalido l'algebra 
		// e lancio un errore.
		tri.it = it;
		tri.str = SA.GetMsg();
		tri.code = CTDOCERR_ARITY;
		if (_pos>=0)
			m_userpos[_pos] = tri;
		else 
			m_userpos.insert(m_userpos.end(),tri);

		m_bRebuildAlgebra = true;
		throw CTeresaDocException(CTDOCERR_ARITY,SA.GetMsg());
	}

#ifndef NDEBUG
	// eccezioni lanciate dalle Assert()
	catch (trsException E) {
		tri.it = it;
		tri.str = E.GetMsg();
		tri.code = CTDOCERR_UNK;
		if (_pos>=0)
			m_userpos[_pos] = tri;
		else 
			m_userpos.insert(m_userpos.end(),tri);

		m_bRebuildAlgebra = true;
		throw CTeresaDocException(CTDOCERR_UNK,E.GetMsg());	
	}
#endif

	// la coppia inserita e` corretta	
	tri.it = it;
	tri.str = *it;
	tri.code = CTDOCERR_GOOD;
	if (_pos>=0)
		m_userpos[_pos] = tri;
	else 
		m_userpos.insert(m_userpos.end(),tri);

	// la teoria e` stata modificata quindi il vecchio ordine 
	// potrebbe non essere piu` valido
	//if (!m_ordstr.empty())
	SetOrder(m_ordstr.c_str(), true);

	// una nuova coppia di termini rende l'eventuale TRS calcolato 
	// non piu` valido
	m_sys->ClearTrs();

	return true;
}


unsigned int CTeresaDoc::GetNumItems() const
{
	return m_IDlist.size();
}


void CTeresaDoc::RemoveItem(unsigned int _itempos)
{
	if (m_IDlist.empty()) return;
	if (_itempos >= m_userpos.size()) return;
	
	if (m_userpos[_itempos].code == CTDOCERR_ERASED)
		return;

	// se la coppia eliminata era corretta invalido l'algebra
	if (m_userpos[_itempos].code == CTDOCERR_GOOD) {
		// l'eliminazione di una coppia di termini 
		// rende l'eventuale TRS calcolato non piu` valido
		m_sys->ClearTrs();
		m_bRebuildAlgebra = true;
	}

	// tolgo la stringa dalla lista di stringhe
	m_IDlist.erase(m_userpos[_itempos].it);
	m_userpos[_itempos].code = CTDOCERR_ERASED;
	
	// la teoria e` stata modificata quindi il vecchio ordine 
	// potrebbe non essere piu` valido
	//if (!m_ordstr.empty())
	SetOrder(m_ordstr.c_str(), true);

	m_dirty = true;

	SetModifiedFlag();
}


void CTeresaDoc::RemoveAllItems()
{
	int size = GetNumItems();
	for (int i=0; i < size; i++)
		RemoveItem(i);
}

//@@ funzione pericolosa, modificare in modo che accetti 
// una posizione in ingresso
string *CTeresaDoc::GetNextItem()
{	
	if (m_IDlist.empty()) return NULL;

	CleanErasedPos();

	m_itempos += 1;
	if (m_itempos >= GetNumItems()) {
		m_itempos = -1;
		return NULL;
	}
	
	return &*(m_userpos[m_itempos].it);
}


string* CTeresaDoc::GetItem(unsigned int _itempos)
{
	if (m_IDlist.empty()) return NULL;
	if (_itempos==-1 && m_itempos>=0) _itempos = m_itempos;
	if (_itempos<0) return NULL;
	if (_itempos >= GetNumItems()) return 0;

	CleanErasedPos();

	return &*(m_userpos[_itempos].it);
}


string* CTeresaDoc::GetItemDesc(unsigned int _itempos)
{	
	if (m_IDlist.empty()) return NULL;
	if (_itempos==-1 && m_itempos>=0) _itempos = m_itempos;
	if (_itempos<0) return NULL;
	if (_itempos >= GetNumItems()) return NULL;
	
	CleanErasedPos();

	return &(m_userpos[_itempos].str);
}


int CTeresaDoc::GetItemCond(unsigned int _itempos)
{	
	if (m_IDlist.empty()) return CTDOCERR_UNK;
	if (_itempos==-1 && m_itempos>=0) _itempos = m_itempos;
	if (_itempos<0) return CTDOCERR_UNK;
	if (_itempos >= GetNumItems()) return CTDOCERR_UNK;

	CleanErasedPos();

	return m_userpos[_itempos].code;
}

//@@ funzione pericolosa, modificare in modo che accetti 
// una posizione in ingresso
string* CTeresaDoc::GetFirstItem()
{
	if (m_IDlist.empty()) return NULL;
	
	CleanErasedPos();

	m_itempos = 0;
	return &*(m_userpos[m_itempos].it);
}


bool CTeresaDoc::ModifyItem(unsigned int _itempos, CString &_str) throw(CTeresaDocException)
{
	trsRawIds::iterator it;
	string oldvalue;

	if (m_IDlist.empty()) return false;
	if (_itempos >= GetNumItems()) return false;

	CleanErasedPos();

	// rimuovo tutti gli spazi (non ammessi) dalla stringa inserita dall'utente
	_str.Remove(' ');

	// memorizzo la stringa da eliminare per eventualmente reinserirla e la tolgo
	// dalla lista.
	oldvalue = *m_userpos[_itempos].it;
	m_IDlist.erase(m_userpos[_itempos].it);
	
	// cancello il TRS eventualmente calcolato
	m_sys->ClearTrs();

	// invalido l'algebra
	m_bRebuildAlgebra = true;

	// provo ad inserire la nuova stringa 
	try {
		InsertItem(_str, _itempos);
	} catch(CTeresaDocException E) {
		if (E.m_code == CTDOCERR_DUP) {
			// reinserisco il vecchio valore
			it = m_IDlist.begin();
			advance(it,_itempos);
			m_userpos[_itempos].it = m_IDlist.insert(it,oldvalue);
		}
		// se l'errore e` di arieta` o sintassi la stringa e` gia` stata inserita.
		// lancio l'errore al chiamante.
		throw E;
	}
	return true;
}


void CTeresaDoc::Refresh()
{
	if(m_bRebuildAlgebra)
		RebuildAlgebra();
	for (int i=0; i<m_userpos.size(); i++) {
		if (m_userpos[i].code == CTDOCERR_ARITY) {
			try {
				m_parser.GetPair((*m_userpos[i].it).c_str(),NULL,m_sys->GetAlgebra());
				m_userpos[i].code = CTDOCERR_GOOD;
				m_userpos[i].str = *m_userpos[i].it;
			} catch(...) {}
		}
	}
}


trsSimArityPairs* CTeresaDoc::GetSignature()
{
	if(m_bRebuildAlgebra)
		RebuildAlgebra();
	return &m_sys->GetAlgebra()->m_signature;
}

trsVarList* CTeresaDoc::GetVarList()
{
	if(m_bRebuildAlgebra)
		RebuildAlgebra();
	return &m_sys->GetAlgebra()->m_variables;
}

trsTermPairs* CTeresaDoc::GetTheory() throw(CTeresaDocException)
{
	m_sys->ClearTheory();
	try {
		m_parser.GetPairs(&m_IDlist,m_sys->GetTheory(),GetAlgebra());
	} catch (trsException E) {
		m_bRebuildAlgebra = true;
		throw CTeresaDocException(CTDOCERR_UNK, E.GetMsg().c_str());
	}
	return m_sys->GetTheory();
}


trsSystem* CTeresaDoc::GetSys() throw(CTeresaDocException)
{
	GetTheory();
	return m_sys;
}


trsAlgebra* CTeresaDoc::GetAlgebra() throw(CTeresaDocException)
{
	if(m_bRebuildAlgebra)
		RebuildAlgebra();

	return m_sys->GetAlgebra();
}


void CTeresaDoc::SetOrder(const char *_ordstr, bool _force) throw(CTeresaDocException)
{
	try {
		m_LPO.SetNewOrder(_ordstr,GetSignature());
		m_badorder = false;
		m_ordstr = m_LPO.GetOrdStr();
		m_ordprob = "";
	} catch(trsOrderException E) {
		if (_force) {
			m_ordstr = _ordstr;
			m_ordprob = E.GetMsg();
			m_badorder = true;
		} else {
			throw CTeresaDocException(CTDOCERR_BAD_ORDER,E.GetMsg());
		}
	}
	SetModifiedFlag();
}

/*
void CTeresaDoc::GetVarList(list<string>* _outlist)
{
	if (_outlist==NULL) return;

	trsVarList::iterator it;
	it = m_sys->GetAlgebra()->m_variables.begin();
	while (it != m_sys->GetAlgebra()->m_variables.end()) {
		_outlist->push_back(*((*it)->GetSim()));
		it++;
	}
}
*/

bool CTeresaDoc::SwapTerms(int _itempos)
{
	Assert( ((_itempos>=0)&&(_itempos < GetNumItems())),"out of boundaries" );
		
	CleanErasedPos();

	string spair, sright;

	if (m_userpos[_itempos].code != CTDOCERR_GOOD)
		return false;

	spair = *(m_userpos[_itempos].it);
	int pos = spair.find("=");
	sright = spair.substr(pos+1);
	spair.erase(pos);
	sright += "=";
	sright += spair;
	*(m_userpos[_itempos].it) = sright;
	m_userpos[_itempos].str = sright;
	SetModifiedFlag();
	return true;
}


void CTeresaDoc::CleanErasedPos()
{
	vector<it_str_code>::iterator it;

	if (!m_dirty) return;

	it = m_userpos.begin();
	while(it != m_userpos.end()) {
		if (it->code == CTDOCERR_ERASED) {
			it = m_userpos.erase(it);
			continue;
		}
		it++;
	}
	m_dirty = false;
}


void CTeresaDoc::Backup()
{
	trsTermPairs::iterator it;
	
	m_IDbackup.clear();
	m_Trsbackup.clear();
	m_ordbackup = m_ordstr;
	m_IDbackup = m_IDlist;
	for (it=m_sys->GetTrs()->begin(); it!=m_sys->GetTrs()->end(); it++)
		m_Trsbackup.push_back(it->GetString());
	m_canundo = true;
}

void CTeresaDoc::Undo()
{
	trsRawIds::iterator it;

	m_sys->ClearTheory();
	m_sys->ClearTrs();
	RemoveAllItems();
	m_itempos = -1;

	m_bRebuildAlgebra = false;
	for (it=m_IDbackup.begin(); it != m_IDbackup.end(); it++) {
		try {
			InsertItem(*it);
		} catch(...){}
	}
	for (it=m_Trsbackup.begin(); it != m_Trsbackup.end(); it++) {
		try {
			InsertTrsRule(CString(it->c_str()));
		} catch(...){}
	}

	m_IDbackup.clear();
	m_Trsbackup.clear();
	SetOrder(m_ordbackup.c_str(),true);
	m_canundo = false;
}


void CTeresaDoc::InsertTrsRule(CString &_str)
{
	// rimuovo tutti gli spazi (non ammessi) dalla stringa inserita dall'utente
	_str.Remove(' ');
	if (_str.IsEmpty())
		return;

	// se l'algebra e` stata invalidata la devo ricostruire
	if (m_bRebuildAlgebra) {
		RebuildAlgebra();
		m_bRebuildAlgebra = false;
	}
	try {
		trsTermPair tp;
		m_parser.GetPair((LPCTSTR)_str, &tp, m_sys->GetAlgebra());
		m_sys->GetTrs()->push_back(tp);
		tp.AutoDispose(false);
	} catch (trsException E) {
		m_sys->ClearTrs();
		throw CTeresaDocException(CTDOCERR_UNK,
			"Impossibile inserire le regole nel TRS.\n");
	}
	SetModifiedFlag();
}


bool CTeresaDoc::WordProblem(CString &_incoppia, CString &_outnorm)
{
	trsTerm *t;

	if (m_sys->GetTrs()->empty())
		return false;

	_incoppia.Remove(' ');
	if (_incoppia.IsEmpty())
		throw CTeresaDocException(CTDOCERR_UNK,"La stringa in ingresso e` vuota!");
		
	trsTermPair tp;
	try {
		trsRewrite rew;
		m_parser.GetPair((LPCTSTR)_incoppia, &tp, m_sys->GetAlgebra());
		t = tp.GetLeft();
		tp.SetLeft(rew.Normalize(tp.GetLeft(),m_sys->GetTrs()));
		if (t != tp.GetLeft()) delete t;
		t = tp.GetRight();
		tp.SetRight(rew.Normalize(tp.GetRight(),m_sys->GetTrs()));
		if (t != tp.GetRight()) delete t;
	} catch (trsException E) {
		string mex("\nImpossibile eseguire:\n");
		mex += E.GetMsg();
		throw CTeresaDocException(CTDOCERR_UNK,mex);
	}
	_outnorm = tp.GetPrintable().c_str();
	return (*tp.GetLeft() == *tp.GetRight());
}


void CTeresaDoc::ClearTrs()
{
	m_sys->ClearTrs();
	RebuildAlgebra();
}
