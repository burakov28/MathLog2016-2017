#include <bits/stdc++.h>


using namespace std;


struct TVertex {
    map < char, int > nxt;
    bool term;
    int ind;

    TVertex() {
        term = false;        
    }
};

struct TBor {
    vector < TVertex > bor;

    TBor () {
        bor.push_back(TVertex());
    }


    int new_node() {
        bor.push_back(TVertex());
        return bor.size() - 1;
    }

    int add(int v, string & s, int pos, int ind) {
        if (pos == (int) s.size()) {
            bor[v].term = true;
            if (ind != -1) {
                bor[v].ind = ind;
            }
            return v;
        }

        if (bor[v].nxt.find(s[pos]) == bor[v].nxt.end()) {
            bor[v].nxt[s[pos]] = new_node();
        }

        return add(bor[v].nxt[s[pos]], s, pos + 1, ind);
    }

    int find(int v, string & s, int pos) {
        if (pos == (int) s.size()) {
            return v;
        }

        if (bor[v].nxt.find(s[pos]) == bor[v].nxt.end()) {
            return -1;
        }
        else {
            return find(bor[v].nxt[s[pos]], s, pos + 1);
        }
    }

    int add(string & s, int ind) {
        return add(0, s, 0, ind);
    }

    int find(string & s) {
        return find(0, s, 0);
    }
};


struct expression {
    uint8_t type;
    expression * left, * right;
    string name;

    bool is_equal(expression * cur) {
        if (type != cur -> type) return false;

        if (type == 0) {
            return name == (cur -> name);
        }

        if (type == 4) {
            return left -> is_equal(cur -> left);
        }

        return (left -> is_equal(cur -> left)) && (right -> is_equal(cur -> right));
    }


    void print(string & ret) {
        if (type == 0) {
            ret += name;
            return;
        }
        if (type == 4) {
            ret += name;
            if (left -> type != 0 && left -> type != 4) {
                ret += '(';
            }
            left -> print(ret);            
            if (left -> type != 0 && left -> type != 4) {
                ret += ')';
            }

            return;
        }
        if (left -> type != 0 && left -> type != 4) {
            ret += '(';
        }       
        left -> print(ret);
        if (left -> type != 0 && left -> type != 4) {
            ret += ')';
        }
        
        ret += name;
        
        if (right -> type != 0 && right -> type != 4) {
            ret += '(';
        }       
        right -> print(ret);
        if (right -> type != 0 && right -> type != 4) {
            ret += ')';
        }
    }

    bool is_contain(expression * axiom, vector < vector < expression * > > & ans) {        
        if (axiom -> type == 0) {
            ans[(axiom -> name[0]) - 'A'].push_back(this);
            return true;
        }

        if (type != axiom -> type) return false;
        
        if (type == 4) {
            return left -> is_contain(axiom -> left, ans);    
        }
        return (left -> is_contain(axiom -> left, ans)) && (right -> is_contain(axiom -> right, ans));
    }

    bool is_contain(expression * axiom) {
        vector < vector < expression * > > ans;
        ans.resize(3);
        
        if (!is_contain(axiom, ans)) {
            //cout << " fuck ";
            return false;
        }
        
        for (int i = 0; i < 3; ++i) {
            int sz = ans[i].size();
            for (int j = 1; j < sz; ++j) {
                if (!(ans[i][0] -> is_equal(ans[i][j]))) return false;
            }
        }
        return true;
    }

    string generation() {
        if (type == 0) {
            return "new variable(\"" + name + "\")";  
        }
        if (type == 4) {
            return "new negation(" + (left -> generation()) + ")";            
        }
        if (type == 1) {
            return "new implication(" + (left -> generation()) + ", " + (right -> generation()) + ")";
        }
        if (type == 2) {
            return "new conjunction(" + (left -> generation()) + ", " + (right -> generation()) + ")";
        }        
        return "new disjunction(" + (left -> generation()) + ", " + (right -> generation()) + ")";        
    }
};

