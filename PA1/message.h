/*
Rayhan Mohammed CS280-006
PA1 Building a Lexical Analyzer
*/
#include <iostream>
#include <map>
#include <fstream>
#include <list>
#include <algorithm>
#include <sstream>
#include <cctype>
#include <string>
#include "lex.h"
#include <istream>

LexItem id_or_kw(const string& lexeme, int linenum) {
    static map<string, Token> MapKey = {
        {"PROGRAM", PROGRAM},
        {"PRINT", PRINT},
        {"IF", IF},
        {"ELSE", ELSE},
        {"END", END},
        {"INTEGER", INTEGER},
        {"REAL", REAL},
        {"CHARACTER", CHARACTER},
        {"THEN", THEN}
    };

    auto checkReserve = MapKey.find(lexeme);
    if (checkReserve != MapKey.end()) {
        return LexItem(checkReserve->second, lexeme, linenum);
    } else {
        return LexItem(IDENT, lexeme, linenum);
    }
}

ostream& operator<<(ostream& out, const LexItem& tok) {
    Token tokenCase = tok.GetToken();
    string lex = tok.GetLexeme();
    int lineNumber = tok.GetLinenum();

    switch (tok.GetToken()) {
        case IDENT:
            out << "IDENT: '" << lex << "'";
            break;
        case ICONST:
        case RCONST:
            out << tokenCase << ": (" << lex << ")";
            break;
        case SCONST:
            out << ": \"" << lex << "\"";
            break;
        case ERR:
            out << "Error in line " << lineNumber << ": Unrecognized Lexeme " << lex ;
            break;
        default: 
            out << tokenCase;
    }

    return out;
}


LexItem getNextToken(istream &in, int &linenumber)
{

    enum LexState
    {
        START,
        INID,
        INSTRING,
        ININT,
        INREAL,
        INCOMMENT
    };
    LexState lexstate = START;
    string lexeme;
    char ch;

    while (in.get(ch))
    {
        switch (lexstate)
        {
        case START:
            if (isalpha(ch))
            {
                lexeme += ch;
                lexstate = INID;
            }
            else if (isdigit(ch))
            {
                lexeme += ch;
                lexstate = ININT;
            }
            else if (ch == '.')
            {
                lexeme += ch;
                lexstate = INREAL;
            }
            else if (ch == '"' || ch == '\'')
            {
                lexeme += ch;
                lexstate = INSTRING;
            }
            else if (ch == '!')
            {
                lexstate = INCOMMENT;
            }
            else if (isspace(ch))
            {
                if (ch == '\n')
                {
                    linenumber++;
                }
            }
            else
            {
                lexeme += ch;
                switch (ch)
                {
                case '+':
                    return LexItem(PLUS, lexeme, linenumber);
                case '-':
                    return LexItem(MINUS, lexeme, linenumber);
                case '<':
                    return LexItem(LTHAN, lexeme, linenumber);
                case '>':
                    return LexItem(GTHAN, lexeme, linenumber);
                case '*':
                    if (in.peek() == '*')
                    {
                        return LexItem(POW, lexeme, linenumber);
                    }
                    if (in.peek() == ',')
                    {
                        return LexItem(DEF, lexeme, linenumber);
                    }
                    else
                    {
                        return LexItem(MULT, lexeme, linenumber);
                    }
                case '/':
                    if (in.peek() == '/')
                    {
                        return LexItem(CAT, lexeme, linenumber);
                    }
                    else
                    {
                        return LexItem(DIV, lexeme, linenumber);
                    }
                case '=':
                    if (in.peek() == '=')
                    {
                        return LexItem(EQ, lexeme, linenumber);
                    }
                    else
                    {
                        return LexItem(ASSOP, lexeme, linenumber);
                    }
                case '(':
                    return LexItem(LPAREN, lexeme, linenumber);
                case ')':
                    return LexItem(RPAREN, lexeme, linenumber);
                case ':':
                    return LexItem(DCOLON, lexeme, linenumber);
                case ',':
                    return LexItem(COMMA, lexeme, linenumber);
                case '.':
                    return LexItem(DOT, lexeme, linenumber);
                default:
                    return LexItem(ERR, lexeme, linenumber);
                }
            }
            break;
        case INID:
            if (isalnum(ch) || ch == '_')
            {
                lexeme += ch;
            }
            else
            {
                in.putback(ch);
                return id_or_kw(lexeme, linenumber);
            }
            break;
        case INSTRING:
            lexeme += ch;
            if ((ch == '"' || ch == '\'') && lexeme[0] == ch)
            {
                return LexItem(SCONST, lexeme, linenumber);
            }
            break;
        case ININT:
            if (isdigit(ch))
            {
                lexeme += ch;
            }
            else if (ch == '.')
            {
                lexeme += ch;
                lexstate = INREAL;
            }
            else
            {
                in.putback(ch);
                return LexItem(ICONST, lexeme, linenumber);
            }
            break;
        case INREAL:
            if (isdigit(ch))
            {
                lexeme += ch;
            }
            else
            {
                in.putback(ch);
                return LexItem(RCONST, lexeme, linenumber);
            }
            break;
        case INCOMMENT:
            if (ch == '\n')
            {
                linenumber++;
                lexstate = START;
            }
            break;
        }
    }

    return LexItem(DONE, "", linenumber);
}
