#include "functions.hpp"

string function::getReplaced(vector<string> params) {
	if (parameter.size() != params.size())
		return FUNC_ERROR_PARAMCOUNT;

	string t = term;
	int pos;
	for (int i=0; i<params.size(); i++) {
		pos = -1;
		while ((pos=t.find(parameter[i], pos+1)) != string::npos) {
			if ( ((pos==0) || !isalpha(t[pos-1])) && ((pos==t.size()-parameter[i].size()) || !isalpha(t[pos+parameter[i].size()])) ) {
				
				t.replace(pos, parameter[i].size(), params[i]);
			}
		}
	}
	return t;
}
