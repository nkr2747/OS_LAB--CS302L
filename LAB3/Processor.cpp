#include <iostream>
#include <algorithm>
#include <vector>
#include "Processor.h"
#include <fstream>
#include <queue>
#include <string>
using namespace std;

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

void Processor::FIFO(vector<Process *> parsed_data, char *process_file)
{
    ofstream output;
    string filename = process_file;
    string timeline = "Schedule(FIFO)";
    timeline += filename[7];
    timeline += ".txt";
    output.open(timeline);
    int time = parsed_data[0]->arrival;
    queue<Process *> ready;
    queue<Process *> waiting;
    ready.push(parsed_data[0]);
    int i = 1;
    int maxTAT = 0;
    float avgTAT = 0;
    output << "CPU0\n";
    while (!ready.empty() || !waiting.empty())
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
            waiting.push(cur);
        }
        else
        {
            cur->completion = time;
            avgTAT += cur->completion - cur->arrival;
            maxTAT = max(maxTAT, cur->completion - cur->arrival);
        }
        // maybe yahan pe main priority queue add karunga kyuki nikalne me eeasy hoga.
        while (!waiting.empty())
        {
            Process *top = waiting.front();
            if (time - top->wait >= top->bursts[top->index])
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
    output << "Maximum TAT: " << maxTAT << endl;
    output << "Average TAT: " << avgTAT / (float)parsed_data.size();
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
    queue<Process *> waiting;

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
                waiting.push(cur);
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
            Process *top = waiting.front();
            if (time - top->wait < top->bursts[top->index])
            {
                time = top->bursts[top->index] + top->wait;
            }
        }
        while (!waiting.empty())
        {
            Process *top = waiting.front();
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
    output << "Maximum TAT: " << maxTAT << endl;
    output << "Average TAT: " << avgTAT / (float)parsed_data.size();
    output.close();
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

        cout << "Ready: " << ready.size() << " waiting: " << waiting.size() << endl;
        if (!ready.empty())
        {
            Process *top = ready.top().second;
            int timeLeft = ready.top().first;
            ready.pop();
            int ind = top->index;
            pair<int, int> cur = {top->p_no, (ind / 2) + 1};
            if (prev != cur)
            {
                if(prev.first != -1)
                output<<"       "<<time-1<<endl;
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

        if (!ready.empty())
        {
            cout << ready.top().first << " " << ready.top().second->p_no << endl;
        }
    }
    // output << "Maximum TAT: " << maxTAT << endl;
    // output << "Average TAT: " << avgTAT / (float)parsed_data.size();
    output<<"       "<<time-1<<endl;
    output.close();
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
    queue<pair<int,Process*>> ready;
    priority_queue<pair<int,Process*>,vector<pair<int,Process*>>,greater<pair<int,Process*>>> waiting;
    int i;
    for(i = 0; i< parsed_data.size(); i++){
        if(parsed_data[i]->arrival <= time){
            ready.push({parsed_data[i]->bursts[0],parsed_data[i]});
        }
        else break;
    }
    int timeLeft = -1;
    Process* top = NULL;
    while(!waiting.empty() || !ready.empty()){
        //cout<< waiting.size() <<" "<<ready.size()<<endl;
        if(!ready.empty()){
            timeLeft = ready.front().first;
            //cout<<timeLeft<<endl;
            top = ready.front().second;
            ready.pop();
            output << "P" <<top->p_no<< ",      "<<time<<"      ";
            if(timeLeft > timeSlice){
                timeLeft -= timeSlice;
                time += timeSlice;
                //ready.push({timeLeft,top});
            }
            else{
                time += timeLeft;
                timeLeft -= timeSlice;
                int ind = top->index;
                if(ind + 1 < top->bursts.size()){
                    top->index = ind +1;
                    top->wait = time;
                    waiting.push({time+top->bursts[top->index],top});
                }
            }
            output<<time<<endl;
        }
        else time++;
        for(;i < parsed_data.size(); i++){
            if(parsed_data[i]->arrival <= time){
                //cout<<parsed_data[i]->bursts[0]<<endl;
                ready.push({parsed_data[i]->bursts[0],parsed_data[i]});
            }
            else break;
        }
        while(!waiting.empty()){
            Process* top = waiting.top().second;
            int estimateTime = waiting.top().first;
            if(time >= estimateTime){
                waiting.pop();
                top->index++;
                ready.push({top->bursts[top->index],top});
            }
            else break;
        }
        if(timeLeft > 0) ready.push({timeLeft,top});
    }
}