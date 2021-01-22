#ifndef __POINT2FACE_H__
#define __POINT2FACE_H__

#include <iostream>
#include <fstream>

struct Calculater
{
    double A =0, B=0, C=0, D=0;
    double x_ball=0, y_ball =0, z_ball=0;
    double x_face=0, y_face=0, z_face=0;
    double distance = 0;
};

void Print(Calculater & ,int & );
void Write(Calculater & , int &, std::ofstream &);


#endif