#ifndef SHARED_H
#define SHARED_H

#include <string>
#include <bits/stdc++.h>

using namespace std;

class Shared
//notif es el handler
{
public:
    Shared();
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