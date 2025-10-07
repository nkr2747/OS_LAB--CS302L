#include<bits/stdc++.h>
using namespace std;

class PageTable
{
public:
    int64_t pageSize;
    int64_t pageFault;
    int64_t pageHit;
    unordered_map<int64_t, int64_t> pageTable;
    PageTable()
    {
        pageHit = 0;
        pageFault = 0;
    }
    PageTable(int pageSize)
    {
        this->pageSize = pageSize;
        pageFault = 0;
        pageHit = 0;
    }
    void setPageSize(int pageSize)
    {
        this->pageSize = pageSize;
    }
    bool isMapped(int64_t page_no)
    {
        return pageTable.find(page_no) != pageTable.end();
    }
};