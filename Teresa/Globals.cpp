/*//////////////////////////////////////////////////////////////////////////////
	FILENAME:	Globals.cpp
	Version:	0.1.0
	Author:		Marco Bortolin
	Created:	27/07/2001 - 22.31.10

	@@ obsoleto
	Copyright (C) 2001 Marco Bortolin
//////////////////////////////////////////////////////////////////////////////*/

#include "StdAfx.h"
#include "Teresa.h"
#include "Globals.h"

/*--------------------------------------------------------------------
	FUNC NAME: 	ErrorExit
	RETURNS: 	void 
	THROWS: 	-

	PURPOSE/DESCRIPTION: 
	Da chiamarsi in caro di errore del programma. Stampa a video la
	stringa inMex e termina il programma se specificato (default true)

--------------------------------------------------------------------*/
void ErrorExit(const char* inMex, bool bShutdown)
{
	MessageBox(NULL,inMex,"Errore",MB_OK|MB_ICONERROR);

	if(bShutdown)
	{
		CTeresaApp* pApp = (CTeresaApp*)AfxGetApp();
		pApp->Shutdown();
	}
}


// EOF Globals.cpp /////////////////////////////////////////////////////////////
