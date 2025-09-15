#include <iostream>
#include <algorithm>
#include <vector>
#include "Classes/Processor.h"
#include <fstream>
#include <sstream>
#include <queue>
#include <string>
using namespace std;

class Run
{
public:
    int left;
    Process *cur;
    Run()
    {
        left = -1;
        cur = NULL;
    }
    Run(int left, Process *cur)
    {
        this->left = left;
        this->cur = cur;
    }
};

class info
{
public:
    int p_no;
    int b_no;
    int start;
};

class RRun
{
public:
    int run;
    int ts;
    Process *cur;
    RRun()
    {
        run = 0;
        ts = 0;
        cur = NULL;
    }
};

void display(vector<Process *> parsed_data)
{
    for (int i = 0; i < parsed_data.size(); i++)
    {
        cout << parsed_data[i]->p_no << " " << parsed_data[i]->arrival << " ";
        for (int j = 0; j < parsed_data[i]->bursts.size(); j++)
        {
            cout << parsed_data[i]->bursts[j] << " ";
        }
        cout << endl;
    }
}

int countGaps(string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: Cannot open file " << filename << endl;
        return -1;
    }

    string line;
    int prev_end = -1;
    int gap_count = 0;
    bool first_line = true;

    while (getline(file, line)) {
        if (first_line) {
            
            first_line = false;
            continue;
        }

        istringstream iss(line);
        string process_info;
        int start, end;

        iss >> process_info >> start >> end;

        if (prev_end != -1 && (start - prev_end > 1)) {
            //cout<<start<<"  "<<prev_end<<endl;
            gap_count += start - prev_end-1;
        }

        prev_end = end;
    }

    file.close();
    return gap_count;
}
pair<int,int> countGapsMC(string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: Cannot open file " << filename << endl;
        return {-1,-1};
    }

    string line;
    int prev_end = -1;
    int gap_count = 0;
    bool first_line = true;

    while (getline(file, line)) {
        if (first_line) {
            
            first_line = false;
            continue;
        }
        else if(line == "CPU1") break;

        istringstream iss(line);
        string process_info;
        int start, end;

        iss >> process_info >> start >> end;

        if (prev_end != -1 && (start - prev_end > 1)) {
            //cout<<start<<"  "<<prev_end<<endl;
            gap_count += start - prev_end-1;
        }

        prev_end = end;
    }
    int ans1 = gap_count;
    prev_end = -1;
    gap_count = 0;
    while (getline(file, line)) {
        istringstream iss(line);
        string process_info;
        int start, end;

        iss >> process_info >> start >> end;

        if (prev_end != -1 && (start - prev_end > 1)) {
            //cout<<start<<"  "<<prev_end<<endl;
            gap_count += start - prev_end-1;
        }

        prev_end = end;
    }
    file.close();
    return {ans1,gap_count};
}


void Processor::FIFO(vector<Process *> parsed_data, char *process_file)
{
    using pr = pair<int, Process *>;
    ofstream output;
    string filename = process_file;
    string timeline = "Schedule(FIFO)";
    timeline += filename[7];
    timeline += ".txt";
    output.open(timeline);
    int time = parsed_data[0]->arrival;
    queue<Process *> ready;
    priority_queue<pr, vector<pr>, greater<pr>> waiting;
    ready.push(parsed_data[0]);
    int i = 1;
    int maxTAT = 0;
    float avgTAT = 0;
    int idleTime = 0;
    output << "CPU0\n";
    while (!ready.empty() || !waiting.empty())
    {
        if (!ready.empty())
        {
            Process *cur = ready.front();
            ready.pop();
            int ind = cur->index;
            output << "P" << to_string(cur->p_no) << "," << to_string((ind / 2) + 1) << "     " << time << "      ";
            time += cur->bursts[ind];
            output << time - 1 << endl;
            ind += 1;
            if (ind < cur->bursts.size())
            {
                cur->index = ind;
                cur->wait = time;
                waiting.push({time + cur->bursts[ind], cur});
            }
            else
            {
                cur->completion = time;
                avgTAT += cur->completion - cur->arrival;
                maxTAT = max(maxTAT, cur->completion - cur->arrival);
            }
        }
        else
        {
            time++;
            idleTime++;
        }
        // maybe yahan pe main priority queue add karunga kyuki nikalne me eeasy hoga. -- kar diya PQ
        while (!waiting.empty())
        {
            Process *top = waiting.top().second;
            if (time >= waiting.top().first)
            {
                waiting.pop();
                if (top->index + 1 < top->bursts.size())
                {
                    top->index++;
                    ready.push(top);
                }
            }
            else
            {
                break;
            }
        }
        for (; i < parsed_data.size(); i++)
        {
            if (parsed_data[i]->arrival <= time)
            {
                ready.push(parsed_data[i]);
            }
            else
            {
                break;
            }
        }
    }
    cout << "Scheduling Algorithm: FIFO\n";
    cout << "Time to Complete: " << time << endl;
    cout << "Maximum TAT: " << maxTAT << endl;
    cout << "Average TAT: " << avgTAT / (float)parsed_data.size() << endl;
    cout << "Runtime of Simulator: " << time - idleTime << endl;
    output.close();
    return;
}

