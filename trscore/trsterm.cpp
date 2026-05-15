/*//////////////////////////////////////////////////////////////////////////////
	FILENAME: trsterm.cpp
	Version:  1.0.0
	Author:   Marco Bortolin, Alberto Minora
	Created:  04/08/2001 - 14.14.25

	Copyright (C) 2001 Marco Bortolin, Alberto Minora
//////////////////////////////////////////////////////////////////////////////*/
#ifdef WIN32
#pragma warning(disable:4786)
#pragma warning(disable:4100)
#endif
#include "trs.h"


///////////////////////////////////////////////////////////////////////
// class trsFunc


/*- trsFunc::GetPosTerm ----------------------------------------------

	Restituisce un puntatore al termine nella posizione specificata
	dalla lista in ingresso
--------------------------------------------------------------------*/
trsTerm *trsFunc::GetPosTerm(list<int> &_l)
{
	list<int>::iterator iti;
	trsArgList::iterator ita;
	trsFunc *t;
	
	t = this;
	iti = _l.begin();
	while (iti != _l.end()) {
		ita = t->GetArgs()->begin();
		
		Assert( ((*iti>=0)&&(*iti<t->GetArieta())), 
			trsException(TRSERR_OUTOFBOUND,
			"GetPosTerm(): invalid position %d",*iti) );
		
		advance(ita,*iti);
		if (++iti == _l.end())
			return *ita;

		Assert( ((*ita)->IsFunc()), trsException(TRSERR_BADTERM,
			"GetPosTerm(): invalid position, is not a trsFunc") );
		
		t = (trsFunc*) *ita;
	}
	// la lista e` per forza vuota
	return this;
}


/*- trsFunc::Duplicate -----------------------------------------------

	Crea un duplicato di questa funzione
--------------------------------------------------------------------*/
trsTerm* trsFunc::Duplicate()
{
	trsFunc* dupfunc;
	trsTerm* arg;
	trsArgList::iterator argit;

	dupfunc = new trsFunc(m_sim, m_algebra);
	
	arg = GetFirstArg(argit);
	while (arg != NULL) {
		//se e` una funzione la duplico e imposto il father
		if (arg->IsFunc()) {
			arg = ((trsFunc*) arg)->Duplicate();
			((trsFunc*) arg)->m_father = dupfunc;
		}
		dupfunc->AddArg(arg);
		arg = GetNextArg(argit);
	}
	//sincronizzo la stringa del duplicato
	dupfunc->SyncStr();
	return dupfunc;
}


/*- trsFunc::Occurs --------------------------------------------------

	Determina se una variabile occorre negli argomenti 
--------------------------------------------------------------------*/
bool trsFunc::Occurs(trsVar* _t)
{
	trsTerm* arg;
	trsArgList::iterator argit;

	arg = GetFirstArg(argit);
	Assert(arg!=NULL,trsException(TRSERR_NULLPTR,"trsFunc::Occurs(): empty arglist"));
	do {
		if (arg->IsVar())
			if (*arg == *_t)
				return true;
		if (arg->IsFunc())
			if (((trsFunc*)arg)->Occurs(_t))
				return true;
		arg = GetNextArg(argit);
	} while (arg != NULL);
	return false;
}


/*- trsFunc::AddArg --------------------------------------------------

	Aggiunge un argomento in coda alla funzione
--------------------------------------------------------------------*/
void trsFunc::AddArg(trsTerm *_arg)
{
	if (_arg->IsFunc()) {
		((trsFunc*) _arg)->m_father = this;
		((trsFunc*) _arg)->m_syncstr = true;
	}

	m_args.push_back(_arg);
	SetSyncstr();
}


/*- trsFunc::SubstArg ------------------------------------------------

	Sostituisce un argomento
--------------------------------------------------------------------*/
void trsFunc::SubstArg(int _pos, trsTerm *_argnew)
{
	trsArgList::iterator it;
	
	Assert(_argnew,trsException(TRSERR_NULLPTR,
		"trsFunc::SubstArg(): NULL argument"));
	Assert( ((_pos>=0)&&(_pos<GetArieta())), 
		trsException(TRSERR_OUTOFBOUND,
		"trsFunc::SubstArg(): invalid position %d (max %d)",_pos,GetArieta()) );
	
	SetSyncstr();

	it = m_args.begin();
	advance(it,_pos);
	if (*it == _argnew) return;
	if ((*it)->IsFunc())
		delete *it;
	*it = _argnew;	
	if((*it)->IsFunc()) {
		((trsFunc*)(*it))->m_father = this;
		// non serve chiamare SetSyncstr poiche` il suo father sono io.
		((trsFunc*)(*it))->m_syncstr = true;
	}
}


