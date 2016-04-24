#ifndef NODE_H
#define NODE_H

#include "common.h"

class Node
{

public:
  Node();
  //Constructor takes the coordinates
  //Node(int node_id, int node_type, double pos_x, double pos_y, double pos_z, double radius, int parent_id);
  //todo: add other ways to take the coordinates

protected:
  //You dont want people messing around here
  double x;
  double y;
  double z;
  double radius;
  int type;
  int id;
  int pid;
  bool branch;
  bool root;
  bool terminal;
  int num_bifs;

public:
  //Functions to manipulate internal variables
  inline void setposX(double pos_x) {this->x = pos_x;}
  inline void setposY(double pos_y) {this->y=pos_y;}
  inline void setposZ(double pos_z) {this->z=pos_z;}
  inline void setRadius(double r)   {this->radius=r;}
  inline void setType(int t)     {this->type=t;}
  inline void setID(int node_id)     {this->id = node_id;}
  inline void setPID(int parent_id)     {this->pid = parent_id;}

  //Functions to see internal variable values
  inline double getX() {return this->x;}
  inline double getY() {return this->y;}
  inline double getZ() {return this->z;}
  inline double getRadius() {return this->radius;}
  inline int getNBifs() {return this->num_bifs;}
  inline int getType() {return this->type;}
  inline int getID()   {return this->id;}
  inline int getPID()  {return this->pid;}

  //Check the status of the node
  inline bool isBranch()   {return this->branch;}
  inline bool isRoot()     {return this->root;}
  inline bool isTerminal() {return this->terminal;}

  //Update status of node
  inline void setBranch(bool br)     {this->branch = br;}
  inline void setRoot(bool rt)       {this->root = rt;}
  inline void setTerminal(bool term) {this->terminal = term;}
  inline void setNBifs(int nbifs) {this->num_bifs = nbifs;}

  //Get the Euclidean distance to another node
  double eucldist(Node *toNode);
};

#endif // NODE_H