void Processor::NPSJF(vector<Process *> parsed_data, char *process_file)
{
    ofstream output;
    string filename = process_file;
    string timeline = "Schedule(NPSJF)";
    timeline += filename[7];
    timeline += ".txt";
    output.open(timeline);
    int time = parsed_data[0]->arrival;

    priority_queue<pair<int, Process *>, vector<pair<int, Process *>>, greater<pair<int, Process *>>> ready;
    priority_queue<pair<int, Process *>, vector<pair<int, Process *>>, greater<pair<int, Process *>>> waiting;

    int i = 0;
    for (; i < parsed_data.size(); i++)
    {
        int ind = parsed_data[i]->index;
        if (parsed_data[i]->arrival <= time)
        {
            ready.push({parsed_data[i]->bursts[ind], parsed_data[i]});
            // cout<<j<<" ";
        }
        else
            break;
    }
    int maxTAT = 0;
    float avgTAT = 0;
    int idleTime = 0;
    output << "CPU0\n";
    while (!ready.empty() || !waiting.empty())
    {
        // cout << "Ready: " << ready.size() << " waiting: " << waiting.size() << endl;
        if (!ready.empty())
        {
            Process *cur = ready.top().second;
            ready.pop();
            int ind = cur->index;
            output << "P" << to_string(cur->p_no) << "," << to_string((ind / 2) + 1) << "     " << time << "      ";

            time += cur->bursts[ind];
            output << time - 1 << endl;
            ind += 1;
            if (ind < cur->bursts.size())
            {
                cur->index = ind;
                cur->wait = time;
                waiting.push({time + cur->bursts[ind], cur});
            }
            else
            {
                cur->completion = time;
                avgTAT += cur->completion - cur->arrival;
                maxTAT = max(maxTAT, cur->completion - cur->arrival);
            }
            // maybe yahan pe main priority queue add karunga kyuki nikalne me eeasy hoga.
        }
        else
        {
            Process *top = waiting.top().second;
            if (time - top->wait < top->bursts[top->index])
            {
                // cout<<time<<"--";
                int t = time;
                time = top->bursts[top->index] + top->wait;
                int incTime = time - t;
                idleTime += incTime;
            }
        }
        while (!waiting.empty())
        {
            Process *top = waiting.top().second;
            if (time - top->wait >= top->bursts[top->index])
            {
                waiting.pop();
                if (top->index + 1 < top->bursts.size())
                {
                    top->index++;
                    ready.push({top->bursts[top->index], top});
                }
            }
            else
            {
                break;
            }
        }
        for (; i < parsed_data.size(); i++)
        {
            if (parsed_data[i]->arrival <= time)
            {
                ready.push({parsed_data[i]->bursts[parsed_data[i]->index], parsed_data[i]});
            }
            else
            {
                break;
            }
        }
    }
    output.close();
    cout << "Scheduling Algorithm: NPSJF\n";
    cout << "Time to Complete: " << time << endl;
    cout << "Maximum TAT: " << maxTAT << endl;
    cout << "Average TAT: " << avgTAT / (float)parsed_data.size() << endl;
    int h = countGaps(timeline);
    cout << "Runtime of Simulator: " << time - h << endl;
    return;
}

void Processor::PSJF(vector<Process *> parsed_data, char *process_file)
{
    // int time = 0;
    ofstream output;
    string filename = process_file;
    string timeline = "Schedule(PSJF)";
    timeline += filename[7];
    timeline += ".txt";
    output.open(timeline);
    // int time = parsed_data[0]->arrival;
    int time = 0;
    int idleTime = 0;
    priority_queue<pair<int, Process *>, vector<pair<int, Process *>>, greater<pair<int, Process *>>> ready;
    priority_queue<pair<int, Process *>, vector<pair<int, Process *>>, greater<pair<int, Process *>>> waiting;
    pair<int, int> prev = {-1, -1};
    int i = 0;
    for (; i < parsed_data.size(); i++)
    {
        if (parsed_data[i]->arrival <= time)
        {
            ready.push({parsed_data[i]->bursts[0], parsed_data[i]});
            // cout<<j<<" ";
        }
        else
            break;
    }
    int maxTAT = 0;
    float avgTAT = 0;
    output << "CPU0\n";
    // Process *prev = NULL;
    while (!ready.empty() || !waiting.empty())
    {

        // cout << "Ready: " << ready.size() << " waiting: " << waiting.size() << endl;
        if (!ready.empty())
        {
            Process *top = ready.top().second;
            int timeLeft = ready.top().first;
            ready.pop();
            int ind = top->index;
            pair<int, int> cur = {top->p_no, (ind / 2) + 1};
            if (prev != cur)
            {
                if (prev.first != -1)
                    output << "       " << time - 1 << endl;
                output << "P" << to_string(top->p_no) << "," << to_string((ind / 2) + 1) << "     " << time;
            }
            prev = cur;
            timeLeft--;
            if (timeLeft > 0)
                ready.push({timeLeft, top});
            else
            {
                ind++;

                if (ind < top->bursts.size())
                {
                    top->index++;
                    top->wait = time;
                    waiting.push({top->bursts[top->index], top});
                }
                else
                {
                    top->completion = time;
                }
            }
            // if (prev != top)
            // {
            //     output << "P" << to_string(top->p_no) << "," << to_string((ind / 2) + 1) << "     " << time << endl;
            // }
            // prev = top;
            time++;
        }
        else
        {
            idleTime++;
            time++;
        }
        while (i < parsed_data.size())
        {
            int ind = parsed_data[i]->index;
            if (parsed_data[i]->arrival <= time)
            {
                ready.push({parsed_data[i]->bursts[ind], parsed_data[i]});
                // cout<<j<<" ";
            }
            else
                break;
            i++;
        }

        while (!waiting.empty())
        {
            Process *top = waiting.top().second;
            if (time - top->wait >= top->bursts[top->index])
            {
                waiting.pop();
                if (top->index + 1 < top->bursts.size())
                {
                    top->index++;
                    ready.push({top->bursts[top->index], top});
                }
            }
            else
            {
                break;
            }
        }
    }
    // output << "Maximum TAT: " << maxTAT << endl;
    // output << "Average TAT: " << avgTAT / (float)parsed_data.size();
    output << "       " << time - 1 << endl;
    output.close();
    for (auto it : parsed_data)
    {
        int TAT = it->completion - it->arrival;
        maxTAT = max(TAT, maxTAT);
        avgTAT += TAT;
    }
    cout << "Scheduling Algorithm: PSJF\n";
    cout << "Time to Complete: " << time << endl;
    cout << "Maximum TAT: " << maxTAT << endl;
    cout << "Average TAT: " << avgTAT / (float)parsed_data.size() << endl;
    int idle = countGaps(timeline);
    cout << "Runtime of Simulator: " << time - idle << endl;
    return;
}

