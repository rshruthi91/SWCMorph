#include "common.h"
#include "tubetree.h"

int main()
{
    std::string str;
    qDebug() << "Enter the name of swc file with path: " << endl;
    std::getline(std::cin, str);
    QString filename(str.c_str());
    QFileInfo fi(filename);

    TubeTree tubetree(filename);
    qDebug() <<"Read the swc file into nodes";

    QString outputvtkfile = fi.absolutePath()+"/"+fi.baseName()+".vtk";
    if(tubetree.writeVtkPoly(outputvtkfile)) {
        qDebug() << "Wrote VTK file";
        return EXIT_SUCCESS;
    }else return EXIT_FAILURE;
}
