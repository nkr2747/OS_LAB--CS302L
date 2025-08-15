#include <iostream>
#include <fstream>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include<vector>

using namespace std;

vector<pid_t> children;

void handler(int sign)
{
	if (sign == SIGUSR1)
	{
		cout<<children.size()<<" ";
		cout<<"ye?"<<endl;
	}
}

int main(int argc, char **argv)
{

	signal(SIGUSR1, handler);

	if (argc != 6)
	{
		cout << "usage: ./partitioner.out <path-to-file> <pattern> <search-start-position> <search-end-position> <max-chunk-size>\nprovided arguments:\n";
		for (int i = 0; i < argc; i++)
			cout << argv[i] << "\n";
		return -1;
	}

	char *file_to_search_in = argv[1];
	char *pattern_to_search_for = argv[2];
	int search_start_position = atoi(argv[3]);
	int search_end_position = atoi(argv[4]);
	int max_chunk_size = atoi(argv[5]);
	pid_t pid1, pid2;
	int cur_chunk_size = search_end_position - search_start_position + 1;
	pid_t leader_pid = getpid();
	while (cur_chunk_size > max_chunk_size)
	{
		pid1 = fork();
		if (pid1 != 0)
		{
			// thhis is still parent--, till now the variables will be in right child

			pid2 = fork();
			if (pid2 == 0)
			{
				// and this block is for right child
				setpgid(pid2,leader_pid);
				cur_chunk_size = cur_chunk_size / 2;
				search_start_position = search_start_position + cur_chunk_size;
			}
		}
		else
		{
			// i think this one block is for left child
			setpgid(pid1,leader_pid);
			cur_chunk_size = cur_chunk_size / 2;
			search_end_position = search_start_position + cur_chunk_size - 1;
		}
		if (pid1 != 0 && pid2 != 0)
		{
			//    waitpid(pid1, NULL, 0);
			//    waitpid(pid2, NULL, 0);
			wait(NULL);
			return 0;
		}
	}
	
	//cout<<"-"<<children.size()<<"-"<<endl;
	const char *program_path = "./part3_searcher";

	// Arguments to pass to the program (first argument is the program name itself)
	// The list must be terminated by a NULL pointer.
	string start_str = to_string(search_start_position);
	string end_str = to_string(search_end_position);
	string GID = to_string(leader_pid);
	execl(program_path, program_path, file_to_search_in, pattern_to_search_for, start_str.c_str(), end_str.c_str(), GID.c_str(), NULL);

	// TODO
	// cout << "[" << my_pid << "] start position = " << search_start_position << " ; end position = " << search_end_position << "\n";
	// cout << "[" << my_pid << "] forked left child " << my_children[0] << "\n";
	// cout << "[" << my_pid << "] forked right child " << my_children[1] << "\n";
	// cout << "[" << my_pid << "] left child returned\n";
	// cout << "[" << my_pid << "] right child returned\n";
	// cout << "[" << my_pid << "] left child returned\n";
	// cout << "[" << my_pid << "] right child returned\n";*/
	// cout << "[" << my_pid << "] forked searcher child " << searcher_pid << "\n";
	// cout << "[" << my_pid << "] searcher child returned \n";
	// cout << "[" << my_pid << "] received SIGTERM\n"; //applicable for Part III of the assignment

	return 0;
}
