/*//////////////////////////////////////////////////////////////////////////////
	FILENAME: trslpoorder.cpp
	Version:  1.0.0
	Authors:  Alberto Minora, Marco Bortolin
	Created:  11/08/2001 - 15.50.00

	Implementazione di trsLPOOrder.
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



/*- trsLPOOrder::trsLPOOrder -----------------------------------------

	Costruttore
--------------------------------------------------------------------*/
trsLPOOrder::trsLPOOrder()
{
	m_readpos = 0;
	SetDevices(NULL,NULL);
}


/*- trsLPOOrder::SetNewOrder -----------------------------------------

	Imposta un nuovo ordinamento
--------------------------------------------------------------------*/
void trsLPOOrder::SetNewOrder(const char* _ordstr, 
	trsSimArityPairs* _segnatura) throw(trsOrderException)
{
	string tok;
	char op;
	int i;
	
	Assert(_segnatura,trsException(TRSERR_NULLPTR,
		"trsLPOOrder::SetNewOrder(): NULL argument"));

	// la vecchia prec list non serve piu`
	m_preclist.clear();
	// nemmeno la funcpos 
	m_funcpos.clear();

	m_segnatura = _segnatura;
	m_ordstr = _ordstr;
	for (i=0;i<m_segnatura->GetSize();i++)
		m_funcpos.push_back(i);
	
	// non e` stata definita una precedenza percio` considero tutte le 
	// funzioni come aventi la stessa precedenza.	
	if (m_ordstr.empty()) {
		AddRemaining();
		return;
	}
	// zompo via gli spazi
	string::size_type pos = 0;
	while ( (pos = m_ordstr.find(' ')) != m_ordstr.npos )
		m_ordstr.erase(pos);

	m_readpos = 0;
	ReadSimbol();
	tok = Token();
	// aggiungo il primo nodo
	AddToken(tok,'>');
	while (m_sim != '\0') {
		op = PrecedenceOp();
		tok = Token();
		AddToken(tok,op);
	}
	AddRemaining();
}


/*- trsLPOOrder::CompareTerm -----------------------------------------

	Confronta due termini nel modo indicato da _mode
--------------------------------------------------------------------*/
bool trsLPOOrder::CompareTerm(trsTerm* _s, trsTerm* _t, int _mode)
{
	Assert((_s&&_t),trsException(TRSERR_NULLPTR,
		"trsLPOOrder::CompareTerm(): NULL argument"));

	switch (_mode) {
		case ORD_EQ:
			if (*_s == *_t)
				return true;
			break;
		case ORD_GR:
			try {
				if (Greater(_s,_t))
					return true;
			} catch(trsOrderException E) {
				if (m_outdev)
					*m_outdev << "(" << _s->GetPrintable() << "," 
						<< _t->GetPrintable() << ") " << E.GetMsg() << "\n";
			}
			break;
		case ORD_NGE:
			try {
				if (!Greater(_s,_t))
					if (!(*_s == *_t))
						return true;
			} catch(trsOrderException E) {
				if (m_outdev)
					*m_outdev << "(" << _s->GetPrintable() << "," 
						<< _t->GetPrintable() << ") " << E.GetMsg() << "\n";
				return true;
			}
			break;
	}
	return false;
}


/*- trsLPOOrder::CompareTermList -------------------------------------

	Confronta due liste di termini nel modo indicato da _mode e in
	caso di fallimento restituisce la coppia di termini che non
	verifica il confronto in _outs e _outt.
	Si assume che le liste di termini non siano vuote.
--------------------------------------------------------------------*/
bool trsLPOOrder::CompareTermList(trsArgList &_s,trsArgList &_t,
		int _mode, trsTerm* &_outs, trsTerm* &_outt)
{
	list<trsTerm*>::iterator its,ends;
	list<trsTerm*>::iterator itt,endt;
	
	its = _s.begin();
	its++;
	ends = _s.end();
	itt = _t.begin();
	itt++;
	endt = _t.end();
	
	while ((its != ends) || (itt != endt)) {
		_outs = *(--its);
		_outt = *(--itt);
		its++;
		itt++;
		if (!CompareTerm(_outs,_outt,_mode))
			return false;
		if (its != ends) its++;
		if (itt != endt) itt++;
	}
	_outs = *(--its);
	_outt = *(--itt);
	if (!CompareTerm(_outs,_outt,_mode))
		return false;
	return true;
}


