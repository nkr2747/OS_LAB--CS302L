#include<iostream>
#include<algorithm>
#include<vector>
#include"Process.h"
using namespace std;

class Processor{
    public:
    void FIFO(vector<Process*> parsed_data,char* filename);
    void NPSJF(vector<Process*> parsed_data,char* filename);
    void PSJF(vector<Process*> parsed_data,char* filename);
    void RR(vector<Process*> parsed_data,char* filename,int timeInterval);
};

class MultiCoreProcessor{
    public:
    void FIFO(vector<Process*> parsed_data,char* filename);
    void NPSJF(vector<Process*> parsed_data,char* filename);
    void PSJF(vector<Process*> parsed_data,char* filename);
    void RR(vector<Process*> parsed_data,char* filename,int timeInterval);
};
// class FIFO{
//     public:
//     void run(vector<Process*> parsed_data,char* filename);
// };
// class NPSJF{
//     public:
//     void run(vector<Process*> parsed_data,char* filename);
// };
// class PSJF{
//     public:
//     void run(vector<Process*> parsed_data,char* filename);
// };
// class RR{
//     public:
//     void run(vector<Process*> parsed_data,char* filename, int timeInterval);
// };