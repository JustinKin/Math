#include <iostream>
#include <fstream>
#include "point2face.h"

using namespace std;

void Print(Calculater &cal_, int &i_)
{
    cout << "\n��" << i_ << "����\n";
    cout << "ƽ�淨����������\n";
    cout << "A = " << cal_.A << "\n"
         << "B = " << cal_.B << "\n"
         << "C = " << cal_.C << "\n"
         << "D = " << cal_.D << "\n";
    cout << "ƽ�����ĵ����꣺\n";
    cout << "x0 = " << cal_.x_face << "\n"
         << "y0 = " << cal_.y_face << "\n"
         << "z0 = " << cal_.z_face << "\n";
    cout << "�������꣺\n";
    cout << "x1 = " << cal_.x_ball << "\n"
         << "y1 = " << cal_.y_ball << "\n"
         << "z1 = " << cal_.z_ball << "\n";
    cout << "���ĵ�ƽ��ľ���Ϊ��" << cal_.distance << "\n\n";
}

void Write(Calculater &cal_, int &i_, ofstream &out_)
{//cout<<"1";
    /*out_ << "��" << i_ << "����\n";
    out_ << "ƽ�淨����������\n";
    out_ << "A = " << cal_.A << "\n"
         << "B = " << cal_.B << "\n"
         << "C = " << cal_.C << "\n"
         << "D = " << cal_.D << "\n";
    out_ << "ƽ�����ĵ����꣺\n";
    out_ << "x0 = " << cal_.x_face << "\n"
         << "y0 = " << cal_.y_face << "\n"
         << "z0 = " << cal_.z_face << "\n";
    out_ << "�������꣺\n";
    out_ << "x1 = " << cal_.x_ball << "\n"
         << "y1 = " << cal_.y_ball << "\n"
         << "z1 = " << cal_.z_ball << "\n";
    out_ << "���ĵ�ƽ��ľ���Ϊ��" << cal_.distance << "\n\n";
    */
    out_ <<cal_.distance << "\n";

    //cout<<"2";
}
