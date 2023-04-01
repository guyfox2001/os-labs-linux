#include <iostream>
#include <filesystem>
#include <vector>
#include <algorithm>
#include <fstream>
#include <unistd.h>
#include <sys/wait.h>


using namespace std;
using namespace filesystem;


struct statm{
    int size, resident, shared, trs, lrs, drs, dt;

    friend istream& operator>>(istream& in, statm& obj){
        in >> obj.size >> obj.size >> obj.resident >> obj.resident >> obj.shared >> obj.trs >> obj.lrs >> obj.drs >> obj.dt;
        return in;
    }

    friend ostream &operator<<(ostream &os, const statm &statm) {
        os << "size: " << statm.size << '\n' << " resident: " << statm.resident << '\n' << " shared: " << statm.shared << '\n' << " trs: "
           << statm.trs << '\n' << " lrs: " << statm.lrs << '\n' << " drs: " << statm.drs << '\n' << " dt: " << statm.dt << '\n';
        return os;
    }
};

struct proc_handler{
    int pid;
    bool is_sbin;
    statm _statm;
    string cmdline;
    friend ostream &operator<<(ostream &os, const proc_handler &handler) {
        os << "pid: " << handler.pid << " is_sbin: " << handler.is_sbin << " _statm: \n\t" << handler._statm;
        return os;
    }
};

bool is_number(const std::string& s)
{
    std::string::const_iterator it = s.begin();
    while (it != s.end() && std::isdigit(*it)) ++it;
    return !s.empty() && it == s.end();
}


statm get_statm(int pid){
    statm res{};
    ifstream _statm("/proc/" + to_string(pid) + "/_statm");
    if(_statm.is_open()){
        _statm >> res;
        _statm.close();
    }
    return res;
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

vector<proc_handler> get_proc_handler(){
    vector<proc_handler> result;
    for(const auto& entry : filesystem::directory_iterator("/proc")) {
        string target =string(entry.path().filename().c_str());
        if(is_number(target)) {
            proc_handler handler{};
            handler.pid = atoi(target.c_str());
            handler.cmdline = get_cmdline(handler.pid);
            if(handler.cmdline.find("sbin") != string::npos)
                handler.is_sbin = true;
            handler._statm = get_statm(handler.pid);
            result.push_back(handler);
            cout << handler;
        }
    }
    return  result;
}


int main() {
    vector<proc_handler> handler = get_proc_handler();
    ofstream out;
    out.open("/home/vattghern/pids-from-system.txt");
    if(out.is_open()) {
        for (auto it: handler) {
            out << it.pid << '\n';
        }
        out.close();
    }

    out.open("/home/vattghern/sbin-pids-from-system.txt");
    if(out.is_open()) {
        for (auto it: handler) {
            if(it.is_sbin)
                out << it.pid << '\n';
        }
        out.close();
    }

    out.open("/home/vattghern/pid-cmd-line.txt");
    if (out.is_open()){
        for (auto it : handler) out << it  << ":" << (it.cmdline.size() == 0 ? "none" : it.cmdline) << '\n';
        out.close();
    }
    return 0;
}
