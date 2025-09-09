#include <iostream>
#include <algorithm>
#include <vector>
#include <fstream>
#include <string>
#include <queue>
using namespace std;

struct pcb_t
{
    int arrival_time;
    int scheduled_time;
    int completion_time;
    int wait_time;
    int cur_pos;
    vector<int> bursts;
};

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        cout << "Invalid format!\nIt should be in:-\n";
        cout << "./main <scheduler-type> <test-file>";
        return 0;
    }
    char *process_file = argv[2];
    char *scheduling_algorithm = argv[1];
    ifstream file;
    file.open(process_file);
    int num;
    vector<struct pcb_t *> processes;
    while (!file.eof())
    {
        num = -2;
        struct pcb_t *temp = new pcb_t;
        while (num != -1)
        {
            if (num == -2)
            {
                file >> num;
                temp->arrival_time = num;
                temp->cur_pos = 0;
                continue;
            }
            file >> num;
            if (num != -1)
                temp->bursts.push_back(num);
        }
        processes.push_back(temp);
    }
    // for (int i = 0; i < processes.size(); i++)
    // {
    //     cout << processes[i]->arrival_time << " ";
    //     for (auto it : processes[i]->bursts)
    //     {
    //         cout << it << " ";
    //     }
    //     cout << endl;
    // }
    int cur_time = processes[0]->arrival_time;
    queue<struct pcb_t *> waiting_q;
    queue<struct pcb_t *> ready_q;
    ready_q.push(processes[0]);
    int cur_process = 1;
    while (!ready_q.empty() || !waiting_q.empty())
    {
        //cout<<cur_time<<" ";
        struct pcb_t *top = ready_q.front();
        ready_q.pop();
        if (cur_time < top->arrival_time)
        {
            cur_time += top->arrival_time - cur_time;
        }
        if(top->cur_pos == 0){
            top->scheduled_time = cur_time;
        }
        cur_time += top->bursts[top->cur_pos];
        top->cur_pos++;
        if (top->cur_pos < top->bursts.size())
        {
            top->wait_time = cur_time;
            waiting_q.push(top);
        }
        else{
            top->completion_time = cur_time;
        }

        for (; cur_process < processes.size(); cur_process++)
        {
            if (processes[cur_process]->arrival_time <= cur_time)
            {
                ready_q.push(processes[cur_process]);
            }
            else
            {
                break;
            }
        }
        while (!waiting_q.empty())
        {
            struct pcb_t *top = waiting_q.front();
            if (top->wait_time + top->bursts[top->cur_pos] <= cur_time)
            {
                waiting_q.pop();
                top->cur_pos++;
                if (top->cur_pos < top->bursts.size())
                {
                    ready_q.push(top);
                }
            }
            else{
                break;
            }
        }
    }
    float avg_completion_time = 0;
    int max_completion_time = 0;
    float avg_waiting_time = 0;
    int max_waiting_time = 0;

    for(auto it: processes){
        int total_completion_time =it->completion_time - it->arrival_time;
        int cpu_time = 0;
        for(int i = 0; i<it->bursts.size(); i+= 1){
            cpu_time += it->bursts[i];
        }
        int waiting_time = total_completion_time - cpu_time;
        //cout<<total_completion_time<<endl;
        if(max_waiting_time<waiting_time){
            max_waiting_time = waiting_time;
        }
        avg_waiting_time += waiting_time;
        if(max_completion_time < total_completion_time){
            max_completion_time = total_completion_time;
        }
        avg_completion_time += total_completion_time;
    }
    avg_completion_time /= processes.size();
    avg_waiting_time /= processes.size();
    cout<<"Max Completion time: "<<max_completion_time<<endl;
    cout<<"Average Completion time: "<<avg_completion_time<<endl;
    cout<<"Max Waiting time: "<<max_waiting_time<<endl;
    cout<<"Average Waiting time: "<<avg_waiting_time<<endl;
    return 0;
}