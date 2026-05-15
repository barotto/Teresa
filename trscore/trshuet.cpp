/*//////////////////////////////////////////////////////////////////////////////
	FILENAME: trshuet.cpp
	Version:  1.1.0
	Authors:  Marco Bortolin, Alberto Minora
	Created:  05/09/2001 - 09.30

	Copyright (C) 2001 Marco Bortolin, Alberto Minora
//////////////////////////////////////////////////////////////////////////////*/
#ifdef WIN32
#pragma warning(disable:4786)
#pragma warning(disable:4100)
#endif
#include "trs.h"
#include <algorithm>
#include <iostream>


/*- trsHuet::Simplify ------------------------------------------------

	Normalizza i termini della coppia _tp
--------------------------------------------------------------------*/
void trsHuet::Simplify(trsTermPair &_tp)
{
	trsTerm *s,*t;

	m_feedback->PrintFuncName("Simplify");
	//LEFT
	try {
		s = m_rew.Normalize(_tp.GetLeft(),m_trs);
	} catch(trsException E) {
		Exit();
		throw trsHuetException(E.GetCode(),E.GetMsg());
	}
	if (_tp.GetLeft() != s) {
		delete _tp.GetLeft();
		_tp.SetLeft(s);
	}

	//RIGHT
	try {
		t = m_rew.Normalize(_tp.GetRight(),m_trs);
	} catch(trsException E) {
		Exit();
		throw trsHuetException(E.GetCode(),E.GetMsg());
	}
	if (_tp.GetRight() != t) {
		delete _tp.GetRight();
		_tp.SetRight(t);
	}
}


/*- trsHuet::Delete --------------------------------------------------

	Se i termini destro e sinistro di _tp sono uguali allora
	pone l'autodispose di _tp a true.
--------------------------------------------------------------------*/
bool trsHuet::Delete(trsTermPair &_tp)
{
	m_feedback->PrintFuncName("Delete");
	if (*(_tp.GetLeft()) == *(_tp.GetRight())) {
		_tp.AutoDispose(true);
		return true;
	}
	return false;
}


/*- trsHuet::Orient --------------------------------------------------

	Se dx>sx allora inverte la coppia _tp. Se i termini non sono
	confrontabili lancia una eccezione.
--------------------------------------------------------------------*/
void trsHuet::Orient(trsTermPair &_tp)
{
	trsTerm* s = _tp.GetLeft();
	trsTerm* t = _tp.GetRight();
	bool greater = false;
	
	m_feedback->PrintFuncName("Orient");
	try {
		greater = m_ord->Greater(s,t);
	} catch(trsOrderException) {
		// s,t non confrontabili
	}
	if (!greater) {
		try {
			if (m_ord->Less(s,t)) {
				_tp.Swap();
			} else {
				throw trsHuetException(0,"Impossible orientare.");
			}
		} catch (trsOrderException) {
			// se non e` possibile orientare s,t allora FAIL
			throw trsHuetException(0,"Impossible orientare.");
		}
	}
}