/*- trsLPOOrder::CompareTermList -------------------------------------

	Confronta una lista di termini con un termine nel modo indicato 
	da _mode.
	Si assume che la liste di termini non sia vuota.
--------------------------------------------------------------------*/
bool trsLPOOrder::CompareTermList(trsArgList &_s, trsTerm *_t, int _mode)
{
	trsTerm *fake1, *fake2;

	Assert(((!_s.empty())&&_t),trsException(TRSERR_NULLPTR,
		"trsLPOOrder::CompareTermList(): NULL argument"));

	list<trsTerm*> l;
	l.push_back(_t);
	return (CompareTermList(_s,l,_mode,fake1,fake2));
}


/*- trsLPOOrder::CompareTermList -------------------------------------

	Confronta un termine con una lista di termini nel modo indicato 
	da _mode.
	Si assume che la liste di termini non sia vuota.
--------------------------------------------------------------------*/
bool trsLPOOrder::CompareTermList(trsTerm* _s, trsArgList &_t, int _mode)
{
	trsTerm *fake1, *fake2;
	list<trsTerm*> l;

	Assert(((!_t.empty())&&_s),trsException(TRSERR_NULLPTR,
		"trsLPOOrder::CompareTermList(): NULL argument"));

	l.push_back(_s);
	return (CompareTermList(l,_t,_mode,fake1,fake2));
}


/*- trsLPOOrder::Precede --------------------------------------------

	Determina se il termine _s precede _t nell'ordinamento corrente
	Naturalmente devono esistere nella lista delle precedenze!!! 
	Me lo assicura la chiamata a SetNewOrder().
--------------------------------------------------------------------*/
bool trsLPOOrder::Precede(trsTerm* _s, trsTerm* _t)
{
	list<node>::iterator nodeit;
		
	Assert((_s&&_t),trsException(TRSERR_NULLPTR,
		"trsLPOOrder::Precede(): NULL argument"));

	// se sono uguali evito di chiamare le due find()
	if (*(_s->GetSim()) == *(_t->GetSim()))
		return false;
		
	nodeit = m_preclist.begin();
	while (nodeit != m_preclist.end()) {
		bool sfound = (find(nodeit->begin(),nodeit->end(),*(_s->GetSim())) 
			!= nodeit->end());
		bool tfound = (find(nodeit->begin(),nodeit->end(),*(_t->GetSim())) 
			!= nodeit->end());
		if (sfound && tfound) return false;
		if (sfound) return true;
		if (tfound) return false;
		nodeit++;
	}

	// qui non ci arrivera` mai
	return false;
}


/*- trsLPOOrder::SamePrec --------------------------------------------

	Determina se i termini _s e _t hanno la stessa precedenza
--------------------------------------------------------------------*/
bool trsLPOOrder::SamePrec(trsTerm *_s, trsTerm *_t)
{ 
	list<node>::iterator nodeit;
		
	Assert((_s&&_t),trsException(TRSERR_NULLPTR,
		"trsLPOOrder::SamePrec(): NULL arguments"));

	// se sono uguali evito di chiamare le due find()
	if (*(_s->GetSim()) == *(_t->GetSim()))
		return true;
	
	nodeit = m_preclist.begin();
	while (nodeit != m_preclist.end()) {
		bool sfound = (find(nodeit->begin(),nodeit->end(),*(_s->GetSim())) 
			!= nodeit->end());
		bool tfound = (find(nodeit->begin(),nodeit->end(),*(_t->GetSim())) 
			!= nodeit->end());
		if (sfound && tfound) return true;
		if (sfound || tfound) return false;
		nodeit++;
	}

	// qui non ci arrivera` mai
	return false;
}


