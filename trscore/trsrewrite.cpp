/*//////////////////////////////////////////////////////////////////////////////
	FILENAME: trsrewrite.cpp
	Version:  1.0.0
	Author:   Marco Bortolin, Alberto Minora
	Created:  30/08/2001 - 17.24.27

	Copyright (C) 2001 Marco Bortolin, Alberto Minora
//////////////////////////////////////////////////////////////////////////////*/
#ifdef WIN32
#pragma warning(disable:4786)
#pragma warning(disable:4100)
#endif
#include "trs.h"
#include <algorithm>
#include <iostream>



/*- trsRewrite::Zip --------------------------------------------------

	Date due liste di termini restituisce una lista di coppie di
	termini, in cui ogni coppia e` formata dagli elementi delle due
	liste che hanno la stessa posizione. Si suppone che le liste in
	input abbiano la stessa lunghezza.
--------------------------------------------------------------------*/
trsTermPairs* trsRewrite::Zip(trsArgList* _s, trsArgList* _t)
{
	trsTermPairs* l;
	trsArgList::iterator its,itt;
	trsTermPair tp(NULL,NULL,false);
	
	l = new trsTermPairs;
	its = _s->begin();
	itt = _t->begin();
	while (its != _s->end()) {
		tp.SetLeft(*its);
		tp.SetRight(*itt);
		l->push_back(tp);
		its++;
		itt++;
	}
	return l;
}


/*- trsRewrite::Match ------------------------------------------------

	Dato il problema di matching _l, restituisce il matcher _sub
--------------------------------------------------------------------*/
void trsRewrite::Match(trsTermPairs &_l,trsSubst &_sub) 
	throw(trsMatchException)
{
	trsTermPairs::iterator it;
	trsVar* v;
	trsTerm* t;
	trsTerm *sx,*dx;
	trsTermPairs* l;
	
	//*
	// scorro tutte le coppie di termini del problema
	it = _l.begin();
	while (it != _l.end()) {
		sx = (*it).GetLeft();
		dx = (*it).GetRight();
		// il termine sinistro e` una variabile ?
		if (sx->IsVar()) {
			v = (trsVar*) sx;
			// appartiene al dominio della sostituzione ? 
			if (_sub.Indomain(v)) {
				t = _sub.GetSubstitute(v);
				// il sostituto e` diverso ? 
				if (!(*t == *dx)) throw trsMatchException();
			} else {
				// aggiungo la coppia alla sost.
				_sub.AddSubPair(v,dx);
			}
		// se il termine destro e` una variabile errore!
		} else if (dx->IsVar()) {
			throw trsMatchException();
		} else {
			// i termini destro e sinistro sono 2 funzioni o costanti.
			// sono diverse?
			if (*(sx->GetSim()) != *(dx->GetSim()))
				throw trsMatchException();
			// confronto i loro argomenti
			if (!sx->IsConst()) {
				l = Zip(((trsFunc*) sx)->GetArgs(),((trsFunc*) dx)->GetArgs());
				try {
					Match(*l,_sub);
				} catch (trsMatchException E)	{
					delete(l);
					throw E;
				}
				delete(l);
			}
		}
		it++;
	}
}


/*- trsRewrite::Match ------------------------------------------------

	Risolve il problema del matching per una singola coppia di termini
--------------------------------------------------------------------*/
void trsRewrite::Match(trsTermPair &_tp,trsSubst &_sub)
	throw(trsMatchException)
{
	trsTermPairs l;
	trsTermPair tp(NULL,NULL,false);

	tp.SetLeft(_tp.GetLeft());
	tp.SetRight(_tp.GetRight());
	l.push_back(tp);
	Match(l,_sub);
}


/*- trsRewrite::Rewrite ----------------------------------------------

	Crea un duplicato del termine _t e gli applica la sola regola _pair;
	Se non ha applicato la regola restituisce NULL altrimenti 
	restituisce il duplicato riscritto.
--------------------------------------------------------------------*/
trsTerm* trsRewrite::Rewrite(trsTerm* _t, trsTermPair* _pair)
{
	trsTermPairs l;
	trsTermPair tp(NULL,NULL,false);

	tp.SetLeft(_pair->GetLeft());
	tp.SetRight(_pair->GetRight());
	l.push_back(tp);
	return Rewrite(_t,&l);
}