/*- trsFunc::GetString -----------------------------------------------

	Restituisce una stringa che, passata al parser, mi consente di 
	riottenere la medesima funzione.
--------------------------------------------------------------------*/
const string *trsFunc::GetString()
{
	if (m_syncstr)
		SyncStr();
	return &m_string;
}


/*- trsFunc::GetPattern ----------------------------------------------


--------------------------------------------------------------------*/
string trsFunc::GetPattern(trsVarList &_l,bool _sync)
{
	if (!m_pattern.empty())
		if (!_sync)
			return m_pattern;
	
	trsArgList::iterator it;
	m_pattern = *GetSim();
	m_pattern += "(";
	for (it=m_args.begin(); it != m_args.end(); it++) {
		Assert(*it != NULL,trsException(0,"NULL m_sim"));
		if ((*it)->IsConst())
			m_pattern += *(*it)->GetSim();
		if ((*it)->IsFunc())
			m_pattern += ((trsFunc*) *it)->GetPattern(_l,_sync);
		if ((*it)->IsVar()) {
			trsVarList::iterator vit;
			int n = 0;
			for (vit=_l.begin(); vit != _l.end(); vit++,n++)
				if (**vit == **it)
					break;
			
			char num[12];
			if (vit == _l.end())
				_l.push_back((trsVar*) *it);
			sprintf(num,"_%d",n);
			m_pattern += num;
		}
		m_pattern += ",";
	}
	//tolgo l'ultima virgola
	m_pattern.erase((m_pattern.end())-1);
	m_pattern += ")";
	return m_pattern;
}


/*- trsFunc::GetPrintable --------------------------------------------

	Restituisce una stringa adatta ad essere stampata su dispositivo 
	di output.
--------------------------------------------------------------------*/
string trsFunc::GetPrintable()
{
	string s;
	
	if (m_sim == NULL)
		return "<null>";

	if (IsInfixOp()) {
		if (m_father) 
			if (m_father->IsInfixOp()) s += "(";
		s += m_args.front()->GetPrintable();
		s += *GetSim();
		s += m_args.back()->GetPrintable();
		if (m_father) 
			if (m_father->IsInfixOp()) s += ")";
		return s;
	}
		
	trsArgList::iterator it;
	s = *GetSim();
	s += "(";
	for (it=m_args.begin(); it != m_args.end(); it++) {
		if (*it != NULL) {
			s += (*it)->GetPrintable();
		} else {
			s += "<null>";
		}
		s += ",";
	}
	//tolgo l'ultima virgola
	s.erase((s.end())-1);
	s += ")";
	return s;
}


/*- trsFunc::trsFunc -------------------------------------------------

	Costruttore
--------------------------------------------------------------------*/
trsFunc::trsFunc(string *_sim, trsAlgebra* _algebra) 
{
	m_sim = _sim;
	m_syncstr = true;
	m_father = NULL;
	m_algebra = _algebra;
}


/*- trsFunc::trsFunc -------------------------------------------------

	Distruttore
--------------------------------------------------------------------*/
trsFunc::~trsFunc() 
{
	trsArgList::iterator it;
	
	//TRS_TRACE_START "~trsFunc()\n" TRS_TRACE_END

	it = m_args.begin();
	while (it != m_args.end()) {
		if (*it != NULL) {
			if ((*it)->IsFunc())
				delete *it;
		}
		it++;
	}
}


/*- trsFunc::GetFirstArg ---------------------------------------------

	Restituisce il primo argomento. Imposta _it da passare a 
	GetNextArg per ottenere gli gli argomenti successivi.
--------------------------------------------------------------------*/
trsTerm	*trsFunc::GetFirstArg(trsArgList::iterator &_it) 
{
	if (m_args.empty()) return NULL;
	_it = m_args.begin();
	return *_it;
}


