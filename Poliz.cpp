#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <fstream>
#include <cstring>
#include <string.h>

using namespace std;

enum type_of_lex
{
    LEX_NULL,
    LEX_PROGRAM,
    LEX_INT,
    LEX_BOOL,
    LEX_STRING,
    LEX_STRING_VAL,
    LEX_READ,
    LEX_WRITE,
    LEX_IF,
    LEX_ELSE,
    LEX_DO,
    LEX_WHILE,
    LEX_TRUE,
    LEX_FALSE,
    LEX_AND,
    LEX_NOT_W,
    LEX_OR,

    LEX_PLUS,
    LEX_MINUS,
    LEX_SLASH,
    LEX_STAR,
    LEX_GRT,
    LEX_LSS,
    LEX_GOE,
    LEX_LOE,
    LEX_EQ,
    LEX_NEQ,
    LEX_ASSIGN,
    LEX_COMMA,
    LEX_SEMICOLON,
    LEX_LPAREN,
    LEX_RPAREN,
    LEX_STBLOCK,
    LEX_FNBLOCK,
    LEX_QOTE,
    LEX_NOT,
    LEX_NUM,
    LEX_ID,
    POLIZ_LABEL,
    POLIZ_ADDRESS,
    POLIZ_GO,
    POLIZ_FGO
};

class Lex{
    type_of_lex t_lex;
    int v_lex;
    string str_lex;
public:
    Lex ( type_of_lex t = LEX_NULL, int v = 0){
        t_lex = t; v_lex = v;
    }
    Lex (type_of_lex t, std::string str = "c"){
        t_lex = t;
        str_lex = str;
    }
    type_of_lex get_type () const { return t_lex; }
    int get_value () const { return v_lex; }
    string get_str_val () const {return str_lex;}
    friend ostream& operator << ( ostream &out, Lex l);
};

class Ident{
    char * name;
    bool declare;
    type_of_lex type;
    bool assign;
    int value;
public:
    Ident (){
        declare = false;
        assign = false;
    }
    char *get_name (){
        return name;
    }
    void put_name (const char *n){
        name = new char[strlen(n) + 1];
        strcpy(name, n);
    }
    bool get_declare (){
        return declare;
    }
    void set_declare (){
        declare = true;
    }
    type_of_lex get_type (){
        return type;
    }
    void set_type (type_of_lex t){
        type = t;
    }
    bool get_assign (){
        return assign;
    }
    void set_assign (){
        assign = true;
    }
    int get_value (){
        return value;
    }
    void set_value (int v){
        value = v;
    }
};

class tabl_ident{
    Ident * p;
    int size;
    int top;
public:
    tabl_ident ( int max_size ){
        p = new Ident[size=max_size];
        top = 1;
    }
    ~tabl_ident (){
        delete []p;
    }
    Ident& operator[] ( int k ){
        return p[k];
    }
    int put ( const char *buf );
};

int tabl_ident::put ( const char *buf ){
    for ( int j=1; j<top; ++j )
    if ( !strcmp(buf, p[j].get_name()) )
    return j;
    p[top].put_name(buf);
    ++top;
    return top-1;
}

class Scanner{
    enum state { H, IDENT, NUMB, COM, QOUT, COMPARE, NEG, DELIM, FIN, SLSH};
    static const char * TW[];
    static type_of_lex words[];
    static const char * TS[];
    static type_of_lex dlms[];
    state CS;
    FILE * fp;
    char c;
    char buf[80];
    int buf_top;
    bool not_end = true;
    void clear (){
        buf_top = 0;
        for ( int j = 0; j < 80; ++j )
        buf[j] = '\0';
    }
    void add (){
        buf [ buf_top ++ ] = c;
    }
    int look ( const char *buf, const char **list ){
        int i = 0;
        while ( list[i] ){
            if ( !strcmp(buf, list[i]) ) return i;
            ++i;
        }
        return 0;
    }
    void gc (){
        c = fgetc (fp);
    }
public:
    Lex get_lex ();
    Scanner ( const char * program ){
        if ((fp = fopen(program, "r")) == nullptr)
        {
            std::cout << "Error: file didn't open" << std::endl;
            exit(1);
        }
        CS = H;
        clear();
        gc();
    }
    bool check_end() const {return not_end;}
    ~Scanner(){fclose(fp);}
    friend ostream & operator << (ostream & out, Lex l);
};

