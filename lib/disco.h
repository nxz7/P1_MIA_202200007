#ifndef DISK_H
#define DISK_H
#include<vector>

#include <string>
#include <algorithm>
#include "../lib/disco.h"
#include "../lib/scanner.h"
#include "../lib/structs.h"
#include "../lib/funz.h"

using namespace std;

class Disk{
    public:
        Disk();
        //se declara el inicio, fin, antes, despues y la particion
        typedef struct _Transition{
            int partition;
            int start;
            int end;
            int before;
            int after;
        } Transition;

//------------------------------------------UNCIONES PARA LOS DISCOS (DEFINICR EN .CPP)

//------------------------------------------MKDISK
        void mkdisk(vector<string> tokens); //SEPARAR LOS DATOS
//------------------------------------------CREAR EL DISCO
        void makeDisk(string s, string f, string u, string p);

//------------------------------------------ELIMINAR
        void rmdisk(vector<string> textzz);

        //funciones - particiones
        void fdisk(vector<string> textzz);
        void generatepartition(string s, string u, string p, string t, string f, string n, string a);
        
        void borrarPart(string d, string p, string n);
        void addpartition(string add, string u, string n, string p);
            
        vector<Structs::Partition> getPartitions(Structs::MBR disk);

        Structs::MBR
        //------------------------------------------recibe la info
        adjust(Structs::MBR mbr, Structs::Partition p, vector<Transition> t, vector<Structs::Partition> ps, int u);

        Structs::Partition findby(Structs::MBR mbr, string name, string path);

//---particion logica --> logicas dentro de las extendidas. Si se elimina extendida se elimina la logica

        void logzz(Structs::Partition partition, Structs::Partition ep, string p);

        vector<Structs::EBR> getlogics(Structs::Partition partition, string p);
        
    private:
        Funz funz;   
    
};


#endif // salir