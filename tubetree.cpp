#include "tubetree.h"
#include "float.h"

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
    this->vol_idx = 100;
    this->vol_idy = 100;
    this->vol_idz = 100;
    infileInfo.setFile(swcfile);
    qDeleteAll(this->NodeList);
    this->NodeList.clear();
    qDeleteAll(this->CompartmentList);
    this->CompartmentList.clear();
    qDeleteAll(this->Segments);
    this->Segments.clear();
    qDeleteAll(this->RootSegList);
    this->RootSegList.clear();
    this->Params =this->Json;
    this->filebasename = "";

    globalstatsdone=false;

    this->avg_segvolcyl = -1;
    this->min_segvolcyl = DBL_MAX;
    this->max_segvolcyl = -1;

    this->avg_compvolcyl = -1;
    this->min_compvolcyl = DBL_MAX;
    this->max_compvolcyl = -1;

    this->avg_segvolfrstm = -1;
    this->min_segvolfrstm = DBL_MAX;
    this->max_segvolfrstm = -1;

    this->avg_compvolfrstm = -1;
    this->min_compvolfrstm = DBL_MAX;
    this->max_compvolfrstm = -1;

    this->avg_segsurfcyl = -1;
    this->min_segsurfcyl = DBL_MAX;
    this->max_segsurfcyl = -1;

    this->avg_compsurfcyl = -1;
    this->min_compsurfcyl = DBL_MAX;
    this->max_compsurfcyl = -1;

    this->avg_segsurffrstm = -1;
    this->min_segsurffrstm = DBL_MAX;
    this->max_segsurffrstm = -1;

    this->avg_compsurffrstm = -1;
    this->min_compsurffrstm = DBL_MAX;
    this->max_compsurffrstm = -1;

    this->total_cylvol = -1;
    this->total_frstmvol = -1;
    this->total_cylsurf = -1;
    this->total_frstmsurf = -1;

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
         if(swcline.isEmpty()) continue;
         QStringList swcnodeparams = swcline.split(" ",QString::SkipEmptyParts);
         if(swcnodeparams[0].at(0) != '#'){
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
                 //I have to support trifurcations due to bad tracing algo inputs
                 //Why do I not have my own traacing algo again?
//                 if(num_bifs > 2 && (swcnode->getType()!= 1)){
//                     qDebug() << "Trifurcation found - unsupported";
//                     qDebug() << "Node number: " << swcnode->getID();
//                     return false;
//                 }
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
     vtkfile << "SCALARS Radius double" <<endl;
     vtkfile << "LOOKUP_TABLE default" <<endl;
     for(iter = this->NodeList.begin();
         iter != this->NodeList.end();
         ++iter){
         vtkfile << iter.value()->getRadius() << endl;
     }
     vtkfile << "SCALARS TypeID int" <<endl;
     vtkfile << "LOOKUP_TABLE default" <<endl;
     for(iter = this->NodeList.begin();
         iter != this->NodeList.end();
         ++iter){
         vtkfile << iter.value()->getType() << endl;
     }
     file.close();
     return true;
 }

 void TubeTree::getSegStats(){
     double total_cylvol=0;
     double total_cylsurf=0;
     double total_frstmvol=0;
     double total_frstmsurf=0;

     long NSegments = (this->Segments.size());

     foreach(Segment *s,this->Segments){
         s->updateParams();
         double svolcyl = s->cylVolume;
         double svolfrstm = s->frstmVolume;
         double ssurfcyl = s->cylSurface;
         double ssurffrstm = s->frstmSurface;

         total_cylvol +=svolcyl;
         total_frstmvol +=svolfrstm;
         total_cylsurf += ssurfcyl;
         total_frstmsurf += ssurffrstm;

         if(svolcyl < this->min_segvolcyl && svolcyl> 0)
             this->min_segvolcyl = svolcyl;
         if(svolcyl > max_segvolcyl)
             this->max_segvolcyl = svolcyl;
         if(svolfrstm < this->min_segvolfrstm && svolfrstm > 0)
             this->min_segvolfrstm = svolfrstm;
         if(svolfrstm > this->max_segvolfrstm)
             this->max_segvolfrstm = svolfrstm;

         if(ssurfcyl < this->min_segsurfcyl && ssurfcyl>0)
             this->min_segsurfcyl = ssurfcyl;
         if(ssurfcyl > max_segsurfcyl)
             this->max_segsurfcyl = ssurfcyl;
         if(ssurffrstm < this->min_segsurffrstm && ssurffrstm>0)
             this->min_segvolfrstm = ssurffrstm;
         if(ssurffrstm > this->max_segsurffrstm)
             this->max_segsurffrstm = ssurffrstm;
     }

     if(!this->globalstatsdone){
         this->total_cylvol = total_cylvol;
         this->total_cylsurf = total_cylsurf;
         this->total_frstmsurf = total_frstmsurf;
         this->total_frstmvol = total_frstmvol;
         this->globalstatsdone = true;
     }

     this->avg_segvolcyl=total_cylvol/NSegments;
     this->avg_segvolfrstm=total_frstmvol/NSegments;
     this->avg_segsurfcyl=total_cylsurf/NSegments;
     this->avg_segsurffrstm=total_frstmsurf/NSegments;
 }

 void TubeTree::getCompStats(){
     double total_cylvol=0;
     double total_cylsurf=0;
     double total_frstmvol=0;
     double total_frstmsurf=0;

     long NCompartments = (this->CompartmentList.size());

     foreach(Compartment *s,this->CompartmentList){
         s->updateParams();
         double svolcyl = s->cylVolume;
         double svolfrstm = s->frstmVolume;
         double ssurfcyl = s->cylSurface;
         double ssurffrstm = s->frstmSurface;

         total_cylvol +=svolcyl;
         total_frstmvol +=svolfrstm;
         total_cylsurf += ssurfcyl;
         total_frstmsurf += ssurffrstm;

         if(svolcyl < this->min_compvolcyl && svolcyl>0)
             this->min_compvolcyl = svolcyl;
         if(svolcyl > max_compvolcyl)
             this->max_compvolcyl = svolcyl;
         if(svolfrstm < this->min_compvolfrstm && svolfrstm>0)
             this->min_compvolfrstm = svolfrstm;
         if(svolfrstm > this->max_compvolfrstm)
             this->max_compvolfrstm = svolfrstm;

         if(ssurfcyl < this->min_compsurfcyl && ssurfcyl>0)
             this->min_compsurfcyl = ssurfcyl;
         if(ssurfcyl > max_compsurfcyl)
             this->max_compsurfcyl = ssurfcyl;
         if(ssurffrstm < this->min_compsurffrstm && ssurffrstm>0)
             this->min_compvolfrstm = ssurffrstm;
         if(ssurffrstm > this->max_compsurffrstm)
             this->max_compsurffrstm = ssurffrstm;
     }

     if(!this->globalstatsdone){
         this->total_cylvol = total_cylvol;
         this->total_cylsurf = total_cylsurf;
         this->total_frstmsurf = total_frstmsurf;
         this->total_frstmvol = total_frstmvol;
         this->globalstatsdone = true;
     }

     this->avg_compvolcyl=total_cylvol/NCompartments;
     this->avg_compvolfrstm=total_frstmvol/NCompartments;
     this->avg_compsurfcyl=total_cylsurf/NCompartments;
     this->avg_compsurffrstm=total_frstmsurf/NCompartments;
 }

 bool TubeTree::writeJson(QString filename)
 {
     getCompStats();
     getSegStats();
     this->Params = (filename.split('.').last() == "json")?
                           this->Json:
                           this->Binary;

     /*QFile saveFile(this->Params == this->Json
             ? QStringLiteral("TubeTree.json")
             : QStringLiteral("TubeTree.dat"));*/

     QFile saveFile(filename);

     if (!saveFile.open(QIODevice::WriteOnly)) {
             qWarning("Couldn't open save file.");
             return false;
         }

     QJsonObject json;

     QJsonObject SegmentObject;
     QJsonArray cylVolSeg,cylsurfSeg,frstmvolSeg,frstmsurfSeg;
     cylVolSeg    = {this->min_segvolcyl   ,this->max_segvolcyl   ,this->avg_segvolcyl};
     cylsurfSeg   = {this->min_segsurfcyl  ,this->max_segsurfcyl  ,this->avg_segsurfcyl};
     frstmvolSeg  = {this->min_segvolfrstm ,this->max_segvolfrstm ,this->avg_segvolfrstm};
     frstmsurfSeg = {this->min_segsurffrstm,this->max_segsurffrstm,this->avg_segsurffrstm};
     SegmentObject["cylVol"]    = cylVolSeg;
     SegmentObject["cylSurf"]   = cylsurfSeg;
     SegmentObject["frstmVol"]  = frstmvolSeg;
     SegmentObject["frstmSurf"] = frstmsurfSeg;

     QJsonObject CompartmentObject;
     QJsonArray cylVolComp,cylsurfComp,frstmvolComp,frstmsurfComp;
     cylVolComp    = {this->min_compvolcyl   ,this->max_compvolcyl   ,this->avg_compvolcyl};
     cylsurfComp   = {this->min_compsurfcyl  ,this->max_compsurfcyl  ,this->avg_compsurfcyl};
     frstmvolComp  = {this->min_compvolfrstm ,this->max_compvolfrstm ,this->avg_compvolfrstm};
     frstmsurfComp = {this->min_compsurffrstm,this->max_compsurffrstm,this->avg_compsurffrstm};
     CompartmentObject["cylVol"]    = cylVolComp;
     CompartmentObject["cylSurf"]   = cylsurfComp;
     CompartmentObject["frstmVol"]  = frstmvolComp;
     CompartmentObject["frstmSurf"] = frstmsurfComp;

     json["Segments"]     = SegmentObject;
     json["Compartments"] = CompartmentObject;

     json["total_cylvol"]    = this->total_cylvol;
     json["total_cylsurf"]   = this->total_cylsurf;
     json["total_frstmvol"]  = this->total_frstmvol;
     json["total_frstmsurf"] = this->total_frstmsurf;

     json["vol_idz"] = this->vol_idz;
     json["vol_idy"] = this->vol_idy;
     json["vol_idx"] = this->vol_idx;

     QJsonDocument saveDoc(json);
     saveFile.write(this->Params == this->Json
         ? saveDoc.toJson()
         : saveDoc.toBinaryData());

     return true;
 }
