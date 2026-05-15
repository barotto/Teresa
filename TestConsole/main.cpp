#ifdef WIN32
#pragma warning(disable:4786)
#pragma warning(disable:4100)
#endif

#include <cstdio>
#include <trs.h>

char nomefile[] = "identita.txt";
trsStdDev stddev;
int main() {
	trsParser         parser;
	trsRawIds         raw;
	trsSystem         sys;
	trsLPOOrder       order;
	FILE* fp;

	char identita[1024], ordstr[1024];
	string rawstr;
	trsTermPairs::iterator it;
//	trsTerm *s,*t;
	string str1,str2;
	
	if ( !(fp = fopen(nomefile,"rb")) ) {
		fprintf(stderr,"O pistola!\n");
		exit(1);
	}
	while (fscanf(fp,"%s",identita) != EOF) {
		if (strcmp(identita,"%")==0) {
			fscanf(fp, "%s", ordstr);
			break;
		}
		raw.push_back(identita);
	}
	fclose(fp);
	try {
		parser.GetPairs(&raw,sys.GetTheory(),sys.GetAlgebra());
		order.SetNewOrder(ordstr,&(sys.GetAlgebra()->m_signature));
	} catch(trsParserException E) {
		printf("%s\n",E.GetMsg().c_str());
		fflush(stdout);
		return EXIT_FAILURE;
	} catch(trsSAException E) {
		printf("%s\n",E.GetMsg().c_str());
		fflush(stdout);
		return EXIT_FAILURE;
	} catch(...) {
		printf("Problema sconosciuto nella lettura del file.\n");
		fflush(stdout);
		return EXIT_FAILURE;
	}
	stddev << "\n--- INPUT ---\n\n" << *sys.GetTheory() << "\n";

/*
	try {
		match.Match(teoria,subst);
	} catch (trsMatchException E) {
		stdoutdev.printf("Ahoo!!! Ma che sta' a ffa`???\n");
	}
	subst.PrintSubst(&stdoutdev);*/

	//visualizzo gli ordinamenti
	/*
	order.VisualizzaOrdinamento();
	it = teoria->begin();
	while (it != teoria->end()) {
		s = it->GetLeft();
		t = it->GetRight();
		if (s->IsFunc()) {
			str1 = *(((trsFunc*)s)->GetString());
		} else {
			str1 = *(s->GetSim());
		}
		if (t->IsFunc()) {
			str2 = *(((trsFunc*)t)->GetString());
		} else {
			str2 = *(t->GetSim());
		}
		try {
			if (order.Equal(s,t))
				printf("\n%s = %s",str1.c_str(),str2.c_str());
			if (order.Greater(s,t))
				printf("\n%s > %s",str1.c_str(),str2.c_str());
			if (order.Less(s,t))
				printf("\n%s < %s",str1.c_str(),str2.c_str());
		} catch(...) {
			printf("\n%s = %s non confrontabili",
				str1.c_str(),str2.c_str());
		}
		it++;
	}
	*/
	
	//NORMALIZZAZIONE
	/*
	trsRewrite rew;
	sprintf(identita,"(X*1)*Z=X*Z");
	trsTermPair tp;
	trsTerm *l,*r;
	try {
		parser.GetPair(identita,&tp,sys.GetAlgebra());
	} catch(...) { 
		return 1; 
	}
	stddev << "Da normalizzare: " << tp << "\n";
	l = rew.Normalize(tp.GetLeft(),sys.GetTheory());
	r = rew.Normalize(tp.GetRight(),sys.GetTheory());
	stddev << "\nNormalizzata: "<< l->GetPrintable() << "="
		<< r->GetPrintable() << "\n"; 
*/

/*
	//UNIFICAZIONE

	trsSubst subst;
	trsUnify uni(stddev);

	stddev << "--- UNIFICAZIONE ---\n\n";
	try {
		uni.Unify(*sys.GetTheory(),subst);
		stddev << "\n * Risultato:\n" << subst;
	} catch (trsUnifyException E) {
		stddev << "\nERRORE Unificazione:\n  \"" << E.GetMsg() << "\"\n\n";
	}
*/

/*
	//COPPIE CRITICHE
	
	trsCriticalPairs cp(&stddev);
	trsTermPairs cplist;
	try {
		stddev << "--- COPPIE CRITICHE ---\n\n";
		cp.CriticalPairs(*sys.GetTheory(),cplist,*sys.GetAlgebra());
		stddev << "\n * Risultato:\n" << cplist;
	} catch (trsException E) {
		stddev << "\nERRORE Coppie critiche:\n  \"" << E.GetMsg() << "\"\n\n";
	}
*/

/*
	// HUET

	trsHuet huet(&stddev,NULL);
	try {
		stddev << "--- HUET COMPLETION ---\n\n";
		huet.Completion(sys,&order);
		stddev << "\n * Risultato:\n";
		PrintTRS(stddev,*sys.GetTrs());
	} catch (trsException E) {
		stddev << "\nERRORE Huet:\n  \"" << E.GetMsg() << "\"\n\n";
	}
*/
	
	// Test procedura di unrename variabili

	trsTermPair tp;
	trsAlgebra alg;
	trsSubst sub;
	trsVar* v;
	string s;

	parser.GetPair("f(X,Y,K)=g(T,H)",&tp,&alg);

	s = "X";
	v = alg.FindVar(&s);
	sub.AddSubPair(v,alg.AddRenamedVar(v));
	s = "Y";
	v = alg.FindVar(&s);
	sub.AddSubPair(v,alg.AddRenamedVar(v));
	s = "K";
	v = alg.FindVar(&s);
	sub.AddSubPair(v,alg.AddRenamedVar(v));
	s = "T";
	sub.AddSubPair(alg.FindVar(&s),alg.AddRenamedVar(v));
	s = "H";
	sub.AddSubPair(alg.FindVar(&s),alg.AddRenamedVar(v));

	stddev << tp << "\n";

	sub.TermApplySub(tp.GetLeft());
	sub.TermApplySub(tp.GetRight());

	stddev << tp << "\n";

	trsVarList::iterator vit;
	vit=alg.m_variables.begin();
	while (vit != alg.m_variables.end()) {
		s = *(*vit)->GetSim();
		if ((s == "T") || (s == "H")) 
			alg.m_variables.erase(vit++);
		else
			vit++;
	}

	tp.UnRename();
	stddev << tp;
	tp.AutoDispose(false);
	delete tp.GetLeft();
	delete tp.GetRight();

	stddev << "\n\nOK GIONNI GLAMOUR\n";
	return 0;
}
