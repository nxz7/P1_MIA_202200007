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

string Funz::upper(string dato)
{
    string u;
    locale loc;
    for (int i = 0; i < dato.length(); i++)
    {
        u += toupper(dato[i], loc);
    }
    return u;
}

string Funz::lower(string dato)
{
    string u;
    locale loc;
    for (int i = 0; i < dato.length(); i++)
    {
        u += tolower(dato[i], loc);
    }
    return u;
}

bool Funz::equiv(string dato1, string dato2)
{
    dato1 = upper(dato1);
    dato2 = upper(dato2);
    if (dato1 == dato2)
    {
        return true;
    }
    return false;
}


//para que tire los errores
void Funz::notif(string title, string message)
{
    cout << "\033[1;31m Error! : \033"
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


void Funz::msmSalida(string title, string message){
    //rosa
    cout << "\033[38;2;255;105;180m(" + title + "): \033[0m" << message << endl;
}