const char *Scanner::TW[] =
    {
        "",
        "program",

        "int", "string",

        "read", "write",

        "if", "else",

        "do", "while",

        "true", "false",
         "and", "not", "or",
        nullptr};
type_of_lex Scanner::words[] =
    {
        LEX_NULL,
        LEX_PROGRAM,
        LEX_INT, LEX_STRING,
        LEX_READ, LEX_WRITE,
        LEX_IF, LEX_ELSE,
        LEX_DO, LEX_WHILE,
        LEX_TRUE, LEX_FALSE,
        LEX_AND, LEX_NOT_W, LEX_OR,
        LEX_NULL};

const char *Scanner::TS[] =
    {
        "",

        "+", "-", "/", "*",

        ">", "<", ">=", "<=", "==", "!=",

        "=",

        ",", ";", "(", ")", "{", "}", "\"",
        "!",
        nullptr};
type_of_lex Scanner::dlms[]{
    LEX_NULL,
    LEX_PLUS, LEX_MINUS, LEX_SLASH, LEX_STAR,
    LEX_GRT, LEX_LSS, LEX_GOE, LEX_LOE, LEX_EQ, LEX_NEQ,
    LEX_ASSIGN,
    LEX_COMMA, LEX_SEMICOLON, LEX_LPAREN, LEX_RPAREN, LEX_STBLOCK, LEX_FNBLOCK, LEX_QOTE,
    LEX_NOT,
    LEX_NULL
};


tabl_ident TID(100);


ostream & operator << (ostream & out, Lex l){
    string type, type_of_table;
    if (l.t_lex <= LEX_OR){
        type = (string)Scanner::TW[l.v_lex];
        type_of_table = "TW: ";
    }
    else if (l.t_lex == LEX_NUM){
        type = "NUM";
        type_of_table = "";
    }
    else if (l.t_lex == LEX_ID){
        type = TID[l.v_lex].get_name();
        out << " < " << type << " | " << "TID: " << l.v_lex  << " > " <<  "\n";
        return out;
    }
    else if (l.t_lex == LEX_STRING_VAL){
        type = "STR";
        out << " < " << type << " | " << type_of_table << l.str_lex << " > " << "\n";
        return out;
    }
    else if ((l.t_lex > LEX_OR) && (l.t_lex < POLIZ_LABEL)){
        type = (string)Scanner::TS[l.v_lex];
        type_of_table = "TS: ";
    }
    out << " < " << type << " | " << type_of_table << l.v_lex << " > " << "\n";
    return out;
};


Lex Scanner::get_lex(){
    int digit, j;
    std::string str;
    CS = H;
    while (true)
    {
        switch (CS)
        {
        case H:
            if (c == '\n' || c == '\r' || c == ' ' || c == '\t')
                gc();
            else if (isalpha(c)){
                clear();
                add();
                gc();
                CS = IDENT;
            }
            else if (isdigit(c)){
                digit = c - '0';
                gc();
                CS = NUMB;
            }
            else if (c == '/'){
                clear();
                add();
                gc();
                if (c == '*'){
                    gc();
                    CS = COM;
                }
                else CS = SLSH;
            }
            else if (c == '"'){
                gc();
                CS = QOUT;
            }
            else if (c == '<' || c == '>' || c == '='){
                clear();
                add();
                gc();
                CS = COMPARE;
            }
            else if (c == '!'){
                clear();
                add();
                gc();
                CS = NEG;
            }
            else if (c != EOF){
                clear();
                add();
                CS = DELIM;
            }
            else CS = FIN;
            break;
        case IDENT:
            if (isalpha(c) || isdigit(c)){
                add();
                gc();
            }
            else if ((j = look(buf, TW)) != 0)
                return Lex(words[j], j);
            else{
                j = TID.put(buf); // new or old
                return Lex(LEX_ID, j);
            }
            break;

        case NUMB:
            if (isdigit(c))
            {
                digit = digit * 10 + c - '0';
                gc();
            }
            else
                return Lex(LEX_NUM, digit);
            break;

        case COM:
            if (c == '*'){
                gc();
                if (c == '/')
                {
                    gc();
                    CS = H;
                }
            }
            else if (c != EOF)
                gc();
            else if (c == EOF) CS = FIN;
            break;
        case SLSH:
            j = look(buf, TS);
            return Lex(dlms[j], j);

        case QOUT:
            if (c == '"'){
                gc();
                return Lex(LEX_STRING_VAL, str);
            }
            else if (c != EOF){
                str.push_back(c);
                gc();
            }
            else if (c == EOF) CS = FIN;
            break;
        case COMPARE:
            if (c == '='){
                add();
                gc();
            }
            j = look(buf, TS);
            return Lex(dlms[j], j);
        case NEG:
            if (c == '='){
                add();
                gc();
            }
            j = look(buf, TS);
            return Lex(dlms[j], j);

        case DELIM:
            if ((j = look(buf, TS))){
                gc();
                return Lex(dlms[j], j);
            }
            else throw c;
        case FIN:
            not_end = false;
            return Lex(LEX_NULL, 0);
        }
    }
};

