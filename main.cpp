#include "common.h"
#include "tubetree.h"

//#define VOL_SERIES

int main(int argc, char *argv[])
{
    if(argc<3){
        qDebug()<< ">>> eXtensible Statistical Engine for Analysis of Tubular Structures <<<" << endl;
        qDebug()<< ">>>                     Shruthi Raghavan                             <<<" << endl;
        qDebug()<< "Usage:" << endl;
        qDebug()<< "To be Done";
        qDebug()<< ">>>                                                                  <<<" << endl;
    }

    std::string str;
    qDebug() << "Enter the name of swc file with path: " << endl;
    std::getline(std::cin, str);
    QString filename(str.c_str());
    QFileInfo fi(filename);

    TubeTree tubetree(filename);
    qDebug() <<"Read the swc file into nodes";

    QString outputvtkfile = fi.absolutePath()+"/"+fi.baseName()+".vtk";
    tubetree.filebasename = fi.baseName();
    if(tubetree.writeVtkPoly(outputvtkfile)) {
        qDebug() << "Wrote VTK file";
    }else return EXIT_FAILURE;

    QStringList sl = fi.baseName().split("_");
    int sl_size = sl.size();
    tubetree.vol_idz = sl[sl_size-1].toInt();
    tubetree.vol_idy = sl[sl_size-2].toInt();
    tubetree.vol_idx = sl[sl_size-3].toInt();
    QString outputparamsfile = fi.absolutePath()+"/"+fi.baseName()+".json";
    if(tubetree.writeJson(outputparamsfile)) {
        qDebug() << "Wrote Params file";
    }else return EXIT_FAILURE;

    return EXIT_SUCCESS;
}
