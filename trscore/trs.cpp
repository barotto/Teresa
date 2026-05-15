/*//////////////////////////////////////////////////////////////////////////////
	FILENAME: trs.cpp
	Version:  1.0.0
	Author:   Marco Bortolin, Alberto minora
	Created:  05/08/2001 - 9.36.03

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



//////////////////////////////////////////////////////////////////////
// class trsStdDev

trsOutputDev& trsStdDev::operator <<(string _src)
{
	cout << _src;
	return *this;
}
trsOutputDev& trsStdDev::operator <<(const char *_src)
{
	cout << _src;
	return *this;
}
	
trsOutputDev& trsStdDev::operator <<(int _src)
{
	cout << _src;
	return *this;
}


/*- operator << ------------------------------------------------------

	Stampa su _dev una lista di numeri interi 
--------------------------------------------------------------------*/
trsOutputDev &operator<<(trsOutputDev &_dev, list<int> &_l)
{
	list<int>::iterator it;
	
	_dev << "{";
	if (!_l.empty()) {
		it = _l.begin();
		_dev << *it;
		it++;
		while (it != _l.end()) {
			_dev << "," << *it;
			it++;
		}
	}
	_dev << "}";
	return _dev;
}



/*- PrintTRS ---------------------------------------------------------

	Stampa sul device _dev il TRS _trs (ogni coppia nel formato l-->r)
--------------------------------------------------------------------*/
trsOutputDev &PrintTRS(trsOutputDev &_dev, trsTermPairs &_trs)
{
	trsTermPairs::iterator it;
	if (_trs.empty()) {
		_dev << "<vuoto>\n";
		return _dev;
	}
	for (it=_trs.begin(); it != _trs.end(); it++) {
		_dev << it->GetPrintable(true) << "\n";
	}
	return _dev;
}


//////////////////////////////////////////////////////////////////////
// class trsStdFBDev
// @@ completare l'implementazione
	
void trsStdFBDev::SetProgressRange(int _lower, int _upper)
{
	m_upper = _upper;
	m_lower = _lower;
	if (_lower > _upper)
		m_lower = m_upper;

}
	
void trsStdFBDev::SetProgressStep(int _step)
{
	m_step = _step;
	if (m_step > (m_upper-m_lower))
		m_step = m_upper-m_lower;
}
	
void trsStdFBDev::SetProgressPos(int _pos)
{
	m_pos = _pos;
	if (_pos > m_upper)
		m_pos = m_upper;
	if (_pos < m_lower)
		m_pos = m_lower;

	//@@
	cerr << ".";
}
	
void trsStdFBDev::StepProgress()
{
	/*
	if (m_pos == m_lower) {
		cerr << m_loop;
		m_loop++;
	}
	*/
	cerr << ".";
	m_pos += m_step;
	if (m_pos>m_upper)
		m_pos = m_lower;
}

void trsStdFBDev::Reset()
{
	m_loop = 0;
	m_pos = 0;
}

void trsStdFBDev::PrintInt(int _vec[], int _size)
{
	if (_vec == NULL) return;

	int i=0;
	while (i<_size) {
		cerr << _vec[i];
		if (++i<_size) cerr << ",";
	}
}	

void trsStdFBDev::PrintFuncName(const char* _str) 
{
	cerr << " '" << _str << "' ";
}

void trsStdFBDev::PrintStr(const char* _str)
{
	cerr << _str;
}


//////////////////////////////////////////////////////////////////////
// class trsAlgebra


/*- trsAlgebra::~trsAlgebra ------------------------------------------

	Distruttore
--------------------------------------------------------------------*/
trsAlgebra::~trsAlgebra()
{
	//@@
	//TRS_TRACE_START "~trsAlgebra()\n" TRS_TRACE_END

	list<trsVar*>::iterator varit;
	list<trsConst*>::iterator constit;

	varit = m_variables.begin();
	constit = m_constants.begin();
	while (varit != m_variables.end()) {
		delete *varit;
		varit++;
	}
	while (constit != m_constants.end()) {
		delete *constit;
		constit++;
	}
}


