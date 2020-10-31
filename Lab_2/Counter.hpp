#ifndef CNT_H
#define CNT_H
#include <iostream>
#include <string>
#include "Locks.hpp"
#include "Barriers.hpp"

using std::string;

enum counter_type {

    lock_counter,
    barrier_counter

};

struct counter_args {
	
	Locks *lock;
	Barriers *barrier;
	int tid;
    int *cntr;
    int NUM_THREADS;
    int NUM_ITERATIONS;

};


class Counters
{
    private:
    counter_type counter;
    pthread_t* threads;
    int NUM_THREADS;
    int NUM_ITERATIONS;

    public:
        Counters(string counter_type, int number_of_threads, int number_of_iterations);
        ~Counters();
        void count(int *counter, string lockType, string barrierType);

};

#endif