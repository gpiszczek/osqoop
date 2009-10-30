/*

This file is part of the TSEAdExt board firmware.
It is released as an example only, as a part of Osqoop.
Osqoop, an open source software oscilloscope.
Copyright (C) 2006--2009 Stephane Magnenat <stephane at magnenat dot net>
http://stephane.magnenat.net
Laboratory of Digital Systems
http://www.eig.ch/fr/laboratoires/systemes-numeriques/
Engineering School of Geneva
http://hepia.hesge.ch/
See authors file in source distribution for details about contributors

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
//  Fichier     : debug.h
//  Cr�e        : 23.11.2005
//  Modifi�     : 
//  Auteur(s)   : St�phane Magnenat
//  Cible       : FX2 (compilateur Keil)
//
//  Description : Debug helper macros
//
//#############################################################################

#ifndef __DEBUG_H__
#define __DEBUG_H__

#include "serial.h"

// Enable/Disable debug
//#define DEBUG_ENABLED

#ifdef DEBUG_ENABLED
	#define DEBUG_PRINT(s) TermPuts(s)
	#define DEBUG_PRINTI(i)	TermPuti(i)
	#define DEBUG_INIT TermInit();
#else
	#define DEBUG_PRINT(s)
	#define DEBUG_PRINTI(i)
	#define DEBUG_INIT 
#endif

#endif // !defined(__SERIAL_H__)