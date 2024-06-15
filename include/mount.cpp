#include "../lib/mount.h"

#include <iostream>
#include <stdlib.h>
#include "string"
#include <locale>

using namespace std;

Mount::Mount(){}

//se monta la particion, tiene que llevar nombre y path
//---> mount 1 (ver requisitos)--> mount (montar si si trae)2 
//-------------------------------------------montar
void Mount::mount(vector<string> textzz) {
    if (textzz.empty()) {
        lstMontados();
        return;
        
    }
    vector<string> required = {"name", "path"};
    string path;
    string name;

//name y path son pbligatorios
    for (auto current : textzz) {
        string id = funz.lower(current.substr(0, current.find('=')));
        current.erase(0, id.length() + 1);
        if (current.substr(0, 1) == "\"") {

            current = current.substr(1, current.length() - 2);

        }

        if (funz.equiv(id, "name")) {

            if (count(required.begin(), required.end(), id)) {

                auto itr = find(required.begin(), required.end(), id);
                required.erase(itr);
                name = current;

            }
        } else if (funz.equiv(id, "path")) {

            if (count(required.begin(), required.end(), id)) {

                auto itr = find(required.begin(), required.end(), id);
                required.erase(itr);
                path = current;

            }
        }
    }
    if (required.size() != 0) {

        funz.notif("MOUNT", "no se encuentran todos los parametros obligatorio para realizar el mount");
        return;
    }
    //si si tiene los parametros ya se intenta montar
    mount2(path, name);
}

//---------------------> ya se reviso que tenga los 2 parametros
void Mount::mount2(string p, string n) {
    //path y name, en un try catch para que no se caiga
    try {

        FILE *validate = fopen(p.c_str(), "r");
        if (validate == NULL) {
            funz.notif("MOUNT", "el id no existe o no es valido, revisar!");
            throw runtime_error("el id no existe o no es valido, revisar!");
        }

        Structs::MBR disk;
        rewind(validate);
        fread(&disk, sizeof(Structs::MBR), 1, validate);
        fclose(validate);

        Structs::Partition partition = dsk.findby(disk, n, p);
        if (partition.part_type == 'E') {

            vector<Structs::EBR> ebrs = dsk.getlogics(partition, p);
            if (!ebrs.empty()) {

                Structs::EBR ebr = ebrs.at(0);
                n = ebr.part_name;
                //se mira lo de la part logca --> ya agregada
            } else {
                funz.notif("MOUNT", "es imposible realizar un mount sobre una particion extendida, revisar!!");
                throw runtime_error("es imposible realizar un mount sobre una particion extendida, revisar!!");
            }
        }

        for (int i = 0; i < 99; i++) {

            if (mounted[i].path == p) {

                for (int j = 0; j < 26; j++) {

                    if (Mount::mounted[i].mpartitions[j].status == '0') {
                        mounted[i].mpartitions[j].status = '1';
                        mounted[i].mpartitions[j].letter = letraMount.at(j);
                        strcpy(mounted[i].mpartitions[j].name, n.c_str());
                        string re = to_string(i + 1) + letraMount.at(j);
                        funz.msmSalida("MOUNT", ">> se ha realizado el mount !!! >> -id=07" + re);
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
                        mounted[i].mpartitions[j].letter = letraMount.at(j);
                        strcpy(mounted[i].mpartitions[j].name, n.c_str());
                        string re = to_string(i + 1) + letraMount.at(j);
                        funz.msmSalida("MOUNT", ">> se ha realizado el mount !!! >> -id=07" + re);
                        return;

                    }
                }
            }
        }
    }
    catch (exception &e) {
        funz.notif("MOUNT", e.what());
        return;
    }
}

