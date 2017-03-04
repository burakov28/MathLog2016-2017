#include <bits/stdc++.h>


using namespace std;


string gn(int a) {
    string ret = "0";
    for (int i = 0; i < a; ++i) {
        ret += "'";
    }
    return ret;
}


void reverse(string a, string b) {
    cout << "@a@b(b=a->a=b)->@b(b=(" + b + ")->(" + b + ")=b)" << "\n";
    cout << "@b(b=(" + b + ")->(" + b + ")=b)" << "\n";
    cout << "@b(b=(" + b + ")->(" + b + ")=b)->((" + a + ")=(" + b + ")->(" + b + ")=(" + a + "))" << "\n";
    cout << "((" + a + ")=(" + b + ")->(" + b + ")=(" + a + "))" << "\n";
    cout << "(" + b + ")=(" + a + ")" << "\n";
}


void generateLessOrEqual(int a, int b) {
    cout << "|-?p(" + gn(a) + "+p=" + gn(b) + ")" << "\n";
    ifstream header;
    header.open("header", ifstream::in);
    int num;
    header >> num;
    string str;
    for (int i = 0; i < num; ++i) {
        header >> str;
        cout << str << "\n";
    }
    
    cout << "@a(a+0=a)->(" + gn(a) + "+0=" + gn(a) + ")" << "\n";
    cout << gn(a) + "+0=" + gn(a) << "\n";
    for (int i = 0; i < b - a; ++i) {
        string sa = "(" + gn(a) + "+" + gn(i) + ")'";
        string sb = gn(a + i + 1);
        string sc = gn(a) + "+" + gn(i + 1);
        cout << "@a@b(a=b->a'=b')->@b(" + gn(a) + "+" + gn(i) + "=b->" + sa + "=b')" << "\n";
        cout << "@b(" + gn(a) + "+" + gn(i) + "=b->" + sa + "=b')" << "\n";
        cout << "@b(" + gn(a) + "+" + gn(i) + "=b->" + sa + "=b')->(" + gn(a) + "+" + gn(i) + "=" + gn(a + i) + "->" + sa + "=" + sb + ")" << "\n";
        cout << gn(a) + "+" + gn(i) + "=" + gn(a + i) + "->" + sa + "=" + sb << "\n";
        cout << sa + "=" + sb << "\n";
        
        cout << "@a@b(a+b'=(a+b)')->@b(" + gn(a) + "+b'=(" + gn(a) + "+b)'" + ")" << "\n";
        cout << "@b(" + gn(a) + "+b'=(" + gn(a) + "+b)'" + ")" << "\n";
        cout << "@b(" + gn(a) + "+b'=(" + gn(a) + "+b)'" + ")->" + sc + "=" + sa << "\n";
        cout << sc + "=" + sa << "\n";
        reverse(sc, sa);

        cout << "@a@b@c(a=b->a=c->b=c)->@b@c(" + sa + "=b->" + sa + "=c->b=c)" << "\n";
        cout << "@b@c(" + sa + "=b->" + sa + "=c->b=c)" << "\n";
        cout << "@b@c(" + sa + "=b->" + sa + "=c->b=c)->@c(" + sa + "=" + sb + "->" + sa + "=c->" + sb + "=c)" << "\n";
        cout << "@c(" + sa + "=" + sb + "->" + sa + "=c->" + sb + "=c)" << "\n";
        cout << "@c(" + sa + "=" + sb + "->" + sa + "=c->" + sb + "=c)->" + sa + "=" + sb + "->" + sa + "=" + sc + "->" + sb + "=" + sc << "\n";
        cout << sa + "=" + sb + "->" + sa + "=" + sc + "->" + sb + "=" + sc << "\n";
        cout << sa + "=" + sc + "->" + sb + "=" + sc << "\n";
        cout << sb + "=" + sc << "\n";
        reverse(sb, sc);
    }
    cout << gn(a) + "+" + gn(b - a) + "=" + gn(b) + "->?p(" + gn(a) + "+p=" + gn(b) + ")" << "\n";
    cout << "?p(" + gn(a) + "+p=" + gn(b) + ")" << "\n";
}


