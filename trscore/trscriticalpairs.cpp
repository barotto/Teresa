/*//////////////////////////////////////////////////////////////////////////////
	FILENAME: trscriticalpairs.cpp
	Version:  1.0.0
	Authors:  Marco Bortolin, Alberto Minora
	Created:  30/08/2001 - 9.42.02

	Copyright (C) 2001 Marco Bortolin, Alberto Minora
//////////////////////////////////////////////////////////////////////////////*/
#ifdef WIN32
#pragma warning(disable:4786)
#pragma warning(disable:4100)
#endif
#include "trs.h"
#include <cstdarg>
#include <cstdio>
#include <iostream>


/*- trsCriticalPairs::DupTrs -----------------------------------------

	Duplica il trs in ingresso rinominando le variabili
--------------------------------------------------------------------*/
void trsCriticalPairs::DupTrs(trsTermPairs &_trs, bool _rename)
{
	trsTermPairs::iterator it;
	trsTermPair tp;

	m_trs.clear();

	for (it=_trs.begin(); it!=_trs.end(); it++) {
		
		Assert((it->GetLeft()&&it->GetRight()),trsException(TRSERR_NULLPTR,
			"trsCriticalPairs::DupTrs(): NULL terms"));

		tp.SetLeft(it->GetLeft()->Duplicate());
		tp.SetRight(it->GetRight()->Duplicate());
		//Rename solo se richiesto (default true)
		if (_rename) 
			tp.Rename();
		m_trs.push_back(tp);
	}
	tp.AutoDispose(false);
}
	

/*- trsCriticalPairs::CriticalPairs ----------------------------------

	Trova le coppie critiche delle regole di _trs confrontate con la 
	sola regola _pair. Le coppie trovate vengono messe in _cp con autodispose 
	a true. Se _unreneme=false allora evita di togliere la rinomina.
--------------------------------------------------------------------*/
void trsCriticalPairs::CriticalPairs(trsTermPairs &_trs, trsTermPair &_pair,
		trsTermPairs &_cp, trsAlgebra &_algebra, bool _unrename)
{
	trsTermPairs::iterator it;
	list<int> poslist;

	m_cp = &_cp;
	m_unrename = _unrename;
	
	//creo un TRS con la sola coppia in input rinominata
	trsTermPair tp(_pair.GetLeft()->Duplicate(),
		_pair.GetRight()->Duplicate(),true);
	tp.Rename();
	m_trs.push_front(tp);
	tp.AutoDispose(false);

	if (m_outdev) {
		int pos = 0;
		*m_outdev << "TRS in ingresso:\n";
		for (it=m_trs.begin(); it!=m_trs.end(); it++,pos++)
			*m_outdev << "[" << pos << "] " << *it << "\n";
		*m_outdev << "Regola di confronto rinominata:\n[" << ++pos << "] "<< tp << "\n";
		*m_outdev << "\n";
	}

	m_rootpos = 0;
	for (it=_trs.begin(); it!=_trs.end(); it++,m_rootpos++) {
		//tanto qui non si confronta mai con se stessa
		CriticalPair(*it, it->GetLeft(), poslist, -1);
	}

	m_trs.clear();
	if (m_unrename)
		_algebra.CleanDupVars();
}


/*- trsCriticalPairs::CriticalPairs ----------------------------------

	Trova le coppie critiche della regola _pair con tutte le regole 
	di _trs. Se _itself=true allora confronta anche con se stessa 
	rinominata.	Le coppie trovate vengono messe in _cp con autodispose 
	a true. Se _unreneme=false allora evita di togliere la rinomina.
--------------------------------------------------------------------*/
void trsCriticalPairs::CriticalPairs(trsTermPair &_pair, trsTermPairs &_trs,
		trsTermPairs &_cp, trsAlgebra &_algebra, bool _itself, bool _unrename)
{
	trsTermPairs::iterator it;
	list<int> poslist;

	m_cp = &_cp;
	m_unrename = _unrename;

	// evito che rinomini il _trs in ingresso
	DupTrs(_trs,false);

	if (_itself) {
		trsTermPair tp(_pair.GetLeft()->Duplicate(),
			_pair.GetRight()->Duplicate(),true);
		m_trs.push_front(tp);
		tp.AutoDispose(false);
	}
	
	_pair.Rename();	

	if (m_outdev) {
		*m_outdev << "Regola in ingresso rinominata:\n[0] "<< _pair << "\n";
		int pos = 1;
		*m_outdev << "TRS di confronto:\n";
		for (it=m_trs.begin(); it!=m_trs.end(); it++,pos++)
			*m_outdev << "[" << pos << "] " << *it << "\n"; 
		*m_outdev << "\n";
	}
	m_rootpos = 0;
	if (_itself)
		CriticalPair(_pair, _pair.GetLeft(), poslist, 0);
	else
		CriticalPair(_pair, _pair.GetLeft(), poslist, -1);
	
	m_trs.clear();
		
	_pair.UnRename();

	if (m_unrename) {
		_algebra.CleanDupVars();
	}
}


