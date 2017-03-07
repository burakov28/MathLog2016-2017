#include <bits/stdc++.h>


using namespace std;


const int MAXLEN = 10000;

char str[MAXLEN];
int parenthesis[MAXLEN];

vector < string > hypothesis, evidence;
string header, needToProve;

bool isUpperCaseLetter(char c) {
    return ('A' <= c && c <= 'Z');
}

bool isLowerCaseLetter(char c) {
    return ('a' <= c && c <= 'z');
}

bool isDigit(char c) {
    return ('0' <= c && c <= '9');
}


struct expression {
    string name;
    vector < expression * > args;


    virtual string toString() {
        return "(" + args[0] -> toString() + ")" + name + "(" + args[1] -> toString() + ")"; 
    }
};


struct implication : expression {
    implication (expression * left, expression * right) {
        name = "->";
        args.push_back(left);
        args.push_back(right);
    }
};

struct disjunction : expression {
    disjunction (expression * left, expression * right) {
        name = "|";
        args.push_back(left);
        args.push_back(right);
    }
};

struct conjunction : expression {
    conjunction (expression * left, expression * right) {
        name = "&";
        args.push_back(left);
        args.push_back(right);
    }
};

struct variable : expression {
    variable (string const & _name, vector < expression * > const & _args) {
        args = _args;
        name = _name;
    }

    variable (string const & _name) {
        name = _name;
    }

    string toString() {
        string ret = name;
        if (args.size() > 0) {
            ret += "(";
            for (int i = 0; i + 1 < (int) args.size(); ++i) {
                ret += args[i] -> toString() + ",";
            }
            ret += args.back() -> toString();
            ret += ")";
        }
        return ret;
    }

};

struct zero : expression {
    zero() {
        name = "0";
    }

    string toString() {
        return name;
    }
};


struct neg : expression {
    neg(expression * _arg) {
        name = "!";
        args.push_back(_arg);
    }

    string toString() {
        return name + "(" + args[0] -> toString() + ")";
    }
};


struct predicate : expression {
    predicate(string const & _name, vector < expression * > const & _args) {
        args = _args;
        name = _name;
    }

    string toString() {
        if (name == "=") {
            return args[0] -> toString() + name + args[1] -> toString();
        }
        string ret = name;
        if (args.size() > 0) {
            ret += "(";
            for (int i = 0; i + 1 < (int) args.size(); ++i) {
                ret += args[i] -> toString() + ",";
            }
            ret += args.back() -> toString();
            ret += ")";
        }
        return ret;
    }
};


struct increase : expression {
    increase(expression * _arg) {
        args.push_back(_arg);
        name = "'";
    }

    string toString() {
        string cname = args[0] -> name;

        if (cname == "'" || cname == "0") {
            return args[0] -> toString() + name;
        }
        else {
            return "(" + args[0] -> toString() + ")" + name;
        }
    }
};


struct sum : expression {
    sum(expression * left, expression * right) {
        args.push_back(left);
        args.push_back(right);
        name = "+";
    }   
};

struct multiply : expression {
    multiply(expression * left, expression * right) {
        args.push_back(left);
        args.push_back(right);
        name = "*";
    }
};

struct quantifier : expression {
    quantifier(string const & _name, expression * var, expression * unary) {
        name = _name;
        args.push_back(var);
        args.push_back(unary);
    }

    string toString() {
        return name + args[0] -> toString() + "(" + args[1] -> toString() + ")";
    }
};

string parseVariable(string & expr, int & pos) {
    string name = "";
    while (pos < (int) expr.size() && (isLowerCaseLetter(expr[pos]) || isDigit(expr[pos]))) {
        name += expr[pos];
        ++pos;
    }
    return name;
}


expression * getIncrease(string & expr, int & pos, expression * x) {
    while (pos < (int) expr.size() && expr[pos] == '\'') {
        ++pos;
        x = new increase(x);
    }
    return x;
}


expression * getVariable(string & expr, int & pos) {
    return new variable(parseVariable(expr, pos), vector < expression * > ());
}


expression * getSum(string &, int &);


