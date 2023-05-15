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
    void put_declare (){
        declare = true;
    }
    type_of_lex get_type (){
        return type;
    }
    void put_type (type_of_lex t){
        type = t;
    }
    bool get_assign (){
        return assign;
    }
    void put_assign (){
        assign = true;
    }
    int get_value (){
        return value;
    }
    void put_value (int v){
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

class Parser{
    Lex cur_lex;
    type_of_lex c_type;
    int c_val;
    string c_strval;
    Scanner scan;

    void gl(){
        cur_lex = scan.get_lex();
        c_type = cur_lex.get_type();
        c_val = cur_lex.get_value();
        c_strval = cur_lex.get_str_val();
    }

    void dec ( type_of_lex type);
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
    Parser(const char * program) : scan(program) {};

    void analyze(){
        gl();
        P();
        cout << "Success" << endl;
    }
};

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
        check_Rparen();

        if (c_type == LEX_STBLOCK) B();
        else S();

        if (c_type == LEX_ELSE){
            gl();
            if (c_type == LEX_STBLOCK) B();
            else S();
        }
    }
    else if (c_type == LEX_WHILE){
        gl();
        check_Lparen();
        PE();
        check_Rparen();
        if (c_type == LEX_STBLOCK) B();
        else S();
    }
    else if (c_type == LEX_READ){
        gl();
        check_Lparen();
        while(c_type == LEX_ID){
            if (c_type != LEX_ID)
                throw cur_lex;
            gl();
            if(c_type == LEX_COMMA) gl();
            else break;
        }
        check_Rparen();
        check_semicolon();
    }
    else if (c_type == LEX_WRITE){
        gl();
        check_Lparen();
        E();
        while(c_type == LEX_COMMA){
            gl();
            E();
        }
        check_Rparen();
        check_semicolon();
    }
    else if (c_type == LEX_ID){
        gl();
        if (c_type == LEX_ASSIGN){
            gl();
            DE();
        }
        else throw cur_lex;
        check_semicolon();
    }
    else B();
}

void Parser::STR()
{
    if (c_type != LEX_ID)
        throw cur_lex;
    while (c_type != LEX_SEMICOLON){
        if (c_type != LEX_ID)
            throw cur_lex;
        gl(); // comma || assign
        if (c_type == LEX_ASSIGN){
            gl();
            if (c_type == LEX_STRING_VAL){}
            else
                throw cur_lex;
            gl(); // comma || semicolon
        }
        if (c_type == LEX_COMMA)
            gl();
    }
    check_semicolon();
};

void Parser::I()
{
    if (c_type != LEX_ID) throw cur_lex;
    while (c_type != LEX_SEMICOLON){
        if (c_type != LEX_ID)
            throw cur_lex;

        gl(); // comma || assign
        if (c_type == LEX_ASSIGN){
            gl();
            if (c_type == LEX_NUM){}
            else throw cur_lex;
            gl(); // comma || semicolon
        }
        if (c_type == LEX_COMMA) gl();
    }
    check_semicolon();
}

void Parser::DE() {E();}

void Parser::PE(){
    E();
    if (c_type == LEX_AND || c_type == LEX_OR){
        gl();
        PE();
    }
}

void Parser::E()
{
    E1();
    if (c_type == LEX_EQ || c_type == LEX_LSS || c_type == LEX_GRT || c_type == LEX_LOE || c_type == LEX_GOE || c_type == LEX_NEQ){
        gl();
        E1();
    }
}

void Parser::E1()
{
    T();
    while (c_type == LEX_PLUS || c_type == LEX_MINUS){
        gl();
        T();
    }
}

void Parser::T()
{
    F();
    while (c_type == LEX_STAR || c_type == LEX_SLASH){
        gl();
        F();
    }
}

void Parser::F()
{
    if (c_type == LEX_ID || c_type == LEX_NUM || c_type == LEX_STRING_VAL || c_type == LEX_TRUE || c_type == LEX_FALSE){
        gl();
    }
    else if (c_type == LEX_NOT)
    {
        gl();
        F();
    }
    else if (c_type == LEX_LPAREN)
    {
        gl();
        E();
        if (c_type == LEX_RPAREN)
            gl();
        else
            throw cur_lex;
    }
    else
        throw cur_lex;
};



int main(int argc, char ** argw) {
    if (argc < 2) {
        cout << "Couldn`t find name" << endl;
        return 1;
    } else {
        Parser p(argw[1]);
        try{
            p.analyze();
        }
        catch (Lex l) {cout << l;};
    }
    return 0;
}