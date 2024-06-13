#ifndef SCANNER_H
#define SCANNER_H
#include <vector>
#include <string>


using namespace std;

class scanner
{
public:
    scanner();
    void start();
    
    void functions(string rec, vector<string> parametros); //Recibe el comando y lista de instrucciones
    string rec(string text);                      
    vector<string> recAgrupar(string text, string text_recAgrupar);
    vector<string> recAgrupar_tokens(string text);
    bool compare(string a, string b);
    string upper(string a);
    //errores
    void errores(string operacion, string mensaje);
    //rsp
    void respuesta(string operacion, string mensaje);
    //excute la que se llama al reconocer
    void funcion_excec(vector<string> tokens);
    //llama esta
    void excec(string path);
    //confirmar si seguir 
    bool confirmar(string mensaje);
};

#endif