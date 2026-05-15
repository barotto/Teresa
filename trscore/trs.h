/*//////////////////////////////////////////////////////////////////////////////
	FILENAME: trs.h
	Version:  1.1.0
	Authors:  Marco Bortolin, Alberto Minora
	Created:  31/07/2001 - 16.01.18
	
	Definizione dei tipi di dati e delle classi adatti a definire un TRS

	N.B.: Le classi e le funzioni definite in questo file ed implementate 
	nei trs*.cpp sono totalmente indipendenti dall'interfaccia utilizzata per il 
	programma e dal target system per il quale si compilano.
//////////////////////////////////////////////////////////////////////////////*/
#ifndef TRS_H
#define TRS_H

#include <list>
#include <string>
#include <set>
#include <cstdarg>
#include <cstdio>

using namespace std;

#define TRS_VERSION_STR "1.1"

#ifdef NDEBUG 
	#define TRS_TRACE_START {
	#define TRS_TRACE_END ;}
#else
	#ifdef TRS_AFX_USED
		#include <afxwin.h> // per la TRACE
		#define TRS_TRACE_START ::AfxTrace(
	#else
		#define TRS_TRACE_START fprintf(stderr,
	#endif
	#define TRS_TRACE_END ); fflush(stderr);
#endif

#ifdef WIN32
	#pragma warning(disable:4786)
	#pragma warning(disable:4100)
#endif


template <class T, class X> inline void Assert(T expr, X x)
{
#ifndef NDEBUG
	if (!expr) throw x;
#endif
}

/*
	Prototipi di classe
*/
class trsOutputDev;
class trsNullDev;
class trsStdDev;
class trsFBDev;
class trsStdFBDev;
class trsException;
class trsTerm;
class trsConst;
class trsFunc;
class trsVar;
class trsTermPair;
class trsSimArityPair;
class trsSimArityPairs;
class trsSAException;
class trsAlgebra;
class trsSystem;
class trsParser;
class trsParserException;
class trsOrder;
class trsOrderException;
class trsLPOOrder;
class trsSubst;
class trsRewrite;
class trsUnify;
class trsUnifyException;
class trsCriticalPairs;
class trsHuet;
class trsHuetException;


//@@ problemi con string.c_str()
void trsTrace(const char* _format, ...);



/*	
	Dispositivi di output
	---------------------
	Utilizzati dalle classi per stampare messaggi durante l'elaborazione
	o il contenuto di particolari oggetti come trsTermPairs, trsSubst,
	trsTermPair.
	Per stampare su un dispositivo particolare quale una finestra di Windows o X 
	derivare da trsOutputDev e definire gli	operatori << .
*/
class trsOutputDev
{
public:	
	virtual trsOutputDev &operator <<(string _src) = 0;
	virtual trsOutputDev &operator <<(const char* _src) = 0;
	virtual trsOutputDev &operator <<(int _src) = 0;
};

// non stampa nulla
class trsNullDev : public trsOutputDev {
public:	
	trsOutputDev &operator <<(string _src) { return *this; }
	trsOutputDev &operator <<(const char* _src) { return *this; }
	trsOutputDev &operator <<(int _src)  { return *this; }	
};

// stampa su stdout (cout)
class trsStdDev : public trsOutputDev {
public:	
	trsOutputDev &operator <<(string _src);
	trsOutputDev &operator <<(const char* _src);
	trsOutputDev &operator <<(int _src);
};

// stampa una lista di numeri interi
trsOutputDev &operator<<(trsOutputDev &_dev, list<int> &_l);


/*
	Dispositivi per feedback 
	------------------------
	Utilizzati per fornire all'utente informazioni circa lo stato
	di una computazione e/o ricevere da esso un comando per la sospensione
	o l'arresto dell'esecuzione.

*/
class trsFBDev
{
public:
	virtual ~trsFBDev(){};

	virtual bool Stop() = 0;

	virtual void SetProgressRange(int _lower, int _upper) = 0;
	virtual void SetProgressStep(int _step) = 0;
	virtual void SetProgressPos(int _pos) = 0;
	virtual void StepProgress() = 0;

