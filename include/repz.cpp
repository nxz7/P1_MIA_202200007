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
        string id_ = funz.lower(current.substr(0,current.find('=')));
        current.erase(0, id_.length()+1);
        if(current.substr(0,1) =="\"")
        {
            current = current.substr(1,current.length()-2);
        }
        if(funz.equiv(id_,"name")){
            if(count(required.begin(), required.end(), id_)){
                auto itr = find(required.begin(), required.end(), id_);
                required.erase(itr);
                name = current;
            }
        }else if(funz.equiv(id_,"id")){
            if(count(required.begin(), required.end(), id_)){
                auto itr = find(required.begin(), required.end(), id_);
                required.erase(itr);
                id = current;
            }
        }else if(funz.equiv(id_,"path")){
            if(count(required.begin(), required.end(), id_)){
                auto itr = find(required.begin(), required.end(), id_);
                required.erase(itr);
                path = current;
            }
        }
    }
    if(required.size()!=0){
        funz.notif("REPORTES", "No estan los parametros necesarios para realizar un reporte");
        return;
    }
    if (funz.equiv(name, "MBR")) {
        mbr(path, id);
    } else if (funz.equiv(name, "DISK")) {
        repDisk(path, id);

    } else if (funz.equiv(name, "INODE")) {
        funz.msmSalida(">>>REPORTE", "EL REPORTE INODE NO SE ENCUENTRA DESARROLLADO AUN!!!");

    } else if (funz.equiv(name, "BLOCK")) {
        funz.msmSalida(">>>REPORTE", "EL REPORTE BLOCK NO SE ENCUENTRA DESARROLLADO AUN!!!");

    } else if (funz.equiv(name, "BM_INODE")) {
        funz.msmSalida(">>>REPORTE", "EL REPORTE BM INODE NO SE ENCUENTRA DESARROLLADO AUN!!!");

    } else if (funz.equiv(name, "BM_BLOCK")) {
        funz.msmSalida(">>>REPORTE", "EL REPORTE BM BLOCK NO SE ENCUENTRA DESARROLLADO AUN!!!");
        
    } else if (funz.equiv(name, "SB")) {
        funz.msmSalida(">>>REPORTE", "EL REPORTE SB  NO SE ENCUENTRA DESARROLLADO AUN!!!");

    } else if (funz.equiv(name, "TREE")) {

        funz.msmSalida(">>>REPORTE", "EL REPORTE TREE NO SE ENCUENTRA DESARROLLADO AUN!!!");

    }  else if (funz.equiv(name, "Journaling")) {
        
        funz.msmSalida(">>>REPORTE", "EL REPORTE JOURNALING NO SE ENCUENTRA DESARROLLADO AUN!!!");
    } else {

        funz.notif("REPORTES", "El reporte elegido no es reconocido, por favor revisar la lista de reportes disponibles");
        return;
    }
}

