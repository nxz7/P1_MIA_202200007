#include "../lib/repz.h"
#include "../lib/disco.h"
#include <iostream>
#include <stdlib.h>
#include "string"
#include <locale>

using namespace std;

Report::Report(){}

void Report::repzCrear(vector<string> textzz, Mount m)
{
    mount = m;
    vector<string> required = {"id","path","name"};
    string name;
    string path;
    string id;
    for(string current:textzz){
        string id_ = shared.lower(current.substr(0,current.find('=')));
        current.erase(0, id_.length()+1);
        if(current.substr(0,1) =="\"")
        {
            current = current.substr(1,current.length()-2);
        }
        if(shared.equiv(id_,"name")){
            if(count(required.begin(), required.end(), id_)){
                auto itr = find(required.begin(), required.end(), id_);
                required.erase(itr);
                name = current;
            }
        }else if(shared.equiv(id_,"id")){
            if(count(required.begin(), required.end(), id_)){
                auto itr = find(required.begin(), required.end(), id_);
                required.erase(itr);
                id = current;
            }
        }else if(shared.equiv(id_,"path")){
            if(count(required.begin(), required.end(), id_)){
                auto itr = find(required.begin(), required.end(), id_);
                required.erase(itr);
                path = current;
            }
        }
    }
    if(required.size()!=0){
        shared.notif("REPORTES", "No estan los parametros necesarios para realizar un reporte");
        return;
    }
    if (shared.equiv(name, "MBR")) {
        mbr(path, id);
    } else if (shared.equiv(name, "DISK")) {
        repDisk(path, id);

    } else if (shared.equiv(name, "INODE")) {
        shared.msmSalida(">>>REPORTE", "EL REPORTE INODE NO SE ENCUENTRA DESARROLLADO AUN!!!");

    } else if (shared.equiv(name, "BLOCK")) {
        shared.msmSalida(">>>REPORTE", "EL REPORTE BLOCK NO SE ENCUENTRA DESARROLLADO AUN!!!");

    } else if (shared.equiv(name, "BM_INODE")) {
        shared.msmSalida(">>>REPORTE", "EL REPORTE BM INODE NO SE ENCUENTRA DESARROLLADO AUN!!!");

    } else if (shared.equiv(name, "BM_BLOCK")) {
        shared.msmSalida(">>>REPORTE", "EL REPORTE BM BLOCK NO SE ENCUENTRA DESARROLLADO AUN!!!");
        
    } else if (shared.equiv(name, "SB")) {
        shared.msmSalida(">>>REPORTE", "EL REPORTE SB  NO SE ENCUENTRA DESARROLLADO AUN!!!");

    } else if (shared.equiv(name, "TREE")) {

        shared.msmSalida(">>>REPORTE", "EL REPORTE TREE NO SE ENCUENTRA DESARROLLADO AUN!!!");

    }  else if (shared.equiv(name, "Journaling")) {
        
        shared.msmSalida(">>>REPORTE", "EL REPORTE JOURNALING NO SE ENCUENTRA DESARROLLADO AUN!!!");
    } else {

        shared.notif("REPORTES", "El reporte elegido no es reconocido, por favor revisar la lista de reportes disponibles");
        return;
    }
}