	virtual void PrintInt(int _vec[], int _size) = 0;

	virtual void PrintStr(const char* _str) = 0;
	virtual void PrintFuncName(const char* _str) = 0;
	
	virtual void Reset() = 0;
};

// stampa su terminale standard i propri messaggi
class trsStdFBDev : public trsFBDev
{
	int m_upper;
	int m_lower;
	int m_step;
	int m_pos;
	int m_loop;

public:
	trsStdFBDev():m_upper(100),m_lower(0),m_step(10),m_pos(0),m_loop(0){};
	
	bool Stop() { return false; }

	void SetProgressRange(int _lower, int _upper);
	void SetProgressStep(int _step);
	void SetProgressPos(int _pos);
	void StepProgress();

	void PrintStr(const char* _str);
	void PrintFuncName(const char* _str);

	void PrintInt(int _vec[], int _size);

	void Reset();
};


/*
	Eccezione
	---------
	Base class dalla quale derivare tutte le eccezioni.
	Per un'immediata derivazione utilizzare le macro
	BEGIN_/END_TRS_EXCEPTION. 
*/
class trsException
{
protected:
	string	m_msg;
	int		m_code;

	void Printmsg(const char* format, va_list ap);

public:
	trsException():m_code(0) { m_msg ="unknown error"; }
	trsException(int code, string &msg);
	trsException(int code, const char* format, ...);

	virtual ~trsException(){};

	void	AppendStr(string &msg);
	void	AppendStr(const char* format, ...);

	string &GetMsg() { return m_msg; }
	int	    GetCode() { return m_code; }
	void	DebugPrint();
};

// macro per l'implementazione di una classe derivata da trsException:
#define BEGIN_TRS_EXCEPTION(xClassName) \
class xClassName : public trsException \
{ \
public: \
	xClassName() : trsException() {} \
	xClassName(int code, string &msg) : trsException(code,msg) {} \
	xClassName(int code, const char* format, ...) : trsException(code,"") \
	{ \
		va_list ap; \
		va_start(ap, format); \
		Printmsg(format,ap); \
		va_end(ap); \
	} \
\
	~xClassName() {}

#define END_TRS_EXCEPTION };

// alcuni codici di uso generale
enum TRSERR_CODES {
	TRSERR_UNK,
	TRSERR_OUTOFBOUND,
	TRSERR_BADTERM,
	TRSERR_NULLPTR,
	TRSERR_USERSTOP
};


/*
	Termine
	-------
*/
class trsTerm
{
protected:
	string *m_sim;

public:
	virtual ~trsTerm() {}

	virtual const string *GetSim() { return m_sim; }
	
	virtual string GetPrintable() { return *m_sim; }
	
	// restituisce una stringa che, passata al parser, 
	// mi consente di riottenere il medesimo termine
	virtual const string *GetString() { return m_sim; }
	// restituisce una stringa che puo` essere usata nei confronti
	// tale che *this==term sse *(this->GetCmpString()) == *(term.GetCmpString())
	virtual const string *GetCmpString() { return m_sim; }
	
	virtual trsTerm *Duplicate() = 0;

	virtual bool IsVar() const = 0;
	virtual bool IsConst() const = 0;
	virtual bool IsFunc() const = 0;
	
	virtual int Size() { return 1; }

	virtual bool operator ==(trsTerm &_cmp)
		{ return ( *GetCmpString() == *(_cmp.GetCmpString()) ); }

};



/*
	Variabile
	---------
*/

// lista di variabili
typedef list<trsVar*> trsVarList;

class trsVar : public trsTerm
{
// trsAlgebra e` l'unica altra classe in grado di maneggiare gli indici
friend trsAlgebra; 
private:

	string m_cmpstr;
	string m_string;

	trsAlgebra* m_algebra;

	int m_id;

