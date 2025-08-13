#include <iostream>
#include <algorithm>
#include <vector>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <cstring>  // for strdup

using namespace std;

pid_t pid1, pid2;

void handler(int sign){
    if(sign == SIGUSR1)
    cout<<"----"<<getpid()<<" "<<sign<<"---"<<endl;
    kill(pid1,SIGUSR1);
    kill(pid2,SIGUSR2);
    kill(getpid(),SIGINT);
}

int size = 32;

int main()
{
    signal(SIGUSR1,handler);
    int size = 128;
    
    cout<<getpid()<<"--parent pid--"<<endl;
    int start = 0;
    int end = 127;
    while(size>32){
        pid1 = fork();
        if(pid1 != 0){
            //and this block is for right child
            pid2 = fork();
            size = size/2;
            start = (start + end)/2 +1;
        }
        else{
            // i think this one block is for left child
            size = size/2;
            end = (start + end)/2;
        }
        if(pid1 != 0 && pid2 != 0){
        //    waitpid(pid1, NULL, 0);
        //    waitpid(pid2, NULL, 0);
            wait(NULL);
            return 0;
        }
    }
    kill(getppid(),SIGUSR1);
    cout<<"PID: "<<getpid()<<" SIZE: "<<size<<" Start "<<start<<" End "<<end<<endl;
    return 0;
}