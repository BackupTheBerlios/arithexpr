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

#ifndef ARITHEXPR_H
#define ARITHEXPR_H

#include <string>
#include <vector>
#include <cmath>

#include "functions.hpp"

using namespace std;

#define ERROR_SYNTAX           "1   Syntax Error"
#define ERROR_BRACKETORDER     "2   Confused about bracket order"
#define ERROR_WRONGPARAMCOUNT  "3   Wrong count of parameters"
#define ERROR_VALUEOUTOFRANGE  "4   Value out of range"
#define ERROR_PARAMISZERO      "5   Parameter is zero, and shouldn't be"
#define ERROR_PARAMNEGATIVE    "6   Parameter is negative"
#define ERROR_COMPLEXNUMBER    "7   The operation would result in a complex number"
#define ERROR_UNKNOWNFUNCTION  "8   Unknown function"
#define ERROR_UNKNOWNVARIABLE  "9   Unknown variable"

/**
 * Diese Struktur wird verwendet um Variablen/Konstanten
 * zu speichern
 */
class variable {
public:
	/** Der Variablenname */
	string name;
	/** Der Wert der Variablen */
	string value;
	/** Legt fest ob die Variable überschrieben/gelöscht werden darf */
	bool readonly;
};

/**
 * \brief Klasse zum parsen arithmetischer Ausdrücke
 * 
 * Die Klasse dient dazu arithmetische Ausdrücke
 * die in einem string zur Verfügung stehen
 * zu parsen und auszuwerten.
 */
class arithExpr {
    private:
    public:
        string expr;
        string error;
	vector<function> functions;
	vector<variable> variables;
        
        /** 
	 * \brief Konstruktor
	 * 
         * \param e ist ein arithmetischer Ausdruck in einem String
         */
        arithExpr(string e, vector<variable> *newvars=NULL);
        
        /**
         * Gibt zurück ob und welcher Fehler während
         * dem initialisieren der Klasse aufgetreten ist.
         * \return Die Funktion gibt einen Zeiger auf die
         * Fehlerbeschreibung zurück.
         */
        string lastError();

	/**
	 * \brief Mit dieser Funktion kann man neue Funktionen
	 * registrieren, die dann in Ausdrücken verwendet werden können.
	 * \param name ist der Name der Funktion
	 * \param parameter ist eine Liste von variablen, die in \p
	 * term vorkommen, und von denen die Funktion abhängig ist (Konstanten
	 * die mit addVariable() hinzugefügt wurden, können aber in Funktionen
	 * auch verwendet werden).
	 * \param term ist der Funktionsterm. In ihm sollten u.a. die mit
	 * \p parameter übergebenen Parameter vorkommen ;-)
	 * \param make_readonly und
	 * \param overwrite entspricht addVariable()
	 * \sa addVariable(), function
	 */
	bool addFunction(string name, vector<string> parameter, string  term, bool make_readonly=false, bool overwrite=true);

	/**
	 * \brief Die Funktion gibt die ID einer Funktion mit dem Namen
	 * \p namen, in \p functions zurück.
	 * \param name ist der Name der zu suchenden Funktion
	 * \return Zurückgegeben wird die ID der Funktion in \p functions
	 * oder -1 wenn sie nicht gefunden wurde.
	 */
	int getFunctionId(string name);
	
	/**
	 * \brief Mit der Funktion kann eine neue Variable definiert
	 * werden.
	 *
	 * \param name ist der Name der Variable die angelegt werden soll.
	 * \param value ist deren Wert (ebenfalls als String!)
	 * \param make_readonly gibt an ob die Variable nachträglich noch
	 * verändert oder gelöscht werden kann.
	 * \param overwrite legt fest ob, wenn schon eine Variable mit
	 * dem Namen exisitert, diese überschrieben werden soll oder nicht.
	 * \return Wenn die Variable erfolgreich gespeichert wurde gibt die
	 * Funktion \p true zurück. Ansonsten false.
	 */
	bool addVariable(string name, string  value, bool make_readonly=false, bool overwrite=true);

	/**
	 * \brief Die Funktion gibt die ID der Variable mit dem Namen
	 * \p name zurück.
	 *
	 * \param name ist der Name der Variable, dessen ID zurück gegeben
	 * werden soll.
	 * \return Die Funktion gibt die ID der Variablen zurück oder -1,
	 * wenn die Variable nicht gefunden wurde.
	 */
	int getVariableId(string name);
        
