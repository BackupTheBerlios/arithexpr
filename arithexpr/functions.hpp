/**
 * \file
 * This is a class written in c++, wich provides the possibility
 * to parse arithmetic expressions and calculate their result
 * very easily.
 * Copyright (C) 2005  Bodo "cypher" Akdeniz
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the 
 * GNU General Public License (http://www.gnu.org/copyleft/gpl.html)
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301, USA.
 */

#ifndef FUNCTIONS_HPP
#define FUNCTIONS_HPP

#include <string>
#include <vector>
#include <iostream> // MUSS WIEDER RAUS!!!

#define FUNC_ERROR_PARAMCOUNT "function 1   Not enough parameters"

using namespace std;

/**
 * Die Klasse wird verwendet um benutzerdefinierte
 * Funktionen zu speichern.
 */
class function {
public:
	/** Legt fest ob die Funktion überschrieben oder
	 * gelöscht werden darf.
	 */
	bool readonly;
	
	/** Der Funktionsname */
	string name;
	
	/** Parameter die der Funktion übergeben werden können.
	 * Sie müssen dann auch in term vorkommen (oder es wäre auf
	 * jeden Fall sinnvoll ;-))
	 */
	vector<string> parameter;
	
	/** Der Funktionsterm */
	string term;
	
	/** Ein vordefinierten Ergebniss, für einen
	 * bestimmtenAufruf z.B. f(0)=1
	 * Wird für rekursive Funktionen benötigt.
	 */
	string def_parameter;
	string def_value;

	/**
	 * \brief Die Funktion ersetzt die Variablen
	 * in der Funktion durch die übergebenen
	 * Parameter und gibt sie als String zurück.
	 * \param params ist eine Liste mit Parametern
	 * \return Zurückgegeben wird der Funktionsterm
	 * mit den ersetzten Variabeln oder
	 * \p FUNC_ERROR_PARAMCOUNT wenn die falsche
	 * Anzahl Parameter übergeben wurden.
	 */
	string getReplaced(vector<string> params);
};

#endif