/*- trsFunc::GetNextArg ----------------------------------------------

	Restituisce il prossimo argomento. Da chiamare in un ciclo iniziato
	con GetFirstArg con lo stesso _it.
--------------------------------------------------------------------*/
trsTerm	*trsFunc::GetNextArg(trsArgList::iterator &_it)
{
	advance(_it,1);
	if (_it == m_args.end()) return NULL;
	return *_it;
}


/*- trsFunc::SetSyncstr ----------------------------------------------

	Pone a true il proprio m_syncstr e quello del proprio padre
--------------------------------------------------------------------*/
void trsFunc::SetSyncstr()
{
	m_syncstr = true;
	if (m_father != NULL)
		m_father->SetSyncstr();
}


/*- trsFunc::SyncStr -------------------------------------------------

	Sincronizza (costruisce) le stringhe (con i suoi sottotermini) 
--------------------------------------------------------------------*/
void trsFunc::SyncStr()
{
	Assert(m_sim != NULL,trsException(0,"trsFunc::SyncStr() : NULL m_sim"));
	
	m_string.erase();
	m_cmpstr.erase();

	if (IsInfixOp()) {
		if (m_father != NULL) 
			if (m_father->IsInfixOp()) m_string += "(";
		m_string += *m_args.front()->GetString();
		m_string += *GetSim();
		m_string += *m_args.back()->GetString();
		if (m_father != NULL) 
			if (m_father->IsInfixOp()) m_string += ")";
	} else {
		//@@ sistemare: fondere con m_cmpstr
		trsArgList::iterator it;
		m_string = *GetSim();
		m_string += "(";
		for (it=m_args.begin(); it != m_args.end(); it++) {
			Assert(*it != NULL,trsException(0,"NULL m_sim"));
			m_string += *(*it)->GetString();
			m_string += ",";
		}
		//tolgo l'ultima virgola
		m_string.erase((m_string.end())-1);
		m_string += ")";
	}

	trsArgList::iterator it;
	m_cmpstr = *GetSim();
	m_cmpstr += "(";
	for (it=m_args.begin(); it != m_args.end(); it++) {
		Assert(*it != NULL,trsException(0,"NULL m_sim"));
		m_cmpstr += *(*it)->GetCmpString();
		m_cmpstr += ",";
	}
	//tolgo l'ultima virgola
	m_cmpstr.erase((m_cmpstr.end())-1);
	m_cmpstr += ")";

	m_syncstr = false;
}


/*- trsFunc::operator == ---------------------------------------------

	Confronta due funzioni. vero sse le stringhe di rappresentazione
	delle due funzioni sono identiche.
--------------------------------------------------------------------*/
bool trsFunc::operator ==(trsTerm &_cmp)
{
	trsFunc* f;
	
	if (_cmp.IsFunc()) {
		f = (trsFunc*) &(_cmp);
		return (*GetCmpString() == *(f->GetCmpString()));
	}
	return false;
}


/*- trsFunc::SetSimbol ----------------------------------------------

	Imposta il simbolo della funzione
	@@ dovrebbe essere privata ed accessibile solo dal parser
--------------------------------------------------------------------*/
void trsFunc::SetSimbol(string* _sim)
{
	if (_sim == NULL) return;
	// il simbolo precedente viene perso ma non eliminato
	m_sim = _sim;
	m_syncstr = true;
	SyncStr();
}


/*- trsFunc::Rename --------------------------------------------------

	Rinomina tutte le proprie variabili
--------------------------------------------------------------------*/
void trsFunc::Rename()
{
	trsVarList vlist;
	trsVarList::iterator vit;
	
	// ottengo una lista delle variabili presenti in questa funzione
	GetVars(vlist);
	if (vlist.empty()) return;

	// creo una sostituzione var->varenamed per ogni variabile in lista
	trsSubst subst;
	for (vit=vlist.begin(); vit != vlist.end(); vit++)
		subst.AddSubPair(*vit,m_algebra->AddRenamedVar(*vit));
	
	// applico la sostituzione alla funzione
	// in teoria dovrebbe restituire this...
	subst.TermApplySub(this);
}


