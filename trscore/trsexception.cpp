/*//////////////////////////////////////////////////////////////////////////////
	FILENAME: trsexception.cpp
	Version:  1.0.0
	Author:   Marco Bortolin, Alberto Minora
	Created:  02/08/2001 - 17.02.34

	Copyright (C) 2001 Marco Bortolin, Alberto Minora
//////////////////////////////////////////////////////////////////////////////*/
#ifdef WIN32
#pragma warning(disable:4786)
#pragma warning(disable:4100)
#endif
#include "trs.h"
#include <iostream>
#include <cstdarg>
#include <cstdio>


/*- trsException::trsException ---------------------------------------

	Costruttore 1
--------------------------------------------------------------------*/
trsException::trsException(int code, string& msg)
{
	m_code = code;
	m_msg = msg;
}


/*- trsException::trsException ---------------------------------------

	Costruttore 2. Argomenti variabili.
--------------------------------------------------------------------*/
trsException::trsException(int code, const char* format, ...)
{
	va_list ap;

	m_code = code;
	m_msg = "";
	va_start(ap, format);
	Printmsg(format,ap);
	va_end(ap);
}


/*- trsException::DebugPrint -----------------------------------------

	Stampa il messaggio d'errore su cerr
--------------------------------------------------------------------*/
void trsException::DebugPrint()
{
	cerr << "CODE: " << m_code << "\nMSGCHAIN:\n" << m_msg;
}


/*- trsException::AppendStr ------------------------------------------

	Appende una stringa a quella gia' memorizzata.
--------------------------------------------------------------------*/
void trsException::AppendStr(string& msg)
{
	m_msg += "\n";
	m_msg += msg;
}


/*- trsException::AppendStr ------------------------------------------

	Appende una stringa a quella gia' memorizzata.
	Argomenti variabili.
--------------------------------------------------------------------*/
void trsException::AppendStr(const char* format, ...)
{
	va_list ap;
	
	m_msg += "\n";
	va_start(ap, format);
	Printmsg(format,ap);
	va_end(ap);
}


/*- trsException::Printmsg -------------------------------------------

	(funzione privata: utilizza la funzione di libreria vsprintf per
	trattare gli argomenti variabili)
--------------------------------------------------------------------*/
void trsException::Printmsg(const char* format, va_list ap)
{
	//@@ buffer un po' ristretto...
	char buf[256];

	vsprintf(buf,format,ap);
	m_msg += buf;
}


// EOF trsException.cpp //////////////////////////////////////////////////////////
