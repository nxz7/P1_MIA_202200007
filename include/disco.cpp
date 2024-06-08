#include "../lib/disco.h"
#include "../lib/structs.h"
#include "../lib/scanner.h"

#include <vector>
#include <iostream>
#include <cstdio> 
#include <stdlib.h>
#include <string>
#include <algorithm>

using namespace std;

scanner scan;
int startValue;

//ACA YA DEFINO EL DISCO
Disk::Disk(){
}

//RECIBE LOS PARAMETROS, se usa en scanner cuando reconoce
void Disk::mkdisk(vector<string> tokens){
    string size = "";
    string u = "";
    string path = "";
    string f = "";
    bool error = false;
    for(string rec:tokens){
        string par = rec.substr(0, rec.find("=")); // -f=b
        rec.erase(0,par.length()+1); // b
        if(scan.compare(par, "f")){
            if(f.empty()){
                //F ES OPCIONAL -> BEST, WORST, FIRST --> SI NO SE TOMA EL 1
                f = rec; // f = b
            }else{
                scan.errores("MKDISK", " parametro f, ya eingresado ->"+par);
            }
        }else if(scan.compare(par, "s")){
            if (size.empty())
            {
                // parametro size
                size = rec;
            }else{
                scan.errores("MKDISK","tamaño ya ingresado ->"+par);
            }
        }else if (scan.compare(par, "u"))
        {
            if (u.empty())
            {
                //OPCIONAL -> SI NO MEGA BYTES
                u = rec;
            }else{
                scan.errores("MKDISK","U, ya esta ingresado -> "+par);
            }
        }else if (scan.compare(par, "path"))
        {
            if (path.empty())
            {
                path = rec;
            }else{
                scan.errores("MKDISK","PATH, revisar el comando"+par);
            }    
        }else{
            scan.errores("MKDISK","parametro no necesario o esperado -->  "+par);
            error = true;
            break;
        }
    }
    // SI HAY UN TIPO ERROR QUE REGRESE DE UNA
    if (error){
        return;
    }

    if (f.empty())
    {
        f = "BF";
    }
    if (u.empty())
    {
        u = "M";
        //unidad en el caso de que no hay, MEGA
    }
//COMPROBASION DEL COMANDO, los obligatorios y los opcionales
    if (path.empty() && size.empty())
    {
        scan.errores("MKDISK", "OBLIGATORIO: Path y Size ");
    }else if(path.empty()){
        scan.errores("MKDISK","OBLIGATORIO: path -> agregar");
    }else if (size.empty())
    {
        scan.errores("MKDISK","OBLIGATORIO: size -> agregar");
    }else if (!scan.compare(f,"BF") && !scan.compare(f,"FF") && !scan.compare(f,"WF"))
    {
        scan.errores("MKDISK","valores F, no valido(valido: BF, FF, WF)");
    }else if (!scan.compare(u,"k") && !scan.compare(u,"m"))
    {
        scan.errores("MKDISK","valores U, no valido(valido: k y m) ");
    }else{
        //LLAMA LA FUNCION
        makeDisk(size,f,u,path);
    }  
}