template <typename T>
class Stack{
    T* s;
    int top = 0, max_size;

public:
    Stack(int max_size = 100) {s = new T[max_size]; this->max_size = max_size;}
    void push(T t){
        if (!is_full())
            s[top++] = t;
        else
            throw "Stack is full";
    }
    T pop(){
        if (!is_empty())
            return s[--top];
        else
            throw "Stack is empty";
    }

    void reset() { top = 0; }
    bool is_empty() const { return top == 0; }
    bool is_full() const { return top == max_size - 1; }
    ~Stack() {delete []s;}
};

class Function{
    char *name = nullptr;
    int poliz_num;
    int num_str = 0;
    int num_int = 0;
    type_of_lex array[30];
    int index = 0;

public:
    Function() {}
    char *get_name() const { return name; }
    void set_name(const char *n){
        name = new char[strlen(n) + 1];
        strcpy(name, n);
    }
    void add_int() { num_int++; }
    void add_str() { num_str++; }
    void push(type_of_lex l) { array[index++] = l; }
    type_of_lex get_el(int i) const { return array[i]; }
    void set_num_str(int i) { num_str = i; }
    int get_num_str() const { return num_str; }
    void set_num_int(int i) { num_int = i; }
    int get_num_int() const { return num_int; }
    void set_poliz_num(int i) { poliz_num = i; }
    int get_poliz_num() const { return poliz_num; }
    void add_var(type_of_lex l){
        if(l == LEX_INT)
            add_int();
        else if(l == LEX_STRING)
            add_str();
        else
            throw l;
        push(l);
    }
};

class FTable{
    Function *table = nullptr;
    int top;
    int size;

public:
    FTable(int const size) : size(size){
        table = new Function[size];
        top = 1;
    }
    ~FTable(){
        if (table != nullptr)
            delete[] table;
    }
    int put(const char *buf){
        table[top].set_name(buf);
        top++;
        return top - 1;
    }
    Function& operator[] (int i) { return table[i]; }
    bool check_fn(const char *buf) const{
        for (int i = 1; i < top; i++)
            if (!strcmp(buf, table[i].get_name()))
                return true;
        return false;
    }
    int get_top() const { return top; }
} FN(100);

class Poliz{
    Lex *p;
    int max_size;
    int top;

public:
    Poliz(int max_size = 100){
        p = new Lex[max_size];
        this->max_size = max_size;
        top = 0;
    }
    ~Poliz() { delete[] p; }
    void put_lex(Lex l){
        p[top++] = l;
    }
    type_of_lex get_c_type() { return p[top - 1].get_type(); }
    void put_lex(Lex l, int place) { p[place] = l; }
    void blank() { top++; }
    int get_top() const { return top; }
    Lex &operator[](int idx)
    {
        if (idx > max_size)
            throw "Poliz: out of array";
        if (idx > top)
            throw "Poliz: indefine element of array";
        return p[idx];
    }
};

ostream & operator << (ostream & s, Poliz & p){
    for (int i = 0; i < p.get_top(); i++){
        if (p[i].get_type() != LEX_NULL)
            s << p[i] << endl;
    }
    return s;
}