        /**
         * Die Funktion erweitert expr um ein paar Kleinigkeiten.
         * Z.B. fügt sie vor "-"-Zeichen vor denen keine Zahl 
         * steht "0" ein.
         */
        void expandExpression();
        
        /**
         * Prüft expr auf Syntax und vorhandensein aller Funktionen.
         * Wird ein Fehler entdeckt kann dieser mit lastError()
         * ermittelt werden.
         * \sa lastError()
         */
        bool validateExpression();
        
        /**
        * Löscht (falls vorhanden) die äußeren Klammern von
        * expr und gibt true zurück wenn welche vorhanden waren
        * \return true wenn äußere Klammern entfernt wurden false
        * wenn es keine äußeren Klammern (mehr) gibt
        */
        bool deleteBrackets();
        
        /**
         * Berrechnet (rekursiv) das Ergebnis von expr.
         */
        double arithExpr::calculate();
        
        /**
         * Die Funktion ersetzt alles was bei dem übergebenen string
         * in Klammern steht durch "0"en und gibt den string zurück.
         * \param e ist ein arithmetischer Ausdruck (mit Klammern)
         * \return Zurückgegeben wird e mit durch "0"en ersetzten Klammer
         * Inhalt. Z.B. wenn man "(a+b*c)*d" an die Funktion übergibt,
         * gibt sie (00000)*d zurück.
         */
         string replaceBrackets(string e);
         
         
         // Hilfsfunktionen
         
         /**
          * Die Funktion durchsucht str der Reihe nach nach den
          * Elementen von words und gibt die Position des
          * ersten gefundenen Elements zurück.
          * \param str ist der zu duchsuchende String.
          * \param words ist eine Liste von Strings, nach denen
          * gesucht werden soll.
	  * \param id ist ein Zeiger auf eine int-Variable, in der
	  * die ID des gefundenen Strings in words gespeichert wird.
          * \return Zurückgegeben wird die Position des gefundenen
          * Wortes oder string::npos wenn nichts gefunden wird.
          */
         unsigned int findFirstOf(string str, vector<string> words, int *id);
         
         /**
          * Die Funktion gibt die Position von item in words
          * zurück.
          * \param words eine Liste von Strings
          * \param item ist der String nach dem words durchsucht werden soll
          * \return Die Position von item in words oder -1 wenn item nicht
          * gefunden wurde.
          */
         int IdOf(vector<string> words, string item);

	 /**
	  * \brief Die Funktion prüft ob der übergebene String eine Zahl ist.
	  *
	  * \param str ist der zu prüfende String
	  * \return Wenn der str eine Zahl enthält wird \p true ansonsten
	  * \p false zurückgegben.
	  */
	 bool isNumber(string str);
         
};

#endif