// Crear funcion makeDisk --< structs y entra el mbr
void Disk::makeDisk(string s, string f, string u, string path){\
    Structs::MBR disco; 
// try catch para que no se caiga por un error --< saque el error

    try{
        //comprobaciones logicas y asignacion: tamaño positivo, M o K

        int size = stoi(s); // convertir e cadena a int
        if (size <=0){
            scan.errores("MKDISK","EL TAMANO NO PUEDE SER 0, debe ser mayor");
        }
        if(scan.compare(u,"M")){
            size = size * 1024 * 1024;
        }
        if(scan.compare(u,"K")){
            size = size * 1024;
        }
        f = f.substr(0,1); 

        //ASIGNAR LOS DATOS PARA CREAR

        disco.mbr_tamano = size;
        disco.mbr_fecha_creacion = time(nullptr);
        disco.disk_fit = toupper(f[0]);
        disco.mbr_disk_signature = rand() % 9999 + 100;  //NUMERO RANDOM

//PUNTERO PARA EL ARCHIVO:

        FILE *file = fopen(path.c_str(),"r"); // convertit Y modo lectura
        if(file != NULL){
            scan.errores("MKDISK","ya existe");
            fclose(file);
            return;
        }

        disco.mbr_Partition_1 = Structs::Partition();
        disco.mbr_Partition_2 = Structs::Partition();
        disco.mbr_Partition_3 = Structs::Partition();
        disco.mbr_Partition_4 = Structs::Partition();

//ver lo de las comillas del path---> EN SCANER SE QUITAN 
        string path2 = path;
        if(path.substr(0,1) == "\""){
            path2 = path.substr(1,path.length()-2);
        };
//ver que si sea .dsk si no no es valido, no se puede crear
//al encontrar el punto
        if(!scan.compare(path.substr(path.find_last_of(".") + 1), "dsk")){
            scan.errores("MKDISK","unico tipo acetable --> .dsk");
            return;
        }

        try{
            FILE *file = fopen(path.c_str(), "w+b");
            //si no es nulo se esrcibe
            if(file!=NULL){
                fwrite("\0", 1, 1, file);
                fseek(file, size-1, SEEK_SET);
                fwrite("\0", 1, 1, file);
                rewind(file);
                fwrite(&disco, sizeof(Structs::MBR), 1, file); //al puntero esrcibir el TAMANO DEL MBR
                fclose(file);
            }else{
                //condicion: si la carpeta no existe se crea
                string comando1 = "mkdir -p \""+ path + "\"";
                string comando2 = "rmdir \""+ path + "\"";
                system(comando1.c_str());
                system(comando2.c_str());
                //luego de que se crea lo mismo
                FILE *file = fopen(path.c_str(), "w+b");
                fwrite("\0",1,1,file);
                fseek(file, size - 1, SEEK_SET);
                fwrite("\0", 1, 1, file);
                rewind(file);
                fwrite(&disco, sizeof(Structs::MBR),1, file);
                fclose(file);
            }

            FILE *imprimir = fopen(path.c_str(), "r");
            if(imprimir!=NULL){
                Structs::MBR discoI;
                fseek(imprimir, 0, SEEK_SET);
                fread(&discoI,sizeof(Structs::MBR), 1,imprimir);  //punteros del mbr
                struct tm *tm;
                tm = localtime(&discoI.mbr_fecha_creacion);
                char mostrar_fecha [20];
                strftime(mostrar_fecha, 20, "%Y/%m/%d %H:%M:%S", tm);                
                scan.respuesta("MKDISK","   >>>> DISCO CREADO!!!!!");

                //SE LEE EL DISCO
                std::cout << ">>>>Nuevo Disco<<<<<<" << std::endl;
                std::cout << "tamano >>  "<< discoI.mbr_tamano << std::endl;
                std::cout << "Fecha >>  "<< mostrar_fecha << std::endl;
                std::cout << "Fit >>  "<< discoI.disk_fit << std::endl;
                std::cout << "disk firma >>  "<< discoI.mbr_disk_signature << std::endl;
                cout << "MBR  BITS >>  " << sizeof(Structs::MBR) << endl;
                std::cout << "Path/RUTA >>  "<< path2 << std::endl;
            }
            fclose(imprimir);

        }catch(const exception& e){
            scan.errores("MKDISK","OCURRIO UN ERROR AL CREAR EL DISCO --> estado: No creado");
        }
    }catch(invalid_argument &e){
        scan.errores("MKDISK","El tamano del disco debe ser entero, NO DECIMAL");
    }

}


