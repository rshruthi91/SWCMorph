#include "tubetree.h"

TubeTree::TubeTree()
{
    qDebug() << "This is not a supported use yet. It is just a placeholder!";
}

TubeTree::TubeTree(QString swcfile)
{
    this->infilepath = swcfile;
    SWCReadNodes();
}

 bool TubeTree::SWCReadNodes(){
     QFile file(this->infilepath);
     if (!file.open(QIODevice::ReadOnly | QIODevice::Text)){
         qDebug() << "Could not open the file in Read Only mode";
         return false;
       }
     QTextStream swchandle(&file);

     while(!swchandle.atEnd()){
         Node swcnode;
         QString swcline = swchandle.readLine();
         QStringList swcnodeparams = swcline.split(" ",QString::SkipEmptyParts);
         if(swcnodeparams[0] != "#"){
             swcnode.setID(swcnodeparams[0].toInt());
             swcnode.setType(swcnodeparams[1].toInt());
             swcnode.setposX(swcnodeparams[2].toDouble());
             swcnode.setposY(swcnodeparams[3].toDouble());
             swcnode.setposZ(swcnodeparams[4].toDouble());
             swcnode.setRadius(swcnodeparams[5].toDouble());
             swcnode.setPID(swcnodeparams[6].toInt());
         } else continue;
         this->NodeList.append(swcnode);
     }
     return true;
}
