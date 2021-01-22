#include <iostream>
#include <fstream>
#include "point2face.h"

using namespace std;

void Print(Calculater &cal_, int &i_)
{
    cout << "\n第" << i_ << "个球\n";
    cout << "平面法向量参数：\n";
    cout << "A = " << cal_.A << "\n"
         << "B = " << cal_.B << "\n"
         << "C = " << cal_.C << "\n"
         << "D = " << cal_.D << "\n";
    cout << "平面中心点坐标：\n";
    cout << "x0 = " << cal_.x_face << "\n"
         << "y0 = " << cal_.y_face << "\n"
         << "z0 = " << cal_.z_face << "\n";
    cout << "球心坐标：\n";
    cout << "x1 = " << cal_.x_ball << "\n"
         << "y1 = " << cal_.y_ball << "\n"
         << "z1 = " << cal_.z_ball << "\n";
    cout << "球心到平面的距离为：" << cal_.distance << "\n\n";
}

void Write(Calculater &cal_, int &i_, ofstream &out_)
{//cout<<"1";
    /*out_ << "第" << i_ << "个球\n";
    out_ << "平面法向量参数：\n";
    out_ << "A = " << cal_.A << "\n"
         << "B = " << cal_.B << "\n"
         << "C = " << cal_.C << "\n"
         << "D = " << cal_.D << "\n";
    out_ << "平面中心点坐标：\n";
    out_ << "x0 = " << cal_.x_face << "\n"
         << "y0 = " << cal_.y_face << "\n"
         << "z0 = " << cal_.z_face << "\n";
    out_ << "球心坐标：\n";
    out_ << "x1 = " << cal_.x_ball << "\n"
         << "y1 = " << cal_.y_ball << "\n"
         << "z1 = " << cal_.z_ball << "\n";
    out_ << "球心到平面的距离为：" << cal_.distance << "\n\n";
    */
    out_ <<cal_.distance << "\n";

    //cout<<"2";
}