//RMDISK --> ELIMINAR
void Disk::rmdisk(vector<string> tokens) {
    string path = "";
    bool error = false;

    // se jala el parametros >> verificar que si venga -> obligatorio
    for (string rec : tokens) {
        string par = rec.substr(0, rec.find("="));
        rec.erase(0, par.length() + 1);

        if (scan.compare(par, "path")) {
            if (path.empty()) {
                path = rec;
            } else {
                scan.errores("RMDISK", "ya hay un path ingresado" + par);
                error = true;
                break;
            }
        } else {
            scan.errores("RMDISK", "parametro inesperado (unicamente se necesita path)" + par);
            error = true;
            break;
        }
    }

    // si hay un error que se salga -> no se caiga el proyecto
    if (error) {
        return;
    }

    // el path si tiene que estar -> si no error
    if (path.empty()) {
        scan.errores("RMDISK", "El path es un parametro obligatorio, por favor agregar");
        return;
    }

    // quitar comillas
    if (path.front() == '"' && path.back() == '"') {
        path = path.substr(1, path.length() - 2);
    }

    // ver si existe
    FILE *file = fopen(path.c_str(), "r");
    if (file != NULL) {
        fclose(file);

        // Delete
        if (remove(path.c_str()) == 0) {
            scan.respuesta("RMDISK", "El disco se a eliminado correctamente: " + path);
        } else {
            scan.errores("RMDISK", "<<ocurrio un error al intentar eliminar el disco>>: " + path);
        }
    } else {
        scan.errores("RMDISK", "Este disco no existe o no es reconocido, revisar: " + path);
    }
}

//---------------------------------

// FDISK

void Disk::fdisk(vector<string> context){
    bool dlt = false;
    for (string current : context) {
        string id = shared.lower(current.substr(0, current.find('=')));
        current.erase(0, id.length() + 1);
        if (current.substr(0, 1) == "\"") {
            current = current.substr(1, current.length() - 2);
        }
        //SI ENCUENTRA EL COMANDO DELETE
        if (shared.compare(id, "delete")) {
            dlt = true;
        }
    }
    if (!dlt) {
        vector<string> required = {"s", "path", "name"};
        string size;
        string u = "k";
        string path;
        string type = "P";
        string f = "WF";
        string name;
        string add;

        for (auto current : context) {
            string id = shared.lower(current.substr(0, current.find('=')));
            current.erase(0, id.length() + 1);
            if (current.substr(0, 1) == "\"") {
                current = current.substr(1, current.length() - 2);
            }

            if (shared.compare(id, "s")) {
                if (count(required.begin(), required.end(), id)) {
                    auto itr = find(required.begin(), required.end(), id);
                    required.erase(itr);
                    size = current;
                }
            } else if (shared.compare(id, "u")) {
                u = current;
            } else if (shared.compare(id, "path")) {
                if (count(required.begin(), required.end(), id)) {
                    auto itr = find(required.begin(), required.end(), id);
                    required.erase(itr);
                    path = current;
                }
            } else if (shared.compare(id, "t")) {
                type = current;
            } else if (shared.compare(id, "f")) {
                f = current;
            } else if (shared.compare(id, "name")) {
                if (count(required.begin(), required.end(), id)) {
                    auto itr = find(required.begin(), required.end(), id);
                    required.erase(itr);
                    name = current;
                }
            } else if (shared.compare(id, "add")) {
                add = current;
                if (count(required.begin(), required.end(), "s")) {
                    auto itr = find(required.begin(), required.end(), "s");
                    required.erase(itr);
                    size = current;
                }
            }
        }
        if (!required.empty()) {
            shared.handler("FDISK", "Parametros obligatorios vacios -> agregarlos");
            return;
        }
        if (add.empty()) {
            generatepartition(size, u, path, type, f, name, add);
        } else {
            addpartition(add, u, name, path);
        }

    } else {
        vector<string> required = {"path", "name", "delete"};
        string _delete;
        string path;
        string name;

        for (auto current : context) {
            string id = shared.lower(current.substr(0, current.find('=')));
            current.erase(0, id.length() + 1);
            if (current.substr(0, 1) == "\"") {
                current = current.substr(1, current.length() - 2);
            }

            if (shared.compare(id, "path")) {
                if (count(required.begin(), required.end(), id)) {
                    auto itr = find(required.begin(), required.end(), id);
                    required.erase(itr);
                    path = current;
                }
            } else if (shared.compare(id, "name")) {
                if (count(required.begin(), required.end(), id)) {

                    auto itr = find(required.begin(), required.end(), id);
                    required.erase(itr);
                    name = current;
                }
            } else if (shared.compare(id, "delete")) {
                if (count(required.begin(), required.end(), id)) {

                    auto itr = find(required.begin(), required.end(), id);
                    required.erase(itr);
                    _delete = current;
                }
            }
        }
        if (required.size() != 0) {
            shared.handler("FDISK", "Parametros obligatorios vacios -> agregarlos");
            return;
        }
        //shared.response("FDISK", "PARTICION AUMENTADA");
        shared.response("FDISK - delete", "comando ejecutado correctamente");
    }
}





