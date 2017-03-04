#include <bits/stdc++.h>


using namespace std;


string name[3] = { "->", "|", "&" };
int MAXVAR = 4;
int MAXLENVAR = 3;
vector < string > vars;

string rand_name() {
    string s = "";
    s += (char) ('A' + rand() % ('Z' - 'A' + 1));
    for (int i = 0; i < MAXLENVAR - 1; ++i) {
        if (rand() % 2) {
            s += (char) ('0' + rand() % 10);
        }
        else {
            s += (char) ('A' + rand() % ('Z' - 'A' + 1));
        }
    }
    return s;
}

 
string rand_var() {
    if (vars.size() == 0) {
        for (int i = 0; i < MAXVAR; ++i) {
            vars.push_back(rand_name());
        }
    }

    return vars[rand() % MAXVAR];
}

string gen_expr(int max_depth) {
    if (max_depth == 1) {
        return rand_var();
    }

    int type = rand() % 4;
    string ret = "";
    if (type == 0) {
        ret += "!(";
        ret += gen_expr(rand() % (max_depth - 1) + 1);
        ret += ")";
        return ret;
    }
                                                                                       
    ret += "(";
    ret += gen_expr(rand() % (max_depth - 1) + 1);
    ret += ")";
    ret += name[type - 1];
    ret += "(";
    ret += gen_expr(rand() % (max_depth - 1) + 1);
    ret += ")";
    return ret;
}


const int LEN = 200;
const int MAXN = 2000;


int main() {    
    srand(time(NULL));
    
    vector < string > used, sug;
    string right = gen_expr(LEN);
    for (int i = 0; i < MAXN; ++i) {
        string left = gen_expr(LEN);
        used.push_back(left);
        string s = "(" + left + ")->(" + right + ")";
        cout << s;
        sug.push_back(s);
        cout << ",";
    }

    for (int i = 0; i < MAXN / 2; ++i) {
        cout << used[MAXN - i - 1];
        if (i != MAXN / 2 - 1) {
            cout << ",";
        }
        else {
            cout << "|-";
        }
    }
    cout << right << endl;

    for (int i = 0; i < MAXN; ++i) {
        cout << sug[i] << "\n";
    }
    for (int i = 0; i < MAXN / 2; ++i) {
        cout << used[MAXN - i - 1] << "\n";
    }

    for (int i = 0; i < MAXN; ++i) {
        cout << right << "\n";
    }

    return 0;
}