/*- trsFunc::UnRename ------------------------------------------------

	Ripristina le variabili rinominate
--------------------------------------------------------------------*/
void trsFunc::UnRename(trsVarList &_vlist, trsSubst &_subst)
{
	trsVarList::iterator vit1;
	
	// ottengo una lista delle variabili presenti in questa funzione
	if (_vlist.empty())
		GetVars(_vlist);
	
	if (_vlist.empty()) return;

	// per ogni variabile nella lista
	for (vit1=_vlist.begin(); vit1 != _vlist.end(); vit1++) {
		// se gia` e` nella subst salta alla prossima
		if (_subst.Indomain(*vit1))
			continue;

		//altrimenti aggiungi nella subst var-->unrenamed
		//@@ non sarebbe piu` logico se la GetUnRenemed() restituisse 
		// la variabile un-rinominata invece di aggungerla alla subst?
		(*vit1)->GetUnRenamed(_vlist,_subst);
	}
	// applico la subst creata
	_subst.TermApplySub(this);
}


/*- trsFunc::GetVars -------------------------------------------------

	Riempie la lista in ingresso con puntatori alle proprie variabili
	e dei suoi sottotermini.
--------------------------------------------------------------------*/
void trsFunc::GetVars(trsVarList &_vlist)
{
	trsArgList::iterator it;
	trsVarList::iterator vit;
	for (it=m_args.begin(); it != m_args.end(); it++) {
		if ((*it)->IsVar()) {
			bool vfound = false;
			for (vit=_vlist.begin();(vit!=_vlist.end())&&(!vfound);vit++)
				if (*vit==*it) vfound = true;
			if (!vfound) 
				_vlist.push_back((trsVar*)(*it));
		}
		if ((*it)->IsFunc())
			((trsFunc*)(*it))->GetVars(_vlist);
	}	
}


/*- trsFunc::IsInfixOp -----------------------------------------------

	Vero sse il simbolo di funzione e` un operatore infisso.
--------------------------------------------------------------------*/
bool trsFunc::IsInfixOp()
{
	return ((*m_sim=="+")||(*m_sim=="-")||(*m_sim=="*")||(*m_sim=="/")||
		(*m_sim=="|")||(*m_sim=="&")||(*m_sim=="\\"));
}


/*- trsFunc::Size ----------------------------------------------------

	Restituisce la dimensione di questa funzione definita come 
	Size del termine + Size degli argomenti
--------------------------------------------------------------------*/
int trsFunc::Size()
{
	int size, argsize;
	trsArgList::iterator argit;

	size = 1;
	argsize = 0;
	for (argit=m_args.begin(); argit != m_args.end(); argit++)
		argsize += (*argit)->Size();
	
	return (size + argsize);
}


//////////////////////////////////////////////////////////////////////
// class trsVar

/*- trsVar -----------------------------------------------------------

	Costruttore per variabile rinominata _id>0 sebbene non venga
	fatto alcun controllo sull'id
--------------------------------------------------------------------*/
trsVar::trsVar(string _name, int _id, trsAlgebra *_algebra)
{
	m_id = _id;
	m_maxid = 0;
	m_algebra = _algebra;
	m_sim = new string(_name); 
	m_cmpstr = *m_sim;
	m_string = *m_sim;
	if (m_id>0) {
		m_cmpstr += "_";
		m_string += "r";
		char num[12]; // dovrebbe bastare per rinomine fino a 99999999999
		sprintf(num,"%d",m_id);
		m_cmpstr += num;
		m_string += num;
	}
}


/*- trsVar::GetNewRenamed --------------------------------------------

	Crea una nuova variabile rinominandola da questa
--------------------------------------------------------------------*/
trsVar *trsVar::GetNewRenamed()
{
	return m_algebra->AddRenamedVar(this);
}


