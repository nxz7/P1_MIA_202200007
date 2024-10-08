#include "../lib/scanner.h"
#include "../lib/disco.h"
#include "../lib/mount.h"
#include "../lib/repz.h"

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
Report report;
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
        std::cout << "\033[33m>>>>>>>>>>>>>>>>> PROYECTO 1 \n\033[0m" << std::endl;
        std::cout << "\033[33m>>>>>>>>>>>>>>>>> NATALIA MARIEL CALDERON ECHEVERRIA  \n\033[0m" << std::endl;
        std::cout << "\033[33m>>>>>>>>>>>>>>>>>202200007 \n\033[0m" << std::endl;
        std::cout << "\033[33m>>>>>>>>>>>>>>>>>INGRESE UN COMANDO(exit -> salir) \n\033[0m" << std::endl;
        cout << ">>";
        while (true)
        {
            string texto;
            getline(cin, texto);
            
            //------------------------salida
            if (equiv(texto, "exit")){
                break;
            }

            string par = rec(texto); //este es la plabra clave
            texto.erase(0,par.length()+1);
            vector<string> parametros = recAgrupar_tokens(texto); //las instrucciones acosiadas a la plabara clave  -parametros
            functions(par, parametros);
            cout << "\n >>>>continuar ? (enter)" << endl;
            getline(cin,texto);
            
            std::cout << "\033[33m>>>>>>>>>>>>>>>>> PROYECTO 1 \n\033[0m" << std::endl;
            std::cout << "\033[33m>>>>>>>>>>>>>>>>> NATALIA MARIEL CALDERON ECHEVERRIA \n\033[0m" << std::endl;
            std::cout << "\033[33m>>>>>>>>>>>>>>>>>202200007 \n\033[0m" << std::endl;
            std::cout << "\033[33m>>>>>>>>>>>>>>>>>INGRESE UN COMANDO(exit -> salir) \n\033[0m" << std::endl;
            cout << ">>";
        }
}



//----------------------------------------------revisar cada inicial, comando y clasificar

void scanner::functions(string rec, vector<string> parametros)
{
    
    if (equiv(rec, "MKDISK"))
    {
        //cout << "Comando reconocido: " << endl;
        cout << "Comando reconocido: MKDISK" << endl;

        cout << "comando recibido:"<<endl;

        for (int i=0; i<parametros.size();i++)
         {
            cout << parametros[i] <<endl;

        }

        disco.mkdisk(parametros); 
        cout << "**************************************" << endl;
    }else if(equiv(rec, "RMDISK")){
        cout << "Comando reconocido: RMDISK" << endl;
        disco.rmdisk(parametros);
        cout << "**************************************" << endl;

    }else if(equiv(rec, "FDISK")){
        cout << "Comando reconocido: FDISK" << endl;
        disco.fdisk(parametros);
        cout << "**************************************" << endl;

    }else if(equiv(rec, "MOUNT")){
        cout << "Comando reconocido: MOUNT" << endl;
        mount.mount(parametros);
        cout << "**************************************" << endl;

    }else if(equiv(rec, "UNMOUNT")){
        cout << "Comando reconocido: UNMOUNT" << endl;
        mount.unmount(parametros);
        cout << "**************************************" << endl;


    }else if(equiv(rec, "MKFS")){
        cout << "Comando reconocido: MKFS" << endl;
        cout << "**************************************" << endl;

    }else if(equiv(rec, "LOGIN")){

        cout << "Comando reconocido: LOGIN" << endl;
        cout << "------> iniciando sesion" << endl;
        cout << "**************************************" << endl;

    }else if(equiv(rec, "LOGOUT")){

        cout << "Comando reconocido: LOGOUT" << endl;
        cout << "------> cerrando sesion" << endl;
        cout << "**************************************" << endl;

    }else if(equiv(rec, "MKGRP")){

        cout << "Comando reconocido:  MKGRP" << endl;
        cout << "**************************************" << endl;

    }else if(equiv(rec, "RMGRP")){

        cout << "Comando reconocido: RMGRP" << endl;
        cout << "**************************************" << endl;

    }else if(equiv(rec, "MKUSR")){

        cout << "Comando reconocido: MKUSR" << endl;
        cout << "**************************************" << endl;

    }else if(equiv(rec, "RMUSR")){

        cout << "Comando reconocido: RMUSR" << endl;
        cout << "**************************************" << endl;

    }else if(equiv(rec, "MKDIR")){

        cout << "Comando reconocido: MKDIR" << endl;
        cout << "**************************************" << endl;

    }else if(equiv(rec, "REP")){
        cout << "Comando reconocido: REPORTES" << endl;
        //generar los dos reportes principaes <--------------------
        //mbr y disk
        report.repzCrear(parametros, mount);
        cout << "**************************************" << endl;


    }else if(equiv(rec, "EXEC")){
        cout << "Comando reconocido: EXEC" << endl;
        runExcec(parametros);
        cout << "**************************************" << endl;

    }else if(equiv(rec.substr(0,1),"#")){
        respuesta(">>>> COMENTARIO RECONOCIDO <3 ",rec);
        cout << "**************************************" << endl;

    }else if(equiv(rec, "PAUSE")){
            string pause;
            respuesta("PAUSE","programa en pausa, seleccione cualquier letra para continuar");
            getline(cin,pause);
            cout << "**************************************" << endl;

    }
    else{
        errores("SYSTEM","EL COMANDO NO ESTA ENTRE LOS COMANDOS RECONOCIDOS, VERIFICAR! >>> \""+rec+"\"");
    }
}