/*- trsHuet::LRSimplify ----------------------------------------------


--------------------------------------------------------------------*/
void trsHuet::LRSimplify(trsTermPair &_tp)
{
	m_feedback->PrintFuncName("L-R Simplify");

	if (m_outdev) {
		*m_outdev << "- Regole del TRS -\n";
		PrintTRS(*m_outdev,*m_trs);
	}

	trsTermPairs *nexttrs, *oldtrs;
	trsTermPairs::iterator it;
	list<bool> *nextmarks;
	list<bool>::iterator itm;
	trsTerm *g, *d, *dnorm, *dup;
	trsTermPair tp(NULL,NULL,false);

	nexttrs = new trsTermPairs();
	oldtrs = new trsTermPairs(*m_trs);
	nextmarks = new list<bool>;
	
	//inserisco la regola l->r nel trs
	m_trs->push_front(_tp);

	it = oldtrs->begin();
	itm = m_marks->begin();
	while (it != oldtrs->end()) {
		if (m_outdev)
			*m_outdev << "Esamino la regola " << it->GetLeft()->GetPrintable() 
			<< "-->" << it->GetRight()->GetPrintable() << "\n";
		dup = it->GetLeft()->Duplicate();
		try {
			g = m_rew.Normalize(dup,&_tp);
		} catch(trsException E) {
			for (it=oldtrs->begin(); it != oldtrs->end(); it++)
				it->AutoDispose(false);
			if (dup->IsFunc()) delete dup;
			m_trs->pop_front();
			delete oldtrs;
			delete nexttrs;
			delete nextmarks;
			Exit();
			throw trsHuetException(E.GetCode(),E.GetMsg());
		}
		if ((g != dup)) //&& (dup->IsFunc())) 
			delete dup;
		d = it->GetRight();		
		it->AutoDispose();
		if (!(*g == *(it->GetLeft()))) {
			// poiche` g e` stato ridotto in g' inserisco g'=d nella teoria
			tp.SetLeft(g);
			tp.SetRight(d);
			if (CheckDuplicate(&m_theory,tp)) {
				if (g->IsFunc()) delete g;
				it++;
				itm++;
				continue;
			}
			if (m_outdev)
				*m_outdev << "   Inserisco la coppia " << g->GetPrintable() << "="
				<< d->GetPrintable() << " nella teoria\n";
			m_theory.push_back(tp);
			it->SetRight(NULL);
		} else {
			if (g->IsFunc()) delete g;
			g = it->GetLeft();
			// g non si puo` ridurre con l->r quindi
			// normalizzo d (dnorm) con trs + l->r
			dup = it->GetRight()->Duplicate();
			try {
				dnorm = m_rew.Normalize(dup,m_trs);
			} catch(trsException E) {
				for (it=oldtrs->begin(); it != oldtrs->end(); it++)
					it->AutoDispose(false);
				if (dup->IsFunc()) delete dup;
				m_trs->pop_front();
				delete oldtrs;
				delete nexttrs;
				delete nextmarks;
				Exit();
				throw trsHuetException(E.GetCode(),E.GetMsg());
			}
			if ((dnorm != dup)) //&& (dup->IsFunc())) 
				delete dup;
			// inserisco g->dnorm nel nuovo trs
			tp.SetLeft(g);
			tp.SetRight(dnorm);
			if (CheckDuplicate(nexttrs,tp)) {
				// la coppia e` gia` presente, la elimino
				if (dnorm->IsFunc()) delete dnorm;
				it++;
				itm++;
				continue;
			}
			//if (dnorm == it->GetRight())
			//	it->SetRight(NULL);
			nexttrs->push_back(tp);
			it->SetLeft(NULL);
			// g->dnorm eredita il marker da g->d 
			nextmarks->push_back(*itm);
			if (m_outdev)
				*m_outdev << "   Inserisco la regola " << g->GetPrintable() << "-->"
				<< dnorm->GetPrintable() << " nel nuovo TRS\n";
		}
		it++;
		itm++;
	}

	//@@ necessario? sembra di no...
	//if (CheckDuplicate(nexttrs,_tp)) {
	//	_tp.AutoDispose();
	//} else {
		if (m_outdev)
			*m_outdev << "Inserisco " << _tp.GetLeft()->GetPrintable() << "-->"
			<< _tp.GetRight()->GetPrintable() << " (l->r) nel nuovo TRS\n";

		//inserisco la regola l->r nel nuovo trs
		nexttrs->push_back(_tp);
		// l->r non e` marcata
		nextmarks->push_back(false);
	//}
	delete m_trs;
	m_trs = nexttrs;
	delete m_marks;
	m_marks = nextmarks;
	delete oldtrs;
}


/*- trsHuet::Deduce --------------------------------------------------


--------------------------------------------------------------------*/
void trsHuet::Deduce()
{
	list<bool>::iterator itm;
	trsTermPairs::iterator it;
	
	Assert(m_theory.empty(),trsHuetException(0,"trsHuet::Deduce(): teoria non vuota"));

	m_feedback->PrintFuncName("Deduce");

	// la teoria e` vuota
	// scelgo una regola l->r non marcata del trs
	trsTermPair *unmarked;
	int pos;
	pos = ChooseRule();
	if (pos < 0)
		return;
	it = m_trs->begin();
	advance(it,pos);
	unmarked = &(*it);

	// riempio un trs con le regole marcate di m_trs
	trsTermPairs markedtrs;
	for (itm=m_marks->begin(),it=m_trs->begin(); it != m_trs->end(); it++,itm++) {
		if (*itm == true)
			markedtrs.push_back(*it);
	}
		
	// cerco le coppie critiche di markedtrs
	if (m_outdev)
		*m_outdev << "Cerco coppie critiche :\n";

	trsTermPairs cp;
	trsTermPairs::iterator cpit;
	
	// cerco prima tra (l,r) e TRS marcato + (l,r)
	bool cpfound = false;
	
	m_crit.CriticalPairs(*unmarked, markedtrs, cp, *m_sys->GetAlgebra(),true,false);
	
	if (!cp.empty()) {
		cpfound = true;
		// inserisco le coppie critiche trovate nella teoria
		for (cpit=cp.begin(); cpit != cp.end(); cpit++) {
			cpit->AutoDispose(false);
			m_theory.push_back(*cpit);
			if (m_outdev)
				*m_outdev << "   <" << *cpit << ">\n";
		}
	}
	
	// poi cerco anche tra TRS marcato e (l,r)
	// markedtrs contiene anche (l,r)
	// pulisco la lista di coppie critiche che ho appena usato
	cp.clear();
	m_crit.CriticalPairs(markedtrs, *unmarked, cp, *m_sys->GetAlgebra(),false);
	
	if (!cp.empty()) {
		cpfound = true;
		// inserisco le coppie critiche trovate nella teoria
		for (cpit=cp.begin(); cpit != cp.end(); cpit++) {
			cpit->AutoDispose(false);
			m_theory.push_back(*cpit);
			if (m_outdev)
				*m_outdev << "   <" << *cpit << ">\n";
		}
	}
	
	// marco la regola che ho usato per le coppie critiche
	itm = m_marks->begin();
	advance(itm,pos);
	*itm = true;

	if (!cpfound) {
		if (m_outdev)
			*m_outdev << "   Nessuna trovata.\n";
		return;
	}
	
}


