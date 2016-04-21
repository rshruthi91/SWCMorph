#ifndef NODE_H
#define NODE_H

#include "common.h"

class Node
{
protected:
  //You dont want people messing around here
  double x;
  double y;
  double z;
  double radius;
  int type;
public:
  //Constructor takes the coordinates
  Node(double pos_x, double pos_y, double pos_z);
  //todo: add other ways to take the coordinates

  //Functions to manipulate internal variables
  inline void setposX(double pos_x) {this->x = pos_x;}
  inline void setposY(double pos_y) {this->y=pos_y;}
  inline void setposZ(double pos_z) {this->z=pos_z;}
  inline void setRadius(double r) {this->radius=r;}
  inline void setType(double t) {this->type=t;}

  //Functions to see internal variable values
  inline double getX() {return this->x;}
  inline double getY() {return this->y;}
  inline double getZ() {return this->z;}
  inline double getR() {return this->radius;}

  //Get the Euclidean distance to another node
  double eucldist(Node *toNode);
};

#endif // NODE_H
