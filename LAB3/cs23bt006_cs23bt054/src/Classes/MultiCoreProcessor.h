#include<iostream>
#include<algorithm>
#include<vector>
#include"Process.h"
using namespace std;
class MultiCoreProcessor{
    public:
    void FIFO(vector<Process*> parsed_data,char* filename);
    void NPSJF(vector<Process*> parsed_data,char* filename);
    void PSJF(vector<Process*> parsed_data,char* filename);
    void RR(vector<Process*> parsed_data,char* filename,int timeInterval);
};