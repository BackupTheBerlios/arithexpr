#include <cstdlib>
#include <iostream>

/* Inkludieren, der arithExpr Headerdatei */
#include "arithexpr.hpp"

using namespace std;

int main(int argc, char *argv[])
{
    /* Anlegen eines Zeigers auf ein arithExpr Objekt */
    arithExpr *ae;
    string e;
    
    cout << "Please give an arithmetic expression: ";
    cin >> e;
    
    /* Initiieren des Objekts und aufrufen des Konstruktors.
     * Diesem wird der arithmetische Ausdruck als String übergeben.
     */
    ae = new arithExpr(e);
    
    /* expr ist die Eigendschaft von arithExpr, die den
     * (unter Umständen) leicht veränderteten Audruck enthält.
     * (Es werden zum Beispiel vor einzelne "-"-Zeichen
     * "0"en gesetzt, dass der Parser richtig damit umgehen kann.
     */
    cout << ae->expr << endl;
    
    /* calculate() berechnet das Ergebnis des Ausdrucks und gibt
     * es als double Wert zurück.
     */
    cout << ae->expr << " = " << ae->calculate() << endl;
    /* Wenn wärend der Berrechnung ein Fehler aufgetreten ist
     * kann man diesen mit lastError() ermitteln. lastError()
     * gibt einen String zurück, an dessen Anfang
     * eine Fehlernummer steht, gefolgt von einer kurzen
     * Beschreibung des Fehlers.
     * Ist kein Fehler aufgetreten gibt lastError() einen
     * leeren String ("") zurück.
     */
    if (ae->lastError() != "")
        cout << "An error occured: " << ae->lastError() << endl;
    
    system("PAUSE"); // comment this line when using linux
    return EXIT_SUCCESS;
}