/*- *trsAlgebra::FindVar ---------------------------------------------

	Trova una variabile nella lista delle variabili
--------------------------------------------------------------------*/
trsVar *trsAlgebra::FindVar(const string *_str, int _id)
{
	list<trsVar*>::iterator it;

	Assert(_str,trsException(TRSERR_NULLPTR,
		"trsAlgebra::FindVar(): NULL argument"));

	it = m_variables.begin();
	while (it != m_variables.end()) {
		if ( (*(*it)->GetSim() == *_str) && ((*it)->m_id == _id) )
			return *it;
		it++;
	}
	return NULL;
}


/*- *trsAlgebra::FindConst -------------------------------------------

	Trova una costante nella lista delle costanti
--------------------------------------------------------------------*/
trsConst *trsAlgebra::FindConst(const string *_str)
{
	list<trsConst*>::iterator it;
	
	Assert(_str,trsException(TRSERR_NULLPTR,
		"trsAlgebra::FindConst(): NULL argument"));

	it = m_constants.begin();
	while (it != m_constants.end()) {
		if (*(*it)->GetSim() == *_str)
			return *it;
		it++;
	}
	return NULL;

}

/*- *trsAlgebra::AddVar ----------------------------------------------

	Aggiunge una nuova variabile (id=0) alla lista delle variabili 
	con simbolo	_sim; retituisce la variabile creata o il puntatore 
	a quella eventualmente gia` presente.
--------------------------------------------------------------------*/
trsVar *trsAlgebra::AddVar(const string *_sim)
{
	Assert(_sim,trsException(TRSERR_NULLPTR,
	"trsAlgebra::AddVar(): NULL argument"));

	// cerco la variabile, se esiste, con lo stesso simbolo 
	trsVar* var0 = FindVar(_sim,0);
	// se gia` esiste restituiscila
	if (var0 != NULL)
		return var0;

	// aggiungo una nuova variabile con simbolo _sim e indice 0
	trsVar* newvar = new trsVar(*_sim,this);
	m_variables.push_back(newvar);
	return newvar;
}


/*- trsAlgebra::AddRenamedVar ----------------------------------------

	Aggiunge una nuova variabile alla lista delle variabli con lo 
	stesso simbolo di _srcvar e indice diverso e unico in tutta la lista
	per quel simbolo. Ritorna un puntatore alla variabile aggiunta. 
--------------------------------------------------------------------*/
trsVar *trsAlgebra::AddRenamedVar(trsVar *_srcvar)
{
	Assert(_srcvar,trsException(TRSERR_NULLPTR,
		"trsAlgebra::AddRenamedVar(): NULL argument"));

	// cerco la variabile di indice 0 per conoscere il giusto indice 
	// per il renaming
	trsVar* var0 = FindVar(_srcvar->GetSim(),0);
	// se NULL mi hanno passato una variabile di un algebra diversa.
	if (var0==NULL) {
		// la variabile deve appartenere a questa algebra
		return NULL;
	}
	// aggiungo una nuova variabile con lo stesso simbolo di _srcvar e indice
	// uguale alla variabile rinominata di indice piu' alto +1 
	trsVar* newvar = new trsVar(*_srcvar->GetSim(),var0->m_maxid+1,this);
	m_variables.push_back(newvar);
	var0->m_maxid += 1;
	return newvar;
}


/*- trsAlgebra::CleanDupVars -----------------------------------------

	Cancella tutte le variabili rinominate.
	Attenzione ad un suo utilizzo improprio (le variabili potrebbero 
	essere usate in qualche termine)!
	@@ suggerimento: implementare il reference counting per le variabili
--------------------------------------------------------------------*/
void trsAlgebra::CleanDupVars()
{
	trsVarList::iterator it;

	it=m_variables.begin();
	while (it != m_variables.end()) {
		if ((*it)->m_id > 0) {
			delete *it;
			m_variables.erase(it++);
			continue;
		} else {
			(*it)->m_maxid = 0;
		}
		it++;
	}
}


// EOF trs.cpp /////////////////////////////////////////////////////////////////