vector<Structs::EBR> Disk::getlogics(Structs::Partition partition, string p) {
    vector<Structs::EBR> ebrs;

    FILE *file = fopen(p.c_str(), "rb+");
    rewind(file);
    Structs::EBR tmp;
    fseek(file, partition.part_start, SEEK_SET);
    fread(&tmp, sizeof(Structs::EBR), 1, file);
    do {
        if (!(tmp.part_status == '0' && tmp.part_next == -1)) {
            if (tmp.part_status != '0') {
                ebrs.push_back(tmp);
            }
            fseek(file, tmp.part_next, SEEK_SET);
            fread(&tmp, sizeof(Structs::EBR), 1, file);
        } else {
            fclose(file);
            break;
        }
    } while (true);
    return ebrs;
}

Structs::Partition Disk::findby(Structs::MBR mbr, string name, string path) {
    Structs::Partition partitions[4];
    partitions[0] = mbr.mbr_Partition_1;
    partitions[1] = mbr.mbr_Partition_2;
    partitions[2] = mbr.mbr_Partition_3;
    partitions[3] = mbr.mbr_Partition_4;

    bool ext = false;
    Structs::Partition extended;
    for (auto &partition : partitions) {
        if (partition.part_status == '1') {
            if (shared.compare(partition.part_name, name)) {
                return partition;
            } else if (partition.part_type == 'E') {
                ext = true;
                extended = partition;
            }
        }
    }
    if (ext) {
        vector<Structs::EBR> ebrs = getlogics(extended, path);
        for (Structs::EBR ebr : ebrs) {
            if (ebr.part_status == '1') {
                if (shared.compare(ebr.part_name, name)) {
                    Structs::Partition tmp;
                    tmp.part_status = '1';
                    tmp.part_type = 'L';
                    tmp.part_fit = ebr.part_fit;
                    tmp.part_start = ebr.part_start;
                    tmp.part_size = ebr.part_size;
                    strcpy(tmp.part_name, ebr.part_name);
                    return tmp;
                }
            }
        }
    }
    throw runtime_error("NO EXISTE UNA PARTICION CON ESE NOMBRE");
}



//PARTICION