expression * getSimpleTerm(string & expr, int & pos) {
    if (expr[pos] == '0') {
        ++pos;
        return getIncrease(expr, pos, new zero());
    }

    if (expr[pos] == '(') {
        ++pos;
        expression * ret = getSum(expr, pos);
        ++pos;
        return getIncrease(expr, pos, ret);        
    }
    
    string name = parseVariable(expr, pos);
    vector < expression * > args;
    if (pos < (int) expr.size() && expr[pos] == '(') {
        ++pos;
        args.push_back(getSum(expr, pos));
        while (expr[pos] == ',') {
            ++pos;
            args.push_back(getSum(expr, pos));
        }
        ++pos;
    }
    return getIncrease(expr, pos, new variable(name, args));    
}


expression * getMultiply(string & expr, int & pos) {
    expression * ret = getSimpleTerm(expr, pos);
    while (pos < (int) expr.size() && expr[pos] == '*') {
        ++pos;
        ret = new multiply(ret, getSimpleTerm(expr, pos));
    }
    return ret;
}


expression * getSum(string & expr, int & pos) {
    expression * ret = getMultiply(expr, pos);
    while (pos < (int) expr.size() && expr[pos] == '+') {
        ++pos;
        ret = new sum(ret, getMultiply(expr, pos));
    }
    return ret;
}


bool isTerm(string & expr) {
    int pos = 0;
    getSum(expr, pos);
    if (pos == (int) expr.size()) return true;
    return false;
}


expression * getUnary(string &, int &);


expression * getQuantifier(string & expr, int & pos) {
    string name = "";
    name += expr[pos];
    ++pos;
    return new quantifier(name, getVariable(expr, pos), getUnary(expr, pos));
}

expression * getPredicate(string & expr, int & pos) {
    string name = "";
    
    vector < expression * > args;
    if (isUpperCaseLetter(expr[pos])) {
        while (pos < (int) expr.size() && (isUpperCaseLetter(expr[pos]) || isDigit(expr[pos]))) {
            name += expr[pos];
            ++pos;
        }
        

        if (pos < (int) expr.size() && expr[pos] == '(') {
            ++pos;
            args.push_back(getSum(expr, pos));
            while (expr[pos] == ',') {
                ++pos;
                args.push_back(getSum(expr, pos));
            }
            ++pos;            
        }

        return new predicate(name, args);
    }
    name = "=";
    args.push_back(getSum(expr, pos));
    assert(expr[pos] == '=');
    ++pos;
    args.push_back(getSum(expr, pos));
    return new predicate(name, args);
}


expression * getImplication(string &, int &);


expression * getUnary(string & expr, int & pos) {
    expression * ret;
    if (expr[pos] == '(') {
        int nxt = parenthesis[pos];
        
        string tmp = "";
        for (int i = pos; i <= nxt; ++i) {
            tmp += expr[i];
        }

        if (isTerm(tmp)) {
            return getPredicate(expr, pos);
        }        
        else {
            ++pos;
            ret = getImplication(expr, pos);
            ++pos;
            return ret;
        }
    }

    if (expr[pos] == '!') {
        ++pos;
        return new neg(getUnary(expr, pos));
    }

    if (expr[pos] == '@' || expr[pos] == '?') {
        return getQuantifier(expr, pos);
    }

    return getPredicate(expr, pos);
}


expression * getConjunction(string & expr, int & pos) {
    expression * ret = getUnary(expr, pos);
    while (pos < (int) expr.size() && expr[pos] == '&') {
        ++pos;
        ret = new conjunction(ret, getUnary(expr, pos));
    }
    return ret;
}


expression * getDisjunction(string & expr, int & pos) {
    expression * ret = getConjunction(expr, pos);
    while (pos < (int) expr.size() && expr[pos] == '|') {
        ++pos;
        ret = new disjunction(ret, getConjunction(expr, pos));
    }
    return ret;
}


expression * getImplication(string & expr, int & pos) {
    expression * left = getDisjunction(expr, pos);
    if (pos < (int) expr.size() && expr[pos] == '-') {
        pos += 2;
        return new implication(left, getImplication(expr, pos));
    }
    else {
        return left;
    }
}

