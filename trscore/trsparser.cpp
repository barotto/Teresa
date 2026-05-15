/*//////////////////////////////////////////////////////////////////////////////
	FILENAME: trsparser.cpp
	Version:  1.0.0
	Authors:  Alberto Minora, Marco Bortolin
	Created:  02/08/2001 - 14.39.17

	Implementazione del trsParser.
	Copyright (C) 2001 Marco Bortolin, Alberto Minora
//////////////////////////////////////////////////////////////////////////////*/
#ifdef WIN32
#pragma warning(disable:4786)
#pragma warning(disable:4100)
#endif
#include "trs.h"
#include <ctype.h>
#include <cstdio>
#include <algorithm>


/*- trsParser::Reset -------------------------------------------------

	Inizializza il parser per una nuova identita
--------------------------------------------------------------------*/
void trsParser::Reset(const char *_raw)
{
	m_readpos = 0;
	m_identity = _raw;
	m_leneq = strlen(_raw);
	m_algebra = NULL;
}


/*- trsParser::Error -------------------------------------------------

	Lancia una eccezione con un messaggio d'errore descrittivo
--------------------------------------------------------------------*/
void trsParser::Error(int _code) throw (trsParserException)
{
	string mex;
	int i;
	
	mex = m_identity;
	mex += "\r\n";
	for (i=0; i<m_readpos-1; i++) 
		mex += " ";
	mex += "^\r\n";
	switch (_code) {
		case PARS_EQUAL_NOT_FOUND:
			mex += "simbolo '=' atteso\r\n";
			break;
		case PARS_CLOSEPAR_NOT_FOUND:
			mex += "simbolo ')' atteso\r\n";
			break;
		case PARS_ALFANUM_NOT_FOUND:
			mex += "simbolo alfanumerico atteso\r\n";
			break;
		case PARS_IDENTITY_NOT_COMPLETE:
			mex += "identita` non completa\r\n";
			break;
		case PARS_END_EXPECTED:
			mex += "attesa fine identita`\r\n";
			break;
		case PARS_ALFAORPAR_NOT_FOUND:
			mex += "atteso alfanumerico o parentesi aperta\r\n";
			break;
		default:
			mex += "errore sconosciuto!\r\n";
	}
	throw trsParserException(_code,mex);
}


/*- trsParser::ReadSimbol --------------------------------------------

	Legge il prossimo simbolo dal buffer
--------------------------------------------------------------------*/
void trsParser::ReadSimbol() 
{
	if (m_readpos>m_leneq) Error(PARS_IDENTITY_NOT_COMPLETE);
	m_sim = m_identity[m_readpos];
	m_readpos++;
}

/*- trsParser::Equation ---------------------------------------------

	Riconosce una equazione
--------------------------------------------------------------------*/
void trsParser::Equation(trsTermPair *_pair) 
{
	trsTerm* trm;
	
	ReadSimbol();
	trm = Term();
	// se e` una costante o una varibile non lo cancello!
	if(_pair!=NULL) {
		_pair->SetLeft(trm);
	} else {
		if (trm->IsFunc())
			delete trm;
	}
	if (m_sim != '=') Error(PARS_EQUAL_NOT_FOUND);
	ReadSimbol();
	trm = Term();
	if(_pair!=NULL) {
		_pair->SetRight(trm);
	} else {
		if (trm->IsFunc())
			delete trm;
	}
	if (m_sim != '\0') Error(PARS_END_EXPECTED);
}


/*- trsParser::Term --------------------------------------------------

	Riconosce un termine
--------------------------------------------------------------------*/
trsTerm* trsParser::Term() 
{
	trsFunc* infixnode=NULL;
	trsFunc* previnfixnode=NULL;
	string   infixop;
	trsTerm* fact=NULL;
	trsSimArityPairs* sig;
	int pos;

	fact = Factor();	
	if (!IsInfixOperator(&infixop)) return fact;
	
	sig = &(m_algebra->m_signature);
		
	do {
		// aggiungo l'op. infisso nella tabella delle funzioni
		pos = sig->Insert(infixop.c_str(),2);

		// creo il termine che contiene l'operatore infisso
		string* sim = sig->GetSim(pos);
		infixnode = new trsFunc(sim,m_algebra);
		if (previnfixnode == NULL) {
			infixnode->AddArg(fact);
		} else {
			infixnode->AddArg(previnfixnode);
		}
		ReadSimbol();
		try {
			fact = Factor();
		} catch (...) {
			delete infixnode;
			throw;
		}
		infixnode->AddArg(fact);		
		// sincronizzo la stringa per ragioni di efficienza
		infixnode->GetString();
		previnfixnode = infixnode;
	} while (IsInfixOperator(&infixop));
	return infixnode;
}


/*- trsParser::MakeVar -----------------------------------------------

	Costruisce una variabile senza duplicarla nella tab. delle var.
--------------------------------------------------------------------*/
trsTerm *trsParser::MakeVar(string* _sim)
{
	trsVar* var;
		
	//si trova gia` nella tab. delle variabili?
	var = m_algebra->FindVar(_sim);
	if (var == NULL) {
		//la aggiungo nella tab.
		var = new trsVar(*_sim,m_algebra);
		m_algebra->m_variables.push_back(var);
	}
	return var;

}


