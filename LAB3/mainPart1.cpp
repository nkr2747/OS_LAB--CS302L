#include <iostream>
#include <bits/stdc++.h>
#include <fstream>
#include "Classes/Processor.h"
//#include "MultiCoreProcessor.h"
using namespace std;

vector<Process *> parser(string location)
{
    ifstream file;
    file.open(location);
    string line = "";
    int count = 0;
    // char first ;
    vector<Process *> parsed_data;
    while (line != "<pre>")
    {
        getline(file, line);
        // cout<<line<<" ";
        count++;
        if (count > 3)
            return parsed_data;
    }
    int p_count = 1;
    while (!file.eof())
    {
        Process *temp = new Process();
        string first;
        file >> first;
        if (first == "</pre></body></html>")
        {
            break;
        }
        temp->arrival = stoi(first);
        // cout<<temp->arrival<<" ";
        int burst;
        file >> burst;

        while (burst != -1)
        {
            // cout<<burst<<" ";
            temp->bursts.push_back(burst);
            file >> burst;
        }
        // file>>burst;
        temp->p_no = p_count;
        p_count++;
        parsed_data.push_back(temp);
    }
    return parsed_data;
}

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        // cout<<argc;
        cout << "Invalid format!\nIt should be in:-\n";
        cout << "./main <scheduler-type>(FIFO/NPSJF/PSJF/RR) <test-file>";
        return 0;
    }
    char *process_file = argv[2];
    char *scheduling_algorithm = argv[1];
    vector<Process *> parsed_data = parser(process_file);
    Processor cpu0;
    // MultiCoreProcessor cpu1;
    // cpu0.RR(parsed_data,process_file,10);
    //cpu0.FIFO(parsed_data,process_file);
    //return 0;
    string algo = scheduling_algorithm;
    //display(parsed_data);
    unordered_map<string,int> mpp;
    mpp["FIFO"] = 1;
    mpp["NPSJF"] = 2;
    mpp["PSJF"] = 3;
    mpp["RR"] = 4;
    switch (mpp[algo])
    {
    case 1:
        cpu0.FIFO(parsed_data,process_file);
        break;
    case 2:
        cpu0.NPSJF(parsed_data,process_file);
        break;
    case 3:
        cpu0.PSJF(parsed_data,process_file);
        break;
    case 4:
        cpu0.RR(parsed_data,process_file,10);
        break;
    
    default:
        break;
    }

    return 0;
}