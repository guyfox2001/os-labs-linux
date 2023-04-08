#include <iostream>
#include <unistd.h>
#include <fstream>
using namespace std;


int main(int argc, char** argv) {
    string file = argv[1];
    std::cout << file << std::endl;
    ofstream out(file);
    if(out.is_open()){
        out << "Hello, ";
        out.close();
    }
    int child_pid = fork();
    if (child_pid == 0){
        out.open(file, ios_base::app);
        if(out.is_open()){
            out << "world;\n";
            out.close();
        }
    }
    return 0;
}
