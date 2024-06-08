#include "../lib/scanner.h"
#include "../lib/disco.h"
#include "../lib/mount.h"

// #include "../lib/report.h"
// #include "../lib/filesystem.h"
// #include "../lib/users.h"
// #include "../lib/filemanager.h"
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
// Report report;
// Users user;
// Shared shared;
// FileManager filemanager;
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
            string tk = token(texto); //este es la plabra clave
            texto.erase(0,tk.length()+1);
            vector<string> tks = split_tokens(texto); //las instrucciones acosiadas a la plabara clave
            functions(tk, tks);
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





void scanner::functions(string token, vector<string> tks)
{
    if (compare(token, "MKDISK"))
    {
        //cout << "Comando reconocido: " << endl;
        cout << "Comando reconocido: MKDISK" << endl;

        cout << "comando recibido:"<<endl;

        for (int i=0; i<tks.size();i++)
         {
            cout << tks[i] <<endl;

        }

        disco.mkdisk(tks); 
    }else if(compare(token, "RMDISK")){
        cout << "Comando reconocido: RMDISK" << endl;
        disco.rmdisk(tks);

    }else if(compare(token, "FDISK")){
        cout << "Comando reconocido: FDISK" << endl;
        disco.fdisk(tks);
    }else if(compare(token, "MOUNT")){
        cout << "Comando reconocido: MOUNT" << endl;
        mount.mount(tks);
    }else if(compare(token, "UNMOUNT")){
        cout << "Comando reconocido: UNMOUNT" << endl;
        mount.unmount(tks);



    }else if(compare(token, "MKFS")){
        cout << "Comando reconocido: MKFS" << endl;
        //FileSystem fileSystem = FileSystem(mount);
        //fileSystem.mkfs(tks);

    }else if(compare(token, "LOGIN")){

        cout << "Comando reconocido: LOGIN" << endl;
        cout << "------> iniciando sesion" << endl;
        if(logued){
            //shared.handler("LOGIN", " ya existe una sesion abierta");
            return;
        }
        //logued = user.login(tks,mount);

    }else if(compare(token, "LOGOUT")){

        cout << "Comando reconocido: LOGOUT" << endl;
        cout << "------> cerrando sesion" << endl;
        if(!logued){
        //    shared.handler("LOGOUT", " debe de iniciar sesion primero");
            return;
        }
        //logued = user.logout();

    }else if(compare(token, "MKGRP")){
        if(!logued){
        //    shared.handler("MKGRP", " debe de iniciar sesion primero");
            return;
        }
        cout << "Comando reconocido:  MKGRP" << endl;
        //user.grp(tks,"MK");

    }else if(compare(token, "RMGRP")){
        if(!logued){
        //    shared.handler("RMGRP", " debe de iniciar sesion primero");
            return;
        }
        cout << "Comando reconocido: RMGRP" << endl;
        //user.grp(tks,"RM");

    }else if(compare(token, "MKUSR")){
        if(!logued){
        //    shared.handler("MKUSR", " debe de iniciar sesion primero");
            return;
        }
        cout << "Comando reconocido: MKUSR" << endl;
        //user.usr(tks,"MK");

    }else if(compare(token, "RMUSR")){
        if(!logued){
        //    shared.handler("RMUSR", " debe de iniciar sesion primero");
            return;
        }
        cout << "Comando reconocido: RMUSR" << endl;
        //user.usr(tks,"RM");

    }else if(compare(token, "MKDIR")){
        if(!logued){
        //    shared.handler("MKDIR", " debe de iniciar sesion primero");
            return;
        }
        string p;
        cout << "Comando reconocido: MKDIR" << endl;
        //Structs::Partition partition = mount.getmount(user.logged.id, &p);
        //filemanager.mkdir(tks, partition, p);
    }else if(compare(token, "REP")){
        cout << "Comando reconocido: REPORTES" << endl;
        //report.generar(tks, mount);
    }else if(compare(token, "EXEC")){
        cout << "Comando reconocido: EXEC" << endl;
        funcion_excec(tks);
    }else if(compare(token.substr(0,1),"#")){
        respuesta("COMENTARIO RECONOCIDO",token);
    }else{
        errores("SYSTEM","EL COMANDO NO ESTA ENTRE LOS COMANDOS RECONOCIDOS, VERIFICAR >>> \""+token+"\"");
    }
}


//revisa la plabra y mira que no sea un comentario, si es se trata como comentario -> break
string scanner::token(string text)
{
    string tkn = "";
    bool terminar = false;
    for (char &c : text){
        if (terminar)
        {
            if (c == ' ' || c == '-'){
                break;
            }
            tkn += c;
        }
        else if ((c != ' ' && !terminar))
        {
            if (c=='#'){
                tkn=text;
                break;
            }else{
                tkn += c;
                terminar = true;
            }
        }
    }
    return tkn;
}

vector<string> scanner::split(string text, string text_split)
{
    vector<string> cadena;
    if (text.empty())
    {
        return cadena;
    }
    
    int n = text.length();
    char char_array[n + 1];
    strcpy(char_array, text.c_str());
    char* point = strtok(char_array, text_split.c_str());
    while (point!=NULL)
    {
        cadena.push_back(string(point));
        point = strtok(NULL, text_split.c_str());
    }
    return cadena;
}

vector<string> scanner::split_tokens(string text){
    vector<string> tokens;
    if (text.empty())
    {
        return tokens;
    }
    text.push_back(' ');
    string token = "";
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
                tokens.push_back(token);
                token = "";
                continue;
            }
            token+=c;
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
    
    cout << "\033[0;42m(" + operacion + ")------> \033[0m"<< mensaje << endl;
}

bool scanner::confirmar(string mensaje){
    cout << mensaje << "Confirmar(N), Otra letra para cancelar" << endl;
    string respuesta;
    getline(cin,respuesta);
    if (compare(respuesta, "N"))
    {
        return true;
    }
    return false;
    
}

void scanner::funcion_excec(vector<string> tokens){
    string path = "";
    for (string token:tokens)
    {
        string tk = token.substr(0, token.find("="));
        token.erase(0,tk.length()+1);
        if (compare(tk, "path"))
        {
            path = token;
        }
    }
    if (path.empty())
    {
        errores("EXEC","Se requiere path para este comando");
        return;
    }
    excec(path);
}

void scanner::excec(string path){
    string filename(path);
    vector <string> lines;
    string line;
    ifstream input_file(filename);
    if(!input_file.is_open()){
        cerr << "No se puede abrir el archivo" << filename << endl;
        return;
    }
    while(getline(input_file,line)){
        lines.push_back(line);
    }
    for(const auto &i:lines){
        string texto = i;
        string tk = token(texto);
        if(texto!=""){
            if(compare(texto,"PAUSE")){
                string pause;
                respuesta("PAUSE","Enter para continuar...");
                getline(cin,pause);
                continue;
            }
            texto.erase(0,tk.length()+1);
            vector <string> tks = split_tokens(texto);
            functions(tk,tks);
        }
    }
    input_file.close();
    return;
}