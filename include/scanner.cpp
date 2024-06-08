#include "../lib/scanner.h"
#include "../lib/disco.h"
#include "../lib/mount.h"

#include <iostream>
#include <stdlib.h>
#include <locale>
#include <cstring>
#include <locale>
#include <fstream>
#include <vector>
#include <cstdlib>
using namespace std;

Disk disco;

Mount mount;

bool logued = false;
scanner::scanner()
{
}
void Clear()
{
    cout << "\x1B[2J\x1B[H";
}

void scanner::start()
{
    system("clear");
        cout << ">>>>>>>>>>>>>>>>> PROYECTO 1 \n" << endl;
        cout << ">>>>>>>>>>>>>>>>> NATALIA MARIEL CALDERON ECHEVERRIA \n" << endl;
        cout << ">>>>>>>>>>>>>>>>>202200007 \n" << endl;
        cout << ">>>>>>>>>>>>>>>>>INGRESE UN COMANDO(exit -> salir) \n" << endl;
        cout << ">>";
        while (true)
        {
            string texto;
            getline(cin, texto);
            Clear();
            if (compare(texto, "exit"))
            {
                break;
            }
            string par = rec(texto); //este es la plabra clave
            texto.erase(0,par.length()+1);
            vector<string> parametros = recAgrupar_tokens(texto); //las instrucciones acosiadas a la plabara clave  -parametros
            functions(par, parametros);
            cout << "\n >>>>continuar ? (enter)" << endl;
            getline(cin,texto);
            Clear();
            cout << ">>>>>>>>>>>>>>>>> PROYECTO 1 \n" << endl;
            cout << ">>>>>>>>>>>>>>>>> NATALIA MARIEL CALDERON ECHEVERRIA \n" << endl;
            cout << ">>>>>>>>>>>>>>>>>202200007 \n" << endl;
            cout << ">>>>>>>>>>>>>>>>>INGRESE UN COMANDO(exit -> salir) \n" << endl;
            cout << ">>";
        }
}





void scanner::functions(string rec, vector<string> parametros)
{
    if (compare(rec, "MKDISK"))
    {
        //cout << "Comando reconocido: " << endl;
        cout << "Comando reconocido: MKDISK" << endl;

        cout << "comando recibido:"<<endl;

        for (int i=0; i<parametros.size();i++)
         {
            cout << parametros[i] <<endl;

        }

        disco.mkdisk(parametros); 
    }else if(compare(rec, "RMDISK")){
        cout << "Comando reconocido: RMDISK" << endl;
        disco.rmdisk(parametros);

    }else if(compare(rec, "FDISK")){
        cout << "Comando reconocido: FDISK" << endl;
        disco.fdisk(parametros);

    }else if(compare(rec, "MOUNT")){
        cout << "Comando reconocido: MOUNT" << endl;
        mount.mount(parametros);

    }else if(compare(rec, "UNMOUNT")){
        cout << "Comando reconocido: UNMOUNT" << endl;
        mount.unmount(parametros);



    }else if(compare(rec, "MKFS")){
        cout << "Comando reconocido: MKFS" << endl;


    }else if(compare(rec, "LOGIN")){

        cout << "Comando reconocido: LOGIN" << endl;
        cout << "------> iniciando sesion" << endl;


    }else if(compare(rec, "LOGOUT")){

        cout << "Comando reconocido: LOGOUT" << endl;
        cout << "------> cerrando sesion" << endl;


    }else if(compare(rec, "MKGRP")){

        cout << "Comando reconocido:  MKGRP" << endl;


    }else if(compare(rec, "RMGRP")){

        cout << "Comando reconocido: RMGRP" << endl;


    }else if(compare(rec, "MKUSR")){

        cout << "Comando reconocido: MKUSR" << endl;


    }else if(compare(rec, "RMUSR")){

        cout << "Comando reconocido: RMUSR" << endl;


    }else if(compare(rec, "MKDIR")){

        cout << "Comando reconocido: MKDIR" << endl;

    }else if(compare(rec, "REP")){
        cout << "Comando reconocido: REPORTES" << endl;
        //generar los dos reportes principaes <--------------------
        //mbr y disk


    }else if(compare(rec, "EXEC")){
        cout << "Comando reconocido: EXEC" << endl;
        runExecute(parametros);

    }else if(compare(rec.substr(0,1),"#")){
        respuesta(">>>> COMENTARIO RECONOCIDO :) ",rec);

    }else{
        errores("SYSTEM","EL COMANDO NO ESTA ENTRE LOS COMANDOS RECONOCIDOS, VERIFICAR! >>> \""+rec+"\"");
    }
}