/*- trsLPOOrder::PrintOrdMsg -----------------------------------------


--------------------------------------------------------------------*/
void trsLPOOrder::PrintOrdMsg(trsTerm *_s, trsTerm *_t, const char* _rule)
{
	*m_outdev << _s->GetPrintable() << " > " << _t->GetPrintable() 
	<< " per " << _rule << "\n";
}

/*- trsLPOOrder::PrintNotOrdMsg --------------------------------------


--------------------------------------------------------------------*/
void trsLPOOrder::PrintNotOrdMsg(trsTerm *_s, trsTerm *_t, const char* _msg)
{
	*m_outdev << _s->GetPrintable() << " non > di  " << _t->GetPrintable() 
	<< " : " << _msg << "\n";
}


/*- trsLPOOrder::Greater ---------------------------------------------

	Vero sse _s >lpo _t
--------------------------------------------------------------------*/
bool trsLPOOrder::Greater(trsTerm *_s, trsTerm *_t) throw(trsOrderException)
{
	trsFunc *fs, *ft;
	
	Assert((_s&&_t),trsException(TRSERR_NULLPTR,
		"trsLPOOrder::Greater(): NULL arguments"));
	
	if (m_feedback->Stop())
		throw trsException(TRSERR_USERSTOP,"Interruzione forzata.");

	if (m_preclist.empty()) return false;
	
	// LPO 1
	if (_t->IsVar()) {
		if (_s->IsVar())
			if (*_t == *_s) {
				if (m_outdev) PrintNotOrdMsg(_s,_t,"variabili uguali");
				return false;
			} else {
				throw trsOrderException(ORD_NOT_COMPARABLES,
					"Termini non confrontabili");
			}
		if (_s->IsConst())
			throw trsOrderException(ORD_NOT_COMPARABLES,
					"Termini non confrontabili");
		
		bool res = ((trsFunc*)_s)->Occurs((trsVar*)_t);
		if (m_outdev)
			if (res)
				PrintOrdMsg(_s,_t,"LPO 1");
			else
				PrintNotOrdMsg(_s,_t,"la variabile non occorre nella funzione");
		return res;
	}
	
	// se _s e` una variabile allora i termini non sono confrontabili
	if (_s->IsVar())
		throw trsOrderException(ORD_NOT_COMPARABLES, 
			"Termini non confrontabili");

	// LPO 2.1
	if (_s->IsFunc()) {
		// per ogni argomento della funzione _s
		// mi chiedo se e` >=lpo rispetto a _t
		if (m_outdev)
			*m_outdev << "esiste un argomento della funzione " 
				<< _s->GetPrintable() << " che e` >=lpo rispetto a "
				<< _t->GetPrintable() << " ?\n";
		fs = (trsFunc*)_s;
		if (!CompareTermList(*(fs->GetArgs()),_t,ORD_NGE)) {
			if (m_outdev) PrintOrdMsg(_s,_t,"LPO 2.1");
			return true;
		}
	}

	// LPO 2.2
	// _s > _t ?
	if (Precede(_s,_t)) {
		// _s >lpo di tutti gli argomenti della funzione _t?
		if (m_outdev)
			*m_outdev << _s->GetPrintable() 
				<< " >lpo di tutti gli argomenti di "
				<< _t->GetPrintable() << " ?\n";
		if (_t->IsConst()) { 
			if (m_outdev) PrintOrdMsg(_s,_t,"LPO 2.2");
			return true;
		}
		ft = (trsFunc*)_t;
		if (CompareTermList(_s,*(ft->GetArgs()),ORD_GR)) {
			if (m_outdev) PrintOrdMsg(_s,_t,"LPO 2.2");
			return true;
		}
	}

	// LPO 2.3
	// se _s ha la stessa precedenza di _t
	if (SamePrec(_s,_t)) {
		if(_s->IsConst()) {
			if (m_outdev) PrintNotOrdMsg(_s,_t,"ovvio");
			return false;
		}
		if(_t->IsConst()) {
			if (m_outdev) PrintOrdMsg(_s,_t,"LPO 2.3");
			return true;
		}
		fs = (trsFunc*)_s;
		ft = (trsFunc*)_t;
		if (fs->GetArieta() < ft->GetArieta()) {
			if (m_outdev) PrintNotOrdMsg(_s,_t,"arieta` inferiore");
			return false;
		}
		if (fs->GetArieta() > ft->GetArieta()) {
			if (m_outdev) PrintOrdMsg(_s,_t,"LPO 2.3");
			return true;
		}
		
		// _s >lpo di tutti gli argomenti della funzione _t?
		if (m_outdev)
			*m_outdev << _s->GetPrintable() 
				<< " >lpo di tutti gli argomenti della funzione " 
				<< _t->GetPrintable() << " ?\n";
		if (!CompareTermList(_s,*(ft->GetArgs()),ORD_GR))
			return false;
		trsTerm *si,*tj;
		if (m_outdev)
			*m_outdev << "cerco il primo argomento di " << _s->GetPrintable() 
				<< " >lpo del corrispondente argomento di " 
				<< _t->GetPrintable() << "\n";
		if (CompareTermList(*(fs->GetArgs()),*(ft->GetArgs()),ORD_EQ,si,tj))
			return false;
		try {
			if (!Greater(si,tj))
				return false;
			if (m_outdev) PrintOrdMsg(_s,_t,"LPO 2.3");
			return true;
		} catch (trsOrderException E) {
			if (m_outdev)
				*m_outdev << "(" << si->GetPrintable() << "," 
					<< tj->GetPrintable() << ") " << E.GetMsg() << "\n";
			return false;
		}
	}
	// se _t > _s allora non rientra nei casi precedenti
	return false;
}