	// ha significato solo nella variabile di indice 0, rappresenta il massimo 
	// indice raggiunto nel renaming. Nelle variabili non 0 occupa spazio 
	// e basta... meglio un aumento in complessita` in spazio che in tempo.
	// Accessibile solo da trsVar e trsAlgebra.
	int m_maxid; 

public:
	trsVar(string _name, trsAlgebra *_algebra):m_id(0),m_maxid(0),m_algebra(_algebra)
	{ 
		m_sim = new string(_name);
		m_cmpstr = _name;
		m_string = _name;
	}
	trsVar(string _name, int _id, trsAlgebra *_algebra);

	~trsVar()
	{   
		//TRS_TRACE_START "~trsVar() : %s\n", m_sim->c_str() TRS_TRACE_END; 
		delete m_sim; 
	}

	bool IsVar() const { return true; }
	bool IsConst() const { return false; }
	bool IsFunc() const { return false; }

	bool operator ==(trsTerm &_cmp)
		{ return ( this == &_cmp ); }

	string GetPrintable() { return m_cmpstr; }
	const string *GetString() { return &m_string; }
	const string *GetCmpString() { return &m_cmpstr; }
	
	trsTerm *Duplicate() { return this; }

	trsVar *GetNewRenamed();
	void    GetUnRenamed(trsVarList &_vlist, trsSubst &_subst);
	
	int GetID() { return m_id; }
};


/*
	Funzione
	--------
*/
typedef list<trsTerm*> trsArgList;

class trsFunc : public trsTerm
{
private:
	string     m_string;
	trsArgList m_args;
	trsFunc*   m_father;
	string     m_pattern;
	string     m_cmpstr;

	trsAlgebra* m_algebra;

	bool m_syncstr; //true = sincronizza stringa con gli argomenti
	void SetSyncstr();
	void SyncStr();

public:
	trsFunc(string* _sim, trsAlgebra* _algebra);

	~trsFunc();

	const string *GetString();
	const string *GetCmpString() {if (m_syncstr) SyncStr(); return &m_cmpstr;}
	string GetPrintable();
	string GetPattern(trsVarList &_l,bool _sync);
	
	int GetArieta() const { return m_args.size(); }	
	trsTerm	*GetFirstArg(trsArgList::iterator &_it);
	trsTerm	*GetNextArg(trsArgList::iterator &_it);
	trsArgList *GetArgs() { return &m_args; }

	void GetVars(trsVarList &_vlist);

	trsFunc *GetFather() { return m_father; }
	
	// @@ dovrebbe poter essere usata solamente dal parser, privata.
	void AddArg(trsTerm *_arg);

	void SubstArg(int _pos, trsTerm *_argnew);
	
	bool IsVar() const { return false; }
	bool IsConst() const { return false; }
	bool IsFunc() const { return true; }
	
	bool operator ==(trsTerm &_cmp);

	bool Occurs(trsVar *_t);
	
	// @@ privata friend parser
	void SetSimbol(string *_sim);
	
	trsTerm *Duplicate();
	
	void Rename();
	void UnRename(trsVarList &_vlist, trsSubst &_subst);

	trsTerm *GetPosTerm(list<int> &_l);

	bool IsInfixOp();

	int Size();
};



/*	
	Costante
	--------
*/
class trsConst : public trsTerm
{
public:
	trsConst(string *_sim) { m_sim = _sim; }
	~trsConst()
	{
		//TRS_TRACE_START "~trsConst() : %s\n", m_sim->c_str() TRS_TRACE_END
	}
	
	bool IsVar() const { return false; }
	bool IsConst() const { return true; }
	bool IsFunc() const { return false; }
	
	trsTerm *Duplicate() { return this; }
	bool operator ==(trsTerm &_cmp)
		{ return ( this == &_cmp ); }
};



/*
	Ordine
	------
	Definisce un ordine tra termini.
*/
BEGIN_TRS_EXCEPTION(trsOrderException)
END_TRS_EXCEPTION

class trsSimArityPairs;

class trsOrder
{
protected:
	trsOutputDev *m_outdev;
	trsFBDev     *m_feedback;
	trsStdFBDev  _m_stdfb;

public:
	virtual ~trsOrder(){};