/*- trsRewrite::Rewrite ----------------------------------------------

	Se non e` possibile applicare alcuna regola restituisce NULL.
	Altrimenti crea un duplicato di _t, vi applica la prima regola
	adatta che trova e lo restituisce.
--------------------------------------------------------------------*/
trsTerm* trsRewrite::Rewrite(trsTerm* _t, trsTermPairs* _trs) 
{
	trsTermPair tp(NULL,NULL,false);
	trsSubst subst;
	trsTermPairs::iterator it;
	trsTerm *sx,*dx;
	trsTerm *newterm;

	if ((_trs == NULL) || (_t == NULL)) return NULL;

	it = _trs->begin();
	while (it != _trs->end()) {
		sx = it->GetLeft();
		dx = it->GetRight();
		try {
			tp.SetLeft(sx);
			tp.SetRight(_t);
			Match(tp,subst); 
			newterm = dx->Duplicate();
			//qui TermApplySub fa la cosa giusta in base al tipo di termine
			//applico il matcher
			newterm = subst.TermApplySub(newterm);
			return newterm;
		} catch(trsMatchException) {
			subst.Reset();
		} 
		it++;
	}
	return NULL;
}


/*- trsRewrite::Normalize --------------------------------------------

	Normalizza _t in base a _trs. Restituisce un puntatore al termine 
	normalizzato che puo` anche non essere uguale a _t.
	Lancia trsException nel caso abbia ottenuto dall'utente un 
	comando di stop.
--------------------------------------------------------------------*/
trsTerm* trsRewrite::Normalize(trsTerm* _t, trsTermPairs* _trs) 
	throw (trsException)
{
	trsTerm *arg, *normarg, *rewterm, *prevterm;
	trsArgList::iterator argit;
	int i;
	string cmpstr;
		
	if (m_feedback->Stop())
		throw trsException(TRSERR_USERSTOP,"Interruzione forzata.");

	if (_t->IsVar()) return _t;
	if (_t->IsConst()) return _t;
	trsFunc* ft = (trsFunc*) _t;
	arg = ft->GetFirstArg(argit);
	i=0;
	while (arg != NULL) {
		normarg = Normalize(arg,_trs);
		if (arg != normarg)
			ft->SubstArg(i,normarg);
		arg = ft->GetNextArg(argit);
		i++;
	}
	prevterm = ft;
	rewterm = Rewrite(prevterm,_trs);
	if (rewterm == NULL) return prevterm;
	cmpstr = *rewterm->GetCmpString();
	prevterm = Normalize(rewterm,_trs);
	if (rewterm != prevterm) {
		delete rewterm;
	} else {
		if (cmpstr == *prevterm->GetCmpString())
			return prevterm;
	}
	//@@ loop infinito possibile
	while ((rewterm = Rewrite(prevterm,_trs)) != NULL) {
		// qui evitiamo che vengano cancellate delle var.
		// che possono essere comunque riscritte da Rewrite()
		if (prevterm->IsFunc())
			delete prevterm;
		cmpstr = *rewterm->GetCmpString();
		prevterm = Normalize(rewterm,_trs);
		if (rewterm != prevterm) {
			delete rewterm;
		} else {
			if (cmpstr == *prevterm->GetCmpString())
				return prevterm;
		}
	}
	return prevterm;
}


/*- trsRewrite::Normalize --------------------------------------------

	Versione per TRS costituito da una sola coppia.
	Lancia trsException nel caso abbia ottenuto dall'utente un 
	comando di stop.
--------------------------------------------------------------------*/
trsTerm* trsRewrite::Normalize(trsTerm* _t, trsTermPair *_tp)
	throw (trsException)
{
	trsTermPairs l;
	
	l.push_back(trsTermPair(_tp->GetLeft(),_tp->GetRight(),false));
	return Normalize(_t,&l);
}

// EOF trsrewrite.cpp //////////////////////////////////////////////////////////
