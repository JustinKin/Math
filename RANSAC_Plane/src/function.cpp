#include <iostream>
#include <vector>
#include <fstream>
#include <ctime>
#include <random>
#include <math.h>
// #include <algorithm>
#include "PLANECONSENSUS.h"

using namespace std;
using namespace Eigen;


void Get3Points(PlaneCoefficients &plane_, int &i_)
{

    int i = i_;
    while(1)
    {

        random_device rde;
        default_random_engine e(rde()+608*i);
        //default_random_engine e(time(0)+608*i);

        uniform_int_distribution<unsigned> rd(0, plane_.pcd.size() - 1);
        unsigned p1=rd(e), p2=rd(e), p3=rd(e);

        Eigen::Matrix3f D_tmp;
        D_tmp <<plane_.pcd[p1].x, plane_.pcd[p1].y, plane_.pcd[p1].z,
                plane_.pcd[p2].x, plane_.pcd[p2].y, plane_.pcd[p2].z,
                plane_.pcd[p3].x, plane_.pcd[p3].y, plane_.pcd[p3].z;
        Eigen::Vector3f a(D_tmp(0,0)-D_tmp(1,0), D_tmp(0,1)-D_tmp(1,1), D_tmp(0,2)-D_tmp(1,2)),
                        b(D_tmp(2,0)-D_tmp(1,0), D_tmp(2,1)-D_tmp(1,1), D_tmp(2,2)-D_tmp(1,2));
        auto c = a.cross(b);

        if(c.dot(c) !=0 && Distance(D_tmp, plane_))
        {
            // cout<<"2"<<endl;
            plane_.D_in <<  plane_.pcd[p1].x, plane_.pcd[p1].y, plane_.pcd[p1].z,
                            plane_.pcd[p2].x, plane_.pcd[p2].y, plane_.pcd[p2].z,
                            plane_.pcd[p3].x, plane_.pcd[p3].y, plane_.pcd[p3].z;
            break;
        }
        ++i;
        // cout<<"c: "<<c<<endl;
        // cout<<plane_.D_in<<endl;
        // int text;
        // cin>>text;

    }
}


void ComputePlane(std::string &filename_,PlaneCoefficients &plane_)
{
    ifstream in;
    in.open(filename_);
    Point p;
    while(!in.eof())
    {
        in >> p.x >> p.y >> p.z;
        plane_.pcd.push_back(p);
    }
    in.close();
    // cout<<"1"<<endl;
    for(int i =0; i<plane_.iter; ++i)
        {
            Get3Points(plane_, i);
            //compute plane coefficients
            Eigen::Vector3f
                a(plane_.D_in(0,0)-plane_.D_in(1,0), plane_.D_in(0,1)-plane_.D_in(1,1), plane_.D_in(0,2)-plane_.D_in(1,2)),
                b(plane_.D_in(1,0)-plane_.D_in(2,0), plane_.D_in(1,1)-plane_.D_in(2,1), plane_.D_in(1,2)-plane_.D_in(2,2));
            auto normal_vector = a.cross(b);//A,B,C of plane
            float d = -(normal_vector[0] * plane_.D_in(0,0) + normal_vector[1] * plane_.D_in(0,1) + normal_vector[2] * plane_.D_in(0,2)) ;// D = -(Ax0 + By0 + Cz0)

            int ins = 0;
            float rad = 0;
            vector <Point> pcd_tmp;
            for(auto c : plane_.pcd)
            {
                Eigen::Vector3f tmp(c.x, c.y, c.z);
                rad = (abs( normal_vector.dot(tmp) + d )) / (sqrt( normal_vector.dot(normal_vector) ));
                if(rad <= plane_.threshold)
                {
                    pcd_tmp.push_back(c);
                    ++ins;
                }
            }
            if(ins > plane_.inliers)
            {
                plane_.coefficients << normal_vector[0], normal_vector[1], normal_vector[2], d;//A,B,C,D of plane
                plane_.inliers = ins;
                plane_.pcd_in.resize(pcd_tmp.size());
                swap(pcd_tmp, plane_.pcd_in);
            }
            double its = 0;
            its = (log(1-plane_.probability)) / (log(1-(pow((plane_.inliers/plane_.pcd.size()),4))));
            //update iter
            if(plane_.inliers >= (plane_.threshold_iter*plane_.pcd.size()))
                {
                    cout<<"iter_i : "<< i<<"\n";
                    break;
                }
            if(its < plane_.iter && its >0)
                plane_.iter = its;
        }
        plane_.coefficients *= 1000;
        cout << plane_.coefficients<<endl<<endl;
        cout<<"iter : "<< plane_.iter<<"\n"<<
              "pcd :"<< plane_.pcd.size()<<
              "\npcd_in :" << plane_.pcd_in.size()<< endl;


}