bool isVariable(expression * expr) {
    return isLowerCaseLetter(expr -> name[0]) && expr -> args.size() == 0;
}



expression * parseExpression(string & expr) {
    int pos = 0;
    vector < int > st;
    for (int i = 0; i < (int) expr.size(); ++i) {
        if (expr[i] == '(') {
            st.push_back(i);
        }
        if (expr[i] == ')') {
            parenthesis[st.back()] = i;
            st.pop_back();
        }
    }
    return getImplication(expr, pos);
}


string axioms[8];
expression * scheme[10];


void getAxioms() {
    ifstream stream;
    stream.open("axioms", ifstream::in);
    for (int i = 0; i < 8; ++i) {
        string ax;
        getline(stream, ax);
        axioms[i] = parseExpression(ax) -> toString();
    }
    stream.close();
};


void getScheme() {
    ifstream stream;
    stream.open("scheme", ifstream::in);
    for (int i = 0; i < 10; ++i) {
        string ax;
        getline(stream, ax);
        scheme[i] = parseExpression(ax);
    }
    stream.close();
}


bool parseWithSchema(expression * expr, expression * schema, vector < vector < string > > & vars) {
    if (isUpperCaseLetter(schema -> name[0])) {
        vars[schema -> name[0] - 'A'].push_back(expr -> toString());
        return true;            
    }

    if (expr -> name != schema -> name || expr -> args.size() != schema -> args.size()) {
        return false;
    }

    int sz = expr -> args.size();
    for (int i = 0; i < sz; ++i) {
        if (!parseWithSchema(expr -> args[i], schema -> args[i], vars)) return false;
    }
    return true;
}


bool getInsertion(expression * expr, expression * afterInsertionExpr, vector < expression * > & insertion, string & varName) {
    if (isVariable(expr) && expr -> name == varName) {
        insertion.push_back(afterInsertionExpr);
        return true;
    }

    if (expr -> name != afterInsertionExpr -> name || expr -> args.size() != afterInsertionExpr -> args.size()) {
        return false;
    }

    int sz = expr -> args.size();
    for (int i = 0; i < sz; ++i) {
        if (!getInsertion(expr -> args[i], afterInsertionExpr -> args[i], insertion, varName)) return false;
    }
    return true;
}

bool equals(expression * a, expression * b) {
    return a -> toString() == b -> toString();
}

bool isAfterInsertion(expression * expr, expression * afterInsertionExpr, expression * insertingExpr, string & varName) {
    vector < expression * > insertion;
    if (!getInsertion(expr, afterInsertionExpr, insertion, varName)) return false;
    for (int i = 0; i < (int) insertion.size(); ++i) {
        if (!equals(insertion[i], insertingExpr)) return false;
    }
    return true;
}

bool isFree(expression * expr, string & varName) {
    if (expr -> name == "@" || expr -> name == "?") {
        if ((expr -> args[0]) -> name == varName) return false;
    }
    for (int i = 0; i < (int) expr -> args.size(); ++i) {
        if (!isFree(expr -> args[i], varName)) return false; 
    }
    return true;
}

bool containsFreeVar(expression * expr, string & varName) {
    if (expr -> name == varName && expr -> args.size() == 0) return true;

    if (expr -> name == "@" || expr -> name == "?") {
        if ((expr -> args[0]) -> name == varName) return false;
    }

    for (int i = 0; i < (int) expr -> args.size(); ++i) {
        if (containsFreeVar(expr -> args[i], varName)) return true;
    }
    return false;
}

void getFreeVars(expression * expr, set < string > & freeVars, multiset < string > & blockVars) {
    if (isVariable(expr) && expr -> args.size() == 0) {
        if (blockVars.find(expr -> name) == blockVars.end()) {
            freeVars.insert(expr -> name);
        }
        return;
    }

    if (expr -> name == "@" || expr -> name == "?") {
        blockVars.insert((expr -> args[0]) -> name);
    }

    for (int i = 0; i < (int) expr -> args.size(); ++i) {
        getFreeVars(expr -> args[i], freeVars, blockVars);
    }

    if (expr -> name == "@" || expr -> name == "?") {
        blockVars.erase(blockVars.find((expr -> args[0]) -> name));
    }
}

