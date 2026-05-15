/*//////////////////////////////////////////////////////////////////////////////
	FILENAME: trstermpair.cpp
	Version:  1.0.0
	Author:   Marco Bortolin, Alberto Minora
	Created:  04/08/2001 - 15.06.24

	Copyright (C) 2001 Marco Bortolin, Alberto Minora
//////////////////////////////////////////////////////////////////////////////*/
#ifdef WIN32
#pragma warning(disable:4786)
#pragma warning(disable:4100)
#endif
#include "trs.h"



/*- operator << ------------------------------------------------------

	Stampa sul device _dev la coppia _pair
--------------------------------------------------------------------*/
trsOutputDev &operator<<(trsOutputDev &_dev, trsTermPair &_pair)
{
	if (_pair.m_left==NULL)
		_dev << "<null>";
	else
		_dev << _pair.m_left->GetPrintable();
	
	_dev << "=";

	if (_pair.m_right==NULL)
		_dev << "<null>";
	else
		_dev << _pair.m_right->GetPrintable();

	return _dev;
}


/*- operator << ------------------------------------------------------

	Stampa su un trsOutputDev un trsTermPairs
--------------------------------------------------------------------*/
trsOutputDev &operator<<(trsOutputDev &_dev, trsTermPairs &_pairs)
{
	trsTermPairs::iterator it;
	for (it = _pairs.begin(); it != _pairs.end(); it++)
		_dev << *it << "\n";
	
	return _dev;
}


/*- trsTermPair::GetPrintable ----------------------------------------

	Restituisce una stringa stampabile della coppia.
	Se _rule = true allora e` nel formato l-->r altrimenti l=r
--------------------------------------------------------------------*/
string trsTermPair::GetPrintable(bool _rule)
{
	string s;

	if (m_left==NULL)
		s += "<null>";
	else
		s += m_left->GetPrintable();
	
	if (_rule)
		s += "-->";
	else
		s += "=";

	if (m_right==NULL)
		s += "<null>";
	else
		s += m_right->GetPrintable();

	return s;
}


/*- trsTermPair::GetString -------------------------------------------

	Restituisce una stringa l=r che, passata al parser, mi consente di 
	riottenere la medesima coppia. Utilizzo tipico: serializzazione.
--------------------------------------------------------------------*/
string trsTermPair::GetString()
{
	// i termini non possono essere NULL (solo DEBUG)
	Assert((m_left&&m_right),trsException(TRSERR_NULLPTR,
		"trsTermPair::GetString() : NULL terms"));

	string s;

	s = *m_left->GetString();
	s += "=";
	s += *m_right->GetString();

	return s;
}


/*- trsTermPair::trsTermPair ----------------------------------------

	Costruttore
--------------------------------------------------------------------*/
trsTermPair::trsTermPair(trsTerm* _inleft, trsTerm* _inright, 
	bool _auto)
{
	m_autodispose = _auto;
	m_left = _inleft;
	m_right = _inright;
}

/*- operator== -------------------------------------------------------

//@@ codice delle 1.30 am, rivedere!
--------------------------------------------------------------------*/
bool trsTermPair::operator ==(trsTermPair& _tp)
{
	bool cmp1=false,cmp2=false;

	if (m_left==NULL && m_right==NULL && _tp.m_left==NULL && _tp.m_right==NULL) return true;
	if (m_left==NULL && _tp.m_right==NULL) return false;
	if (m_right==NULL && _tp.m_left==NULL) return false;
	
	if (!(m_left!=NULL && _tp.m_left!=NULL))
		cmp1 = (*m_left == *_tp.m_left);	
	
	if (!(m_right!=NULL && _tp.m_right!=NULL))	
		cmp2 = (*m_right == *_tp.m_right);

	return (cmp1 && cmp2);
}


/*- trsTermPair::~trsTermPair ----------------------------------------

	Distruttore
--------------------------------------------------------------------*/
trsTermPair::~trsTermPair()
{
	// se sono costanti o variabili non devono essere distrutte, 
	// ci pensa l'algebra
	if (m_autodispose) {
		if (m_left != NULL)
			if (m_left->IsFunc())
				delete m_left;
		if (m_right != NULL)
			if (m_right->IsFunc())
				delete m_right;
	}
}


/*- trsTermPair::Swap ------------------------------------------------

	Inverte il termine di destra con quello di sinistra e viceversa
--------------------------------------------------------------------*/
void trsTermPair::Swap()
{
	trsTerm* temp = m_left;
	m_left = m_right;
	m_right = temp;
}


/*- trsTermPair::Rename ----------------------------------------------


--------------------------------------------------------------------*/
void trsTermPair::Rename()
{
	trsVarList vlist;
	trsVarList::iterator vit;
	
	Assert(m_left&&m_right,trsException(TRSERR_NULLPTR,"NULL terms in pair"));

	// ottengo una lista delle variabili presenti nei due termini
	GetVars(vlist);

	if (vlist.empty()) return;

	// creo una sostituzione var->varenamed per ogni variabile in lista
	trsSubst subst;
	for (vit=vlist.begin(); vit != vlist.end(); vit++)
		subst.AddSubPair(*vit,(*vit)->GetNewRenamed());
	
	// applico la sostituzione ai termini
	m_left = subst.TermApplySub(m_left);
	m_right = subst.TermApplySub(m_right);
}


/*- trsTermPair::GetVars ---------------------------------------------

	restituisce la lista delle variabili presenti nei due termini
--------------------------------------------------------------------*/
void trsTermPair::GetVars(trsVarList &_vlist)
{
	//LEFT
	if (m_left->IsFunc())
		((trsFunc*)m_left)->GetVars(_vlist);
	if (m_left->IsVar())
		_vlist.push_back((trsVar*)m_left);
	//RIGHT
	if (m_right->IsFunc())
		((trsFunc*)m_right)->GetVars(_vlist);		
	if (m_right->IsVar()) {
		trsVarList::iterator vit;
		bool vfound = false;
		for (vit=_vlist.begin();(vit!=_vlist.end())&&(!vfound);vit++)
			if (**vit == *m_right) vfound = true;
		if (!vfound) 
			_vlist.push_back((trsVar*)m_right);
	}
}


/*- trsTermPair::UnRename --------------------------------------------

	Toglie la rinomina delle variabili dei termini destro e sinistro
--------------------------------------------------------------------*/
void trsTermPair::UnRename()
{
	trsVarList vlist;
	trsSubst subst;
	
	Assert(m_left&&m_right,trsException(TRSERR_NULLPTR,"NULL terms in pair"));
	
	//si raccolgono tutte le variabili della coppia di termini
	GetVars(vlist);
	
	if (vlist.empty()) return;
	
	//LEFT
	if (m_left->IsFunc())
		((trsFunc*)m_left)->UnRename(vlist,subst);
	if (m_left->IsVar()) {
		((trsVar*)m_left)->GetUnRenamed(vlist,subst);
		m_left = subst.GetSubstitute((trsVar*)m_left);
	}
	//RIGHT
	if (m_right->IsFunc())
		((trsFunc*)m_right)->UnRename(vlist,subst);
	if (m_right->IsVar()) {
		((trsVar*)m_right)->GetUnRenamed(vlist,subst);
		m_right = subst.GetSubstitute((trsVar*)m_right);
	}
}


// EOF trstermpair.cpp /////////////////////////////////////////////////////////
