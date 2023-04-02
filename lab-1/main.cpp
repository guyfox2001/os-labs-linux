#include "types.h"
#include <filesystem>
#include <vector>
#include <algorithm>
#include <fstream>
#include <unistd.h>


using namespace std;
using namespace filesystem;


bool is_number(const std::string& s);
avg_atom get_avg_atom(int pid);
int get_ppid(int pid);
string get_cmdline(int pid);
vector<proc_handler> get_proc_handler();
sched_stat get_sched_stat(int pid);


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
        for (const auto& it : handler) out << it.pid << ":" << (it.cmdline.size() == 0 ? "none" : it.cmdline) << '\n';
        out.close();
    }

    out.open("/home/vattghern/abs-shared-resident.txt");
    if (out.is_open()){
        for(const auto& it: handler)
            out << it.pid << ":" << std::abs(it._statm.resident - it._statm.shared ) << "\n\tstatm:\n" << it._statm;
        out.close();
    }

    sort(handler.begin(), handler.end());
    out.open("/home/vattghern/resiter-proc.txt");
    if(out.is_open()){
        for(const auto &it : handler)
            out << it;
        out.close();
    }

    return 0;
}

bool is_number(const std::string& s)
{
    std::string::const_iterator it = s.begin();
    while (it != s.end() && std::isdigit(*it)) ++it;
    return !s.empty() && it == s.end();
}

statm get_statm(int pid){
    statm res{};
    ifstream _statm("/proc/" + to_string(pid) + "/statm");
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

int get_ppid(int pid){
    int res = -1;
    string line;
    ifstream stat("/proc/" + to_string(pid) +"/stat");
    if (stat.is_open()){
        int c=0;
        while (c!=4) {
            getline(stat, line, ' ');
            c++;
        }
        stat.close();
        res = atoi(line.c_str());
    }
    return res;
}

avg_atom get_avg_atom(int pid){
    avg_atom res={0, 0};
    string cmd = "cat /proc/" + to_string(pid) + "/sched | grep  -e se.sum -e nr_sw | grep -Eo '[0-9]+\\.?+[0-9]{1,100}' >> /home/vattghern/tmp-grep";
    execl(cmd.c_str(), nullptr);
    ifstream tmp("/home/vattghern/tmp-grep");
    if (tmp.is_open()){
        tmp >> res;
        tmp.close();
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
            handler.ppid = get_ppid(handler.pid);
            handler.cmdline = get_cmdline(handler.pid);
            if(handler.cmdline.find("sbin") != string::npos)
                handler.is_sbin = true;
            handler.atom = get_avg_atom(handler.pid);
            if(handler.ppid != -1){
                handler.sched = get_sched_stat(handler.pid);
            }
            handler._statm = get_statm(handler.pid);
            result.push_back(handler);
            cout << handler;
        }
    }
    return  result;
}

sched_stat get_sched_stat(int pid) {
    ifstream stat("/proc/" + to_string(pid) +"/stat");
    sched_stat res ={-1,-1,-1};
    if(stat.is_open()){

        stat.close();
    }
    return res;
}
