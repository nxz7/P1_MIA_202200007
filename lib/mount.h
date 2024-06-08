#ifndef MOUNT_H
#define MOUNT_H

#include <string>
#include <bits/stdc++.h>
#include "../lib/shared.h"
#include "../lib/structs.h"
#include "../lib/disco.h"

using namespace std;

class Mount
{
    public:
    Mount();
    typedef struct _MP
    {
        char letter;
        char status = '0';
        char name[16]; //como se definio antes < 16

    }MountedPartition; //l aparticion

    typedef struct _MD{
        char path[150];
        char status = '0';
        MountedPartition mpartitions[26];
    }MountedDisk; //el disco MONTADO

    MountedDisk mounted[99];
//MONTAR Y DESMONTAR

    void mount(vector<string> command);
    void unmount(vector<string> command); //este es el que se esta usando, pasa el commando id
    void mount(string path, string name);
    void unmount(string id);  //RECIBE SOLO EL ID


    void discosMontado(); //LISTA DE LOS MONTADOS

    Structs::Partition getMount(string id, string *p);

    private:
    Disk dsk;
    Shared shared;
    vector<char> simbolos = {
            'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p',
            'q','r','s','t','u','v','w','x','y','z',
            '0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};
    vector<string> mountedIds;

};



#endif