//revisa la plabra y mira que no sea un comentario, si es se trata como comentario -> break
string scanner::rec(string text)
{
    string pal = "";
    bool fin = false;
    for (char &c : text){
        if (fin)
        {
            if (c == ' ' || c == '-'){
                break;
            }
            pal += c;
        }
        else if ((c != ' ' && !fin))
        {
            if (c=='#'){
                pal=text;
                break;
            }else{
                pal += c;
                fin = true;
            }
        }
    }
    return pal;
}

vector<string> scanner::recAgrupar(string text, string text_recAgrupar)
{
    vector<string> strr;
    if (text.empty())
    {
        return strr;
    }
    
    int n = text.length();
    char char_array[n + 1];
    strcpy(char_array, text.c_str());
    char* point = strtok(char_array, text_recAgrupar.c_str());
    while (point!=NULL)
    {
        strr.push_back(string(point));
        point = strtok(NULL, text_recAgrupar.c_str());
    }
    return strr;
}

vector<string> scanner::recAgrupar_tokens(string text){
    vector<string> tokens;
    if (text.empty())
    {
        return tokens;
    }
    text.push_back(' ');
    string rec = "";
    int estado = 0;
    for(char &c: text){
        if (estado ==0 && c=='-')
        {
            estado = 1;

        }else if(estado==0 && c=='#'){
            continue;
        }else if(estado!=0){
            if (estado == 1)
            {
                if(c=='='){
                    estado = 2;
                }else if(c == ' '){
                    continue;
                }
            }else if(estado == 2){
                if (c=='\"')
                {
                    estado = 3;
                }else{
                    estado = 4;
                }
                
            }else if(estado == 3){
                if (c=='\"')
                {
                    estado = 4;
                }
            }else if (estado==4 && c=='\"')
            {
                tokens.clear();
                continue;
            }else if (estado ==4 && c==' ')
            {
                estado = 0;
                tokens.push_back(rec);
                rec = "";
                continue;
            }
            rec+=c;
        }
    }
    return tokens;
}

string scanner::upper(string a){
    string up="";
    for(char &a: a){
        up+=toupper(a);
    }
    return up;  
}

bool scanner::compare(string a, string b){
    if (upper(a)==upper(b))
    {
        return true;
    }
    return false;
}

void scanner::errores(string operacion, string mensaje){
    
    cout << "\033[1;41m Error\033"<< "\033[0;31m(" + operacion + ")~~> \033[0m"<< mensaje << endl;
}

void scanner::respuesta(string operacion, string mensaje){
    
    cout << "\033[0;42m<" + operacion + ">---------> \033[0m"<< mensaje << endl;
}

bool scanner::confirmar(string mensaje){
    cout << mensaje << "Confirmar(n), Otra letra para cancelar" << endl;
    string respuesta;
    getline(cin,respuesta);
    if (compare(respuesta, "n"))
    {
        return true;
    }
    return false;
    
}

void scanner::runExecute(vector<string> tokens){
    string path = "";
    for (string rec:tokens)
    {
        string par = rec.substr(0, rec.find("="));
        rec.erase(0,par.length()+1);
        if (compare(par, "path"))
        {
            path = rec;
        }
    }
    if (path.empty())
    {
        errores("EXEC","el path es un dato obligatorio, agregar");
        return;
    }
    excec(path);
}

void scanner::excec(string path){
    string filename(path);
    vector <string> flz;
    string line;
    ifstream input_file(filename);
    if(!input_file.is_open()){
        cerr << "no se pudo ejecutar el comando, debido al archivo" << filename << endl;
        return;
    }
    while(getline(input_file,line)){
       flz.push_back(line);
    }
    for(const auto &i:flz){
        string texto = i;
        string par = rec(texto);
        if(texto!=""){
            if(compare(texto,"PAUSE")){
                string pause;
                respuesta("PAUSE","desea continuar? (enter)");
                getline(cin,pause);
                continue;
            }
            texto.erase(0,par.length()+1);
            vector <string> parametros = recAgrupar_tokens(texto);
            functions(par,parametros);
        }
    }
    input_file.close();
    return;
}