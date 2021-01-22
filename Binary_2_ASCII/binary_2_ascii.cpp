#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <sstream>

using namespace std;

int main(int argc, char *argv[])
{
    string  infilename = argv[1],
            line;
    ifstream infile(infilename,ios::binary);
    if(!infile)
    {
        cout<< "* open file error *" <<endl;
        return 0;
    }

    string  outfilename = infilename;
            outfilename[outfilename.size()-1] = 't';
            outfilename[outfilename.size()-2] = 'x';
            outfilename[outfilename.size()-3] = 't';
    ofstream outfile(outfilename);

   float fea[2903];
    int n=0;
    while(infile.read((char *)&fea[0], 12))
    {
        n++;
    }
    cout<<n<<endl;
    for(int i=0;i<2903;i++)
        cout<<fea[i]<<" ";













    infile.close();
    outfile.close();



    return 0;
}