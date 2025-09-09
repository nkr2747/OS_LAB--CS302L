#include <bitset>/stdc++.h>
#include <queue>
#include "MultiCoreProcessor.h"
#include <fstream>
using namespace std;

void MultiCoreProcessor::FIFO(vector<Process *> parsed_data, char *process_file)
{
    // ofstream output;
    // string filename = process_file;
    // string timeline = "Schedule(FIFO)";
    // timeline += filename[7];
    // timeline += ".txt";
    // output.open(timeline);
    int time = parsed_data[0]->arrival;
    queue<Process *> ready;
    queue<Process *> waiting;
    ready.push(parsed_data[0]);
    int i = 1;
    int maxTAT = 0;
    float avgTAT = 0;
    int run1 = 0;
    int run2 = 0;
    int time = 0;
    // output << "CPU0\n";
    while (!ready.empty() || !waiting.empty())
    {
        for(; i< parsed_data.size(); i++){
            if(parsed_data[i]->arrival <= time){
                ready.push(parsed_data[i]);
            }
            else break;
        }
        while(!waiting.empty()){
            Process* top = waiting.front();
            int index = top->index;
            int ioBurst = top->bursts[index];
            int last = top->wait;
            if(last + ioBurst <= time){
                waiting.pop();
                ready.push(top);
            }
            else{
                break;
            }
        }
        if (run1 == 0)
        {
            if (!ready.empty())
            {
                Process *top = ready.front();
                ready.pop();
                int index = top->index;
                run1 = top->bursts[index];
            }
        }
        if (run2 == 0)
        {
            if (!ready.empty())
            {
                Process *top = ready.front();
                ready.pop();
                int index = top->index;
                run2 = top->bursts[index];
            }
        }
        if(run1 > 0) run1--;
        if(run2 > 0) run2--;
        time++;
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
    cout<<time;
    // output << "Maximum TAT: " << maxTAT << endl;
    // output << "Average TAT: " << avgTAT / (float)parsed_data.size();
    // output.close();
    return;
}