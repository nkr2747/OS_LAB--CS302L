#include <bits/stdc++.h>
#include "PageTable.h"
#include "LRUCache.h"
using namespace std;

inline void hash_combine(size_t& seed, size_t val) {
    seed ^= val + 0x9e3779b9 + (seed << 6) + (seed >> 2);
}

class pair_hash_2 {
    public:
    size_t operator()(const pair<int64_t,int64_t>& p) const noexcept {
        size_t seed = 0;
        hash_combine(seed, hash<int64_t>{}(p.first));
        hash_combine(seed, hash<int64_t>{}(p.second));
        return seed;
    }
};

void FIFO_global(int pageSize, int totFrames, string sourceFile)
{
    queue<int64_t> freeFrames;
    for (int i = 0; i < totFrames; i++)
    {
        freeFrames.push(i);
    }
    queue<pair<int64_t, int64_t>> q;
    PageTable proc[4];
    for (int i = 0; i < 4; i++)
    {
        proc[i].setPageSize(pageSize);
    }
    ifstream file;
    file.open(sourceFile);
    ofstream output;
    string filename = "result_";
    filename += "FIFO_global_";
    filename += to_string(pageSize);
    filename += "_";
    filename += to_string(totFrames);
    filename += ".txt";
    output.open(filename);
    string line = "";
    int64_t p_no;
    int64_t virtual_address;
    while (getline(file, line, ','))
    {
        p_no = stoll(line);
        getline(file, line);
        virtual_address = stoll(line);
        int64_t page_no = virtual_address / pageSize;
        if (!proc[p_no].isMapped(page_no))
        {
            output << "Miss" << endl;
            proc[p_no].pageFault++;
            if (freeFrames.empty())
            {
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
            float hit_rate = float(proc[i].pageHit + 0.0000) / float(proc[i].pageHit + proc[i].pageFault + 0.00000);
            cout << "Process " << i << endl;
            cout << "Hit Rate: " << hit_rate * 100 << "%" << endl;
            cout << "Hits: " << proc[i].pageHit << " Misses: " << proc[i].pageFault << endl;
        }
    }
    return;
}

void FIFO_local(int pageSize,int totFrames, string sourceFile){
     queue<int64_t> freeFrames[4];
    int i = 0;
    int j;
    for (j = i; j < i + totFrames/4; j++)
    {
        freeFrames[0].push(j);
    }
    i = j;
    for (j = i; j < i + totFrames/4; j++)
    {
        freeFrames[1].push(j);
    }
    i = j;
    for (j = i; j < i + totFrames/4; j++)
    {
        freeFrames[2].push(j);
    }
    i = j;
    for (j = i; j < i + totFrames/4; j++)
    {
        freeFrames[3].push(j);
    }
    queue< int64_t> q[4];
    PageTable proc[4];
    for (int i = 0; i < 4; i++)
    {
        proc[i].setPageSize(pageSize);
    }
    ifstream file;
    file.open(sourceFile);
    ofstream output;
    string filename = "result_";
    filename += "FIFO_local_";
    filename += to_string(pageSize);
    filename += "_";
    filename += to_string(totFrames);
    filename += ".txt";
    output.open(filename);
    string line = "";
    int64_t p_no;
    int64_t virtual_address;
    while (getline(file, line, ','))
    {
        p_no = stoll(line);
        getline(file, line);
        virtual_address = stoll(line);
        int64_t page_no = virtual_address / pageSize;
        if (!proc[p_no].isMapped(page_no))
        {
            output << "Miss" << endl;
            proc[p_no].pageFault++;
            if (freeFrames[p_no].empty())
            {
                int64_t t_page_no = q[p_no].front();
                q[p_no].pop();
                int64_t free_frame_no = proc[p_no].pageTable[t_page_no];
                proc[p_no].pageTable.erase(t_page_no);
                freeFrames[p_no].push(free_frame_no);
            }
            int64_t frame_no = freeFrames[p_no].front();
            freeFrames[p_no].pop();
            proc[p_no].pageTable[page_no] = frame_no;
            q[p_no].push(page_no);
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
    return ;
}

void LRU_global(int pageSize, int totFrames, string sourceFile){
    LRUCache lru(totFrames);
    vector<PageTable> proc(4);

    queue<int> freeFrames;
    for (int i=0;i<totFrames;i++) freeFrames.push(i);

    ifstream file(sourceFile);
    string filename = "result_";
    filename += "LRU_global_";
    filename += to_string(pageSize);
    filename += "_";
    filename += to_string(totFrames);
    filename += ".txt";
    ofstream output;
    output.open(filename);
    string line;
    while (getline(file,line,',')) {
        int p_no = stoi(line);
        getline(file,line);
        int64_t va = stoll(line);
        int page_no = va / pageSize;

        bool hit = lru.isHit(p_no,page_no);
        if (!proc[p_no].isMapped(page_no)) {
            output << "Miss\n";
            proc[p_no].pageFault++;
            if (freeFrames.empty()) {
                auto victim = lru.getLRU();
                int v_p = victim.first, v_page = victim.second;
                int free_frame = proc[v_p].pageTable[v_page];
                proc[v_p].pageTable.erase(v_page);
                freeFrames.push(free_frame);
            }
            int frame_no = freeFrames.front(); freeFrames.pop();
            proc[p_no].pageTable[page_no] = frame_no;
            lru.put(p_no,page_no);
        } else {
            output << "Hit\n";
            proc[p_no].pageHit++;
        }
    }

    for (int i=0;i<4;i++) {
        int64_t hits = proc[i].pageHit;
        int64_t faults = proc[i].pageFault;
        if (hits+faults>0) {
            double hit_rate = double(hits)/(hits+faults);
            cout << "Process " << i << "\n";
            cout << "Hit Rate: " << hit_rate*100 << "%\n";
            cout << "Hits: " << hits << " Misses: " << faults << "\n";
        }
    }
    return;
}

void LRU_local(int pageSize, int totFrames, string sourceFile){
    LRUCache lru[4];
    for(int i = 0; i < 4; i++){
        lru[i].setCapacity(totFrames/4);
    }
    vector<PageTable> proc(4);

    queue<int> freeFrames[4];
    int i = 0;
    int j;
    for (j = i; j < i + totFrames/4; j++)
    {
        freeFrames[0].push(j);
    }
    i = j;
    for (j = i; j < i + totFrames/4; j++)
    {
        freeFrames[1].push(j);
    }
    i = j;
    for (j = i; j < i + totFrames/4; j++)
    {
        freeFrames[2].push(j);
    }
    i = j;
    for (j = i; j < i + totFrames/4; j++)
    {
        freeFrames[3].push(j);
    }

    ifstream file(sourceFile);
    ofstream output;
    string filename = "result_";
    filename += "LRU_local_";
    filename += to_string(pageSize);
    filename += "_";
    filename += to_string(totFrames);
    filename += ".txt";
    output.open(filename);
    string line;
    while (getline(file,line,',')) {
        int p_no = stoi(line);
        getline(file,line);
        int64_t va = stoll(line);
        int page_no = va / pageSize;

        bool hit = lru[p_no].isHit(p_no,page_no);
        if (!proc[p_no].isMapped(page_no)) {
            output << "Miss\n";
            proc[p_no].pageFault++;
            if (freeFrames[p_no].empty()) {
                auto victim = lru[p_no].getLRU();
                int v_p = victim.first, v_page = victim.second;
                int free_frame = proc[v_p].pageTable[v_page];
                proc[v_p].pageTable.erase(v_page);
                freeFrames[p_no].push(free_frame);
            }
            int frame_no = freeFrames[p_no].front(); freeFrames[p_no].pop();
            proc[p_no].pageTable[page_no] = frame_no;
            lru[p_no].put(p_no,page_no);
        } else {
            output << "Hit\n";
            proc[p_no].pageHit++;
        }
    }

    for (int i=0;i<4;i++) {
        int64_t hits = proc[i].pageHit;
        int64_t faults = proc[i].pageFault;
        if (hits+faults>0) {
            double hit_rate = double(hits)/(hits+faults);
            cout << "Process " << i << "\n";
            cout << "Hit Rate: " << hit_rate*100 << "%\n";
            cout << "Hits: " << hits << " Misses: " << faults << "\n";
        }
    }
    return ;
}

void RDM_global(int pageSize, int totFrames, string sourceFile){
    queue<int64_t> freeFrames;
    for (int i = 0; i < totFrames; i++)
    {
        freeFrames.push(i);
    }
    vector<pair<int64_t, int64_t>> q;
    PageTable proc[4];
    for (int i = 0; i < 4; i++)
    {
        proc[i].setPageSize(pageSize);
    }
    ifstream file;
    file.open(sourceFile);
    ofstream output;
    string filename = "result_";
    filename += "RDM_global_";
    filename += to_string(pageSize);
    filename += "_";
    filename += to_string(totFrames);
    filename += ".txt";
    output.open(filename);
    string line = "";
    int64_t p_no;
    int64_t virtual_address;
    int count = 0;
    while (getline(file, line, ','))
    {
        p_no = stoll(line);
        getline(file, line);
        virtual_address = stoll(line);
        int64_t page_no = virtual_address / pageSize;
        if (!proc[p_no].isMapped(page_no))
        {
            output << "Miss" << endl;
            proc[p_no].pageFault++;
            if (freeFrames.empty())
            {
                unsigned seed = chrono::system_clock::now().time_since_epoch().count();
                mt19937 generator(seed+count);
                count++;
                uniform_int_distribution<int> distribution(0, q.size() - 1);
                int randomIndex = distribution(generator);
                cout<<randomIndex<<" ";
                int64_t t_p_no = q[randomIndex].first;
                int64_t t_page_no = q[randomIndex].second;
                q.erase(q.begin()+randomIndex);
                int64_t free_frame_no = proc[t_p_no].pageTable[t_page_no];
                proc[t_p_no].pageTable.erase(t_page_no);
                freeFrames.push(free_frame_no);
            }
            int64_t frame_no = freeFrames.front();
            freeFrames.pop();
            proc[p_no].pageTable[page_no] = frame_no;
            q.push_back({p_no, page_no});
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
            float hit_rate = float(proc[i].pageHit + 0.0000) / float(proc[i].pageHit + proc[i].pageFault + 0.00000);
            cout << "Process " << i << endl;
            cout << "Hit Rate: " << hit_rate * 100 << "%" << endl;
            cout << "Hits: " << proc[i].pageHit << " Misses: " << proc[i].pageFault << endl;
        }
    }
    return ;
}

void RDM_local(int pageSize, int totFrames, string sourceFile){
    queue<int64_t> freeFrames[4];
    int i = 0;
    int j;
    for (j = i; j < i + totFrames/4; j++)
    {
        freeFrames[0].push(j);
    }
    i = j;
    for (j = i; j < i + totFrames/4; j++)
    {
        freeFrames[1].push(j);
    }
    i = j;
    for (j = i; j < i + totFrames/4; j++)
    {
        freeFrames[2].push(j);
    }
    i = j;
    for (j = i; j < i + totFrames/4; j++)
    {
        freeFrames[3].push(j);
    }
    vector<int64_t> q[4];
    PageTable proc[4];
    for (int i = 0; i < 4; i++)
    {
        proc[i].setPageSize(pageSize);
    }
    ifstream file;
    file.open(sourceFile);
    ofstream output;
    output.open("result.txt");
    string line = "";
    int64_t p_no;
    int64_t virtual_address;
    int count = 0;
    while (getline(file, line, ','))
    {
        p_no = stoll(line);
        getline(file, line);
        virtual_address = stoll(line);
        int64_t page_no = virtual_address / pageSize;
        if (!proc[p_no].isMapped(page_no))
        {
            output << "Miss" << endl;
            proc[p_no].pageFault++;
            if (freeFrames[p_no].empty())
            {
                unsigned seed = chrono::system_clock::now().time_since_epoch().count();
                mt19937 generator(seed+count);
                count++;
                uniform_int_distribution<int> distribution(0, q[p_no].size() - 1);
                int randomIndex = distribution(generator);
                int64_t t_page_no = q[p_no][randomIndex];
                q[p_no].erase(q[p_no].begin()+randomIndex);
                int64_t free_frame_no = proc[p_no].pageTable[t_page_no];
                proc[p_no].pageTable.erase(t_page_no);
                freeFrames[p_no].push(free_frame_no);
            }
            int64_t frame_no = freeFrames[p_no].front();
            freeFrames[p_no].pop();
            proc[p_no].pageTable[page_no] = frame_no;
            q[p_no].push_back(page_no);
        }
        else
        {
            proc[p_no].pageHit++;
            output << "Hit" << endl;
        }
    }
    int sum = 0;
    for (int i = 0; i < 4; i++)
    {
        if ((proc[i].pageHit + proc[i].pageFault) != 0)
        {
            float hit_rate = float(proc[i].pageHit + 0.0000) / float(proc[i].pageHit + proc[i].pageFault + 0.00000);
            cout << "Process " << i << endl;
            cout << "Hit Rate: " << hit_rate * 100 << "%" << endl;
            cout << "Hits: " << proc[i].pageHit << " Misses: " << proc[i].pageFault << endl;
            sum += proc[i].pageHit+proc[i].pageFault;
        }
    }
    return ;
}

void OPT_global(int pageSize, int totFrames, string sourceFile){
    ifstream file(sourceFile);
    vector<pair<int64_t,int64_t>> accesses;
    string line;
    while (getline(file, line, ',')) {
        int64_t p_no = stoll(line);
        getline(file, line);
        int64_t va = stoll(line);
        int64_t page_no = va / pageSize;
        accesses.push_back({p_no, page_no});
    }
    file.close();
    if (accesses.empty()) {
        cerr << "No accesses read from combined.trace\n";
        return;
    }
    unordered_map<pair<int64_t,int64_t>, queue<int>, pair_hash_2> futurePos;
    for (int i = 0; i < (int)accesses.size(); ++i) {
        futurePos[accesses[i]].push(i);
    }
    queue<int> freeFrames;
    for (int i = 0; i < totFrames; ++i) freeFrames.push(i);

    vector<PageTable> proc(4);
    for (int i = 0; i < 4; ++i) proc[i].setPageSize(pageSize);

    string outname = string("result_OPT_global_") + to_string(pageSize) + "_" + to_string(totFrames) + ".txt";
    ofstream output(outname);
    using pr = pair<int,pair<int,int64_t>> ;
    priority_queue<pr,vector<pr>,greater<pr>> pq;
    for (int idx = 0; idx < (int)accesses.size(); ++idx) {
        auto cur = accesses[idx];
        int64_t p_no = cur.first;
        int64_t page_no = cur.second;
        futurePos[{p_no,page_no}].pop();
        int next = INT_MAX;
        if(!futurePos[{p_no,page_no}].empty()){
            next = futurePos[{p_no,page_no}].front();
            pq.push({next,{p_no,page_no}});
        }
        if (!proc[p_no].isMapped(page_no)) {
            output << "Miss\n";
            proc[p_no].pageFault++;
            if (freeFrames.empty()) {
                int victim_p = -1;
                int64_t victim_page = -1;
                int best_next_use = -1;

                victim_p = pq.top().second.first;
                victim_page = pq.top().second.second;

                pq.pop();

                int64_t evicted_frame = proc[victim_p].pageTable[victim_page];
                proc[victim_p].pageTable.erase(victim_page);
                freeFrames.push((int)evicted_frame);
            }
            int64_t frame_no = freeFrames.front(); freeFrames.pop();
            proc[p_no].pageTable[page_no] = frame_no;
        } else {
            proc[p_no].pageHit++;
            output << "Hit\n";
        }
    }
    for (int i = 0; i < 4; ++i) {
        int64_t hits = proc[i].pageHit;
        int64_t faults = proc[i].pageFault;
        double hit_rate = double(hits) / double(hits + faults);
        cout << "Process " << i << "\n";
        cout << "  Hit Rate: " << (hit_rate * 100.0) << "%\n";
        cout << "  Hits: " << hits << "  Misses: " << faults << "\n";
    }
    output.close();
    return ;
}

void OPT_local(int pageSize, int totFrames, string sourceFile){
    ifstream file(sourceFile);
    vector<pair<int64_t,int64_t>> accesses;
    string line;
    while (getline(file, line, ',')) {
        int64_t p_no = stoll(line);
        getline(file, line);
        int64_t va = stoll(line);
        int64_t page_no = va / pageSize;
        accesses.push_back({p_no, page_no});
    }
    file.close();
    unordered_map<pair<int64_t,int64_t>, queue<int>, pair_hash_2> futurePos;
    for (int i = 0; i < (int)accesses.size(); ++i) {
        futurePos[accesses[i]].push(i);
    }
    queue<int> freeFrames[4];
    int i = 0;
    int j;
    for (j = i; j < i + totFrames/4; j++)
    {
        freeFrames[0].push(j);
    }
    i = j;
    for (j = i; j < i + totFrames/4; j++)
    {
        freeFrames[1].push(j);
    }
    i = j;
    for (j = i; j < i + totFrames/4; j++)
    {
        freeFrames[2].push(j);
    }
    i = j;
    for (j = i; j < i + totFrames/4; j++)
    {
        freeFrames[3].push(j);
    }

    vector<PageTable> proc(4);
    for (int i = 0; i < 4; ++i) proc[i].setPageSize(pageSize);

    string outname = string("result_OPT_global_") + to_string(pageSize) + "_" + to_string(totFrames) + ".txt";
    ofstream output(outname);
    using pr = pair<int,int64_t>;
    priority_queue<pr,vector<pr>,greater<pr>> pq[4];
    for (int idx = 0; idx < (int)accesses.size(); ++idx) {
        auto cur = accesses[idx];
        int64_t p_no = cur.first;
        int64_t page_no = cur.second;
        futurePos[{p_no,page_no}].pop();
        int next = INT_MAX;
        if(!futurePos[{p_no,page_no}].empty()){
            next = futurePos[{p_no,page_no}].front();
            pq[p_no].push({next,page_no});
        }
        if (!proc[p_no].isMapped(page_no)) {
            output << "Miss\n";
            proc[p_no].pageFault++;
            if (freeFrames[p_no].empty()) {
                int victim_p = -1;
                int64_t victim_page = -1;
                int best_next_use = -1;

                victim_p = p_no;
                victim_page = pq[p_no].top().second;

                pq[p_no].pop();

                int64_t evicted_frame = proc[victim_p].pageTable[victim_page];
                proc[victim_p].pageTable.erase(victim_page);
                freeFrames[p_no].push((int)evicted_frame);
            }
            int64_t frame_no = freeFrames[p_no].front(); freeFrames[p_no].pop();
            proc[p_no].pageTable[page_no] = frame_no;
        } else {
            proc[p_no].pageHit++;
            output << "Hit\n";
        }
    }
    for (int i = 0; i < 4; ++i) {
        int64_t hits = proc[i].pageHit;
        int64_t faults = proc[i].pageFault;
        double hit_rate = double(hits) / double(hits + faults);
        cout << "Process " << i << "\n";
        cout << "  Hit Rate: " << (hit_rate * 100.0) << "%\n";
        cout << "  Hits: " << hits << "  Misses: " << faults << "\n";
    }

    output.close();
    return ;
}

int main(int argc, char *argv[])
{
    if(argc != 6){
        cout<<"format: <pageSize> <totalFrames> <replacement policy(FIFO/LRU/OPT/RDM)> <allocation policy(global/local)> <path to traceFile>";
        return 1;
    }
    int pageSize = atoi(argv[1]);
    int totFrames = atoi(argv[2]);
    string replacementPolicy = argv[3];
    string allocationPolicy = argv[4];
    string filename = argv[5];

    unordered_map<string,int> mpp_replacement;
    mpp_replacement["FIFO"] = 1;
    mpp_replacement["LRU"] = 2;
    mpp_replacement["OPT"] = 3;
    mpp_replacement["RDM"] = 4;
    switch(mpp_replacement[replacementPolicy]){
        case 1:
        if(allocationPolicy == "local"){
            FIFO_local(pageSize,totFrames,filename);
        }
        else if(allocationPolicy == "global"){
            FIFO_global(pageSize,totFrames,filename);
        }
        else{
            cout<<"Invalid allocation type\n";
        }
        break;
        case 2:
        if(allocationPolicy == "local"){
            LRU_local(pageSize,totFrames,filename);
        }
        else if(allocationPolicy == "global"){
            LRU_global(pageSize,totFrames,filename);
        }
        else{
            cout<<"Invalid allocation type\n";
        }
        break;
        case 3:
        if(allocationPolicy == "local"){
            RDM_local(pageSize,totFrames,filename);
        }
        else if(allocationPolicy == "global"){
            RDM_global(pageSize,totFrames,filename);
        }
        else{
            cout<<"Invalid allocation type\n";
        }
        break;
        case 4:
        if(allocationPolicy == "local"){
            OPT_local(pageSize,totFrames,filename);
        }
        else if(allocationPolicy == "global"){
            OPT_global(pageSize,totFrames,filename);
        }
        else{
            cout<<"Invalid allocation type\n";
        }
        break;
        default:
        cout<<"Invalid replacement Policy\n";
    }
}