bool checkFree(expression * expr, string & varName, vector < string > & vars, multiset < string > & blockVars) {
    if (expr -> name == varName || expr -> args.size() == 0) {
        for (int i = 0; i < (int) vars.size(); ++i) {
            if (blockVars.find(vars[i]) != blockVars.end()) return false;
        }
        return true;
    }  

    if (expr -> name == "@" || expr -> name == "?") {
        if ((expr -> args[0]) -> name == varName) return true;
        blockVars.insert((expr -> args[0]) -> name);
    }

    for (int i = 0; i < (int) expr -> args.size(); ++i) {
        if (!checkFree(expr -> args[i], varName, vars, blockVars)) return false;
    }

    if (expr -> name == "@" || expr -> name == "?") {
        blockVars.erase(blockVars.find((expr -> args[0]) -> name));
    }
    return true;
}

bool isReadyForInsertion(expression * expr, string & varName, expression * insertion) {
    set < string > freeVars;
    multiset < string > blockVars;
    getFreeVars(insertion, freeVars, blockVars);                                            
    vector < string > vars;
    for (set < string >::iterator it = freeVars.begin(); it != freeVars.end(); ++it) {
        vars.push_back(*it);
    }

    blockVars.clear();
    return checkFree(expr, varName, vars, blockVars);
}




bool isAxiom(string & expr) {
    for (int i = 0; i < 8; ++i) {
        if (expr == axioms[i]) return true;
    }
    return false;
}

bool isSimpleScheme(expression * expr) {
    vector < vector < string > > vars (3);
    for (int i = 0; i < 10; ++i) {
        for (int j = 0; j < 3; ++j) vars[j].clear();

        if (parseWithSchema(expr, scheme[i], vars)) {
            bool flag = true;
            for (int j = 0; j < 3; ++j) {
                for (int k = 1; k < (int) vars[j].size(); ++k) {
                    if (vars[j][0] != vars[j][k]) {
                        flag = false;
                        break;
                    }
                }
                if (!flag) break;
            }
            if (flag) {
                return true;
            }
        }
    }
    return false;
}


bool isFromAnySchema(expression * expr) {
    if (expr -> name != "->") return false;

    expression * left = expr -> args[0];
    expression * right = expr -> args[1];
    if (left -> name != "@") return false;
    expression * var = left -> args[0];
    expression * pred = left -> args[1];

    vector < expression * > insertion;
    if (!getInsertion(pred, right, insertion, var -> name)) return false;
    for (int i = 1; i < (int) insertion.size(); ++i) {
        if (insertion[0] -> toString() != insertion[i] -> toString()) return false;
    }

    if (insertion.size() == 0) return true;

    return isReadyForInsertion(pred, var -> name, insertion[0]);
}

bool isToSomeSchema(expression * expr) {
    if (expr -> name != "->") return false;

    expression * left = expr -> args[0];
    expression * right = expr -> args[1];
    if (right -> name != "?") return false;
    expression * var = right -> args[0];
    expression * pred = right -> args[1];

    vector < expression * > insertion;
    if (!getInsertion(pred, left, insertion, var -> name)) return false;

    for (int i = 1; i < (int) insertion.size(); ++i) {
        if (insertion[0] -> toString() != insertion[i] -> toString()) return false;
    }

    if (insertion.size() == 0) return true;

    return isReadyForInsertion(pred, var -> name, insertion[0]);
}

bool isInductiveSchema(expression * expr) {   
    if (expr -> name != "->") return false;
    expression * pred = expr -> args[1];
    expression * left = expr -> args[0];
    
    if (left -> name != "&") return false;

    expression * basis = left -> args[0];
    expression * inductiveStep = left -> args[1];

    if (inductiveStep -> name != "@") return false;
    expression * var = inductiveStep -> args[0];
    expression * nxt = inductiveStep -> args[1];
    if (nxt -> name != "->") return false;

    expression * from = nxt -> args[0];
    expression * to = nxt -> args[1];
    
    return (isFree(pred, var -> name) && equals(pred, from) && isAfterInsertion(pred, basis, new zero(), var -> name) &&
            isAfterInsertion(pred, to, new increase(new variable(var -> name)), var -> name));
}