/*- trsCriticalPairs::CriticalPairs ----------------------------------

	Trova le coppie critiche di tutte le regole di _trs.
	Le coppie trovate vengono messe in _cp con autodispose a true.
	Se _unreneme=false allora evita di togliere la rinomina.
--------------------------------------------------------------------*/
void trsCriticalPairs::CriticalPairs(trsTermPairs &_trs, trsTermPairs &_cp, 
	trsAlgebra &_algebra, bool _unrename)
{
	trsTermPairs::iterator it;
	list<int> poslist;

	m_cp = &_cp;
	m_unrename = _unrename;
	DupTrs(_trs);
	if (m_outdev) {
		*m_outdev <<"TRS in ingresso:\n";
		int pos = 0;
		for (it=_trs.begin(); it!=_trs.end(); it++,pos++)
			*m_outdev << "[" << pos << "] " << *it << "\n"; 
		*m_outdev << "\n";
		*m_outdev << "TRS con variabili rinominate:\n";
		for (it=m_trs.begin(); it!=m_trs.end(); it++,pos++)
			*m_outdev << "[" << pos << "] " << *it << "\n"; 
		*m_outdev << "\n";
	}
	m_rootpos = 0;
	for (it=_trs.begin(); it!=_trs.end(); it++,m_rootpos++) {
		CriticalPair(*it, it->GetLeft(), poslist, m_rootpos);
	}

	m_trs.clear();
	if (m_unrename)
		_algebra.CleanDupVars();
}


/*- trsCriticalPairs::CriticalPair -----------------------------------


--------------------------------------------------------------------*/
void trsCriticalPairs::CriticalPair(trsTermPair &_pair, trsTerm *_root,
	list<int> &_l, int _trspos)
{
	trsUnify unif(NULL,m_feedback);
	trsTermPairs::iterator it;
	trsSubst sub;
	trsTermPair tp(NULL,NULL,false);
	int mtrspos;

	if (!_pair.GetLeft()->IsFunc())
		return;

	mtrspos = 0;
	for (it=m_trs.begin(); it!=m_trs.end(); it++,mtrspos++) {
		if ((_trspos>=0) && (_trspos==mtrspos)) continue;
		tp.SetLeft(_pair.GetLeft());
		tp.SetRight(it->GetLeft());
		try {
			unif.Unify(tp,sub);
		} catch(trsUnifyException) { continue; }
		if (m_outdev) { 
			*m_outdev << "Unificazione riuscita tra ";
			*m_outdev << "[" << m_rootpos << "]" << _l << " ";
			*m_outdev << tp.GetLeft()->GetPrintable();
			*m_outdev << " e ";
			*m_outdev << "[" << mtrspos+m_trs.size() << "] ";
			*m_outdev << tp.GetRight()->GetPrintable();
			*m_outdev << "\n" << "mgu:\n" << sub;
		}
		AddCP(_pair.GetLeft(),_pair.GetRight(),it->GetRight(),sub,_root,_l);
		sub.Reset();
	}
	trsTerm* arg;
	trsArgList::iterator argit;
	int argpos;

	trsFunc* fl1 = (trsFunc*)_pair.GetLeft();
	arg = fl1->GetFirstArg(argit);
	argpos = 0;
	_l.push_back(argpos);
	tp.SetRight(_pair.GetRight());
	while(arg) {
		_l.pop_back();
		_l.push_back(argpos);
		tp.SetLeft(arg);
		CriticalPair(tp,_root,_l,-1);
		arg = fl1->GetNextArg(argit);
		argpos++;
	}
	_l.pop_back();
}


/*- trsCriticalPairs::AddCP ------------------------------------------


--------------------------------------------------------------------*/
void trsCriticalPairs::AddCP(trsTerm *_l1, trsTerm *_r1, trsTerm *_r2,
	trsSubst &_sub, trsTerm *_root, list<int> &_l)
{
	trsTermPair tp; 
	trsTerm *root;
	trsTerm *r1,*r2;
	trsFunc *father;
	
	Assert((_root&&_l1&&_r1&&_r2),trsException(TRSERR_NULLPTR,
		"trsCriticalPairs::AddCP() : NULL argument"));

	r1 = _r1->Duplicate();	
	r1 = _sub.TermApplySub(r1);
	tp.SetLeft(r1);
	r2 = _r2->Duplicate();	
	r2 = _sub.TermApplySub(r2);
	
	root = _root->Duplicate();
	
	// root e` sicuramente una funzione
	trsTerm* t = ((trsFunc*)root)->GetPosTerm(_l);
	if (root != t) {
		father = ((trsFunc*)t)->GetFather();
		father->SubstArg(_l.back(),r2);
	} else {
		delete root;
		root = r2;
	}
	_sub.TermApplySub(root);
	tp.SetRight(root);
	if (m_outdev) *m_outdev << "Trovata coppia critica : <" 
		<< tp.GetLeft()->GetPrintable() << "," << tp.GetRight()->GetPrintable() 
		<< ">\n";
	
	if (CheckDuplicate(tp)) {
		if (m_outdev) *m_outdev << "   Coppia già trovata.\n";
	} else if (*tp.GetLeft() == *tp.GetRight()) {
	 	if (m_outdev) *m_outdev << "   Termini uguali.\n";
	} else {
		if (m_unrename)
			tp.UnRename();
		m_cp->push_back(tp);
		tp.AutoDispose(false);
	}
}


/*- trsCriticalPairs::CheckDuplicate ---------------------------------

	Vero sse nella lista in m_cp esistono duplicati di _tp 
--------------------------------------------------------------------*/
bool trsCriticalPairs::CheckDuplicate(trsTermPair &_tp)
{
	trsTermPairs::iterator it;

	for (it=m_cp->begin(); it != m_cp->end(); it++) 
	{
		if (*(it->GetLeft()) == *(_tp.GetLeft()))
			if (*(it->GetRight()) == *(_tp.GetRight()))
				return true;
	}
	return false;
}


// EOF trscriticalpairs.cpp ////////////////////////////////////////////////////