class Parser{
    Lex cur_lex;
    type_of_lex c_type;
    int c_val;
    type_of_lex tmp;
    string c_strval;
    Scanner scan;
    Stack <int> st_int;
    Stack <type_of_lex> st_lex;
    Stack <string> st_str;


    void gl(){
        cur_lex = scan.get_lex();
        c_type = cur_lex.get_type();
        c_val = cur_lex.get_value();
        c_strval = cur_lex.get_str_val();
    }

    void check_id ();
    void check_op ();
    void check_not ();
    void check_decl();
    void eq_type ();
    void eq_bool ();

    void check_Lparen(){
        if (c_type != LEX_LPAREN)
            throw cur_lex;
        gl();
    }
    
    void check_Rparen(){
        if (c_type != LEX_RPAREN)
            throw cur_lex;
        gl();
    }

    void check_semicolon(){
        if (c_type != LEX_SEMICOLON)
            throw cur_lex;
        gl();
    }

    void P();
    void B();
    void S();
    void DE();
    void PE();
    void E();
    void E1();
    void T();
    void F();

    void STR();
    void I();

public:
    Poliz prog;
    Parser(char *program) : scan(program), prog(1000) {}
    void analyze(){
        gl();
        P();
        cout << "Success" << endl;
    }
};

void Parser::check_id(){
        if (TID[c_val].get_declare())
            st_lex.push(TID[c_val].get_type());
        else {
            throw "not declarated";
        }
}

void Parser::check_not(){
    if (st_lex.pop() != LEX_BOOL)
        throw "wrong type is in not";
    else
    {
        st_lex.push(LEX_BOOL);
        prog.put_lex(Lex(LEX_NOT, 0));
    }
}

void Parser::check_op(){
    type_of_lex t1, t2, op, r, t;
    t2 = st_lex.pop();
    op = st_lex.pop();
    t1 = st_lex.pop();
    t = t1;
    if (t1 == t2)
    {
        if (t == LEX_BOOL)
        {
            if (op == LEX_AND || op == LEX_OR)
                r = LEX_BOOL;
            else
                throw op;
        }
        else if (t == LEX_INT)
        {
            if (op == LEX_EQ || op == LEX_NEQ || op == LEX_GRT || op == LEX_LSS || op == LEX_GOE || op == LEX_LOE)
                r = LEX_BOOL;
            else if (op == LEX_MINUS || op == LEX_PLUS || op == LEX_SLASH || op == LEX_STAR)
                r = LEX_INT;
            else
                throw op;
        }
        else if (t == LEX_STRING)
        {
            if (op == LEX_EQ || op == LEX_NEQ || op == LEX_GRT || op == LEX_LSS || op == LEX_GOE || op == LEX_LOE)
                r = LEX_BOOL;
            else if (op == LEX_PLUS)
                r = LEX_STRING;
            else
                throw op;
        }
        else
            throw t;
    }
    else
        throw t2;
    st_lex.push(r);
    prog.put_lex(Lex(op, 0));
}

void Parser::eq_bool(){
    if (st_lex.pop() != LEX_BOOL)
        throw "expression is not boolean";
}

void Parser::eq_type(){
    if (st_lex.pop() != st_lex.pop())
        throw "wrong types are in assignment";
}

void Parser::check_decl(){
    if (!TID[c_val].get_declare())
        throw cur_lex;
}

void Parser::P(){
    if (c_type != LEX_PROGRAM){
        throw cur_lex;
    }else{
        gl();
        B();
    }
}

void Parser::B(){
    if (c_type == LEX_STBLOCK){
        gl();
        while(c_type != LEX_FNBLOCK){ S();}
        gl();
    } else throw cur_lex;
}