void Processor::RR(vector<Process *> parsed_data, char *process_file, int timeInterval)
{
    ofstream output;
    string filename = process_file;
    string timeline = "Schedule(RR)";
    timeline += filename[7];
    timeline += ".txt";
    output.open(timeline);
    int time = 0;
    time = parsed_data[0]->arrival;
    int timeSlice = timeInterval;
    int maxTAT = 0;
    float avgTAT = 0;
    int idleTime = 0;
    queue<pair<int, Process *>> ready;
    priority_queue<pair<int, Process *>, vector<pair<int, Process *>>, greater<pair<int, Process *>>> waiting;
    int i;
    for (i = 0; i < parsed_data.size(); i++)
    {
        if (parsed_data[i]->arrival <= time)
        {
            ready.push({parsed_data[i]->bursts[0], parsed_data[i]});
        }
        else
            break;
    }
    int timeLeft = -1;
    Process *top = NULL;
    while (!waiting.empty() || !ready.empty())
    {
        // cout<< waiting.size() <<" "<<ready.size()<<endl;
        if (!ready.empty())
        {
            timeLeft = ready.front().first;
            // cout<<timeLeft<<endl;
            top = ready.front().second;
            ready.pop();
            output << "P" << top->p_no << "," << to_string((top->index + 1) / 2 + 1) << "     " << time << "      ";
            // cout << "P" <<top->p_no<< ","<<to_string((top->index+1)/2 + 1)<<"     "<<time<<"      ";
            if (timeLeft > timeSlice)
            {
                timeLeft -= timeSlice;
                time += timeSlice;
                // ready.push({timeLeft,top});
            }
            else
            {
                time += timeLeft;
                timeLeft -= timeSlice;
                int ind = top->index;
                if (ind + 1 < top->bursts.size())
                {
                    top->index = ind + 1;
                    top->wait = time;
                    waiting.push({time + top->bursts[top->index], top});
                }
                else
                {
                    top->completion = time;
                    int TAT = top->completion - top->arrival;
                    maxTAT = max(maxTAT, TAT);
                    avgTAT += TAT;
                }
            }
            output << time << endl;
            // cout<<time<<endl;
        }
        else
        {
            time++;
            idleTime++;
        }
        for (; i < parsed_data.size(); i++)
        {
            if (parsed_data[i]->arrival <= time)
            {
                // cout<<parsed_data[i]->bursts[0]<<endl;
                ready.push({parsed_data[i]->bursts[0], parsed_data[i]});
            }
            else
                break;
        }
        while (!waiting.empty())
        {
            Process *top = waiting.top().second;
            int estimateTime = waiting.top().first;
            if (time >= estimateTime)
            {
                waiting.pop();
                top->index++;
                ready.push({top->bursts[top->index], top});
            }
            else
                break;
        }
        if (timeLeft > 0)
            ready.push({timeLeft, top});
    }
    output.close();
    cout << "Scheduling Algorithm: Round Robin\n";
    cout << "Time to Complete: " << time << endl;
    cout << "Maximum TAT: " << maxTAT << endl;
    cout << "Average TAT: " << avgTAT / (float)parsed_data.size() << endl;
    int idle = countGaps(timeline);
    cout << "Runtime of Simulator: " << time - idle << endl;
    return;
}

