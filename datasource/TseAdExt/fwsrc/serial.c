/*

This file is part of the TSEAdExt board firmware.
It is released as an example only, as a part of Osqoop.
Osqoop, an open source software oscilloscope.
Copyright (C) 2006 Stephane Magnenat <stephane at magnenat dot net>
Laboratory of Digital Systems http://www.eig.ch/labsynum.htm
Engineering School of Geneva http://www.eig.ch

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

*/

//#############################################################################
//
//  HES-SO EIG Laboratoire de Syst�me Num�riques
//
//  Fichier     : hal.c
//  Cr�e        : 14.11.2003
//  Modifi�     : 26.02.2005
//  Auteur(s)   : G.Wisner
//  Cible       : FX2 (compilateur Keil)
//
//  Description : Abstraction CPU.
//
//#############################################################################



//#############################################################################
//
//                              SUPPORT TERMINAL
//
//#############################################################################


#include "fx2.h"
#include "fx2regs.h"
#include <types.h>
#include <string.h>  // fonction strlen()

#ifndef __TERM_PORT__
   #define __TERM_PORT__ 0
#endif

#if __TERM_PORT__ == 0
   #define RCTL PCON
   #define SBUF SBUF0
   #define SCON SCON0
   #define SCON_TI TI
   #define SCON_RI RI
#elif __TERM_PORT__ == 1
   #define RCTL EICON
   #define SBUF SBUF1
   #define SCON SCON1
   #define SCON_TI TI1
   #define SCON_RI RI1
#else
   #error
#endif

#define NCOL 8
static CHAR xdata StrBuf[10+(NCOL*5)+1];

#define Dig2Char(d) ((d) > 9) ? (d)+'A'-10 : (d)+'0'


//=============================================================================
// Routine     : itoa()
//
// Description : Convertit un entier en une cha�ne de caract�re et stock le
//               r�sultat vers la location point�e par l'argument "string".
//               L'argument "radix" est ignor� (repr�sentation hexa toujours
//               utilis�e).
//
// e           : value - valeur enti�re dont on veut la repr�sentation
//               string - pointeur sur le tampon de destination (! � la taille)
//               radix - ignor� (hexa toujours consid�r�)
// s           : pointeur sur la cha�ne (tampon de destination)
//=============================================================================
static PCHAR itoa(LONG value, PCHAR string /*, LONG radix*/)
{
   BYTE  xdata DigNum;
   PCHAR xdata pStr = string;

   if ((ULONG)value < 0x100) DigNum = 2;
   else if ((ULONG)value < 0x10000) DigNum = 4;
   else DigNum = 8;

   while (DigNum--) *pStr++ = (CHAR) Dig2Char((value >> (4*DigNum)) & 0xF);
   *pStr = '\0';

   return (string);
}

//=============================================================================
// Routine     : TermInit()
//
// Description : Initialise le port s�rie avec les param�tres : 115200 bits/s ,
//               sans parit� , sans contr�le de flux
//=============================================================================
VOID TermInit(VOID)
{
   SCON  = 0x50;
   UART230 |= (1 << __TERM_PORT__);
   RCTL &= ~0x80;
   SCON_TI = 1;
}

//=============================================================================
// Routine     : TermGetc()
//
// Description : Lecture d'un caract�re � partir de l'interface de communica-
//               tion s�rie asynchrone (polling mode). Cette routine est non
//               blocante. Si il n'y a pas de caract�re disponible lors de
//               l'appel, la routine retourne -1.
//
// s           : caract�re obtenu de l'interface (-1 si pas de carac. dispo.)
//=============================================================================
INT TermGetc(VOID)
{
   INT c = (INT)(-1);

   if (SCON_RI) {
      c = (INT)SBUF;
      SCON_RI = 0;
   }
   return c;
}

//=============================================================================
// Routine     : TermPutc()
//
// Description : Ecriture d'un caract�re vers l'interface de communication s�-
//               rie asynchrone (polling mode). Cette routine est blocante.
//               Elle ne retourne que lorsque le caract�re transmis en argument
//               a pu �tre �crit vers l'interface.
//
// e           : c - caract�re � �crire
// s           : caract�re �crit
//=============================================================================
CHAR TermPutc(CHAR c)
{
   if (c == '\n')  {
      while (!SCON_TI);
      SCON_TI = 0;
      SBUF = '\r';
   }
   while (!SCON_TI);
   SCON_TI = 0;
   return (SBUF = c);
}

//=============================================================================
// Routine     : TermPuts()
//
// Description : Ecriture d'une cha�ne de caract�res vers l'interface de com-
//               munication s�rie asynchrone (polling mode). Le caract�re de
//               fin de cha�ne n'est pas transmis.
//
//               remarque : TermPuts() ajoute l'�chappement '\n' � la cha�ne.
//
// e           : string - cha�ne de caract�res � �crire vers l'interface
//=============================================================================
VOID TermPuts(const PCHAR string)
{
   //EnterCS();
   while (*string) TermPutc(*string++);
   //ExitCS();
}

//=============================================================================
// Routine     : TermPuti()
//
// Description : Ecriture de la repr�sentation hexa d'un nombre vers l'inter-
//               face de communication s�rie asynchrone (polling mode).
//
// e           : v - nombre dont on veut transmettre la repr�sentation
//=============================================================================
VOID TermPuti(DWORD v)
{
   StrBuf[0] = '0';
   StrBuf[1] = 'x';
   itoa(v, &StrBuf[2] /*, 16*/);
   TermPuts(StrBuf);
}

//=============================================================================
// Routine     : TermDump()
//
// Description : Dump m�moire via l'interface de communication s�rie async.
//               Cette routine permet de visualiser sur un terminal "Len" oc-
//               tets m�moire � partir de l'adresse de base "pData".
//
// e           : pData - adresse de base de la zone m�moire � visualiser
//               Len   - nombre d'octets � visualiser
//=============================================================================
VOID TermDump(PBYTE pData, BYTE Len)
{
   BYTE  xdata Col;
   PCHAR xdata pStr;

   while (Len) {

      // adresse
      pStr = StrBuf;
      *pStr++ = '@';
      *pStr++ = ' ';
      *pStr++ = '0';
      *pStr++ = 'x';
      pStr += strlen(itoa((WORD)pData, pStr /*, 16*/));
	  *pStr++ = ' ';
	  *pStr++ = '|';

	  // 8 octets � compter de l'adresse
	  for (Col = 0; Col < NCOL; Col++) {
         *pStr++ = ' ';
         *pStr++ = '0';
         *pStr++ = 'x';
         itoa(*(PBYTE)pData++, pStr /*, 16*/);
		 pStr += 2;
		 if ( --Len == 0 ) break;
      }

      *pStr = '\0';
      TermPuts(StrBuf);
   }
}