	// lanciano una eccezione quando i due termini non sono confrontabili.
	virtual bool Greater(trsTerm* _s, trsTerm* _t) throw(trsOrderException) = 0;
	virtual bool Less(trsTerm* _s, trsTerm* _t) throw(trsOrderException) = 0;
	virtual bool Equal(trsTerm* _s, trsTerm* _t) throw(trsOrderException) = 0;

	void SetDevices(trsOutputDev *_outdev, trsFBDev *_feedback)
	{
		m_feedback = _feedback;
		if (m_feedback==NULL) 
			m_feedback = &_m_stdfb;
		m_outdev = _outdev;
	}
};

enum ORD_ERR_CODES{
	ORD_ALFANUM_NOT_FOUND,
	ORD_PRECOP_NOT_FOUND,
	ORD_BAD_FUNCNAME,	
	ORD_DUP_FUNCNAME,
	ORD_NOT_COMPARABLES
};

enum ORD_COMPARE_MODES{
	ORD_GR,
	ORD_EQ,
	ORD_NGE
};

// Ordine LPO
class trsLPOOrder : public trsOrder
{
	typedef list<string> node;
	list<node> m_preclist;
	char       m_sim;
	string     m_ordstr;
	int        m_readpos;
	int        m_len;
	list<int>  m_funcpos;
	int        m_tokenpos;
	trsSimArityPairs *m_segnatura;

	void   ReadSimbol();
	void   AddRemaining();
	void   AddToken(string &_tok, char _op);
	string Token();
	char   PrecedenceOp();

	void Error(int _code, const char* _sim) throw(trsOrderException);
	
	bool Precede(trsTerm* _s,trsTerm* _t);
	bool SamePrec(trsTerm* _s,trsTerm* _t);
	
	// confronta due termini
	bool CompareTerm(trsTerm* _s, trsTerm* _t,int _mode);
	
	// confronta due liste di termini
	bool CompareTermList(trsArgList &_s,trsArgList &_t,
		int _mode, trsTerm* &_outs, trsTerm* &_outt);
	bool CompareTermList(trsArgList &_s,trsTerm* _t, int _mode);
	bool CompareTermList(trsTerm* _s,trsArgList &_t, int _mode);

	void PrintOrdMsg(trsTerm *_s, trsTerm *_t, const char* _rule);
	void PrintNotOrdMsg(trsTerm *_s, trsTerm *_t, const char* _msg);

public:
	trsLPOOrder();
	~trsLPOOrder() {};
	
	bool Greater(trsTerm* _s, trsTerm* _t)  throw(trsOrderException);
	bool Less(trsTerm* _s, trsTerm* _t)  throw(trsOrderException);
	bool Equal(trsTerm* _s, trsTerm* _t)  throw(trsOrderException);
	
	void SetNewOrder(const char* _ordstr, trsSimArityPairs* _segnatura)
		throw(trsOrderException);

	const string &GetOrdStr() { return m_ordstr; }
};



/*
	Sostituzione
	------------
*/
class trsSubst
{
	class trsSubstPair
	{
	public:
		trsSubstPair():ter(NULL),var(NULL){}
		~trsSubstPair() 
		{
			if (ter!=NULL)
				if (ter->IsFunc())
					delete ter;
		}
		
		trsVar* var;
		trsTerm* ter;
	};
	
	list<trsSubstPair> m_subst;

public:
	void AddSubPair(trsVar *_var, trsTerm *_ter);
	bool Indomain(trsVar *_var);
	bool Inrange(trsTerm *_ter);
	trsTerm *GetSubstitute(trsVar *_var);
	trsTerm *TermApplySub(trsTerm *_ter);
	void Reset();

