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
}

int main(int argc, char ** argw) {
    if (argc < 2) {
        cout << "Couldn`t find name" << endl;
        return 1;
    } else {
        Scanner s(argw[1]);
        while (s.check_end()) {
            cout << s.get_lex() << endl;
        }
    }
    return 0;
}