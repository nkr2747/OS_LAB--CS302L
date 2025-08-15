#include <iostream>
#include <fstream>
#include <cstring>
#include <unistd.h>
#include <signal.h>

using namespace std;

int main(int argc, char **argv)
{
    if (argc != 5)
    {
        cout << "usage: ./part2_searcher.out <path-to-file> <pattern> <search-start-position> <search-end-position>\nprovided arguments:\n";
        for (int i = 0; i < argc; i++)
            cout << argv[i] << "\n";
        return -1;
    }

    char *file_to_search_in = argv[1];
    char *pattern_to_search_for = argv[2];
    int search_start_position = atoi(argv[3]);
    int search_end_position = atoi(argv[4]);

    // TODO
    int len = 0;
    for(int i = 0; pattern_to_search_for[i] != '\0'; i++){
        len++;
    }
    ifstream textFile;
    textFile.open(file_to_search_in);
    textFile.seekg(search_start_position);
    string part_to_search_from = "";
    while (textFile.good() && textFile.tellg() <= search_end_position)
    {
        char c = textFile.get();
        part_to_search_from += c;
        //cout << c;
    }
    //cout << endl;
    //cout << part_to_search;
    char start_char = pattern_to_search_for[0];
    int patter_size = len;
    int window_size = part_to_search_from.size();
    for(int i = 0; i< window_size; i++){
        if(part_to_search_from[i] == start_char){
            bool flag = true;
            for(int j = 1; j<patter_size; j++){
                if(j+i>= window_size || pattern_to_search_for[j] != part_to_search_from[i+j]){
                    flag = false;
                    break;
                }
            }
            if(flag){
                auto pid = getpid();
                cout<<"["<<getpid()<<"] found at "<<i+search_start_position<<endl;
                return 0;
            }
        }
    }
    auto pid = getpid();
    cout<<"["<<getpid()<<"] didn't find"<<endl;
    return 0;
}