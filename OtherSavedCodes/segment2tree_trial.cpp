// void TubeTree::makeTrees() {
//     QVector<Segment*> traversedSegments;
//     traversedSegments.clear();
//     for(int cnt=0;cnt<this->num_trees;cnt++){
//         SegmentList* tree = new SegmentList;
//         foreach(Segment* s, this->Segments){
//             if(s->CompartmentList.first()->getStart()->isRoot() &&
//                     traversedSegments.count(s) < s->CompartmentList.first()->getStart()->getNBifs())
//             {
//                 if(traversedSegments.count(s) == 0) {
//                     s->setRoot(true);
//                     tree->setRoot(s);
//                     //tree->setParent(NULL); - Implicit
//                 }
//                 traversedSegments.append(s);
//                 Segment* prev_segment = s;
//                 foreach(Segment* next, this->Segments){
//                     Segment* curr_segment = next;
//                     if(prev_segment->CompartmentList.last()->getEnd() ==
//                             curr_segment->CompartmentList.first()->getStart()){
//                         tree->addChild(curr_segment);
//                         if(curr_segment->CompartmentList.last()->getEnd()->isTerminal())
//                             break;
//                         prev_segment = curr_segment;
//                     }
//                 }
//             }
//             this->Trees.append(tree);
//         }
//     }
// }