void Parser::S(){
    int pl0, pl1, pl2, pl3;
    if(c_type == LEX_INT){
        gl();
        I();
    }
    else if(c_type == LEX_STRING){
        gl();
        STR();
    }
    else if(c_type == LEX_IF){
        gl();
        check_Lparen();
        PE();
        eq_bool();
        check_Rparen();
        pl2 = prog.get_top();
        prog.blank();
        prog.put_lex(Lex(POLIZ_FGO, 0));

        if (c_type == LEX_STBLOCK) B();
        else S();

        if (c_type == LEX_ELSE){
            pl3 = prog.get_top();
            prog.blank();
            prog.put_lex(Lex(POLIZ_GO, 0));
            prog.put_lex(Lex(POLIZ_LABEL, prog.get_top()), pl2);
            gl();
            if (c_type == LEX_STBLOCK){
                B();
                prog.put_lex(Lex(POLIZ_LABEL, prog.get_top()), pl3);
            }
            else{
                S();
                prog.put_lex(Lex(POLIZ_LABEL, prog.get_top()), pl3);
            }
        }
        else prog.put_lex(Lex(POLIZ_LABEL, prog.get_top()), pl2);
    }
    else if (c_type == LEX_WHILE){
        pl0 = prog.get_top();
        gl();
        check_Lparen();
        PE();
        eq_bool();
        check_Rparen();
        pl1 = prog.get_top();
        prog.blank();
        prog.put_lex(Lex(POLIZ_FGO, 0));

        if (c_type == LEX_STBLOCK) B();
        else S();

        prog.put_lex(Lex(POLIZ_LABEL, pl0));
        prog.put_lex(Lex(POLIZ_GO, 0));
        prog.put_lex(Lex(POLIZ_LABEL, prog.get_top()), pl1);
    }
    else if (c_type == LEX_READ){
        int i;
        i = FN.put("read");
        gl();
        check_Lparen();
        while(c_type == LEX_ID)
        {
            if (c_type != LEX_ID)
                throw cur_lex;
            check_decl();
            FN[i].add_var(TID[c_val].get_type());
            prog.put_lex(Lex(POLIZ_ADDRESS, c_val));
            gl();
            if(c_type == LEX_COMMA)
                gl();
            else break;
        }
        check_Rparen();
        check_semicolon();
        FN[i].set_poliz_num(prog.get_top());
        prog.put_lex(Lex(LEX_READ, 0));
    }
    else if (c_type == LEX_WRITE)
    {
        int i;
        i = FN.put("write");
        gl();
        check_Lparen();
        E();
        FN[i].add_var(tmp);
        while(c_type == LEX_COMMA)
        {
            gl();
            E();
            FN[i].add_var(tmp);
        }
        check_Rparen();
        check_semicolon();
        FN[i].set_poliz_num(prog.get_top());
        prog.put_lex(Lex(LEX_WRITE, 0));
    }
    else if (c_type == LEX_ID)
    {
        check_id();
        prog.put_lex(Lex(POLIZ_ADDRESS, c_val));
        gl();
        if (c_type == LEX_ASSIGN)
        {
            gl();
            DE();
            eq_type();
            prog.put_lex(Lex(LEX_ASSIGN, 0));
        }
        else
            throw cur_lex;
        check_semicolon();
        prog.put_lex(Lex(LEX_SEMICOLON, 0));
    }
    else
        B();
}

void Parser::STR(){
    if (c_type != LEX_ID)
        throw cur_lex;

    int var_num;
    while (c_type != LEX_SEMICOLON)
    {
        if (c_type != LEX_ID)
            throw cur_lex;

        var_num = c_val;
        if (TID[c_val].get_declare() == true)
            throw "double declaration";

        TID[var_num].set_declare();
        TID[var_num].set_type(LEX_STRING);

        gl(); // comma || assign
        if (c_type == LEX_ASSIGN)
        {
            prog.put_lex(Lex(POLIZ_ADDRESS, var_num));
            gl();
            if (c_type == LEX_STRING_VAL)
            {
                TID[var_num].set_assign();
                prog.put_lex(Lex(LEX_STRING_VAL, c_strval));
                prog.put_lex(Lex(LEX_ASSIGN, 0));
                prog.put_lex(Lex(LEX_SEMICOLON, 0));
            }
            else
                throw cur_lex;
            gl(); // comma || semicolon
        }
        if (c_type == LEX_COMMA)
            gl();
    }
    check_semicolon();
};