/*- trsHuet::DupTheory -----------------------------------------------

	Duplica la teoria in ingresso
--------------------------------------------------------------------*/
void trsHuet::DupTheory(trsTermPairs &_theory)
{
	trsTermPairs::iterator it;

	m_theory.clear();

	for (it=_theory.begin(); it!=_theory.end(); it++) {
		
		Assert((it->GetLeft() && it->GetRight()),trsException(TRSERR_NULLPTR,
			"trsHuet::DupTheory(): NULL terms"));

		m_theory.push_back(trsTermPair(it->GetLeft()->Duplicate(),
			it->GetRight()->Duplicate(),false));
	}
}


/*- *trsHuet::ChooseRule ---------------------------------------------

	Sceglie una regola non marcata dal trs sulla quale calcolare le 
	coppie critiche. Se non esistono regole non marcate ritorna -1.
--------------------------------------------------------------------*/
int trsHuet::ChooseRule()
{
	trsTermPairs::iterator trsit, cruleit; // c=choosen
	list<bool>::iterator markit, cmarkit;
	int size, cursize, cpos, pos;

	if (m_trs->empty())
		return -1;

	size = INT_MAX;
	cruleit = m_trs->end();
	cmarkit = m_marks->end();
	for (trsit=m_trs->begin(),markit=m_marks->begin(),pos=0;
		trsit != m_trs->end(); trsit++,markit++,pos++) {
		
		if (*markit == true) continue;
		cursize = trsit->GetLeft()->Size() + trsit->GetRight()->Size();
		if (cursize >= size) continue;
		cruleit = trsit;
		cmarkit = markit;
		cpos = pos;
		size = cursize;
	}

	if (size == INT_MAX) 
		return -1;

	return cpos;
}


/*- trsHuet::Completion ----------------------------------------------

	Completa la teoria del trsSystem in input con la procedura di Huet.

	Del feedback device usa la PrintInt col seguente vettore:
	pos  significato
	0    numero del loop piu` esterno
	1    dimensione del TRS

	Utilizza la PrintFuncName con le seguenti stringhe:
	"Simplify",	"Delete", "Orient",	"L-R Simplify",	"Deduce"
--------------------------------------------------------------------*/
void trsHuet::Completion(trsSystem &_sys, trsOrder *_ord)
	throw (trsHuetException)
{		
	trsTermPairs::iterator it;
	int fbvec[2];

	Assert(_ord,trsException(TRSERR_NULLPTR,
			"trsHuet::Completion(): NULL order"));
	
	m_trs = new trsTermPairs();
	m_marks = new list<bool>;
	m_ord = _ord;
	m_ord->SetDevices(NULL,m_feedback);
	m_sys = &_sys;
			
	DupTheory(*_sys.GetTheory());
		
	fbvec[0]=0; //loop
	fbvec[1]=0; //dim trs
	int loopnum = 0;
	int tsize = 0;
	m_feedback->Reset();
	m_feedback->SetProgressStep(1);
	while ((!m_theory.empty()) || 
		(find(m_marks->begin(),m_marks->end(),false) != m_marks->end())) {
		
		if (m_outdev)
			*m_outdev << "\nciclo: "<< loopnum << "\n";

		m_feedback->PrintInt(fbvec,2);
		fbvec[0] += 1;
		tsize = m_theory.size();
		m_feedback->SetProgressRange(0,tsize);

		while (!m_theory.empty()) {
			if (m_outdev)
				*m_outdev << "\n--- Teoria ---\n" << m_theory << "--------------\n";
			
			// l'utente vuole bloccare?
			if (m_feedback->Stop()) {
				Exit();
				return;
			}
			// scelgo, normalizzo e oriento una coppia s=t della teoria
			if ( (it = ChoosePair()) == m_theory.end()) {
				m_feedback->SetProgressPos(tsize);
				continue;
			}

			LRSimplify(*it);

			if (m_theory.size() > tsize) {
				tsize += m_theory.size()-tsize;
				m_feedback->SetProgressRange(0,tsize);
			}
			m_feedback->SetProgressPos(tsize-m_theory.size());
			fbvec[1] = m_trs->size();
			m_feedback->PrintInt(fbvec,2);

			// tolgo dalla teoria la coppia s=t
			m_theory.erase(it);
		}
		Deduce();
	}
	if (m_outdev)
		*m_outdev << "\nCompletamento terminato.\n";
	
	Exit();
}


