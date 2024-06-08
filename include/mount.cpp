#include "../lib/mount.h"

#include <iostream>
#include <stdlib.h>
#include "string"
#include <locale>

using namespace std;

Mount::Mount(){}

void Mount::mount(vector<string> command){
    
    //SOLO PARA LISTA LOS QUE ESTAN MONTADOS
    if(command.empty()){
        discosMontado();
        return;
    }

    vector<string> required = { "path", "name" }; //SON OBLIGATORIOS
    string path;
    string name;

    for (auto current : command){ 

        string id = shared.lower(current.substr(0, current.find("=")));
        current.erase(0, id.length() + 1);


        //SOLO PARA ELIMINAR LAS COMILLAS 
        if(current.substr(0,1) == "\""){
            current.substr(1,current.length()-2);
        };
//--------------------
//ORDENAR LOS DOS DATOS OBLIGATORIOS

        if(shared.compare(id, "name")){
            if(count(required.begin(), required.end(), id)){
                auto itr = find(required.begin(), required.end(), id);
                required.erase(itr);
                name = current;
            }
        } else if (shared.compare(id, "path")) {
            if (count(required.begin(), required.end(), id)) {
                auto itr = find(required.begin(), required.end(), id);
                required.erase(itr);
                path = current;
            }
        }
    };
    if (required.size() != 0) {
        shared.handler("MOUNT", "EL NOMBRE Y EL PATH SON PARAMETROS OBLIGATORIOS");
        return;
    };
    mount(path, name);
}


//mount abrir (validar) >> estructura/asignar

void Mount::mount(string p, string n){
    try{
        //VALIDAR QUE LO QUE SE QUIERA MONTAR EXISTE - (solo intentar abrir)
        FILE *validate = fopen(p.c_str(), "r");
        if (validate == NULL) {
            shared.handler("MOUNT", "ESTE DISCO NO EXISTE -- REVISAR");
        }

//se manda a llamar 
        Structs::MBR disk;
        rewind(validate);
        fread(&disk, sizeof(Structs::MBR), 1, validate);
        fclose(validate);
        

        //se mira si es una extendida, si esta vadia e monta una logica
        Structs::Partition partition = dsk.findby(disk, n, p);
        if (partition.part_type == 'E') {
            vector<Structs::EBR> ebrs = dsk.getlogics(partition, p);
            if (!ebrs.empty()) {
                Structs::EBR ebr = ebrs.at(0);
                n = ebr.part_name;

                //agregar logica
                shared.handler("", "se montará una partición lógica(esta dentro de una extendida)");
            } else {
                throw runtime_error("No se puede montar una extendida");
            }
        }

//se recorre los montados, si esta es que ya esta
        for (int i = 0; i < 99; i++) {
            if (mounted[i].path == p) {
                for (int j = 0; j < 26; j++) {
                    if (Mount::mounted[i].mpartitions[j].status == '0') {
                        //ESTABLECER LA MANERA EN LA QUE SE PIDE #id=07Disco1
                        mounted[i].mpartitions[j].status = '1';
                        mounted[i].mpartitions[j].letter = simbolos.at(j);
                        strcpy(mounted[i].mpartitions[j].name, n.c_str());
                        string re = to_string(i + 1) + simbolos.at(j);
                        //lista
                        mountedIds.push_back("07" + re);

                        shared.response("MOUNT", "SE MONTO EL DISCO ---> -id=07" + re);
                        return;
                    }
                }
            }
        }

        for (int i = 0; i < 99; i++) {
            if (mounted[i].status == '0') {
                mounted[i].status = '1';
                strcpy(mounted[i].path, p.c_str());
                for (int j = 0; j < 26; j++) {
                    if (Mount::mounted[i].mpartitions[j].status == '0') {
                        mounted[i].mpartitions[j].status = '1';
                        mounted[i].mpartitions[j].letter = simbolos.at(j);
                        strcpy(mounted[i].mpartitions[j].name, n.c_str());
                        string re = to_string(i + 1) + simbolos.at(j);
                        mountedIds.push_back("07" + re);
                        shared.response("MOUNT", "SE MONTO EL DISCO ---> -id=07" + re);
                        return;
                    }
                }
            }
        }

    }catch(exception &e){
        shared.handler("MOUNT", e.what());
        return;
    }

}

void Mount::discosMontado() {
    cout << "\n<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<MONTADOS>>>>>>>>>>>>>>>>>>>>>>>>>>>" << endl;
        cout << "\n<<<< id >>>>  - <<<< nombre >>>>" << endl;
    for (int i = 0; i < 99; i++) {
        for (int j = 0; j < 26; j++) {
            if (mounted[i].mpartitions[j].status == '1') {
                cout << "> 07" << i + 1 << simbolos.at(j) << "disco - " << mounted[i].mpartitions[j].name << endl;
            }
        }
    }
}

void Mount::unmount(vector<string> command) {
    string id;
    for (auto current : command) {
        string key = shared.lower(current.substr(0, current.find("=")));
        current.erase(0, key.length() + 1);

        // SOLO PARA ELIMINAR LAS COMILLAS 
        if (current.substr(0, 1) == "\"") {
            current = current.substr(1, current.length() - 2);
        }

        if (shared.compare(key, "id")) {
            id = current;
        }
    }

    if (id.empty()) {
        shared.handler("UNMOUNT", "EL ID ES UN PARAMETRO OBLIGATORIO");
        return;
    }

    // ver si esta en la lista
    auto it = find(mountedIds.begin(), mountedIds.end(), id);
    if (it != mountedIds.end()) {
        // sisi quitarlo de la lista de los discos montados
        mountedIds.erase(it);

        // quitarlo del array
        for (int i = 0; i < 99; i++) {
            for (int j = 0; j < 26; j++) {
                string re = to_string(i + 1) + simbolos.at(j);
                if ("07" + re == id && mounted[i].mpartitions[j].status == '1') {
                    mounted[i].mpartitions[j].status = '0';
                    shared.response("UNMOUNT", "SE DESMONTO EL DISCO ---> -id=07" + re);
                    return;
                }
            }
        }
    } else {
        shared.handler("UNMOUNT", "ID no encontrado: " + id);
    }
}