void Report::mbr(string p, string id) {
    try {
        string path;
        Structs::Partition partition = mount.getmount(id, &path);

        FILE *file = fopen(path.c_str(), "rb+");
        if (file == NULL) {
            throw runtime_error("el disco al que se hace referencia no existe o no esta disponible");
        }

        Structs::MBR disco;
        rewind(file);
        fread(&disco, sizeof(Structs::MBR), 1, file);
        fclose(file);

        string pd = p.substr(0, p.find('.'));
        pd += ".dot";
        FILE *doc = fopen(pd.c_str(), "r");
        if (doc == NULL) {
            string cmm = "mkdir -p \"" + pd + "\"";
            string cmm2 = "rmdir \"" + pd + "\"";
            system(cmm.c_str());
            system(cmm2.c_str());
        } else {
            fclose(doc);
        }

        Structs::Partition partitions[4];
        partitions[0] = disco.mbr_Partition_1;
        partitions[1] = disco.mbr_Partition_2;
        partitions[2] = disco.mbr_Partition_3;
        partitions[3] = disco.mbr_Partition_4;
        struct tm *tm;
        tm = localtime(&disco.mbr_fecha_creacion);
        char mostrar_fecha [20];
        strftime(mostrar_fecha, 20, "%Y/%m/%d %H:%M:%S", tm);
        string content;
        content = "digraph G{\n"
                  "rankdir=TB;\n"
                  "graph [ dpi = \"100\" ]; \n"
                  "forcelabels= true;\n"
                  "node [shape = plaintext, color=black];\n"
                  "top_label [label=\"TABLA BMR/EBR\", shape=none, fontsize=\"27\", fontcolor=purple];\n"
                  "bgcolor=lightyellow;\n"
                  "general [label = <<table>\n"
                  "<tr><td COLSPAN = '2' BGCOLOR=\"yellow\"><font color=\"black\">MBR</font></td></tr>\n"
                  "<tr><td BGCOLOR=\"pink\">Nombre</td><td BGCOLOR=\"green\" >Valor</td></tr>\n"
                  "<tr>\n"
                  "<td BGCOLOR=\"pink\" >tamaño del mbr</td>\n"
                  "<td>" +
                  to_string(disco.mbr_tamano) + "</td>\n"
                                               "</tr>\n"
                                               "<tr>\n"
                                               "<td BGCOLOR=\"pink\">MBR FECHA :</td>\n"
                                               "<td>" +
                  string(mostrar_fecha) + "</td>\n"
                                                    "</tr>\n"
                                                    "<tr>\n"
                                                    "<td BGCOLOR=\"pink\" >MBR FIRMA :</td>\n"
                                                    "<td>" +
                  to_string(disco.mbr_disk_signature) + "</td>\n"
                                                       "</tr>\n"
                                                       "<tr>\n"
                                                       "<td BGCOLOR=\"pink\">FIT DISK</td>\n"
                                                       "<td>" +
                  string(1, disco.disk_fit) + "</td>\n"
                                             "</tr>\n";

        Structs::Partition extended;
        bool ext = false;
        for (int i = 0; i < 4; ++i) {
            if (partitions[i].part_status == '1') {
                if (partitions[i].part_type == 'E') {
                    ext = true;
                    extended = partitions[i];
                }
                content += "<tr>\n"
                           "<td BGCOLOR=\"pink\"> estado particion" + to_string(i + 1) + "</td>\n"
                                                                   "<td>" +
                           partitions[i].part_status + " </td >\n"
                                                       "</tr>\n"
                                                       "<tr>\n"
                                                       "<td BGCOLOR=\"pink\">tipo de particion" + to_string(i + 1) + "</td>\n"
                                                                                             "<td>" +
                           partitions[i].part_type + "</td>\n"
                                                     "</tr>\n"
                                                     "<tr>\n"
                                                     "<td BGCOLOR=\"pink\">fit de la paticion" + to_string(i + 1) + "</td>\n"
                                                                                          "<td>" +
                           partitions[i].part_fit + "</td>\n"
                                                    "</tr>\n"
                                                    "<tr>\n"
                                                    "<td BGCOLOR=\"pink\">inicio particion" + to_string(i + 1) + "</td>\n"
                                                                                           "<td>" +
                           to_string(partitions[i].part_start) + "</td>\n"
                                                                 "</tr>\n"
                                                                 "<tr>\n"
                                                                 "<td BGCOLOR=\"pink\" >tmano de la particion" + to_string(i + 1) + "</td>\n"
                                                                                                       "<td>" +
                           to_string(partitions[i].part_size) + "</td>\n"
                                                                "</tr>\n"
                                                                "<tr>\n"
                                                                "<td BGCOLOR=\"pink\">nombre part" + to_string(i + 1) + "</td>\n"
                                                                                                      "<td>" +
                           partitions[i].part_name + "</td>\n"
                                                     "</tr>\n";
            }
        }
        int count = 0;
        if (ext) {
            vector<Structs::EBR> ebrs = disk.getlogics(extended, path);
            for (Structs::EBR ebr : ebrs) {
                content += "<tr><td BORDER=\"0\"></td><td BORDER=\"0\"></td></tr>"
                           "<tr><td COLSPAN = '2' BGCOLOR=\"yellow\"><font color=\"black\">EBR " +
                           to_string(count + 1) +
                           "</font></td></tr>\n"
                           "<tr><td BGCOLOR=\"pink\">Nombre</td><td BGCOLOR=\"green\" >Valor</td></tr>\n"
                           "<tr>\n"
                           "<td BGCOLOR=\"pink\" >estado de la particion" + to_string(count + 1) + "</td>\n"
                                                                       "<td>" +
                           ebr.part_status + "</td>\n"
                                             "</tr>\n"
                                             "<tr>\n"
                                             "<td BGCOLOR=\"pink\" >fit de la paticion" + to_string(count + 1) + "</td>\n"
                                                                                      "<td>" +
                           ebr.part_fit + "</td>\n"
                                          "</tr>\n"
                                          "<tr>\n"
                                          "<td BGCOLOR=\"pink\" >inicio particion" + to_string(count + 1) + "</td>\n"
                                                                                     "<td>" +
                           to_string(ebr.part_start) + "</td>\n"
                                                       "</tr>\n"
                                                       "<tr>\n"
                                                       "<td BGCOLOR=\"pink\" >tamaño particion" + to_string(count + 1) + "</td>\n"
                                                                                                 "<td>" +
                           to_string(ebr.part_size) + "</td>\n"
                                                      "</tr>\n"
                                                      "<tr>\n"
                                                      "<td BGCOLOR=\"pink\" >siguiente part" + to_string(count + 1) + "</td>\n"
                                                                                               "<td>" +
                           to_string(ebr.part_next) + "</td>\n"
                                                      "</tr>\n"
                                                      "<tr>\n"
                                                      "<td BGCOLOR=\"pink\" >nombre part" + to_string(count + 1) + "</td>\n"
                                                                                               "<td>" +
                           ebr.part_name + "</td>\n"
                                           "</tr>\n";
                count++;
            }

        }
        content += "</table>>];";
        content += "\n\n}\n";
        ofstream outfile(pd);
        outfile << content.c_str() << endl;
        outfile.close();
        string function = "dot -Tjpg " + pd + " -o " + p;
        system(function.c_str());
        //function = "rm \"" + pd + "\"";
        //system(function.c_str());
        shared.msmSalida(">>>REPORTE", "EL REPORTE MBR SE HA CREADO CON EXITO !!!");
    } catch (exception &e) {
        shared.notif(">>>REPORTE", e.what());
    }
}