void Disk::generatepartition(string s, string u, string p, string t, string f, string n, string a){
    try{
        //VERIFICACIONES DE ERRORES
        startValue = 0;
        int i = stoi(s);
        if(i <= 0){
            shared.handler("FDISK", "EL TAMANO DEBE SER MAYOR DE 0");
            return;
        }
        //STANDARD: KILO SI NO VIENE ---> COMPARAR ENTRE B, K Y M
        if(shared.compare(u, "b") || shared.compare(u, "k") || shared.compare(u, "m")){
            if(!shared.compare(u, "b")){
                //TERMARIO   --> MEGA Y BYTES
                i *= (shared.compare(u, "k") ? 1024 : 1024 * 1024);
            }
        }else{
            shared.handler("FDISK", "El valor  debe ser: M,B O K; cualquiera de esos no es valido");
            return;
        }
        //ruta --> las comillas ver si tiene y quitarlas
        if(p.substr(0,1) == "\""){
            p = p.substr(1,p.length()-2);
            //! menos 2, se quita la 1
        }
        //particion -> primario, extenido y logico
        if(!(shared.compare(t, "p") || shared.compare(t, "e") || shared.compare(t, "l"))){
            shared.handler("FDISK", "El valor  debe ser: P, E o L; cualquiera de esos no es valido");
            return;
        }
        //BEST FIR, WORST FIT Y FIRST FIT -- IGUAL QUE EN MK
        if(!(shared.compare(f, "bf") || shared.compare(f, "ff") || shared.compare(f, "wf"))){
            shared.handler("FDISK", "El valor  debe ser: FF,BF o WF; cualquiera de esos no es valido");
            return;
        }
        Structs::MBR disco;

        //VERIFICACION QUE PIDE - ERROR
        FILE *file = fopen(p.c_str(), "rb+");
        if(file == NULL){
            shared.handler("FDISK", "Se intenta partir un disco que no existe -> revisar ruta");
            return;
        }else{
            rewind(file);
            //SE LEE Y SE REESCTRUCTURA
            fread(&disco, sizeof(Structs::MBR), 1, file);
        }
        fclose(file);

// EMPIEZA EL VECTOR DE PARTICION, INICIO VACIO - LISTA DE PARTICION
// ERROR -< MBR AL INICIO!!!

        vector<Structs::Partition> particiones = getPartitions(disco); 
        vector<Transition> between;

        int used = 0;
        int ext = 0;
        int c = 1;
        // BASE ES EL PESO
        int base = sizeof(disco);
        // SE OBTIENEN LAS PARTICIONES
        Structs::Partition extended;

        //RECORRER LAS PARTICIONES ---> recorrer las 4 particiones creadas
//particiones creadas, datos vacios
        for(Structs::Partition p : particiones){
            if(p.part_status == '1'){
                Transition trn;
                trn.partition = c;
                trn.start = p.part_start;
                //puntero final, donde inicio mas el tamano
                trn.end = p.part_start + p.part_size;

                trn.before = trn.start - base;
                base = trn.end;

                if(used != 0){
                    //esto es si ya se modifico, si es dist de 0
                    between.at(used-1).after = trn.start - between.at(used-1).end; 
                }
                between.push_back(trn);
                used++;

//particion extendida (adentro logicas)
                if(p.part_type == 'e' || p.part_type == 'E'){
                    ext++;
                    extended = p;
                }
            }
            //revisar porque max 4. SI ES LOGICA COMO VA DENTRO SI

            if(used == 4 && !(shared.compare(t, "l"))){
                shared.handler("FDISK", "Particiones: 4 - NO SE PUEDEN CREAR MAS PARTICIONES PRIMARIAS");
                return;
            }else if(ext==1 && !(shared.compare(t, "e"))){
                shared.handler("FDISK", "--- NO SE PUEDEN CREAR MAS PARTICIONES EXTENDIDAS, ya hay 1");
                return;
            }
            c++;
        }
        //LOGICA PROPIA DE LAS PARTICIONES LOGICAS --< DENTRO DE EXTENDIDAD
        if(ext == 0 && shared.compare(t, "l")){
            shared.handler("FDISK", "LAS PARTICIONES LOGICAS NECESITAN DE UNA PARTICION EXTENDIDA. <no se puede crear una sin una extendida previa>");
            return;
        }
        if(used != 0){
            between.at(between.size()-1).after = disco.mbr_tamano - between.at(between.size()-1).end;
        }

        try{
            findby(disco, n,p);
            shared.handler("FDISK", "nombre de particion repetido, cambiar nombre o revisar particiones");
            return;
        }catch(exception &e){}

        // ACA YA SE HACE LA PARTICIOM ---> LUEGO DE LAS VERIFICACIONES, FIJO LOS DATOS SON VALIDOS
        Structs::Partition newPartition;
        newPartition.part_status = '1';
        newPartition.part_size = i; //size
        newPartition.part_type = toupper(t[0]); // P, E, L --> T
        newPartition.part_fit = toupper(f[0]); // B, F, W ---> F
        strcpy(newPartition.part_name, n.c_str()); // NOMBRE DE LA NUEVA

        if(shared.compare(t, "l")){
            // Aqui se crea la particion logica
            shared.response("FDISK", "<<Particion logica>>");
        }

        disco = adjust(disco, newPartition, between, particiones, used);



        FILE *bfile = fopen(p.c_str(), "rb+");
        if(bfile != NULL){
            fseek(bfile, 0, SEEK_SET);
            fwrite(&disco, sizeof(Structs::MBR), 1, bfile);
            if(shared.compare(t,"e")){
                Structs::EBR ebr;
                ebr.part_start = startValue;
                fseek(bfile, startValue, SEEK_SET);
                fwrite(&ebr, sizeof(Structs::EBR), 1, bfile);
            }
            fclose(bfile);
            shared.response("FDISK", "Particion creada!!!!");
        }

    }catch (invalid_argument &e) {
        shared.handler("FDISK", "el valor de s debe ser entero, no decimal");
        return;
    }
    catch (exception &e) {
        shared.handler("FDISK", e.what());
        return;
    }
};

