#include "common.h"
#include "tubetree.h"

#define DEBUGPGM
#define MAX_ARGS

ParamsFormat writeParams;

int main(int argc, char *argv[])
{
    std::string infilestr = "";
    QString coutfilestr = "";
    QString poutfilestr = "";
    bool input_found = false;
    bool coutput_found = false;
    bool poutput_found = false;
    bool write_vtk = false;
    bool write_params = false;
    bool vol_series = false;
    bool allow_nfurc = false;
    bool type_fix = false;
    if(argc<3){
        qDebug()<< ">>> eXtensible Statistical Engine for Analysis of Tubular Structures <<<" << endl;
        qDebug()<< ">>>                     Shruthi Raghavan                             <<<" << endl;
        qDebug()<< "Usage:" << endl;
        qDebug()<< "To be Done";
        qDebug()<< ">>>                                                                  <<<" << endl;
        return EXIT_SUCCESS;
    }
    else{
        std::string switch_name;
        int i=0;
        for(i=1;i<argc;i++){
            switch_name = argv[i];
            if((switch_name == "-i") || (switch_name == "-I")){
                i++;
                infilestr = argv[i];
                //infilestr.erase(std::remove(infilestr.begin(),infilestr.end(),'\"'),infilestr.end());
                input_found = true;
            } else if((switch_name == "-c") || (switch_name == "-C")){
                i++;
                std::string convformat = argv[i];
                if(convformat == "vtk") write_vtk = true;
                else{
                    qDebug() << "vtk is the only supported format for conversion now.";
                    return EXIT_FAILURE;
                }
            } else if((switch_name == "-p") || (switch_name == "-P")){
                i++;
                std::string inParamsFormat = argv[i];
                if(inParamsFormat == "json"){
                    write_params = true;
                    writeParams = Json;
                }
                else if(inParamsFormat == "dat"){
                    write_params = true;
                    writeParams = Binary;
                }
                else {
                    qDebug() << "Unsupported Params Format. Please use json or binary options only";
                    return EXIT_FAILURE;
                }
            } else if((switch_name == "-o")||(switch_name == "-O")){
                i++;
                std::string outfile = argv[i];
                coutfilestr = outfile.c_str();
                poutfilestr = outfile.c_str();
                //outfile.erase(std::remove(outfile.begin(),outfile.end(),'\"'),outfile.end());
                //if(outfilestr.startsWith("\"")) outfilestr.remove(0,1);
                //if(outfilestr.endsWith("\"")) outfilestr.remove(outfilestr.size()-1,1);
                //if(outfilestr.endsWith("\\")) outfilestr.remove(outfilestr.size()-1,1);
                coutput_found = true;
                poutput_found = true;
            } else if((switch_name == "-co")||(switch_name == "-CO")){
                i++;
                std::string outfile = argv[i];
                coutfilestr = outfile.c_str();
                //outfile.erase(std::remove(outfile.begin(),outfile.end(),'\"'),outfile.end());
                //if(outfilestr.startsWith("\"")) outfilestr.remove(0,1);
                //if(outfilestr.endsWith("\"")) outfilestr.remove(outfilestr.size()-1,1);
                //if(outfilestr.endsWith("\\")) outfilestr.remove(outfilestr.size()-1,1);
                coutput_found = true;
            } else if((switch_name == "-po")||(switch_name == "-PO")){
                i++;
                std::string outfile = argv[i];
                poutfilestr = outfile.c_str();
                //outfile.erase(std::remove(outfile.begin(),outfile.end(),'\"'),outfile.end());
                //if(outfilestr.startsWith("\"")) outfilestr.remove(0,1);
                //if(outfilestr.endsWith("\"")) outfilestr.remove(outfilestr.size()-1,1);
                //if(outfilestr.endsWith("\\")) outfilestr.remove(outfilestr.size()-1,1);
                poutput_found = true;
            } else if(switch_name == "-vol_series"){
                vol_series = true;
            } else if(switch_name == "-allow_nfurc"){
                allow_nfurc = true;
            } else if(switch_name == "-type_fix"){
                type_fix = true;
            } else {
                std::cout << "Unrecognized argument: " << switch_name;
                return EXIT_FAILURE;
            }
        }
    }

    if(!input_found) {
        qDebug() << "No input file specified. Please supply an input using [-i <Path-to-file>]" <<endl;
        return EXIT_FAILURE;
    }
    if(!coutput_found && !poutput_found) {
        qDebug() << "No output path specified. All output files will be saved to the input file's location." << endl;
        qDebug() << "MAY CAUSE OVERWRITING OF EXISTING FILES! BE CAREFUL!" << endl;
    }

    QString filename(infilestr.c_str());
    //if(filename.startsWith("\"")) filename.remove(0,1);
    //if(filename.endsWith("\"")) filename.remove(filename.size()-1,1);
    QFileInfo fi(filename);
    if(!fi.exists()){
        qDebug() << "File " << filename << " does not exist" << endl;
        return EXIT_FAILURE;
    }

    /*This will read the file and Compute all the parameters*/
    //This will probably have to change. This is intended as a data structure. the use should be the programmer's choice.
    TubeTree tubetree(filename);

#ifdef DEBUGPGM
    qDebug() <<"Read the swc file into nodes";
#endif

    if(write_vtk){
        QString outputvtkfile;
        if(coutput_found){
            outputvtkfile = coutfilestr+"/"+fi.baseName()+".vtk";
        }else{
            outputvtkfile = fi.absolutePath()+"/"+fi.baseName()+".vtk";
        }
        tubetree.filebasename = fi.baseName();
        if(tubetree.writeVtkPoly(outputvtkfile)) {
            //qDebug() << "Wrote VTK file";
        }else {
            qDebug() << "Could not write VTK file";
            //return EXIT_FAILURE;
        }
    }

    if(vol_series){
    QStringList sl = fi.baseName().split("_");
    int sl_size = sl.size();
    tubetree.vol_idz = sl[sl_size-1].toInt();
    tubetree.vol_idy = sl[sl_size-2].toInt();
    tubetree.vol_idx = sl[sl_size-3].toInt();
    }

    if(write_params){
        QString outputparamsfile;
        if(poutput_found){
            outputparamsfile = poutfilestr+"/"+fi.baseName()+".json";
        }else{
            outputparamsfile = fi.absolutePath()+"/"+fi.baseName()+".json";
        }
        if(writeParams==Json){
            if(tubetree.writeJson(outputparamsfile)) {
                qDebug() << "Wrote Params file";
            }else return EXIT_FAILURE;
        }
        else if(writeParams==Binary){
            if(tubetree.writeDat(outputparamsfile)){
                qDebug() << "Wrote Params File";
            } else return EXIT_FAILURE;
        }
    }

    return EXIT_SUCCESS;
}
