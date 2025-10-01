#include <iostream>
#include <unordered_set>
#include <vector>
#include <queue>
#include <unordered_map>
#include <fstream>
#include <string>
#include <cstdlib>
#include <cstdint>
using namespace std;

queue<int> freeFrames;

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

int main(int argc, char *argv[])
{
    int pageSize = atoi(argv[1]);
    int totFrames = atoi(argv[2]);
    queue<int64_t> freeFrames;
    for (int i = 0; i < totFrames; i++)
    {
        freeFrames.push(i);
    }
    //---------(p_no, page_no)-------
    queue<pair<int64_t, int64_t>> q; // queue hai global ke liye, process_no,page_no
    PageTable proc[4];
    for (int i = 0; i < 4; i++)
    {
        proc[i].setPageSize(pageSize);
    }
    ifstream file;
    file.open("combined.trace");
    ofstream output;
    output.open("result.txt");
    string line = "";
    int64_t p_no;
    int64_t virtual_address;
    while (getline(file, line, ','))
    {
        // cout <<"P_No. "<< line <<" ";
        p_no = stoll(line);
        getline(file, line);
        virtual_address = stoll(line);
        // cout <<"Virtual Address:- "<< line <<endl;

        // p_no or VA mil chuka hai ab check karna hai pehle se pageTable me h kya?
        int64_t page_no = virtual_address / pageSize;
        if (!proc[p_no].isMapped(page_no))
        {
            // is section me aye mtlb miss hua hai
            output << "Miss" << endl;
            proc[p_no].pageFault++; // page fault increase karo
            // ab ata hai main chij, aagar freePages Available ho to problem nhi hai, nikal ke map kardo
            if (freeFrames.empty())
            {
                // agar freeFrames nhi hai to ab replace karna hoga! to i guess yahan concept aya hai global ya local ka
                // easy rakhne ke liye abhi global wala leke chalte hai
                int64_t t_p_no = q.front().first;
                int64_t t_page_no = q.front().second;
                q.pop();
                int64_t free_frame_no = proc[t_p_no].pageTable[t_page_no];
                proc[t_p_no].pageTable.erase(t_page_no);
                freeFrames.push(free_frame_no);
            }
            int64_t frame_no = freeFrames.front();
            freeFrames.pop();
            proc[p_no].pageTable[page_no] = frame_no;
            q.push({p_no, page_no});
        }
        else
        {
            proc[p_no].pageHit++;
            output << "Hit" << endl;
        }
    }
    for (int i = 0; i < 4; i++)
    {
        if ((proc[i].pageHit + proc[i].pageFault) != 0)
        {
            float hit_rate = float(proc[i].pageHit+ 0.0000) / float(proc[i].pageHit + proc[i].pageFault+ 0.00000);
            cout << "Process " << i <<endl;
            cout<< "Hit Rate: "<<hit_rate*100<<"%"<<endl;
            cout<< "Hits: " << proc[i].pageHit << " Misses: "<< proc[i].pageFault<< endl;
        }
    }
    // int64_t address = 140729058804640;
    // cout << address;
    return 0;
}