void MultiCoreProcessor::FIFO(vector<Process *> parsed_data, char *process_file)
{
    ofstream output1;
    // ofstream output2;
    string filename = process_file;
    string timeline1 = "ScheduleMC(FIFO)";
    // string timeline2 = "ScheduleMC1(FIFO)";
    timeline1 += filename[7];
    // timeline2 += filename[7];
    timeline1 += ".txt";
    // timeline2 += ".txt";
    output1.open(timeline1);
    // output2.open(timeline2);

    int time = parsed_data[0]->arrival - 1;
    int idleTime = -1;
    queue<Process *> ready;
    vector<string> ans0;
    vector<string> ans1;
    priority_queue<pair<int, Process *>, vector<pair<int, Process *>>, greater<pair<int, Process *>>> waiting;
    ready.push(parsed_data[0]);
    int i = 1;
    int maxTAT = 0;
    float avgTAT = 0;
    // Run run1;
    // Run run2;
    int run1 = 0;
    Process *p1 = NULL;
    Process *p2 = NULL;
    int run2 = 0;
    // int time = 0;
    //  output << "CPU0\n";
    string temp1 = "";
    string temp2 = "";
    while (!ready.empty() || !waiting.empty() || run1 != 0 || run2 != 0)
    {
        time++;
        if (run1 > 0)
        {
            run1--;
        }
        if (run2 > 0)
        {
            run2--;
        }
        if (run1 == 0 && run2 == 0)
        {
            idleTime++;
        }
        for (; i < parsed_data.size(); i++)
        {
            if (parsed_data[i]->arrival <= time)
            {
                ready.push(parsed_data[i]);
            }
            else
                break;
        }
        while (!waiting.empty())
        {
            Process *top = waiting.top().second;
            int index = top->index;
            int ioBurst = top->bursts[index];
            int last = top->wait;
            if (last + ioBurst <= time)
            {
                top->index++;
                waiting.pop();
                ready.push(top);
            }
            else
            {
                break;
            }
        }
        if (run1 == 0)
        {
            if (p1)
            {
                int ind = p1->index;
                ind += 1;
                temp1 += to_string(time - 1);
                ans0.push_back(temp1);
                temp1 = "";
                // output1 << time-1<<endl;
                // cout << time<<endl;
                if (ind < p1->bursts.size())
                {
                    p1->index = ind;
                    p1->wait = time;

                    waiting.push({time + p1->bursts[ind], p1});
                }
                else
                {
                    p1->completion = time;
                }
            }
            p1 = NULL;
            if (!ready.empty())
            {
                Process *top = ready.front();
                ready.pop();
                int index = top->index;
                run1 = top->bursts[index];
                p1 = top;
                temp1 += "P";
                temp1 += to_string(p1->p_no);
                temp1 += ",";
                temp1 += to_string((p1->index / 2) + 1);
                temp1 += "      ";
                temp1 += to_string(time);
                temp1 += "      ";
                // output1<<"P"<< p1->p_no <<","<<(p1->index / 2) + 1<<"     "<<time<<"      ";
                // cout<<p1->index << "P"<< p1->p_no <<","<<(p1->index / 2) + 1<<"     "<<time<<"      ";
                //  run1.p = top;
                //  cout<<run1.run<<endl;
            }
        }
        if (run2 == 0)
        {
            if (p2)
            {
                int ind = p2->index;
                ind += 1;
                // output2 << time-1<<endl;
                temp2 += to_string(time - 1);
                ans1.push_back(temp2);
                temp2 = "";
                if (ind < p2->bursts.size())
                {
                    p2->index = ind;
                    p2->wait = time;

                    waiting.push({time + p2->bursts[ind], p2});
                }
                else
                {
                    p2->completion = time;
                }
            }
            p2 = NULL;
            if (!ready.empty())
            {
                Process *top = ready.front();
                ready.pop();
                int index = top->index;
                run2 = top->bursts[index];
                p2 = top;
                temp2 += "P";
                temp2 += to_string(p2->p_no);
                temp2 += ",";
                temp2 += to_string((p2->index / 2) + 1);
                temp2 += "      ";
                temp2 += to_string(time);
                temp2 += "      ";
                // output2<<"P"<< p2->p_no <<","<<(p2->index / 2) + 1<<"     "<<time<<"      ";
                //  run2.p = top;
            }
        }
        // output << "Time: " << time << endl;
        // if (p1)
        // {
        //     output << "Processor1- " << p1->p_no <<" "<< (p1->index / 2) + 1<< " " << run1 << endl;
        // }
        // if (p2)
        // {
        //     output << "Processor2- " << p2->p_no <<" "<< (p2->index / 2) + 1<< " " << run2 << endl;
        // }

        // cout<<run1<<" "<<run2<<endl;

        // Process *cur = ready.front();
        // ready.pop();
        // int ind = cur->index;
        // output << "P" << to_string(cur->p_no) << "," << to_string((ind / 2) + 1) << "     " << time << "      ";

        // time += cur->bursts[ind];
        // output << time - 1 << endl;
        // ind += 1;
        // if (ind < cur->bursts.size())
        // {
        //     cur->index = ind;
        //     cur->wait = time;
        //     waiting.push(cur);
        // }
        // else
        // {
        //     cur->completion = time;
        //     avgTAT += cur->completion - cur->arrival;
        //     maxTAT = max(maxTAT, cur->completion - cur->arrival);
        // }
        // // maybe yahan pe main priority queue add karunga kyuki nikalne me eeasy hoga.
        // while (!waiting.empty())
        // {
        //     Process *top = waiting.front();
        //     if (time - top->wait >= top->bursts[top->index])
        //     {
        //         waiting.pop();
        //         if (top->index + 1 < top->bursts.size())
        //         {
        //             top->index++;
        //             ready.push(top);
        //         }
        //     }
        //     else
        //     {
        //         break;
        //     }
        // }
        // for (; i < parsed_data.size(); i++)
        // {
        //     if (parsed_data[i]->arrival <= time)
        //     {
        //         ready.push(parsed_data[i]);
        //     }
        //     else
        //     {
        //         break;
        //     }
        // }
    }
    output1 << "CPU0\n";
    for (int i = 0; i < ans0.size(); i++)
    {
        output1 << ans0[i] << endl;
    }
    output1 << "CPU1\n";
    for (int i = 0; i < ans1.size(); i++)
    {
        output1 << ans1[i] << endl;
    }
    cout << "Scheduling Algorithm: FIFO\n";
    cout << "Time to Complete: " << time << endl;
    output1.close();
    // output2.close();
    for (int i = 0; i < parsed_data.size(); i++)
    {
        int TAT = parsed_data[i]->completion - parsed_data[i]->arrival;
        maxTAT = max(maxTAT, TAT);
        avgTAT += TAT;
    }
    cout << "Maximum TAT: " << maxTAT << endl;
    cout << "Average TAT: " << avgTAT / (float)parsed_data.size() << endl;
    auto idle = countGapsMC(timeline1);
    cout << "Runtime of Simulator(CPU1): " << time - idle.first << endl;
    cout << "Runtime of Simulator(CPU2): " << time - idle.second << endl;
    return;
}

