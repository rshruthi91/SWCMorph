#include "node.h"

//Node::Node(int node_id, int node_type, double pos_x, double pos_y, double pos_z, double node_radius, int parent_id)
//{
//    this->x = pos_x;
//    this->y = pos_y;
//    this->z = pos_z;
//    this->id = node_id;
//    this->radius = node_radius;
//    this->pid = parent_id;
//    this->type = node_type;
//}


Node::Node()
{
    this->x = 0.0;
    this->y = 0.0;
    this->z = 0.0;
    this->radius = 0.0;
    this->type = 0;
    this->id = 0;
    this->pid = 0;
    this->branch = false;
    this->root = false;
    this->terminal = false;
    this->num_bifs = 0;
}

double Node::eucldist(Node *toNode) {
  double out=0.0;
  out =  std::sqrt(std::pow(absdiff(toNode->getX(),this->x),2)+
                   std::pow(absdiff(toNode->getY(),this->y),2)+
                   std::pow(absdiff(toNode->getZ(),this->z),2));
  return out;
}