/*- trsLPOOrder::Less ------------------------------------------------


--------------------------------------------------------------------*/
bool trsLPOOrder::Less(trsTerm *_s, trsTerm *_t) throw(trsOrderException)
{
	Assert((_s&&_t),trsException(TRSERR_NULLPTR,
		"trsLPOOrder::Less(): NULL argument"));

	if (m_preclist.empty()) return false;

	if (Greater(_t,_s))
		return true;
	return false;
}


/*- trsLPOOrder::Equal -----------------------------------------------


--------------------------------------------------------------------*/
bool trsLPOOrder::Equal(trsTerm *_s, trsTerm *_t) throw(trsOrderException)
{
	Assert((_s&&_t),trsException(TRSERR_NULLPTR,
		"trsLPOOrder::Equal(): NULL arguments"));

	if (m_preclist.empty()) return false;

	return (*_s == *_t);
}


/*- trsLPOOrder::AddRemaining ----------------------------------------

	Aggiunge le funzioni rimanenti alla struttura dati delle precedenze
--------------------------------------------------------------------*/
void trsLPOOrder::AddRemaining()
{
	list<int>::iterator intit;
	string func;
	
	if (m_funcpos.empty())
		return;

	if (!m_ordstr.empty())
		m_ordstr += ">";

	node newfunc;
	func = *m_segnatura->GetSim(*m_funcpos.begin());
	newfunc.push_back(func);
	m_ordstr += func;
	
	m_preclist.push_back(newfunc);
	m_funcpos.pop_front();
	intit = m_funcpos.begin();
	while (intit != m_funcpos.end()) {
		func = *m_segnatura->GetSim(*intit);
		//m_preclist.back().push_back(func);
		//m_ordstr += "=";
		newfunc.clear();
		newfunc.push_back(func);
		m_preclist.push_back(newfunc);
		m_ordstr += ">";
		m_ordstr += func;
		intit++;
	}
	m_funcpos.clear();
}


