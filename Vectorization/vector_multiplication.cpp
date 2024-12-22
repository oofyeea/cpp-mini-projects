#include <bits/stdc++.h>
#include <thread>
#include <chrono>
#include <random>
using namespace std;
using namespace std::chrono;

#define ll long long

void vector_addition(const vector<ll>& first, const vector<ll>& second, vector<ll>& result, int start, int end) {
    for (int i=start; i<end; i++)   {
        result[i] = first[i] * second[i];
    }
}

void print_first_10(vector<ll> first, vector<ll> second, vector<ll> result)  {
    cout << "First vector:  ";
    for (int i=0; i<10; i++)    {
        cout << first[i] << " ";
    }
    cout << endl;
    cout << "Second vector: ";
    for (int i=0; i<10; i++)    {
        if ((int) log10(second[i]) < (int) log10(first[i])) {
            for (int i=0; i<(int) log10(first[i])- (int) log10(second[i]); i++) {
                cout << " ";
            }
        }
        cout << second[i] << " ";
    }
    cout << endl;
    cout << "Result vector: ";
    for (int i=0; i<10; i++)    {
        cout << result[i] << " ";
    }
    cout << endl << endl;
}
int main()  {

    // SETTING UP VECTORS
    int num_values = 160000000;
    vector<ll> first, second, result(num_values);

    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(0, pow(2,8));

    for (int i=0; i<num_values; i++)  {
        first.push_back(dis(gen));
        second.push_back(dis(gen));
    }

    // IMPLEMENTATION OF VETORIZED ADDITION
    int num_threads = thread::hardware_concurrency();
    int thread_operations = num_values/num_threads;
    vector<thread> threads;

    // TIME BENCHMARK FOR VECTORIZATION
    auto vectorization_start = high_resolution_clock::now();
    for (int i=0; i<num_threads; i++)  {
        int start = i*thread_operations;
        int end = start + thread_operations;
        threads.emplace_back(vector_addition,cref(first),cref(second),ref(result),start,end);
    }

    for (auto &thread : threads)    {
        thread.join();
    }
    auto vectorization_stop = high_resolution_clock::now();
    auto vectorized_duration = duration_cast<milliseconds>(vectorization_stop - vectorization_start);

    cout << "Time taken for vectorization implementation: " << vectorized_duration.count() << " milliseconds " << endl;
    print_first_10(first,second,result);

    // TIME BENCHMARK OF REGULAR ADDITION
    auto regular_start = high_resolution_clock::now();

    for (int i=0; i<first.size(); i++)    {
        result[i] = first[i] * second[i];
    }

    auto regular_stop = high_resolution_clock::now();
    auto regular_duration = duration_cast<milliseconds>(regular_stop - regular_start);

    cout << "Time taken for non-vectorized implementation: " << regular_duration.count() << " milliseconds " << endl;
    print_first_10(first,second,result);

    /*
        NOTE: Difference exists mainly for >10000000 values in vector
    */
}