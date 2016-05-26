#ifndef COMMON
#define COMMON

#include <cmath>
#include <stdlib.h>
#include <iostream>


#include <QLinkedList>
#include <QTextStream>
#include <QFile>
#include <QDebug>
#include <QVector>
#include <QFileInfo>

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

# define PI 3.14159265358979323846  /* pi */

//Argument Parameters to the executable can be defined here


//Inline this function to avoid multiple definitions
inline double absdiff(double a, double b) {
  if(a>b) return a-b;
  else return b-a;
}

#endif // COMMON