/*- trsHuet::ChoosePair ----------------------------------------------

	Sceglie una coppia di termini dalla teoria, la normalizza, la 
	orienta e ne resituisce un iteratore.
--------------------------------------------------------------------*/
trsTermPairs::iterator trsHuet::ChoosePair()
{
	trsTermPairs::iterator it;
	trsTerm *ldup, *rdup;
	
	it=m_theory.begin();
	if (m_outdev) {
		*m_outdev << "TRS corrente:\n";
		PrintTRS(*m_outdev,*m_trs);
	}

	while (it != m_theory.end()) {
		// copie di backup
		ldup = it->GetLeft()->Duplicate();
		rdup = it->GetRight()->Duplicate();
		if (m_outdev)
			*m_outdev << "Considero la coppia " << *it << "\n";

		Simplify(*it);
			
		if (m_outdev)
			*m_outdev << "Normalizzata : " << *it << "\n";

		if (Delete(*it)) {
			if (ldup->IsFunc())	delete ldup;
			if (rdup->IsFunc())	delete rdup;
			m_theory.erase(it++);
			if (m_outdev)
				*m_outdev << "Termini uguali, coppia eliminata.\n";
			m_feedback->StepProgress();
			continue;
		}
		try {	
			if (m_outdev)
				*m_outdev << "Orientata : ";
			Orient(*it);
		} catch(trsHuetException E) {
			if (m_outdev)
				*m_outdev << "impossibile orientare\n";
			if (++it == m_theory.end()) {
				if (m_outdev)
					*m_outdev << "\nNessuna coppia nella teoria e` orientabile.\n" 
					<< "Completamento fallito.\n";
				if (ldup->IsFunc())	delete ldup;
				if (rdup->IsFunc())	delete rdup;
				Exit();
				throw E;
			} else {
				it--;
				if (it->GetLeft()->IsFunc()) delete it->GetLeft();
				if (it->GetRight()->IsFunc()) delete it->GetRight();
				it->SetLeft(ldup);
				it->SetRight(rdup);
				it++;
				continue;
			}
		} catch (trsException E) {
			if (ldup->IsFunc()) delete ldup;
			if (rdup->IsFunc()) delete rdup;
			Exit();
			throw trsHuetException(E.GetCode(),E.GetMsg());
		}
		if (m_outdev)
			*m_outdev << it->GetLeft()->GetPrintable() << "-->"
				<< it->GetRight()->GetPrintable() << "\n";
		if (ldup->IsFunc()) delete ldup;
		if (rdup->IsFunc()) delete rdup;
		return it;
	}
	// vale (it == m_theory.end())
	return it;
}


/*- trsHuet::CheckDuplicate ------------------------------------------

	Vero sse in _l esistono duplicati di _tp 
--------------------------------------------------------------------*/
bool trsHuet::CheckDuplicate(trsTermPairs *_l, trsTermPair &_tp)
{
	trsTermPairs::iterator it;

	for (it=_l->begin(); it != _l->end(); it++) 
	{
		if (*(it->GetLeft()) == *(_tp.GetLeft()))
			if (*(it->GetRight()) == *(_tp.GetRight()))
				return true;
	}
	return false;
}


/*- trsHuet::Exit ----------------------------------------------------

	Compie le ultime azioni prima del termine
--------------------------------------------------------------------*/
void trsHuet::Exit()
{
	trsTermPairs::iterator it;
	trsTermPair tp;

	//scrivo il TRS calcolato nel trsSystem
	for (it=m_trs->begin(); it != m_trs->end(); it++) {
		tp.SetLeft(it->GetLeft());
		tp.SetRight(it->GetRight());
		tp.UnRename();
		m_sys->GetTrs()->push_back(tp);
	}
	tp.AutoDispose(false);
	
	//cerr << " end";

	// elimino il trs con i marcatori delle regole
	delete m_trs;
	delete m_marks;
	// m_theory potrebbe non essere vuota
	for (it=m_theory.begin(); it != m_theory.end(); it++)
		it->AutoDispose(true);
	// prima della CleanDupVars() bisogna cancellare m_theory poiche` in essa 
	// ci potrebbero essere variabili rinominate che, cancellate e poi 
	// dereferenziate nel distruttore del trsTermPair, potrebbero generare gpf
	m_theory.clear();

	m_sys->GetAlgebra()->CleanDupVars();
	m_ord->SetDevices(NULL,NULL);
}


// EOF trshuet.cpp /////////////////////////////////////////////////////////////