//para desmontar se necesita lo solo el id, revisa si lo trae
//UNMOUNT1(REVISAR PARAMETROS)--> UNMOUNT2(DESMONTA EN SERIO)
void Mount::unmount(vector<string> textzz) {
    vector<string> required = {"id"};
    string idDesMontar;

    for (int i = 0; i < textzz.size(); i++) {

        string current = textzz.at(i);
        string id = current.substr(0, current.find("="));
        current.erase(0, id.length() + 1);

        if (funz.equiv(id, "id")) {

            auto itr = find(required.begin(), required.end(), id);
            required.erase(itr);
            idDesMontar = current;

        }
    }
    if (required.size() != 0) {

        funz.notif("UNMOUNT", "no se encuentran todos los parametros obligatorio para realizar el unmount");
        return;

    }
    unmount2(idDesMontar);
}
//----------------------------------listas
void Mount::lstMontados() {

    cout << "\n<<<<<<<<<<<<<<<<<<<<<<LISTA DE MONTADOS >>>>>>>>>>>>>>>>>>>>>>"
         << endl;
    for (int i = 0; i < 99; i++) {

        for (int j = 0; j < 26; j++) {

            if (mounted[i].mpartitions[j].status == '1') {

                cout << "> 07" << i + 1 << letraMount.at(j) << "----- " << mounted[i].mpartitions[j].name << endl;

            }
        }
    }
}



//----------------------------------desmontar
void Mount::unmount2(string id) {
    try {
        //fijo debe empezar por el 07, si no de una no existe
        if (!(id[0] == '0' && id[1] == '7')) {
            funz.notif("MOUNT", "El id no es correcto, el error se encuentra en los 2 primeros numeros!!");
            throw runtime_error("El id no es correcto, el error se encuentra en los 2 primeros numeros");
        }
        string past = id;
        char letter = id[id.length() - 1];
        id.erase(0, 2);
        id.pop_back();
        int i = stoi(id) - 1;
        if (i < 0) {
            funz.notif("MOUNT", "el id no existe o no es valido, revisar!");
            throw runtime_error("el id no existe o no es valido, revisar!");
        }

        for (int j = 0; j < 26; j++) {
            if (mounted[i].mpartitions[j].status == '1') {
                if (mounted[i].mpartitions[j].letter == letter) {

                    MountedPartition mp = MountedPartition();
                    mounted[i].mpartitions[j] = mp;
                    funz.msmSalida("UNMOUNT", ">> se ha realizado el unmount !!! >> -id=" + past);
                    return;

                }
            }
        }

        funz.notif("MOUNT", "el id indicado no existe o no esta disponible, no se pudo desmontar --> unable to unmount");
        throw runtime_error("el id indicado no existe o no esta disponible, no se pudo desmontar --> unable to unmount");

    }
    catch (invalid_argument &e) {
        //revisa si el id es valido, entero
        funz.notif("UNMOUNT", "el id del disco no es correcto, no se encuentra o no es valido, revisar!");
        return;

    }
    catch (exception &e) {

        funz.notif("UNMOUNT", e.what());
        return;

    }
}

Structs::Partition Mount::getmount(string id, string *p) {

    if (!(id[0] == '0' && id[1] == '7')) {

        funz.notif("MOUNT", "El id no es correcto, el error se encuentra en los 2 primeros numeros");
        throw runtime_error("El id no es correcto, el error se encuentra en los 2 primeros numeros");


    }

    string past = id;
    char letter = id[id.length() - 1];
    id.erase(0, 2);
    id.pop_back();
    int i = stoi(id) - 1;

    if (i < 0) {

        funz.notif("MOUNT", "el id no existe o no es valido, revisar!");
        throw runtime_error("el id no existe o no es valido, revisar!");
        

    }

    for (int j = 0; j < 26; j++) {
        if (mounted[i].mpartitions[j].status == '1') {
            if (mounted[i].mpartitions[j].letter == letter) {

                FILE *validate = fopen(mounted[i].path, "r");

                if (validate == NULL) {

                    //en caso de que no lo pueda abrir
                    funz.notif("MOUNT", "el disco al que hace referencia no existe o no esta disponible, revisar!");
                    throw runtime_error("el disco al que hace referencia no existe o no esta disponible, revisar!");

                }

                Structs::MBR disk;
                rewind(validate);
                fread(&disk, sizeof(Structs::MBR), 1, validate);
                fclose(validate);
                *p = mounted[i].path;
                return dsk.findby(disk, mounted[i].mpartitions[j].name, mounted[i].path);
            }
        }
    }

    funz.notif("MOUNT", "la particion a la que hace referencia no existe o no esta disponible, revisar!");
    throw runtime_error("la particion a la que hace referencia no existe o no esta disponible, revisar!");
}