//revisa la plabra y mira que no sea un comentario, si es se trata como comentario -> break
string scanner::rec(string text){

    string wrdd = "";
    bool fin = false;
    for (char &c : text){

        if (fin){
            if (c == ' ' || c == '-'){
                break;
            }
            wrdd += c;
        }
        else if ((c != ' ' && !fin)){

            if (c=='#'){
                wrdd=text;
                break;
            }else{

                wrdd += c;
                fin = true;
            }
        }
    }
    return wrdd;
}

vector<string> scanner::recAgrupar(string text, string text_recAgrupar){

    vector<string> strr;
    if (text.empty()){
        return strr;
    }
    
    int n = text.length();
    char char_array[n + 1];
    strcpy(char_array, text.c_str());
    char* point = strtok(char_array, text_recAgrupar.c_str());

    while (point!=NULL){

        strr.push_back(string(point));
        point = strtok(NULL, text_recAgrupar.c_str());

    }

    return strr;
}

vector<string> scanner::recAgrupar_tokens(string text){
    vector<string> tokens;

    if (text.empty()){

        return tokens;

    }

    text.push_back(' ');
    string rec = "";
    int estado = 0;
    //automata simple para reconocer: comentarios, parametros, parlabras
    for(char &c: text){

        if (estado ==0 && c=='-'){

            estado = 1;

        }else if(estado==0 && c=='#'){

            continue;

        }else if(estado!=0){

            if (estado == 1){

                if(c=='='){

                    estado = 2;

                }else if(c == ' '){

                    continue;

                }
            }else if(estado == 2){
                if (c=='\"'){

                    estado = 3;

                }else{

                    estado = 4;

                }
                
            }else if(estado == 3){

                if (c=='\"'){

                    estado = 4;
                }
            }else if (estado==4 && c=='\"'){

                tokens.clear();
                continue;

            }else if (estado ==4 && c==' '){

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

bool scanner::equiv(string a, string b){
    if (upper(a)==upper(b)){

        return true;
    }
    return false;
}

void scanner::errores(string operacion, string mensaje){
    
    cout << "\033[1;41m Error!!\033"<< "\033[0;31m(" + operacion + ")~~> \033[0m"<< mensaje << endl;

}

void scanner::respuesta(string operacion, string mensaje){
    //rosado
    cout << "\033[48;2;255;105;180m<" + operacion + ">---------> \033[0m"<< mensaje << endl;
}


bool scanner::confirmar(string mensaje){
    cout << mensaje << "Confirmar, presione s" << endl;
    string respuesta;
    getline(cin,respuesta);
    if (equiv(respuesta, "s")){

        return true;
    }
    return false;
    
}

void scanner::runExcec(vector<string> tokens){
    string path = "";

    for (string token:tokens){

        string tk = token.substr(0, token.find("="));
        token.erase(0,tk.length()+1);
        if (equiv(tk, "path")){

            path = token;
        }
    }

    if (path.empty()){

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
        cerr << "No se puede abrir el archivo---->" << filename << endl;
        return;
    }

    while(getline(input_file,line)){
        lines.push_back(line);
    }
    //loop de ir leyendo comanod por comanod
    for(const auto &i:lines){

        string texto = i;
        string tk = rec(texto);
        if(texto!=""){

            if(equiv(texto,"PAUSE")){

                string pause;
                respuesta("PAUSE","programa en pausa, seleccione cualquier letra para continuar");
                getline(cin,pause);
                continue;

            }

            texto.erase(0,tk.length()+1);
            vector <string> tks = recAgrupar_tokens(texto);
            functions(tk,tks);

        }
    }

    input_file.close();
    return;
}