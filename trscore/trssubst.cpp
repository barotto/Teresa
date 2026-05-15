/*//////////////////////////////////////////////////////////////////////////////
	FILENAME: trssubst.cpp
	Version:  1.0.0
	Authors:  Alberto Minora, Marco Bortolin
	Created:  29/08/2001 - 09.37

	Copyright (C) 2001 Marco Bortolin, Alberto Minora
//////////////////////////////////////////////////////////////////////////////*/
#ifdef WIN32
#pragma warning(disable:4786)
#pragma warning(disable:4100)
#endif
#include "trs.h"
#include <algorithm>
#include <iostream>


/*- trsSubst::AddSubPair ---------------------------------------------

	Aggiunge una nuova coppia (v,t) nella sostituzione.
	Il termine _ter viene duplicato
--------------------------------------------------------------------*/
void trsSubst::AddSubPair(trsVar* _var, trsTerm* _ter)
{
	trsSubstPair sp;
	
	sp.var = _var;
	sp.ter = _ter->Duplicate();

	m_subst.push_back(sp);
	sp.ter = NULL;
	sp.var = NULL;
}


/*- trsSubst::Indomain -----------------------------------------------

	Determina se una variabile è nel dominio della sostituzione
--------------------------------------------------------------------*/
bool trsSubst::Indomain(trsVar* _var)
{	
	list<trsSubstPair>::iterator it;
	for (it=m_subst.begin(); it != m_subst.end(); it++) {
		if (*it->var == *_var)
			return true;
	}
	return false;
}


/*- trsSubst::Inrange ------------------------------------------------

	Restituisce vero se il termine _ter e` presente come sostituto
	di una variabile. Falso altrimenti.
--------------------------------------------------------------------*/
bool trsSubst::Inrange(trsTerm *_ter)
{
	list<trsSubstPair>::iterator it;
	for (it=m_subst.begin(); it != m_subst.end(); it++) {
		if (*it->ter == *_ter)
			return true;
	}
	return false;
}


/*- trsSubst::GetSubstitute ------------------------------------------

	Restituisce il termine sostituto di una variabile.
--------------------------------------------------------------------*/
trsTerm* trsSubst::GetSubstitute(trsVar* _var)
{
	list<trsSubstPair>::iterator it;
	trsTerm* ret;

	it = m_subst.begin();
	while (it != m_subst.end()) {
		if (*(it->var) == *_var) {
			ret = it->ter;
			if (it->ter->IsFunc())
				ret = ((trsFunc*)it->ter)->Duplicate();
			return (ret);
		}
		it++;
	}
	return _var;
}


/*- trsSubst::TermApplySub -------------------------------------------

	Applica la sostituzione al termine _ter. Se _ter e` una funzione
	sostituisce tutte le variabili presenti nella subst e restituisce 
	_ter. Se e` una variabile restituisce il suo sostituto (se presente). 
	Se e` una costante restituisce se stesso.
--------------------------------------------------------------------*/
trsTerm* trsSubst::TermApplySub(trsTerm* _ter)
{
	trsFunc* f;
	trsTerm* child;
	trsArgList::iterator it;
	int posarg;
	

	if (_ter->IsVar())
		return GetSubstitute((trsVar*) _ter);
	
	if (_ter->IsConst())
		return _ter;

	f = (trsFunc*) _ter;
	it = f->GetArgs()->begin();
	posarg = 0;
	while (it != f->GetArgs()->end()) {
		if ((*it)->IsVar()) {
			child = GetSubstitute((trsVar*) *it);
			//lo sostituisco nel termine in input
			f->SubstArg(posarg,child);
		} else {
			if ((*it)->IsFunc()) {
				trsTerm* t = TermApplySub(*it);
				f->SubstArg(posarg,t);
			}
		}
		posarg++;
		it++;
	}
	return f;
}



/*- trsSubst::PrintSubst ---------------------------------------------

	Stampa la sostituzione sul device _out. (obsoleta, usare <<)
--------------------------------------------------------------------*/
void trsSubst::PrintSubst(trsOutputDev &_out)
{
	list<trsSubstPair>::iterator it;

	if (m_subst.empty()) {
		_out << "Sostituzione vuota!\n";
	}

	it = m_subst.begin();
	while (it != m_subst.end()) {
		_out << it->var->GetPrintable() << " -> " << it->ter->GetPrintable() << "\n";
		it++;
	}
}
	

/*- operator << ------------------------------------------------------

	Stampa su un trsOutputDev la sostituzione
--------------------------------------------------------------------*/
trsOutputDev &operator <<(trsOutputDev &_dev, trsSubst &_subst)
{
	list<trsSubst::trsSubstPair>::iterator it;

	if (_subst.m_subst.empty()) {
		_dev << "Sostituzione vuota." << "\n";
	}

	it = _subst.m_subst.begin();
	while (it != _subst.m_subst.end()) {
		_dev << it->var->GetPrintable() << " -> " << it->ter->GetPrintable() << "\n";
		it++;
	}
	return _dev;
}


/*- trsSubst::Reset --------------------------------------------------

	Cancella tutto il contenuto della sostituzione
--------------------------------------------------------------------*/
void trsSubst::Reset()
{
	m_subst.clear();
}


// EOF trssubst.cpp ///////////////////////////////////////////////////////////
