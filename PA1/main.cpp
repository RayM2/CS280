/*
Rayhan Mohammed PA1
March 3, 2024
*/

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <stdio.h>
#include <ctype.h>
#include <list>
#include <algorithm>
#include "message.h"
#include "lex.h"

int main(int argc, char *argv[])
{
    string filename;
    bool allFlag = false;
    bool intFlag = false;
    bool realFlag = false;
    bool strFlag = false;
    bool idFlag = false;
    bool kwFlag = false;

    list<int> integerList;
    list<float> realList;
    list<string> stringList;
    map<string, int> identifierMap;
    map<string, int> keywordMap;
    string onlyOne = "ONLY ONE FILE NAME IS ALLOWED.";
    string noInput = "NO SPECIFIED INPUT FILE.";

    if (argc >= 2)
    {
        filename = argv[1];
    }

    else
    {
        cout << noInput << endl;
        exit(1);
    }

    for (int i = 2; i < argc; i++)
    {
        string argument = argv[i];
        if (argument == "-all")
        {
            allFlag = true;
        }
        else if (argument == "-int")
        {
            intFlag = true;
        }
        else if (argument == "-real")
        {
            realFlag = true;
        }
        else if (argument == "-str")
        {
            strFlag = true;
        }
        else if (argument == "-id")
        {
            idFlag = true;
        }
        else if (argument == "-kw")
        {
            kwFlag = true;
        }
        else if (string(argv[i])[0] == '-')
        {
            cout << "UNRECOGNIZED FLAG {" << argv[i] << "}" << std::endl;
            exit(1);
        }

        else
        {
            cout << onlyOne << endl;
            exit(1);
        }
    }

    ifstream readFile;
    readFile.open(filename.c_str());

    if (!readFile.is_open())
    {
        cout << "CANNOT OPEN THE FILE " + filename << endl;
        exit(1);
    }

    if (readFile.peek() == EOF)
    {
        cout << "Empty File." << std::endl;
        exit(1);
    }
    int numLines = 1;
    int numTokens = 0;

    LexItem nextToken;
    while ((nextToken = getNextToken(readFile, numLines)).GetToken() != ERR && nextToken.GetToken() != DONE)
    {

        map<Token, string> enumStringMap = {
            {Token::IF, "IF"},
            {Token::ELSE, "ELSE"},
            {Token::PRINT, "PRINT"},
            {Token::INTEGER, "INTEGER"},
            {Token::REAL, "REAL"},
            {Token::CHARACTER, "CHARACTER"},
            {Token::END, "END"},
            {Token::THEN, "THEN"},
            {Token::PROGRAM, "PROGRAM"},
            {Token::LEN, "LEN"},
            {Token::IDENT, "IDENT"},
            {Token::ICONST, "ICONST"},
            {Token::RCONST, "RCONST"},
            {Token::SCONST, "SCONST"},
            {Token::BCONST, "BCONST"},
            {Token::PLUS, "PLUS"},
            {Token::MINUS, "MINUS"},
            {Token::MULT, "MULT"},
            {Token::DIV, "DIV"},
            {Token::ASSOP, "ASSOP"},
            {Token::EQ, "EQ"},
            {Token::POW, "POW"},
            {Token::GTHAN, "GTHAN"},
            {Token::LTHAN, "LTHAN"},
            {Token::CAT, "CAT"},
            {Token::COMMA, "COMMA"},
            {Token::LPAREN, "LPAREN"},
            {Token::RPAREN, "RPAREN"},
            {Token::DOT, "DOT"},
            {Token::DCOLON, "DCOLON"},
            {Token::DEF, "DEF"},
            {Token::ERR, "ERR"},
            {Token::DONE, "DONE"},
            {Token::THEN, "THEN"}

        };

        numLines = nextToken.GetLinenum();
        numTokens += 1;

        // keyword adder:
        if ((nextToken.GetToken() == IF) || (nextToken.GetToken() == ELSE) || (nextToken.GetToken() == PRINT) || (nextToken.GetToken() == INTEGER) ||
    (nextToken.GetToken() == REAL) || (nextToken.GetToken() == CHARACTER) || (nextToken.GetToken() == END) || (nextToken.GetToken() == THEN) ||
    (nextToken.GetToken() == PROGRAM) || (nextToken.GetToken() == LEN))
    {
        string key = nextToken.GetLexeme();
        map<string, int>::iterator iter = keywordMap.find(key);

        if (iter != keywordMap.end())
        {
            iter->second += 1;
        }
        else
        {
            keywordMap[key] = 1;
        }
    }
        // identifier adder
        if (nextToken.GetToken() == IDENT)
        {
            string key = nextToken.GetLexeme();
            auto iter = identifierMap.find(key);

            if (iter != identifierMap.end())
            {
                iter->second += 1;
            }
            else
            {
                identifierMap[key] = 1;
            }
        }
        if (nextToken.GetToken() == SCONST)
        {
            stringList.push_back(nextToken.GetLexeme());
        }
        if (nextToken.GetToken() == RCONST)
        {

            auto it = find(realList.begin(), realList.end(), std::stof(nextToken.GetLexeme()));

            if (it == realList.end())
            {
                realList.push_back(std::stof(nextToken.GetLexeme()));
            }
        }
        if (nextToken.GetToken() == ICONST)
        {

            auto it = find(integerList.begin(), integerList.end(), stoi(nextToken.GetLexeme()));

            if (it == integerList.end())
            {
                integerList.push_back(stoi(nextToken.GetLexeme()));
            }
        }
        if (allFlag)
        {
            cout << nextToken << endl;
        }
    }
    if (nextToken == ERR)
    {
        cout << "Error in line " << nextToken.GetLinenum() << ": Unrecognized Lexeme {" << nextToken.GetLexeme() << "}" << endl;
        readFile.close();
        exit(1);
    }
    if (nextToken == DONE)
    {
        cout << endl;
        cout << "Lines: " << numLines - 1 << endl;
        cout << "Total Tokens: " << numTokens << endl;
        cout << "Identifiers: " << identifierMap.size() << endl;
        cout << "Integers: " << integerList.size() << endl;
        cout << "Reals: " << realList.size() << endl;
        cout << "Strings: " << stringList.size() << endl;
        if (idFlag)
    {
        if (!(identifierMap.empty()))
        {
            cout << "IDENTIFIERS:" << endl;
            map<string, int>::iterator it = identifierMap.begin();
            map<string, int>::iterator last = identifierMap.end();
            --last;
            

            for (; it != last; ++it)
            {
                cout << it->first << " (" << it->second << "), ";
            }
            cout << it->first << " (" << it->second << ")" << endl;
        }
    }
        if (kwFlag)
{
    if (!(keywordMap.empty()))
    {
        cout << "KEYWORDS:" << endl;
        map<string, int>::iterator it = keywordMap.begin();
        map<string, int>::iterator last = keywordMap.end();
        --last;
        

        for (; it != last; ++it)
        {
            cout << it->first << " (" << it->second << "), ";
        }
        cout << it->first << " (" << it->second << ")" << endl;
    }
}
        if (intFlag)
{
    if (!(integerList.empty()))
{
    integerList.sort();

    cout << "INTEGERS:" << endl;
    list<int>::iterator it = integerList.begin();
    list<int>::iterator last = integerList.end();
    --last;

    for (; it != last; ++it)
    {
        cout << *it << ", ";
    }
    cout << *it << endl;
}
}
        if (realFlag)
{
    if (!(realList.empty()))
    {
        realList.sort();

        cout << "REALS:" << endl;
        list<float>::iterator it = realList.begin();
        list<float>::iterator last = realList.end();
        --last;
        

        for (; it != last; ++it)
        {
            cout << *it << ", ";
        }
        cout << *it << endl;
    }
}
        if (strFlag)
{
        if (strFlag)
{
    if (!stringList.empty())
    {
        cout << "STRINGS:" << endl;
        list<string>::iterator it = stringList.begin();
        list<string>::iterator last = stringList.end();
        --last;
        

        for (; it != last; ++it)
        {
            string temp = *it;
            temp = "\"" + temp.substr(1, (temp.size() - 2)) + "\", ";
            cout << temp;
        }

        string temp = *it;
        temp = "\"" + temp.substr(1, (temp.size() - 2)) + "\"";
        cout << temp << endl;
    }
}
}
    }
    readFile.close();
    exit(0);
}