#include <iostream>
#include <cmath>
#include <pthread.h>
#include <unistd.h>
#include <iomanip>
#include <sys/wait.h>

using namespace std;

pthread_mutex_t _mutex;

int completed_thread_count = 0;

struct ThreadData {
    int k;
    int n;
    int N;
    double func_val;
    int count;
};
 double r(double v, int p){
     return round(v*pow(10, p))/pow(10,p);
 }

void* thread_func(void* arg) {
    auto* data = (ThreadData*)arg;
    double sum = 0;
    double term = 1;
    for (int j = 1; j <= data->n; j++) {
        term *= -data->func_val * data->func_val / ((2 * j) * (2 * j - 1));
        sum += term;
    }
    pthread_mutex_lock(&_mutex);
    cout << std::fixed << "Thread " << gettid() << " calculated taylor series = " << sum << endl;
    completed_thread_count++;
    pthread_mutex_unlock(&_mutex);
    return nullptr;
}

int main(int argc, char** argv) {
    if (argc != 4) {
        cerr << "Undefined argument" << endl;
        return 1;
    }
    int k = atoi(argv[1]);
    int N = atoi(argv[2]);
    int n = atoi(argv[3]);
    double* y = new double[k];
    for (int i = 0; i < k; i++) {
        y[i] = sin(2 * M_PI*i/N);
        cout << "Val " << i + 1 << ": " << y[i] << endl;
    }


    pthread_t* threads = new pthread_t[k];
    for (int i = 0; i < k; i++) {
        ThreadData data = {k, n, N, y[i], 0};
        if (pthread_create(&threads[i], NULL, thread_func, &data) != 0) {
            cerr << "Failed to create thread " << i << endl;
            return 1;
        }
    }

    while (completed_thread_count !=k){
        usleep(10);
    }

    return 0;
}