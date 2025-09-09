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
    output << "       " << time - 1 << endl;
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
            output << "P" << top->p_no << ",      " << time << "      ";
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
            }
            output << time << endl;
        }
        else
            time++;
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
}

void MultiCoreProcessor::FIFO(vector<Process *> parsed_data, char *process_file)
{
     ofstream output1;
    // ofstream output2;
    string filename = process_file;
    string timeline1 = "ScheduleMC(FIFO)";
    //string timeline2 = "ScheduleMC1(FIFO)";
    timeline1 += filename[7];
    //timeline2 += filename[7];
    timeline1 += ".txt";
    //timeline2 += ".txt";
    output1.open(timeline1);
    //output2.open(timeline2);

    int time = parsed_data[0]->arrival-1;
    queue<Process *> ready;
    vector<string> ans0;
    vector<string> ans1;
    priority_queue<pair<int,Process *>,vector<pair<int,Process*>>,greater<pair<int,Process*>>> waiting;
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
        if (run1 > 0){
            run1--;
        }
        if (run2 > 0){
            run2--;
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
                temp1 += to_string(time-1);
                ans0.push_back(temp1);
                temp1 = "";
                //output1 << time-1<<endl;
                //cout << time<<endl;
                if (ind < p1->bursts.size())
                {
                    p1->index = ind;
                    p1->wait = time;
                    
                    waiting.push({time+p1->bursts[ind],p1});
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
                //output1<<"P"<< p1->p_no <<","<<(p1->index / 2) + 1<<"     "<<time<<"      ";
                //cout<<p1->index << "P"<< p1->p_no <<","<<(p1->index / 2) + 1<<"     "<<time<<"      ";
                // run1.p = top;
                // cout<<run1.run<<endl;
            }
        }
        if (run2 == 0)
        {
            if (p2)
            {
                int ind = p2->index;
                ind += 1;
                //output2 << time-1<<endl;
                temp2 += to_string(time-1);
                ans1.push_back(temp2);
                temp2 = "";
                if (ind < p2->bursts.size())
                {
                    p2->index = ind;
                    p2->wait = time;
                    
                    waiting.push({time+p2->bursts[ind],p2});
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
                //output2<<"P"<< p2->p_no <<","<<(p2->index / 2) + 1<<"     "<<time<<"      ";
                // run2.p = top;
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
        
        //cout<<run1<<" "<<run2<<endl;
        
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
    output1 <<"CPU0\n";
    for(int i = 0; i< ans0.size(); i++){
        output1 << ans0[i] <<endl;
    }
    output1 <<"CPU1\n";
    for(int i = 0; i< ans1.size(); i++){
        output1 << ans1[i] <<endl;
    }
    cout <<"Time: "<< time << endl;
    output1.close();
    //output2.close();
    // for (int i = 0; i < parsed_data.size(); i++)
    // {
    //     cout<<parsed_data[i]->p_no <<"--"<<parsed_data[i]->completion -  parsed_data[i]->arrival<< endl;
    // }
    // output << "Maximum TAT: " << maxTAT << endl;
    // output << "Average TAT: " << avgTAT / (float)parsed_data.size();
    // output.close();
    return;
}