/*- *trsVar::GetUnRenamed --------------------------------------------


--------------------------------------------------------------------*/
void trsVar::GetUnRenamed(trsVarList &_vlist, trsSubst &_subst)
{
	trsVarList::iterator vit,vit2,vit3;
	trsVar* v;
	bool found;

	//cercati nella subst
	//se ci sei esci
	if (_subst.Indomain(this))
		return;

	//cercati nella lista passata
	//se non ci sei aggiungiti
	found = false;
	for (vit=_vlist.begin(); vit != _vlist.end(); vit++) {
		if ( *this == **vit) {
			found = true;
			break;
		}
	}
	if (!found) {
		_vlist.push_back(this);
		vit = _vlist.end();
		vit--;
	}

	//se hai id=0 aggiungi nella subst te stessa
	if (m_id==0) {
		_subst.AddSubPair(this,this);
		return;
	}

	//altrimenti crea un tuo sostituto
	//aggiungi in _subst this-->sostituto
	//restituisci il sostituto

	//nella lista e` presente una var con stesso simbolo e id=0 ?
	found = false;
	for (vit2=vit; vit2 != _vlist.end(); vit2++) {
		if ((*(*vit2)->GetSim() == *m_sim) && 
			((*vit2)->GetID() == 0)) {
			found = true;
			break;
		}
	}
	//oppure la var (nell'algebra) con stesso simbolo e id=0 e` gia` stata 
	//inserita nella subst?
	if (!found) {
		v = m_algebra->FindVar(m_sim);
		if (!_subst.Inrange(v)) {
			_subst.AddSubPair(this,v);
			return;
		}
	}
	//cerca nell'algebra la prima var con id=0 il cui simbolo non
	//ricorra nella lista e che ancora non sia nella subst
	for (vit2=m_algebra->m_variables.begin(); 
		vit2 != m_algebra->m_variables.end(); vit2++)
	{
		if ((*vit2)->GetID() == 0) {
			//cerca nella lista una variabile col medesimo simbolo
			//se la trovi allora la var e` riservata
			found = false;
			for (vit3=_vlist.begin(); vit3 != _vlist.end(); vit3++)
				if (*(*vit3)->GetSim() == *(*vit2)->GetSim()) {
					found = true;
					break;
				}
			if (found) 
				continue;
			if (!_subst.Inrange(*vit2)) {
				_subst.AddSubPair(this,*vit2);
				return;
			}
		}
	}
	//crea un nuovo simbolo di variabile formato da m_sim + i
	string sim(*m_sim);
	found = false;
	vit2=vit;
	vit2--;
	while (vit2 != _vlist.end()) {
		if (*(*vit2)->GetSim() == *m_sim)
			if ((*vit2)->GetID() != 0) {
				found = true;
				break;
			}
		vit2--;
	}
	if (!found) {
		sim += "1";
	} else {
		v = (trsVar*)_subst.GetSubstitute(*vit2);
		int num;
		string suff;
		if (v->GetSim()->find(sim) == 0) {
			suff = v->GetSim()->substr(sim.size());
			if (suff.find_first_not_of("0123456789") != suff.npos)
				suff = "0";
			num = atoi(suff.c_str()) + 1;
		} else {
			num = 1;
		}
		char buf[12];
		sprintf(buf,"%d",num);
		sim += buf;
		// devo cercare nella lista se gia` c'e` una variabile con lo 
		// stesso simbolo, altrimenti devo crearne uno nuovo
		for (vit2=_vlist.begin(); vit2 != _vlist.end(); vit2++) {
			if (*(*vit2)->GetSim() == sim) {
				vit2 = _vlist.begin();
				num++;
				sprintf(buf,"%d",num);
				sim = *m_sim;
				sim += buf;
			}
		}
	}
	// cerca nell'algebra se una variabile con tale simbolo e` gia` 
	// presente altrimenti creala
	v = m_algebra->AddVar(&sim);
	_subst.AddSubPair(this,v);

/*  
esempio:
	coppia: (X_1*Y)+f(X_1,Y_1,K_2)=g(K_1,K_3)
	lista: {X_1, Y, Y_1, K_2, K_1, K_3}
	algebra: {X, X_1, Y, Y_1, Z, K, K_1, K_2, K_3}
	subst: {X_1-->X, Y-->Y, Y_1-->Z, K_2-->K, K_1-->K1, K_3-->K2}
	risultato: (X*Y)+f(X,Z,K)=g(K1,K2)
	nuova algebra: {X, Y, Z, K, K1, K2}
*/
}


// EOF trsterm.cpp /////////////////////////////////////////////////////////////