void MultiCoreProcessor::NPSJF(vector<Process *> parsed_data, char *process_file)
{
    ofstream output1;

    string filename = process_file;
    string timeline1 = "ScheduleMC(NPSJF)";

    timeline1 += filename[7];

    timeline1 += ".txt";

    output1.open(timeline1);

    int time = parsed_data[0]->arrival - 1;
    priority_queue<pair<int, Process *>, vector<pair<int, Process *>>, greater<pair<int, Process *>>> ready;
    vector<string> ans0;
    vector<string> ans1;
    priority_queue<pair<int, Process *>, vector<pair<int, Process *>>, greater<pair<int, Process *>>> waiting;
    ready.push({parsed_data[0]->bursts[0], parsed_data[0]});
    int i = 1;
    int maxTAT = 0;
    float avgTAT = 0;
    int idleTime = -1;
    int run1 = 0;
    int n = parsed_data.size();
    Process *p1 = NULL;
    Process *p2 = NULL;
    int run2 = 0;
    string temp1 = "";
    string temp2 = "";
    while (!ready.empty() || !waiting.empty() || run1 != 0 || run2 != 0)
    {
        time++;
        if (run1 > 0)
        {
            run1--;
        }
        if (run2 > 0)
        {
            run2--;
        }
        if (run1 == 0 && run2 == 0)
            idleTime++;
        for (; i < parsed_data.size(); i++)
        {
            if (parsed_data[i]->arrival <= time)
            {
                ready.push({parsed_data[i]->bursts[0], parsed_data[i]});
            }
            else
                break;
        }
        while (!waiting.empty())
        {
            Process *top = waiting.top().second;
            int index = top->index;
            int ioBurst = top->bursts[index];
            int last = top->wait;
            if (last + ioBurst <= time)
            {
                top->index++;
                waiting.pop();
                ready.push({top->bursts[top->index], top});
            }
            else
            {
                break;
            }
        }
        if (run1 == 0)
        {
            if (p1)
            {
                int ind = p1->index;
                ind += 1;
                temp1 += to_string(time - 1);
                ans0.push_back(temp1);
                temp1 = "";
                if (ind < p1->bursts.size())
                {
                    p1->index = ind;
                    p1->wait = time;

                    waiting.push({time + p1->bursts[ind], p1});
                }
                else
                {
                    p1->completion = time;
                }
            }
            p1 = NULL;
            if (!ready.empty())
            {
                Process *top = ready.top().second;
                ready.pop();
                int index = top->index;
                run1 = top->bursts[index];
                p1 = top;
                temp1 += "P";
                temp1 += to_string(p1->p_no);
                temp1 += ",";
                temp1 += to_string((p1->index / 2) + 1);
                temp1 += "      ";
                temp1 += to_string(time);
                temp1 += "      ";
            }
        }
        if (run2 == 0)
        {
            if (p2)
            {
                int ind = p2->index;
                ind += 1;
                temp2 += to_string(time - 1);
                ans1.push_back(temp2);
                temp2 = "";
                if (ind < p2->bursts.size())
                {
                    p2->index = ind;
                    p2->wait = time;

                    waiting.push({time + p2->bursts[ind], p2});
                }
                else
                {
                    p2->completion = time;
                }
            }
            p2 = NULL;
            if (!ready.empty())
            {
                Process *top = ready.top().second;
                ready.pop();
                int index = top->index;
                run2 = top->bursts[index];
                p2 = top;
                temp2 += "P";
                temp2 += to_string(p2->p_no);
                temp2 += ",";
                temp2 += to_string((p2->index / 2) + 1);
                temp2 += "      ";
                temp2 += to_string(time);
                temp2 += "      ";
            }
        }
    }
    output1 << "CPU0\n";
    for (int i = 0; i < ans0.size(); i++)
    {
        output1 << ans0[i] << endl;
    }
    output1 << "CPU1\n";
    for (int i = 0; i < ans1.size(); i++)
    {
        output1 << ans1[i] << endl;
    }
    cout << "Scheduling Algorithm: Non Preemptive SJF\n";
    cout << "Time to complete: " << time << endl;
    output1.close();
    for (auto &it : parsed_data)
    {
        int TAT = it->completion - it->arrival;
        maxTAT = max(maxTAT, TAT);
        avgTAT += TAT;
    }
    cout << "Average Turnaround Time: " << avgTAT / n << endl;
    cout << "Maximuum Turnaround Time: " << maxTAT << endl;
    auto idle = countGapsMC(timeline1);
    cout << "Runtime of Simulator(CPU1): " << time - idle.first << endl;
    cout << "Runtime of Simulator(CPU2): " << time - idle.second << endl;
    return;
}