/*- trsParser::MakeConst ---------------------------------------------

	Costruisce una costante senza duplicarla nella tab. delle cost.
--------------------------------------------------------------------*/
trsTerm *trsParser::MakeConst(string* _sim,trsSimArityPairs* _sig)
{
	string *simbol;
	trsConst *c;
	int pos;

	//inserisco il simbolo nella segnatura
	pos = _sig->Insert(_sim->c_str(),0);	
	simbol = _sig->GetSim(pos);
	
	//si trova gia` nella tab. delle costanti?
	c = m_algebra->FindConst(simbol);
	if (c == NULL) {
		//la aggiungo nella tab.
		c = new trsConst(simbol);
		m_algebra->m_constants.push_back(c);
	}
	return c;
}


/*- trsParser::Factor ------------------------------------------------

	Riconosce un fattore
--------------------------------------------------------------------*/
trsTerm *trsParser::Factor() 
{
	string simbol;
	trsSimArityPairs* sig;
	
	sig = &(m_algebra->m_signature);
		
	if (isupper(m_sim)) {
		//Variabile
		while (isalnum(m_sim)) {
			simbol += m_sim;
			ReadSimbol();
		}
		return MakeVar(&simbol);
	}
	else if (isdigit(m_sim)) {
		//Costante
		while (isalnum(m_sim)) {
			simbol += m_sim;
			ReadSimbol();
		}
		return MakeConst(&simbol,sig);
	} 
	else if (islower(m_sim)) {
		while (isalnum(m_sim)) {
			simbol += m_sim;
			ReadSimbol();
		}
		//Costante o Funzione?
		if (m_sim == '(') {
			//funzione
			// creo con NULL perche` non ho il simbolo nella segnatura!
			trsFunc *func = new trsFunc(NULL,m_algebra);
			ReadSimbol();
			try {
				ArgumentList(func,&simbol,sig);
			} catch (...) {
				delete func;
				throw;
			}
			if (m_sim != ')') {
				delete func;
				Error(PARS_CLOSEPAR_NOT_FOUND);
			}
			ReadSimbol();
			return func;
		} else {
			//costante
			return MakeConst(&simbol,sig);
		}
	}
	else if (m_sim == '(') {
		ReadSimbol();
		trsTerm *t = Term();
		if (m_sim != ')') Error(PARS_CLOSEPAR_NOT_FOUND);
		ReadSimbol();
		return t;
	}
	else {
		Error(PARS_ALFAORPAR_NOT_FOUND);
	}

	// qui non ci arrivera` mai
	return NULL;
}


/*- trsParser::ArgumentList ------------------------------------------

	Riconosce una lista di argomenti di funzione
--------------------------------------------------------------------*/
void trsParser::ArgumentList(trsFunc *_func, string *_sim, 
	trsSimArityPairs *_sig)
{
	trsTerm *arg;

	arg = Term();
	_func->AddArg(arg);
	while (m_sim == ',') {
		ReadSimbol();
		arg = Term();
		_func->AddArg(arg);
	}
	
	int pos = _sig->Insert(_sim->c_str(),_func->GetArieta());
			
	// imposto il simbolo della funzione
	string* simbol = _sig->GetSim(pos);
	_func->SetSimbol(simbol);
}


/*- trsParser::IsInfixOperator ---------------------------------------

	Riconosce un operatore infisso
--------------------------------------------------------------------*/
bool trsParser::IsInfixOperator(string *_op) 
{
	*_op = m_sim;

	return ((m_sim=='+')||(m_sim=='-')||(m_sim=='*')||(m_sim=='/')||
		(m_sim=='|')||(m_sim=='&')||(m_sim=='\\'));
}



//////////////////////////////////////////////////////////////////////
// INTERFACCIA PUBBLICA


/*- trsParser::GetPair -----------------------------------------------
	
	Data una stringa crea una coppia di termini controllando l'arieta`
	delle funzioni gia` presenti nella teoria tramite una 
	trsAlgebra (che eventualmente puo` essere aggiornata con la 
	nuova coppia di termini)
--------------------------------------------------------------------*/
void trsParser::GetPair(const char *_raw, trsTermPair* _outpair, 
	trsAlgebra *_outalgebra) throw (trsParserException, trsSAException)
{
	Reset(_raw);
	m_algebra = _outalgebra;
	Equation(_outpair);
}


/*- trsParser::GetPairs ----------------------------------------------

	Dato un insieme di stringhe restituisce un insieme di coppie di 
	termini e una trsSimArityPairs (simbolo di funzione, arieta`)
--------------------------------------------------------------------*/
void trsParser::GetPairs(const trsRawIds *_raw,	trsTermPairs *_outpairs, 
	trsAlgebra *_outalgebra) throw (trsParserException, trsSAException) 
{
	trsRawIds::const_iterator rawit;
	trsTermPair tp;

	rawit = _raw->begin();
	while (rawit != _raw->end()) {
		GetPair((*rawit).c_str(),&tp,_outalgebra);
		_outpairs->push_back(tp);
		rawit++;
	}
	tp.AutoDispose(false);
}



// EOF trsParser.cpp ///////////////////////////////////////////////////////////