	void PrintSubst(trsOutputDev &_out);
	friend trsOutputDev &operator <<(trsOutputDev &_dev, trsSubst &_subst);
};



/*
	Coppia di termini
	-----------------
*/
class trsTermPair
{
private:
	trsTerm *m_left;
	trsTerm *m_right;
	bool m_autodispose;
	
public:
	trsTermPair():m_left(NULL),m_right(NULL),m_autodispose(true) {};
	trsTermPair(trsTerm* _inleft, trsTerm* _inright, bool _auto=true);

	~trsTermPair();

	trsTerm *GetLeft() { return m_left; }
	trsTerm *GetRight() { return m_right; }

	void SetRight(trsTerm *_right) { m_right = _right; }
	void SetLeft(trsTerm *_left) { m_left = _left; }

	void AutoDispose(bool _disp=true) { m_autodispose = _disp; }

	void Rename();
	void UnRename();

	void Swap();
	bool operator ==(trsTermPair& _tp);
	
	string GetPrintable(bool _rule=false);
	
	string GetString();

	friend trsOutputDev &operator<<(trsOutputDev &_dev, trsTermPair &_pair);
	
	void GetVars(trsVarList &_vlist);
};


// Insieme di identita` scritte come stringhe da tradurre in termini.
typedef list<string> trsRawIds;


// Insieme di coppie di termini.
typedef list<trsTermPair> trsTermPairs;

trsOutputDev &operator<<(trsOutputDev &_dev, trsTermPairs &_pairs);
trsOutputDev &PrintTRS(trsOutputDev &_dev, trsTermPairs &_trs);


/*
	Unify
	-----
	Risolve il problema dell'unificazione
*/
BEGIN_TRS_EXCEPTION(trsUnifyException)
END_TRS_EXCEPTION

class trsUnify
{
	trsOutputDev *m_outdev;
	trsFBDev     *m_feedback;
	trsStdFBDev  _m_stdfb;

	trsTermPairs m_list;

	void Clash() throw(trsUnifyException);
	void OccursCheck() throw(trsUnifyException);
	bool Orient();
	bool Decompose();
	bool Eliminate();
	bool Delete();
	bool CheckDuplicate(trsTermPair &_tp);
	void CheckDuplicates();

	void PrintMsg(const char* _funcname, trsTermPair &_tp);
	void PrintList();

public:
	trsUnify (trsOutputDev *_outdev=NULL, trsFBDev *_feedback=NULL)
		{ SetDevices(_outdev, _feedback); }
	~trsUnify(){}

	void Unify(trsTermPairs &_l, trsSubst &_unifier) throw(trsUnifyException);
	void Unify(trsTermPair &_tp,trsSubst &_unifier) throw(trsUnifyException);

	void SetDevices(trsOutputDev *_outdev, trsFBDev *_feedback)
	{
		m_feedback = _feedback;
		if (m_feedback==NULL) 
			m_feedback = &_m_stdfb;
		m_outdev = _outdev;
	}
};



/*
	Rewrite
	-------
	Riscrive termini
*/
class trsRewrite
{
	BEGIN_TRS_EXCEPTION(trsMatchException)
	END_TRS_EXCEPTION
	
	trsFBDev     *m_feedback;
	trsOutputDev *m_outdev;
	trsStdFBDev  _m_stdfb;

	trsTermPairs* Zip(trsArgList* _s, trsArgList* _t);

	void Match(trsTermPairs &_l,trsSubst &_sub) throw(trsMatchException);
	void Match(trsTermPair &_tp,trsSubst &_sub) throw(trsMatchException);

public:
	trsRewrite(trsOutputDev *_outdev=NULL, trsFBDev *_feedback=NULL)
		{ SetDevices(_outdev, _feedback); }
	~trsRewrite(){}

	trsTerm* Rewrite(trsTerm* _t, trsTermPairs* _trs);
	trsTerm* Rewrite(trsTerm* _t, trsTermPair* _pair);

	trsTerm* Normalize(trsTerm* _t, trsTermPairs* _trs) throw (trsException);
	trsTerm* Normalize(trsTerm* _t, trsTermPair *_tp) throw (trsException);