struct variable : expression {

    variable (string _name) {
        name = _name;
        type = 0;
    }
};


struct implication : expression {
    
    implication(expression * _left, expression * _right) {
        left = _left; right = _right;
        type = 1;
        name = "->";
    }
};

struct conjunction : expression {
    
    conjunction(expression * _left, expression * _right) {
        left = _left; right = _right;
        type = 2;
        name = "&";
    }
};


struct disjunction : expression {
    
    disjunction(expression * _left, expression * _right) {
        left = _left; right = _right;
        type = 3;
        name = "|";
    }
};


struct negation : expression {
    
    negation(expression * _left) {
        left = _left;
        type = 4;
        name = "!";
    }
};

                                                 
void tokenize(string & s, vector < string > & tokens) {
    tokens.clear();
    int n = s.size();
    string cs;
    for (int i = 0; i < n;) {
        if (s[i] == '(' || s[i] == ')' || s[i] == '&' || s[i] == '|' || s[i] == '!') {
            cs = "";
            cs += s[i];
            tokens.push_back(cs);
            ++i;
            continue;   
        }
        if (s[i] == '-' && s[i + 1] == '>') {
            cs = "";
            cs = "->";
            tokens.push_back(cs);
            i += 2;
            continue;
        }

        cs = "";
        while (i < n && (('A' <= s[i] && s[i] <= 'Z') || ('0' <= s[i] && s[i] <= '9'))) {
            cs += s[i];
            ++i;
        }
        if (cs != "") {
            tokens.push_back(cs);
        }
    }
}

expression * get_implication(vector < string > &, int &);


expression * get_negation(vector < string > & tokens, int & pos) {
    if (tokens[pos] == "(") {
        ++pos;
        expression * ret = get_implication(tokens, pos);
        ++pos;
        return ret;
    }
    if (tokens[pos] == "!") {
        ++pos;
        expression * ret = get_negation(tokens, pos);
        return new negation(ret);
    }

    ++pos;
    return new variable(tokens[pos - 1]);
}


expression * get_conjunction(vector < string > & tokens, int & pos) {
    expression * left = get_negation(tokens, pos);
    if (pos == (int) tokens.size() || tokens[pos] != "&") return left;
    ++pos;
    expression * right = get_conjunction(tokens, pos);
    return new conjunction(left, right);
}


expression * get_disjunction(vector < string > & tokens, int & pos) {
    expression * left = get_conjunction(tokens, pos);
    if (pos == (int) tokens.size() || tokens[pos] != "|") return left;
    ++pos;
    expression * right = get_disjunction(tokens, pos);
    return new disjunction(left, right); 
}


expression * get_implication(vector < string > & tokens, int & pos) {
    expression * left = get_disjunction(tokens, pos);
    if (pos == (int) tokens.size() || tokens[pos] != "->") return left;
    ++pos;
    expression * right = get_implication(tokens, pos);
    return new implication(left, right); 
}


expression * get_expression(string & s) {
    vector < string > tokens;
    tokenize(s, tokens);
    int pos = 0;
    expression * expr = get_implication(tokens, pos);
    return expr;
    //expr -> print();
}    

// method generation was used ;)
expression * axioms[10] = {
    new implication(new variable("A"), new implication(new variable("B"), new variable("A"))),
    new implication(new implication(new variable("A"), new variable("B")), new implication(new implication(new variable("A"), new implication(new variable("B"), new variable("C"))), new implication(new variable("A"), new variable("C")))),
    new implication(new variable("A"), new implication(new variable("B"), new conjunction(new variable("A"), new variable("B")))),
    new implication(new conjunction(new variable("A"), new variable("B")), new variable("A")),
    new implication(new conjunction(new variable("A"), new variable("B")), new variable("B")),
    new implication(new variable("A"), new disjunction(new variable("A"), new variable("B"))),
    new implication(new variable("B"), new disjunction(new variable("A"), new variable("B"))),
    new implication(new implication(new variable("A"), new variable("C")), new implication(new implication(new variable("B"), new variable("C")), new implication(new disjunction(new variable("A"), new variable("B")), new variable("C")))),
    new implication(new implication(new variable("A"), new variable("B")), new implication(new implication(new variable("A"), new negation(new variable("B"))), new negation(new variable("A")))),
    new implication(new negation(new negation(new variable("A"))), new variable("A"))    
};