//las particiones de una
vector<Structs::Partition> Disk::getPartitions(Structs::MBR mbr){
    vector<Structs::Partition> partitions;
    partitions.push_back(mbr.mbr_Partition_1);
    partitions.push_back(mbr.mbr_Partition_2);
    partitions.push_back(mbr.mbr_Partition_3);
    partitions.push_back(mbr.mbr_Partition_4);
    return partitions;
}

Structs::MBR

//que reciba el vector de transiciones
Disk::adjust(Structs::MBR mbr, Structs::Partition p, vector<Transition> t, vector<Structs::Partition> ps, int u){
    if (u == 0) {
        //primer puntero -< primera transicion
        p.part_start = sizeof(mbr);
        startValue = p.part_start;
        mbr.mbr_Partition_1 = p;
        return mbr;
    } else {
        Transition toUse;
        int c = 0;
        for (Transition tr : t) {
            if (c == 0) {
                toUse = tr;
                c++;
                continue;
            }

            if (toupper(mbr.disk_fit) == 'F') {
                if (toUse.before >= p.part_size || toUse.after >= p.part_size) {
                    break;
                }
                toUse = tr;
            } else if (toupper(mbr.disk_fit) == 'B') {
                if (toUse.before < p.part_size || toUse.after < p.part_size) {
                    toUse = tr;
                } else {
                    if (tr.before >= p.part_size || tr.after >= p.part_size) {
                        int b1 = toUse.before - p.part_size;
                        int a1 = toUse.after - p.part_size;
                        int b2 = tr.before - p.part_size;
                        int a2 = tr.after - p.part_size;

                        if ((b1 < b2 && b1 < a2) || (a1 < b2 && a1 < a2)) {
                            c++;
                            continue;
                        }
                        toUse = tr;
                    }
                }
            } else if (toupper(mbr.disk_fit) == 'W') {
                if (!(toUse.before >= p.part_size) || !(toUse.after >= p.part_size)) {
                    toUse = tr;
                } else {
                    if (tr.before >= p.part_size || tr.after >= p.part_size) {
                        int b1 = toUse.before - p.part_size;
                        int a1 = toUse.after - p.part_size;
                        int b2 = tr.before - p.part_size;
                        int a2 = tr.after - p.part_size;

                        if ((b1 > b2 && b1 > a2) || (a1 > b2 && a1 > a2)) {
                            c++;
                            continue;
                        }
                        toUse = tr;
                    }
                }
            }
            c++;
        }
        if (toUse.before >= p.part_size || toUse.after >= p.part_size) {
            if (toupper(mbr.disk_fit) == 'F') {
                if (toUse.before >= p.part_size) {
                    p.part_start = (toUse.start - toUse.before);
                    startValue = p.part_start;
                } else {
                    p.part_start = toUse.end;
                    startValue = p.part_start;
                }
            } else if (toupper(mbr.disk_fit) == 'B') {
                int b1 = toUse.before - p.part_size;
                int a1 = toUse.after - p.part_size;

                if ((toUse.before >= p.part_size && b1 < a1) || !(toUse.after >= p.part_start)) {
                    p.part_start = (toUse.start - toUse.before);
                    startValue = p.part_start;
                } else {
                    p.part_start = toUse.end;
                    startValue = p.part_start;
                }
            } else if (toupper(mbr.disk_fit) == 'W') {
                int b1 = toUse.before - p.part_size;
                int a1 = toUse.after - p.part_size;

                if ((toUse.before >= p.part_size && b1 > a1) || !(toUse.after >= p.part_start)) {
                    p.part_start = (toUse.start - toUse.before);
                    startValue = p.part_start;
                } else {
                    p.part_start = toUse.end;
                    startValue = p.part_start;
                }
            }
            Structs::Partition partitions[4];
            for (int i = 0; i < ps.size(); i++) {
                partitions[i] = ps.at(i);
            }
            for (auto &partition : partitions) {
                if (partition.part_status == '0') {
                    partition = p;
                    break;
                }
            }

            Structs::Partition aux;
            for (int i = 3; i >= 0; i--) {
                for (int j = 0; j < i; j++) {
                    if ((partitions[j].part_start > partitions[j + 1].part_start)) {
                        aux = partitions[j + 1];
                        partitions[j + 1] = partitions[j];
                        partitions[j] = aux;
                    }
                }
            }

            for (int i = 3; i >= 0; i--) {
                for (int j = 0; j < i; j++) {
                    if (partitions[j].part_status == '0') {
                        aux = partitions[j];
                        partitions[j] = partitions[j + 1];
                        partitions[j + 1] = aux;
                    }
                }
            }
            mbr.mbr_Partition_1 = partitions[0];
            mbr.mbr_Partition_2 = partitions[1];
            mbr.mbr_Partition_3 = partitions[2];
            mbr.mbr_Partition_4 = partitions[3];
            return mbr;
        } else {
            throw runtime_error("YA NO HAY ESPACION PARA LA PARTICION");
        }
    }
}

