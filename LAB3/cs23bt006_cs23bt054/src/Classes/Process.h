#include<iostream>
#include<algorithm>
#include<vector>

using namespace std;
class Process{
    public:
    int p_no;
    int arrival;
    int completion;
    int index;
    int wait;
    vector<int> bursts;
    Process(){
        p_no = 0;
        index = 0;
    };
};