void Report::repDisk(string p, string id){
    try {
        string path;
        Structs::Partition partition = mount.getmount(id, &path);

        FILE *file = fopen(path.c_str(), "rb+");
        if (file == NULL) {
            throw runtime_error("EL DISCO AL QUE HACE REFERENCIA NO EXISTE O NO ESTA DISPONIBLE");
        }

        Structs::MBR disk;
        rewind(file);
        fread(&disk, sizeof(Structs::MBR), 1, file);
        fclose(file);

        string pd = p.substr(0, p.find('.'));
        pd += ".dot";
        FILE *doc = fopen(pd.c_str(), "r");
        if (doc == NULL) {
            string cmm = "mkdir -p \"" + pd + "\"";
            string cmm2 = "rmdir \"" + pd + "\"";
            system(cmm.c_str());
            system(cmm2.c_str());
        } else {
            fclose(doc);
        }

        Structs::Partition partitions[4];
        partitions[0] = disk.mbr_Partition_1;
        partitions[1] = disk.mbr_Partition_2;
        partitions[2] = disk.mbr_Partition_3;
        partitions[3] = disk.mbr_Partition_4;
        Structs::Partition extended;
        bool ext = false;
        for (int i = 0; i < 4; ++i) {
            if (partitions[i].part_status == '1') {
                if (partitions[i].part_type == 'E') {
                    ext = true;
                    extended = partitions[i];
                }
            }
        }

        string content;
        content = "digraph G{\n"
                  "rankdir=TB;\n"
                  "forcelabels= true;\n"
                  "graph [ dpi = \"100\" ]; \n"
                  "node [shape = plaintext, color=\"purple\"];\n"
                    "bgcolor=lightgrey;\n"
                  "top_label [label=\"DISK - DIAGRAMA\", shape=none, fontsize=27, fontcolor=\"purple\"];\n";
        content += "nodo1 [label = <<table>\n";
        content += "<tr>\n";

        int positions[5] = {0, 0, 0, 0, 0};
        int positionsii[5] = {0, 0, 0, 0, 0};
        positions[0] = disk.mbr_Partition_1.part_start - (1 + sizeof(Structs::MBR));
        positions[1] =
                disk.mbr_Partition_2.part_start - (disk.mbr_Partition_1.part_start + disk.mbr_Partition_1.part_size);
        positions[2] =
                disk.mbr_Partition_3.part_start - (disk.mbr_Partition_2.part_start + disk.mbr_Partition_2.part_size);
        positions[3] =
                disk.mbr_Partition_4.part_start - (disk.mbr_Partition_3.part_start + disk.mbr_Partition_3.part_size);
        positions[4] = disk.mbr_tamano + 1 - (disk.mbr_Partition_4.part_start + disk.mbr_Partition_4.part_size);
        copy(positions, positionsii, positionsii);
        for (size_t j = 0; j < 5; j++) {
            bool negative = false;
            for (size_t i = 0; i < 4; i++) {
                if (positions[i] < 0) {
                    negative = true;
                }
                if (positions[i] <= 0 && positionsii[i] <= 0 && negative && positions[i + 1] > 0) {
                    positions[i] = positions[i] + positions[i + 1];
                    positions[i + 1] = 0;
                }
            }
            negative = false;
        }
        int logic = 0;
        string tmplogic;
        if (ext) {
            tmplogic = "<tr>\n";
            Structs::EBR aux;
            FILE *ext = fopen(path.c_str(), "r+b");
            fseek(ext, extended.part_start, SEEK_SET);
            fread(&aux, sizeof(Structs::EBR), 1, ext);
            fclose(ext);
            while (aux.part_next != -1) {
                float res = (float) aux.part_size / (float) disk.mbr_tamano;
                res = round(res * 10000.00F) / 100.00F;
                tmplogic += "<td>EBR</td>";
                tmplogic += "<td>PART-LOGICA\n" + to_string(res) + "% del disco</td>\n";
                float resta = (float) aux.part_next - ((float) aux.part_start + (float) aux.part_size);
                resta = resta / disk.mbr_tamano;
                resta = resta * 10000.00F;
                resta = round(resta) / 100.00F;
                if (resta != 0) {
                    tmplogic += "<td>PART-LOGICA\n" + to_string(resta) + "% libre</td>\n";
                    logic++;
                }
                logic += 2;
                FILE *ext2 = fopen(path.c_str(), "r+b");
                fseek(ext2, aux.part_next, SEEK_SET);
                fread(&aux, sizeof(Structs::EBR), 1, ext2);
                fclose(ext2);
            }
            float res = (float) aux.part_size / (float) disk.mbr_tamano;
            res = round(res * 10000.00F) / 100.00F;
            tmplogic += "<td>EBR</td>";
            tmplogic += "<td>PART-LOGICA\n" + to_string(res) + "%</td>\n";
            float resta = (float) extended.part_size -
                          ((float) aux.part_start + (float) aux.part_size - (float) extended.part_start);
            resta = resta / disk.mbr_tamano;
            resta = resta * 10000.00F;
            resta = round(resta) / 100.00F;
            if (resta != 0) {
                tmplogic += "<td>ESPACION LIBRE\n" + to_string(resta) + "% </td>\n";
                logic++;
            }
            tmplogic += "</tr>\n\n";
            logic += 2;
        }

        for (int i = 0; i < 4; ++i) {
            if (partitions[i].part_type == 'E') {
                content += "<td COLSPAN='" + to_string(logic) + "'> PART-EXTENDIDA </td>\n";
            } else {
                if (positions[i] != 0) {
                    float res = (float) positions[i] / (float) disk.mbr_tamano;
                    res = round(res * 100.0F * 100.0F) / 100.0F;
                    content += "<td ROWSPAN='2'> ESPACIO LIBRE \n" + to_string(res) + "% </td>";
                } else {
                    float res = ((float) partitions[i].part_size) / (float) disk.mbr_tamano;
                    res = round(res * 10000.00F) / 100.00F;
                    content += "<td ROWSPAN='2'> PART-PRIMARIA \n" + to_string(res) + "% </td>";
                }
            }

        }
        if (positions[4] != 0) {
            float res = (float) positions[4] / (float) disk.mbr_tamano;
            res = round(res * 100.0F * 100.0F) / 100.0F;
            content += "<td ROWSPAN='2'> ESPACIO LIBRE \n" + to_string(res) + "%</td>";
        }

        content += "</tr>\n\n";
        content += tmplogic;
        content += "</table>>];\n}\n";
        ofstream outfile(pd);
        outfile << content.c_str() << endl;
        outfile.close();
        string function = "dot -Tjpg " + pd + " -o " + p;
        system(function.c_str());
        //function = "rm \"" + pd + "\"";
        //system(function.c_str());
        shared.msmSalida("REPORT", "EL REPORTE SE GENERO DE MANERA CORRECTA!!!!");
    } catch (exception &e) {
        shared.notif("REPORT", e.what());
    }
}