void Disk::addpartition(string add, string u, string n, string p) {
    try {
        int i = stoi(add);

        if (shared.compare(u, "b") || shared.compare(u, "k") || shared.compare(u, "m")) {

            if (!shared.compare(u, "b")) {
                i *= (shared.compare(u, "k")) ? 1024 : 1024 * 1024;
            }
        } else {
            throw runtime_error("-u necesita valores específicos");
        }


        FILE *file = fopen(p.c_str(), "rb+");
        if (file == NULL) {
            throw runtime_error("disco no existente");
        }

        Structs::MBR disk;
        rewind(file);
        fread(&disk, sizeof(Structs::MBR), 1, file);

        findby(disk, n, p);

        Structs::Partition partitions[4];
        partitions[0] = disk.mbr_Partition_1;
        partitions[1] = disk.mbr_Partition_2;
        partitions[2] = disk.mbr_Partition_3;
        partitions[3] = disk.mbr_Partition_4;


        for (int i = 0; i < 4; i++) {
            if (partitions[i].part_status == '1') {
                if (shared.compare(partitions[i].part_name, n)) {
                    if ((partitions[i].part_size + (i)) > 0) {
                        if (i != 3) {
                            if (partitions[i + 1].part_start != 0) {
                                if (((partitions[i].part_size + (i) +
                                    partitions[i].part_start) <=
                                    partitions[i + 1].part_start)) {
                                    partitions[i].part_size += i;
                                    break;
                                } else {
                                    throw runtime_error("se sobrepasa el límite(1)");
                                    shared.response("FDISK", "PARTICION AUMENTADA");
                                }
                            }
                        }
                        if ((partitions[i].part_size + i +
                            partitions[i].part_start) <= disk.mbr_tamano) {
                            partitions[i].part_size += i;
                            break;
                        } else {
                            throw runtime_error("se sobrepasa el límite(2)");
                        }

                    }
                }
            }
        }

        disk.mbr_Partition_1 = partitions[0];
        disk.mbr_Partition_2 = partitions[1];
        disk.mbr_Partition_3 = partitions[2];
        disk.mbr_Partition_4 = partitions[3];

        rewind(file);
        fwrite(&disk, sizeof(Structs::MBR), 1, file);
        shared.response("FDISK", "PARTICION AUMENTADA");
        fclose(file);
    }
    catch (exception &e) {
        shared.handler("FDISK", e.what());
        return;
    }

}