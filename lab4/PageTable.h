#include<iostream>
#include <unordered_set>
using namespace std;

class PageTable{
    public:
    int pageSize;
    int tableSize;
    vector<int> st;
    PageTable(int pageSize,int tableSize){
        this->pageSize = pageSize;
        this->tableSize = tableSize;
    }
    
};