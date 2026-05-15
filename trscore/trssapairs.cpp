/*//////////////////////////////////////////////////////////////////////////////
	FILENAME: trssapairs.cpp
	Version:  1.0.0
	Author:   Alberto Minora, Marco Bortolin
	Created:  10/08/2001 - 16.47.05

	Copyright (C) 2001 Marco Bortolin, Alberto Minora
//////////////////////////////////////////////////////////////////////////////*/
#ifdef WIN32
#pragma warning(disable:4786)
#pragma warning(disable:4100)
#endif

#include "trs.h"
#include <algorithm>


/*- trsSimArityPairs::Insert -----------------------------------------
	Aggiunge una coppia (s,a) alla tabella controllando che la funzione 
	non sia gia` presente con arieta` differente, nel qual caso lancia 
	una eccezione. Ritorna la posizione della coppia inserita o di 
	quella eventualmente gia` presente, identica, nella lista.
--------------------------------------------------------------------*/
int trsSimArityPairs::Insert(const char *_sim, unsigned int _arity) 
	throw(trsSAException)
{	
	trsSimArityPair sa;
	int pos;
	
	sa.funcsim = _sim;
	sa.arity = _arity;
	if (m_tab.empty()) {
		m_tab.push_back(sa);
		return m_tab.size()-1;
	}
	m_it = m_tab.begin();
	pos = 0;
	while (m_it != m_tab.end()) {
		if (m_it->funcsim == _sim) {
			//controllo arieta`
			if ((*m_it).arity != _arity)
				throw trsSAException(0,
				"%s: funzione gia` dichiarata con arieta` differente (da %d a %d)\r\n",
				_sim, m_it->arity, _arity);
			else
				return pos;
		}
		pos++;
		m_it++;
	}
	m_tab.push_back(sa);
	return m_tab.size()-1;
}


/*- trsSimArityPairs::Find -------------------------------------------

	Trova una coppia (s,a). Ritorna la sua posizione o -1 se non presente 
--------------------------------------------------------------------*/
int trsSimArityPairs::Find(const char *_sim, int _arity)
{	
	int pos;
	
	pos=0;
	m_it = m_tab.begin();
	while (m_it != m_tab.end()) {
		if ((*m_it).funcsim == _sim) {
			//controllo arieta`
			if (_arity==-1)
				return(pos);
			if ((*m_it).arity != _arity)
				return(-1);
			else
				return(pos);
		}
		m_it++;
		pos++;
	}
	return(-1);
}


/*- trsSimArityPairs::GetSim -----------------------------------------

	Restituisce il simbolo della funzione identificata da _pos
--------------------------------------------------------------------*/
string* trsSimArityPairs::GetSim(int _pos)
{	
	if ((_pos < 0) || (_pos > GetSize())) return NULL;
	m_it = m_tab.begin();
	advance(m_it,_pos);
	return &m_it->funcsim;
}


/*- trsSimArityPairs::GetArity ---------------------------------------

	Restituisce l'arieta` della funzione identificata da _pos
--------------------------------------------------------------------*/
int trsSimArityPairs::GetArity(int _pos)
{	
	if ((_pos < 0) || (_pos > GetSize())) return(-1);
	m_it = m_tab.begin();
	advance(m_it,_pos);
	return m_it->arity;
}


// EOF trssapairs.cpp //////////////////////////////////////////////////////////
