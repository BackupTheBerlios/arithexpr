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
 * <a href="http://www.gnu.org/copyleft/gpl.html">GNU General Public License</a>
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301, USA.
 */

#include "arithexpr.hpp"

arithExpr::arithExpr(string e, vector<variable> *newvars) {
    expr = e;
    error = "";
    if (newvars != NULL)
	    variables = *newvars;
    /** \TODO Execute needed functions in the right order */
    expandExpression();
    if (validateExpression()) {
        deleteBrackets();
        
    }
    else {
       error = ERROR_SYNTAX;
    }
}

void arithExpr::expandExpression() {
    /** Leerzeichen löschen */ // Irgendwas ist komisch...
    unsigned int pos = 0;
    while ((pos=expr.find(" ")) != string::npos) {
        expr.replace(pos, 1, "");  
    }
    
    /* Vor "alleinstehende" "-"-Zeichen "0"en setzen */
    if (expr[0] == '-') {
        expr = "0" + expr;
    }
    
    pos=0;
    while ((pos = expr.find("-", pos+1)) != string::npos) {
        if (expr[pos-1] == '(') {
            expr = expr.substr(0, pos) + "0" + expr.substr(pos);
        }
    }
}

bool arithExpr::validateExpression() {
    /* Prüfen ob alle Klammern geschlossen wurden */
    int open_brackets=0;
    for (int i=0; i<expr.size(); i++) {
        if (expr[i] == '(')
            open_brackets++;
        if (expr[i] == ')')
            open_brackets--;
    }
    if (open_brackets != 0)
       return false;
    
    return true;
}

bool arithExpr::deleteBrackets() {
    int open_brackets;
    bool candelete=true;
  
    /* Prüfen ob expr überhaupt mit einer Klammer beginnt */
    if ((expr[0] == '(') && (expr[expr.size()-1] == ')')) {
        open_brackets = 1;
    }
    else {
        return false;
    }
  
    for (int i=1; i<expr.size()-1; i++) {
        if (expr[i] == '(')
            open_brackets++;
        if (expr[i] == ')')
            open_brackets--;
      
        if (open_brackets == 0)
          candelete = false;
    }
    if (candelete) {
        expr = expr.substr(1, expr.size()-2);
    } 
}

