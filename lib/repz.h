#ifndef REPORT_H
#define REPORT_H

#include <string>
#include <bits/stdc++.h>
#include "../lib/funz.h"
#include "../lib/structs.h"
#include "../lib/disco.h"

#include "../lib/mount.h"

using namespace std;

class Report {
    public:
    Report();
//------------------------------------------ REVISAR QUE TIPO DE REPORTE SE VA A HACER
    void repzCrear(vector<string> textzz, Mount m);
//------------------------------------------ REPORTE MBR Y EBR (TABLA)
    void mbr(string p, string id);
//------------------------------------------REPORTE DEL DISCO (DIBUJO DE PARITCION/ESPACIOS)
    void repDisk(string p, string id);

    private:
    Funz funz;
    Disk disk;
    Mount mount;

};
#endif