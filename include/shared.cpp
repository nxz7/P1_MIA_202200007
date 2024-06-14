#include "../lib/shared.h"

#include <iostream>
#include <stdlib.h>
#include "string"
#include <locale>

#include <bits/stdc++.h>

using namespace std;

Shared::Shared()
{
}

string Shared::upper(string s)
{
    string u;
    locale loc;
    for (int i = 0; i < s.length(); i++)
    {
        u += toupper(s[i], loc);
    }
    return u;
}

string Shared::lower(string s)
{
    string u;
    locale loc;
    for (int i = 0; i < s.length(); i++)
    {
        u += tolower(s[i], loc);
    }
    return u;
}

bool Shared::equiv(string s1, string s2)
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
void Shared::notif(string title, string message)
{
    cout << "\033[1;31m Error: \033"
        << "\033[0;31m(" + title + ")~> \033[0m"
        << message << endl;
}


//si y no, pide confirmacion
bool Shared::confirmation(string title, string message)
{
    cout << "\033[1;36m Confirmación: \033"
        << "\033[0;36m(" + title + ")~> \033[0m"
        << "¿" + message + "? Y/N : ";
    string action;
    getline(cin, action);
    if (equiv(action, "y") || equiv(action, "yes"))
    {
        return true;
    }
    return false;
}

void Shared::msmSalida(string title, string message)
{
    cout << "\033[0;32m (" + title + "): \033[0m"
        << message << endl;
}