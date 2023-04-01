#include <iostream>
#include <filesystem>
#include <vector>
#include <algorithm>
#include <fstream>


using namespace std;
using namespace filesystem;

bool is_number(const std::string& s)
{
    std::string::const_iterator it = s.begin();
    while (it != s.end() && std::isdigit(*it)) ++it;
    return !s.empty() && it == s.end();
}

vector<int> get_proc(){
    vector<int> result;
    for(auto entry : filesystem::directory_iterator("/proc")) {
        string target =string(entry.path().filename().c_str());
        if(is_number(target))
            result.push_back(atoi(target.c_str()));
    }
    return  result;
}

string get_cmdline(int pid){
    string res;
    ifstream cmdline("/proc/" + to_string(pid) +"/cmdline");
    if (cmdline.is_open()){
        res.clear();
        getline(cmdline, res);
    }
    return res;
}

int main() {
    get_proc();
    ofstream out;
    out.open("/home/vattghern/pid-cmd-line.txt");
    if (out.is_open()){
        for (auto it : get_proc()) out << it  << ":" << (get_cmdline(it).size() == 0 ? "none" : get_cmdline(it)) << '\n';
    }
    out.close();
    return 0;
}