bool isHypothesis(string & expr) {
    for (int i = 0; i < (int) hypothesis.size(); ++i) {
        if (hypothesis[i] == expr) return true;
    }
    return false;
}

vector < expression * > evidenced;
set < string > evidencedString;


bool isModusPonens(expression * expr, int & whole) {
    for (int i = 0; i < (int) evidenced.size(); ++i) {
        expression * curExpr = evidenced[i];
        if (curExpr -> name == "->" && equals(curExpr -> args[1], expr) && evidencedString.find((curExpr -> args[0]) -> toString()) != evidencedString.end()) {
            whole = i;
            return true;   
        }
    }
    return false;
}


bool isGeneralization(expression * expr, string & varName) {
    if (expr -> name != "->") return false;
    expression * left = expr -> args[0];
    expression * anyRight = expr -> args[1];
    if (anyRight -> name != "@") return false;
    expression * var = anyRight -> args[0];
    expression * right = anyRight -> args[1];
    

    if (containsFreeVar(left, var -> name)) return false;
    varName = (var -> name);
    expression * tmp = new implication(left, right);

    bool ans = evidencedString.find(tmp -> toString()) != evidencedString.end();
    return ans;    
}


bool isReduction(expression * expr, string & varName) {
    if (expr -> name != "->") return false;
    expression * someLeft = expr -> args[0];
    expression * right = expr -> args[1];
    if (someLeft -> name != "?") return false;

    expression * var = someLeft -> args[0];
    expression * left = someLeft -> args[1];

    if (containsFreeVar(right, var -> name)) return false;
    varName = (var -> name);
    expression * tmp = new implication(left, right);

    bool ans = evidencedString.find(tmp -> toString()) != evidencedString.end();
    return ans;
}

string getHypothesis() {
    int sz = header.size();
    int pos = 0;
    string ret = "";
    for (int i = 0; i + 1 < sz; ++i) {
        if (header[i] == '|' && header[i + 1] == '-') {
            pos = i + 2;
            break;
        }
    }

    string cur = "";
    int bal = 0;
    for (int i = 0; i < pos - 2; ++i) {
        //cout << cur << endl;
        if (header[i] == '(') ++bal;
        if (header[i] == ')') --bal;

        if (header[i] == ',' && bal == 0) {
            hypothesis.push_back(cur);
            cur = "";
        }
        else {
            cur += header[i];
        }
    }

    if (cur != "") hypothesis.push_back(cur);

    for (int i = 0; i < (int) hypothesis.size(); ++i) {
        hypothesis[i] = parseExpression(hypothesis[i]) -> toString();
    }

    for (int i = pos; i < sz; ++i) {
        ret += header[i];
    }

    return parseExpression(ret) -> toString();
}

vector < string > rebuilt;

string replace(string & expr, vector < string > & vals) {
    string ret = "";
    for (int i = 0; i < (int) expr.size(); ++i) {
        if ('A' <= expr[i] && expr[i] <= 'C') {
            ret += vals[expr[i] - 'A'];
        }
        else {
            ret += expr[i];
        }
    }
    return ret;
}

void generateProve(int type, vector < string > & vals) {
    ifstream stream;
    if (type == 1) stream.open("firstSchema", ifstream::in);
    if (type == 2) stream.open("secondSchema", ifstream::in);
    if (type == 3) stream.open("thirdSchema", ifstream::in);
    int num;
    stream >> num;
    for (int i = 0; i < num; ++i) {
        string expr;
        stream >> expr;
        rebuilt.push_back(replace(expr, vals));
    }
}