	void SetDevices(trsOutputDev *_outdev, trsFBDev *_feedback)
	{
		m_feedback = _feedback;
		if (m_feedback==NULL) 
			m_feedback = &_m_stdfb;
		m_outdev = _outdev;
	}
};



/*
	Coppie Critiche
	---------------
*/
class trsCriticalPairs
{
	trsOutputDev *m_outdev;
	trsFBDev     *m_feedback;
	trsStdFBDev  _m_stdfb;

	trsTermPairs *m_cp;
	trsTermPairs m_trs;
	int m_rootpos;
	bool m_unrename;

	void CriticalPair(trsTermPair &_pair, trsTerm *_root, list<int> &_l, 
		int _trspos);
	void DupTrs(trsTermPairs &_trs, bool _rename=true);
	void AddCP(trsTerm *_l1,trsTerm *_r1,trsTerm *_r2,trsSubst &_sub,
		trsTerm *_root, list<int> &_l);
	bool CheckDuplicate(trsTermPair &_tp);

public:
	trsCriticalPairs(trsOutputDev *_outdev=NULL, trsFBDev *_feedback=NULL)
		{ SetDevices(_outdev, _feedback); }

	void CriticalPairs(trsTermPairs &_trs, trsTermPairs &_cp, 
		trsAlgebra &_algebra,bool _unrename);
	void CriticalPairs(trsTermPair &_pair, trsTermPairs &_trs,
		trsTermPairs &_cp, trsAlgebra &_algebra, bool _itself,bool _unrename);
	void CriticalPairs(trsTermPairs &_trs, trsTermPair &_pair,
		trsTermPairs &_cp, trsAlgebra &_algebra,bool _unrename);

	void SetDevices(trsOutputDev *_outdev, trsFBDev *_feedback)
	{
		m_feedback = _feedback;
		if (m_feedback==NULL) 
			m_feedback = &_m_stdfb;
		m_outdev = _outdev;
	}

};



/*
	Segnatura
	---------
*/
// coppia (simbolo di funzione,arieta`)
class trsSimArityPair {
public:
	string funcsim;
	unsigned int arity;
};

BEGIN_TRS_EXCEPTION(trsSAException)
END_TRS_EXCEPTION

class trsSimArityPairs 
{
private:
	list<trsSimArityPair> m_tab;
	list<trsSimArityPair>::iterator m_it;

public:
	virtual ~trsSimArityPairs()
	{
		//TRS_TRACE_START "~trsSimArityPairs()\n" TRS_TRACE_END
	} 
	int Insert(const char *_sim, unsigned int _arity) throw(trsSAException); 
	int Find(const char *_sim, int _arity=-1);

	int     GetSize() { return m_tab.size(); }
	string *GetSim(int _pos);
	int     GetArity(int _pos);
};



/*
	Algebra
	-------
*/

class trsAlgebra
{
// tabella delle costanti
// non accessibile all'esterno, serve solo all'algebra
typedef list<trsConst*> trsConstList;
	
public:
	~trsAlgebra();
	trsSimArityPairs m_signature;
	trsVarList m_variables;
	//per motivi di complessita` in spazio le costanti sono tutte inserite
	//nella seguente lista, che e` parte di questa classe per raggrupparla
	//con le 2 precedenti liste.
	trsConstList m_constants;

	trsVar   *FindVar(const string *_str, int _id=0);
	trsConst *FindConst(const string *_str);
	
	trsVar *AddVar(const string *_sim);
	trsVar *AddRenamedVar(trsVar *_srcvar);
	void    CleanDupVars();
};



/*
	Parser
	------
	Classe che incorpora funzioni di controllo e traduzione di stringhe grezze
	in termini.
*/

BEGIN_TRS_EXCEPTION(trsParserException)
friend trsParser;
private:
	string	m_raw;
public:
	const string &GetRawString() const { return m_raw; }
END_TRS_EXCEPTION

enum PARS_ERR_CODES{
	PARS_EQUAL_NOT_FOUND,
	PARS_CLOSEPAR_NOT_FOUND,
	PARS_ALFANUM_NOT_FOUND,
	PARS_ALFAORPAR_NOT_FOUND,
	PARS_IDENTITY_NOT_COMPLETE,
	PARS_END_EXPECTED,
	PARS_WRONG_ARITY
};

class trsParser
{
	char        m_sim;      //ultimo simbolo letto
	int         m_readpos;  //posizione testina di lettura sull'input
	const char *m_identity; //identita' da analizzare
	int         m_leneq;    //lung. dell'equazione da analizzare
	trsAlgebra *m_algebra;  //algebra
	