/**
 * \mainpage
 *
 * \section inhalt Inhalt
 *  - \ref einleitung
 *  - \ref benutzung
 *  - \ref ausdruck
 *    - \ref ausdruck_beachten
 *    - \ref ausdruck_operatoren
 *      - \ref rechen_operatoren
 *      - \ref vergleichs_operatoren
 *  - \ref helfen
 *  - \ref copyright
 *  - \ref kontakt
 * 
 * \section einleitung Einleitung
 * Diese Klasse dient dazu, arithmetische Ausdrücke wie "4^2-3*(sin(5)+ln(2))" auszuwerten
 * bzw. auszurechnen. Sie ist sehr einfach zu benutzen: man muss sie nur initialisieren und
 * die Methode calculate() aufrufen, um das Resultat zu erhalten.
 * 
 * \section benutzung Benutzung
 * Die Benutzung anhand eines Beispiels (da man eigendlich nur 2 Methoden benötigt um die
 * Klasse zu benutzen ist das ausreichend denke ich):
 * \include main.cpp
 * 
 * Beim linken ist wichtig, \p -lm als Parameter zu übergeben, da arithExpr die
 * "cmath"-Bibliothek verwendet.
 * Mit dem \p g++ würde das kompilieren/linken zum Beispiel so gehen:
 * \code
 * g++ -c *.cpp
 * g++ -lm *.o -o example_program
 * \endcode
 *
 * \section ausdruck Was zählt als arithmetischer Ausdruck?
 * Bis jetzt kann man nur Terme ohne Variablen und ohne eigene Funktionen (wie f(x) im
 * allgemeinen eine ist) verwenden. Beispiele für gültige Ausdrücke wären:
 * \li \p "2^3+4" -> 12
 * \li \p "sin(4)/cos(4)=tan(4)" -> 1
 * \li \p "2.71828183^ln(17)" -> 17
 * \li \p "12*(4+6^(2/3)*2)" -> 127.246
 *
 * \subsection ausdruck_beachten Folgendes sollte (/muss) man beachten
 * \li Leerzeichen in einem Ausdruck sollten vermieden werden
 * \li Funktionsparameter müssen \em immer in Klammern stehen
 * \li Sollte ein Exponent negativ sein, \em muss er in Klammern (z.B. richtig: "3^(-2)"; falsch: "3^-2")
 * \li Operatoren müssen \em immer geschrieben werden (z.B. darf man nicht, wie in der Mathematik
 * üblich, den Malpunk, bzw. das Sternchen zwischen Zahlen und Klammern weglassen)
 * 
 * \subsection ausdruck_operatoren Welche Operatoren und Funktionen gibt es?
 * Bis jetzt wurden folgende Operatoren und Funktionen implementiert.
 * In Zukunft soll noch die Möglichkeit hinzukommen einfach eigene Funktionen zu definieren,
 * im Moment muss man sich aber noch mit diesen abfinden.
 *
 * \subsubsection rechen_operatoren "Rechen"-Operatoren und Funktionen
 * Sie geben das Ergebnis der entsprechenden Operation zurück.
 * 
 *  - +, -, *, / \n
 *  Die üblichen arithmetischen Operatoren\n
 *  Beispiele: \p "4+5", \p "4/5"
 * 
 *  - ^, ln() \n
 *  Potenz und (natürlicher) Logarithmus\n
 *  Beispiele: \p "4^2", \p "ln(3)"
 * 
 *  - sin(), cos(), tan() \n
 *  Sinus, Cosinus und Tangens \n
 *  Beispiele: \p sin(3), \p tan(6)
 *
 *  - asin(), acos(), atan() \n
 *  Die Umkehrfunktionen von Sinus, Cosinus und Tangens \n
 *  Beispiele: \p acos(4)
 *
 *  - abs() \n
 *  abs() liefert den Betrag eines Ausdrucks \n
 *  Beispiele: \p abs(4) -> 4, \p abs(-5) -> 5
 *
 *  - if(bedingung,dann,sonst) \n
 *  \p bedingung muss ein Ausdruck sein, der einen
 *  \ref vergleichs_operatoren Vergleichsoperator enthält.
 *  Ist der Ausdruck wahr (z.B. 2=2), dann ergibt die Operation
 *  den Wert des Ausdrucks \p dann, wenn nicht ergibt die
 *  den Wert von \p sonst. \n
 *  Beispiele: \p if(3=3,4,5) -> 4, \p if(4<3,1,6) -> 6
 *
 * \subsubsection vergleichs_operatoren Vergleichsoperatoren
 * Sie geben wenn der Vergleich wahr ist \p 1, falls er falsch ist \p 0
 * zurück. Vergleichsoperatoren gibt es folgende:
 *
 * \p = ("gleich"), \p >= ("größer oder gleich"), \p <=
 * ("kleiner oder gleich"), \p < ("kleiner"), \p > ("größer"),
 * \p != ("ungleich")
 *
 * Die Vergleichsoperatoren werden alle gleich Benutzt:
 * \p [operand1][operator][operand2]
 * 
 * \b Beispiele:
 *  - "4=4" -> 1
 *  - "4>4" -> 0
 *  - "5<=9" -> 1
 *
 * \section helfen Ich will helfen!
 * Wenn du an dem Projekt mithelfen willst melde dich bei mir
 * (siehe \ref kontakt), schicke mir Fehler, Anmerkungen,
 * mathematische Funktionen die noch nicht implementiert sind,
 * oder was sonst noch nützlich sein sollte.
 * Verwende die Klasse und helfe so Fehler zu finden, 
 * verbessere sie ggf. und schicke sie mir zu.
 * Übersetzungen der Dokumentation in beliebige Sprachen
 * sind auch immer willkommen.
 *
 * \section copyright Copyright
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
 *
 * \section kontakt Kontakt
 * Unter meiner eMail-Addresse \b cypher@user.berlios.de
 * bin ich eigendlich immer zu erreichen.
 * Noch schneller gehts meistens per ICQ (http://www.icq.com/).
 * Meine Nummer ist \b 229442050. Mehr Informationen gibts auf der Projektseite
 * http://developer.berlios.de/projects/arithexpr/
 */

/**
 * \example main.cpp
 * Das ist ein Beispiel zur Benutzung von arithExpr.
 */
