#ifndef MOUNT_H
#define MOUNT_H

#include <string>
#include <bits/stdc++.h>
#include "../lib/shared.h"
#include "../lib/structs.h"
#include "../lib/disco.h"

using namespace std;

class Mount {
    public:
    Mount();
//-----------------partes fijas
    typedef struct _MP
    {
        char letter;
        char status = '0';
        char name[20];
    }MountedPartition;

    typedef struct _MD
    {
        char path[150];
        char status = '0';
        MountedPartition mpartitions[26];
    }MountedDisc;

    MountedDisc mounted[99];
//------------------------------------montar 1 y 2
    void mount(vector<string> textzz);

    void mount(string p, string n);
//------------------------------------desmontar 1 y 2

    void unmount(vector<string> textzz);

    void unmount(string id);

//-----------------------------------lista para ver las partdiscos montados

    void lstMontados();
//------------------------------ buscar y devolver --> uso REP
    Structs::Partition getmount(string id, string *p);

    private:
    Disk dsk;
    Shared shared;
    //LETRA PARA LOS ID, EMPIAZA CON A 
    vector<char> letraMount = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r',
                             's', 't', 'u', 'v', 'w', 'x', 'y', 'z', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};
};
#endif