void generateNotLessOrEqual(int na, int nb) {
    cout << "|-@p(!(p+" + gn(na) + "=" + gn(nb) + "))" << "\n";
    ifstream stream;
    stream.open("header", ifstream::in);
    int n;
    stream >> n;
    for (int i = 0; i < n; ++i) {
        string s;
        stream >> s;
        cout << s << "\n";
    }

    

    string a = "(p+" + gn(na - nb - 1) + ")'";
    cout << "@a(!(a'=0))->(!(" + a + "=0))" << "\n";
    cout << "(!(" + a + "=0))" << "\n";
    
    string b = "0";
    for (int i = 0; i < nb; ++i) {
        cout << "@a@b(!(a=b)->!(a'=b'))->@b(!(" + a + "=b)->!(" + a + "'=b'))" << "\n";
        cout << "@b(!(" + a + "=b)->!(" + a + "'=b'))" << "\n";
        cout << "@b(!(" + a + "=b)->!(" + a + "'=b'))->(!(" + a + "=" + b + ")->!(" + a + "'=" + b + "'))" << "\n";
        cout << "(!(" + a + "=" + b + ")->!(" + a + "'=" + b + "'))" << "\n";
        cout << "!(" + a + "'=" + b + "')" << "\n";
        a += "'";
        b += "'";
    }

    a = "(p+" + gn(na - nb - 1) + ")'";
    b = "p+" + gn(na - nb - 1) + "'";
    cout << "@a@b((a+b)'=(a+b'))->@b((p+b)'=(p+b'))" << "\n";
    cout << "@b((p+b)'=(p+b'))" << "\n";
    cout << "@b((p+b)'=(p+b'))->(" + a + "=" + b + ")" << "\n";
    cout << "(" + a + "=" + b + ")" << "\n";
    for (int i = 0; i < nb; ++i) {
        cout << "@a@b(a=b->b=a)->@b((" + a + "=b)->(b=" + a + "))" << "\n";
        cout << "@b((" + a + "=b)->(b=" + a + "))" << "\n";
        cout << "@b((" + a + "=b)->(b=" + a + "))->((" + a + "=" + b +")->("+ b + "=" + a + "))" << "\n";
        cout << "((" + a + "=" + b +")->("+ b + "=" + a + "))" << "\n";
        cout << "("+ b + "=" + a + ")" << "\n";

        cout << "@a@b((a=b)->(a'=b'))->@b((" + b + "=b)->((" + b + ")'=b'))" << "\n";
        cout << "@b((" + b + "=b)->((" + b + ")'=b'))" << "\n";
        cout << "@b((" + b + "=b)->((" + b + ")'=b'))->((" + b + "=" + a + ")->((" + b + ")'=" + a + "'))" << "\n";
        cout << "((" + b + "=" + a + ")->((" + b + ")'=" + a + "'))" << "\n";
        cout << "((" + b + ")'=" + a + "')" << "\n";

        cout << "@a@b((a+b)'=(a+b'))->@b((p+b)'=(p+b'))" << "\n";
        cout << "@b((p+b)'=(p+b'))" << "\n";
        cout << "@b((p+b)'=(p+b'))->((" + b + ")'=" + b + "')" << "\n";
        cout << "((" + b + ")'=" + b + "')" << "\n";

        cout << "@a@b@c((a=b)->(a=c)->(b=c))->@b@c(((" + b + ")'=b)->((" + b + ")'=c)->(b=c))" << "\n";
        cout << "@b@c(((" + b + ")'=b)->((" + b + ")'=c)->(b=c))" << "\n";
        cout << "@b@c(((" + b + ")'=b)->((" + b + ")'=c)->(b=c))->@c(((" + b + ")'=" + a + "')->((" + b + ")'=c)->(" + a + "'=c))" << "\n";
        cout << "@c(((" + b + ")'=" + a + "')->((" + b + ")'=c)->(" + a + "'=c))" << "\n";
        cout << "@c(((" + b + ")'=" + a + "')->((" + b + ")'=c)->(" + a + "'=c))->(((" + b + ")'=" + a + "')->((" + b + ")'=" + b + "')->(" + a + "'=" + b + "'))" << "\n";
        cout << "(((" + b + ")'=" + a + "')->((" + b + ")'=" + b + "')->(" + a + "'=" + b + "'))" << "\n";
        cout << "((" + b + ")'=" + b + "')->(" + a + "'=" + b + "')" << "\n";
        cout << "(" + a + "'=" + b + "')" << "\n";
        a += "'";
        b += "'";       
    }
    a = "(p+" + gn(na - nb - 1) + ")";
    for (int i = 0; i < nb + 1; ++i) a += "'";
    b = "p+" + gn(na);
    string c = gn(nb);

    cout << "@a@b@c((a=b)->(!(a=c))->(!(b=c)))->@b@c((" + a + "=b)->(!(" + a + "=c))->(!(b=c)))" << "\n";
    cout << "@b@c((" + a + "=b)->(!(" + a + "=c))->(!(b=c)))" << "\n";
    cout << "@b@c((" + a + "=b)->(!(" + a + "=c))->(!(b=c)))->@c((" + a + "=" + b + ")->(!(" + a + "=c))->(!(" + b + "=c)))" << "\n";
    cout << "@c((" + a + "=" + b + ")->(!(" + a + "=c))->(!(" + b + "=c)))" << "\n";
    cout << "@c((" + a + "=" + b + ")->(!(" + a + "=c))->(!(" + b + "=c)))->((" + a + "=" + b + ")->(!(" + a + "=" + c + "))->(!(" + b + "=" + c + ")))" << "\n";
    cout << "((" + a + "=" + b + ")->(!(" + a + "=" + c + "))->(!(" + b + "=" + c + ")))" << "\n";
    cout << "(!(" + a + "=" + c + "))->(!(" + b + "=" + c + "))" << "\n";
    cout << "!(" + b + "=" + c + ")" << "\n";
    string ab = "!(" + b + "=" + c + ")";
    cout << ab << "->((0=0)->(0=0)->(0=0))->" << ab << "\n";
    cout << "((0=0)->(0=0)->(0=0))->" << ab << "\n";
    cout << "((0=0)->(0=0)->(0=0))->@p(" << ab + ")" << "\n";
    cout << "@p(" << ab + ")" << "\n";
}   


int main() {
    freopen("proof", "w", stdout);
    int a, b;
    cin >> a >> b;
    if (a <= b) {
        generateLessOrEqual(a, b);
    }
    else {
        generateNotLessOrEqual(a, b);
    }
    return 0;
}