	void Error(int _code) throw (trsParserException);
	void ReadSimbol();
	void Equation(trsTermPair *_pair);
	trsTerm* Term();
	trsTerm* Factor();
	
	void ArgumentList(trsFunc *_func, string *_sim, trsSimArityPairs *_sig);
	
	bool IsInfixOperator(string *_op);
	void Reset(const char *_raw);
	
	trsTerm *MakeVar(string* _sim);
	trsTerm *MakeConst(string* _sim,trsSimArityPairs* _sig);
		
public:
	void GetPair(const char *_raw, trsTermPair* _outpair, 
		trsAlgebra *_outalgebra) 
		throw (trsParserException,trsSAException);
	void GetPairs(const trsRawIds *_raw, trsTermPairs *_outpairs, 
		trsAlgebra *_outalgebra) 
		throw (trsParserException, trsSAException);
};



/*
	Huet completion
	---------------
*/
BEGIN_TRS_EXCEPTION(trsHuetException)
END_TRS_EXCEPTION

class trsHuet
{
	trsStdFBDev  _m_stdfb;

	trsOutputDev *m_outdev;
	trsFBDev     *m_feedback;

	trsTermPairs m_theory;
	trsTermPairs *m_trs;
	list<bool> *m_marks;
	trsRewrite       m_rew;
	trsCriticalPairs m_crit;
	trsOrder        *m_ord;
	trsSystem       *m_sys;
	void trsHuet::DupTheory(trsTermPairs &_theory);

	int ChooseRule();
	trsTermPairs::iterator ChoosePair();

	void Simplify(trsTermPair &_tp);
	bool Delete(trsTermPair &_tp);
	void Orient(trsTermPair &_tp);
	void LRSimplify(trsTermPair &_tp);
	void Deduce();
	bool CheckDuplicate(trsTermPairs *_l, trsTermPair &_tp);
	void Exit();

public:
	trsHuet(trsOutputDev *_outdev=NULL, trsFBDev *_feedback=NULL)
		{ SetDevices(_outdev, _feedback); }
	
	void Completion(trsSystem &_sys, trsOrder *_ord) throw (trsHuetException);
	
	void SetDevices(trsOutputDev *_outdev, trsFBDev *_feedback)
	{
		m_outdev = _outdev;
		m_feedback = _feedback;
		if (m_feedback==NULL) 
			m_feedback = &_m_stdfb;
		m_rew.SetDevices(NULL,m_feedback);
		m_crit.SetDevices(NULL,m_feedback);
	}
};



/* 
	Sistema
	-------
	Ragruppa una teoria, un TRS e l'algebra di supporto. 
	Necessario per garantire la corretta distruzione degli oggetti.
*/
class trsSystem
{	
	trsAlgebra   m_algebra;
	trsTermPairs m_theory;
	trsTermPairs m_trs;

public:
	~trsSystem() 
	{ 
		//TRS_TRACE_START "~trsSystem()\n" TRS_TRACE_END
		m_trs.clear();		
		m_theory.clear();
		// infine distrugge l'algebra
	}
	trsAlgebra *GetAlgebra() { return &m_algebra; }
	trsTermPairs *GetTheory() { return &m_theory; }
	trsTermPairs *GetTrs() { return &m_trs; }

	void ClearTheory() { m_theory.clear(); }
	void ClearTrs() { m_trs.clear(); }
};



#endif //TRS_H

// EOF trs.h ///////////////////////////////////////////////////////////////////