/*- trsLPOOrder::AddToken --------------------------------------------

	Aggiunge un token alla lista delle precedenze
--------------------------------------------------------------------*/
void trsLPOOrder::AddToken(string &_tok, char _op)
{	
	list<int>::iterator intit;
	int pos;

	// se mi passano la stringa vuota allora mi chiedono di aggiungere
	// un token che non esiste!
	if (_tok.empty()) Error(ORD_ALFANUM_NOT_FOUND,NULL);
	
	// cerco nella segnatura il token, se non lo trovo allora errore
	pos = m_segnatura->Find(_tok.c_str());
	if(pos < 0) {
		// sposto la testina di lettura all'inizio del token
		m_readpos = m_tokenpos;
		Error(ORD_BAD_FUNCNAME,_tok.c_str());
	}
	// se la funzione e` gia` stata inserita allora errore
	intit = find(m_funcpos.begin(),m_funcpos.end(),pos);
	if (intit == m_funcpos.end()) {
		// sposto la testina di lettura all'inizio del token
		m_readpos = m_tokenpos;
		Error(ORD_DUP_FUNCNAME,_tok.c_str());
	}	
	// inserisco il token nella preclist nel punto determinato da _op
	if (_op == '>') {
		node newfunc;
		newfunc.push_back(_tok);
		m_preclist.push_back(newfunc);
	} 
	if (_op == '=') {
		// almeno un elemento ci deve essere
		m_preclist.back().push_back(_tok);
	}
	intit = find(m_funcpos.begin(),m_funcpos.end(),pos);
	m_funcpos.erase(intit);	
}


/*- trsLPOOrder::Token -----------------------------------------------

	Riconosce un token
--------------------------------------------------------------------*/
string trsLPOOrder::Token()
{
	string str;

	// salvo la posizione della testina all'inizio del token
	m_tokenpos = m_readpos;
	
	while ((m_sim != '>') && (m_sim != '=') && (m_sim != '\0')) {
		str += m_sim;
		ReadSimbol();
	}
	return str;
}


/*- trsLPOOrder::PrecedenceOp ----------------------------------------

	Riconosce un operatore di precedenza
--------------------------------------------------------------------*/
char trsLPOOrder::PrecedenceOp()
{
	char op;

	op = m_sim;
	if ((m_sim == '>') || (m_sim == '=')) {
		ReadSimbol();
		return op;
	} else {
		Error(ORD_PRECOP_NOT_FOUND,NULL);
	}

	// qui non ci arrivera` mai
	return false;
}


/*- trsLPOOrder::ReadSimbol ------------------------------------------

	Legge il simbolo corrente dalla stringa
--------------------------------------------------------------------*/
void trsLPOOrder::ReadSimbol()
{
	m_sim = m_ordstr[m_readpos];
	m_readpos++;
}


/*- trsLPOOrder::Error -----------------------------------------------

	Lancia una eccezione
--------------------------------------------------------------------*/
void trsLPOOrder::Error(int _code, const char* _sim) throw(trsOrderException)
{
	string mex;
	int i;
	
	mex = m_ordstr;
	mex += "\r\n";
	for (i=0; i<m_readpos-1; i++) 
		mex += " ";
	mex += "^\r\n";
	switch (_code) {
		case ORD_ALFANUM_NOT_FOUND:
			mex += "Simbolo di funzione atteso\r\n";
			break;
		case ORD_PRECOP_NOT_FOUND:
			mex += "Simbolo di precedenza atteso\r\n";
			break;
		case ORD_BAD_FUNCNAME:
			mex += "La funzione \"";
			mex += _sim;
			mex += "\" non appartiene alla segnatura\r\n";			
			break;			
		case ORD_DUP_FUNCNAME:
			mex += "Funzione \"";
			mex += _sim;
			mex += "\" duplicata\r\n";
			break;
		default:
			mex += "errore sconosciuto!\r\n";
	}
	throw trsOrderException(_code,mex);
}


// EOF trslpoorder.cpp //////////////////////////////////////////////////////////