void Report::mbr(string p, string id) {
    try {
        string path;
        Structs::Partition partition = mount.getmount(id, &path);

        FILE *file = fopen(path.c_str(), "rb+");
        if (file == NULL) {
            throw runtime_error("El disco al que se hace referencia no existe o no está disponible");
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

        //  fecha formateada
        struct tm *tm;
        tm = localtime(&disco.mbr_fecha_creacion);
        char mostrar_fecha[20];
        strftime(mostrar_fecha, 20, "%Y/%m/%d %H:%M:%S", tm);

        // el DOT inicia
        string content;
        content += "digraph G {\n";
        content += "rankdir=TB;\n";
        content += "graph [dpi=\"100\", bgcolor=\"#fff9e6\", fontname=\"Helvetica\"];\n";
        content += "node [shape=plaintext, color=\"black\", fontname=\"Helvetica\"];\n";
        content += "\n";

        content += "top_label [label=\"TABLA MBR/EBR\", shape=none, fontsize=27, fontcolor=\"#ff69b4\"];\n";
        content += "\n";

        // Tabla MBR
        content += "general [label=<<table border=\"0\" cellborder=\"1\" cellspacing=\"0\" cellpadding=\"8\" bgcolor=\"white\" style=\"rounded\" color=\"#ff69b4\">\n";
        content += "<tr><td COLSPAN=\"2\" BGCOLOR=\"#ffcc00\"><font color=\"black\">MBR</font></td></tr>\n";
        content += "<tr><td BGCOLOR=\"#ff69b4\"><font color=\"white\">Nombre</font></td><td BGCOLOR=\"#ffcc99\"><font color=\"black\">Valor</font></td></tr>\n";
        content += "<tr><td BGCOLOR=\"#ff69b4\"><font color=\"white\">Tamaño del MBR</font></td><td>" + to_string(disco.mbr_tamano) + "</td></tr>\n";
        content += "<tr><td BGCOLOR=\"#ff69b4\"><font color=\"white\">MBR Fecha</font></td><td>" + string(mostrar_fecha) + "</td></tr>\n";
        content += "<tr><td BGCOLOR=\"#ff69b4\"><font color=\"white\">MBR Firma</font></td><td>" + to_string(disco.mbr_disk_signature) + "</td></tr>\n";
        content += "<tr><td BGCOLOR=\"#ff69b4\"><font color=\"white\">Fit Disk</font></td><td>" + string(1, disco.disk_fit) + "</td></tr>\n";
        content += "\n";

        // particiones---> inf, p y e
        for (int i = 0; i < 4; ++i) {
            if (partitions[i].part_status == '1') {
                content += "<tr><td COLSPAN=\"2\" BGCOLOR=\"#ffcc00\"><font color=\"black\">Partición " + to_string(i + 1) + "</font></td></tr>\n";
                content += "<tr><td BGCOLOR=\"#ff69b4\"><font color=\"white\">Estado Partición " + to_string(i + 1) + "</font></td><td>" + string(1, partitions[i].part_status) + "</td></tr>\n";
                content += "<tr><td BGCOLOR=\"#ff69b4\"><font color=\"white\">Tipo de Partición " + to_string(i + 1) + "</font></td><td>" + string(1, partitions[i].part_type) + "</td></tr>\n";
                content += "<tr><td BGCOLOR=\"#ff69b4\"><font color=\"white\">Fit de la Partición " + to_string(i + 1) + "</font></td><td>" + partitions[i].part_fit + "</td></tr>\n";
                content += "<tr><td BGCOLOR=\"#ff69b4\"><font color=\"white\">Inicio Partición " + to_string(i + 1) + "</font></td><td>" + to_string(partitions[i].part_start) + "</td></tr>\n";
                content += "<tr><td BGCOLOR=\"#ff69b4\"><font color=\"white\">Tamaño de la Partición " + to_string(i + 1) + "</font></td><td>" + to_string(partitions[i].part_size) + "</td></tr>\n";
                content += "<tr><td BGCOLOR=\"#ff69b4\"><font color=\"white\">Nombre Partición " + to_string(i + 1) + "</font></td><td>" + partitions[i].part_name + "</td></tr>\n";
                content += "\n";
            }
        }

        // ------------------EBRS
        int count = 0;
        Structs::Partition extended;
        bool ext = false;
        for (int i = 0; i < 4; ++i) {
            if (partitions[i].part_status == '1' && partitions[i].part_type == 'E') {
                ext = true;
                extended = partitions[i];
                break; // EXTENIDAD
            }
        }
        if (ext) {
            //LOG
            vector<Structs::EBR> ebrs = disk.getlogics(extended, path);
            for (Structs::EBR ebr : ebrs) {
                content += "<tr><td BORDER=\"0\"></td><td BORDER=\"0\"></td></tr>\n";
                content += "<tr><td COLSPAN=\"2\" BGCOLOR=\"#ffcc00\"><font color=\"black\">EBR " + to_string(count + 1) + "</font></td></tr>\n";
                content += "<tr><td BGCOLOR=\"#ff69b4\"><font color=\"white\">Nombre</font></td><td BGCOLOR=\"#ffcc99\"><font color=\"black\">Valor</font></td></tr>\n";
                content += "<tr><td BGCOLOR=\"#ff69b4\"><font color=\"white\">Estado de la Partición " + to_string(count + 1) + "</font></td><td>" + string(1, ebr.part_status) + "</td></tr>\n";
                content += "<tr><td BGCOLOR=\"#ff69b4\"><font color=\"white\">Fit de la Partición " + to_string(count + 1) + "</font></td><td>" + ebr.part_fit + "</td></tr>\n";
                content += "<tr><td BGCOLOR=\"#ff69b4\"><font color=\"white\">Inicio Partición " + to_string(count + 1) + "</font></td><td>" + to_string(ebr.part_start) + "</td></tr>\n";
                content += "<tr><td BGCOLOR=\"#ff69b4\"><font color=\"white\">Tamaño Partición " + to_string(count + 1) + "</font></td><td>" + to_string(ebr.part_size) + "</td></tr>\n";
                content += "<tr><td BGCOLOR=\"#ff69b4\"><font color=\"white\">Siguiente Partición " + to_string(count + 1) + "</font></td><td>" + to_string(ebr.part_next) + "</td></tr>\n";
                content += "<tr><td BGCOLOR=\"#ff69b4\"><font color=\"white\">Nombre Partición " + to_string(count + 1) + "</font></td><td>" + ebr.part_name + "</td></tr>\n";
                content += "\n";
                count++;
            }
        }

        content += "</table>>];\n";
        content += "}\n";

        // DOT
        ofstream outfile(pd);
        outfile << content;
        outfile.close();

        // Convertir A imagen
        string function = "dot -Tjpg " + pd + " -o " + p;
        system(function.c_str());

        // SE CREAAA
        funz.msmSalida(">>>REPORTE", "EL REPORTE MBR SE HA CREADO CON ÉXITO !!!");
    }catch (exception &e) {
        // notff errores
        funz.notif(">>>REPORTE", e.what());
    }
}


void Report::repDisk(string p, string id) {
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

//LAS PARTICIONES--------------------------
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

        // ACA INICIA-----------------
        string content;
        content = "digraph G{\n"
                  "rankdir=TB;\n"
                  "forcelabels=true;\n"
                  "graph [ dpi=\"100\", bgcolor=\"lightgrey\", fontname=\"Helvetica\" ];\n"
                  "node [shape=plaintext, color=\"purple\", fontname=\"Helvetica\"];\n"
                  "top_label [label=\"DISK - DIAGRAMA\", shape=none, fontsize=27, fontcolor=\"purple\"];\n";
        content += "nodo1 [label = <<table border=\"0\" cellborder=\"1\" cellspacing=\"0\" cellpadding=\"4\" bgcolor=\"white\" style=\"rounded\" color=\"purple\">\n";
        content += "<tr>\n";

        // Posiciones para las particiones
        int positions[5] = {0, 0, 0, 0, 0};
        int positionsii[5] = {0, 0, 0, 0, 0};

        positions[0] = disk.mbr_Partition_1.part_start - (1 + sizeof(Structs::MBR));
        positions[1] = disk.mbr_Partition_2.part_start - (disk.mbr_Partition_1.part_start + disk.mbr_Partition_1.part_size);
        positions[2] = disk.mbr_Partition_3.part_start - (disk.mbr_Partition_2.part_start + disk.mbr_Partition_2.part_size);
        positions[3] = disk.mbr_Partition_4.part_start - (disk.mbr_Partition_3.part_start + disk.mbr_Partition_3.part_size);
        positions[4] = disk.mbr_tamano + 1 - (disk.mbr_Partition_4.part_start + disk.mbr_Partition_4.part_size);
        copy(begin(positions), end(positions), begin(positionsii));
//-----------------------------------------------------------------
        // ajuste de posiciones negativas
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

        // logcas dentros de las extendidas(logicas)
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
                tmplogic += "<td bgcolor=\"#DA70D6\" border=\"0\">EBR</td>";
                tmplogic += "<td bgcolor=\"#D8BFD8\" border=\"0\">PART-LOGICA<br/>" + to_string(res) + "% del disco</td>\n";
                float resta = (float) aux.part_next - ((float) aux.part_start + (float) aux.part_size);
                resta = resta / disk.mbr_tamano;
                resta = resta * 10000.00F;
                resta = round(resta) / 100.00F;
                if (resta != 0) {
                    tmplogic += "<td bgcolor=\"#E6E6FA\" border=\"0\">PART-LOGICA<br/>" + to_string(resta) + "% libre</td>\n";
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
            tmplogic += "<td bgcolor=\"#DA70D6\" border=\"0\">EBR</td>";
            tmplogic += "<td bgcolor=\"#D8BFD8\" border=\"0\">PART-LOGICA<br/>" + to_string(res) + "%</td>\n";
            float resta = (float) extended.part_size - ((float) aux.part_start + (float) aux.part_size - (float) extended.part_start);
            resta = resta / disk.mbr_tamano;
            resta = resta * 10000.00F;
            resta = round(resta) / 100.00F;
            if (resta != 0) {
                tmplogic += "<td bgcolor=\"#E6E6FA\" border=\"0\">ESPACION LIBRE<br/>" + to_string(resta) + "% </td>\n";
                logic++;
            }
            tmplogic += "</tr>\n\n";
            logic += 2;
        }

        // se ordenan para que se mire bien, las filas!
        for (int i = 0; i < 4; ++i) {
            if (partitions[i].part_type == 'E') {
                content += "<td COLSPAN='" + to_string(logic) + "' bgcolor=\"#DDA0DD\" border=\"0\">PART-EXTENDIDA</td>\n";
            } else {
                if (positions[i] != 0) {
                    float res = (float) positions[i] / (float) disk.mbr_tamano;
                    res = round(res * 100.0F * 100.0F) / 100.0F;
                    content += "<td ROWSPAN='2' bgcolor=\"#E6E6FA\" border=\"0\">ESPACIO LIBRE<br/>" + to_string(res) + "%</td>";
                } else {
                    float res = ((float) partitions[i].part_size) / (float) disk.mbr_tamano;
                    res = round(res * 10000.00F) / 100.00F;
                    content += "<td ROWSPAN='2' bgcolor=\"#E6E6FA\" border=\"0\">PART-PRIMARIA<br/>" + to_string(res) + "%</td>";
                }
            }
        }
        if (positions[4] != 0) {
            float res = (float) positions[4] / (float) disk.mbr_tamano;
            res = round(res * 100.0F * 100.0F) / 100.0F;
            content += "<td ROWSPAN='2' bgcolor=\"#E6E6FA\" border=\"0\">ESPACIO LIBRE<br/>" + to_string(res) + "%</td>";
        }

        content += "</tr>\n\n";
        content += tmplogic;
        content += "</table>>];\n}\n";

        // se guarda en el dot de graphviz
        ofstream outfile(pd);
        outfile << content.c_str() << endl;
        outfile.close();

        // se genera la imagen dese el dot
        string function = "dot -Tjpg " + pd + " -o " + p;
        system(function.c_str());

        // Reporte generado correctamente
        funz.msmSalida("REPORT", "EL REPORTE SE GENERO DE MANERA CORRECTA!!!!");
    } catch (exception &e) {
        // Manejo de excepciones ------< erroes
        funz.notif("REPORT", e.what());
    }
}