vector < expression * > exprs, suggestions;
vector < int > evidenced_hash;
map < pair < int, int >, int > impls;
TBor suggestions_bor, evidenced_bor;


int is_axiom(expression * expr) {
    for (int i = 0; i < 10; ++i) {
        if (expr -> is_contain(axioms[i])) {
            return i + 1;
        }
    }
    return -1;
}


int main() {
    ios_base::sync_with_stdio(false);
    freopen("evidence.in", "r", stdin);
    freopen("evidence.out", "w", stdout);
    
    string header;
    getline(cin, header);

    int sz = header.size();
    cout << header << "\n";
    string cs = "";
    string expr_s = "";
    for (int i = 0; i < sz; ) {
        
        if (header[i] == ',') {
            if (cs != "") {
                suggestions.push_back(get_expression(cs));                
                cs = "";
            }
            ++i;
            continue;
        }

        if (header[i] == '|' && header[i + 1] == '-') {
            if (cs != "") {
                suggestions.push_back(get_expression(cs));                
                cs = "";
            }
            i += 2;
            while (i < sz) {
                expr_s += header[i];
                ++i;
            }
            break;
        }
        cs += header[i];
        ++i;
    }

     
    for (int i = 0; i < (int) suggestions.size(); ++i) {
        cs = "";
        suggestions[i] -> print(cs);
        suggestions_bor.add(cs, i + 1);
    }
    

    //expression * expr = get_expression(expr_s);

    string cur_expr_s;
    while (getline(cin, cur_expr_s)) {
        //cout << cur_expr_s << endl;
        exprs.push_back(get_expression(cur_expr_s));
        //cout << "hey" << endl;   
    }

    //cout << "lol" << endl;
   
    int n = exprs.size();
    for (int i = 0; i < n; ++i) {
        /*
        for (int j = num_hyp; j < (int) evidenced.size(); ++j) {
            evidenced[j] -> print();
            cout << " ";
        }
        cout << endl;
        */
        cs = "";
        cout << "(" << i + 1 << ") ";
        exprs[i] -> print(cs);
        cout << cs;
        cout << " (";
        
        bool flag = false;

        int num = suggestions_bor.find(cs);
        if (num != -1) {
            cout << "Предп. " << suggestions_bor.bor[num].ind;
            flag = true;
        }
        
        if (!flag) {
            num = is_axiom(exprs[i]);
            if (num != -1) {
                flag = true;
                cout << "Сх. акс. " << num;
            }
        }

        int right = evidenced_bor.add(cs, i + 1);
        if (!flag) {            
            for (int j = 0; j < (int) evidenced_hash.size(); ++j) {
                int left = evidenced_hash[j];
                map < pair < int, int >, int >::iterator it = impls.find(make_pair(left, right));
                if (it != impls.end()) {
                    flag = true;
                    cout << "M.P. " << evidenced_bor.bor[left].ind << ", " << (*it).second;
                    break;
                }
            }
        }

        if (flag) {
            if (exprs[i] -> type == 1) {
                cs = "";
                (exprs[i] -> left) -> print(cs);
                int lop = evidenced_bor.add(cs, -1);
                
                cs = "";
                (exprs[i] -> right) -> print(cs);
                int rop = evidenced_bor.add(cs, -1);  
            
                impls[make_pair(lop, rop)] = i + 1;                
            }
            evidenced_hash.push_back(right);
        }
        else {
            cout << "Не доказано";
        }

        cout << ")\n";
    }
    //cout << clock() << endl;
    return 0;
}