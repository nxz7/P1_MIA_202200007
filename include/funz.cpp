#include "../lib/funz.h"

#include <iostream>
#include <stdlib.h>
#include "string"
#include <locale>

#include <bits/stdc++.h>

using namespace std;

Funz::Funz()
{
}

string Funz::upper(string s)
{
    string u;
    locale loc;
    for (int i = 0; i < s.length(); i++)
    {
        u += toupper(s[i], loc);
    }
    return u;
}

string Funz::lower(string s)
{
    string u;
    locale loc;
    for (int i = 0; i < s.length(); i++)
    {
        u += tolower(s[i], loc);
    }
    return u;
}

bool Funz::equiv(string s1, string s2)
{
    s1 = upper(s1);
    s2 = upper(s2);
    if (s1 == s2)
    {
        return true;
    }
    return false;
}


//para que tire los errores
void Funz::notif(string title, string message)
{
    cout << "\033[1;31m Error: \033"
        << "\033[0;31m(" + title + ")~> \033[0m"
        << message << endl;
}


//si y no, pide confirmacion
bool Funz::confirmation(string title, string message)
{
    cout << "\033[1;36m desea confirmar: \033"
        << "\033[0;36m(" + title + ")~> \033[0m"
        << "¿" + message + "? s/n : ";
    string action;
    getline(cin, action);
    if (equiv(action, "S") || equiv(action, "s")){
        
        return true;
    }
    return false;
}

void Funz::msmSalida(string title, string message)
{
    cout << "\033[0;32m (" + title + "): \033[0m"
        << message << endl;
}