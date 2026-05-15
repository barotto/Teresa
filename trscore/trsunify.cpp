/*//////////////////////////////////////////////////////////////////////////////
	FILENAME: trsunify.cpp
	Version:  1.0.0
	Authors:  Marco Bortolin, Alberto Minora 
	Created:  18/08/2001 - 16.12

	Copyright (C) 2001 Marco Bortolin, Alberto Minora
//////////////////////////////////////////////////////////////////////////////*/
#ifdef WIN32
#pragma warning(disable:4786)
#pragma warning(disable:4100)
#endif
#include "trs.h"
#include <algorithm>
#include <iostream>



/*- trsUnify::Unify --------------------------------------------------

	Risolve il problema dell'unificazione per la lista di coppie _l
	Mette in _unifier l'unificatore risultante
--------------------------------------------------------------------*/
void trsUnify::Unify(trsTermPairs &_l, trsSubst &_unifier)
	throw(trsUnifyException)
{
	trsTermPairs::iterator it;
	trsTermPair tp; //autodispose = true

	m_list.clear();

	// copio FISICAMENTE la lista (con autodipose a true) in modo da poterla 
	// maneggiare liberamente
	it = _l.begin();
	while (it != _l.end()) {
		trsTerm *pterm;
		//LEFT
		pterm = it->GetLeft();
		if(pterm->IsFunc())
			pterm = ((trsFunc*)(pterm))->Duplicate();
		tp.SetLeft(pterm);
		//RIGHT
		pterm = it->GetRight();
		if(pterm->IsFunc())
			pterm = ((trsFunc*)(pterm))->Duplicate();
		tp.SetRight(pterm);

		m_list.push_back(tp);
		
		tp.SetLeft(NULL);
		tp.SetRight(NULL);
		it++;
	}

	bool end;
	do
	{
		end=true;
		CheckDuplicates();
		Clash();
		OccursCheck();
		if(Decompose()) { end=false; PrintList();}
		if(Delete()) { end=false; PrintList();}
		if(Orient()) { end=false; PrintList();}
		if(Eliminate()) { end=false; PrintList();}
	} while(!end);

	if(m_list.empty())
		throw trsUnifyException(0,"Non esistono piu` coppie, unificazione terminata.");

	// copio il risultato in _unifier
	for (it=m_list.begin(); it != m_list.end(); it++)
		_unifier.AddSubPair((trsVar*)it->GetLeft(),it->GetRight());

	if (m_outdev) *m_outdev << "Unificazione avvenuta con successo.\n";
}



/*- trsUnify::Unify --------------------------------------------------

	Risolve l'unificazione per una sola coppia di termini
--------------------------------------------------------------------*/
void trsUnify::Unify(trsTermPair &_tp,trsSubst &_unifier) 
	throw(trsUnifyException)
{
	trsTermPairs l;
	trsTermPair tp(NULL,NULL,false);

	tp.SetLeft(_tp.GetLeft());
	tp.SetRight(_tp.GetRight());
	l.push_back(tp);
	Unify(l,_unifier);
}


/*- trsUnify::PrintMsg -----------------------------------------------

	Stanpa un messaggio nel fomato "_funcname sulla coppia _tp"
--------------------------------------------------------------------*/
void trsUnify::PrintMsg(const char* _funcname, trsTermPair &_tp)
{
	if (m_outdev) *m_outdev << _funcname <<" sulla coppia "<< _tp <<"\n";
}


/*- trsUnify::PrintList ----------------------------------------------

	Stampa sul device la lista di coppie attualmente in memoria
--------------------------------------------------------------------*/
void trsUnify::PrintList()
{
	if (m_outdev) *m_outdev << "Nuove coppie:\n" << m_list;
}


