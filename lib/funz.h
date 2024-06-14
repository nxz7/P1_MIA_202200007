#ifndef FUNZ_H
#define FUNZ_H

#include <string>
#include <bits/stdc++.h>

using namespace std;

class Funz
//notif es el handler
{
public:
    Funz();
    //------------------------------------------mayuscula
    string upper(string s);
    //------------------------------------------minuscula
    string lower(string s);
    //------------------------------------------compara dos, si son igual T, si son dif F
    bool equiv(string s1, string s2);
    //------------------------------------------mensaje que se muestra - error
    void notif(string title, string message);
    bool confirmation(string title, string message);
    //------------------------------------------mensaje de respuesta 
    void msmSalida(string title, string message);

};

#endif // END OF DECLARATION