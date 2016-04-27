#include "common.h"
#include "tubetree.h"

int main()
{
    std::string str;
    qDebug() << "Enter the name of swc file with path: " << endl;
    std::getline(std::cin, str);
    QString filename(str.c_str());

    TubeTree tubetree(filename);
    qDebug() <<"Read the swc file into nodes";

    QString outputvtkfile = "C:/Users/Shruthi/Desktop/M3R.vtk";
    if(tubetree.writeVtkPoly(outputvtkfile)) {
        qDebug() << "Wrote VTK file";
        return EXIT_SUCCESS;
    }else return EXIT_FAILURE;
}