int checkCorrectnessAndRebuild() {
    string hypo = "";
    if (hypothesis.size() != 0) {
        hypo = hypothesis.back();
    }
    int last = 0;
    int sz = evidence.size();
    for (int i = 0; i < (int) evidence.size(); ++i) {
        string expr = evidence[i];

        expression * parseExpr = parseExpression(expr);
        
        expr = parseExpr -> toString();

        if (100 * i / sz > last) {
            ++last;
            cerr << last << endl;
        }
        
        if (isAxiom(expr)) {
            rebuilt.push_back(expr);
            if (hypo != "") {                
                rebuilt.push_back("(" + expr + ")->(" + hypo + ")->(" + expr + ")");
                rebuilt.push_back("(" + hypo + ")->(" + expr + ")");
            }
            //cout << "Axiom" << "\n";
            evidenced.push_back(parseExpr);
            evidencedString.insert(parseExpr -> toString());
            continue;
        }

        if (isSimpleScheme(parseExpr)) {
            rebuilt.push_back(expr);
            if (hypo != "") {
               rebuilt.push_back("(" + expr + ")->(" + hypo + ")->(" + expr + ")");
               rebuilt.push_back("(" + hypo + ")->(" + expr + ")");            
            }
            //cout << "Simple Axiom Schema" << "\n";
            evidenced.push_back(parseExpr);
            evidencedString.insert(parseExpr -> toString());
            continue;
        }

        

        if (isHypothesis(expr)) {
            if (expr == hypo) {
                string truth = "a+0=a";
                rebuilt.push_back(truth);
                rebuilt.push_back("(" + hypo + ")->(" + truth + ")->(" + hypo + ")");
                rebuilt.push_back("(" + truth + ")->(" + hypo + ")->(" + truth + ")");
                rebuilt.push_back("(" + hypo + ")->(" + truth + ")");
                rebuilt.push_back("((" + hypo + ")->(" + truth + "))->((" + hypo + ")->(" + truth + ")->(" + hypo + "))->((" + hypo + ")->(" + hypo + "))");
                rebuilt.push_back("((" + hypo + ")->(" + truth + ")->(" + hypo + "))->((" + hypo + ")->(" + hypo + "))");
                rebuilt.push_back("(" + hypo + ")->(" + hypo + ")");
            }
            else {
                rebuilt.push_back(expr);
                if (hypo != "") {                    
                    rebuilt.push_back("(" + expr + ")->(" + hypo + ")->(" + expr + ")");
                    rebuilt.push_back("(" + hypo + ")->(" + expr + ")");
                }
            }
            //cout << "Hypothesis" << "\n";
            evidenced.push_back(parseExpr);
            evidencedString.insert(parseExpr -> toString());
            continue;
        }

        if (isFromAnySchema(parseExpr)) {
            rebuilt.push_back(expr);
            if (hypo != "") {                    
                rebuilt.push_back("(" + expr + ")->(" + hypo + ")->(" + expr + ")");
                rebuilt.push_back("(" + hypo + ")->(" + expr + ")");
            }            
            //cout << "From Any Schema" << "\n";
            evidenced.push_back(parseExpr);
            evidencedString.insert(parseExpr -> toString());
            continue;
        }

        if (isToSomeSchema(parseExpr)) {
            rebuilt.push_back(expr);
            if (hypo != "") {                    
                rebuilt.push_back("(" + expr + ")->(" + hypo + ")->(" + expr + ")");
                rebuilt.push_back("(" + hypo + ")->(" + expr + ")");
            }
            //cout << "To Some Schema" << "\n";
            evidenced.push_back(parseExpr);
            evidencedString.insert(parseExpr -> toString());
            continue;
        }

        if (isInductiveSchema(parseExpr)) {
            rebuilt.push_back(expr);
            if (hypo != "") {                    
                rebuilt.push_back("(" + expr + ")->(" + hypo + ")->(" + expr + ")");
                rebuilt.push_back("(" + hypo + ")->(" + expr + ")");
            }
            //cout << "Inductive Schema" << "\n";
            evidenced.push_back(parseExpr);
            evidencedString.insert(parseExpr -> toString());
            continue;
        }

        string name;
        if (isGeneralization(parseExpr, name) && (hypo == "" || !containsFreeVar(parseExpression(hypo), name))) {
            if (hypo == "") {
                rebuilt.push_back(expr);                
            }
            else {
                expression * left = parseExpr -> args[0];
                expression * right = (parseExpr -> args[1]) -> args[1];
                string phi = left -> toString();
                string psi = right -> toString();

                vector < string > tmp = { hypo, phi, psi };
                generateProve(1, tmp);
                rebuilt.push_back("(" + hypo + ")&(" + phi + ")->(" + psi + ")");
                rebuilt.push_back("(" + hypo + ")&(" + phi + ")->@" + name + "(" + psi + ")");
                tmp = { hypo, phi, "@" + name + "(" + psi + ")" };
                generateProve(2, tmp);
                rebuilt.push_back("(" + hypo + ")->(" + phi + ")->@" + name + "(" + psi + ")");
            }
            //cout << "Generalization Inference Rule" << "\n";
            evidenced.push_back(parseExpr);
            evidencedString.insert(parseExpr -> toString());
            continue;
        }

        if (isReduction(parseExpr, name) && (hypo == "" || !containsFreeVar(parseExpression(hypo), name))) {
            if (hypo == "") {
                rebuilt.push_back(expr);
            }
            else {
                expression * right = parseExpr -> args[1];
                expression * left = (parseExpr -> args[0]) -> args[1];
                string psi = left -> toString();
                string phi = right -> toString();
                
                vector < string > tmp = { hypo, phi, psi };
                generateProve(3, tmp);
                rebuilt.push_back("(" + psi + ")->(" + hypo + ")->(" + phi + ")");
                rebuilt.push_back("?" + name + "(" + psi + ")->(" + hypo + ")->(" + phi + ")");
                
                tmp = { "?" + name + "(" + psi + ")", hypo, phi };
                generateProve(3, tmp);
                rebuilt.push_back("(" + hypo + ")->?" + name + "(" + psi + ")->(" + phi + ")");
            }
            //cout << "Reduction Inference Rule" << "\n";
            evidenced.push_back(parseExpr);
            evidencedString.insert(parseExpr -> toString());
            continue;
        }
        
        int whole;
        if (isModusPonens(parseExpr, whole)) {
            if (hypo == "") {
                rebuilt.push_back(expr);
            }
            else {
                expression * wh = evidenced[whole];
                string l = (wh -> args[0]) -> toString();
                string r = (wh -> args[1]) -> toString();

                rebuilt.push_back("((" + hypo + ")->(" + l + "))->((" + hypo + ")->(" + l + ")->(" + r + "))->((" + hypo + ")->(" + r + "))");
                rebuilt.push_back("((" + hypo + ")->(" + l + ")->(" + r + "))->((" + hypo + ")->(" + r + "))");
                rebuilt.push_back("(" + hypo + ")->(" + r + ")");
            }
            
            //cout << "Modus Ponens" << "\n";
            evidenced.push_back(parseExpr);
            evidencedString.insert(parseExpr -> toString());
            continue;
        }

        return i + 1;
    }
    return -1;
}