/*- trsUnify::Delete -------------------------------------------------

	Delete (pag.74 Term Rewriting and All That)
--------------------------------------------------------------------*/
bool trsUnify::Delete()
{
	trsTermPairs::iterator it;
	bool ret=false;

	it = m_list.begin();
	while (it != m_list.end()) {
		if ( *(it->GetLeft()) == *(it->GetRight())) {
			PrintMsg("Delete (1)",*it);
			m_list.erase(it++);
			ret = true;
		} else {
			it++;
		}
	}
	return ret;
}


/*- trsUnify::Decompose ----------------------------------------------

	Decompose (pag.74 Term Rewriting and All That)
--------------------------------------------------------------------*/
bool trsUnify::Decompose()
{
	trsTermPairs::iterator it;
	bool ret=false; 

	it=m_list.begin();
	while (it != m_list.end()) {
		
		// se uno dei due e` una variabile continua
		if(it->GetLeft()->IsVar() || it->GetRight()->IsVar()) {
			it++;
			continue;
		}
		// se hanno lo stesso simbolo
		if ( *it->GetLeft()->GetSim() == *it->GetRight()->GetSim() ) {
			ret = true;
			// se il destro e` una costante lo e` pure il sinistro
			if (it->GetRight()->IsConst()) {
				// equivale alla Delete su questa coppia
				PrintMsg("Delete (2)",*it);
				m_list.erase(it++);
				continue;
			}
			// altrimenti sono funzioni
			PrintMsg("Decompose (2)",*it);
			// crea n nuove coppie con gli n argomenti delle due funzioni
			trsTermPair tp;
			trsArgList::iterator arglit, argrit;
			trsTerm* argl = ((trsFunc*)(it->GetLeft()))->GetFirstArg(arglit);
			trsTerm* argr = ((trsFunc*)(it->GetRight()))->GetFirstArg(argrit);
			while(argl) {
				argl = argl->Duplicate();
				argr = argr->Duplicate();
				tp.SetLeft(argl);
				tp.SetRight(argr);
				if (!CheckDuplicate(tp)) {
					m_list.push_back(tp);
				} else {
					// sono duplicati, vanno eliminati
					if (argl->IsFunc()) delete argl;
					if (argr->IsFunc())	delete argr;
				}
				argl = ((trsFunc*)(it->GetLeft()))->GetNextArg(arglit);
				argr = ((trsFunc*)(it->GetRight()))->GetNextArg(argrit);
			}
			tp.SetLeft(NULL);
			tp.SetRight(NULL);
			// togli la vecchia coppia decomposta
			m_list.erase(it++);
			continue;
		}
		it++;
	} // while()
	return ret;
}


/*- trsUnify::Orient -------------------------------------------------

	Orient (pag.74 Term Rewriting and All That)
--------------------------------------------------------------------*/
bool trsUnify::Orient()
{
	trsTermPairs::iterator it;
	bool ret = false;

	for (it=m_list.begin(); it != m_list.end(); it++) {
		// se il termine di destra e` una variabile e quello di sinistra no
		// allora swappa
		if((it->GetRight()->IsVar()) && (!it->GetLeft()->IsVar())) {
			PrintMsg("Orient (3)",*it);
			it->Swap();
			ret = true;
		}
	}	
	return ret;
}



