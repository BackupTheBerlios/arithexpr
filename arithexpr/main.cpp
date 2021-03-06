#include <cstdlib>
#include <iostream>
#include <vector>

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
     * Diesem wird der arithmetische Ausdruck als String �bergeben.
     */
    ae = new arithExpr(e);

    /* F�gt dem Objekt eine neue Variable hinzu, die
     * dann in Berechnungen verwendet werden kann.
     */
    ae->addVariable("e", "2.71828183", true);

    /* Nun f�gen wir dem Objekt noch eine Funktion hinzu */
    vector<string> params;
    params.push_back("x");
    ae->addFunction("f", params, "x^2");
    
    /* und noch eine */
    params.clear();
    params.push_back("x");
    ae->addFunction("g", params, "f(x-1)+5");
    
    /* expr ist die Eigendschaft von arithExpr, die den
     * (unter Umst�nden) leicht ver�nderteten Audruck enth�lt.
     * (Es werden zum Beispiel vor einzelne "-"-Zeichen
     * "0"en gesetzt, dass der Parser richtig damit umgehen kann.
     */
    cout << ae->expr << endl;
    
    /* calculate() berechnet das Ergebnis des Ausdrucks und gibt
     * es als double Wert zur�ck.
     */
    cout << ae->expr << " = " << ae->calculate() << endl;
    /* Wenn w�rend der Berrechnung ein Fehler aufgetreten ist
     * kann man diesen mit lastError() ermitteln. lastError()
     * gibt einen String zur�ck, an dessen Anfang
     * eine Fehlernummer steht, gefolgt von einer kurzen
     * Beschreibung des Fehlers.
     * Ist kein Fehler aufgetreten gibt lastError() einen
     * leeren String ("") zur�ck.
     */
    if (ae->lastError() != "")
        cout << "An error occured: " << ae->lastError() << endl;
    
    system("PAUSE"); // comment this line when using linux
    return EXIT_SUCCESS;
}