void ShowResult(std::string &filename_,PlaneCoefficients &plane_);

bool Distance(Eigen::Matrix3f &D_,PlaneCoefficients &plane_)
{
    Eigen::Vector3f A, B;
    for(int i = 0 ,j = 1; i<2; ++j)
    {
        A << D_(i,0), D_(i,1), D_(i,2);
        B << D_(j,0), D_(j,1), D_(j,2);
        if(sqrt((A-B).dot(A-B)) < plane_.threshold_dst)
            return false;
        if(j == 2)
        {
            ++i;
            j = i ;//watch out , ++j in the end , needn't +1
        }
    }
    return true;
}

/* void OptimizeModel(PlaneCoefficients &plane_)
{
    Eigen::Matrix4f D_tmp;
    auto p = plane_.pcd_in.begin();
    int amount = ( plane_.pcd_in.size() - plane_.pcd_in.size()%4 );
    cout<<"\n\nthe plane_.pcd_in.size() : "<< plane_.pcd_in.size() <<"\n";
    cout<<"the amount : "<< amount <<"\n";

    for(int i = 0 ; (i+4) < amount; ++i, ++p )
    {
        D_tmp <<     (*p).x,     (*p).y,     (*p).z, 1,
                 (*(p+1)).x, (*(p+1)).y, (*(p+1)).z, 1,
                 (*(p+2)).x, (*(p+2)).y, (*(p+2)).z, 1,
                 (*(p+3)).x, (*(p+3)).y, (*(p+3)).z, 1;
        if(abs(D_tmp.determinant()) !=0 && Distance(D_tmp, plane_))
        {
            // cout<<"D_tmp : \n"<< D_tmp <<"\n";
            // for(;;){ }

            for(int k = 0,j=1; k<3; ++k,++j)
            {
                Eigen::Vector3f
                    point_k(D_tmp(k,0),D_tmp(k,1),D_tmp(k,2)),
                    point_j(D_tmp(j,0),D_tmp(j,1),D_tmp(j,2));
                plane_.d(k,0) = 0.5 * (point_j.dot(point_j) - point_k.dot(point_k));
                plane_.D.row(k) = (point_j - point_k);
            }
            plane_.Dx = plane_.D;
            plane_.Dx.col(0) = plane_.d;
            plane_.Dy = plane_.D;
            plane_.Dy.col(1) = plane_.d;
            plane_.Dz = plane_.D;
            plane_.Dz.col(2) = plane_.d;
            //Cramer's Rule
            float d = (plane_.D.determinant());
            plane_.coe[0] = (plane_.Dx.determinant()) / d;
            plane_.coe[1] = (plane_.Dy.determinant()) / d;
            plane_.coe[2] = (plane_.Dz.determinant()) / d;
            Eigen::Vector3f
                vcoe((plane_.coe[0]-D_tmp(0,0)),(plane_.coe[1]-D_tmp(0,1)),(plane_.coe[2]-D_tmp(0,2)));
            plane_.coe[3] = sqrt(vcoe.dot(vcoe));
            plane_.coe_o.push_back(plane_.coe);
            // plane_.coe *= 1000;
            // plane_.coe[3] *= 2;
            // cout<<"plane_.coe : \n"<< plane_.coe <<"\n";
            // for(;;){ }

        }
    }
    // accumulate(plane_.coe_o.begin(),plane_.coe_o.end(),0);
    Eigen::Vector4f coe_sum;
    for(auto c : plane_.coe_o)
    {
        coe_sum += c;
    }
    plane_.coefficients = coe_sum / plane_.coe_o.size();
    plane_.coefficients *= 1000;
    plane_.coefficients[3] *= 2;
    cout<<"\n\nthe optimize plane : \n"<< plane_.coefficients <<"\n";
    cout<<"the coe NO : \n"<< plane_.coe_o.size() <<"\n";
    cout<<"plane_.coe_o[0] : \n"<< plane_.coe_o[0] <<"\n";

    //TODO:最小二乘
    //球心基于球的三维正态分布


} */