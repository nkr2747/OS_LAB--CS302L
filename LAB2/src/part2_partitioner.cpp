#include <iostream>
#include <fstream>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>

using namespace std;

int main(int argc, char **argv)
{
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
	while (cur_chunk_size > max_chunk_size)
	{
		cout<<"["<<getpid()<<"] start position = "<<search_start_position<<" ; end position = "<<search_end_position<<endl;
		pid1 = fork();
		if (pid1 != 0)
		{
			// and this block is for right child
			cout<<"["<<getpid()<<"] forked left child "<<pid1<<endl;
			pid2 = fork();
			if (pid2 == 0)
			{
				cur_chunk_size = cur_chunk_size / 2;
				search_start_position = search_start_position+ cur_chunk_size ;
			}
			else{
				cout<<"["<<getpid()<<"] forked right child "<<pid2<<endl;
			}
		}
		else
		{
			// i think this one block is for left child
			cur_chunk_size = cur_chunk_size / 2;
			search_end_position = search_start_position + cur_chunk_size -1;
		}
		if (pid1 != 0 && pid2 != 0)
		{
			waitpid(pid1, NULL, 0);
			cout<<"["<<getpid()<<"] left child returned"<<endl;
			waitpid(pid2, NULL, 0);
			cout<<"["<<getpid()<<"] right child returned"<<endl;
			//wait(NULL);
			return 0;
		}
	}
	const char *program_path = "./part2_searcher";

	// Arguments to pass to the program (first argument is the program name itself)
	// The list must be terminated by a NULL pointer.
	string start_str = to_string(search_start_position);
	string end_str = to_string(search_end_position);
	pid_t searcher_child = fork();
	if(searcher_child == 0){
		execl(program_path, program_path, file_to_search_in, pattern_to_search_for, start_str.c_str(), end_str.c_str(), NULL);
	}
	else{
		cout<<"["<<getpid()<<"] forked searcher child "<<searcher_child<<endl;
		wait(NULL);
		cout<<"["<<getpid()<<"] searcher child returned"<<endl;
	}

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