int main() {
    freopen("proof", "r", stdin);
    freopen("rebuiltProof", "w", stdout);
    
    scanf("%s\n", str);

    header = string(str);

    needToProve = getHypothesis();
    

    while (scanf("%s\n", str) >= 0) {
        evidence.push_back(string(str));     
        //cout << parseExpression(evidence.back()) -> toString() << endl;
    }
    
    getAxioms();
    getScheme();
    
    //cout << parseExpression(evidence.back()) -> toString() << endl;
    //cout << scheme[8] -> toString() << endl;;
    
    int res = checkCorrectnessAndRebuild();


    if (res != -1) {
        cout << "Incorrect! First incorrect formula's number is " << res << endl;
        return 0;
    }
    
    for (int i = 0; i + 2 < (int) hypothesis.size(); ++i) {
        cout << hypothesis[i] << ",";
    }

    if (hypothesis.size() > 1) {
        cout << hypothesis[hypothesis.size() - 2];   
    }
    cout << "|-";
    if (hypothesis.size() > 0) {
        cout << "(" + hypothesis.back() + ")->(" + needToProve + ")" << "\n";
    } else {
        cout << needToProve << "\n";
    }
    for (int i = 0; i < (int) rebuilt.size(); ++i) {
        cout << rebuilt[i] << "\n";
    }

    //cout << res << endl;
    
    




        
    return 0;
}