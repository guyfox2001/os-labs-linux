#include <iostream>
#include <unistd.h>
#include <algorithm>
#include <sys/wait.h>


using namespace std;

bool is_number(const std::string& s)
{
    std::string::const_iterator it = s.begin();
    while (it != s.end() && std::isdigit(*it)) ++it;
    return !s.empty() && it == s.end();
}

int main(int argc, char** argv) {

    if (!is_number(string(argv[1])) || atoi(argv[1]) <=0){
        cerr << "Undefined argument\n";
        return 2;
    }

    int N = atoi(argv[1]);
    for(int i =0; i<N; i++){
        int pid = fork();
        int status;
        switch(pid){
            case 0:{
                cout << "Child pid: " << getpid() << '\n';
                return 0;
            }
            case -1: return 1;
            default:
                cout<< "Child is run: " << i << '\n';
                waitpid(pid, &status, 0);
                if (!WIFEXITED(status) || WEXITSTATUS(status) != 0) {
                    cerr << "Error: child process " << i << " (pid " << pid << ") failed\n";
                    return 1;
                }
        }
    }



    return 0;
}