void MultiCoreProcessor::RR(vector<Process *> parsed_data, char *process_file, int timeSlice)
{
    ofstream output;
    string fileName = "ScheduleMC(RR)";
    fileName += process_file[7];
    fileName += ".txt";
    output.open(fileName);
    vector<string> vec1;
    vector<string> vec2;
    using pr = pair<int, Process *>;
    queue<pr> ready;
    priority_queue<pr, vector<pr>, greater<pr>> waiting;

    int time = parsed_data[0]->arrival;
    int i = 0;
    int n = parsed_data.size();
    string temp1 = "";
    string temp2 = "";
    while (i < n && time >= parsed_data[i]->arrival)
    {
        ready.push({parsed_data[i]->bursts[0], parsed_data[i]});
        i++;
    }

    RRun run1;
    RRun run2;
    int idleTime = 0;
    while (!ready.empty() || !waiting.empty() || run1.cur || run2.cur)
    {
        // cpu1 khali hoga to uspe dalenge
        if (!run1.cur && !ready.empty())
        {
            pr top = ready.front();
            ready.pop();
            temp1 = "P";
            int burst = top.first;
            Process *proc = top.second;

            run1.cur = proc;
            run1.run = burst;
            run1.ts = timeSlice;
            temp1 += to_string(proc->p_no);
            temp1 += ",";
            temp1 += to_string(((proc->index / 2) + 1));
            temp1 += "    ";
            temp1 += to_string(time);
            temp1 += "    ";
            //output1 << "P" << proc->p_no << "," << (proc->index / 2) + 1 << "    " << time << "     ";
        }

        // cpu2 khali hoga to uspe dalenge
        if (!run2.cur && !ready.empty())
        {
            pr top = ready.front();
            ready.pop();
            int burst = top.first;
            Process *proc = top.second;
            temp2 = "P";
            run2.cur = proc;
            run2.run = burst;
            run2.ts = timeSlice;
            temp2 += to_string(proc->p_no);
            temp2 += ",";
            temp2 += to_string(((proc->index / 2) + 1));
            temp2 += "    ";
            temp2 += to_string(time);
            temp2 += "    ";
            //output2 << "P" << proc->p_no << "," << (proc->index / 2) + 1 << "    " << time << "     ";
        }

        // Execute CPU1
        if (run1.cur)
        {
            run1.run--;
            run1.ts--;
            if (run1.run == 0)
            {
                temp1 += to_string(time);
                temp1 += "\n";
                vec1.push_back(temp1);
                //output1 << time  << endl;
                run1.cur->index++;
<<<<<<< HEAD
                if (run1.cur->index < run1.cur->bursts.size()){
                    waiting.push(make_pair(time + 1 + run1.cur->bursts[run1.cur->index], run1.cur));
=======
                if (run1.cur->index < run1.cur->bursts.size())
                {
                    waiting.push({time + 1 + run1.cur->bursts[run1.cur->index], run1.cur});
>>>>>>> 255da247d97f2a56ed4b6dbc8c8c74d53221f47f
                }
                else{
                    run1.cur->completion = time+1;
                }
                run1.cur = nullptr;
            }
            else if (run1.ts == 0)
            {
                temp1 += to_string(time);
                temp1 += "\n";
                vec1.push_back(temp1);
<<<<<<< HEAD
                //output1 << time  << endl;
                ready.push(make_pair(run1.run, run1.cur));
=======
                // output1 << time  << endl;
                ready.push({run1.run, run1.cur});
>>>>>>> 255da247d97f2a56ed4b6dbc8c8c74d53221f47f
                run1.cur = nullptr;
            }
        }

        // Execute CPU2
        if (run2.cur)
        {
            run2.run--;
            run2.ts--;
            if (run2.run == 0)
            {
                temp2 += to_string(time);
                temp2 += "\n";
                vec2.push_back(temp2);
                //output2 << time  << endl;
                run2.cur->index++;
<<<<<<< HEAD
                if (run2.cur->index < run2.cur->bursts.size()){
                    waiting.push(make_pair(time + 1 + run2.cur->bursts[run2.cur->index], run2.cur));
=======
                if (run2.cur->index < run2.cur->bursts.size())
                {
                    waiting.push({time + 1 + run2.cur->bursts[run2.cur->index], run2.cur});
>>>>>>> 255da247d97f2a56ed4b6dbc8c8c74d53221f47f
                }
                else{
                    run2.cur->completion = time + 1;
                }
                run2.cur = nullptr;
            }
            else if (run2.ts == 0)
            {
                temp2 += to_string(time);
                temp2 += "\n";
                vec2.push_back(temp2);
<<<<<<< HEAD
                //output2 << time  << endl;
                ready.push(make_pair(run2.run, run2.cur));
=======
                // output2 << time  << endl;
                ready.push({run2.run, run2.cur});
>>>>>>> 255da247d97f2a56ed4b6dbc8c8c74d53221f47f
                run2.cur = nullptr;
            }
        }
        if (run1.cur == nullptr && run2.cur == nullptr)
        {
            idleTime++;
            //cout<<time<<" ";
        }
        // waiting se ready me if possible
        while (!waiting.empty() && waiting.top().first <= time + 1)
        {
            Process *temp = waiting.top().second;
            waiting.pop();
            temp->index++;
            if (temp->index < temp->bursts.size())
                ready.push({temp->bursts[temp->index], temp});
        }
        while (i < n && parsed_data[i]->arrival <= time + 1)
        {
            ready.push({parsed_data[i]->bursts[0], parsed_data[i]});
            i++;
        }

        time++;
    }
    cout <<"Scheduling Algorithm: Round Robin"<<endl;
    cout <<"Time Slice: "<<timeSlice<<endl;
    cout << "Time to complete: " << time << endl;
    //output1.close();
    //output2.close();
    output << "CPU0\n";
    for(auto it: vec1){
        output << it;
    }
    output << "CPU1\n";
    for(auto it: vec2){
        output << it;
    }
    output.close();
    float turnAround = 0;
    int maxTAT = 0;
    for(auto it: parsed_data){
        int TAT = it->completion - it->arrival;
        maxTAT = max(maxTAT, TAT);
        turnAround += TAT;
    }
<<<<<<< HEAD
    cout<< "Average Turnaround Time: "<<turnAround/n<<endl;
    cout<< "Maximum Turnaround Time: "<<maxTAT<<endl;
=======
    cout << "Average Turnaround Time: " << turnAround / n << endl;
    cout << "Maximum Turnaround Time: " << maxTAT << endl;

    auto idle = countGapsMC(fileName);
    cout << "Runtime of Simulator(CPU1): " << time - idle.first << endl;
    cout << "Runtime of Simulator(CPU2): " << time - idle.second << endl;
>>>>>>> 255da247d97f2a56ed4b6dbc8c8c74d53221f47f
    return;
}


//  void MultiCoreProcessor::NPSJF(vector<Process*> parsed_data, char *process_file){
//     ofstream output1;

//     string filename = process_file;
//     string timeline1 = "ScheduleMC(PSJF)";
 
//     timeline1 += filename[7];

//     timeline1 += ".txt";

//     output1.open(timeline1);

    // // Push all processes that have arrived by current time into ready queue
    // for (; i < n; i++)
    // {
    //     Process *proc = parsed_data[i];
    //     if (proc->arrival <= time)
    //         ready.push({proc->bursts[0], proc});
    //     else
    //         break;
    // }

//     int time = parsed_data[0]->arrival-1;
//     priority_queue<pair<int,Process *>,vector<pair<int,Process*>>,greater<pair<int,Process*>>> ready;
//     vector<string> ans0;
//     vector<string> ans1;
//     priority_queue<pair<int,Process *>,vector<pair<int,Process*>>,greater<pair<int,Process*>>> waiting;
//     ready.push({parsed_data[0]->bursts[0],parsed_data[0]});
//     int i = 1;
//     int maxTAT = 0;
//     float avgTAT = 0;
//     int run1 = 0;
//     Process *p1 = NULL;
//     Process *p2 = NULL;
//     int run2 = 0;
//     string temp1 = "";
//     string temp2 = "";
//     while (!ready.empty() || !waiting.empty() || run1 != 0 || run2 != 0)
//     {
//         time++;
//         if (run1 > 0){
//             run1--;
//         }
//         if (run2 > 0){
//             run2--;
//         }

//         for (; i < parsed_data.size(); i++)
//         {
//             if (parsed_data[i]->arrival <= time)
//             {
//                 ready.push({parsed_data[i]->bursts[0],parsed_data[i]});
//             }
//             else
//                 break;
//         }
//         while (!waiting.empty())
//         {
//             Process *top = waiting.top().second;
//             int index = top->index;
//             int ioBurst = top->bursts[index];
//             int last = top->wait;
//             if (last + ioBurst <= time)
//             {
//                 top->index++;
//                 waiting.pop();
//                 ready.push({top->bursts[top->index],top});
//             }
//             else
//             {
//                 break;
//             }
//         }
//         if (run1 == 0)
//         {
//             if (p1)
//             {
//                 int ind = p1->index;
//                 ind += 1;
//                 temp1 += to_string(time-1);
//                 ans0.push_back(temp1);
//                 temp1 = "";
//                 if (ind < p1->bursts.size())
//                 {
//                     p1->index = ind;
//                     p1->wait = time;
                    
//                     waiting.push({time+p1->bursts[ind],p1});
//                 }
//                 else
//                 {
//                     p1->completion = time;
//                 }
//             }
//             p1 = NULL;
//             if (!ready.empty())
//             {
//                 Process *top = ready.top().second;
//                 ready.pop();
//                 int index = top->index;
//                 run1 = top->bursts[index];
//                 p1 = top;
//                 temp1 += "P";
//                 temp1 += to_string(p1->p_no);
//                 temp1 += ",";
//                 temp1 += to_string((p1->index / 2) + 1);
//                 temp1 += "      ";
//                 temp1 += to_string(time);
//                 temp1 += "      ";
//             }
//         }
//         if (run2 == 0)
//         {
//             if (p2)
//             {
//                 int ind = p2->index;
//                 ind += 1;
//                 temp2 += to_string(time-1);
//                 ans1.push_back(temp2);
//                 temp2 = "";
//                 if (ind < p2->bursts.size())
//                 {
//                     p2->index = ind;
//                     p2->wait = time;
                    
//                     waiting.push({time+p2->bursts[ind],p2});
//                 }
//                 else
//                 {
//                     p2->completion = time;
//                 }
//             }
//             p2 = NULL;
//             if (!ready.empty())
//             {
//                 Process *top = ready.top().second;
//                 ready.pop();
//                 int index = top->index;
//                 run2 = top->bursts[index];
//                 p2 = top;
//                 temp2 += "P";
//                 temp2 += to_string(p2->p_no);
//                 temp2 += ",";
//                 temp2 += to_string((p2->index / 2) + 1);
//                 temp2 += "      ";
//                 temp2 += to_string(time);
//                 temp2 += "      ";
//             }
//         }
//     }
//     output1 <<"CPU0\n";
//     for(int i = 0; i< ans0.size(); i++){
//         output1 << ans0[i] <<endl;
//     }
//     output1 <<"CPU1\n";
//     for(int i = 0; i< ans1.size(); i++){
//         output1 << ans1[i] <<endl;
//     }
//     cout <<"Time: "<< time << endl;
//     output1.close();
//     return;
// }

// void MultiCoreProcessor::RR(vector<Process *> parsed_data, char *process_file, int timeSlice)
// {
//     ofstream output;
//     string fileName = "ScheduleMC(RR)";
//     fileName += process_file[7];
//     fileName += ".txt";
//     output.open(fileName);
//     vector<string> vec1;
//     vector<string> vec2;
//     using pr = pair<int, Process *>;
//     queue<pr> ready;
//     priority_queue<pr, vector<pr>, greater<pr>> waiting;

//     int time = parsed_data[0]->arrival;
//     int i = 0;
//     int n = parsed_data.size();
//     string temp1 = "";
//     string temp2 = "";
//     while (i < n && time >= parsed_data[i]->arrival)
//     {
//         ready.push({parsed_data[i]->bursts[0], parsed_data[i]});
//         i++;
//     }

//     RRun run1;
//     RRun run2;

//     while (!ready.empty() || !waiting.empty() || run1.cur || run2.cur)
//     {
//         // cpu1 khali hoga to uspe dalenge
//         if (!run1.cur && !ready.empty())
//         {
//             pr top = ready.front();
//             ready.pop();
//             temp1 = "P";
//             int burst = top.first;
//             Process *proc = top.second;

//             run1.cur = proc;
//             run1.run = burst;
//             run1.ts = timeSlice;
//             temp1 += to_string(proc->p_no);
//             temp1 += ",";
//             temp1 += to_string(((proc->index / 2) + 1));
//             temp1 += "    ";
//             temp1 += to_string(time);
//             temp1 += "    ";
//             //output1 << "P" << proc->p_no << "," << (proc->index / 2) + 1 << "    " << time << "     ";
//         }

//         // cpu2 khali hoga to uspe dalenge
//         if (!run2.cur && !ready.empty())
//         {
//             pr top = ready.front();
//             ready.pop();
//             int burst = top.first;
//             Process *proc = top.second;
//             temp2 = "P";
//             run2.cur = proc;
//             run2.run = burst;
//             run2.ts = timeSlice;
//             temp2 += to_string(proc->p_no);
//             temp2 += ",";
//             temp2 += to_string(((proc->index / 2) + 1));
//             temp2 += "    ";
//             temp2 += to_string(time);
//             temp2 += "    ";
//             //output2 << "P" << proc->p_no << "," << (proc->index / 2) + 1 << "    " << time << "     ";
//         }

//         // Execute CPU1
//         if (run1.cur)
//         {
//             run1.run--;
//             run1.ts--;
//             if (run1.run == 0)
//             {
//                 temp1 += to_string(time);
//                 temp1 += "\n";
//                 vec1.push_back(temp1);
//                 //output1 << time  << endl;
//                 run1.cur->index++;
//                 if (run1.cur->index < run1.cur->bursts.size()){
//                     waiting.push(make_pair(time + 1 + run1.cur->bursts[run1.cur->index], run1.cur));
//                 }
//                 else{
//                     run1.cur->completion = time+1;
//                 }
//                 run1.cur = nullptr;
//             }
//             else if (run1.ts == 0)
//             {
//                 temp1 += to_string(time);
//                 temp1 += "\n";
//                 vec1.push_back(temp1);
//                 //output1 << time  << endl;
//                 ready.push(make_pair(run1.run, run1.cur));
//                 run1.cur = nullptr;
//             }
//         }

//         // Execute CPU2
//         if (run2.cur)
//         {
//             run2.run--;
//             run2.ts--;
//             if (run2.run == 0)
//             {
//                 temp2 += to_string(time);
//                 temp2 += "\n";
//                 vec2.push_back(temp2);
//                 //output2 << time  << endl;
//                 run2.cur->index++;
//                 if (run2.cur->index < run2.cur->bursts.size()){
//                     waiting.push(make_pair(time + 1 + run2.cur->bursts[run2.cur->index], run2.cur));
//                 }
//                 else{
//                     run2.cur->completion = time + 1;
//                 }
//                 run2.cur = nullptr;
//             }
//             else if (run2.ts == 0)
//             {
//                 temp2 += to_string(time);
//                 temp2 += "\n";
//                 vec2.push_back(temp2);
//                 //output2 << time  << endl;
//                 ready.push(make_pair(run2.run, run2.cur));
//                 run2.cur = nullptr;
//             }
//         }

//         // waiting se ready me if possible
//         while (!waiting.empty() && waiting.top().first <= time + 1)
//         {
//             Process *temp = waiting.top().second;
//             waiting.pop();
//             temp->index++;
//             if (temp->index < temp->bursts.size())
//                 ready.push(make_pair(temp->bursts[temp->index], temp));
//         }
//         while (i < n && parsed_data[i]->arrival <= time + 1)
//         {
//             ready.push(make_pair(parsed_data[i]->bursts[0], parsed_data[i]));
//             i++;
//         }

//         time++;
//     }
//     cout <<"Scheduling Algorithm: Round Robin"<<endl;
//     cout <<"Time Slice: "<<timeSlice<<endl;
//     cout << "Time to complete: " << time << endl;
//     //output1.close();
//     //output2.close();
//     output << "CPU0\n";
//     for(auto it: vec1){
//         output << it;
//     }
//     output << "CPU1\n";
//     for(auto it: vec2){
//         output << it;
//     }
//     output.close();
//     float turnAround = 0;
//     int maxTAT = 0;
//     for(auto it: parsed_data){
//         int TAT = it->completion - it->arrival;
//         maxTAT = max(maxTAT, TAT);
//         turnAround += TAT;
//     }
//     cout<< "Average Turnaround Time: "<<turnAround/n<<endl;
//     cout<< "Maximum Turnaround Time: "<<maxTAT<<endl;
//     return;
// }
