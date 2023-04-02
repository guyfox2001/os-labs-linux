//
// Created by vattghern on 02.04.2023.
//
#include <iostream>
#include <iomanip>
using namespace std;

#ifndef LAB_1_TYPES_H
#define LAB_1_TYPES_H

struct avg_atom{
    double sum_exec;
    int nr_switches;
    friend istream& operator>>(istream& in, avg_atom& obj){
        in >> obj.sum_exec >> obj.nr_switches;
        return in;
    }


    friend ostream &operator<<(ostream &os, const avg_atom &atom) {
        if(atom.sum_exec && atom.nr_switches){
            os << std::fixed << atom.sum_exec / atom.nr_switches;
            return os;
        }

        if(atom.sum_exec ){
            os << std::fixed << atom.sum_exec;
            return os;
        }
        os << std::fixed << 0.0;
        return os;
    }
};
struct sched_stat{
    long utime, kernel_time;
    long double sleep_time;

    friend istream& operator>>(istream& in, sched_stat& obj){
        in >> obj.utime >> obj.kernel_time >> obj.sleep_time;
        return in;
    }

    friend ostream &operator<<(ostream &os, const sched_stat &stat) {
        os << '\t' << "utime:"       << std::string(10, ' ')  << stat.utime << '\n'
           << '\t' << "kernel_time:" << std::string(10, ' ')  << stat.kernel_time << '\n'
           << '\t' << "sleep_time:"  << std::string(10, ' ')  << stat.sleep_time << '\n';
        return os;
    }
};
struct statm{
    int size, resident, shared, trs, lrs, drs, dt;
    friend istream& operator>>(istream& in, statm& obj){
        in >> obj.size >> obj.size >> obj.resident >> obj.resident >> obj.shared >> obj.trs >> obj.lrs >> obj.drs >> obj.dt;
        return in;
    }

    friend ostream &operator<<(ostream &os, const statm &statm) {
        os  << '\t' << std::left << std::setw(10)  << "size:"      << std::setw(12) << statm.size << '\n'
            << '\t' << std::left << std::setw(10)  << "resident: " << std::setw(12) << statm.resident << '\n'
            << '\t' << std::left << std::setw(10)  << "shared: "   << std::setw(12) << statm.shared << '\n'
            << '\t' << std::left << std::setw(10)  << "trs: "      << std::setw(12) << statm.trs << '\n'
            << '\t' << std::left << std::setw(10)  << "lrs: "      << std::setw(12) << statm.lrs << '\n'
            << '\t' << std::left << std::setw(10)  << "drs: "      << std::setw(12) << statm.drs << '\n'
            << '\t' << std::left << std::setw(10)  << "dt: "       << std::setw(12) << statm.dt << '\n';
        return os;
    }
};
struct proc_handler{
    int pid, ppid;
    bool is_sbin;
    statm _statm;
    avg_atom atom;
    sched_stat sched;
    string cmdline;

    bool operator<(const proc_handler &rhs) const {
        return ppid < rhs.ppid;
    }

    bool operator>(const proc_handler &rhs) const {
        return rhs < *this;
    }

    bool operator<=(const proc_handler &rhs) const {
        return !(rhs < *this);
    }

    bool operator>=(const proc_handler &rhs) const {
        return !(*this < rhs);
    }

    friend ostream &operator<<(ostream &os, const proc_handler &handler) {
        os  << "\npid: "        << std::string(9, ' ') << handler.pid << '\n'
            << "ppid: "         << std::string(8, ' ') << handler.ppid << '\n'
            << "sbin: "         << std::string(8, ' ') << std::boolalpha << handler.is_sbin << '\n'
            << "cmdline: "      << std::string(5, ' ') << std::boolalpha << handler.cmdline << '\n'
            << "avg_atom: "     << std::string(4, ' ') << handler.atom << '\n'
            << "sched_stat:\n " << std::right << handler.sched << '\n'
            << "statm:\n"       << std::right << handler._statm;
        return os;
    }
};

#endif //LAB_1_TYPES_H
