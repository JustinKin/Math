/*
                           _ooOoo_
                          o8888888o
                          88" . "88
                          (| -_- |)
                          O\  =  /O
                       ____/`---'\____
                     .'  \\|     |//  `.
                    /  \\|||  :  |||//  \
                   /  _||||| -:- |||||-  \
                   |   | \\\  -  /// |   |
                   | \_|  ''\---/''  |   |
                   \  .-\__  `-`  ___/-. /
                 ___`. .'  /--.--\  `. . __
              ."" '<  `.___\_<|>_/___.'  >'"".
             | | :  `- \`.;`\ _ /`;.`/ - ` : | |
             \  \ `-.   \_ __\ /__ _/   .-` /  /
        ======`-.____`-.___\_____/___.-`____.-'======
                           `=---='
        ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
        	一心向善	好运相伴	佛祖保佑	永无BUG
*/
#include <iostream>
#include <string>
#include <vector>
#include <chrono>
#include <algorithm>
#include <fstream>
#include <math.h>

#include "point2face.h"

using namespace std;

int main(int argc, char *argv[])
{
   cout << "  Mission Start:  \n\n";
   auto start = chrono::high_resolution_clock::now();
   //=================================================================//

   cout << "创建文件...\n";
   string ofile, ifile;
   cin>>ofile;
   ofile += ".txt";
   cout << "输出数据文件为："<<ofile<<"\n";
   ofstream out; //write mode ,frome memory to file
   ifstream in;
   Calculater cal;
   int i = 1,j=0;
   cout << "balls NO. :";
   cin >>j;
   cout << "原始数据文件: ";
   cin>>ifile;
   ifile += ".txt";
   in.open(ifile);
   while (1)
   {
      out.open(ofile, ofstream::app);//TODO:放在循环外面写入失败，原因不明

      /*
      cout << "请输入平面法向量：（A，B，C）\n";
      cin >> cal.A >> cal.B >> cal.C;
      cout << "请输入平面的中心点坐标：（x0,y0,z0）\n";
      cin >> cal.x_face >> cal.y_face >> cal.z_face;
      cal.D = -(cal.A * cal.x_face + cal.B * cal.y_face + cal.C * cal.z_face);
      cout << "请输入球心坐标：（x1,y1,z1）\n";
      cin >> cal.x_ball >> cal.y_ball >> cal.z_ball;
      */
      in >> cal.A >> cal.B >> cal.C;
      in >> cal.x_face >> cal.y_face >> cal.z_face;
      cal.D = -(cal.A * cal.x_face + cal.B * cal.y_face + cal.C * cal.z_face);
      in >> cal.x_ball >> cal.y_ball >> cal.z_ball;

      cal.distance = (abs(cal.A * cal.x_ball + cal.B * cal.y_ball + cal.C * cal.z_ball + cal.D)) / (sqrt(cal.A * cal.A + cal.B * cal.B + cal.C * cal.C));

      //Print(cal, i);
      Write(cal, i, out);
      out.close();

      if(i == j)
         break;
      ++i;
   }in.close();

   //=================================================================//
   cout << "\n\n";
   auto end = chrono::high_resolution_clock::now();
   cout << "Elapsed Time:  " << std::chrono::duration<double>(end - start).count() << " s\n\n"
        << "**Mission Completed !**\n\n\n"
        << "\a";
   return 0;
}
