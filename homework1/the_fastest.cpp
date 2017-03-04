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
set < pair < int, int > > evidenced;
map < int, pair < int, int > > modus_ponens_pairs;
map < int, vector < pair < int, int > > > right_parts;
TBor suggestions_bor, evidenced_bor;


int is_axiom(expression * expr) {
    for (int i = 0; i < 10; ++i) {
        if (expr -> is_contain(axioms[i])) {
            return i + 1;
        }
    }
    return -1;
}


char * input;
int pos, len;
void build_tokenizer(FILE * file) {
    fseek(file, 0, SEEK_END);
    len = ftell(file);
    rewind(file);
    input = (char *) malloc(sizeof(char) * (len + 1));
    len = fread(input, 1, len, file);                                 
    pos = 0;
}
bool get_string(string & str) {
    str = "";
    while (pos < len && str == "") {
        while (pos < len && input[pos] != '\n') {
            str += input[pos];
            ++pos;
        }
        ++pos;
    }
    return (str != "");
}


int main() {
    FILE * file = freopen("evidence.in", "r", stdin);
    freopen("evidence.out", "w", stdout);
    build_tokenizer(file);
    
    string header;
    get_string(header);

    int sz = header.size();
    puts(header.c_str());

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
    

    string cur_expr_s;
    while (get_string(cur_expr_s)) { 
        exprs.push_back(get_expression(cur_expr_s));
    }
   
    int n = exprs.size();
    for (int i = 0; i < n; ++i) {
        cs = "";
        printf("(%d) ", i + 1);
        exprs[i] -> print(cs);
        printf("%s", cs.c_str());
        printf(" (");
        
        bool flag = false;

        int num = suggestions_bor.find(cs);
        if (num != -1) {
            printf("Предп. %d", suggestions_bor.bor[num].ind);
            flag = true;
        }
        
        if (!flag) {
            num = is_axiom(exprs[i]);
            if (num != -1) {
                flag = true;
                printf("Сх. акс. %d", num);
            }
        }

        int current = evidenced_bor.add(cs, i + 1);
        
        if (!flag) {                        
            if (modus_ponens_pairs.count(current) != 0) {
                pair < int, int > MP = modus_ponens_pairs[current];
                printf("M.P. %d, %d", MP.second, MP.first);
                flag = true;
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

                set < pair < int, int > >::iterator it = evidenced.lower_bound(make_pair(lop, -1));
                if (it != evidenced.end() && (*it).first == lop) {
                    modus_ponens_pairs[rop] = make_pair(i + 1, (*it).second);
                }
                else {
                    right_parts[lop].push_back(make_pair(rop, i + 1));
                }                
            }

            vector < pair < int, int > > imps = right_parts[current];
            if (imps.size() != 0) {
                for (int j = 0; j < (int) imps.size(); ++j) {
                    modus_ponens_pairs[imps[j].first] = make_pair(imps[j].second, i + 1);
                }
                right_parts[current].clear();
            }
            evidenced.insert(make_pair(current, i + 1));
        }
        else {
            printf("Не доказано");
        }
        printf(")\n");        
    }
    //printf("%lld\n", clock());
    return 0;
}