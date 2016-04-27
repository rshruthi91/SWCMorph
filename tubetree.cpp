#include "tubetree.h"

TubeTree::TubeTree()
{
    this->num_trees = 0;
    this->infilepath ="";
    qDeleteAll(this->NodeList);
    this->NodeList.clear();
    qDeleteAll(this->CompartmentList);
    this->CompartmentList.clear();
    qDeleteAll(this->Segments);
    this->Segments.clear();
    qDeleteAll(this->RootSegList);
    this->RootSegList.clear();
    qDebug() << "This is not a supported use yet. It is just a placeholder!";
}

TubeTree::TubeTree(QString swcfile)
{
    this->populated = false;
    this->num_trees = 0;
    this->infilepath = swcfile;
    qDeleteAll(this->NodeList);
    this->NodeList.clear();
    qDeleteAll(this->CompartmentList);
    this->CompartmentList.clear();
    qDeleteAll(this->Segments);
    this->Segments.clear();
    qDeleteAll(this->RootSegList);
    this->RootSegList.clear();
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
    getRootSegments();
    connectSegments();
    foreach(Segment* s, this->Segments){
        s->updateParams();
    }
    this->populated = true;
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
             if(swcnode->getPID() == -1) {
                 num_trees++;
                 swcnode->setRoot(true);
             }
         } else continue;
         this->NodeList.insert(swcnode->getID(),swcnode);
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

 void TubeTree::getRootSegments() {
     foreach(Segment* s, this->Segments) {
         if(s->CompartmentList.first()->getStart()->isRoot()) {
             s->setRoot(true);
             this->RootSegList.append(s);
         }
     }
 }

 void TubeTree::connectSegments(){
     //Populate Children Segments
     foreach(Segment*s , this->Segments) {
         if( ( s->getChildren().isEmpty() &&
              !(s->CompartmentList.last()->getEnd()->isTerminal())
              ) ||
             ( s->getParents().isEmpty() &&
               !(s->CompartmentList.first()->getStart()->isRoot())
              )
            ) {
             foreach(Segment* cs, this->Segments) {
                 if(s->CompartmentList.last()->getEnd() ==
                    cs->CompartmentList.first()->getStart()){
                     s->addChild(cs);
                 }
                 if(s->CompartmentList.first()->getStart() ==
                    cs->CompartmentList.last()->getEnd()){
                     s->addParent(cs);
                 }
             }
         }
     }
 }

 bool TubeTree::writeVtkPoly(QString filename){
     if(!(this->populated)) return false;
     int size_of_lines = 0;
     foreach(Segment* s, this->Segments) {
         size_of_lines += s->CompartmentList.size();
     }
     //Now add the segments.size() for entry : numPoints of each lines field
     //Also one more segments.size() for endPoint() of last compartment
     size_of_lines += 2*this->Segments.size();
     QFile file(filename);
         file.open(QIODevice::WriteOnly | QIODevice::Text);
     QTextStream vtkfile(&file);
     //Write VTK Header
     vtkfile << "# vtk DataFile Version "
             << vtk_major_version << "."
             << vtk_minor_version << endl;
     vtkfile << "eXample File" <<endl; //Can be an input argument
     vtkfile << "ASCII" << endl;

     vtkfile <<endl;

     vtkfile << "DATASET POLYDATA" <<endl;
     vtkfile << "POINTS "
             << this->NodeList.size()
             << " double" << endl;
     QVector<int> id_list;
     QMap<int, Node*>::const_iterator iter;
     for(iter = this->NodeList.begin();
         iter != this->NodeList.end();
         ++iter){
         vtkfile << iter.value()->getX() << " "
                 << iter.value()->getY() << " "
                 << iter.value()->getZ() << endl;
         id_list.append(iter.value()->getID());
     }

     vtkfile <<endl;

     vtkfile << "LINES " << this->Segments.size()
             << " " << size_of_lines << endl;
     foreach(Segment* s, this->Segments) {
         int numPoints=0;
         QString Line="";
         foreach(Compartment* c, s->CompartmentList) {
             Line += QString::number(id_list.indexOf(c->getStart()->getID()));
             Line += " ";
             numPoints++;
             if(c == s->CompartmentList.last() ){
                 Line += QString::number(id_list.indexOf(c->getEnd()->getID()));
                 numPoints++;
                 vtkfile << numPoints << " " << Line << endl;
                 Line = "";
                 numPoints=0;
             }
         }
     }

     vtkfile <<endl;

     vtkfile << "POINT_DATA " << this->NodeList.size() << endl;
     vtkfile << "SCALARS Radius+Type double 2" <<endl;
     vtkfile << "LOOKUP_TABLE default" <<endl;
     for(iter = this->NodeList.begin();
         iter != this->NodeList.end();
         ++iter){
         vtkfile << iter.value()->getRadius() << " "
                 << iter.value()->getType() << endl;
     }
     file.close();
     return true;
 }
