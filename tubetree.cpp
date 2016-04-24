#include "tubetree.h"

TubeTree::TubeTree()
{
    qDebug() << "This is not a supported use yet. It is just a placeholder!";
}

TubeTree::TubeTree(QString swcfile)
{
    this->infilepath = swcfile;
    if(!SWCReadNodes()){
        NodeList.clear();
        CompartmentList.clear();
        throw std::exception();
    }
    if(!makeCompartments()){
        NodeList.clear();
        CompartmentList.clear();
        throw std::exception();
    }

    makeSegments();
}

 bool TubeTree::SWCReadNodes(){
     QFile file(this->infilepath);
     if (!file.open(QIODevice::ReadOnly | QIODevice::Text)){
         qDebug() << "Could not open the file in Read Only mode";
         return false;
       }
     QTextStream swchandle(&file);

     while(!swchandle.atEnd()){
         Node *swcnode = new Node;
         QString swcline = swchandle.readLine();
         QStringList swcnodeparams = swcline.split(" ",QString::SkipEmptyParts);
         if(swcnodeparams[0] != "#"){
             swcnode->setID(swcnodeparams[0].toInt());
             swcnode->setType(swcnodeparams[1].toInt());
             swcnode->setposX(swcnodeparams[2].toDouble());
             swcnode->setposY(swcnodeparams[3].toDouble());
             swcnode->setposZ(swcnodeparams[4].toDouble());
             swcnode->setRadius(swcnodeparams[5].toDouble());
             swcnode->setPID(swcnodeparams[6].toInt());
             if(swcnode->getPID() == -1) swcnode->setRoot(true);
         } else continue;
         this->NodeList.append(swcnode);
     }
     return true;
}

 bool TubeTree::makeCompartments() {
     int num_bifs = 0;
     foreach(Node* swcnode, this->NodeList) {
         foreach(Node* nextnode, this->NodeList) {
             if(nextnode->getPID() == swcnode->getID()) {
                 num_bifs++;
                 if(num_bifs > 2 && (swcnode->getType()!= 1)){
                     qDebug() << "Trifurcation found - unsupported";
                     qDebug() << "Node number: " << swcnode->getID();
                     return false;
                 }
                 Compartment* c = new Compartment;
                 c->setStart(swcnode);
                 c->setEnd(nextnode);
                 CompartmentList.append(c);
             }
         }
         if(num_bifs == 0) {
             swcnode->setTerminal(true);
         }
         else {
             CompartmentList.last()->getStart()->setNBifs(num_bifs);
             if(num_bifs > 1)
                 CompartmentList.last()->getStart()->setBranch(true);
         }
         num_bifs = 0;
     }
     return true;
 }

 void TubeTree::makeSegments() {
     QVector<Compartment*> traversedCompartments;
     foreach(Compartment* comp, this->CompartmentList) {
         Segment* s = new Segment;
         if(
                 comp->getStart()->isBranch()||
                 comp->getStart()->isRoot()
                 )
         {
             if(traversedCompartments.count(comp) >= comp->getStart()->getNBifs()) continue;
             s->addCompartment(comp);
             traversedCompartments.append(comp);
             Compartment* prev_comp = comp;
             //constructing compartments of the segment
             foreach(Compartment* innercomp,this->CompartmentList) {
                 Compartment* curr_comp = innercomp;
                 if(curr_comp->getStart() == prev_comp->getEnd()) {
                     if(innercomp->getStart()->isBranch()) break;
                     s->addCompartment(innercomp);
                     prev_comp = innercomp;
                 }
             }
             this->Segments.append(s);
         } else delete(s);
     }
 }