double arithExpr::calculate() {
    string tmp = expr;
    unsigned int pos;
    tmp = replaceBrackets(tmp);  // Klammerinhalte durch "0"en ersetzen
    arithExpr *param1, *param2;
    
    int op;
    vector<string> ops;
    /* In dieser Liste MUSS die Priorität der Operatoren stimmen !!!!!
     * Die die zuletzt bearbeitet werden kommen zuerst.
     * Deswegen z.B. erst "+", dann "*".
     * Außerdem müssen längere Operatoren vor kürzeren kommen!
     */
    /* Vergleichsoperatoren */
    ops.push_back("!="); 
    ops.push_back("<=");
    ops.push_back(">="); 
    ops.push_back("=");
    ops.push_back(">");
    ops.push_back("<");
    /* arithmetische Operationen */
    ops.push_back("+");
    ops.push_back("-");
    ops.push_back("*");
    ops.push_back("/");
    ops.push_back("^");
    if ((pos = findFirstOf(tmp, ops, &op)) != string::npos)
    {
        param1 = new arithExpr(expr.substr(0, pos), &variables);
        param2 = new arithExpr(expr.substr(pos+1), &variables);
        if (op ==  IdOf(ops, "+"))
            return (param1->calculate() + param2->calculate());
        if (op == IdOf(ops, "-"))
            return (param1->calculate() - param2->calculate());
        if (op == IdOf(ops, "*"))
            return (param1->calculate() * param2->calculate());
        if (op == IdOf(ops, "/"))
            return (param1->calculate() / param2->calculate());
        if (op == IdOf(ops, "^")) {
            double x1 = param1->calculate();
            double x2 = param2->calculate();
            if ((x1<0) && (x2-round(x2)!=0)) {
                error = ERROR_COMPLEXNUMBER;
                return 0;
            }
            return pow(x1, x2);
        }
        if (op == IdOf(ops, "="))
            return (param1->calculate() == param2->calculate());
        if (op == IdOf(ops, ">"))
            return (param1->calculate() > param2->calculate());
        if (op == IdOf(ops, "<"))
            return (param1->calculate() < param2->calculate());
        if (op == IdOf(ops, ">="))
            return (param1->calculate() >= param2->calculate());
        if (op == IdOf(ops, "<="))
            return (param1->calculate() <= param2->calculate());
        if (op == IdOf(ops, "!="))
            return (param1->calculate() != param2->calculate());
    }
    
    /* Funktionen */
    if (expr.find("(", 0) != string::npos) {
        string rawparams, funcName;
        vector<string> params;
        int bracket1, bracket2;
        
        bracket1 = expr.find("(", 0);
        bracket2=expr.find(")", 0);
        
        if (bracket1 >= bracket2) {
            error = ERROR_BRACKETORDER;
            return 0;
        }
        rawparams = expr.substr(bracket1+1, bracket2-bracket1-1);
        rawparams = "," + rawparams + ",";
        
        int pos1=0, pos2;
        while ((pos2=rawparams.find(",", pos1+1)) != string::npos) {
            params.push_back(rawparams.substr(pos1+1, pos2-pos1-1));
            pos1 = pos2;
        }
        
        funcName = expr.substr(0, bracket1);
        
        // Parameter zur weiterverarbeitung initialisieren
        vector<arithExpr *> parameter(params.size());
        for (int i=0; i<parameter.size(); i++) {
            parameter[i] = new arithExpr(params[i], &variables);
        }
        
        if (funcName == "sin") {
            if (parameter.size() != 1) {
                error = ERROR_WRONGPARAMCOUNT;
                return 0;
            }
            return sin(parameter[0]->calculate());
        }
        if (funcName == "asin") {
            if (parameter.size() != 1) {
                error = ERROR_WRONGPARAMCOUNT;
                return 0;
            }
            double x=parameter[0]->calculate();
            if ((x<-1) || (x>1)) {
                error = ERROR_VALUEOUTOFRANGE;
                return 0;
            }
            
            return asin(x);
        }
        if (funcName == "cos") {
            if (parameter.size() != 1) {
                error = ERROR_WRONGPARAMCOUNT;
                return 0;
            }
            return cos(parameter[0]->calculate());
        }
        if (funcName == "acos") {
            if (parameter.size() != 1) {
                error = ERROR_WRONGPARAMCOUNT;
                return 0;
            }
            double x=parameter[0]->calculate();
            if ((x<-1) || (x>1)) {
                error = ERROR_VALUEOUTOFRANGE;
                return 0;
            }
            
            return acos(parameter[0]->calculate());
        }
        if (funcName == "tan") {
            if (parameter.size() != 1) {
                error = ERROR_WRONGPARAMCOUNT;
                return 0;
            }
            return tan(parameter[0]->calculate());
        }
        if (funcName == "atan") {
            if (parameter.size() != 1) {
                error = ERROR_WRONGPARAMCOUNT;
                return 0;
            }
            double x=parameter[0]->calculate();
            if ((x<-1) || (x>1)) {
                error = ERROR_VALUEOUTOFRANGE;
                return 0;
            }
            
            return atan(parameter[0]->calculate());
        }
        if (funcName == "ln") {
            if (parameter.size() != 1) {
                error = ERROR_WRONGPARAMCOUNT;
                return 0;
            }
            double x = parameter[0]->calculate();
            if (x < 0) {
                error = ERROR_PARAMISZERO;
                return 0;
            }
            if (x == 0) {
                error = ERROR_PARAMNEGATIVE;
                return 0;
            }
            return log(x);
        }
	if (funcName == "abs") {
            if (parameter.size() != 1) {
                error = ERROR_WRONGPARAMCOUNT;
                return 0;
            }
	    double x = parameter[0]->calculate();
	    if (x >= 0)
		    return x;
	    else
		    return -x;
	}
	if (funcName == "if") {
            if (parameter.size() != 3) {
                error = ERROR_WRONGPARAMCOUNT;
                return 0;
	    }
	    if (parameter[0]->calculate() == 1)
		    return parameter[1]->calculate();
	    else  return parameter[2]->calculate();
	}
	
	error = ERROR_UNKNOWNFUNCTION;
	return 0;
    }

    /* Wenn der Ausdruck eine Zahl ist gib sie zurück */
    if (isNumber(expr))
	    return atof(expr.c_str());
   
    int id;
    if ((id=getVariableId(expr)) != -1) {
        if (isNumber(variables[id].value))
            return atof(variables[id].value.c_str());
    }
    
    error = ERROR_UNKNOWNVARIABLE;
    return 0;
}

int arithExpr::getVariableId(string name) {
	for (int i=0; i<variables.size();i++) {
		if (variables[i].name == name)
			return i;
	}
	return -1;
}

bool arithExpr::addVariable(string name, string value, bool make_readonly, bool overwrite) {
	int id = getVariableId(name);

	if (!overwrite && (id > 0))
		return false;
	
	if (id == -1) {
		variable newvar;
		newvar.name = name;
		newvar.value = value;
		newvar.readonly = make_readonly;
		
		variables.push_back(newvar);
		return true;
	}
	
	if (variables[id].readonly)
		return 0;
	
	variables[id].value = value;
	variables[id].readonly = make_readonly;
	return true;
}

bool arithExpr::isNumber(string str) {
	for (int i=0; i<str.size(); i++) {
		if (!isdigit(str[i]) && (str[i] != '.'))
			return false;
	}
	return true;
}

string arithExpr::replaceBrackets(string e) {
    int open_brackets = 0;
    for (int i=0; i<e.size(); i++) {
        switch (e[i]) {
            case '(': 
                open_brackets++;
                break;
            case ')':
                open_brackets--;
                break;
            default:
                if (open_brackets > 0)
                    e[i] = '0';
                break;
        }
    }
    return e;
}

string arithExpr::lastError() {
    return error;
}

unsigned int arithExpr::findFirstOf(string str, vector<string> words, int *id) {
    unsigned int pos = string::npos;
    for (int i=0; i<words.size(); i++) {
        pos = str.find(words[i], 0);
        if (pos != string::npos)
        {
            *id = i;
            return pos;
        }
    }
    *id = -1;
    return pos;
}

int arithExpr::IdOf(vector<string> words, string item) {
    for (int i=0; i<words.size(); i++) {
        if (words[i] == item)
           return i;
    }
    return -1;
}
