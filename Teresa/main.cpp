#include <cstdio>
#include <trs.h>

char nomefile[] = "identita.txt";

void main() {
	trsParser parser;
	trsRawIds raw;
	trsTermPairs teoria;
	trsSimArityPairs segnatura;
	trsLPOOrder order;
	FILE* fp;
	char identita[1024], ordstr[1024];
	string rawstr;
	
	if ( !(fp = fopen(nomefile,"rb")) ) {
		fprintf(stderr,"O pistola!\n");
		exit(1);
	}
	while (fscanf(fp,"%s",identita) != EOF) {
		if (strcmp(identita,"%")==0) {
			fscanf(fp, "%s", ordstr);
			break;
		}
		raw.insert(identita);
	}
	fclose(fp);
	
	try {
		parser.GetPairs(&raw,&teoria,&segnatura);
		order.SetNewOrder(ordstr,&segnatura);
	} catch(trsException E) {
		printf(E.GetMsg().c_str());
		exit(0);
	}
	order.VisualizzaOrdinamento();
	printf("\n\nOK GIONNI GLAMOUR\n");
}