/*- trsUnify::Eliminate ----------------------------------------------

	Eliminate (pag.74 Term Rewriting and All That)
--------------------------------------------------------------------*/
bool trsUnify::Eliminate()
{
	trsTermPairs::iterator it,it2;
	trsSubst subst;
	bool ret=false;

	for (it=m_list.begin(); it != m_list.end(); it++) 
	{
		if (!it->GetLeft()->IsVar()) continue;
		//if (it->GetRight()->IsVar()) continue; //@@ e` corretto?
		// il sinistro e` per forza una variabile
		trsVar* vl = (trsVar*)(it->GetLeft());
		if (it->GetRight()->IsFunc()) {
			trsFunc* fr = (trsFunc*)(it->GetRight());
			if (fr->Occurs(vl)) {
				PrintMsg("Occurs-Check (6)",*it);
				throw trsUnifyException(0,"Occurs-Check: impossibile unificare.");
			}
		}
		subst.Reset();
		subst.AddSubPair(vl,it->GetRight());
		// cerca la variabile nella lista di coppie
		for (it2=m_list.begin(); it2 != m_list.end(); it2++) {
			// se stai passando sulla stessa coppia del primo ciclo saltala
			if (it == it2) continue;
			// riscrivi con la regola vr->ft
			string before, after;
			before = it2->GetLeft()->GetPrintable();
			before += "=";
			before += it2->GetRight()->GetPrintable();
			it2->SetRight(subst.TermApplySub(it2->GetRight()));
			it2->SetLeft(subst.TermApplySub(it2->GetLeft()));
			after = it2->GetLeft()->GetPrintable();
			after += "=";
			after += it2->GetRight()->GetPrintable();
			if (before != after) {
				ret=true;
				if (m_outdev)
					*m_outdev << "Eliminate (4) con " 
					<< vl->GetPrintable() << "->" << it->GetRight()->GetPrintable() 
					<< " su " << before << " : " << after <<"\n";
			}
		}
	}
	return ret;
}


/*- trsUnify::Clash --------------------------------------------------

	Clash (pag.77 Term Rewriting and All That)
--------------------------------------------------------------------*/
void trsUnify::Clash() throw(trsUnifyException)
{	
	trsTermPairs::iterator it;

	for (it=m_list.begin(); it != m_list.end(); it++) 
	{
		if( ((!it->GetRight()->IsVar()) && (!it->GetLeft()->IsVar()))
			&& (it->GetLeft()->GetSim() != it->GetRight()->GetSim())) 
		{
			PrintMsg("Clash (5)",*it);
			throw trsUnifyException(0,"Clash: impossibile unificare.");
		}
	}
}


/*- trsUnify::OccursCheck --------------------------------------------

	Occurs-Check (pag.77 Term Rewriting and All That)
--------------------------------------------------------------------*/
void trsUnify::OccursCheck() throw(trsUnifyException)
{
	trsTermPairs::iterator it;

	for (it=m_list.begin(); it != m_list.end(); it++) 
	{
		if(it->GetLeft()->IsVar() && it->GetRight()->IsFunc())
		{
			trsVar* vl = (trsVar*)(it->GetLeft());
			trsFunc* fr = (trsFunc*)(it->GetRight());
			if (fr->Occurs(vl)) {
				PrintMsg("Occurs-Check (6)",*it);
				throw trsUnifyException(0,"Occurs-Check: impossibile unificare.");
			}
		}
	}
}


/*- trsUnify::CheckDuplicate -----------------------------------------

	Ritorna true sse nella lista in memoria esistono duplicati di _tp 
--------------------------------------------------------------------*/
bool trsUnify::CheckDuplicate(trsTermPair &_tp)
{
	trsTermPairs::iterator it;

	for (it=m_list.begin(); it != m_list.end(); it++) 
	{
		if (*(it->GetLeft()) == *(_tp.GetLeft()))
			if (*(it->GetRight()) == *(_tp.GetRight()))
				return true;
	}
	return false;
}


/*- trsUnify::CheckDuplicates ----------------------------------------

	Elimina tutti i duplicati dalla lista di coppie
--------------------------------------------------------------------*/
void trsUnify::CheckDuplicates()
{
	trsTermPairs::iterator it,it2;
	
	it=m_list.begin();
	while (it != m_list.end()) {
		it2=m_list.begin();
		while (it2 != m_list.end()) 
		{
			if (it==it2) {
				it2++; 
				continue;
			}
			if (*(it2->GetLeft()) == *(it->GetLeft())) {
				if (*(it2->GetRight()) == *(it->GetRight())) {
					m_list.erase(it2++);
					continue;
				}
			}
			it2++;
		}
		it++;
	}
}


// EOF trsunify.cpp ///////////////////////////////////////////////////////////