void Parser::I(){
    if (c_type != LEX_ID)
        throw cur_lex;

    int var_num;

    while (c_type != LEX_SEMICOLON){
        if (c_type != LEX_ID)
            throw cur_lex;

        var_num = c_val;
        if (TID[c_val].get_declare() == true)
            throw "double declaration";

        TID[var_num].set_declare();

        TID[var_num].set_type(LEX_INT);

        gl(); // comma || assign
        if (c_type == LEX_ASSIGN){
            prog.put_lex(Lex(POLIZ_ADDRESS, var_num));
            gl();
            if (c_type == LEX_NUM){
                TID[var_num].set_assign();
                prog.put_lex(Lex(LEX_NUM, c_val));
                prog.put_lex(Lex(LEX_ASSIGN, 0));
                prog.put_lex(Lex(LEX_SEMICOLON, 0));
            } else throw cur_lex;
            gl(); // comma || semicolon
        }
        if (c_type == LEX_COMMA)
            gl();
    }
    check_semicolon();
}

void Parser::DE(){
    int count = 0;
    E();
    while(prog.get_c_type() == POLIZ_ADDRESS){
        count++;
        gl();
        if (c_type == LEX_ID){
            check_id();
            int i = c_val;
            gl();
            if(c_type == LEX_ASSIGN)
                prog.put_lex(Lex(POLIZ_ADDRESS, i));
            else 
                prog.put_lex(Lex(LEX_ID, i));
        }
        if (c_type == LEX_NUM){
            st_lex.push(LEX_INT);
            prog.put_lex(cur_lex);
            gl();
        }
        else if (c_type == LEX_STRING_VAL){
            st_lex.push(LEX_STRING);
            prog.put_lex(cur_lex);
            gl();
        }
    }
    for (int i = 0; i < count; i++)
        prog.put_lex(Lex(LEX_ASSIGN, 0));
}

void Parser::PE(){
    E();
    if (c_type == LEX_AND || c_type == LEX_OR){
        st_lex.push(c_type);
        gl();
        PE();
        check_op();
    }
}

void Parser::E(){
    E1();
    if (c_type == LEX_EQ || c_type == LEX_LSS || c_type == LEX_GRT || c_type == LEX_LOE || c_type == LEX_GOE || c_type == LEX_NEQ){
        st_lex.push(c_type);
        gl();
        E1();
        check_op();
    }
}

void Parser::E1(){
    T();
    while (c_type == LEX_PLUS || c_type == LEX_MINUS){
        st_lex.push(c_type);
        gl();
        T();
        check_op();
    }
}

void Parser::T(){
    F();
    while (c_type == LEX_STAR || c_type == LEX_SLASH){
        st_lex.push(c_type);
        gl();
        F();
        check_op();
    }
}

void Parser::F(){
    if (c_type == LEX_ID){
        check_id();
        int i = c_val;
        tmp = TID[i].get_type();
        gl();
        if(c_type == LEX_ASSIGN)
                prog.put_lex(Lex(POLIZ_ADDRESS, i));
        else 
                prog.put_lex(Lex(LEX_ID, i));
    }
    else if (c_type == LEX_NUM){
        tmp = LEX_INT;
        st_lex.push(LEX_INT);
        prog.put_lex(cur_lex);
        gl();
    }
    else if (c_type == LEX_STRING_VAL){
        tmp = LEX_STRING;
        st_lex.push(LEX_STRING);
        prog.put_lex(cur_lex);
        gl();
    }
    else if (c_type == LEX_TRUE){
        st_lex.push(LEX_BOOL);
        prog.put_lex(Lex(LEX_TRUE, true));
        gl();
    }
    else if (c_type == LEX_FALSE){
        st_lex.push(LEX_BOOL);
        prog.put_lex(Lex(LEX_FALSE, false));
        gl();
    }
    else if (c_type == LEX_NOT){
        gl();
        F();
        check_not();
    }
    else if (c_type == LEX_LPAREN){
        gl();
        E();
        if (c_type == LEX_RPAREN)
            gl();
        else
            throw cur_lex;
    }
    else
        throw cur_lex;
}


int main(int argc, char ** argw) {
    if (argc < 2) {
        cout << "Couldn`t find name" << endl;
        return 1;
    } else {
        Parser p(argw[1]);
        try{
            p.analyze();
        }
        catch (Lex l) {cout << l;
            return 1;
        }
        catch (const char * s) {cout << s << endl;
            return 1;
        }
        cout << p.prog;
    }
    return 0;
}